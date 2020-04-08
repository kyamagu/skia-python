#include "common.h"
#include <pybind11/operators.h>

const int SkRegion::kOpCnt;

void initRegion(py::module &m) {
py::class_<SkRegion> region(m, "Region", R"docstring(
    SkRegion describes the set of pixels used to clip SkCanvas.

    SkRegion is compact, efficiently storing a single integer rectangle, or a
    run length encoded array of rectangles. SkRegion may reduce the current
    SkCanvas clip, or may be drawn as one or more integer rectangles. SkRegion
    iterator returns the scan lines or rectangles contained by it, optionally
    intersecting a bounding rectangle.
    )docstring");

py::class_<SkRegion::Cliperator>(region, "Cliperator", R"docstring(
    Returns the sequence of rectangles, sorted along y-axis, then x-axis, that
    make up SkRegion intersected with the specified clip rectangle.
    )docstring")
    .def(py::init<const SkRegion&, const SkIRect&>(),
        "Sets SkRegion::Cliperator to return elements of SkIRect array in "
        "SkRegion within clip.")
    .def("done", &SkRegion::Cliperator::done,
        "Returns true if SkRegion::Cliperator is pointing to final SkIRect "
        "in SkRegion.")
    .def("next", &SkRegion::Cliperator::next,
        "Advances iterator to next SkIRect in SkRegion contained by clip.")
    .def("rect", &SkRegion::Cliperator::rect,
        "   Returns SkIRect element in SkRegion, intersected with clip "
        "passed to SkRegion::Cliperator constructor.")
    ;

py::class_<SkRegion::Iterator>(region, "Iterator", R"docstring(
    Returns sequence of rectangles, sorted along y-axis, then x-axis, that make
    up SkRegion.
    )docstring")
    .def(py::init<>(),
        "Initializes SkRegion::Iterator with an empty SkRegion.")
    .def(py::init<const SkRegion&>(),
        "Sets SkRegion::Iterator to return elements of SkIRect array in "
        "region.")
    .def("rewind", &SkRegion::Iterator::rewind,
        "SkPoint SkRegion::Iterator to start of SkRegion.")
    .def("reset", &SkRegion::Iterator::reset,
        "Resets iterator, using the new SkRegion.")
    .def("done", &SkRegion::Iterator::done,
        "Returns true if SkRegion::Iterator is pointing to final SkIRect in "
        "SkRegion.")
    .def("next", &SkRegion::Iterator::next,
        "Advances SkRegion::Iterator to next SkIRect in SkRegion if it is not "
        "done.")
    .def("rect", &SkRegion::Iterator::rect,
        "Returns SkIRect element in SkRegion.")
    .def("rgn", &SkRegion::Iterator::rgn,
        "Returns SkRegion if set; otherwise, returns nullptr.")
    ;

py::class_<SkRegion::Spanerator>(region, "Spanerator", R"docstring(
    Returns the line segment ends within SkRegion that intersect a horizontal
    line.
    )docstring")
    .def(py::init<const SkRegion&, int, int, int>(),
        "Sets SkRegion::Spanerator to return line segments in SkRegion on "
        "scan line.")
    .def("next", &SkRegion::Spanerator::next,
        "Advances iterator to next span intersecting SkRegion within line "
        "segment provided in constructor.")
    ;

py::enum_<SkRegion::Op>(region, "Op")
    .value("kDifference_Op", SkRegion::Op::kDifference_Op)
    .value("kIntersect_Op", SkRegion::Op::kIntersect_Op)
    .value("kUnion_Op", SkRegion::Op::kUnion_Op)
    .value("kXOR_Op", SkRegion::Op::kXOR_Op)
    .value("kReverseDifference_Op", SkRegion::Op::kReverseDifference_Op)
    .value("kReplace_Op", SkRegion::Op::kReplace_Op)
    .value("kLastOp", SkRegion::Op::kLastOp)
    .export_values();

region
    .def(py::init<>(), "Constructs an empty SkRegion.")
    .def(py::init<const SkRegion&>(),
        "Constructs a copy of an existing region.")
    .def(py::init<const SkIRect&>(),
        "Constructs a rectangular SkRegion matching the bounds of rect.")
    .def(py::self == py::self,
        "Compares SkRegion and other; returns true if they enclose exactly "
        "the same area.")
    .def(py::self != py::self,
        "Compares SkRegion and other; returns true if they do not enclose "
        "the same area.")
    .def("set", &SkRegion::set,
        "Sets SkRegion to src, and returns true if src bounds is not empty.")
    .def("swap", &SkRegion::swap,
        "Exchanges SkIRect array of SkRegion and other.")
    .def("isEmpty", &SkRegion::isEmpty,
        "Returns true if SkRegion is empty.")
    .def("isRect", &SkRegion::isRect,
        "Returns true if SkRegion is one SkIRect with positive dimensions.")
    .def("isComplex", &SkRegion::isComplex,
        "Returns true if SkRegion is described by more than one rectangle.")
    .def("getBounds", &SkRegion::getBounds,
        "Returns minimum and maximum axes values of SkIRect array.")
    .def("computeRegionComplexity", &SkRegion::computeRegionComplexity,
        "Returns a value that increases with the number of elements in "
        "SkRegion.")
    .def("getBoundaryPath", &SkRegion::getBoundaryPath,
        "Appends outline of SkRegion to path.")
    .def("setEmpty", &SkRegion::setEmpty, "Constructs an empty SkRegion.")
    .def("setRect", &SkRegion::setRect,
        "Constructs a rectangular SkRegion matching the bounds of rect.")
    .def("setRects", &SkRegion::setRects,
        "Constructs SkRegion as the union of SkIRect in rects array.")
    .def("setRegion", &SkRegion::setRegion,
        "Constructs a copy of an existing region.")
    .def("setPath", &SkRegion::setPath,
        "Constructs SkRegion to match outline of path within clip.")
    .def("intersects",
        py::overload_cast<const SkIRect&>(&SkRegion::intersects, py::const_),
        "Returns true if SkRegion intersects rect.")
    .def("intersects",
        py::overload_cast<const SkRegion&>(&SkRegion::intersects, py::const_),
        "Returns true if SkRegion intersects other.")
    .def("contains",
        py::overload_cast<int32_t, int32_t>(&SkRegion::contains, py::const_),
        "Returns true if SkIPoint (x, y) is inside SkRegion.")
    .def("contains",
        py::overload_cast<const SkIRect&>(&SkRegion::contains, py::const_),
        "Returns true if other is completely inside SkRegion.")
    .def("contains",
        py::overload_cast<const SkRegion&>(&SkRegion::contains, py::const_),
        "Returns true if other is completely inside SkRegion.")
    .def("quickContains", &SkRegion::quickContains,
        "Returns true if SkRegion is a single rectangle and contains r.")
    .def("quickReject",
        py::overload_cast<const SkIRect&>(&SkRegion::quickReject, py::const_),
        "Returns true if SkRegion does not intersect rect.")
    .def("quickReject",
        py::overload_cast<const SkRegion&>(&SkRegion::quickReject, py::const_),
        "Returns true if SkRegion does not intersect rgn.")
    .def("translate",
        py::overload_cast<int, int>(&SkRegion::translate),
        "Offsets SkRegion by ivector (dx, dy).")
    .def("translate",
        py::overload_cast<int, int, SkRegion*>(
            &SkRegion::translate, py::const_),
        "Offsets SkRegion by ivector (dx, dy), writing result to dst.")
    .def("op",
        py::overload_cast<const SkIRect&, SkRegion::Op>(&SkRegion::op),
        "Replaces SkRegion with the result of SkRegion op rect.")
    .def("op",
        py::overload_cast<const SkRegion&, SkRegion::Op>(&SkRegion::op),
        "Replaces SkRegion with the result of SkRegion op rgn.")
    .def("op",
        py::overload_cast<const SkIRect&, const SkRegion&, SkRegion::Op>(
            &SkRegion::op),
        "Replaces SkRegion with the result of rect op rgn.")
    .def("op",
        py::overload_cast<const SkRegion&, const SkIRect&, SkRegion::Op>(
            &SkRegion::op),
        "Replaces SkRegion with the result of rgn op rect.")
    .def("op",
        py::overload_cast<const SkRegion&, const SkRegion&, SkRegion::Op>(
            &SkRegion::op),
        "Replaces SkRegion with the result of rgna op rgnb.")
    .def("writeToMemory", &SkRegion::writeToMemory,
        "Writes SkRegion to buffer, and returns number of bytes written.")
    .def("readFromMemory", &SkRegion::readFromMemory,
        "Constructs SkRegion from buffer of size length.")
    .def_readonly_static("kOpCnt", &SkRegion::kOpCnt)
    ;
}
