#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <skia.h>

namespace py = pybind11;

void initIPoint(py::module &m) {
py::class_<SkIPoint>(m, "IPoint")
    .def("x", &SkIPoint::x, "Returns x-axis value of SkIPoint.")
    .def("y", &SkIPoint::y, "Returns y-axis value of SkIPoint.")
    .def("isZero", &SkIPoint::isZero,
        "Returns true if fX and fY are both zero.")
    .def("set", &SkIPoint::set, "Sets fX to x and fY to y.")
    .def(-py::self, "Returns SkIPoint changing the signs of fX and fY.")
    .def("__iadd__", &SkIPoint::operator+=, "Offsets SkIPoint by ivector v.",
        py::is_operator())
    .def("__isub__", &SkIPoint::operator-=,
        "Subtracts ivector v from SkIPoint.", py::is_operator())
    .def("equals", &SkIPoint::equals,
        "Returns true if SkIPoint is equivalent to SkIPoint constructed from "
        "(x, y).")
    .def_static("Make", &SkIPoint::Make, "Sets fX to x, fY to y.")
    .def_readwrite("fX", &SkIPoint::fX, "x-axis value")
    .def_readwrite("fY", &SkIPoint::fY, "y-axis value")
    .def(py::self == py::self, "Returns true if a is equivalent to b.")
    .def(py::self != py::self, "Returns true if a is not equivalent to b.")
    .def(py::self - py::self,
        "Returns ivector from b to a; computed as (a.fX - b.fX, a.fY - b.fY).")
    .def("__add__", [](const SkIPoint& a, const SkIVector& b) {
        return a + b;
    },
    "Returns SkIPoint resulting from SkIPoint a offset by ivector b, "
    "computed as: (a.fX + b.fX, a.fY + b.fY).", py::is_operator())
    .def("__radd__", [](const SkIPoint& a, const SkIVector& b) {
        return b + a;
    },
    "Returns SkIPoint resulting from SkIPoint a offset by ivector b, "
    "computed as: (b.fX + a.fX, b.fY + a.fY).", py::is_operator())
    ;
}
