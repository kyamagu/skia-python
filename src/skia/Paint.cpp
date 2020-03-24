#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initPaint(py::module &m) {
py::class_<SkPaint>(m, "Paint")
    .def(py::init<>())
    .def("setAntiAlias", &SkPaint::setAntiAlias,
        "Requests, but does not require, that edge pixels draw opaque or "
        "with partial transparency.")
    ;
}
