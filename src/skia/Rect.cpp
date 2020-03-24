#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initRect(py::module &m) {
    py::class_<SkRect>(m, "Rect")
        ;
}
