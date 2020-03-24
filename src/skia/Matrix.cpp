#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initMatrix(py::module &m) {
py::class_<SkMatrix>(m, "Matrix")
    ;
py::class_<SkM44>(m, "M44")
    ;
py::class_<SkRSXform>(m, "RSXform")
    ;
}
