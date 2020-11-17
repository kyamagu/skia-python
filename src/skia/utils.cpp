#include "common.h"
#include <pybind11/numpy.h>


template <>
sk_sp<SkShader> CloneFlattenable(const SkShader& shader) {
    auto data = shader.serialize();
    auto flat = SkShader::Deserialize(
        shader.getFlattenableType(), data->data(), data->size());
    return sk_sp<SkShader>(reinterpret_cast<SkShader*>(flat.release()));
}

template <>
sk_sp<SkColorFilter> CloneFlattenable(const SkColorFilter& colorFilter) {
    auto data = colorFilter.serialize();
    auto flat = SkColorFilter::Deserialize(
        colorFilter.getFlattenableType(), data->data(), data->size());
    return sk_sp<SkColorFilter>(
        reinterpret_cast<SkColorFilter*>(flat.release()));
}

sk_sp<SkColorSpace> CloneColorSpace(const SkColorSpace* cs) {
    return (cs) ? CloneFlattenable(*cs) : sk_sp<SkColorSpace>(nullptr);
}


sk_sp<SkImage> CloneImage(const SkImage& image) {
    SkPixmap pixmap;
    if (image.peekPixels(&pixmap))
        return SkImage::MakeRasterCopy(pixmap);
    return SkImage::MakeFromEncoded(image.encodeToData());
}

size_t ValidateBufferToImageInfo(
    const SkImageInfo& imageInfo,
    const py::buffer_info& buffer,
    size_t rowBytes) {
    if (buffer.ndim == 0)
        throw py::value_error("Buffer does not have dimensions.");
    if (buffer.ndim == 1 && rowBytes == 0)
        rowBytes = imageInfo.minRowBytes();
    else if (buffer.ndim > 1)
        rowBytes = buffer.strides[0];

    if (!imageInfo.validRowBytes(rowBytes))
        throw py::value_error(py::str(
            "Row bytes is smaller than required (expected {}, given {})"
            ).format(imageInfo.minRowBytes(), rowBytes));

    size_t byteSize = buffer.strides[0] * buffer.shape[0];
    if (byteSize < imageInfo.computeByteSize(rowBytes))
        throw py::value_error(py::str(
            "Byte size is smaller than required (expected {}, given {})"
            ).format(imageInfo.computeByteSize(rowBytes), byteSize));
    return rowBytes;
}

SkImageInfo NumPyToImageInfo(py::array array, SkColorType ct, SkAlphaType at,
                             const SkColorSpace* cs) {
    if (!(array.flags() & py::array::c_style))
        throw py::value_error("Array must be C-style contiguous.");
    if (array.ndim() <= 1)
        throw py::value_error("Number of dimensions must be 2 or more.");
    if (array.shape(0) == 0 || array.shape(1) == 0)
        throw py::value_error(py::str(
            "Width and height must be greater than 0. "
            "(width={}, height={})").format(array.shape(1), array.shape(0)));
    // TODO: automatic colortype inference.
    auto imageInfo = SkImageInfo::Make(
        array.shape(1), array.shape(0), ct, at, CloneColorSpace(cs));
    auto pixelSize = (array.ndim() == 2) ?
        array.strides(1) : array.strides(2) * array.shape(2);
    if (pixelSize != imageInfo.bytesPerPixel())
        throw py::value_error(py::str(
            "Incorrect number of color channels (expected {} bytes per pixel, "
            "given {} bytes per pixel).").format(
            imageInfo.bytesPerPixel(), pixelSize));
    return imageInfo;
}

py::buffer_info ImageInfoToBufferInfo(
    const SkImageInfo& imageInfo, void* data, ssize_t rowBytes, bool readonly) {
    ssize_t width = imageInfo.width();
    ssize_t height = imageInfo.height();
    ssize_t bytesPerPixel = imageInfo.bytesPerPixel();
    if (!rowBytes)
        rowBytes = imageInfo.minRowBytes();
    switch (imageInfo.colorType()) {
        case kAlpha_8_SkColorType:
        case kGray_8_SkColorType:
            return py::buffer_info(
                data, bytesPerPixel, "B", 2, { height, width },
                { rowBytes, bytesPerPixel }, readonly);

        case kRGB_565_SkColorType:
        case kARGB_4444_SkColorType:
            return py::buffer_info(
                data, bytesPerPixel, "H", 2, { height, width },
                { rowBytes, bytesPerPixel }, readonly);

        case kRGBA_8888_SkColorType:
        case kRGB_888x_SkColorType:
        case kBGRA_8888_SkColorType:
            return py::buffer_info(
                data, 1, "B", 3, { height, width, ssize_t(4) },
                { rowBytes, bytesPerPixel, ssize_t(1) }, readonly);

        case kRGBA_1010102_SkColorType:
        case kBGRA_1010102_SkColorType:
        case kRGB_101010x_SkColorType:
        case kBGR_101010x_SkColorType:
            return py::buffer_info(
                data, bytesPerPixel, "I", 2, { height, width },
                { rowBytes, bytesPerPixel }, readonly);

        case kRGBA_F16Norm_SkColorType:
        case kRGBA_F16_SkColorType:
            return py::buffer_info(
                data, 2, "e", 3, { height, width, ssize_t(4) },
                { rowBytes, bytesPerPixel, ssize_t(2) }, readonly);

        case kRGBA_F32_SkColorType:
            return py::buffer_info(
                data, 4, "f", 3, { height, width, ssize_t(4) },
                { rowBytes, bytesPerPixel, ssize_t(4) }, readonly);

        case kR8G8_unorm_SkColorType:
            return py::buffer_info(
                data, 1, "B", 3, { height, width, ssize_t(2) },
                { rowBytes, bytesPerPixel, ssize_t(1) }, readonly);

        case kA16_float_SkColorType:
            return py::buffer_info(
                data, 2, "e", 2, { height, width },
                { rowBytes, bytesPerPixel }, readonly);

        case kR16G16_float_SkColorType:
            return py::buffer_info(
                data, 2, "e", 3, { height, width, ssize_t(2) },
                { rowBytes, bytesPerPixel, ssize_t(2) }, readonly);

        case kA16_unorm_SkColorType:
            return py::buffer_info(
                data, 2, "<H", 2, { height, width },
                { rowBytes, bytesPerPixel }, readonly);

        case kR16G16_unorm_SkColorType:
            return py::buffer_info(
                data, 2, "<H", 3, { height, width, ssize_t(2) },
                { rowBytes, bytesPerPixel, ssize_t(2) }, readonly);

        case kR16G16B16A16_unorm_SkColorType:
            return py::buffer_info(
                data, 2, "<H", 3, { height, width, ssize_t(4) },
                { rowBytes, bytesPerPixel, ssize_t(2) }, readonly);

        case kUnknown_SkColorType:
        default:
            throw std::runtime_error("Unsupported color type");
    }
}

py::memoryview ImageInfoToMemoryView(
    const SkImageInfo& imageInfo, void* data, ssize_t rowBytes, bool readonly) {
    ssize_t width = imageInfo.width();
    ssize_t height = imageInfo.height();
    ssize_t bytesPerPixel = imageInfo.bytesPerPixel();
    if (!rowBytes)
        rowBytes = imageInfo.minRowBytes();
    switch (imageInfo.colorType()) {
        case kAlpha_8_SkColorType:
        case kGray_8_SkColorType:
            return py::memoryview::from_buffer(
                data, bytesPerPixel, "B", { height, width },
                { rowBytes, bytesPerPixel }, readonly);

        case kRGB_565_SkColorType:
        case kARGB_4444_SkColorType:
            return py::memoryview::from_buffer(
                data, bytesPerPixel, "H", { height, width },
                { rowBytes, bytesPerPixel }, readonly);

        case kRGBA_8888_SkColorType:
        case kRGB_888x_SkColorType:
        case kBGRA_8888_SkColorType:
            return py::memoryview::from_buffer(
                data, 1, "B", { height, width, ssize_t(4) },
                { rowBytes, bytesPerPixel, ssize_t(1) }, readonly);

        case kRGBA_1010102_SkColorType:
        case kBGRA_1010102_SkColorType:
        case kRGB_101010x_SkColorType:
        case kBGR_101010x_SkColorType:
            return py::memoryview::from_buffer(
                data, bytesPerPixel, "I", { height, width },
                { rowBytes, bytesPerPixel }, readonly);

        case kRGBA_F16Norm_SkColorType:
        case kRGBA_F16_SkColorType:
            return py::memoryview::from_buffer(
                data, 2, "e", { height, width, ssize_t(4) },
                { rowBytes, bytesPerPixel, ssize_t(2) }, readonly);

        case kRGBA_F32_SkColorType:
            return py::memoryview::from_buffer(
                data, 4, "f", { height, width, ssize_t(4) },
                { rowBytes, bytesPerPixel, ssize_t(4) }, readonly);

        case kR8G8_unorm_SkColorType:
            return py::memoryview::from_buffer(
                data, 1, "B", { height, width, ssize_t(2) },
                { rowBytes, bytesPerPixel, ssize_t(1) }, readonly);

        case kA16_float_SkColorType:
            return py::memoryview::from_buffer(
                data, 2, "e", { height, width },
                { rowBytes, bytesPerPixel }, readonly);

        case kR16G16_float_SkColorType:
            return py::memoryview::from_buffer(
                data, 2, "e", { height, width, ssize_t(2) },
                { rowBytes, bytesPerPixel, ssize_t(2) }, readonly);

        case kA16_unorm_SkColorType:
            return py::memoryview::from_buffer(
                data, 2, "<H", { height, width },
                { rowBytes, bytesPerPixel }, readonly);

        case kR16G16_unorm_SkColorType:
            return py::memoryview::from_buffer(
                data, 2, "<H", { height, width, ssize_t(2) },
                { rowBytes, bytesPerPixel, ssize_t(2) }, readonly);

        case kR16G16B16A16_unorm_SkColorType:
            return py::memoryview::from_buffer(
                data, 2, "<H", { height, width, ssize_t(4) },
                { rowBytes, bytesPerPixel, ssize_t(2) }, readonly);

        case kUnknown_SkColorType:
        default:
            throw std::runtime_error("Unsupported color type");
    }
}


py::dict ImageInfoToArrayInterface(
    const SkImageInfo& imageInfo, size_t rowBytes) {
    using namespace pybind11::literals;
    auto byteorder = py::module::import("sys")
        .attr("byteorder").cast<std::string>();
    std::string bytemark((byteorder == "little") ? "<" : ">");
    int width = imageInfo.width();
    int height = imageInfo.height();
    int bytesPerPixel = imageInfo.bytesPerPixel();
    if (!rowBytes)
        rowBytes = imageInfo.minRowBytes();
    switch (imageInfo.colorType()) {
        case kAlpha_8_SkColorType:
        case kGray_8_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width),
                "typestr"_a = py::str("|u1"),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel),
                "version"_a = 3
            );

        case kRGB_565_SkColorType:
        case kARGB_4444_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width),
                "typestr"_a = py::str("{}u2").format(bytemark),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel),
                "version"_a = 3
            );

        case kRGBA_8888_SkColorType:
        case kRGB_888x_SkColorType:
        case kBGRA_8888_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width, 4),
                "typestr"_a = py::str("|u1"),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel, 1),
                "version"_a = 3
            );

        case kRGBA_1010102_SkColorType:
        case kBGRA_1010102_SkColorType:
        case kRGB_101010x_SkColorType:
        case kBGR_101010x_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width),
                "typestr"_a = py::str("{}u4").format(bytemark),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel),
                "version"_a = 3
            );

        case kRGBA_F16Norm_SkColorType:
        case kRGBA_F16_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width, 4),
                "typestr"_a = py::str("{}f2").format(bytemark),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel, 2),
                "version"_a = 3
            );

        case kRGBA_F32_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width, 4),
                "typestr"_a = py::str("{}f4").format(bytemark),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel, 4),
                "version"_a = 3
            );

        case kR8G8_unorm_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width, 2),
                "typestr"_a = py::str("|u1"),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel, 1),
                "version"_a = 3
            );

        case kA16_float_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width),
                "typestr"_a = py::str("{}f2").format(bytemark),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel),
                "version"_a = 3
            );

        case kR16G16_float_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width, 2),
                "typestr"_a = py::str("{}f2").format(bytemark),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel, 2),
                "version"_a = 3
            );

        case kA16_unorm_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width),
                "typestr"_a = py::str("<u2"),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel),
                "version"_a = 3
            );

        case kR16G16_unorm_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width, 2),
                "typestr"_a = py::str("<u2"),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel, 2),
                "version"_a = 3
            );

        case kR16G16B16A16_unorm_SkColorType:
            return py::dict(
                "shape"_a = py::make_tuple(height, width, 4),
                "typestr"_a = py::str("<f2"),
                "strides"_a = py::make_tuple(rowBytes, bytesPerPixel, 2),
                "version"_a = 3
            );

        case kUnknown_SkColorType:
        default:
            throw std::runtime_error("Unsupported color type");
    }
}
