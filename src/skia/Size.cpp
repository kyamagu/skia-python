#include "common.h"

void initSize(py::module &m) {
// ISize
py::class_<SkISize>(m, "ISize")
    .def(py::init(&SkISize::MakeEmpty))
    .def(py::init(&SkISize::Make))
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

// Size
py::class_<SkSize>(m, "Size")
    .def(py::init(&SkSize::MakeEmpty))
    .def(py::init(py::overload_cast<SkScalar, SkScalar>(&SkSize::Make)))
    .def(py::init(py::overload_cast<const SkISize&>(&SkSize::Make)))
    .def("set", &SkSize::set)
    .def("isZero", &SkSize::isZero,
        "Returns true iff fWidth == 0 && fHeight == 0.")
    .def("isEmpty", &SkSize::isEmpty,
        "Returns true if either width or height are <= 0.")
    .def("setEmpty", &SkSize::setEmpty, "Set the width and height to 0.")
    .def("width", &SkSize::width)
    .def("height", &SkSize::height)
    .def("equals", &SkSize::equals)
    .def("toRound", &SkSize::toRound)
    .def("toCeil", &SkSize::toCeil)
    .def("toFloor", &SkSize::toFloor)
    .def_static("Make", py::overload_cast<SkScalar, SkScalar>(&SkSize::Make))
    .def_static("Make", py::overload_cast<const SkISize&>(&SkSize::Make))
    .def_static("MakeEmpty", &SkSize::MakeEmpty)
    .def_readwrite("fWidth", &SkSize::fWidth)
    .def_readwrite("fHeight", &SkSize::fHeight)
    ;
}
