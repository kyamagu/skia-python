#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void Path(py::module &m) {
py::class_<SkPath>(m, "Path")
    .def(py::init<>())
    .def("moveTo",
        py::overload_cast<SkScalar, SkScalar>(&SkPath::moveTo),
        "Adds beginning of contour at SkPoint (x, y).")
    .def("lineTo",
        py::overload_cast<SkScalar, SkScalar>(&SkPath::lineTo),
        "Adds line from last point to (x, y).")
    .def("close", &SkPath::close,
        "Appends kClose_Verb to SkPath.")
    ;
}
