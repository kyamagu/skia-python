#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void ImageInfo(py::module &m) {
py::class_<SkImageInfo>(m, "ImageInfo")
    .def(py::init<>())
    ;
}
