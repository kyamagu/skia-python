#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initGrContext(py::module &m) {
py::class_<GrContext>(m, "GrContext")
    ;
}
