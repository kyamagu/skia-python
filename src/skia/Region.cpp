#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initRegion(py::module &m) {
    py::class_<SkRegion>(m, "Region")
        ;
}
