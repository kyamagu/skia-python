#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

void Image(py::module &m) {
py::class_<SkImage, sk_sp<SkImage>>(m, "Image")
    .def("encodeToData", [](const SkImage& image) {
        sk_sp<SkData> data(image.encodeToData());
        if (!data) {
            throw std::runtime_error("Encoding failed");
        }
        return data;
    },
        "Encodes SkImage pixels, returning result as SkData.")
    .def("imageInfo", &SkImage::imageInfo,
        "Returns a SkImageInfo describing the width, height, color type, alpha "
        "type, and color space of the SkImage.")
    .def("width", &SkImage::width, "Returns pixel count in each row.")
    .def("height", &SkImage::height, "Returns pixel row count.")
    .def("dimensions", &SkImage::dimensions,
        "Returns SkISize { width(), height() }.")
    .def("bounds", &SkImage::bounds,
        "Returns SkIRect { 0, 0, width(), height() }.")
    .def("isAlphaOnly", &SkImage::isAlphaOnly,
        "Returns true if SkImage pixels represent transparency only.")
    .def("isLazyGenerated", &SkImage::isLazyGenerated,
        "Returns true if SkImage is backed by an image-generator or other "
        "service that creates and caches its pixels or texture on-demand.")
    .def("isOpaque", &SkImage::isOpaque,
        "Returns true if pixels ignore their alpha value and are treated as "
        "fully opaque.")
    .def("isTextureBacked", &SkImage::isTextureBacked,
        "Returns true the contents of SkImage was created on or uploaded to "
        "GPU memory, and is available as a GPU texture.")
    ;
}
