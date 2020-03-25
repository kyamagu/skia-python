#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <skia.h>

namespace py = pybind11;

void initIRect(py::module &m) {
py::class_<SkIRect>(m, "IRect")
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
}
