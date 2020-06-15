#include "common.h"
#include <pybind11/numpy.h>


template <>
sk_sp<SkShader> CloneFlattenable(const SkShader& shader) {
    auto data = shader.serialize();
    auto flat = SkShader::Deserialize(
        shader.getFlattenableType(), data->data(), data->size());
    return sk_sp<SkShader>(reinterpret_cast<SkShader*>(flat.release()));
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

    if (rowBytes < imageInfo.minRowBytes())
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
