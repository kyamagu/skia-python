#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <skia.h>

namespace py = pybind11;

void initPoint(py::module &m) {
// IPoint
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
// Point
py::class_<SkPoint>(m, "Point")
    .def("x", &SkPoint::x, "Returns x-axis value of SkPoint or vector.")
    .def("y", &SkPoint::y, "Returns y-axis value of SkPoint or vector.")
    .def("isZero", &SkPoint::isZero,
        "Returns true if fX and fY are both zero.")
    .def("set", &SkPoint::set, "Sets fX to x and fY to y.")
    .def("iset", py::overload_cast<int32_t, int32_t>(&SkPoint::iset),
        "Sets fX to x and fY to y, promoting integers to SkScalar values.")
    .def("iset", py::overload_cast<const SkIPoint&>(&SkPoint::iset),
        "Sets fX to p.fX and fY to p.fY, promoting integers to SkScalar "
        "values.")
    .def("setAbs", &SkPoint::setAbs,
        "Sets fX to absolute value of pt.fX; and fY to absolute value of "
        "pt.fY.")
    .def("offset", &SkPoint::offset, "Adds offset (dx, dy) to SkPoint.")
    .def("length", &SkPoint::length,
        "Returns the Euclidean distance from origin, computed as: "
        "sqrt(x * x + y * y)")
    .def("distanceToOrigin", &SkPoint::distanceToOrigin,
        "Returns the Euclidean distance from origin, computed as: "
        "sqrt(fX * fX + fY * fY)")
    .def("normalize", &SkPoint::normalize,
        "Scales (fX, fY) so that length() returns one, while preserving "
        "ratio of fX to fY, if possible.")
    .def("setNormalize", &SkPoint::setNormalize,
        "Sets vector to (x, y) scaled so length() returns one, and so that "
        "(fX, fY) is proportional to (x, y).")
    .def("setLength", py::overload_cast<SkScalar>(&SkPoint::setLength),
        "Scales vector so that distanceToOrigin() returns length, if possible.")
    .def("setLength",
        py::overload_cast<SkScalar, SkScalar, SkScalar>(&SkPoint::setLength),
        "Sets vector to (x, y) scaled to length, if possible.")
    .def("scale",
        py::overload_cast<SkScalar, SkPoint*>(&SkPoint::scale, py::const_),
        "Sets dst to SkPoint times scale.")
    .def("scale", py::overload_cast<SkScalar>(&SkPoint::scale),
        "Scales SkPoint in place by scale.")
    .def("negate", &SkPoint::negate, "Changes the sign of fX and fY.")
    .def(-py::self, "Returns SkPoint changing the signs of fX and fY.")
    .def("__iadd__", &SkPoint::operator+=, "Adds vector v to SkPoint.",
        py::is_operator())
    .def("__isub__", &SkPoint::operator-=, "Subtracts vector v from SkPoint.",
        py::is_operator())
    .def(py::self * SkScalar(), "Returns SkPoint multiplied by scale.")
    .def(py::self *= SkScalar(), "Multiplies SkPoint by scale.")
    .def("isFinite", &SkPoint::isFinite,
        "Returns true if both fX and fY are measurable values.")
    .def("equals", &SkPoint::equals,
        "Returns true if SkPoint is equivalent to SkPoint constructed from "
        "(x, y).")
    .def("cross", &SkPoint::cross,
        "Returns the cross product of vector and vec.")
    .def("dot", &SkPoint::dot,
        "Returns the dot product of vector and vector vec.")
    .def_static("Make", &SkPoint::Make, "Sets fX to x, fY to y.")
    .def_static("Offset", py::overload_cast<SkPoint[], int, const SkVector&>(
        &SkPoint::Offset),
        "Adds offset to each SkPoint in points array with count entries.")
    .def_static("Offset", py::overload_cast<SkPoint[], int, SkScalar, SkScalar>(
        &SkPoint::Offset),
        "Adds offset (dx, dy) to each SkPoint in points array of length count.")
    .def_static("Length", &SkPoint::Length,
        "Returns the Euclidean distance from origin, computed as: "
        "sqrt(x * x + y * y)")
    .def_static("Normalize", &SkPoint::Normalize,
        "Scales (vec->fX, vec->fY) so that length() returns one, while "
        "preserving ratio of vec->fX to vec->fY, if possible.")
    .def_static("Distance", &SkPoint::Distance,
        "Returns the Euclidean distance between a and b.")
    .def_static("DotProduct", &SkPoint::DotProduct,
        "Returns the dot product of vector a and vector b.")
    .def_static("CrossProduct", &SkPoint::CrossProduct,
        "Returns the cross product of vector a and vector b.")
    .def_readwrite("fX", &SkPoint::fX, "x-axis value")
    .def_readwrite("fY", &SkPoint::fY, "y-axis value")
    .def(py::self == py::self, "Returns true if a is equivalent to b.")
    .def(py::self != py::self, "Returns true if a is not equivalent to b.")
    .def(py::self - py::self,
        "Returns vector from b to a; computed as (a.fX - b.fX, a.fY - b.fY).")
    .def("__add__", [](const SkPoint& a, const SkVector& b) {
        return a + b;
    },
    "Returns SkPoint resulting from SkPoint a offset by vector b, "
    "computed as: (a.fX + b.fX, a.fY + b.fY).", py::is_operator())
    .def("__radd__", [](const SkPoint& a, const SkVector& b) {
        return b + a;
    },
    "Returns SkPoint resulting from SkPoint a offset by vector b, "
    "computed as: (b.fX + a.fX, b.fY + a.fY).", py::is_operator())
    ;
// Point3
py::class_<SkPoint3>(m, "Point3")
    .def("x", &SkPoint3::x)
    .def("y", &SkPoint3::y)
    .def("z", &SkPoint3::z)
    .def("set", &SkPoint3::set)
    .def("length", &SkPoint3::length,
        "Return the Euclidian distance from (0,0,0) to the point.")
    .def("normalize", &SkPoint3::normalize,
        "Set the point (vector) to be unit-length in the same direction as it "
        "already points.")
    .def("makeScale", &SkPoint3::makeScale,
        "Return a new point whose X, Y and Z coordinates are scaled.")
    .def("scale", &SkPoint3::scale,
        "Scale the point's coordinates by scale.")
    .def(-py::self,
        "Return a new point whose X, Y and Z coordinates are the negative of "
        "the original point's.")
    .def("__iadd__", &SkPoint3::operator+=,
        "Add v's coordinates to the point's.", py::is_operator())
    .def("__isub__", &SkPoint3::operator-=,
        "Subtract v's coordinates from the point's.", py::is_operator())
    .def("isFinite", &SkPoint3::isFinite,
        "Returns true if fX, fY, and fZ are measurable values.")
    .def("dot", &SkPoint3::dot)
    .def("cross", &SkPoint3::cross)
    .def_static("Make", &SkPoint3::Make)
    .def_static("Length", &SkPoint3::Length,
        "Returns the Euclidian distance from (0,0,0) to (x,y,z)")
    .def_static("DotProduct", &SkPoint3::DotProduct,
        "Returns the dot product of a and b, treating them as 3D vectors.")
    .def_static("CrossProduct", &SkPoint3::CrossProduct,
        "Returns the cross product of a and b, treating them as 3D vectors.")
    .def_readwrite("fX", &SkPoint3::fX, "x-axis value")
    .def_readwrite("fY", &SkPoint3::fY, "y-axis value")
    .def_readwrite("fZ", &SkPoint3::fZ, "z-axis value")
    .def(py::self == py::self, "Returns true if a is equivalent to b.")
    .def(py::self != py::self, "Returns true if a is not equivalent to b.")
    .def(py::self - py::self,
        "Returns a new point whose coordinates are the difference between a "
        "and b (i.e., a - b)")
    .def(py::self + py::self,
        "Returns a new point whose coordinates are the sum of a and b (a + b)")
    .def(SkScalar() * py::self)
    ;
}
