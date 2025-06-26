#include "common.h"
#include <include/core/SkRRect.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/iostream.h>

namespace py = pybind11;

constexpr size_t SkRRect::kSizeInMemory;

void initRect(py::module &m) {
// IRect
py::class_<SkIRect>(m, "IRect", R"docstring(
    :py:class:`IRect` holds four 32-bit integer coordinates describing the upper
    and lower bounds of a rectangle.

    :py:class:`IRect` may be created from outer bounds or from position, width,
    and height. :py:class:`IRect` describes an area; if its right is less than
    or equal to its left, or if its bottom is less than or equal to its top, it
    is considered empty.

    Example::

        irect = skia.IRect(0, 0, 180, 120)
        irect = skia.IRect((0, 0, 180, 120))  # Convert from tuple
        print(irect.width(), irect.height())
        left, top, right, bottom = tuple(irect)  # Convert to tuple

    )docstring")
    .def(py::init(&SkIRect::MakeEmpty),
        R"docstring(
        Creates :py:class:`IRect` set to (0, 0, 0, 0).

        Many other rectangles are empty; if left is equal to or greater than
        right, or if top is equal to or greater than bottom. Setting all members
        to zero is a convenience, but does not designate a special empty
        rectangle.

        :return: bounds (0, 0, 0, 0)
        )docstring")
    .def(py::init(&SkIRect::MakeWH),
        R"docstring(
        Returns constructed :py:class:`IRect` set to (0, 0, w, h).

        Does not validate input; w or h may be negative.

        :w: width of constructed :py:class:`IRect`
        :h: height of constructed :py:class:`IRect`
        :return: bounds (0, 0, w, h)
        )docstring",
        py::arg("w"), py::arg("h"))
    .def(py::init(&SkIRect::MakeLTRB),
        R"docstring(
        Creates :py:class:`IRect` set to: (x, y, x + w, y + h).

        Does not validate input; w or h may be negative.

        :l: integer stored in fLeft
        :t: integer stored in fTop
        :r: integer stored in fRight
        :b: integer stored in fBottom
        :return: bounds (l, t, r, b)
        )docstring",
        py::arg("l"), py::arg("t"), py::arg("r"), py::arg("b"))
    .def(py::init(
        [] (py::tuple t) {
            if (t.size() == 0)
                return SkIRect::MakeEmpty();
            else if (t.size() == 2)
                return SkIRect::MakeWH(
                    t[0].cast<int32_t>(), t[1].cast<int32_t>());
            else if (t.size() == 4)
                return SkIRect::MakeLTRB(
                    t[0].cast<int32_t>(), t[1].cast<int32_t>(),
                    t[2].cast<int32_t>(), t[3].cast<int32_t>());
            else
                throw py::value_error("Invalid tuple.");
        }),
        py::arg("t"))
    .def("__iter__",
        [](const SkIRect& r) {
            return py::make_iterator(&r.fLeft, &r.fLeft + 4);
        },
        py::keep_alive<0, 1>())
    .def("__len__", [] (const SkIRect& r) { return 4; })
    .def("__repr__",
        [](const SkIRect& r) {
            std::stringstream s;
            s << "IRect(" << r.fLeft << ", " << r.fTop << ", " <<
                r.fRight << ", " << r.fBottom << ")";
            return s.str();
        })
    .def("left", &SkIRect::left,
        R"docstring(
        Returns left edge of :py:class:`IRect`, if sorted.

        Call :py:meth:`sort` to reverse fLeft and fRight if needed.

        :return: fLeft
        )docstring")
    .def("top", &SkIRect::top,
        R"docstring(
        Returns top edge of :py:class:`IRect`, if sorted.

        Call :py:meth:`isEmpty` to see if :py:class:`IRect` may be invalid, and
        :py:meth:`sort` to reverse fTop and fBottom if needed.

        :return: fTop
        )docstring")
    .def("right", &SkIRect::right,
        R"docstring(
        Returns right edge of :py:class:`IRect`, if sorted.

        Call :py:meth:`sort` to reverse fLeft and fRight if needed.

        :return: fRight
        )docstring")
    .def("bottom", &SkIRect::bottom,
        R"docstring(
        Returns bottom edge of :py:class:`IRect`, if sorted.

        Call :py:meth:`isEmpty` to see if :py:class:`IRect` may be invalid, and
        :py:meth:`sort` to reverse fTop and fBottom if needed.

        Returns
        fBottom
        )docstring")
    .def("x", &SkIRect::x,
        R"docstring(
        Returns left edge of :py:class:`IRect`, if sorted.

        Call :py:meth:`isEmpty` to see if :py:class:`IRect` is valid, and
        :py:meth:`sort` to reverse fLeft and fRight if needed.

        :return: fLeft
        )docstring")
    .def("y", &SkIRect::y,
        R"docstring(
        Returns top edge of :py:class:`IRect`, if sorted.

        Call :py:meth:`isEmpty` to see if :py:class:`IRect` is invalid, and
        :py:meth:`sort` to reverse fTop and fBottom if needed.

        :return: fTop
        )docstring")
    .def("topLeft", &SkIRect::topLeft)
    .def("width", &SkIRect::width,
        R"docstring(
        Returns span on the x-axis.

        This does not check if :py:class:`IRect` is sorted, or if result fits in
        32-bit signed integer; result may be negative.

        :return: fRight minus fLeft
        )docstring")
    .def("height", &SkIRect::height,
        R"docstring(
        Returns span on the y-axis.

        This does not check if :py:class:`IRect` is sorted, or if result fits in
        32-bit signed integer; result may be negative.

        :return: fBottom minus fTop
        )docstring")
    .def("size", &SkIRect::size, "Returns spans on the x-axis and y-axis.")
    .def("width64", &SkIRect::width64,
        R"docstring(
        Returns span on the x-axis.

        This does not check if :py:class:`IRect` is sorted, so the result may be
        negative. This is safer than calling :py:meth:`width` since
        :py:meth:`width` might overflow in its calculation.

        :return: fRight minus fLeft cast to int64_t
        )docstring")
    .def("height64", &SkIRect::height64,
        R"docstring(
        Returns span on the y-axis.

        This does not check if :py:class:`IRect` is sorted, so the result may be
        negative. This is safer than calling :py:meth:`height` since
        :py:meth:`height` might overflow in its calculation.

        :return: fBottom minus fTop cast to int64_t
        )docstring")
    .def("isEmpty64", &SkIRect::isEmpty64,
        R"docstring(
        Returns true if fLeft is equal to or greater than fRight, or if fTop is
        equal to or greater than fBottom.

        Call :py:meth:`sort` to reverse rectangles with negative
        :py:meth:`width64` or :py:meth:`height64`.

        :return: true if :py:meth:`width64` or :py:meth:`height64` are zero or
            negative
        )docstring")
    .def("isEmpty", &SkIRect::isEmpty,
        R"docstring(
        Returns true if :py:meth:`width` or :py:meth:`height` are zero or
        negative.

        :return: true if :py:meth:`width` or :py:meth:`height` are zero or
            negative
        )docstring")
    .def("setEmpty", &SkIRect::setEmpty,
        R"docstring(
        Sets :py:class:`IRect` to (0, 0, 0, 0).

        Many other rectangles are empty; if left is equal to or greater than
        right, or if top is equal to or greater than bottom. Setting all members
        to zero is a convenience, but does not designate a special empty
        rectangle.
        )docstring")
    .def("setLTRB", &SkIRect::setLTRB,
        R"docstring(
        Sets :py:class:`IRect` to (left, top, right, bottom).

        left and right are not sorted; left is not necessarily less than right.
        top and bottom are not sorted; top is not necessarily less than bottom.

        :param left:   stored in fLeft
        :param top:    stored in fTop
        :param right:  stored in fRight
        :param bottom: stored in fBottom
        )docstring",
        py::arg("left"), py::arg("top"), py::arg("right"), py::arg("bottom"))
    .def("setXYWH", &SkIRect::setXYWH,
        R"docstring(
        Sets :py:class:`IRect` to: (x, y, x + width, y + height).

        Does not validate input; width or height may be negative.

        :param x:  stored in fLeft
        :param y:  stored in fTop
        :param width:   added to x and stored in fRight
        :param height:  added to y and stored in fBottom
        )docstring",
        py::arg("x"), py::arg("y"), py::arg("width"), py::arg("height"))
    .def("setWH", &SkIRect::setWH, py::arg("width"), py::arg("height"))
    .def("makeOffset",
        py::overload_cast<int32_t, int32_t>(&SkIRect::makeOffset, py::const_),
        R"docstring(
        Returns :py:class:`IRect` offset by (dx, dy).

        If dx is negative, :py:class:`IRect` returned is moved to the left. If
        dx is positive, :py:class:`IRect` returned is moved to the right. If dy
        is negative, :py:class:`IRect` returned is moved upward. If dy is
        positive, :py:class:`IRect` returned is moved downward.

        :dx:  offset added to fLeft and fRight
        :dy:  offset added to fTop and fBottom
        :return: :py:class:`IRect` offset by dx and dy, with original width and
            height
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("makeOffset",
        py::overload_cast<SkIVector>(&SkIRect::makeOffset, py::const_),
        R"docstring(
        Returns :py:class:`IRect` offset by (offset.x(), offset.y()).

        If offset.x() is negative, :py:class:`IRect` returned is moved to the
        left. If offset.x() is positive, :py:class:`IRect` returned is moved to
        the right. If offset.y() is negative, :py:class:`IRect` returned is
        moved upward. If offset.y() is positive, :py:class:`IRect` returned is
        moved downward.

        :offset:  translation vector
        :return: :py:class:`IRect` translated by offset, with original width and
            height
        )docstring",
        py::arg("offset"))
    .def("makeInset", &SkIRect::makeInset,
        R"docstring(
        Returns :py:class:`IRect`, inset by (dx, dy).

        If dx is negative, :py:class:`IRect` returned is wider. If dx is
        positive, :py:class:`IRect` returned is narrower. If dy is negative,
        :py:class:`IRect` returned is taller. If dy is positive,
        :py:class:`IRect` returned is shorter.

        :param dx:  offset added to fLeft and subtracted from fRight
        :param dy:  offset added to fTop and subtracted from fBottom
        :return: :py:class:`IRect` inset symmetrically left and right, top and
            bottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("makeOutset", &SkIRect::makeOutset,
        R"docstring(
        Returns :py:class:`IRect`, outset by (dx, dy).

        If dx is negative, :py:class:`IRect` returned is narrower. If dx is
        positive, :py:class:`IRect` returned is wider. If dy is negative,
        :py:class:`IRect` returned is shorter. If dy is positive,
        :py:class:`IRect` returned is taller.

        :param dx: offset subtracted to fLeft and added from fRight
        :param dy: offset subtracted to fTop and added from fBottom
        :return: :py:class:`IRect` outset symmetrically left and right, top and
            bottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("offset", py::overload_cast<int32_t, int32_t>(&SkIRect::offset),
        R"docstring(
        Offsets :py:class:`IRect` by adding dx to fLeft, fRight; and by adding
        dy to fTop, fBottom.

        If dx is negative, moves :py:class:`IRect` returned to the left. If dx
        is positive, moves :py:class:`IRect` returned to the right. If dy is
        negative, moves :py:class:`IRect` returned upward. If dy is positive,
        moves :py:class:`IRect` returned downward.

        :dx:  offset added to fLeft and fRight
        :dy:  offset added to fTop and fBottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("offset", py::overload_cast<const SkIPoint&>(&SkIRect::offset),
        R"docstring(
        Offsets :py:class:`IRect` by adding delta.fX to fLeft, fRight; and by
        adding delta.fY to fTop, fBottom.

        If delta.fX is negative, moves :py:class:`IRect` returned to the left.
        If delta.fX is positive, moves :py:class:`IRect` returned to the right.
        If delta.fY is negative, moves :py:class:`IRect` returned upward. If
        delta.fY is positive, moves :py:class:`IRect` returned downward.

        :delta:  offset added to :py:class:`IRect`
        )docstring",
        py::arg("delta"))
    .def("offsetTo", &SkIRect::offsetTo,
        R"docstring(
        Offsets :py:class:`IRect` so that fLeft equals newX, and fTop equals
        newY.

        width and height are unchanged.

        :param newX:  stored in fLeft, preserving :py:meth:`width`
        :param newY:  stored in fTop, preserving :py:meth:`height`
        )docstring",
        py::arg("newX"), py::arg("newY"))
    .def("inset", &SkIRect::inset,
        R"docstring(
        Insets :py:class:`IRect` by (dx,dy).

        If dx is positive, makes :py:class:`IRect` narrower. If dx is negative,
        makes :py:class:`IRect` wider. If dy is positive, makes
        :py:class:`IRect` shorter. If dy is negative, makes :py:class:`IRect`
        taller.

        :param dx:  offset added to fLeft and subtracted from fRight
        :param dy:  offset added to fTop and subtracted from fBottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("outset", &SkIRect::outset,
        R"docstring(
        Outsets :py:class:`IRect` by (dx, dy).

        If dx is positive, makes :py:class:`IRect` wider. If dx is negative,
        makes :py:class:`IRect` narrower. If dy is positive, makes
        :py:class:`IRect` taller. If dy is negative, makes :py:class:`IRect`
        shorter.

        :param dx:  subtracted to fLeft and added from fRight
        :param dy:  subtracted to fTop and added from fBottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("adjust", &SkIRect::adjust,
        R"docstring(
        Adjusts :py:class:`IRect` by adding dL to fLeft, dT to fTop, dR to
        fRight, and dB to fBottom.

        If dL is positive, narrows :py:class:`IRect` on the left. If negative,
        widens it on the left. If dT is positive, shrinks :py:class:`IRect` on
        the top. If negative, lengthens it on the top. If dR is positive,
        narrows :py:class:`IRect` on the right. If negative, widens it on the
        right. If dB is positive, shrinks :py:class:`IRect` on the bottom. If
        negative, lengthens it on the bottom.

        The resulting :py:class:`IRect` is not checked for validity. Thus, if
        the resulting :py:class:`IRect` left is greater than right, the
        :py:class:`IRect` will be considered empty. Call :py:meth:`sort` after
        this call if that is not the desired behavior.

        :param dL:  offset added to fLeft
        :param dT:  offset added to fTop
        :param dR:  offset added to fRight
        :param dB:  offset added to fBottom
        )docstring",
        py::arg("dL"), py::arg("dT"), py::arg("dR"), py::arg("dB"))
    .def("__contains__",
        [] (const SkIRect& r, const SkIPoint& p) {
            return r.contains(p.fX, p.fY);
        },
        py::is_operator())
    .def("__contains__",
        py::overload_cast<const SkIRect&>(&SkIRect::contains, py::const_),
        py::is_operator())
    .def("__contains__",
        py::overload_cast<const SkRect&>(&SkIRect::contains, py::const_),
        py::is_operator())
    .def("contains",
        py::overload_cast<int32_t, int32_t>(&SkIRect::contains, py::const_),
        R"docstring(
        Returns true if: fLeft <= x < fRight && fTop <= y < fBottom.

        Returns false if :py:class:`IRect` is empty.

        Considers input to describe constructed :py:class:`IRect`: (x, y, x + 1,
        y + 1) and returns true if constructed area is completely enclosed by
        :py:class:`IRect` area.

        :x:  test :py:class:`IPoint` x-coordinate
        :y:  test :py:class:`IPoint` y-coordinate
        :return: true if (x, y) is inside :py:class:`IRect`
        )docstring",
        py::arg("x"), py::arg("y"))
    .def("contains",
        py::overload_cast<const SkIRect&>(&SkIRect::contains, py::const_),
        R"docstring(
        Returns true if :py:class:`IRect` contains r.

        Returns false if :py:class:`IRect` is empty or r is empty.

        :py:class:`IRect` contains r when :py:class:`IRect` area completely
        includes r area.

        :r: :py:class:`IRect` contained
        :return: true if all sides of :py:class:`IRect` are outside r
        )docstring",
        py::arg("r"))
    .def("contains",
        py::overload_cast<const SkRect&>(&SkIRect::contains, py::const_),
        R"docstring(
        Returns true if :py:class:`IRect` contains r.

        Returns false if :py:class:`IRect` is empty or r is empty.

        :py:class:`IRect` contains r when :py:class:`IRect` area completely
        includes r area.

        :r: :py:class:`Rect` contained
        :return: true if all sides of :py:class:`IRect` are outside r
        )docstring",
        py::arg("r"))
    .def("containsNoEmptyCheck", &SkIRect::containsNoEmptyCheck,
        R"docstring(
        Returns true if :py:class:`IRect` contains construction.

        Asserts if :py:class:`IRect` is empty or construction is empty, and if
        SK_DEBUG is defined.

        Return is undefined if :py:class:`IRect` is empty or construction is
        empty.

        :param r: :py:class:`IRect` contained
        :return: true if all sides of :py:class:`IRect` are outside r
        )docstring",
        py::arg("r"))
    .def("intersect",
        py::overload_cast<const SkIRect&>(&SkIRect::intersect),
        R"docstring(
        Returns true if :py:class:`IRect` intersects r, and sets
        :py:class:`IRect` to intersection.

        Returns false if :py:class:`IRect` does not intersect r, and leaves
        :py:class:`IRect` unchanged.

        Returns false if either r or :py:class:`IRect` is empty, leaving
        :py:class:`IRect` unchanged.

        :r: limit of result
        :return: true if r and :py:class:`IRect` have area in common
        )docstring",
        py::arg("r"))
    .def("intersect",
        py::overload_cast<const SkIRect&, const SkIRect&>(&SkIRect::intersect),
        R"docstring(
        Returns true if a intersects b, and sets :py:class:`IRect` to
        intersection.

        Returns false if a does not intersect b, and leaves :py:class:`IRect`
        unchanged.

        Returns false if either a or b is empty, leaving :py:class:`IRect`
        unchanged.

        :a: :py:class:`IRect` to intersect
        :b: :py:class:`IRect` to intersect
        :return: true if a and b have area in common
        )docstring",
        py::arg("a"), py::arg("b"))
    .def("join", &SkIRect::join,
        R"docstring(
        Sets :py:class:`IRect` to the union of itself and r.

        Has no effect if r is empty. Otherwise, if :py:class:`IRect` is empty,
        sets :py:class:`IRect` to r.

        :param r: expansion :py:class:`IRect`
        )docstring",
        py::arg("r"))
    .def("sort", &SkIRect::sort,
        R"docstring(
        Swaps fLeft and fRight if fLeft is greater than fRight; and swaps fTop
        and fBottom if fTop is greater than fBottom.

        Result may be empty, and :py:meth:`width` and :py:meth:`height` will be
        zero or positive.
        )docstring")
    .def("makeSorted", &SkIRect::makeSorted,
        R"docstring(
        Returns :py:class:`IRect` with fLeft and fRight swapped if fLeft is
        greater than fRight; and with fTop and fBottom swapped if fTop is
        greater than fBottom.

        Result may be empty; and :py:meth:`width` and :py:meth:`height` will be
        zero or positive.

        :return: sorted :py:class:`IRect`
        )docstring")
    .def_static("MakeEmpty", &SkIRect::MakeEmpty,
        R"docstring(
        Returns constructed :py:class:`IRect` set to (0, 0, 0, 0).

        Many other rectangles are empty; if left is equal to or greater than
        right, or if top is equal to or greater than bottom. Setting all members
        to zero is a convenience, but does not designate a special empty
        rectangle.

        :return: bounds (0, 0, 0, 0)
        )docstring")
    .def_static("MakeWH", &SkIRect::MakeWH,
        R"docstring(
        Returns constructed :py:class:`IRect` set to (0, 0, w, h).

        Does not validate input; w or h may be negative.

        :param w: width of constructed :py:class:`IRect`
        :param h: height of constructed :py:class:`IRect`
        :return: bounds (0, 0, w, h)
        )docstring",
        py::arg("w"), py::arg("h"))
    .def_static("MakeSize", &SkIRect::MakeSize,
        R"docstring(
        Returns constructed :py:class:`IRect` set to (0, 0, size.width(),
        size.height()).

        Does not validate input; size.width() or size.height() may be negative.

        :param size: values for :py:class:`IRect` width and height
        :return: bounds (0, 0, size.width(), size.height())
        )docstring",
        py::arg("size"))
    .def_static("MakeLTRB", &SkIRect::MakeLTRB,
        R"docstring(
        Returns constructed :py:class:`IRect` set to (l, t, r, b).

        Does not sort input; :py:class:`IRect` may result in fLeft greater than
        fRight, or fTop greater than fBottom.

        :param l: integer stored in fLeft
        :param t: integer stored in fTop
        :param r: integer stored in fRight
        :param b: integer stored in fBottom
        :return: bounds (l, t, r, b)
        )docstring",
        py::arg("l"), py::arg("t"), py::arg("r"), py::arg("b"))
    .def_static("MakeXYWH", &SkIRect::MakeXYWH,
        R"docstring(
        Returns constructed :py:class:`IRect` set to: (x, y, x + w, y + h).

        Does not validate input; w or h may be negative.

        :param x: stored in fLeft
        :param y: stored in fTop
        :param w: added to x and stored in fRight
        :param h: added to y and stored in fBottom
        :return: bounds at (x, y) with width w and height h
        )docstring",
        py::arg("x"), py::arg("y"), py::arg("w"), py::arg("h"))
    .def_static("Intersects", &SkIRect::Intersects,
        R"docstring(
        Returns true if a intersects b.

        Returns false if either a or b is empty, or do not intersect.

        :param a: :py:class:`IRect` to intersect
        :param b: :py:class:`IRect` to intersect
        :return: true if a and b have area in common
        )docstring",
        py::arg("a"), py::arg("b"))
    .def_readwrite("fLeft", &SkIRect::fLeft, "smaller x-axis bounds")
    .def_readwrite("fTop", &SkIRect::fTop, "smaller y-axis bounds")
    .def_readwrite("fRight", &SkIRect::fRight, "larger x-axis bounds")
    .def_readwrite("fBottom", &SkIRect::fBottom, "larger y-axis bounds")
    .def(py::self == py::self,
        R"docstring(
        Returns true if all members in self: fLeft, fTop, fRight, and fBottom;
        are identical to corresponding members in other.

        :param other: :py:class:`IRect` to compare
        :return: true if members are equal
        )docstring",
        py::arg("other"))
    .def(py::self != py::self,
        R"docstring(
        Returns true if any member in self: fLeft, fTop, fRight, and fBottom; is
        not identical to the corresponding member in other.

        :param other: :py:class:`IRect` to compare
        :return: true if members are not equal
        )docstring",
        py::arg("other"))
    ;

py::implicitly_convertible<py::tuple, SkIRect>();

// Rect
py::class_<SkRect>(m, "Rect", R"docstring(
    :py:class:`Rect` holds four float coordinates describing the upper and lower
    bounds of a rectangle.

    :py:class:`Rect` may be created from outer bounds or from position, width,
    and height. :py:class:`Rect` describes an area; if its right is less than or
    equal to its left, or if its bottom is less than or equal to its top, it is
    considered empty.

    Example::

        rect = skia.Rect(0, 0, 180, 120)
        rect = skia.Rect((0, 0, 180, 120))  # Convert from tuple
        print(rect.width(), rect.height())
        left, top, right, bottom = tuple(rect)  # Convert to tuple
    )docstring")
    .def(py::init(&SkRect::MakeEmpty),
        R"docstring(
        Returns constructed :py:class:`Rect` set to (0, 0, 0, 0).

        Many other rectangles are empty; if left is equal to or greater than
        right, or if top is equal to or greater than bottom. Setting all members
        to zero is a convenience, but does not designate a special empty
        rectangle.

        :return: bounds (0, 0, 0, 0)
        )docstring")
    .def(py::init(&SkRect::MakeWH),
        R"docstring(
        Returns constructed :py:class:`Rect` set to :py:class:`Scalar` values
        (0, 0, w, h).

        Does not validate input; w or h may be negative.

        Passing integer values may generate a compiler warning since
        :py:class:`Rect` cannot represent 32-bit integers exactly. Use
        :py:class:`IRect` for an exact integer rectangle.

        :w: Scalar width of constructed :py:class:`Rect`
        :h: Scalar height of constructed :py:class:`Rect`
        :return: bounds (0, 0, w, h)
        )docstring",
        py::arg("w"), py::arg("h"))
    .def(py::init(&SkRect::MakeLTRB),
        R"docstring(
        Returns constructed :py:class:`Rect` set to (l, t, r, b).

        Does not sort input; :py:class:`Rect` may result in fLeft greater than
        fRight, or fTop greater than fBottom.

        :l:  Scalar stored in fLeft
        :t:  Scalar stored in fTop
        :r:  Scalar stored in fRight
        :b:  Scalar stored in fBottom
        :return: bounds (l, t, r, b)
        )docstring",
        py::arg("l"), py::arg("t"), py::arg("r"), py::arg("b"))
    .def(py::init(py::overload_cast<const SkISize&>(&SkRect::Make)),
        R"docstring(
        Returns constructed :py:class:`Rect` set to (0, 0, size.width(),
        size.height()).

        Does not validate input; size.width() or size.height() may be negative.

        :size: integer values for :py:class:`Rect` width and height
        :return: bounds (0, 0, size.width(), size.height())
        )docstring",
        py::arg("size"))
    .def(py::init(py::overload_cast<const SkIRect&>(&SkRect::Make)),
        R"docstring(
        Returns constructed :py:class:`Rect` set to irect, promoting integers
        to scalar.

        Does not validate input; fLeft may be greater than fRight, fTop may be
        greater than fBottom.

        :irect: integer unsorted bounds
        :return: irect members converted to Scalar
        )docstring",
        py::arg("irect"))
    .def(py::init(
        [] (py::tuple t) {
            if (t.size() == 0)
                return SkRect::MakeEmpty();
            else if (t.size() == 2)
                return SkRect::MakeWH(
                    t[0].cast<SkScalar>(), t[1].cast<SkScalar>());
            else if (t.size() == 4)
                return SkRect::MakeXYWH(
                    t[0].cast<SkScalar>(), t[1].cast<SkScalar>(),
                    t[2].cast<SkScalar>(), t[3].cast<SkScalar>());
            else
                throw py::value_error("Invalid tuple.");
        }),
        py::arg("t"))
    .def("__iter__",
        [](const SkRect& r) {
            return py::make_iterator(&r.fLeft, &r.fLeft + 4);
        },
        py::keep_alive<0, 1>())
    .def("__len__", [] (const SkRect& r) { return 4; })
    .def("__repr__", [](const SkRect& r) {
        std::stringstream s;
        s << "Rect(" << r.fLeft << ", " << r.fTop << ", " <<
            r.fRight << ", " << r.fBottom << ")";
        return s.str();
    })
    .def("isEmpty", &SkRect::isEmpty,
        R"docstring(
        Returns true if fLeft is equal to or greater than fRight, or if fTop is
        equal to or greater than fBottom.

        Call :py:meth:`sort` to reverse rectangles with negative
        :py:meth:`width` or :py:meth:`height`.

        :return: true if :py:meth:`width` or :py:meth:`height` are zero or
            negative
        )docstring")
    .def("isSorted", &SkRect::isSorted,
        R"docstring(
        Returns true if fLeft is equal to or less than fRight, or if fTop is
        equal to or less than fBottom.

        Call :py:meth:`sort` to reverse rectangles with negative
        :py:meth:`width` or :py:meth:`height`.

        :return: true if :py:meth:`width` or :py:meth:`height` are zero or
            positive
        )docstring")
    .def("isFinite", &SkRect::isFinite,
        R"docstring(
        Returns true if all values in the rectangle are finite: SK_ScalarMin or
        larger, and SK_ScalarMax or smaller.

        :return: true if no member is infinite or NaN
        )docstring")
    .def("x", &SkRect::x,
        R"docstring(
        Returns left edge of :py:class:`Rect`, if sorted.

        Call :py:meth:`isSorted` to see if :py:class:`Rect` is valid. Call
        :py:meth:`sort` to reverse fLeft and fRight if needed.

        :return: fLeft
        )docstring")
    .def("y", &SkRect::y,
        R"docstring(
        Returns top edge of SkRect, if sorted.

        Call :py:meth:`isEmpty` to see if SkRect may be invalid, and
        :py:meth:`sort` to reverse fTop and fBottom if needed.

        :return: fTop
        )docstring")
    .def("left", &SkRect::left,
        R"docstring(
        Returns left edge of SkRect, if sorted.

        Call :py:meth:`isSorted` to see if SkRect is valid. Call :py:meth:`sort`
        to reverse fLeft and fRight if needed.

        :return: fLeft
        )docstring")
    .def("top", &SkRect::top,
        R"docstring(
        Returns top edge of SkRect, if sorted.

        Call :py:meth:`isEmpty` to see if SkRect may be invalid, and
        :py:meth:`sort` to reverse fTop and fBottom if needed.

        :return: fTop
        )docstring")
    .def("right", &SkRect::right,
        R"docstring(
        Returns right edge of SkRect, if sorted.

        Call :py:meth:`isSorted` to see if SkRect is valid. Call :py:meth:`sort`
        to reverse fLeft and fRight if needed.

        :return: fRight
        )docstring")
    .def("bottom", &SkRect::bottom,
        R"docstring(
        Returns bottom edge of SkRect, if sorted.

        Call :py:meth:`isEmpty` to see if SkRect may be invalid, and
        :py:meth:`sort` to reverse fTop and fBottom if needed.

        :return: fBottom
        )docstring")
    .def("width", &SkRect::width,
        R"docstring(
        Returns span on the x-axis.

        This does not check if :py:class:`Rect` is sorted, or if result fits in
        32-bit float; result may be negative or infinity.

        :return: fRight minus fLeft
        )docstring")
    .def("height", &SkRect::height, "Returns span on the y-axis.")
    .def("centerX", &SkRect::centerX,
        R"docstring(
        Returns average of left edge and right edge.

        Result does not change if SkRect is sorted. Result may overflow to
        infinity if :py:class:`Rect` is far from the origin.

        :return: midpoint on x-axis
        )docstring")
    .def("centerY", &SkRect::centerY,
        R"docstring(
        Returns average of top edge and bottom edge.

        Result does not change if :py:class:`Rect` is sorted.

        :return: midpoint on y-axis
        )docstring")
    .def("toQuad",
        [] (const SkRect& rect) {
            std::vector<SkPoint> quad(4);
            rect.toQuad(&quad[0]);
            return quad;
        },
        R"docstring(
        Returns four points in quad that enclose :py:class:`Rect` ordered as:
        top-left, top-right, bottom-right, bottom-left.

        :return: corners of :py:class:`Rect`
        )docstring")
    .def("setEmpty", &SkRect::setEmpty,
        R"docstring(
        Sets :py:class:`Rect` to (0, 0, 0, 0).

        Many other rectangles are empty; if left is equal to or greater than
        right, or if top is equal to or greater than bottom. Setting all members
        to zero is a convenience, but does not designate a special empty
        rectangle.
        )docstring")
    .def("setLTRB", &SkRect::setLTRB,
        R"docstring(
        Sets :py:class:`Rect` to (left, top, right, bottom).

        left and right are not sorted; left is not necessarily less than right.
        top and bottom are not sorted; top is not necessarily less than bottom.

        :param left:   stored in fLeft
        :param top:    stored in fTop
        :param right:  stored in fRight
        :param bottom: stored in fBottom
        )docstring",
        py::arg("left"), py::arg("top"), py::arg("right"), py::arg("bottom"))
    .def("setBounds",
        [] (SkRect& rect, const std::vector<SkPoint>& points) {
            rect.setBounds({&points[0], points.size()});
        },
        R"docstring(
        Sets to bounds of :py:class:`Point` array with count entries.

        If count is zero or smaller, or if :py:class:`Point` array contains an
        infinity or NaN, sets to (0, 0, 0, 0).

        Result is either empty or sorted: fLeft is less than or equal to fRight,
        and fTop is less than or equal to fBottom.

        :param points: :py:class:`Point` array
        )docstring",
        py::arg("points"))
    .def("setBoundsCheck",
        [] (SkRect& rect, const std::vector<SkPoint>& points) {
            return rect.setBoundsCheck({&points[0], points.size()});
        },
        R"docstring(
        Sets to bounds of :py:class:`Point` array with count entries.

        Returns false if count is zero or smaller, or if :py:class:`Point` array
        contains an infinity or NaN; in these cases sets :py:class:`Rect` to
        (0, 0, 0, 0).

        Result is either empty or sorted: fLeft is less than or equal to fRight,
        and fTop is less than or equal to fBottom.

        :param points: :py:class:`Point` array
        :return: true if all :py:class:`Point` values are finite
        )docstring",
        py::arg("points"))
    .def("setBoundsNoCheck",
        [] (SkRect& rect, const std::vector<SkPoint>& points) {
            rect.setBoundsNoCheck({&points[0], points.size()});
        },
        R"docstring(
        Sets to bounds of :py:class:`Point` pts array with count entries.

        If any :py:class:`Point` in pts contains infinity or NaN, all
        :py:class:`Rect` dimensions are set to NaN.

        :param points: :py:class:`Point` array
        )docstring",
        py::arg("points"))
    .def("set", py::overload_cast<const SkIRect&>(&SkRect::set),
        R"docstring(
        Sets :py:class:`Rect` to src, promoting src members from integer to
        scalar.

        Very large values in src may lose precision.

        :src: integer :py:class:`Rect`
        )docstring",
        py::arg("src"))
    .def("set", py::overload_cast<const SkPoint&, const SkPoint&>(&SkRect::set),
        R"docstring(
        Sets bounds to the smallest :py:class:`Rect` enclosing :py:class:`Point`
        p0 and p1.

        The result is sorted and may be empty. Does not check to see if values
        are finite.

        :p0:  corner to include
        :p1:  corner to include
        )docstring",
        py::arg("p0"), py::arg("p1"))
    .def("setXYWH", &SkRect::setXYWH,
        R"docstring(
        Sets :py:class:`Rect` to (x, y, x + width, y + height).

        Does not validate input; width or height may be negative.

        :param x:   stored in fLeft
        :param y:   stored in fTop
        :param width:   added to x and stored in fRight
        :param height:  added to y and stored in fBottom
        )docstring",
        py::arg("x"), py::arg("y"), py::arg("width"), py::arg("height"))
    .def("setWH", &SkRect::setWH,
        R"docstring(
        Sets :py:class:`Rect` to (0, 0, width, height).

        Does not validate input; width or height may be negative.

        :param width:   stored in fRight
        :param height:  stored in fBottom
        )docstring",
        py::arg("width"), py::arg("height"))
    .def("setIWH", &SkRect::setIWH, py::arg("width"), py::arg("height"))
    .def("makeOffset",
        py::overload_cast<SkScalar, SkScalar>(&SkRect::makeOffset, py::const_),
        R"docstring(
        Returns :py:class:`Rect` offset by (dx, dy).

        If dx is negative, :py:class:`Rect` returned is moved to the left. If dx
        is positive, :py:class:`Rect` returned is moved to the right. If dy is
        negative, :py:class:`Rect` returned is moved upward. If dy is positive,
        :py:class:`Rect` returned is moved downward.

        :dx: added to fLeft and fRight
        :dy: added to fTop and fBottom
        :return: :py:class:`Rect` offset on axes, with original width and height
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("makeOffset",
        py::overload_cast<SkVector>(&SkRect::makeOffset, py::const_),
        R"docstring(
        Returns :py:class:`Rect` offset by v.

        :v: added to rect
        :return: :py:class:`Rect` offset on axes, with original width and height
        )docstring",
        py::arg("v"))
    .def("makeInset", &SkRect::makeInset,
        R"docstring(
        Returns :py:class:`Rect`, inset by (dx, dy).

        If dx is negative, :py:class:`Rect` returned is wider. If dx is
        positive, :py:class:`Rect` returned is narrower. If dy is negative,
        :py:class:`Rect` returned is taller. If dy is positive, :py:class:`Rect`
        returned is shorter.

        :param dx: added to fLeft and subtracted from fRight
        :param dy: added to fTop and subtracted from fBottom
        :return: :py:class:`Rect` inset symmetrically left and right, top and
            bottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("makeOutset", &SkRect::makeOutset,
        R"docstring(
        Returns :py:class:`Rect`, outset by (dx, dy).

        If dx is negative, :py:class:`Rect` returned is narrower. If dx is
        positive, :py:class:`Rect` returned is wider. If dy is negative,
        :py:class:`Rect` returned is shorter. If dy is positive,
        :py:class:`Rect` returned is taller.

        :param dx: subtracted to fLeft and added from fRight
        :param dy: subtracted to fTop and added from fBottom
        :return: :py:class:`Rect` outset symmetrically left and right, top and
            bottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("offset",
        py::overload_cast<SkScalar, SkScalar>(&SkRect::offset),
        R"docstring(
        Offsets :py:class:`Rect` by adding dx to fLeft, fRight; and by adding dy
        to fTop, fBottom.

        If dx is negative, moves :py:class:`Rect` to the left. If dx is
        positive, moves :py:class:`Rect` to the right. If dy is negative, moves
        :py:class:`Rect` upward. If dy is positive, moves :py:class:`Rect`
        downward.

        :dx:  offset added to fLeft and fRight
        :dy:  offset added to fTop and fBottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("offset", py::overload_cast<const SkPoint&>(&SkRect::offset),
        R"docstring(
        Offsets :py:class:`Rect` by adding delta.fX to fLeft, fRight; and by
        adding delta.fY to fTop, fBottom.

        If delta.fX is negative, moves :py:class:`Rect` to the left. If delta.fX
        is positive, moves :py:class:`Rect` to the right. If delta.fY is
        negative, moves :py:class:`Rect` upward. If delta.fY is positive, moves
        :py:class:`Rect` downward.

        :param delta: added to :py:class:`Rect`
        )docstring",
        py::arg("delta"))
    .def("offsetTo", &SkRect::offsetTo,
        R"docstring(
        Offsets :py:class:`Rect` so that fLeft equals newX, and fTop equals
        newY.

        width and height are unchanged.

        :param newX: stored in fLeft, preserving :py:meth:`width`
        :param newY: stored in fTop, preserving :py:meth:`height`
        )docstring",
        py::arg("newX"), py::arg("newY"))
    .def("inset", &SkRect::inset,
        R"docstring(
        Insets :py:class:`Rect` by (dx, dy).

        If dx is positive, makes :py:class:`Rect` narrower. If dx is negative,
        makes :py:class:`Rect` wider. If dy is positive, makes :py:class:`Rect`
        shorter. If dy is negative, makes :py:class:`Rect` taller.

        :param dx:  added to fLeft and subtracted from fRight
        :param dy:  added to fTop and subtracted from fBottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("outset", &SkRect::outset,
        R"docstring(
        Outsets :py:class:`Rect` by (dx, dy).

        If dx is positive, makes :py:class:`Rect` wider. If dx is negative,
        makes :py:class:`Rect` narrower. If dy is positive, makes
        :py:class:`Rect` taller. If dy is negative, makes :py:class:`Rect`
        shorter.

        :param dx:  subtracted to fLeft and added from fRight
        :param dy:  subtracted to fTop and added from fBottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("intersect",
        py::overload_cast<const SkRect&>(&SkRect::intersect),
        R"docstring(
        Returns true if :py:class:`Rect` intersects r, and sets :py:class:`Rect`
        to intersection.

        Returns false if :py:class:`Rect` does not intersect r, and leaves
        :py:class:`Rect` unchanged.

        Returns false if either r or :py:class:`Rect` is empty, leaving
        :py:class:`Rect` unchanged.

        :r: limit of result
        :return: true if r and :py:class:`Rect` have area in common
        )docstring",
        py::arg("r"))
    .def("intersect",
        py::overload_cast<const SkRect&, const SkRect&>(&SkRect::intersect),
        R"docstring(
        Returns true if a intersects b, and sets :py:class:`Rect` to
        intersection.

        Returns false if a does not intersect b, and leaves :py:class:`Rect`
        unchanged.

        Returns false if either a or b is empty, leaving :py:class:`Rect`
        unchanged.

        :a: :py:class:`Rect` to intersect
        :b: :py:class:`Rect` to intersect
        :return: true if a and b have area in common
        )docstring",
        py::arg("a"), py::arg("b"))
    .def("intersects", &SkRect::intersects,
        R"docstring(
        Returns true if :py:class:`Rect` intersects r.

        Returns false if either r or :py:class:`Rect` is empty, or do not
        intersect.

        :param r:  :py:class:`Rect` to intersect
        :return: true if r and :py:class:`Rect` have area in common
        )docstring",
        py::arg("r"))
    .def("join", &SkRect::join,
        R"docstring(
        Sets :py:class:`Rect` to the union of itself and r.

        Has no effect if r is empty. Otherwise, if :py:class:`Rect` is empty,
        sets :py:class:`Rect` to r.

        :param r: expansion :py:class:`Rect`
        )docstring",
        py::arg("r"))
    .def("joinNonEmptyArg", &SkRect::joinNonEmptyArg,
        R"docstring(
        Sets :py:class:`Rect` to the union of itself and r.

        Asserts if r is empty and SK_DEBUG is defined. If :py:class:`Rect` is
        empty, sets :py:class:`Rect` to r.

        May produce incorrect results if r is empty.

        :param r: expansion :py:class:`Rect`
        )docstring",
        py::arg("r"))
    .def("joinPossiblyEmptyRect", &SkRect::joinPossiblyEmptyRect,
        R"docstring(
        Sets :py:class:`Rect` to the union of itself and the construction.

        May produce incorrect results if :py:class:`Rect` or r is empty.

        :param r:  expansion :py:class:`Rect`
        )docstring",
        py::arg("r"))
    .def("__contains__",
        [] (const SkRect& r, const SkPoint& p) {
            return r.contains(p.fX, p.fY);
        },
        py::is_operator())
    .def("__contains__",
        py::overload_cast<const SkRect&>(&SkRect::contains, py::const_),
        py::is_operator())
    .def("__contains__",
        py::overload_cast<const SkIRect&>(&SkRect::contains, py::const_),
        py::is_operator())
    .def("contains",
        py::overload_cast<SkScalar, SkScalar>(&SkRect::contains, py::const_),
        R"docstring(
        Returns true if: fLeft <= x < fRight && fTop <= y < fBottom.

        Returns false if :py:class:`Rect` is empty.

        :x:  test :py:class:`Point` x-coordinate
        :y:  test :py:class:`Point` y-coordinate
        :return: true if (x, y) is inside :py:class:`Rect`
        )docstring",
        py::arg("x"), py::arg("y"))
    .def("contains",
        py::overload_cast<const SkRect&>(&SkRect::contains, py::const_),
        R"docstring(
        Returns true if :py:class:`Rect` contains r.

        Returns false if :py:class:`Rect` is empty or r is empty.

        :py:class:`Rect` contains r when :py:class:`Rect` area completely
        includes r area.

        :r: :py:class:`Rect` contained
        :return: true if all sides of :py:class:`Rect` are outside r
        )docstring",
        py::arg("r"))
    .def("contains",
        py::overload_cast<const SkIRect&>(&SkRect::contains, py::const_),
        R"docstring(
        Returns true if :py:class:`Rect` contains r.

        Returns false if :py:class:`Rect` is empty or r is empty.

        :py:class:`Rect` contains r when :py:class:`Rect` area completely
        includes r area.

        :r: :py:class:`IRect` contained
        :return: true if all sides of :py:class:`Rect` are outside r
        )docstring",
        py::arg("r"))
    .def("round", py::overload_cast<>(&SkRect::round, py::const_),
        R"docstring(
        Returns :py:class:`IRect` by adding 0.5 and discarding the fractional
        portion of :py:class:`Rect` members, using (
        ``SkScalarRoundToInt(fLeft)``, ``SkScalarRoundToInt(fTop)``,
        ``SkScalarRoundToInt(fRight)``, ``SkScalarRoundToInt(fBottom)``).

        :return: rounded :py:class:`IRect`
        )docstring")
    .def("roundIn",
        [] (const SkRect& rect) {
            SkIRect dst;
            rect.roundIn(&dst);
            return dst;
        },
        R"docstring(
        Sets :py:class:`Rect` by rounding up fLeft and fTop; and discarding the
        fractional portion of fRight and fBottom, using (
        ``SkScalarCeilToInt(fLeft)``, ``SkScalarCeilToInt(fTop)``,
        ``SkScalarFloorToInt(fRight)``, ``SkScalarFloorToInt(fBottom)``).
        )docstring")
    .def("roundOut", py::overload_cast<>(&SkRect::roundOut, py::const_),
        R"docstring(
        Sets :py:class:`IRect` by discarding the fractional portion of fLeft and
        fTop; and rounding up fRight and fBottom, using (
        ``SkScalarFloorToInt(fLeft)``, ``SkScalarFloorToInt(fTop)``,
        ``SkScalarCeilToInt(fRight)``, ``SkScalarCeilToInt(fBottom)``).

        :return: rounded :py:class:`IRect`
        )docstring")
    .def("sort", &SkRect::sort,
        R"docstring(
        Swaps fLeft and fRight if fLeft is greater than fRight; and swaps fTop
        and fBottom if fTop is greater than fBottom.

        Result may be empty; and :py:meth:`width` and :py:meth:`height` will be
        zero or positive.
        )docstring")
    .def("makeSorted", &SkRect::makeSorted,
        R"docstring(
        Returns :py:class:`Rect` with fLeft and fRight swapped if fLeft is
        greater than fRight; and with fTop and fBottom swapped if fTop is
        greater than fBottom.

        Result may be empty; and :py:meth:`width` and :py:meth:`height` will be
        zero or positive.

        :return: sorted :py:class:`Rect`
        )docstring")
    .def("asScalars",
        [] (const SkRect& r) {
            return py::memoryview::from_buffer(
                const_cast<SkScalar*>(r.asScalars()),
                sizeof(SkScalar),
                py::format_descriptor<SkScalar>::value,
                { 4 },
                { sizeof(SkScalar) },
                true
            );
        },
        R"docstring(
        Returns pointer to first scalar in :py:class:`Rect`, to treat it as an
        array with four entries.

        :return: pointer to fLeft
        )docstring")
    .def("dump",
        [] (const SkRect& rect, bool asHex) {
            py::scoped_ostream_redirect stream;
            rect.dump(asHex);
        },
        R"docstring(
        Writes text representation of :py:class:`Rect` to standard output.

        Set asHex to true to generate exact binary representations of floating
        point numbers.

        :param asHex: true if Scalar values are written as hexadecimal
        )docstring",
        py::arg("asHex") = false)
    .def("dumpHex",
        [] (const SkRect& rect) {
            py::scoped_ostream_redirect stream;
            rect.dumpHex();
        },
        R"docstring(
        Writes text representation of :py:class:`Rect` to standard output.

        The representation may be directly compiled as C++ code. Floating point
        values are written in hexadecimal to preserve their exact bit pattern.
        The output reconstructs the original :py:class:`Rect`.

        Use instead of dump() when submitting
        )docstring")
    .def_static("MakeEmpty", &SkRect::MakeEmpty,
        R"docstring(
        Returns constructed :py:class:`Rect` set to (0, 0, 0, 0).

        Many other rectangles are empty; if left is equal to or greater than
        right, or if top is equal to or greater than bottom. Setting all members
        to zero is a convenience, but does not designate a special empty
        rectangle.

        :return: bounds (0, 0, 0, 0)
        )docstring")
    .def_static("MakeWH", &SkRect::MakeWH,
        R"docstring(
        Returns constructed :py:class:`Rect` set to :py:class:`Scalar` values
        (0, 0, w, h).

        Does not validate input; w or h may be negative.

        Passing integer values may generate a compiler warning since
        :py:class:`Rect` cannot represent 32-bit integers exactly. Use
        :py:class:`IRect` for an exact integer rectangle.

        :param w: Scalar width of constructed :py:class:`Rect`
        :param h: Scalar height of constructed :py:class:`Rect`
        :return: bounds (0, 0, w, h)
        )docstring",
        py::arg("w"), py::arg("h"))
    .def_static("MakeIWH", &SkRect::MakeIWH,
        R"docstring(
        Returns constructed :py:class:`Rect` set to integer values (0, 0, w, h).

        Does not validate input; w or h may be negative.

        Use to avoid a compiler warning that input may lose precision when
        stored. Use :py:class:`IRect` for an exact integer rectangle.

        :param w: integer width of constructed :py:class:`Rect`
        :param h: integer height of constructed :py:class:`Rect`
        :return: bounds (0, 0, w, h)
        )docstring",
        py::arg("w"), py::arg("h"))
    .def_static("MakeSize", &SkRect::MakeSize,
        R"docstring(
        Returns constructed :py:class:`Rect` set to (0, 0, size.width(),
        size.height()).

        Does not validate input; size.width() or size.height() may be negative.

        :param size:  Scalar values for :py:class:`Rect` width and height
        :return: bounds (0, 0, size.width(), size.height())
        )docstring",
        py::arg("size"))
    .def_static("MakeLTRB", &SkRect::MakeLTRB,
        R"docstring(
        Returns constructed :py:class:`Rect` set to (l, t, r, b).

        Does not sort input; :py:class:`Rect` may result in fLeft greater than
        fRight, or fTop greater than fBottom.

        :param l:  Scalar stored in fLeft
        :param t:  Scalar stored in fTop
        :param r:  Scalar stored in fRight
        :param b:  Scalar stored in fBottom
        :return: bounds (l, t, r, b)
        )docstring",
        py::arg("l"), py::arg("t"), py::arg("r"), py::arg("b"))
    .def_static("MakeXYWH", &SkRect::MakeXYWH,
        R"docstring(
        Returns constructed :py:class:`Rect` set to (x, y, x + w, y + h).

        Does not validate input; w or h may be negative.

        :param x:  stored in fLeft
        :param y:  stored in fTop
        :param w:  added to x and stored in fRight
        :param h:  added to y and stored in fBottom
        :return: bounds at (x, y) with width w and height h
        )docstring",
        py::arg("x"), py::arg("y"), py::arg("w"), py::arg("h"))
    .def_static("Make", py::overload_cast<const SkISize&>(&SkRect::Make),
        R"docstring(
        Returns constructed :py:class:`Rect` set to (0, 0, size.width(),
        size.height()).

        Does not validate input; size.width() or size.height() may be negative.

        :size: integer values for :py:class:`Rect` width and height
        :return: bounds (0, 0, size.width(), size.height())
        )docstring",
        py::arg("size"))
    .def_static("Make", py::overload_cast<const SkIRect&>(&SkRect::Make),
        R"docstring(
        Returns constructed :py:class:`Rect` set to irect, promoting integers
        to scalar.

        Does not validate input; fLeft may be greater than fRight, fTop may be
        greater than fBottom.

        :irect: integer unsorted bounds
        :return: irect members converted to Scalar
        )docstring",
        py::arg("irect"))
    .def_static("Intersects",
        py::overload_cast<const SkRect&, const SkRect&>(&SkRect::Intersects),
        R"docstring(
        Returns true if a intersects b.

        Returns false if either a or b is empty, or do not intersect.

        :param a:  :py:class:`Rect` to intersect
        :param b:  :py:class:`Rect` to intersect
        :return: true if a and b have area in common
        )docstring",
        py::arg("a"), py::arg("b"))
    .def_readwrite("fLeft", &SkRect::fLeft, "smaller x-axis bounds")
    .def_readwrite("fTop", &SkRect::fTop, "smaller y-axis bounds")
    .def_readwrite("fRight", &SkRect::fRight, "larger x-axis bounds")
    .def_readwrite("fBottom", &SkRect::fBottom, "larger y-axis bounds")
    .def(py::self == py::self,
        R"docstring(
        Returns true if all members in self: fLeft, fTop, fRight, and fBottom;
        are equal to the corresponding members in other.

        self and other are not equal if either contain NaN. self and other are
        equal if members contain zeroes with different signs.

        :param other: :py:class:`Rect` to compare
        :return: true if members are equal
        )docstring",
        py::arg("other"))
    .def(py::self != py::self,
        R"docstring(
        Returns true if any in self: fLeft, fTop, fRight, and fBottom; does not
        equal to the corresponding members in other.

        self and other are not equal if either contain NaN. self and other are
        equal if members contain zeroes with different signs.

        :param other: :py:class:`Rect` to compare
        :return: true if members are not equal
        )docstring",
        py::arg("other"))
    ;

py::implicitly_convertible<py::tuple, SkRect>();

py::class_<SkRRect> rrect(m, "RRect", R"docstring(
    :py:class:`RRect` describes a rounded rectangle with a bounds and a pair of
    radii for each corner.

    The bounds and radii can be set so that :py:class:`RRect` describes: a
    rectangle with sharp corners; a circle; an oval; or a rectangle with one or
    more rounded corners.

    :py:class:`RRect` allows implementing CSS properties that describe rounded
    corners. :py:class:`RRect` may have up to eight different radii, one for
    each axis on each of its four corners.

    :py:class:`RRect` may modify the provided parameters when initializing
    bounds and radii. If either axis radii is zero or less: radii are stored as
    zero; corner is square. If corner curves overlap, radii are proportionally
    reduced to fit within bounds.
    )docstring");

py::enum_<SkRRect::Type>(rrect, "Type")
    .value("kEmpty_Type", SkRRect::Type::kEmpty_Type, "zero width or height")
    .value("kRect_Type", SkRRect::Type::kRect_Type,
        "non-zero width and height, and zeroed radii")
    .value("kOval_Type", SkRRect::Type::kOval_Type,
        "non-zero width and height filled with radii")
    .value("kSimple_Type", SkRRect::Type::kSimple_Type,
        "non-zero width and height with equal radii")
    .value("kNinePatch_Type", SkRRect::Type::kNinePatch_Type,
        "non-zero width and height with axis-aligned radii")
    .value("kComplex_Type", SkRRect::Type::kComplex_Type,
        "non-zero width and height with arbitrary radii")
    .value("kLastType", SkRRect::Type::kLastType, "largest Type value")
    .export_values();

py::enum_<SkRRect::Corner>(rrect, "Corner")
    .value("kUpperLeft_Corner", SkRRect::Corner::kUpperLeft_Corner,
        "index of top-left corner radii")
    .value("kUpperRight_Corner", SkRRect::Corner::kUpperRight_Corner,
        "index of top-right corner radii")
    .value("kLowerRight_Corner", SkRRect::Corner::kLowerRight_Corner,
        "index of bottom-right corner radii")
    .value("kLowerLeft_Corner", SkRRect::Corner::kLowerLeft_Corner,
        "index of bottom-left corner radii")
    .export_values();

rrect
    .def(py::init(),
        R"docstring(
        Initializes bounds at (0, 0), the origin, with zero width and height.

        Initializes corner radii to (0, 0), and sets type of kEmpty_Type.

        :return: empty :py:class:`RRect`
        )docstring")
    .def(py::init<const SkRRect&>(),
        R"docstring(
        Initializes to copy of rrect bounds and corner radii.

        :rrect: bounds and corner to copy
        :return: copy of rrect
        )docstring",
        py::arg("rrect"))
    .def(py::init(&SkRRect::MakeRectXY),
        R"docstring(
        Initializes to rounded rectangle with the same radii for all four
        corners.

        If rect is empty, sets to kEmpty_Type. Otherwise, if xRad and yRad are
        zero, sets to kRect_Type. Otherwise, if xRad is at least half
        rect.width() and yRad is at least half rect.height(), sets to
        kOval_Type. Otherwise, sets to kSimple_Type.

        :rect:    bounds of rounded rectangle
        :xRad:    x-axis radius of corners
        :yRad:    y-axis radius of corners
        :return: rounded rectangle
        )docstring",
        py::arg("rect"), py::arg("xRad"), py::arg("yRad"))
    .def("__repr__",
        [] (const SkRRect& r) {
            std::stringstream s;
            auto rect = r.rect();
            s << "RRect(" << rect.fLeft << ", " << rect.fTop << ", "
                << rect.fRight << ", " << rect.fBottom << ", "
                << r.type() << ")";
            return s.str();
        })
    .def("getType", &SkRRect::getType)
    .def("type", &SkRRect::type)
    .def("isEmpty", &SkRRect::isEmpty)
    .def("isRect", &SkRRect::isRect)
    .def("isOval", &SkRRect::isOval)
    .def("isSimple", &SkRRect::isSimple)
    .def("isNinePatch", &SkRRect::isNinePatch)
    .def("isComplex", &SkRRect::isComplex)
    .def("width", &SkRRect::width,
        R"docstring(
        Returns span on the x-axis.

        This does not check if result fits in 32-bit float; result may be
        infinity.

        :return: rect().fRight minus rect().fLeft
        )docstring")
    .def("height", &SkRRect::height,
        R"docstring(
        Returns span on the y-axis.

        This does not check if result fits in 32-bit float; result may be
        infinity.

        :return: rect().fBottom minus rect().fTop
        )docstring")
    .def("getSimpleRadii", &SkRRect::getSimpleRadii,
        R"docstring(
        Returns top-left corner radii.

        If :py:meth:`type` returns kEmpty_Type, kRect_Type, kOval_Type, or
        kSimple_Type, returns a value representative of all corner radii. If
        :py:meth:`type` returns kNinePatch_Type or kComplex_Type, at least one
        of the remaining three corners has a different value.

        :return: corner radii for simple types
        )docstring")
    .def("setEmpty", &SkRRect::setEmpty,
        R"docstring(
        Sets bounds to zero width and height at (0, 0), the origin.

        Sets corner radii to zero and sets type to kEmpty_Type.
        )docstring")
    .def("setRect", &SkRRect::setRect,
        R"docstring(
        Sets bounds to sorted rect, and sets corner radii to zero.

        If set bounds has width and height, and sets type to kRect_Type;
        otherwise, sets type to kEmpty_Type.

        :param rect:    bounds to set
        )docstring",
        py::arg("rect"))
    .def("setOval", &SkRRect::setOval,
        R"docstring(
        Sets bounds to oval, x-axis radii to half oval.width(), and all y-axis
        radii to half oval.height().

        If oval bounds is empty, sets to kEmpty_Type. Otherwise, sets to
        kOval_Type.

        :param oval:    bounds of oval
        )docstring",
        py::arg("oval"))
    .def("setRectXY", &SkRRect::setRectXY,
        R"docstring(
        Sets to rounded rectangle with the same radii for all four corners.

        If rect is empty, sets to kEmpty_Type. Otherwise, if xRad or yRad is
        zero, sets to kRect_Type. Otherwise, if xRad is at least half
        rect.width() and yRad is at least half rect.height(), sets to
        kOval_Type. Otherwise, sets to kSimple_Type.

        :param rect:  bounds of rounded rectangle
        :param xRad:  x-axis radius of corners
        :param yRad:  y-axis radius of corners
        )docstring",
        py::arg("rect"), py::arg("xRad"), py::arg("yRad"))
    .def("setNinePatch", &SkRRect::setNinePatch,
        R"docstring(
        Sets bounds to rect.

        Sets radii to (leftRad, topRad), (rightRad, topRad), (rightRad,
        bottomRad), (leftRad, bottomRad).

        If rect is empty, sets to kEmpty_Type. Otherwise, if leftRad and
        rightRad are zero, sets to kRect_Type. Otherwise, if topRad and
        bottomRad are zero, sets to kRect_Type. Otherwise, if leftRad and
        rightRad are equal and at least half rect.width(), and topRad and
        bottomRad are equal at least half rect.height(), sets to kOval_Type.
        Otherwise, if leftRad and rightRad are equal, and topRad and bottomRad
        are equal, sets to kSimple_Type. Otherwise, sets to kNinePatch_Type.

        Nine patch refers to the nine parts defined by the radii: one center
        rectangle, four edge patches, and four corner patches.

        :param rect:    bounds of rounded rectangle
        :param leftRad: left-top and left-bottom x-axis radius
        :param topRad:  left-top and right-top y-axis radius
        :param rightRad:    right-top and right-bottom x-axis radius
        :param bottomRad:   left-bottom and right-bottom y-axis radius
        )docstring",
        py::arg("rect"), py::arg("leftRad"), py::arg("topRad"),
        py::arg("rightRad"), py::arg("bottomRad"))
    .def("setRectRadii",
        [] (SkRRect& rrect, const SkRect& rect,
            const std::vector<SkVector>& radii) {
            if (radii.size() != 4)
                throw py::value_error("radii must have 4 elements");
            rrect.setRectRadii(rect, &radii[0]);
        },
        R"docstring(
        Sets bounds to rect.

        Sets radii array for individual control of all for corners.

        If rect is empty, sets to kEmpty_Type. Otherwise, if one of each corner
        radii are zero, sets to kRect_Type. Otherwise, if all x-axis radii are
        equal and at least half rect.width(), and all y-axis radii are equal at
        least half rect.height(), sets to kOval_Type. Otherwise, if all x-axis
        radii are equal, and all y-axis radii are equal, sets to kSimple_Type.
        Otherwise, sets to kNinePatch_Type.

        :param rect:    bounds of rounded rectangle
        :param radii:   corner x-axis and y-axis radii
        )docstring",
        py::arg("rect"), py::arg("radii"))
    .def("rect", &SkRRect::rect,
        R"docstring(
        Returns bounds.

        Bounds may have zero width or zero height. Bounds right is greater than
        or equal to left; bounds bottom is greater than or equal to top. Result
        is identical to :py:meth:`getBounds`.

        :return: bounding box
        )docstring")
    .def("radii",  py::overload_cast<SkRRect::Corner>(&SkRRect::radii, py::const_),
        R"docstring(
        Returns scalar pair for radius of curve on x-axis and y-axis for one
        corner.

        Both radii may be zero. If not zero, both are positive and finite.

        :return: x-axis and y-axis radii for one corner
        )docstring",
        py::arg("corner"))
    .def("getBounds", &SkRRect::getBounds,
        R"docstring(
        Returns bounds.

        Bounds may have zero width or zero height. Bounds right is greater than
        or equal to left; bounds bottom is greater than or equal to top. Result
        is identical to :py:meth:`rect`.

        :return: bounding box
        )docstring")
    .def("inset",
        py::overload_cast<SkScalar, SkScalar, SkRRect*>(
            &SkRRect::inset, py::const_),
        R"docstring(
        Copies :py:class:`RRect` to dst, then insets dst bounds by dx and dy,
        and adjusts dst radii by dx and dy.

        dx and dy may be positive, negative, or zero. dst may be
        :py:class:`RRect`.

        If either corner radius is zero, the corner has no curvature and is
        unchanged. Otherwise, if adjusted radius becomes negative, pins radius
        to zero. If dx exceeds half dst bounds width, dst bounds left and right
        are set to bounds x-axis center. If dy exceeds half dst bounds height,
        dst bounds top and bottom are set to bounds y-axis center.

        If dx or dy cause the bounds to become infinite, dst bounds is zeroed.

        :dx:  added to rect().fLeft, and subtracted from rect().fRight
        :dy:  added to rect().fTop, and subtracted from rect().fBottom
        :dst: insets bounds and radii
        )docstring",
        py::arg("dx"), py::arg("dy"), py::arg("dst"))
    .def("inset",
        py::overload_cast<SkScalar, SkScalar>(&SkRRect::inset),
        R"docstring(
        Insets bounds by dx and dy, and adjusts radii by dx and dy.

        dx and dy may be positive, negative, or zero.

        If either corner radius is zero, the corner has no curvature and is
        unchanged. Otherwise, if adjusted radius becomes negative, pins radius
        to zero. If dx exceeds half bounds width, bounds left and right are set
        to bounds x-axis center. If dy exceeds half bounds height, bounds top
        and bottom are set to bounds y-axis center.

        If dx or dy cause the bounds to become infinite, bounds is zeroed.

        :dx:  added to rect().fLeft, and subtracted from rect().fRight
        :dy:  added to rect().fTop, and subtracted from rect().fBottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("outset",
        py::overload_cast<SkScalar, SkScalar, SkRRect*>(
            &SkRRect::outset, py::const_),
        R"docstring(
        Outsets dst bounds by dx and dy, and adjusts radii by dx and dy.

        dx and dy may be positive, negative, or zero.

        If either corner radius is zero, the corner has no curvature and is
        unchanged. Otherwise, if adjusted radius becomes negative, pins radius
        to zero. If dx exceeds half dst bounds width, dst bounds left and right
        are set to bounds x-axis center. If dy exceeds half dst bounds height,
        dst bounds top and bottom are set to bounds y-axis center.

        If dx or dy cause the bounds to become infinite, dst bounds is zeroed.

        :dx:  subtracted from rect().fLeft, and added to rect().fRight
        :dy:  subtracted from rect().fTop, and added to rect().fBottom
        :dst: outset bounds and radii
        )docstring",
        py::arg("dx"), py::arg("dy"), py::arg("dst"))
    .def("outset",
        py::overload_cast<SkScalar, SkScalar>(&SkRRect::outset),
        R"docstring(
        Outsets bounds by dx and dy, and adjusts radii by dx and dy.

        dx and dy may be positive, negative, or zero.

        If either corner radius is zero, the corner has no curvature and is
        unchanged. Otherwise, if adjusted radius becomes negative, pins radius
        to zero. If dx exceeds half bounds width, bounds left and right are set
        to bounds x-axis center. If dy exceeds half bounds height, bounds top
        and bottom are set to bounds y-axis center.

        If dx or dy cause the bounds to become infinite, bounds is zeroed.

        :dx:  subtracted from rect().fLeft, and added to rect().fRight
        :dy:  subtracted from rect().fTop, and added to rect().fBottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("offset", &SkRRect::offset,
        R"docstring(
        Translates :py:class:`RRect` by (dx, dy).

        :param dx:  offset added to rect().fLeft and rect().fRight
        :parma dy:  offset added to rect().fTop and rect().fBottom
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("makeOffset", &SkRRect::makeOffset,
        R"docstring(
        Returns :py:class:`RRect` translated by (dx, dy).

        :param dx:  offset added to rect().fLeft and rect().fRight
        :param dy:  offset added to rect().fTop and rect().fBottom
        :return: :py:class:`RRect` bounds offset by (dx, dy), with unchanged
            corner radii
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("contains", &SkRRect::contains,
        R"docstring(
        Returns true if rect is inside the bounds and corner radii, and if
        :py:class:`RRect` and rect are not empty.

        :param rect:  area tested for containment
        :return: true if SkRRect contains rect
        )docstring",
        py::arg("rect"))
    .def("isValid", &SkRRect::isValid,
        R"docstring(
        Returns true if bounds and radii values are finite and describe a
        :py:class:`RRect` :py:class:`RRect.Type` that matches
        :py:meth:`getType`.

        All :py:class:`RRect` methods construct valid types, even if the input
        values are not valid. Invalid :py:class:`RRect` data can only be
        generated by corrupting memory.

        :return: true if bounds and radii match :py:meth:`type`
        )docstring")
    .def("writeToMemory", [] (const SkRRect& rrect) {
            std::vector<char> buffer(SkRRect::kSizeInMemory);
            rrect.writeToMemory(&buffer[0]);
            return py::bytes(&buffer[0], buffer.size());
        },
        R"docstring(
        Writes :py:class:`RRect` to buffer.

        Writes kSizeInMemory bytes, and returns kSizeInMemory, the number of
        bytes written.

        :return: bytes written, kSizeInMemory
        )docstring")
    .def("readFromMemory",
        [] (SkRRect& rrect, const std::string& buffer) {
            return rrect.readFromMemory(&buffer[0], buffer.size());
        },
        R"docstring(
        Reads :py:class:`RRect` from buffer, reading kSizeInMemory bytes.

        Returns kSizeInMemory, bytes read if length is at least kSizeInMemory.
        Otherwise, returns zero.

        :param buffer:  memory to read from
        :return: bytes read, or 0 if length is less than kSizeInMemory
        )docstring",
        py::arg("buffer"))
    .def("transform", &SkRRect::transform,
        R"docstring(
        Transforms by :py:class:`RRect` by matrix, storing result in dst.

        Returns true if :py:class:`RRect` transformed can be represented by
        another :py:class:`RRect`. Returns false if matrix contains
        transformations that are not axis aligned.

        Asserts in debug builds if :py:class:`RRect` equals dst.

        :param matrix:  :py:class:`Matrix` specifying the transform
        :param dst: :py:class:`RRect` to store the result
        :return: true if transformation succeeded.
        )docstring",
        py::arg("matrix"), py::arg("dst"))
    .def("dump",
        [] (const SkRRect& rrect, bool asHex) {
            py::scoped_ostream_redirect stream;
            rrect.dump(asHex);
        },
        R"docstring(
        Writes text representation of :py:class:`RRect` to standard output.

        Set asHex true to generate exact binary representations of floating
        point numbers.

        :param asHex:  true if Scalar values are written as hexadecimal
        )docstring",
        py::arg("asHex") = false)
    .def("dumpToString", &SkRRect::dumpToString, py::arg("asHex"))
    .def("dumpHex",
        [] (const SkRRect& rrect) {
            py::scoped_ostream_redirect stream;
            rrect.dumpHex();
        },
        R"docstring(
        Writes text representation of :py:class:`RRect` to standard output.

        The representation may be directly compiled as C++ code. Floating point
        values are written in hexadecimal to preserve their exact bit pattern.
        The output reconstructs the original :py:class:`RRect`.
        )docstring")
    .def_static("MakeEmpty", &SkRRect::MakeEmpty,
        R"docstring(
        Initializes bounds at (0, 0), the origin, with zero width and height.

        Initializes corner radii to (0, 0), and sets type of kEmpty_Type.

        :return: empty :py:class:`RRect`
        )docstring")
    .def_static("MakeRect", &SkRRect::MakeRect,
        R"docstring(
        Initializes to copy of r bounds and zeroes corner radii.

        :param r: bounds to copy
        :return: copy of r
        )docstring",
        py::arg("r"))
    .def_static("MakeOval", &SkRRect::MakeOval,
        R"docstring(
        Sets bounds to oval, x-axis radii to half oval.width(), and all y-axis
        radii to half oval.height().

        If oval bounds is empty, sets to kEmpty_Type. Otherwise, sets to
        kOval_Type.

        :param oval:    bounds of oval
        :return: oval
        )docstring",
        py::arg("oval"))
    .def_static("MakeRectXY", &SkRRect::MakeRectXY,
        R"docstring(
        Sets to rounded rectangle with the same radii for all four corners.

        If rect is empty, sets to kEmpty_Type. Otherwise, if xRad and yRad are
        zero, sets to kRect_Type. Otherwise, if xRad is at least half
        rect.width() and yRad is at least half rect.height(), sets to
        kOval_Type. Otherwise, sets to kSimple_Type.

        :param rect:    bounds of rounded rectangle
        :param xRad:    x-axis radius of corners
        :param yRad:    y-axis radius of corners
        :return: rounded rectangle
        )docstring",
        py::arg("rect"), py::arg("xRad"), py::arg("yRad"))
    .def_readonly_static("kSizeInMemory", &SkRRect::kSizeInMemory)
    .def(py::self == py::self,
        R"docstring(
        Returns true if bounds and radii in self are equal to bounds and radii
        in other.

        self and other are not equal if either contain NaN. self and other are
        equal if members contain zeroes with different signs.

        :param other: :py:class:`Rect` bounds and radii to compare
        :return: true if members are equal
        )docstring",
        py::arg("other"))
    .def(py::self != py::self,
        R"docstring(
        Returns true if bounds and radii in self are not equal to bounds and
        radii in other.

        self and other are not equal if either contain NaN. self and other are
        equal if members contain zeroes with different signs.

        :param other: :py:class:`Rect` bounds and radii to compare
        :return: true if members are not equal
        )docstring",
        py::arg("other"))
    ;
}
