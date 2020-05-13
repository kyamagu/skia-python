#include "common.h"
#include <pybind11/numpy.h>

template<typename T>
using NumPy = py::array_t<T, py::array::c_style | py::array::forcecast>;

void initImage(py::module &m) {
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

py::enum_<SkTileMode>(m, "TileMode")
    .value("kClamp", SkTileMode::kClamp,
        "Replicate the edge color if the shader draws outside of its original "
        "bounds.")
    .value("kRepeat", SkTileMode::kRepeat,
        "Repeat the shader's image horizontally and vertically.")
    .value("kMirror", SkTileMode::kMirror,
        "Repeat the shader's image horizontally and vertically, alternating "
        "mirror images so that adjacent images always seam.")
    .value("kDecal", SkTileMode::kDecal,
        "Only draw within the original domain, return transparent-black "
        "everywhere else.")
    .value("kLastTileMode", SkTileMode::kLastTileMode,
        "")
    .export_values();

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

    .. rubric:: Classes

    .. autosummary::
        :nosignatures:

        ~Image.CompressionType
        ~Image.BitDepth
        ~Image.CachingHint
    )docstring");

py::enum_<SkImage::CompressionType>(image, "CompressionType")
    .value("kNone", SkImage::CompressionType::kNone)
    .value("kETC2_RGB8_UNORM", SkImage::CompressionType::kETC2_RGB8_UNORM)
    .value("kBC1_RGB8_UNORM", SkImage::CompressionType::kBC1_RGB8_UNORM)
    .value("kBC1_RGBA8_UNORM", SkImage::CompressionType::kBC1_RGBA8_UNORM)
    .value("kLast", SkImage::CompressionType::kLast)
    .export_values();

py::enum_<SkImage::BitDepth>(image, "BitDepth")
    .value("kU8", SkImage::BitDepth::kU8)
    .value("kF16", SkImage::BitDepth::kF16)
    .export_values();

py::enum_<SkImage::CachingHint>(image, "CachingHint")
    .value("kAllow_CachingHint", SkImage::CachingHint::kAllow_CachingHint)
    .value("kDisallow_CachingHint", SkImage::CachingHint::kDisallow_CachingHint)
    .export_values();

image
    .def(py::init([] (NumPy<uint8_t> array) {
        py::buffer_info info = array.request();
        if (info.ndim <= 1)
            throw std::runtime_error(
                "Number of dimensions must be 2 or more.");
        if (info.shape[0] == 0 || info.shape[1] == 0)
            throw std::runtime_error(
                "Width and height must be greater than 0.");
        auto imageinfo = SkImageInfo::MakeN32Premul(
            info.shape[1], info.shape[0]);
        auto data = SkData::MakeWithoutCopy(
            info.ptr, info.shape[0] * info.strides[0]);
        return SkImage::MakeRasterData(imageinfo, data, info.strides[0]);
    }))
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
            :py:attr:`~AlphaType.kOpaque_SkAlphaType`
        )docstring")
    .def("makeShader",
        py::overload_cast<SkTileMode, SkTileMode, const SkMatrix*>(
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
        py::arg("tmy") = SkTileMode::kClamp, py::arg("localMatrix") = nullptr)
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
        py::arg("pixmap").none(false))
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
        GrContext is invalid. lazy image may be invalid and may not draw to
        raster surface or GPU surface or both.

        :param skia.GrContext context: GPU context
        :return: true if :py:class:`Image` can be drawn
        )docstring",
        py::arg("context") = nullptr)
    // .def("flush",
    //     py::overload_cast<GrContext*, const GrFlushInfo&>(&SkImage::flush),
    //     "Flushes any pending uses of texture-backed images in the GPU "
    //     "backend.")
    .def("flush",
        py::overload_cast<GrContext*>(&SkImage::flush),
        R"docstring(
        Flushes any pending uses of texture-backed images in the GPU backend.

        Uses a default GrFlushInfo.

        :param skia.GrContext context: GPU context
        )docstring",
        py::arg("context").none(false))
    // .def("getBackendTexture", &SkImage::getBackendTexture,
    //     "Retrieves the back-end texture.")
    .def("readPixels",
        [] (const SkImage& image, const SkImageInfo& info, py::buffer dst,
            size_t dstRowBytes, int srcX, int srcY,
            SkImage::CachingHint cachingHint) {
            auto b = dst.request();
            size_t given = (b.ndim) ? b.shape[0] * b.strides[0] : 0;
            if (given < info.computeByteSize(dstRowBytes))
                throw std::runtime_error("dstPixels is smaller than required.");
            return image.readPixels(
                info, b.ptr, dstRowBytes, srcX, srcY, cachingHint);
        },
        R"docstring(
        Copies :py:class:`Rect` of pixels from :py:class:`Image` to dstPixels.

        Copy starts at offset (srcX, srcY), and does not exceed
        :py:class:`Image` (:py:meth:`width`, :py:meth:`height`).

        dstInfo specifies width, height, :py:class:`ColorType`,
        :py:class:`AlphaType`, and :py:class:`ColorSpace` of destination.
        dstRowBytes specifies the gap from one destination row to the next.
        Returns true if pixels are copied. Returns false if:

        - ``dstInfo.addr`` equals nullptr
        - dstRowBytes is less than ``dstInfo.minRowBytes``
        - :py:class:`PixelRef` is nullptr

        Pixels are copied only if pixel conversion is possible. If
        :py:class:`Image` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, or
        :py:attr:`~ColorType.kAlpha_8_ColorType`; ``dstInfo.colorType``
        must match. If :py:class:`Image` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, ``dstInfo.colorSpace`` must
        match. If :py:class:`Image` :py:class:`AlphaType` is
        :py:attr:`~AlphaType.kOpaque_AlphaType`, ``dstInfo.alphaType`` must
        match. If :py:class:`Image` :py:class:`ColorSpace` is nullptr,
        ``dstInfo.colorSpace`` must match. Returns false if pixel conversion is
        not possible.

        srcX and srcY may be negative to copy only top or left of source.
        Returns false if :py:meth:`width` or :py:meth:`height` is zero or
        negative. Returns false if abs(srcX) >= Image :py:meth:`width`, or if
        abs(srcY) >= Image :py:meth:`height`.

        If cachingHint is :py:attr:`~Image.CachingHint.kAllow_CachingHint`,
        pixels may be retained locally. If cachingHint is
        :py:attr:`~Image.CachingHint.kDisallow_CachingHint`, pixels are not
        added to the local cache.

        :dstInfo: destination width, height, :py:class:`ColorType`,
            :py:class:`AlphaType`, :py:class:`ColorSpace`
        :dstPixels:   destination pixel storage
        :dstRowBytes: destination row length
        :srcX: column index whose absolute value is less than
            :py:meth:`width`
        :srcY: row index whose absolute value is less than
            :py:meth:`height`
        :return: true if pixels are copied to dstPixels
        )docstring",
        py::arg("dstInfo"), py::arg("dstPixels"), py::arg("dstRowBytes"),
        py::arg("srcX") = 0, py::arg("srcY") = 0,
        py::arg("cachingHint") = SkImage::kAllow_CachingHint)
    .def("readPixels",
        (bool (SkImage::*)(const SkPixmap&, int, int, SkImage::CachingHint)
            const) &SkImage::readPixels,
        "Copies a SkRect of pixels from SkImage to dst.",
        py::arg("dst"), py::arg("srcX"), py::arg("srcY"),
        py::arg("cachingHint") = SkImage::CachingHint::kAllow_CachingHint)
    .def("scalePixels", &SkImage::scalePixels,
        "Copies SkImage to dst, scaling pixels to fit dst.width() and "
        "dst.height(), and converting pixels to match dst.colorType() and "
        "dst.alphaType().")
    .def("encodeToData",
        (sk_sp<SkData> (SkImage::*)(SkEncodedImageFormat, int) const)
        &SkImage::encodeToData,
        "Encodes SkImage pixels, returning result as SkData.")
    .def("encodeToData",
        (sk_sp<SkData> (SkImage::*)() const) &SkImage::encodeToData,
        "Encodes SkImage pixels, returning result as SkData.")
    .def("refEncodedData", &SkImage::refEncodedData,
        "Returns encoded SkImage pixels as SkData, if SkImage was created from "
        "supported encoded stream format.")
    .def("makeSubset", &SkImage::makeSubset, "Returns subset of SkImage.")
    // .def("makeTextureImage", &SkImage::makeTextureImage,
    //     "Returns SkImage backed by GPU texture associated with context.")
    .def("makeNonTextureImage", &SkImage::makeNonTextureImage,
        "Returns raster image or lazy image.")
    .def("makeRasterImage", &SkImage::makeRasterImage, "Returns raster image.")
    // .def("makeWithFilter",
    //     (sk_sp<SkImage> (SkImage::*)(GrContext*, const SkImageFilter*,
    //         const SkIRect&, const SkIRect&, SkIRect*, SkIPoint*))
    //     &SkImage::makeWithFilter,
    //     "Creates filtered SkImage.")
    // .def("makeWithFilter",
    //     (sk_sp<SkImage> (SkImage::*)(const SkImageFilter*,
    //         const SkIRect&, const SkIRect&, SkIRect*, SkIPoint*))
    //     &SkImage::makeWithFilter,
    //     "To be deprecated.")
    .def("asLegacyBitmap", &SkImage::asLegacyBitmap,
        "Deprecated.")
    .def("isLazyGenerated", &SkImage::isLazyGenerated,
        "Returns true if SkImage is backed by an image-generator or other "
        "service that creates and caches its pixels or texture on-demand.")
    .def("makeColorSpace",
        [] (const SkImage& image, const SkColorSpace* target) {
            return image.makeColorSpace(CloneColorSpace(target));
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
        )docstring")
    .def("makeColorTypeAndColorSpace", &SkImage::makeColorTypeAndColorSpace,
        "Experimental.")
    .def("reinterpretColorSpace", &SkImage::reinterpretColorSpace,
        "Creates a new SkImage identical to this one, but with a different "
        "SkColorSpace.")
    .def_static("MakeRasterCopy", &SkImage::MakeRasterCopy,
        "Creates SkImage from SkPixmap and copy of pixels.")
    .def_static("MakeRasterData", &SkImage::MakeRasterData,
        "Creates SkImage from SkImageInfo, sharing pixels.")
    .def_static("MakeFromRaster", &SkImage::MakeFromRaster,
        "Creates SkImage from pixmap, sharing SkPixmap pixels.")
    .def_static("MakeFromBitmap", &SkImage::MakeFromBitmap,
        "Creates SkImage from bitmap, sharing or copying bitmap pixels.")
    // .def_static("MakeFromGenerator", &SkImage::MakeFromGenerator,
    //     "Creates SkImage from data returned by imageGenerator.")
    .def_static("MakeFromEncoded", &SkImage::MakeFromEncoded,
        "Return an image backed by the encoded data, but attempt to defer "
        "decoding until the image is actually used/drawn.",
        py::arg("encoded"), py::arg("subset") = nullptr)
    .def_static("DecodeToRaster",
        (sk_sp<SkImage> (*)(const void*, size_t, const SkIRect*))
        &SkImage::DecodeToRaster,
        "Decode the data in encoded/length into a raster image.",
        py::arg("encoded"), py::arg("length"), py::arg("subset") = nullptr)
    .def_static("DecodeToRaster",
        (sk_sp<SkImage> (*)(const sk_sp<SkData>&, const SkIRect*))
        &SkImage::DecodeToRaster,
        py::arg("data"), py::arg("subset") = nullptr)
    .def_static("DecodeToTexture",
        (sk_sp<SkImage> (*)(GrContext*, const void*, size_t, const SkIRect*))
        &SkImage::DecodeToTexture,
        "Decode the data in encoded/length into a texture-backed image.",
        py::arg("ctx"), py::arg("encoded"), py::arg("length"),
        py::arg("subset") = nullptr)
    .def_static("DecodeToTexture",
        (sk_sp<SkImage> (*)(GrContext*, const sk_sp<SkData>&, const SkIRect*))
        &SkImage::DecodeToTexture,
        py::arg("ctx"), py::arg("data"), py::arg("subset") = nullptr)
    // .def_static("MakeTextureFromCompressed",
    //     &SkImage::MakeTextureFromCompressed,
    //     "Creates a GPU-backed SkImage from compressed data.")
    // .def_static("MakeFromCompressed", &SkImage::MakeFromCompressed,
    //     "To be deprecated.")
    .def_static("MakeRasterFromCompressed", &SkImage::MakeRasterFromCompressed,
        "Creates a CPU-backed SkImage from compressed data.")
    .def_static("MakeFromTexture",
        (sk_sp<SkImage> (*)(GrContext*, const GrBackendTexture&,
            GrSurfaceOrigin, SkColorType, SkAlphaType, sk_sp<SkColorSpace>))
        &SkImage::MakeFromTexture,
        "Creates SkImage from GPU texture associated with context.")
    // .def_static("MakeFromTexture",
    //     (sk_sp<SkImage> (*)(GrContext*, const GrBackendTexture&,
    //         GrSurfaceOrigin, SkColorType, SkAlphaType, sk_sp<SkColorSpace>,
    //         TextureReleaseProc, ReleaseContext))
    //     &SkImage::MakeFromTexture,
    //     "Creates SkImage from GPU texture associated with context.")
    // .def_static("MakeFromCompressedTexture",
    //     &SkImage::MakeFromCompressedTexture,
    //     "Creates an SkImage from a GPU backend texture.")
    .def_static("MakeCrossContextFromPixmap",
        &SkImage::MakeCrossContextFromPixmap,
        "Creates SkImage from pixmap.")
    /*
    .def_static("MakeFromAdoptedTexture",
        &SkImage::MakeFromAdoptedTexture,
        "Creates SkImage from backendTexture associated with context.")
    .def_static("MakeFromYUVATexturesCopy", &SkImage::MakeFromYUVATexturesCopy,
        "Creates an SkImage by flattening the specified YUVA planes into a "
        "single, interleaved RGBA image.")
    .def_static("MakeFromYUVATexturesCopyWithExternalBackend",
        &SkImage::MakeFromYUVATexturesCopyWithExternalBackend,
        "Creates an SkImage by flattening the specified YUVA planes into a "
        "single, interleaved RGBA image.")
    .def_static("MakeFromYUVATextures", &SkImage::MakeFromYUVATextures,
        "Creates an SkImage by storing the specified YUVA planes into an image,"
        " to be rendered via multitexturing.")
    .def_static("MakeFromYUVAPixmaps", &SkImage::MakeFromYUVAPixmaps,
        "Creates SkImage from pixmap array representing YUVA data.")
    .def_static("MakeFromYUVTexturesCopy", &SkImage::MakeFromYUVTexturesCopy,
        "To be deprecated.")
    .def_static("MakeFromYUVTexturesCopyWithExternalBackend",
        &SkImage::MakeFromYUVTexturesCopyWithExternalBackend,
        "To be deprecated.")
    .def_static("MakeFromNV12TexturesCopy", &SkImage::MakeFromNV12TexturesCopy,
        "Creates SkImage from copy of nv12Textures, an array of textures on "
        "GPU.")
    .def_static("MakeFromNV12TexturesCopyWithExternalBackend",
        &SkImage::MakeFromNV12TexturesCopyWithExternalBackend,
        "Creates SkImage from copy of nv12Textures, an array of textures on "
        "GPU.")
    */
    .def_static("MakeFromPicture", &SkImage::MakeFromPicture,
        "Creates SkImage from picture.")
    // .def_static("MakeBackendTextureFromSkImage",
    //     &SkImage::MakeBackendTextureFromSkImage,
    //     "Creates a GrBackendTexture from the provided SkImage.")
    ;
}
