#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <skia.h>
#include <sstream>

namespace py = pybind11;

constexpr size_t SkRRect::kSizeInMemory;

void initRect(py::module &m) {
// IRect
py::class_<SkIRect>(m, "IRect")
    // Python additions.
    .def(py::init(&SkIRect::MakeEmpty))
    .def(py::init(&SkIRect::MakeWH))
    .def(py::init(&SkIRect::MakeLTRB))
    .def("__repr__", [](const SkIRect& r) {
        std::stringstream s;
        s << "IRect(" << r.fLeft << ", " << r.fTop << ", " <<
            r.fRight << ", " << r.fBottom << ")";
        return s.str();
    })
    // Wrappers.
    .def("left", &SkIRect::left, "Returns left edge of SkIRect, if sorted.")
    .def("top", &SkIRect::top, "Returns top edge of SkIRect, if sorted.")
    .def("right", &SkIRect::right, "Returns right edge of SkIRect, if sorted.")
    .def("bottom", &SkIRect::bottom,
        "Returns bottom edge of SkIRect, if sorted.")
    .def("x", &SkIRect::x, "Returns left edge of SkIRect, if sorted.")
    .def("y", &SkIRect::y, "Returns top edge of SkIRect, if sorted.")
    .def("topLeft", &SkIRect::topLeft)
    .def("width", &SkIRect::width, "Returns span on the x-axis.")
    .def("height", &SkIRect::height, "Returns span on the y-axis.")
    .def("size", &SkIRect::size, "Returns spans on the x-axis and y-axis.")
    .def("width64", &SkIRect::width64, "Returns span on the x-axis.")
    .def("height64", &SkIRect::height64, "Returns span on the y-axis.")
    .def("isEmpty64", &SkIRect::isEmpty64,
        "Returns true if fLeft is equal to or greater than fRight, or if fTop "
        "is equal to or greater than fBottom.")
    .def("isEmpty", &SkIRect::isEmpty,
        "Returns true if width() or height() are zero or negative.")
    .def("setEmpty", &SkIRect::setEmpty, "Sets SkIRect to (0, 0, 0, 0).")
    .def("setLTRB", &SkIRect::setLTRB,
        "Sets SkIRect to (left, top, right, bottom).")
    .def("setXYWH", &SkIRect::setXYWH,
        "Sets SkIRect to: (x, y, x + width, y + height).")
    .def("setWH", &SkIRect::setWH)
    .def("makeOffset",
        (SkIRect (SkIRect::*)(int32_t, int32_t) const) &SkIRect::makeOffset,
        "Returns SkIRect offset by (dx, dy).")
    .def("makeOffset",
        (SkIRect (SkIRect::*)(SkIVector) const) &SkIRect::makeOffset,
        "Returns SkIRect offset by (offset.x(), offset.y()).")
    .def("makeInset", &SkIRect::makeInset,
        "Returns SkIRect, inset by (dx, dy).")
    .def("makeOutset", &SkIRect::makeOutset,
        "Returns SkIRect, outset by (dx, dy).")
    .def("offset", py::overload_cast<int32_t, int32_t>(&SkIRect::offset),
        "Offsets SkIRect by adding dx to fLeft, fRight; and by adding dy to "
        "fTop, fBottom.")
    .def("offset", py::overload_cast<const SkIPoint&>(&SkIRect::offset),
        "Offsets SkIRect by adding delta.fX to fLeft, fRight; and by adding "
        "delta.fY to fTop, fBottom.")
    .def("offsetTo", &SkIRect::offsetTo,
        "Offsets SkIRect so that fLeft equals newX, and fTop equals newY.")
    .def("inset", &SkIRect::inset, "Insets SkIRect by (dx,dy).")
    .def("outset", &SkIRect::outset, "Outsets SkIRect by (dx, dy).")
    .def("adjust", &SkIRect::adjust,
        "Adjusts SkIRect by adding dL to fLeft, dT to fTop, dR to fRight, and "
        "dB to fBottom.")
    .def("contains",
        (bool (SkIRect::*)(int32_t, int32_t) const) &SkIRect::contains,
        "Returns true if: fLeft <= x < fRight && fTop <= y < fBottom.")
    .def("contains",
        (bool (SkIRect::*)(const SkIRect&) const) &SkIRect::contains,
        "Returns true if SkIRect contains r.")
    .def("contains",
        (bool (SkIRect::*)(const SkRect&) const) &SkIRect::contains,
        "Returns true if SkRect contains r. ")
    .def("containsNoEmptyCheck", &SkIRect::containsNoEmptyCheck,
        "Returns true if SkIRect contains construction.")
    .def("intersect",
        (bool (SkIRect::*)(const SkIRect&)) &SkIRect::intersect,
        "Returns true if SkIRect intersects r, and sets SkIRect to "
        "intersection.")
    .def("intersect",
        (bool (SkIRect::*)(const SkIRect&, const SkIRect&)) &SkIRect::intersect,
        "Returns true if a intersects b, and sets SkIRect to intersection.")
    .def("join", &SkIRect::join, "Sets SkIRect to the union of itself and r.")
    .def("sort", &SkIRect::sort,
        "Swaps fLeft and fRight if fLeft is greater than fRight; and swaps "
        "fTop and fBottom if fTop is greater than fBottom.")
    .def("makeSorted", &SkIRect::makeSorted,
        "Returns SkIRect with fLeft and fRight swapped if fLeft is greater "
        "than fRight; and with fTop and fBottom swapped if fTop is greater "
        "than fBottom.")
    .def_static("MakeEmpty", &SkIRect::MakeEmpty,
        "Returns constructed SkIRect set to (0, 0, 0, 0).")
    .def_static("MakeWH", &SkIRect::MakeWH,
        "Returns constructed SkIRect set to (0, 0, w, h).")
    .def_static("MakeSize", &SkIRect::MakeSize,
        "Returns constructed SkIRect set to (0, 0, size.width(), "
        "size.height()).")
    .def_static("MakeLTRB", &SkIRect::MakeLTRB,
        "Returns constructed SkIRect set to (l, t, r, b).")
    .def_static("MakeXYWH", &SkIRect::MakeXYWH,
        "Returns constructed SkIRect set to: (x, y, x + w, y + h).")
    .def_static("Intersects", &SkIRect::Intersects,
        "Returns true if a intersects b.")
    .def_readwrite("fLeft", &SkIRect::fLeft, "smaller x-axis bounds")
    .def_readwrite("fTop", &SkIRect::fTop, "smaller y-axis bounds")
    .def_readwrite("fRight", &SkIRect::fRight, "larger x-axis bounds")
    .def_readwrite("fBottom", &SkIRect::fBottom, "larger y-axis bounds")
    .def(py::self == py::self,
        "Returns true if all members in a: fLeft, fTop, fRight, and fBottom; "
        "are identical to corresponding members in b.")
    .def(py::self != py::self,
        "Returns true if any member in a: fLeft, fTop, fRight, and fBottom; is "
        "not identical to the corresponding member in b.")
    ;
// Rect
py::class_<SkRect>(m, "Rect")
    // Python additions.
    .def(py::init(&SkRect::MakeEmpty))
    .def(py::init(&SkRect::MakeWH))
    .def(py::init(&SkRect::MakeLTRB))
    .def("__repr__", [](const SkRect& r) {
        std::stringstream s;
        s << "Rect(" << r.fLeft << ", " << r.fTop << ", " <<
            r.fRight << ", " << r.fBottom << ")";
        return s.str();
    })
    // Wrappers.
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
py::class_<SkRRect> rrect(m, "RRect");
py::enum_<SkRRect::Type>(rrect, "Type");
py::enum_<SkRRect::Corner>(rrect, "Corner");
rrect
    .def(py::init(),
        "Initializes bounds at (0, 0), the origin, with zero width and height.")
    .def(py::init<const SkRRect&>(),
        "Initializes to copy of rrect bounds and corner radii.")
    .def("getType", &SkRRect::getType)
    .def("type", &SkRRect::type)
    .def("isEmpty", &SkRRect::isEmpty)
    .def("isRect", &SkRRect::isRect)
    .def("isOval", &SkRRect::isOval)
    .def("isSimple", &SkRRect::isSimple)
    .def("isNinePatch", &SkRRect::isNinePatch)
    .def("isComplex", &SkRRect::isComplex)
    .def("width", &SkRRect::width, "Returns span on the x-axis.")
    .def("height", &SkRRect::height, "Returns span on the y-axis.")
    .def("getSimpleRadii", &SkRRect::getSimpleRadii,
        "Returns top-left corner radii.")
    .def("setEmpty", &SkRRect::setEmpty,
        "Sets bounds to zero width and height at (0, 0), the origin.")
    .def("setRect", &SkRRect::setRect,
        "Sets bounds to sorted rect, and sets corner radii to zero.")
    .def("setOval", &SkRRect::setOval,
        "Sets bounds to oval, x-axis radii to half oval.width(), and all "
        "y-axis radii to half oval.height().")
    .def("setRectXY", &SkRRect::setRectXY,
        "Sets to rounded rectangle with the same radii for all four corners.")
    .def("setNinePatch", &SkRRect::setNinePatch,
        "Sets bounds to rect.")
    .def("setRectRadii", &SkRRect::setRectRadii,
        "Sets bounds to rect.")
    .def("rect", &SkRRect::rect, "Returns bounds.")
    .def("radii", &SkRRect::radii,
        "Returns scalar pair for radius of curve on x-axis and y-axis for one "
        "corner.")
    .def("getBounds", &SkRRect::getBounds, "Returns bounds.")
    .def("inset",
        py::overload_cast<SkScalar, SkScalar, SkRRect*>(
            &SkRRect::inset, py::const_),
        "Copies SkRRect to dst, then insets dst bounds by dx and dy, and "
        "adjusts dst radii by dx and dy.")
    .def("inset",
        py::overload_cast<SkScalar, SkScalar>(&SkRRect::inset),
        "Insets bounds by dx and dy, and adjusts radii by dx and dy.")
    .def("outset",
        py::overload_cast<SkScalar, SkScalar, SkRRect*>(
            &SkRRect::outset, py::const_),
        "Outsets dst bounds by dx and dy, and adjusts radii by dx and dy.")
    .def("outset",
        py::overload_cast<SkScalar, SkScalar>(&SkRRect::outset),
        "Outsets bounds by dx and dy, and adjusts radii by dx and dy.")
    .def("offset", &SkRRect::offset, "Translates SkRRect by (dx, dy).")
    .def("makeOffset", &SkRRect::makeOffset,
        "Returns SkRRect translated by (dx, dy).")
    .def("contains", &SkRRect::contains,
        "Returns true if rect is inside the bounds and corner radii, and if "
        "SkRRect and rect are not empty.")
    .def("isValid", &SkRRect::isValid,
        "Returns true if bounds and radii values are finite and describe a "
        "SkRRect SkRRect::Type that matches getType().")
    .def("writeToMemory", &SkRRect::writeToMemory, "Writes SkRRect to buffer.")
    .def("readFromMemory", &SkRRect::readFromMemory,
        "Reads SkRRect from buffer, reading kSizeInMemory bytes.")
    .def("transform", &SkRRect::transform,
        "Transforms by SkRRect by matrix, storing result in dst.")
    .def("dump", py::overload_cast<bool>(&SkRRect::dump, py::const_),
        "Writes text representation of SkRRect to standard output.")
    .def("dump", py::overload_cast<>(&SkRRect::dump, py::const_),
        "Writes text representation of SkRRect to standard output.")
    .def("dumpHex", &SkRRect::dumpHex,
        "Writes text representation of SkRRect to standard output.")
    .def_static("MakeEmpty", &SkRRect::MakeEmpty,
        "Initializes bounds at (0, 0), the origin, with zero width and height.")
    .def_static("MakeRect", &SkRRect::MakeRect,
        "Initializes to copy of r bounds and zeroes corner radii.")
    .def_static("MakeOval", &SkRRect::MakeOval,
        "Sets bounds to oval, x-axis radii to half oval.width(), and all "
        "y-axis radii to half oval.height().")
    .def_static("MakeRectXY", &SkRRect::MakeRectXY,
        "Sets to rounded rectangle with the same radii for all four corners.")
    .def_readonly_static("kSizeInMemory", &SkRRect::kSizeInMemory)
    .def(py::self == py::self,
        "Returns true if bounds and radii in a are equal to bounds and radii "
        "in b.")
    .def(py::self != py::self,
        "Returns true if bounds and radii in a are not equal to bounds and "
        "radii in b.")
    ;
}
