#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initBlendMode(py::module &m) {
py::enum_<SkBlendMode>(m, "BlendMode");
py::enum_<SkBlendModeCoeff>(m, "BlendModeCoeff");
m.def("BlendMode_AsCoeff", &SkBlendMode_AsCoeff,
    "Returns true if 'mode' is a coefficient-based blend mode "
    "(<= kLastCoeffMode).");
m.def("BlendMode_Name", &SkBlendMode_Name,
    "Returns name of blendMode as null-terminated C string.");
}
