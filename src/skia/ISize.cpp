#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initISize(py::module &m) {
py::class_<SkISize>(m, "ISize")
    .def("set", &SkISize::set)
    .def("isZero", &SkISize::isZero,
        "Returns true iff fWidth == 0 && fHeight == 0.")
    .def("isEmpty", &SkISize::isEmpty,
        "Returns true if either width or height are <= 0.")
    .def("setEmpty", &SkISize::setEmpty, "Set the width and height to 0.")
    .def("width", &SkISize::width)
    .def("height", &SkISize::height)
    .def("area", &SkISize::area)
    .def("equals", &SkISize::equals)
    .def_static("Make", &SkISize::Make)
    .def_static("MakeEmpty", &SkISize::MakeEmpty)
    .def_readwrite("fWidth", &SkISize::fWidth)
    .def_readwrite("fHeight", &SkISize::fHeight)
    ;
}
