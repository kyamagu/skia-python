#include "common.h"
#include <include/codec/SkEncodedImageFormat.h>
#include <include/core/SkSamplingOptions.h>
#include <include/gpu/GrBackendSurface.h>
#include <include/gpu/GpuTypes.h>
#include <include/encode/SkJpegEncoder.h>
#include <include/encode/SkPngEncoder.h>
#include <include/encode/SkWebpEncoder.h>
#include <include/core/SkTextureCompressionType.h>
#include <src/core/SkMipmapBuilder.h>

#include <pybind11/numpy.h>
#include <pybind11/stl.h> // std::nullopt needs this.

namespace {

bool ImageReadPixels(
    const SkImage& image, GrDirectContext* context,
    const SkImageInfo& imageInfo, py::buffer dstPixels, size_t dstRowBytes,
    int srcX, int srcY, SkImage::CachingHint hint) {
    auto info = dstPixels.request(true);
    auto rowBytes = ValidateBufferToImageInfo(imageInfo, info, dstRowBytes);
    return image.readPixels(
        context, imageInfo, info.ptr, rowBytes, srcX, srcY, hint);
}

sk_sp<SkImage> ImageFromBuffer(
    py::buffer b, const SkISize dimensions, SkColorType ct, SkAlphaType at,
    const SkColorSpace* cs, bool copy) {
    auto info = b.request();
    auto imageInfo = SkImageInfo::Make(dimensions, ct, at, CloneColorSpace(cs));
    size_t rowBytes = ValidateBufferToImageInfo(imageInfo, info);
    size_t size = imageInfo.computeByteSize(rowBytes);
    auto data = (copy) ?
        SkData::MakeWithCopy(info.ptr, size) :
        SkData::MakeWithoutCopy(info.ptr, size);
    return SkImages::RasterFromData(imageInfo, data, rowBytes);
}

sk_sp<SkImage> ImageFromArray(py::array array, SkColorType ct, SkAlphaType at,
                            const SkColorSpace* cs, bool copy) {
    auto imageInfo = NumPyToImageInfo(array, ct, at, cs);
    size_t size = array.shape(0) * array.strides(0);
    auto data = (copy) ?
        SkData::MakeWithCopy(array.data(), size) :
        SkData::MakeWithoutCopy(array.data(), size);
    return SkImages::RasterFromData(imageInfo, data, array.strides(0));
}

std::unique_ptr<SkBitmap> ImageToBitmap(
    const SkImage& image, SkColorType ct, SkAlphaType at,
    const SkColorSpace* cs) {
    std::unique_ptr<SkBitmap> bitmap(new SkBitmap());
    if (!bitmap)
        throw std::bad_alloc();
    if (ct == kUnknown_SkColorType)
        ct = image.colorType();
    if (at == kUnknown_SkAlphaType)
        at = image.alphaType();
    auto imageInfo = SkImageInfo::Make(
        image.width(), image.height(), ct, at, CloneColorSpace(cs));
    if (!bitmap->tryAllocPixels(imageInfo))
        throw std::bad_alloc();
    if (!image.readPixels(bitmap->pixmap(), 0, 0))
        throw std::runtime_error("Failed to read pixels.");
    return bitmap;
}

sk_sp<SkImage> ImageOpen(py::object fp) {
    sk_sp<SkData> data(nullptr);
    if (hasattr(fp, "seek") && hasattr(fp, "read")) {
        fp.attr("seek")(0);
        auto buffer = fp.attr("read")().cast<py::buffer>();
        // TODO: Check maximum file size.
        auto info = buffer.request();
        size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
        data = SkData::MakeWithCopy(info.ptr, size);
        if (!data)
            throw std::bad_alloc();
    }
    else {
        auto path = fp.cast<std::string>();
        data = SkData::MakeFromFileName(path.c_str());
        if (!data)
            throw py::value_error(
                py::str("File not found: {}").format(path));
    }
    auto image = SkImages::DeferredFromEncodedData(data);
    if (!image)
        throw std::runtime_error("Failed to decode an image");
    return image;
}

void ImageSave(const SkImage& image, py::object fp,
               SkEncodedImageFormat format, int quality) {
    sk_sp<SkData> data = image.refEncodedData();
    switch (format) {
    case SkEncodedImageFormat::kWEBP:
        {
            SkWebpEncoder::Options options;
            if (quality < 100) {
                options.fCompression = SkWebpEncoder::Compression::kLossy;
                options.fQuality = quality;
            } else {
                options.fCompression = SkWebpEncoder::Compression::kLossless;
                // in lossless mode, this is effort. 70 is the default effort in SkImageEncoder,
                // which follows Blink and WebPConfigInit.
                options.fQuality = 70;
            }
            data = SkWebpEncoder::Encode(nullptr, &image, options);
        }
        break;

    case SkEncodedImageFormat::kJPEG:
        {
            SkJpegEncoder::Options options;
            options.fQuality = quality;
            data = SkJpegEncoder::Encode(nullptr, &image, options);
        }
        break;

    case SkEncodedImageFormat::kPNG:
    default:
        {
             SkPngEncoder::Options options; // Not used
             data = SkPngEncoder::Encode(nullptr, &image, {});
        }
        break;
    }
    auto decoded = SkImages::DeferredFromEncodedData(data);
    if (!data)
        throw std::runtime_error("Failed to encode an image.");
    if (hasattr(fp, "write"))
        fp.attr("write")(data);
    else {
        auto path = fp.cast<std::string>();
        SkFILEWStream stream(path.c_str());
        stream.write(data->data(), data->size());
    }
}

sk_sp<SkImage> ImageConvert(
    const SkImage& image, SkColorType ct, SkAlphaType at,
    const SkColorSpace* cs) {
    if (ct == kUnknown_SkColorType)
        ct = image.colorType();
    if (at == kUnknown_SkAlphaType)
        at = image.alphaType();
    if (at == image.alphaType()) {
        if (ct == image.colorType())
            return image.makeColorSpace(nullptr, CloneColorSpace(cs));
        return image.makeColorTypeAndColorSpace(nullptr, ct, CloneColorSpace(cs));
    }

    auto imageInfo = SkImageInfo::Make(
        image.width(), image.height(), ct, at, CloneColorSpace(cs));
    auto buffer = SkData::MakeUninitialized(imageInfo.computeMinByteSize());
    if (!buffer)
        throw std::bad_alloc();
    if (!image.readPixels(
        imageInfo, buffer->writable_data(), imageInfo.minRowBytes(), 0, 0))
        throw std::runtime_error("Failed to convert pixels.");
    return SkImages::RasterFromData(imageInfo, buffer, imageInfo.minRowBytes());
}

sk_sp<SkImage> ImageResize(
    const SkImage& image, int width, int height, SkSamplingOptions& samplingOptions,
    SkImage::CachingHint cachingHint) {
    auto imageInfo = image.imageInfo().makeWH(width, height);
    auto buffer = SkData::MakeUninitialized(imageInfo.computeMinByteSize());
    if (!buffer)
        throw std::bad_alloc();
    auto pixmap = SkPixmap(
        imageInfo, buffer->writable_data(), imageInfo.minRowBytes());
    if (!image.scalePixels(pixmap, samplingOptions, cachingHint))
        throw std::runtime_error("Failed to resize image.");
    return SkImages::RasterFromData(imageInfo, buffer, imageInfo.minRowBytes());
}

}  // namespace

void initImage(py::module &m) {
py::enum_<skgpu::Budgeted>(m, "Budgeted", R"docstring(
    Indicates whether an allocation should count against a cache budget.
    )docstring")
    .value("kNo", skgpu::Budgeted::kNo)
    .value("kYes", skgpu::Budgeted::kYes)
    .export_values();

/*
py::enum_<SkFilterQuality>(m, "FilterQuality",
    R"docstring(
    Controls how much filtering to be done when scaling/transforming complex
    colors e.g. image.
    )docstring")
    .value("kNone_FilterQuality", SkFilterQuality::kNone_SkFilterQuality,
        "fastest but lowest quality, typically nearest-neighbor")
    .value("kLow_FilterQuality", SkFilterQuality::kLow_SkFilterQuality,
        "typically bilerp")
    .value("kMedium_FilterQuality", SkFilterQuality::kMedium_SkFilterQuality,
        "typically bilerp + mipmaps for down-scaling")
    .value("kHigh_FilterQuality", SkFilterQuality::kHigh_SkFilterQuality,
        "slowest but highest quality, typically bicubic or better")
    .value("kLast_FilterQuality", SkFilterQuality::kLast_SkFilterQuality)
    .export_values();
*/

py::enum_<SkEncodedImageFormat>(m, "EncodedImageFormat", R"docstring(
    Enum describing format of encoded data.
    )docstring")
    .value("kBMP", SkEncodedImageFormat::kBMP)
    .value("kGIF", SkEncodedImageFormat::kGIF)
    .value("kICO", SkEncodedImageFormat::kICO)
    .value("kJPEG", SkEncodedImageFormat::kJPEG)
    .value("kPNG", SkEncodedImageFormat::kPNG)
    .value("kWBMP", SkEncodedImageFormat::kWBMP)
    .value("kWEBP", SkEncodedImageFormat::kWEBP)
    .value("kPKM", SkEncodedImageFormat::kPKM)
    .value("kKTX", SkEncodedImageFormat::kKTX)
    .value("kASTC", SkEncodedImageFormat::kASTC)
    .value("kDNG", SkEncodedImageFormat::kDNG)
    .value("kHEIF", SkEncodedImageFormat::kHEIF)
    .export_values();


py::class_<SkMipmapBuilder>(m, "MipmapBuilder")
    .def(py::init<const SkImageInfo&>())
    .def("countLevels", &SkMipmapBuilder::countLevels)
    .def("level", &SkMipmapBuilder::level)
    .def("attachTo",
        py::overload_cast<sk_sp<const SkImage>>(&SkMipmapBuilder::attachTo),
        R"docstring(
        If these levels are compatible with src, return a new Image that
        combines src's base level with these levels as mip levels.

        If not compatible, this returns nullptr.
        )docstring")
    ;


py::class_<SkImage, sk_sp<SkImage>, SkRefCnt> image(m, "Image",
    R"docstring(
    :py:class:`Image` describes a two dimensional array of pixels to draw.

    The pixels may be decoded in a raster bitmap, encoded in a
    :py:class:`Picture` or compressed data stream, or located in GPU memory as a
    GPU texture.

    :py:class:`Image` cannot be modified after it is created. :py:class:`Image`
    may allocate additional storage as needed; for instance, an encoded
    :py:class:`Image` may decode when drawn.

    :py:class:`Image` width and height are greater than zero. Creating an
    :py:class:`Image` with zero width or height returns :py:class:`Image` equal
    to nullptr.

    :py:class:`Image` may be created from :py:class:`Bitmap`,
    :py:class:`Pixmap`, :py:class:`Surface`, :py:class:`Picture`, encoded
    streams, GPU texture, YUV_ColorSpace data, or hardware buffer. Encoded
    streams supported include BMP, GIF, HEIF, ICO, JPEG, PNG, WBMP, WebP.
    Supported encoding details vary with platform.

    ``skia-python`` supports a few high-level methods in addition to C++ API::

        image = skia.Image.open('/path/to/image.png')
        image = image.resize(120, 120)
        image = image.convert(alphaType=skia.kUnpremul_AlphaType)
        image.save('/path/to/output.jpg', skia.kJPEG)

    NumPy arrays can be directly imported or exported::

        image = skia.Image.fromarray(array)
        array = image.toarray()

    General pixel buffers can be exchanged in the following approach::

        image = skia.Image.frombytes(pixels, (100, 100))
        pixels = image.tobytes()

    )docstring",
    py::buffer_protocol());

py::enum_<SkTextureCompressionType>(image, "CompressionType")
    .value("kNone", SkTextureCompressionType::kNone)
    .value("kETC2_RGB8_UNORM", SkTextureCompressionType::kETC2_RGB8_UNORM)
    .value("kBC1_RGB8_UNORM", SkTextureCompressionType::kBC1_RGB8_UNORM)
    .value("kBC1_RGBA8_UNORM", SkTextureCompressionType::kBC1_RGBA8_UNORM)
    .value("kLast", SkTextureCompressionType::kLast)
    .export_values();

py::enum_<SkImages::BitDepth>(image, "BitDepth")
    .value("kU8", SkImages::BitDepth::kU8)
    .value("kF16", SkImages::BitDepth::kF16)
    .export_values();

py::enum_<SkImage::CachingHint>(image, "CachingHint")
    .value("kAllow_CachingHint", SkImage::CachingHint::kAllow_CachingHint)
    .value("kDisallow_CachingHint", SkImage::CachingHint::kDisallow_CachingHint)
    .export_values();

py::enum_<SkImage::LegacyBitmapMode>(image, "LegacyBitmapMode")
    .value("kRO_LegacyBitmapMode", SkImage::kRO_LegacyBitmapMode,
        R"docstring(
        returned bitmap is read-only and immutable
        )docstring")
    .export_values();

image
    // Python methods.
    .def_buffer([] (const SkImage& image) {
        SkPixmap pixmap;
        if (!image.peekPixels(&pixmap))
            throw std::runtime_error(
                "Image is not raster, call makeRasterImage().");
        return ImageInfoToBufferInfo(
            pixmap.info(), pixmap.writable_addr(), pixmap.rowBytes());
    })
    .def_property_readonly("__array_interface__",
        [] (const SkImage& image) {
            SkPixmap pixmap;
            if (image.peekPixels(&pixmap))
                return ImageInfoToArrayInterface(
                    pixmap.info(), pixmap.rowBytes());
            else {
                auto imageInfo = image.imageInfo();
                return ImageInfoToArrayInterface(
                    imageInfo, imageInfo.minRowBytes());
            }
        })
    .def("tobytes",
        [] (const SkImage& image) -> py::object {
            SkPixmap pixmap;
            if (image.peekPixels(&pixmap))
                return py::module::import("builtins").attr("bytes")(pixmap);
            else {
                auto imageInfo = image.imageInfo();
                py::bytes bytes(nullptr, imageInfo.computeMinByteSize());
                void* ptr = reinterpret_cast<void*>(
                    PyBytes_AS_STRING(bytes.ptr()));
                if (!image.readPixels(
                    imageInfo, ptr, imageInfo.minRowBytes(), 0, 0))
                    throw std::runtime_error("Failed to read pixels.");
                return std::move(bytes);
            }
        },
        R"docstring(
        Creates python `bytes` object from internal pixels.

        When the image is raster, the returned bytes share the internal buffer.
        Otherwise, pixels are copied to a newly allocated python `bytes`.
        )docstring")
    .def_static("frombytes", &ImageFromBuffer,
        R"docstring(
        Creates a new :py:class:`Image` from bytes.

        :param pixels: raw bytes of pixels
        :param skia.ISize dimensions: (width, height) tuple
        :param skia.ColorType colorType: color type of the array
        :param skia.AlphaType alphaType: alpha type of the array
        :param skia.ColorSpace colorSpace: range of colors; may be nullptr
        :param bool copy: Whether to copy pixels. When false is specified,
            :py:class:`Image` shares the pixel buffer without copy.
        )docstring",
        py::arg("array"), py::arg("dimensions"),
        py::arg("colorType") = kN32_SkColorType,
        py::arg("alphaType") = kUnpremul_SkAlphaType,
        py::arg("colorSpace") = nullptr, py::arg("copy") = true)
    .def_static("fromarray", &ImageFromArray,
        R"docstring(
        Creates a new :py:class:`Image` from numpy array.

        :param numpy.ndarray array: numpy ndarray of shape=(height, width,
            channels) and appropriate dtype. Must have non-zero width and
            height, and the valid number of channels for the specified color
            type.
        :param skia.ColorType colorType: color type of the array
        :param skia.AlphaType alphaType: alpha type of the array
        :param skia.ColorSpace colorSpace: range of colors; may be nullptr
        :param bool copy: Whether to copy pixels.
        )docstring",
        py::arg("array"), py::arg("colorType") = kN32_SkColorType,
        py::arg("alphaType") = kUnpremul_SkAlphaType,
        py::arg("colorSpace") = nullptr, py::arg("copy") = true)
    .def("toarray", &ReadToNumpy<SkImage>,
        R"docstring(
        Exports a ``numpy.ndarray``.

        :param srcX: offset into readable pixels on x-axis; may be negative
        :param srcY: offset into readable pixels on y-axis; may be negative
        :param colorType: target :py:class:`ColorType`
        :param alphaType: target :py:class:`AlphaType`
        :param colorSpace: target :py:class:`ColorSpace`
        :return: numpy.ndarray
        )docstring",
        py::arg("srcX") = 0, py::arg("srcY") = 0,
        py::arg("colorType") = kUnknown_SkColorType,
        py::arg("alphaType") = kUnpremul_SkAlphaType,
        py::arg("colorSpace") = nullptr)
    .def_static("open", &ImageOpen,
        R"docstring(
        Creates :py:attr:`Image` from file path or file-like object.

        Shortcut for the following::

            if hasattr(fp, 'read') and hasattr(fp, 'seek'):
                fp.seek(0)
                data = skia.Data.MakeWithCopy(fp.read())
            else:
                data = skia.Data.MakeFromFileName(fp)
            image = skia.Image.MakeFromEncoded(data)

        :param fp: file path or file-like object that has `seek` and `read`
            method. file must be opened in binary mode.
        )docstring",
        py::arg("fp"))
    .def("save", &ImageSave,
        R"docstring(
        Saves :py:attr:`Image` to file path or file-like object.

        Shortcut for the following::

            data = image.encodeToData(encodedImageFormat, quality)
            if hasattr(fp, 'write'):
                fp.write(data)
            else:
                with open(fp, 'wb') as f:
                    f.write(data)

        :param fp: file path or file-like object that has `write` method. file
            must be opened in writable binary mode.
        :param skia.EncodedImageFormat encodedImageFormat:
            one of: :py:attr:`~EncodedImageFormat.kJPEG`,
            :py:attr:`~EncodedImageFormat.kPNG`,
            :py:attr:`~EncodedImageFormat.kWEBP`
        :param int quality: encoder specific metric with 100 equaling best
        )docstring",
        py::arg("fp"),
        py::arg("encodedImageFormat") = SkEncodedImageFormat::kPNG,
        py::arg("quality") = 100)
    .def("bitmap", &ImageToBitmap,
        R"docstring(
        Creates :py:class:`Bitmap` from :py:class:`Image`.

        Pixels are always allocated and copied.

        :param colorType: color type of :py:class:`Bitmap`. If
            :py:attr:`~skia.kUnknown_ColorType`, uses the same colorType as
            :py:class:`Image`.
        :param alphaType: alpha type of :py:class:`Bitmap`. If
            :py:attr:`~skia.kUnknown_AlphaType`, uses the same alphaType as
            :py:class:`Image`.
        :param colorSpace: color space of :py:class:`Bitmap`.
        :return: :py:class:`Bitmap`
        )docstring",
        py::arg("colorType") = kUnknown_SkColorType,
        py::arg("alphaType") = kUnknown_SkAlphaType,
        py::arg("colorSpace") = nullptr)
    .def("convert", &ImageConvert,
        R"docstring(
        Creates :py:class:`Image` in target :py:class:`ColorType`,
        :py:class:`AlphatType`, and :py:class:`ColorSpace`. Raises if
        :py:class:`Image` could not be created.

        Pixels are converted only if pixel conversion is possible. If
        :py:class:`Image` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, or
        :py:attr:`~ColorType.kAlpha_8_ColorType`; colorType must
        be the same. If :py:class:`Image` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, colorSpace must
        be the same. If :py:class:`Image` :py:class:`AlphaType` is
        :py:attr:`~AlphaType.kOpaque_AlphaType`, alphaType must be the same.
        If :py:class:`Image` :py:class:`ColorSpace` is nullptr,
        colorSpace must be the same. Raises if pixel conversion is
        not possible.

        :param colorType: target color type. If
            :py:attr:`~skia.kUnknown_ColorType` is given, uses the same
            colorType as :py:class:`Image`.
        :param alphaType: target alpha type. If
            :py:attr:`~skia.kUnknown_AlphaType` is given, uses the same
            alphaType as :py:class:`Image`.
        :param colorSpace: target color space.
        :return: :py:class:`Image`
        )docstring",
        py::arg("colorType") = kUnknown_SkColorType,
        py::arg("alphaType") = kUnknown_SkAlphaType,
        py::arg("colorSpace") = nullptr)
    .def("resize", &ImageResize,
        R"docstring(
        Creates :py:class:`Image` by scaling pixels to fit width and height.
        Raises if :py:class:`Image` could not be scaled.

        Scales the image, with filterQuality, to match width and height.
        filterQuality :py:attr:`~FilterQuality.None_FilterQuality` is fastest,
        typically implemented with nearest neighbor filter.
        :py:attr:`~FilterQuality.kLow_FilterQuality` is typically implemented
        with bilerp filter. :py:attr:`~FilterQuality.kMedium_FilterQuality` is
        typically implemented with bilerp filter, and mip-map filter when size
        is reduced. :py:attr:`~FilterQuality.kHigh_FilterQuality` is slowest,
        typically implemented with bicubic filter.

        If cachingHint is :py:attr:`~Image.CachingHint.kAllow_CachingHint`,
        pixels may be retained locally. If cachingHint is
        :py:attr:`~Image.CachingHint.kDisallow_CachingHint`, pixels are not
        added to the local cache.

        :param int width: target width
        :param int height: target height
        :param skia.SamplingOptions options: sampling options
        :param skia.Image.CachingHint cachingHint: Caching hint
        :return: :py:class:`Image`
        )docstring",
        py::arg("width"), py::arg("height"),
        py::arg("options") = SkSamplingOptions(),
        py::arg("cachingHint") = SkImage::kAllow_CachingHint)
    .def("__repr__",
        [] (const SkImage& image) {
            return py::str("Image({}, {}, {}, {})").format(
                image.width(), image.height(), image.colorType(),
                image.alphaType());
        })
    .def("_repr_png_",
        [] (const SkImage& image) {
            auto data = SkPngEncoder::Encode(nullptr, &image, {});
            if (!data)
                throw std::runtime_error("Failed to encode an image.");
            return py::bytes(
                static_cast<const char*>(data->data()), data->size());
        })

    // C++ wrappers.
    .def_static("MakeRasterCopy", &SkImages::RasterFromPixmapCopy,
        R"docstring(
        Creates :py:class:`Image` from :py:class:`Pixmap` and copy of pixels.

        Since pixels are copied, :py:class:`Pixmap` pixels may be modified or
        deleted without affecting :py:class:`Image`.

        :py:class:`Image` is returned if :py:class:`Pixmap` is valid. Valid
        :py:class:`Pixmap` parameters include: dimensions are greater than zero;
        each dimension fits in 29 bits; :py:class:`ColorType` and
        :py:class:`AlphaType` are valid, and :py:class:`ColorType` is not
        :py:attr:`~ColorType.kUnknown_ColorType`; row bytes are large enough to
        hold one row of pixels; pixel address is not nullptr.

        :param skia.Pixmap pixmap: :py:class:`ImageInfo`, pixel address, and row
            bytes
        :return: copy of :py:class:`Pixmap` pixels, or nullptr
        )docstring",
        py::arg("pixmap"))
    .def_static("MakeRasterData",
        [] (const SkImageInfo& imageInfo, py::buffer b, size_t dstRowBytes) {
            auto info = b.request();
            auto rowBytes = ValidateBufferToImageInfo(
                imageInfo, info, dstRowBytes);
            auto data = SkData::MakeWithoutCopy(
                info.ptr, info.strides[0] * info.shape[0]);
            return SkImages::RasterFromData(imageInfo, data, rowBytes);
        },
        R"docstring(
        Creates :py:class:`Image` from :py:class:`ImageInfo`, sharing pixels.

        :py:class:`Image` is returned if :py:class:`ImageInfo` is valid. Valid
        :py:class:`ImageInfo` parameters include: dimensions are greater than
        zero; each dimension fits in 29 bits; :py:class:`ColorType` and
        :py:class:`AlphaType` are valid, and :py:class:`ColorType` is not
        :py:attr:`~ColorType.kUnknown_ColorType`; rowBytes are large enough to
        hold one row of pixels; pixels is not nullptr, and contains enough data
        for :py:class:`Image`.

        :param skia.ImageInfo info: contains width, height,
            :py:class:`AlphaType`, :py:class:`ColorType`, :py:class:`ColorSpace`
        :param Union[bytes,bytearray,memoryview] pixels: pixel storage
        :param int rowBytes: size of pixel row or larger
        :return: :py:class:`Image` sharing pixels, or nullptr
        )docstring",
        py::arg("info"), py::arg("pixels").none(false), py::arg("rowBytes"))
    .def_static("MakeFromRaster",
        [] (const SkPixmap& pixmap) {
            return SkImages::RasterFromPixmap(pixmap, nullptr, nullptr);
        },
        R"docstring(
        Creates :py:class:`Image` from pixmap, sharing :py:class:`Pixmap`
        pixels.

        :py:class:`Image` is returned if pixmap is valid. Valid
        :py:class:`Pixmap` parameters include: dimensions are greater than zero;
        each dimension fits in 29 bits; :py:class:`ColorType` and
        :py:class:`AlphaType` are valid, and :py:class:`ColorType` is not
        :py:attr:`kUnknown_ColorType`; row bytes are large enough to hold one
        row of pixels; pixel address is not nullptr.

        :param skia.Pixmap pixmap: :py:class:`ImageInfo`, pixel address, and row
            bytes
        :return: :py:class:`Image` sharing pixmap
        )docstring",
        py::arg("pixmap").none(false))
    .def_static("MakeFromBitmap", &SkImages::RasterFromBitmap,
        R"docstring(
        Creates :py:class:`Image` from bitmap, sharing or copying bitmap pixels.

        If the bitmap is marked immutable, and its pixel memory is shareable, it
        may be shared instead of copied.

        :py:class:`Image` is returned if bitmap is valid. Valid
        :py:class:`Bitmap` parameters include: dimensions are greater than zero;
        each dimension fits in 29 bits; :py:class:`ColorType` and
        :py:class:`AlphaType` are valid, and :py:class:`ColorType` is not
        :py:attr:`kUnknown_ColorType`; row bytes are large enough to hold one
        row of pixels; pixel address is not nullptr.

        :param skia.Bitmap bitmap: :py:class:`ImageInfo`, row bytes, and pixels
        :return: created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("bitmap"))
    // .def_static("MakeFromGenerator", &SkImages::DeferredFromGenerator,
    //     "Creates SkImage from data returned by imageGenerator.")
    .def_static("MakeFromEncoded", &SkImages::DeferredFromEncodedData,
        R"docstring(
        Return an image backed by the encoded data, but attempt to defer
        decoding until the image is actually used/drawn.

        This deferral allows the system to cache the result, either on the CPU
        or on the GPU, depending on where the image is drawn. If memory is low,
        the cache may be purged, causing the next draw of the image to have to
        re-decode.

        If the encoded format is not supported, nullptr is returned.

        :param encoded: the encoded data
        :return: created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("encoded"), py::arg("alphaType") = std::nullopt)
/*
    .def_static("MakeTextureFromCompressed",
        &SkImages::TextureFromCompressedTextureData,
        R"docstring(
        Creates a GPU-backed :py:class:`Image` from compressed data.

        This method will return an :py:class:`Image` representing the compressed
        data. If the GPU doesn't support the specified compression method, the
        data will be decompressed and then wrapped in a GPU-backed image.

        Note: one can query the supported compression formats via
        :py:meth:`GrContext.compressedBackendFormat`.

        :param skia.GrDirectContext context: GPU context
        :param skia.Data data: compressed data to store in :py:class:`Image`
        :param int width: width of full :py:class:`Image`
        :param int height: height of full :py:class:`Image`
        :param skia.CompressionType type: type of compression used
        :param skia.GrMipmapped mipMapped: does 'data' contain data for all the
            mipmap levels?
        :param skia.GrProtected isProtected: do the contents of 'data' require
            DRM protection (on Vulkan)?
        :return: created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("data"), py::arg("width"),
        py::arg("height"), py::arg("type"),
        py::arg("mipMapped") = GrMipmapped::kNo,
        py::arg("isProtected") = GrProtected::kNo)
    .def_static("MakeRasterFromCompressed", &SkImages::RasterFromCompressedTextureData,
        R"docstring(
        Creates a CPU-backed :py:class:`Image` from compressed data.

        This method will decompress the compressed data and create an image
        wrapping it. Any mipmap levels present in the compressed data are
        discarded.

        :param skia.Data data: compressed data to store in SkImage
        :param int width: width of full SkImage
        :param int height: height of full SkImage
        :param skia.Image.CompressionType type: type of compression used
        :return: created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("data"), py::arg("width"), py::arg("height"), py::arg("type"))
    .def_static("MakeFromTexture",
        [] (GrRecordingContext* context, const GrBackendTexture& texture,
            GrSurfaceOrigin origin, SkColorType colorType,
            SkAlphaType alphaType, const SkColorSpace* cs) {
            return SkImages::BorrowTextureFrom(
                context, texture, origin, colorType, alphaType,
                CloneColorSpace(cs));
        },
        R"docstring(
        Creates :py:class:`Image` from GPU texture associated with context.

        Caller is responsible for managing the lifetime of GPU texture.

        :py:class:`Image` is returned if format of backendTexture is recognized
        and supported. Recognized formats vary by GPU back-end.

        :param skia.GrRecordingContext context: GPU context
        :param skia.GrBackendTexture backendTexture: texture residing on GPU
        :param skia.ColorSpace colorSpace: range of colors; may be nullptr
        :return: created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("texture"), py::arg("origin"),
        py::arg("colorType"), py::arg("alphaType"),
        py::arg("colorSpace") = nullptr)
    .def_static("MakeFromCompressedTexture",
        [] (GrRecordingContext* context, const GrBackendTexture& texture,
            GrSurfaceOrigin origin, SkAlphaType alphaType,
            const SkColorSpace* cs) {
            return SkImages::TextureFromCompressedTexture(
                context, texture, origin, alphaType, CloneColorSpace(cs),
                nullptr, nullptr);
        },
        R"docstring(
        Creates an :py:class:`Image` from a GPU backend texture.

        An :py:class:`Image` is returned if the format of backendTexture is
        recognized and supported. Recognized formats vary by GPU back-end.

        :param skia.GrRecordingContext context: the GPU context
        :param skia.GrBackendTexture backendTexture: a texture already allocated
            by the GPU
        :param skia.AlphaType alphaType: This characterizes the nature of the
            alpha values in the backend texture. For opaque compressed formats
            (e.g., ETC1) this should usually be set to
            :py:attr:`~AlphaType.kOpaque_AlphaType`.
        :param skia.ColorSpace colorSpace: This describes the color space of
            this image's contents, as seen after sampling. In general, if the
            format of the backend texture is SRGB, some linear colorSpace should
            be supplied (e.g., :py:meth:`ColorSpace.MakeSRGBLinear` ). If the
            format of the backend texture is linear, then the colorSpace should
            include a description of the transfer function as well (e.g.,
            :py:meth:`ColorSpace.MakeSRGB` ).
        :return: created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("texture"), py::arg("origin"),
        py::arg("alphaType"), py::arg("colorSpace") = nullptr)
    .def_static("MakeCrossContextFromPixmap",
        &SkImages::CrossContextTextureFromPixmap,
        R"docstring(
        Creates :py:class:`Image` from pixmap.

        :py:class:`Image` is uploaded to GPU back-end using context.

        Created :py:class:`Image` is available to other GPU contexts, and is
        available across thread boundaries. All contexts must be in the same GPU
        share group, or otherwise share resources.

        When :py:class:`Image` is no longer referenced, context releases texture
        memory asynchronously.

        :py:class:`GrBackendTexture` created from pixmap is uploaded to match
        :py:class:`Surface` created with dstColorSpace. :py:class:`ColorSpace`
        of :py:class:`Image` is determined by pixmap.colorSpace().

        :py:class:`Image` is returned referring to GPU back-end if context is
        not nullptr, format of data is recognized and supported, and if context
        supports moving resources between contexts. Otherwise, pixmap pixel data
        is copied and :py:class:`Image` as returned in raster format if
        possible; nullptr may be returned. Recognized GPU formats vary by
        platform and GPU back-end.

        :param skia.GrDirectContext context: GPU context
        :param skia.Pixmap pixmap: :py:class:`ImageInfo`, pixel address, and row
            bytes
        :param bool buildMips: create :py:class:`Image` as mip map if true
        :param bool limitToMaxTextureSize: downscale image to GPU maximum
            texture size, if necessary
        :return: created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("pixmap"), py::arg("buildMips"),
        py::arg("limitToMaxTextureSize") = false)
    .def_static("MakeFromAdoptedTexture",
        [] (GrRecordingContext* context, const GrBackendTexture& backendTexture,
            GrSurfaceOrigin origin, SkColorType colorType,
            SkAlphaType alphaType, const SkColorSpace* colorSpace) {
            return SkImages::AdoptTextureFrom(
                context, backendTexture, origin, colorType, alphaType,
                CloneColorSpace(colorSpace));
        },
        R"docstring(
        Creates :py:class:`Image` from backendTexture associated with context.

        backendTexture and returned :py:class:`Image` are managed internally,
        and are released when no longer needed.

        :py:class:`Image` is returned if format of backendTexture is recognized
        and supported. Recognized formats vary by GPU back-end.

        :param skia.GrContext context: GPU context
        :param skia.GrBackendTexture backendTexture: texture residing on GPU
        :param skia.ColorSpace colorSpace: range of colors; may be nullptr
        :return: created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("backendTexture"), py::arg("origin"),
        py::arg("colorType"),
        py::arg("alphaType") = SkAlphaType::kPremul_SkAlphaType,
        py::arg("colorSpace") = nullptr)
    .def_static("MakeFromYUVATexturesCopy",
        [] (GrRecordingContext* context,
            SkYUVColorSpace yuvColorSpace,
            const std::vector<GrBackendTexture>& yuvaTextures,
            const std::vector<SkYUVAIndex>& yuvaIndices,
            SkISize imageSize,
            GrSurfaceOrigin imageOrigin,
            const SkColorSpace* imageColorSpace) {
            if (yuvaIndices.size() != 4)
                throw py::value_error("yuvaIndices must have 4 elements.");
            return SkImages::TextureFromYUVATexturesCopy(
                context, yuvColorSpace, yuvaTextures.data(), yuvaIndices.data(),
                imageSize, imageOrigin, CloneColorSpace(imageColorSpace));
        },
        R"docstring(
        Creates an :py:class:`Image` by flattening the specified YUVA planes
        into a single, interleaved RGBA image.

        :param context:         GPU context
        :param yuvColorSpace:   How the YUV values are converted to RGB
        :param yuvaTextures:    array of (up to four) YUVA textures on GPU which
            contain the, possibly interleaved, YUVA planes
        :param yuvaIndices:     array indicating which texture in yuvaTextures,
            and channel in that texture, maps to each component of YUVA.
        :param imageSize:       size of the resulting image
        :param imageOrigin:     origin of the resulting image.
        :param imageColorSpace: range of colors of the resulting image; may be
            nullptr
        :return:                created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("yuvColorSpace"), py::arg("yuvaTextures"),
        py::arg("yuvaIndices"), py::arg("imageSize"), py::arg("imageOrigin"),
        py::arg("imageColorSpace") = nullptr)
    .def_static("MakeFromYUVATexturesCopyWithExternalBackend",
        [] (GrRecordingContext* context,
            SkYUVColorSpace yuvColorSpace,
            const std::vector<GrBackendTexture>& yuvaTextures,
            const std::vector<SkYUVAIndex>& yuvaIndices,
            SkISize imageSize,
            GrSurfaceOrigin imageOrigin,
            const GrBackendTexture& backendTexture,
            const SkColorSpace* imageColorSpace) {
            if (yuvaIndices.size() != 4)
                throw py::value_error("yuvaIndices must have 4 elements.");
            return SkImages::TextureFromYUVATexturesCopyWithExternalBackend(
                context, yuvColorSpace, yuvaTextures.data(), yuvaIndices.data(),
                imageSize, imageOrigin, backendTexture,
                CloneColorSpace(imageColorSpace), nullptr, nullptr);
        },
        R"docstring(
        Creates an :py:class:`Image` by flattening the specified YUVA planes
        into a single, interleaved RGBA image. 'backendTexture' is used to store
        the result of the flattening.

        :param context:         GPU context
        :param yuvColorSpace:   How the YUV values are converted to RGB
        :param yuvaTextures:    array of (up to four) YUVA textures on GPU which
            contain the, possibly interleaved, YUVA planes
        :param yuvaIndices:     array indicating which texture in yuvaTextures,
            and channel in that texture, maps to each component of YUVA.
        :param imageSize:       size of the resulting image
        :param imageOrigin:     origin of the resulting image.
        :param backendTexture:  the resource that stores the final pixels
        :param imageColorSpace: range of colors of the resulting image; may be
            nullptr
        :return:                created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("yuvColorSpace"), py::arg("yuvaTextures"),
        py::arg("yuvaIndices"), py::arg("imageSize"), py::arg("imageOrigin"),
        py::arg("backendTexture"), py::arg("imageColorSpace") = nullptr)
    .def_static("MakeFromYUVATextures",
        [] (GrDirectContext* context,
            SkYUVColorSpace yuvColorSpace,
            const std::vector<GrBackendTexture>& yuvaTextures,
            const std::vector<SkYUVAIndex>& yuvaIndices,
            SkISize imageSize,
            GrSurfaceOrigin imageOrigin,
            const SkColorSpace* imageColorSpace) {
            if (yuvaIndices.size() != 4)
                throw py::value_error("yuvaIndices must have 4 elements.");
            return SkImages::TextureFromYUVATextures(
                context, yuvColorSpace, yuvaTextures.data(), yuvaIndices.data(),
                imageSize, imageOrigin, CloneColorSpace(imageColorSpace));
        },
        R"docstring(
        Creates an :py:class:`Image` by flattening the specified YUVA planes
        into an image, to be rendered via multitexturing.

        When all the provided backend textures can be released
        'textureReleaseProc' will be called with 'releaseContext'. It will be
        called even if this method fails.

        :param context:         GPU context
        :param yuvColorSpace:   How the YUV values are converted to RGB
        :param yuvaTextures:    array of (up to four) YUVA textures on GPU which
            contain the, possibly interleaved, YUVA planes
        :param yuvaIndices:     array indicating which texture in yuvaTextures,
            and channel in that texture, maps to each component of YUVA.
        :param imageSize:       size of the resulting image
        :param imageOrigin:     origin of the resulting image.
        :param imageColorSpace: range of colors of the resulting image; may be
            nullptr
        :return:                created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("yuvColorSpace"), py::arg("yuvaTextures"),
        py::arg("yuvaIndices"), py::arg("imageSize"), py::arg("imageOrigin"),
        py::arg("imageColorSpace") = nullptr)
    .def_static("MakeFromYUVAPixmaps",
        [] (GrRecordingContext* context,
            SkYUVColorSpace yuvColorSpace,
            const std::vector<SkPixmap>& yuvaPixmaps,
            const std::vector<SkYUVAIndex>& yuvaIndices,
            SkISize imageSize,
            GrSurfaceOrigin imageOrigin,
            bool buildMips,
            bool limitToMaxTextureSize,
            const SkColorSpace* imageColorSpace) {
            if (yuvaIndices.size() != 4)
                throw py::value_error("yuvaIndices must have 4 elements.");
            return SkImages::TextureFromYUVAPixmaps(
                context, yuvColorSpace, yuvaPixmaps.data(), yuvaIndices.data(),
                imageSize, imageOrigin, buildMips, limitToMaxTextureSize,
                CloneColorSpace(imageColorSpace));
        },
        R"docstring(
        Creates :py:class:`Image` from pixmap array representing YUVA data.
        :py:class:`Image` is uploaded to GPU back-end using context.

        Each GrBackendTexture created from yuvaPixmaps array is uploaded to
        match :py:class:`Surface` using :py:class:`ColorSpace` of
        :py:class:`Pixmap`. :py:class:`ColorSpace` of :py:class:`Image` is
        determined by imageColorSpace.

        :py:class:`Image` is returned referring to GPU back-end if context is
        not nullptr and format of data is recognized and supported. Otherwise,
        nullptr is returned. Recognized GPU formats vary by platform and GPU
        back-end.

        :param context:               GPU context
        :param yuvColorSpace:         How the YUV values are converted to RGB
        :param yuvaPixmaps:           array of (up to four) :py:class:`Pixmap`
                                      which contain the, possibly interleaved,
                                      YUVA planes
        :param yuvaIndices:           array indicating which pixmap in
                                      yuvaPixmaps, and channel in that pixmap,
                                      maps to each component of YUVA.
        :param imageSize:             size of the resulting image
        :param imageOrigin:           origin of the resulting image.
        :param buildMips:             create internal YUVA textures as mip map
                                      if true
        :param limitToMaxTextureSize: downscale image to GPU maximum texture
                                      size, if necessary
        :param imageColorSpace:       range of colors of the resulting image;
                                      may be nullptr
        :return:                      created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("yuvaPixmaps"), py::arg("yuvaTextures"),
        py::arg("yuvaIndices"), py::arg("imageSize"), py::arg("imageOrigin"),
        py::arg("buildMips"), py::arg("limitToMaxTextureSize") = false,
        py::arg("imageColorSpace") = nullptr)
    .def_static("MakeFromYUVAPixmaps",
        [] (GrRecordingContext* context,
            const SkYUVAPixmaps& pixmaps,
            GrMipMapped buildMips,
            bool limitToMaxTextureSize,
            const SkColorSpace* imageColorSpace) {
            return SkImages::TextureFromYUVAPixmaps(
                context, pixmaps, buildMips, limitToMaxTextureSize,
                CloneColorSpace(imageColorSpace));
        },
        R"docstring(
        Creates :py:class:`Image` from :py:class:`YUVAPixmaps`.

        The image will remain planar with each plane converted to a texture
        using the passed :py:class:`GrRecordingContext`.

        :py:class:`YUVAPixmaps` has a :py:class:`YUVAInfo` which specifies the
        transformation from YUV to RGB. The :py:class:`ColorSpace` of the
        resulting RGB values is specified by imageColorSpace. This will be the
        :py:class:`ColorSpace` reported by the image and when drawn the RGB
        values will be converted from this space into the destination space (if
        the destination is tagged).

        Currently, this is only supported using the GPU backend and will fail if
        context is nullptr.

        :py:class:`YUVAPixmaps` does not need to remain valid after this
        returns.

        :param context:               GPU context
        :param pixmaps:               The planes as pixmaps with supported
                                      :py:class:`YUVAInfo` that specifies
                                      conversion to RGB.
        :param buildMips:             create internal YUVA textures as mip map
                                      if kYes. This is silently ignored if the
                                      context does not support mip maps.
        :param limitToMaxTextureSize: downscale image to GPU maximum texture
                                      size, if necessary
        :param imageColorSpace:       range of colors of the resulting image;
                                      may be nullptr
        :return:                      created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("pixmaps"),
        py::arg("buildMips") = GrMipmapped::kNo,
        py::arg("limitToMaxTextureSize") = false,
        py::arg("imageColorSpace") = nullptr)
    .def_static("MakeFromNV12TexturesCopy",
        [] (GrDirectContext* context,
            SkYUVColorSpace yuvColorSpace,
            const std::vector<GrBackendTexture>& nv12Textures,
            GrSurfaceOrigin imageOrigin,
            const SkColorSpace* imageColorSpace) {
            if (nv12Textures.size() != 2)
                throw py::value_error("nv12Textures must have 2 elements.");
            return SkImage::MakeFromNV12TexturesCopy(
                context, yuvColorSpace, nv12Textures.data(), imageOrigin,
                CloneColorSpace(imageColorSpace));
        },
        R"docstring(
        Creates :py:class:`Image` from copy of nv12Textures, an array of
        textures on GPU. nv12Textures[0] contains pixels for YUV component y
        plane. nv12Textures[1] contains pixels for YUV component u plane,
        followed by pixels for YUV component v plane.
        Returned :py:class:`Image` has the dimensions nv12Textures[2].
        yuvColorSpace describes how YUV colors convert to RGB colors.

        :param context:         GPU context
        :param yuvColorSpace:   How the YUV values are converted to RGB
        :param nv12Textures:    array of YUV textures on GPU
        :param imageColorSpace: range of colors; may be nullptr
        :return:                created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("yuvColorSpace"), py::arg("nv12Textures"),
        py::arg("imageOrigin"), py::arg("imageColorSpace") = nullptr)
    .def_static("MakeFromNV12TexturesCopyWithExternalBackend",
        [] (GrDirectContext* context,
            SkYUVColorSpace yuvColorSpace,
            const std::vector<GrBackendTexture>& nv12Textures,
            GrSurfaceOrigin imageOrigin,
            const GrBackendTexture& backendTexture,
            const SkColorSpace* imageColorSpace) {
            if (nv12Textures.size() != 2)
                throw py::value_error("nv12Textures must have 2 elements.");
            return SkImage::MakeFromNV12TexturesCopyWithExternalBackend(
                context, yuvColorSpace, nv12Textures.data(), imageOrigin,
                backendTexture, CloneColorSpace(imageColorSpace), nullptr,
                nullptr);
        },
        R"docstring(
        Creates :py:class:`Image` from copy of nv12Textures, an array of
        textures on GPU. nv12Textures[0] contains pixels for YUV component y
        plane. nv12Textures[1] contains pixels for YUV component u plane,
        followed by pixels for YUV component v plane.
        Returned :py:class:`Image` has the dimensions nv12Textures[2] and stores
        pixels in backendTexture. yuvColorSpace describes how YUV colors convert
        to RGB colors.

        :param context:         GPU context
        :param yuvColorSpace:   How the YUV values are converted to RGB
        :param nv12Textures:    array of YUV textures on GPU
        :param backendTexture:  the resource that stores the final pixels
        :param imageColorSpace: range of colors; may be nullptr
        :return:                created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("yuvColorSpace"), py::arg("nv12Textures"),
        py::arg("imageOrigin"), py::arg("backendTexture"),
        py::arg("imageColorSpace") = nullptr)
*/
    .def_static("MakeFromPicture",
        [] (sk_sp<SkPicture>& picture, const SkISize& dimensions,
            const SkMatrix* matrix, const SkPaint* paint,
            SkImages::BitDepth bitDepth, const SkColorSpace* colorSpace) {
            return SkImages::DeferredFromPicture(
                picture, dimensions, matrix, paint, bitDepth,
                CloneColorSpace(colorSpace));
        },
        R"docstring(
        Creates :py:class:`Image` from picture.

        Returned :py:class:`Image` width and height are set by dimensions.
        :py:class:`Image` draws picture with matrix and paint, set to bitDepth
        and colorSpace.

        If matrix is nullptr, draws with identity :py:class:`Matrix`. If paint
        is nullptr, draws with default :py:class:`Paint`. colorSpace may be
        nullptr.

        :param skia.Picture picture: stream of drawing commands
        :param skia.ISize dimensions:  width and height
        :param skia.Matrix matrix: :py:class:`Matrix` to rotate, scale,
            translate, and so on; may be nullptr
        :param skia.Paint paint: :py:class:`Paint` to apply transparency,
            filtering, and so on; may be nullptr
        :param skia.Image.BitDepth bitDepth:    8-bit integer or 16-bit float:
            per component
        :param skia.ColorSpace colorSpace:  range of colors; may be nullptr
        :return: created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("picture"), py::arg("dimensions"), py::arg("matrix") = nullptr,
        py::arg("paint") = nullptr,
        py::arg("bitDepth") = SkImages::BitDepth::kU8,
        py::arg("colorSpace") = nullptr)
    .def("imageInfo", &SkImage::imageInfo,
        R"docstring(
        Returns a :py:class:`ImageInfo` describing the width, height, color
        type, alpha type, and color space of the :py:class:`Image`.

        :return: image info of :py:class:`Image`.
        )docstring")
    .def("width", &SkImage::width,
        R"docstring(
        Returns pixel count in each row.

        :return: pixel width in :py:class:`Image`
        )docstring")
    .def("height", &SkImage::height,
        R"docstring(
        Returns pixel row count.

        :return: pixel height in :py:class:`Image`
        )docstring")
    .def("dimensions", &SkImage::dimensions,
        R"docstring(
        Returns :py:class:`ISize` (:py:meth:`width`, :py:meth:`height`).

        :return: integral size of :py:meth:`width` and :py:meth:`height`
        )docstring")
    .def("bounds", &SkImage::bounds,
        R"docstring(
        Returns :py:class:`IRect` (0, 0, :py:meth:`width`, :py:meth:`height`).

        :return: integral rectangle from origin to :py:meth:`width` and
            :py:meth:`height`
        )docstring")
    .def("uniqueID", &SkImage::uniqueID,
        R"docstring(
        Returns value unique to image.

        :py:class:`Image` contents cannot change after :py:class:`Image` is
        created. Any operation to create a new :py:class:`Image` will receive
        generate a new unique number.

        :return: unique identifier
        )docstring")
    .def("alphaType", &SkImage::alphaType,
        R"docstring(
        Returns :py:class:`AlphaType`.

        :py:class:`AlphaType` returned was a parameter to an :py:class:`Image`
        constructor, or was parsed from encoded data.

        :return: :py:class:`AlphaType` in :py:class:`Image`
        )docstring")
    .def("colorType", &SkImage::colorType,
        R"docstring(
        Returns :py:class:`ColorType` if known; otherwise, returns
        :py:attr:`~ColorType.kUnknown_ColorType`.

        :return: :py:class:`ColorType` of :py:class:`Image`
        )docstring")
    .def("colorSpace", &SkImage::colorSpace,
        R"docstring(
        Returns :py:class:`ColorSpace`, the range of colors, associated with
        :py:class:`Image`.

        The reference count of :py:class:`ColorSpace` is unchanged. The returned
        :py:class:`ColorSpace` is immutable.

        :py:class:`ColorSpace` returned was passed to an :py:class:`Image`
        constructor, or was parsed from encoded data. :py:class:`ColorSpace`
        returned may be ignored when :py:class:`Image` is drawn, depending on
        the capabilities of the :py:class:`Surface` receiving the drawing.

        :return: :py:class:`ColorSpace` in :py:class:`Image`, or nullptr
        )docstring",
        py::return_value_policy::reference)
    .def("refColorSpace", &SkImage::refColorSpace,
        R"docstring(
        Returns a smart pointer to :py:class:`ColorSpace`, the range of colors,
        associated with :py:class:`Image`.

        The smart pointer tracks the number of objects sharing this
        :py:class:`ColorSpace` reference so the memory is released when the
        owners destruct.

        The returned :py:class:`ColorSpace` is immutable.

        :py:class:`ColorSpace` returned was passed to an :py:class:`Image`
        constructor, or was parsed from encoded data. :py:class:`ColorSpace`
        returned may be ignored when :py:class:`Image` is drawn, depending on
        the capabilities of the :py:class:`Surface` receiving the drawing.

        :return: :py:class:`ColorSpace` in :py:class:`Image`, or nullptr,
            wrapped in a smart pointer
        )docstring")
    .def("isAlphaOnly", &SkImage::isAlphaOnly,
        R"docstring(
        Returns true if :py:class:`Image` pixels represent transparency only.

        If true, each pixel is packed in 8 bits as defined by
        :py:attr:`~ColorType.kAlpha_8_ColorType`.

        :return: true if pixels represent a transparency mask
        )docstring")
    .def("isOpaque", &SkImage::isOpaque,
        R"docstring(
        Returns true if pixels ignore their alpha value and are treated as fully
        opaque.

        :return: true if :py:class:`AlphaType` is
            :py:attr:`~AlphaType.kOpaque_AlphaType`
        )docstring")
    .def("makeShader",
        py::overload_cast<SkTileMode, SkTileMode, const SkSamplingOptions&, const SkMatrix*>(
            &SkImage::makeShader, py::const_),
        R"docstring(
        Creates :py:class:`Shader` from :py:class:`Image`.

        :py:class:`Shader` dimensions are taken from :py:class:`Image`.
        :py:class:`Shader` uses :py:class:`TileMode` rules to fill drawn area
        outside :py:class:`Image`. localMatrix permits transforming
        :py:class:`Image` before :py:class:`Canvas` matrix is applied.

        :param skia.TileMode tmx: tiling in the x direction
        :param skia.TileMode tmy: tiling in the y direction
        :param skia.Matrix localMatrix: :py:class:`Image` transformation, or
            nullptr
        :return: :py:class:`Shader` containing :py:class:`Image`
        )docstring",
        py::arg("tmx") = SkTileMode::kClamp,
        py::arg("tmy") = SkTileMode::kClamp, py::arg("samplingOptions") = SkSamplingOptions(), py::arg("localMatrix") = nullptr)
    // TODO: Other makeShader overloads.
    .def("peekPixels", &SkImage::peekPixels,
        R"docstring(
        Copies :py:class:`Image` pixel address, row bytes, and
        :py:class:`ImageInfo` to pixmap, if address is available, and returns
        true.

        If pixel address is not available, return false and leave pixmap
        unchanged.

        :param skia.Pixmap pixmap: storage for pixel state if pixels are
            readable; otherwise, ignored
        :return: true if :py:class:`Image` has direct access to pixels
        )docstring",
        py::arg("pixmap"))
    .def("isTextureBacked", &SkImage::isTextureBacked,
        R"docstring(
        Returns true the contents of :py:class:`Image` was created on or
        uploaded to GPU memory, and is available as a GPU texture.

        :return: true if :py:class:`Image` is a GPU texture
        )docstring")
    .def("isValid", &SkImage::isValid,
        R"docstring(
        Returns true if :py:class:`Image` can be drawn on either raster surface
        or GPU surface.

        If context is nullptr, tests if :py:class:`Image` draws on raster
        surface; otherwise, tests if :py:class:`Image` draws on GPU surface
        associated with context.

        :py:class:`Image` backed by GPU texture may become invalid if associated
        GrRecordingContext is invalid. lazy image may be invalid and may not draw
        to raster surface or GPU surface or both.

        :param skia.GrRecordingContext context: GPU context
        :return: true if :py:class:`Image` can be drawn
        )docstring",
        py::arg("context") = nullptr)
/*
    .def("flush",
        py::overload_cast<GrDirectContext*, const GrFlushInfo&>(&SkImage::flush),
        R"docstring(
        Flushes any pending uses of texture-backed images in the GPU backend. If
        the image is not texture-backed (including promise texture images) or if
        the GrDirectContext does not have the same context ID as the context
        backing the image then this is a no-op.

        If the image was not used in any non-culled draws in the current queue
        of work for the passed GrDirectContext then this is a no-op unless the
        GrFlushInfo contains semaphores or a finish proc. Those are respected
        even when the image has not been used.

        :param context:  the context on which to flush pending usages of the
            image.
        :param info:     flush options
        )docstring"
        )
    .def("flush",
        py::overload_cast<GrDirectContext*>(&SkImage::flush),
        py::arg("context").none(false))
    .def("flushAndSubmit", &SkImage::flushAndSubmit,
        R"docstring(
        Version of :py:meth:`flush` that uses a default GrFlushInfo.

        Also submits the flushed work to the GPU.
        )docstring",
        py::arg("context").none(false))
    .def("getBackendTexture", &SkImage::getBackendTexture,
        R"docstring(
        Retrieves the back-end texture. If :py:class:`Image` has no back-end
        texture, an invalid object is returned. Call
        :py:meth:`GrBackendTexture.isValid` to determine if the result is valid.

        If flushPendingGrContextIO is true, completes deferred I/O operations.

        If origin in not nullptr, copies location of content drawn into
        :py:class:`Image`.

        :param flushPendingGrContextIO: flag to flush outstanding requests
        :return: back-end API texture handle; invalid on failure
        )docstring",
        py::arg("flushPendingGrContextIO"), py::arg("origin") = nullptr)
*/
    .def("readPixels", &ImageReadPixels,
        R"docstring(
        Copies a :py:class:`Rect` of pixels from :py:class:`Image` to dst. Copy
        starts at (srcX, srcY), and does not exceed :py:class:`Image` (width(),
        height()).

        dstInfo specifies width, height, :py:class:`ColorType`,
        :py:class:`AlphaType`, and :py:class:`ColorSpace` of destination.
        dstRowBytes specifics the gap from one destination row to the next.
        Returns true if pixels are copied. Returns false if:

        - dstInfo.addr() equals nullptr
        - dstRowBytes is less than ``dstInfo.minRowBytes()``
        - :py:class:`PixelRef` is nullptr

        Pixels are copied only if pixel conversion is possible. If
        :py:class:`Image` :py:class:`ColorType` is
        :py:attr:`~kGray_8_ColorType`, or :py:attr:`~kAlpha_8_ColorType`;
        dst.colorType() must match. If :py:class:`Image` :py:class:`ColorType`
        is :py:attr:`~kGray_8_ColorType`, dst.colorSpace() must match. If
        :py:class:`Image` :py:class:`AlphaType` is
        :py:attr:`~kOpaque_AlphaType`, dst.alphaType() must match. If
        :py:class:`Image` :py:class:`ColorSpace` is nullptr, dst.colorSpace()
        must match. Returns false if pixel conversion is not possible.

        srcX and srcY may be negative to copy only top or left of source.
        Returns false if width() or height() is zero or negative. Returns false
        if abs(srcX) >= Image width(), or if abs(srcY) >= Image height().

        If cachingHint is kAllow_CachingHint, pixels may be retained locally.
        If cachingHint is kDisallow_CachingHint, pixels are not added to the
        local cache.

        :param context:      the GrDirectContext in play, if it exists
        :param dstInfo:      destination width, height, pixels,
                             :py:class:`ColorType`, :py:class:`AlphaType`,
                             :py:class:`ColorSpace`
        :param dstPixels:    destination pixel storage
        :param dstRowBytes:  destination row length
        :param srcX:         column index whose absolute value is less than
                             width()
        :param srcY:         row index whose absolute value is less than
                             height()
        :param cachingHint:  whether the pixels should be cached locally
        :return:             true if pixels are copied to dstPixels
        )docstring",
        py::arg("context"), py::arg("dstInfo"), py::arg("dstPixels"),
        py::arg("dstRowBytes"), py::arg("srcX") = 0, py::arg("srcY") = 0,
        py::arg("cachingHint") = SkImage::CachingHint::kAllow_CachingHint)
    .def("readPixels",
        py::overload_cast<GrDirectContext*, const SkPixmap&, int, int,
            SkImage::CachingHint>(&SkImage::readPixels, py::const_),
        R"docstring(
        Copies a :py:class:`Rect` of pixels from :py:class:`Image` to dst. Copy
        starts at (srcX, srcY), and does not exceed :py:class:`Image` (width(),
        height()).

        dst specifies width, height, :py:class:`ColorType`,
        :py:class:`AlphaType`, :py:class:`ColorSpace`, pixel storage, and row
        bytes of destination. dst.rowBytes() specifics the gap from one
        destination row to the next. Returns true if pixels are copied. Returns
        false if:

        - dst pixel storage equals nullptr
        - dst.rowBytes is less than :py:meth:`ImageInfo.minRowBytes`
        - :py:class:`PixelRef` is nullptr

        Pixels are copied only if pixel conversion is possible. If
        :py:class:`Image` :py:class:`ColorType` is
        :py:attr:`~kGray_8_ColorType`, or :py:attr:`~kAlpha_8_ColorType`;
        dst.colorType() must match. If :py:class:`Image` :py:class:`ColorType`
        is :py:attr:`~kGray_8_ColorType`, dst.colorSpace() must match. If
        :py:class:`Image` :py:class:`AlphaType` is
        :py:attr:`~kOpaque_AlphaType`, dst.alphaType() must match. If
        :py:class:`Image` :py:class:`ColorSpace` is nullptr, dst.colorSpace()
        must match. Returns false if pixel conversion is not possible.

        srcX and srcY may be negative to copy only top or left of source.
        Returns false if width() or height() is zero or negative. Returns false
        if abs(srcX) >= Image width(), or if abs(srcY) >= Image height().

        If cachingHint is kAllow_CachingHint, pixels may be retained locally.
        If cachingHint is kDisallow_CachingHint, pixels are not added to the
        local cache.

        :param context:      the GrDirectContext in play, if it exists
        :param dst:          destination :py:class:`Pixmap`:
                             :py:class:`ImageInfo`, pixels, row bytes
        :param srcX:         column index whose absolute value is less than
                             width()
        :param srcY:         row index whose absolute value is less than
                             height()
        :param cachingHint:  whether the pixels should be cached locally
        :return:             true if pixels are copied to dst
        )docstring",
        py::arg("context"), py::arg("dst"), py::arg("srcX"), py::arg("srcY"),
        py::arg("cachingHint") = SkImage::CachingHint::kAllow_CachingHint)
    .def("readPixels",
        [] (const SkImage& image, const SkImageInfo& dstInfo,
            py::buffer dstPixels, size_t dstRowBytes, int srcX, int srcY,
            SkImage::CachingHint cachingHint) {
            return ImageReadPixels(
                image, nullptr, dstInfo, dstPixels, dstRowBytes, srcX, srcY,
                cachingHint);
        },
        R"docstring(
        Deprecated. Use the variants that accept a GrDirectContext.
        )docstring",
        py::arg("dstInfo"), py::arg("dstPixels"), py::arg("dstRowBytes"),
        py::arg("srcX") = 0, py::arg("srcY") = 0,
        py::arg("cachingHint") = SkImage::kAllow_CachingHint)
    .def("readPixels",
        py::overload_cast<const SkPixmap&, int, int, SkImage::CachingHint>(
            &SkImage::readPixels, py::const_),
        R"docstring(
        Deprecated. Use the variants that accept a `GrDirectContext`.
        )docstring",
        py::arg("dst"), py::arg("srcX"), py::arg("srcY"),
        py::arg("cachingHint") = SkImage::CachingHint::kAllow_CachingHint)
    // .def("asyncRescaleAndReadPixels", &SkImage::asyncRescaleAndReadPixels)
    // .def("asyncRescaleAndReadPixelsYUV420",
    //      &SkImage::asyncRescaleAndReadPixelsYUV420)
    .def("scalePixels", &SkImage::scalePixels,
        R"docstring(
        Copies :py:class:`Image` to dst, scaling pixels to fit ``dst.width()``
        and ``dst.height()``, and converting pixels to match ``dst.colorType()``
        and ``dst.alphaType()``.

        Returns true if pixels are copied. Returns false if ``dst.addr()`` is
        nullptr, or ``dst.rowBytes()`` is less than dst
        :py:meth:`ImageInfo.minRowBytes`.

        Pixels are copied only if pixel conversion is possible. If
        :py:class:`Image` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, or
        :py:attr:`~ColorType.kAlpha_8_ColorType`; ``dst.colorType()`` must
        match. If :py:class:`Image` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, ``dst.colorSpace()`` must
        match. If :py:class:`Image` :py:class:`AlphaType` is
        :py:attr:`~AlphaType.kOpaque_AlphaType`, ``dst.alphaType()`` must match.
        If :py:class:`Image` :py:class:`ColorSpace` is nullptr,
        ``dst.colorSpace()`` must match. Returns false if pixel conversion is
        not possible.

        Scales the image, with filterQuality, to match ``dst.width()`` and
        ``dst.height()``. filterQuality
        :py:attr:`~FilterQuality.None_FilterQuality` is fastest, typically
        implemented with nearest neighbor filter.
        :py:attr:`~FilterQuality.kLow_FilterQuality` is typically implemented
        with bilerp filter. :py:attr:`~FilterQuality.kMedium_FilterQuality` is
        typically implemented with bilerp filter, and mip-map filter when size
        is reduced. :py:attr:`~FilterQuality.kHigh_FilterQuality` is slowest,
        typically implemented with bicubic filter.

        If cachingHint is :py:attr:`~Image.CachingHint.kAllow_CachingHint`,
        pixels may be retained locally. If cachingHint is
        :py:attr:`~Image.CachingHint.kDisallow_CachingHint`, pixels are not
        added to the local cache.

        :param skia.Pixmap dst: destination :py:class:`Pixmap`:
            :py:class:`ImageInfo`, pixels, row bytes
        :param skia.FilterQuality filterQuality: Filter quality
        :param skia.Image.CachingHint cachingHint: Caching hint
        :return: true if pixels are scaled to fit dst
        )docstring",
        py::arg("dst"),
        py::arg("samplingOptions") = SkSamplingOptions(),
        py::arg("cachingHint") = SkImage::kAllow_CachingHint)
    .def("encodeToData",
        [] (SkImage& image, SkEncodedImageFormat format, int quality) {
            sk_sp<SkData> data = image.refEncodedData();
            switch (format) {
            case SkEncodedImageFormat::kWEBP:
                {
                SkWebpEncoder::Options options;
                if (quality < 100) {
                    options.fCompression = SkWebpEncoder::Compression::kLossy;
                    options.fQuality = quality;
                } else {
                    options.fCompression = SkWebpEncoder::Compression::kLossless;
                    // in lossless mode, this is effort. 70 is the default effort in SkImageEncoder,
                    // which follows Blink and WebPConfigInit.
                    options.fQuality = 70;
                }
                data = SkWebpEncoder::Encode(nullptr, &image, options);
                }
                break;

            case SkEncodedImageFormat::kJPEG:
                {
                SkJpegEncoder::Options options;
                options.fQuality = quality;
                data = SkJpegEncoder::Encode(nullptr, &image, options);
                }
                break;

            case SkEncodedImageFormat::kPNG:
            default:
                {
                SkPngEncoder::Options options; // Not used
                data = SkPngEncoder::Encode(nullptr, &image, {});
                }
                break;
            }
            auto decoded = SkImages::DeferredFromEncodedData(data);
            return data;
        },
        R"docstring(
        Encodes :py:class:`Image` pixels, returning result as :py:class:`Data`.

        Returns nullptr if encoding fails, or if encodedImageFormat is not
        supported.

        :py:class:`Image` encoding in a format requires both building with one
        or more of: SK_ENCODE_JPEG, SK_ENCODE_PNG, SK_ENCODE_WEBP; and platform
        support for the encoded format.

        If SK_BUILD_FOR_MAC or SK_BUILD_FOR_IOS is defined, encodedImageFormat
        can additionally be one of: :py:attr:`~EncodedImageFormat.kICO`,
        :py:attr:`~EncodedImageFormat.kBMP`,
        :py:attr:`~EncodedImageFormat.kGIF`.

        quality is a platform and format specific metric trading off size and
        encoding error. When used, quality equaling 100 encodes with the least
        error. quality may be ignored by the encoder.

        :param skia.EncodedImageFormat encodedImageFormat:
            one of: :py:attr:`~EncodedImageFormat.kJPEG`,
            :py:attr:`~EncodedImageFormat.kPNG`,
            :py:attr:`~EncodedImageFormat.kWEBP`
        :param int quality: encoder specific metric with 100 equaling best
        :return: encoded :py:class:`Image`, or nullptr
        )docstring",
        py::arg("encodedImageFormat"), py::arg("quality"))
    .def("encodeToData",
        [] (SkImage& image) {
            sk_sp<SkData> data = image.refEncodedData();
            if (!data) {
                data = SkPngEncoder::Encode(nullptr, &image, {});
            }
            return data;
        },
        R"docstring(
        Encodes :py:class:`Image` pixels, returning result as :py:class:`Data`.

        Returns existing encoded data if present; otherwise, :py:class:`Image`
        is encoded with :py:attr:`~EncodedImageFormat.kPNG`. Skia must be built
        with SK_ENCODE_PNG to encode :py:class:`Image`.

        Returns nullptr if existing encoded data is missing or invalid, and
        encoding fails.

        :return: encoded :py:class:`Image`, or nullptr
        )docstring")
    .def("refEncodedData", &SkImage::refEncodedData,
        R"docstring(
        Returns encoded :py:class:`Image` pixels as :py:class:`Data`, if
        :py:class:`Image` was created from supported encoded stream format.

        Platform support for formats vary and may require building with one or
        more of: SK_ENCODE_JPEG, SK_ENCODE_PNG, SK_ENCODE_WEBP.

        Returns nullptr if :py:class:`Image` contents are not encoded.

        :return: encoded :py:class:`Image`, or nullptr
        )docstring")
    .def("makeSubset",
        [] (SkImage& image, const SkIRect& subset, GrDirectContext* direct) {
            return image.makeSubset(direct, subset);
        },
        R"docstring(
        Returns subset of :py:class:`Image`.

        subset must be fully contained by :py:class:`Image`
        :py:meth:`dimensions`. The implementation may share pixels, or may copy
        them.

        Returns nullptr if subset is empty, or subset is not contained by
        bounds, or pixels in :py:class:`Image` could not be read or copied.

        :param skia.IRect subset: bounds of returned :py:class:`Image`
        :return: partial or full :py:class:`Image`, or nullptr
        )docstring",
        py::arg("subset"), py::arg("direct") = nullptr)
    .def("hasMipmaps", &SkImage::hasMipmaps,
        R"docstring(
        Returns true if the image has mipmap levels.
        )docstring")
    .def("withDefaultMipmaps", &SkImage::withDefaultMipmaps,
        R"docstring(
        Returns an image with the same "base" pixels as the this image, but with
        mipmap levels automatically generated and attached.
        )docstring")
/*
    .def("makeTextureImage", &SkImage::makeTextureImage,
        R"docstring(
        Returns :py:class:`Image` backed by GPU texture associated with context.

        Returned :py:class:`Image` is compatible with :py:class:`Surface`
        created with dstColorSpace. The returned :py:class:`Image` respects
        mipMapped setting; if mipMapped equals :py:attr:`GrMipmapped.kYes`, the
        backing texture allocates mip map levels.

        The mipMapped parameter is effectively treated as kNo if MIP maps are
        not supported by the GPU.

        Returns original :py:class:`Image` if the image is already
        texture-backed, the context matches, and mipMapped is compatible with
        the backing GPU texture. :py:class:`Budgeted` is ignored in this case.

        Returns nullptr if context is nullptr, or if :py:class:`Image` was
        created with another GrDirectContext.

        :param GrDirectContext context: the GrDirectContext in play, if it
            exists
        :param GrMipmapped mipMapped: whether created :py:class:`Image` texture
            must allocate mip map levels
        :param skia.Budgeted budgeted: whether to count a newly created texture
            for the returned image counts against the GrDirectContext's budget.
        :return: created :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context").none(false), py::arg("mipMapped") = GrMipmapped::kNo,
        py::arg("budgeted") = SkBudgeted::kYes)
*/
    .def("makeNonTextureImage", &SkImage::makeNonTextureImage,
        R"docstring(
        Returns raster image or lazy image.

        Copies :py:class:`Image` backed by GPU texture into CPU memory if
        needed. Returns original :py:class:`Image` if decoded in raster bitmap,
        or if encoded in a stream.

        Returns nullptr if backed by GPU texture and copy fails.

        :return: raster image, lazy image, or nullptr
        )docstring")
    .def("makeRasterImage", py::overload_cast<SkImage::CachingHint>(&SkImage::makeRasterImage, py::const_),
        R"docstring(
        Returns raster image.

        Copies :py:class:`Image` backed by GPU texture into CPU memory, or
        decodes :py:class:`Image` from lazy image. Returns original
        :py:class:`Image` if decoded in raster bitmap.

        Returns nullptr if copy, decode, or pixel read fails.

        If cachingHint is :py:attr:`Image.CachingHint.kAllow_CachingHint`,
        pixels may be retained locally. If cachingHint is
        :py:attr:`Image.CachingHint.kDisallow_CachingHint`, pixels are not added
        to the local cache.

        :param skia.Image.CachingHint cachingHint: Caching hint
        :return: raster image, or nullptr
        )docstring",
        py::arg("cachingHint") = SkImage::kAllow_CachingHint)
    .def("makeWithFilter",
        py::overload_cast<GrRecordingContext*, const SkImageFilter*,
            const SkIRect&, const SkIRect&, SkIRect*, SkIPoint*>(
                &SkImage::makeWithFilter, py::const_),
        R"docstring(
        Creates filtered :py:class:`Image`.

        filter processes original :py:class:`Image`, potentially changing color,
        position, and size. subset is the bounds of original :py:class:`Image`
        processed by filter. clipBounds is the expected bounds of the filtered
        :py:class:`Image`. outSubset is required storage for the actual bounds
        of the filtered :py:class:`Image`. offset is required storage for
        translation of returned :py:class:`Image`.

        Returns nullptr if :py:class:`Image` could not be created. If nullptr is
        returned, outSubset and offset are undefined.

        Useful for animation of :py:class:`ImageFilter` that varies size from
        frame to frame. Returned :py:class:`Image` is created larger than
        required by filter so that GPU texture can be reused with different
        sized effects. outSubset describes the valid bounds of GPU texture
        returned. offset translates the returned :py:class:`Image` to keep
        subsequent animation frames aligned with respect to each other.

        :param skia.GrRecordingContext context: the GrRecordingContext in play -
            if it exists
        :param skia.ImageFilter filter: how :py:class:`Image` is sampled when
            transformed
        :param skia.IRect subset:  bounds of :py:class:`Image` processed by
            filter
        :param skia.IRect clipBounds:  expected bounds of filtered
            :py:class:`Image`
        :param skia.IRect outSubset:   output for returned :py:class:`Image`
            bounds
        :param skia.IPoint offset:  output for returned :py:class:`Image`
            translation
        :return: filtered :py:class:`Image`, or nullptr
        )docstring",
        py::arg("context"), py::arg("filter"), py::arg("subset"),
        py::arg("clipBounds"), py::arg("outSubset").none(false),
        py::arg("offset").none(false))
/*
    .def_static("MakeBackendTextureFromImage",
        [] (GrDirectContext* context, sk_sp<SkImage>& image,
            GrBackendTexture* backendTexture) {
            return SkImages::GetBackendTextureFromImage(
                context, image, backendTexture, nullptr);
        },
        R"docstring(
        Creates a :py:class:`GrBackendTexture` from the provided
        :py:class:`Image`.

        Returns true and stores result in backendTexture if texture is created;
        otherwise, returns false and leaves backendTexture unmodified.

        If :py:class:`Image` is both texture backed and singly referenced, image
        is returned in backendTexture without conversion or making a copy.
        :py:class:`Image` is singly referenced if it was transferred solely
        using std::move().

        If :py:class:`Image` is not texture backed, returns texture with
        :py:class:`Image` contents.

        :param skia.GrContext context: GPU context
        :param skia.Image image:   :py:class:`Image` used for texture
        :param skia.GrBackendTexture backendTexture:  storage for back-end
            texture
        :return: true if back-end texture was created
        )docstring",
        py::arg("context"), py::arg("image"), py::arg("backendTexture"))
*/
    .def("asLegacyBitmap", &SkImage::asLegacyBitmap,
        R"docstring(
        Deprecated.

        Creates raster :py:class:`Bitmap` with same pixels as :py:class:`Image`.
        If legacyBitmapMode is :py:attr:`~Image.kRO_LegacyBitmapMode`, returned
        bitmap is read-only and immutable. Returns true if :py:class:`Bitmap` is
        stored in bitmap. Returns false and resets bitmap if :py:class:`Bitmap`
        write did not succeed.

        :param bitmap: storage for legacy :py:class:`Bitmap`
        :param legacyBitmapMode: bitmap is read-only and immutable
        :return: true if :py:class:`Bitmap` was created
        )docstring",
        py::arg("bitmap").none(false),
        py::arg("legacyBitmapMode") = SkImage::kRO_LegacyBitmapMode)
    .def("isLazyGenerated", &SkImage::isLazyGenerated,
        R"docstring(
        Returns true if :py:class:`Image` is backed by an image-generator or
        other service that creates and caches its pixels or texture on-demand.

        :return: true if :py:class:`Image` is created as needed
        )docstring")
    .def("makeColorSpace",
        [] (const SkImage& image, const SkColorSpace* target,
            GrDirectContext* direct) {
            return image.makeColorSpace(direct, CloneColorSpace(target));
        },
        R"docstring(
        Creates :py:class:`Image` in target :py:class:`ColorSpace`.

        Returns nullptr if :py:class:`Image` could not be created.

        Returns original :py:class:`Image` if it is in target
        :py:class:`ColorSpace`. Otherwise, converts pixels from
        :py:class:`Image` :py:class:`ColorSpace` to target
        :py:class:`ColorSpace`. If :py:class:`Image` colorSpace() returns
        nullptr, :py:class:`Image` :py:class:`ColorSpace` is assumed to be sRGB.

        :param skia.ColorSpace target: :py:class:`ColorSpace` describing color
            range of returned :py:class:`Image`
        :return: created :py:class:`Image` in target :py:class:`ColorSpace`
        )docstring",
        py::arg("target"), py::arg("direct") = nullptr)
    .def("makeColorTypeAndColorSpace",
        [] (const SkImage& image, SkColorType ct, const SkColorSpace* cs,
            GrDirectContext* direct) {
            return image.makeColorTypeAndColorSpace(
                direct, ct, CloneColorSpace(cs));
        },
        R"docstring(
        Experimental.

        Creates :py:class:`Image` in target :py:class:`ColorType` and
        :py:class:`ColorSpace`. Returns nullptr if :py:class:`Image` could not
        be created.

        Returns original :py:class:`Image` if it is in target
        :py:class:`ColorType` and :py:class:`ColorSpace`.

        :param skia.ColorType targetColorType: :py:class:`ColorType` of returned
            :py:class:`Image`
        :param skia.ColorSpace targetColorSpace: :py:class:`ColorSpace` of
            returned :py:class:`Image`
        :return: created :py:class:`Image` in target :py:class:`ColorType` and
            :py:class:`ColorSpace`
        )docstring",
        py::arg("targetColorType"), py::arg("targetColorSpace") = nullptr,
        py::arg("direct") = nullptr)
    .def("reinterpretColorSpace",
        [] (const SkImage& image, const SkColorSpace* cs) {
            return image.reinterpretColorSpace(CloneColorSpace(cs));
        },
        R"docstring(
        Creates a new :py:class:`Image` identical to this one, but with a
        different :py:class:`ColorSpace`.

        This does not convert the underlying pixel data, so the resulting image
        will draw differently.
        )docstring",
        py::arg("newColorSpace") = nullptr)
    ;
}
