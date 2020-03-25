#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <skia.h>

namespace py = pybind11;

void initRect(py::module &m) {
py::class_<SkRect>(m, "Rect")
    .def("isEmpty", &SkRect::isEmpty,
        "Returns true if fLeft is equal to or greater than fRight, or if "
        "fTop is equal to or greater than fBottom.")
    .def("isSorted", &SkRect::isSorted,
        "Returns true if fLeft is equal to or less than fRight, or if fTop is "
        "equal to or less than fBottom.")
    .def("isFinite", &SkRect::isFinite,
        "Returns true if all values in the rectangle are finite: SK_ScalarMin "
        "or larger, and SK_ScalarMax or smaller.")
    .def("x", &SkRect::x, "Returns left edge of SkRect, if sorted.")
    .def("y", &SkRect::y, "Returns top edge of SkRect, if sorted.")
    .def("left", &SkRect::left, "Returns left edge of SkRect, if sorted.")
    .def("top", &SkRect::top, "Returns top edge of SkRect, if sorted.")
    .def("right", &SkRect::right, "Returns right edge of SkRect, if sorted.")
    .def("bottom", &SkRect::bottom,
        "Returns bottom edge of SkRect, if sorted.")
    .def("width", &SkRect::width, "Returns span on the x-axis.")
    .def("height", &SkRect::height, "Returns span on the y-axis.")
    .def("centerX", &SkRect::centerX,
        "Returns average of left edge and right edge.")
    .def("centerY", &SkRect::centerY,
        "Returns average of top edge and bottom edge.")
    .def("setEmpty", &SkRect::setEmpty, "Sets SkRect to (0, 0, 0, 0).")
    .def("set", py::overload_cast<const SkIRect&>(&SkRect::set),
        "Sets SkRect to src, promoting src members from integer to scalar.")
    .def("setLTRB", &SkRect::setLTRB,
        "Sets SkRect to (left, top, right, bottom).")
    .def("setBounds", &SkRect::setBounds,
        "Sets to bounds of SkPoint array with count entries.")
    .def("setBoundsNoCheck", &SkRect::setBoundsNoCheck,
        "Sets to bounds of SkPoint pts array with count entries.")
    .def("set", py::overload_cast<const SkPoint&, const SkPoint&>(&SkRect::set),
        "Sets bounds to the smallest SkRect enclosing SkPoint p0 and p1.")
    .def("setXYWH", &SkRect::setXYWH,
        "Sets SkRect to: (x, y, x + width, y + height).")
    .def("setWH", &SkRect::setWH, "Sets SkRect to (0, 0, width, height).")
    .def("setIWH", &SkRect::setIWH)
    .def("makeOffset",
        (SkRect (SkRect::*)(SkScalar, SkScalar) const) &SkRect::makeOffset,
        "Returns SkRect offset by (dx, dy).")
    .def("makeOffset",
        (SkRect (SkRect::*)(SkVector) const) &SkRect::makeOffset,
        "Returns SkRect offset by v.")
    .def("makeInset", &SkRect::makeInset, "Returns SkRect, inset by (dx, dy).")
    .def("makeOutset", &SkRect::makeOutset,
        "Returns SkRect, outset by (dx, dy).")
    .def("offset", py::overload_cast<SkScalar, SkScalar>(&SkRect::offset),
        "Offsets SkRect by adding dx to fLeft, fRight; and by adding dy to "
        "fTop, fBottom.")
    .def("offset", py::overload_cast<const SkPoint&>(&SkRect::offset),
        "Offsets SkRect by adding delta.fX to fLeft, fRight; and by adding "
        "delta.fY to fTop, fBottom.")
    .def("offsetTo", &SkRect::offsetTo,
        "Offsets SkRect so that fLeft equals newX, and fTop equals newY.")
    .def("inset", &SkRect::inset, "Insets SkRect by (dx,dy).")
    .def("outset", &SkRect::outset, "Outsets SkRect by (dx, dy).")
    .def("intersect",
        (bool (SkRect::*)(const SkRect&)) &SkRect::intersect,
        "Returns true if SkRect intersects r, and sets SkRect to "
        "intersection.")
    .def("intersect",
        (bool (SkRect::*)(const SkRect&, const SkRect&)) &SkRect::intersect,
        "Returns true if a intersects b, and sets SkRect to intersection.")
    .def("intersects", &SkRect::intersects,
        "Returns true if SkRect intersects r.")
    .def("join", &SkRect::join, "Sets SkRect to the union of itself and r.")
    .def("joinNonEmptyArg", &SkRect::joinNonEmptyArg,
        "Sets SkRect to the union of itself and r.")
    .def("joinPossiblyEmptyRect", &SkRect::joinPossiblyEmptyRect,
        "Sets SkRect to the union of itself and the construction.")
    .def("contains",
        (bool (SkRect::*)(SkScalar, SkScalar) const) &SkRect::contains,
        "Returns true if: fLeft <= x < fRight && fTop <= y < fBottom.")
    .def("contains",
        (bool (SkRect::*)(const SkRect&) const) &SkRect::contains,
        "Returns true if SkRect contains r.")
    .def("contains",
        (bool (SkRect::*)(const SkIRect&) const) &SkRect::contains,
        "Returns true if SkRect contains r. ")
    .def("round", py::overload_cast<SkIRect*>(&SkRect::round, py::const_),
        "Sets SkIRect by adding 0.5 and discarding the fractional portion of "
        "SkRect members, using (SkScalarRoundToInt(fLeft), "
        "SkScalarRoundToInt(fTop), SkScalarRoundToInt(fRight), "
        "SkScalarRoundToInt(fBottom)).")
    .def("roundOut", py::overload_cast<SkIRect*>(&SkRect::roundOut, py::const_),
        "Sets SkIRect by discarding the fractional portion of fLeft and fTop; "
        "and rounding up fRight and fBottom, using (SkScalarFloorToInt(fLeft), "
        "SkScalarFloorToInt(fTop), SkScalarCeilToInt(fRight), "
        "SkScalarCeilToInt(fBottom)).")
    .def("roundOut", py::overload_cast<SkRect*>(&SkRect::roundOut, py::const_),
        "Sets SkRect by discarding the fractional portion of fLeft and fTop; "
        "and rounding up fRight and fBottom, using (SkScalarFloorToInt(fLeft), "
        "SkScalarFloorToInt(fTop), SkScalarCeilToInt(fRight), "
        "SkScalarCeilToInt(fBottom)).")
    .def("roundIn", &SkRect::roundIn,
        "Sets SkRect by rounding up fLeft and fTop; and discarding the "
        "fractional portion of fRight and fBottom, using "
        "(SkScalarCeilToInt(fLeft), SkScalarCeilToInt(fTop), "
        "SkScalarFloorToInt(fRight), SkScalarFloorToInt(fBottom)).")
    .def("round", py::overload_cast<>(&SkRect::round, py::const_),
        "Returns SkIRect by adding 0.5 and discarding the fractional portion "
        "of SkRect members, using (SkScalarRoundToInt(fLeft), "
        "SkScalarRoundToInt(fTop), SkScalarRoundToInt(fRight), "
        "SkScalarRoundToInt(fBottom)).")
    .def("roundOut", py::overload_cast<>(&SkRect::roundOut, py::const_),
        "Sets SkIRect by discarding the fractional portion of fLeft and fTop; "
        "and rounding up fRight and fBottom, using (SkScalarFloorToInt(fLeft), "
        "SkScalarFloorToInt(fTop), SkScalarCeilToInt(fRight), "
        "SkScalarCeilToInt(fBottom)).")
    .def("sort", &SkRect::sort,
        "Swaps fLeft and fRight if fLeft is greater than fRight; and swaps "
        "fTop and fBottom if fTop is greater than fBottom.")
    .def("makeSorted", &SkRect::makeSorted,
        "Returns SkRect with fLeft and fRight swapped if fLeft is greater "
        "than fRight; and with fTop and fBottom swapped if fTop is greater "
        "than fBottom.")
    .def("asScalars", &SkRect::asScalars,
        "Returns pointer to first scalar in SkRect, to treat it as an array "
        "with four entries.",
        py::return_value_policy::reference)
    .def("dump", py::overload_cast<bool>(&SkRect::dump, py::const_),
        "Writes text representation of SkRect to standard output.")
    .def("dump", py::overload_cast<>(&SkRect::dump, py::const_),
        "Writes text representation of SkRect to standard output.")
    .def("dumpHex", &SkRect::dumpHex,
        "Writes text representation of SkRect to standard output.")
    .def_static("MakeEmpty", &SkRect::MakeEmpty,
        "Returns constructed SkRect set to (0, 0, 0, 0).")
    .def_static("MakeWH", &SkRect::MakeWH,
        "Returns constructed SkRect set to SkScalar values (0, 0, w, h).")
    .def_static("MakeIWH", &SkRect::MakeIWH,
        "Returns constructed SkRect set to integer values (0, 0, w, h).")
    .def_static("MakeSize", &SkRect::MakeSize,
        "Returns constructed SkRect set to (0, 0, size.width(), "
        "size.height()).")
    .def_static("MakeLTRB", &SkRect::MakeLTRB,
        "Returns constructed SkRect set to (l, t, r, b).")
    .def_static("MakeXYWH", &SkRect::MakeXYWH,
        "Returns constructed SkRect set to: (x, y, x + w, y + h).")
    .def_static("Make", py::overload_cast<const SkISize&>(&SkRect::Make),
        "Returns constructed SkIRect set to (0, 0, size.width(), "
        "size.height()).")
    .def_static("Make", py::overload_cast<const SkIRect&>(&SkRect::Make),
        "Returns constructed SkIRect set to irect, promoting integers to "
        "scalar.")
    .def_static("Intersects",
        py::overload_cast<const SkRect&, const SkRect&>(&SkRect::Intersects),
        "Returns true if a intersects b.")
    .def_readwrite("fLeft", &SkRect::fLeft, "smaller x-axis bounds")
    .def_readwrite("fTop", &SkRect::fTop, "smaller y-axis bounds")
    .def_readwrite("fRight", &SkRect::fRight, "larger x-axis bounds")
    .def_readwrite("fBottom", &SkRect::fBottom, "larger y-axis bounds")
    .def(py::self == py::self,
        "Returns true if all members in a: fLeft, fTop, fRight, and fBottom; "
        "are identical to corresponding members in b.")
    .def(py::self != py::self,
        "Returns true if any member in a: fLeft, fTop, fRight, and fBottom; is "
        "not identical to the corresponding member in b.")
    ;
}
