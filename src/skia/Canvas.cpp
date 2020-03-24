#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initCanvas(py::module &m) {
py::class_<SkCanvas>(m, "Canvas")
    .def("drawPath", &SkCanvas::drawPath,
        "Draws SkPath path using clip, SkMatrix, and SkPaint paint.")
    .def("translate", &SkCanvas::translate,
        "Translates SkMatrix by dx along the x-axis and dy along the y-axis.")
    .def("clear", &SkCanvas::clear,
        "Fills clip with color using SkBlendMode::kSrc")
    ;
}
