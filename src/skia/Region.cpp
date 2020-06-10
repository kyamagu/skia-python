#include "common.h"
#include <pybind11/operators.h>
#include <pybind11/stl.h>

const int SkRegion::kOpCnt;

namespace {

template <typename A, SkRegion::Op op>
SkRegion& ApplyOp1(SkRegion& self, const A& other) {
    self.op(other, op);
    return self;
}

template <typename A, typename B, SkRegion::Op op>
SkRegion ApplyOp2(const A& self, const B& other) {
    SkRegion region;
    region.op(self, other, op);
    return region;
}

}  // namespace

void initRegion(py::module &m) {
py::class_<SkRegion> region(m, "Region", R"docstring(
    :py:class:`Region` describes the set of pixels used to clip
    :py:class:`Canvas`.

    :py:class:`Region` is compact, efficiently storing a single integer
    rectangle, or a run length encoded array of rectangles. :py:class:`Region`
    may reduce the current :py:class:`Canvas` clip, or may be drawn as one or
    more integer rectangles. :py:class:`Region` iterator returns the scan lines
    or rectangles contained by it, optionally intersecting a bounding rectangle.

    :py:class:`Region` supports a few operators::

        regionA == regionB  # Equality
        regionA != regionB  # Inequality

        regionA - regionB   # Difference
        regionA & regionB   # Intersect
        regionA | regionB   # Union
        regionA ^ regionB   # XOR

        regionA -= regionB  # In-place Difference
        regionA &= regionB  # In-place Intersect
        regionA |= regionB  # In-place Union
        regionA ^= regionB  # In-place XOR
    )docstring");

py::class_<SkRegion::Cliperator>(region, "Cliperator", R"docstring(
    Returns the sequence of rectangles, sorted along y-axis, then x-axis, that
    make up :py:class:`Region` intersected with the specified clip rectangle.
    )docstring")
    .def(py::init<const SkRegion&, const SkIRect&>(),
        R"docstring(
        Sets :py:class:`~Region.Cliperator` to return elements of
        :py:class:`IRect` array in :py:class:`Region` within clip.

        :param skia.Region region: :py:class:`Region` to iterate
        :param skia.IRect clip: bounds of iteration
        :return: :py:class:`Region` iterator
        )docstring",
        py::arg("region"), py::arg("clip"))
    .def("done", &SkRegion::Cliperator::done,
        R"docstring(
        Returns true if :py:class:`~Region.Cliperator` is pointing to final
        :py:class:`IRect` in :py:class:`Region`.

        :return: true if data parsing is complete
        )docstring")
    .def("next", &SkRegion::Cliperator::next,
        R"docstring(
        Advances iterator to next :py:class:`IRect` in :py:class:`Region`
        contained by clip.
        )docstring")
    .def("rect", &SkRegion::Cliperator::rect,
        R"docstring(
        Returns :py:class:`IRect` element in :py:class:`Region`, intersected
        with clip passed to :py:class:`~Region.Cliperator` constructor.

        Does not return predictable results if :py:class:`Region` is empty.

        :return: part of :py:class:`Region` inside clip as :py:class:`IRect`
        )docstring")
    .def("__iter__",
        [] (const SkRegion::Cliperator& it) { return it; })
    .def("__next__",
        [] (SkRegion::Cliperator& it) {
            if (it.done())
                throw py::stop_iteration();
            auto rect = it.rect();
            it.next();
            return rect;
        })
    ;

py::class_<SkRegion::Iterator>(region, "Iterator", R"docstring(
    Returns sequence of rectangles, sorted along y-axis, then x-axis, that make
    up :py:class:`Region`.
    )docstring")
    .def(py::init<>(),
        R"docstring(
        Initializes :py:class:`~Region.Iterator` with an empty
        :py:class:`Region`.

        :py:meth:`done` on :py:class:`~Region.Iterator` returns true. Call
        :py:meth:`reset` to initialize :py:class:`~Region.Iterator` at a later
        time.

        :return: empty :py:class:`Region` iterator
        )docstring")
    .def(py::init<const SkRegion&>(),
        R"docstring(
        Sets :py:class:`~Region.Iterator` to return elements of
        :py:class:`IRect` array in region.

        :region: :py:class:`Region` to iterate
            :py:class:`Region` iterator
        )docstring",
        py::arg("region"))
    .def("rewind", &SkRegion::Iterator::rewind,
        R"docstring(
        :py:class:`Point` :py:class:`~Region.Iterator` to start of
        :py:class:`Region`.

        Returns true if :py:class:`Region` was set; otherwise, returns false.

        :return: true if :py:class:`Region` was set
        )docstring")
    .def("reset", &SkRegion::Iterator::reset,
        R"docstring(
        Resets iterator, using the new :py:class:`Region`.

        :param region: :py:class:`Region` to iterate
        )docstring",
        py::arg("region"))
    .def("done", &SkRegion::Iterator::done,
        R"docstring(
        Returns true if :py:class:`~Region.Iterator` is pointing to final
        :py:class:`IRect` in :py:class:`Region`.

        :return: true if data parsing is complete
        )docstring")
    .def("next", &SkRegion::Iterator::next,
        R"docstring(
        Advances :py:class:`~Region.Iterator` to next :py:class:`IRect` in
        :py:class:`Region` if it is not done.
        )docstring")
    .def("rect", &SkRegion::Iterator::rect,
        R"docstring(
        Returns :py:class:`IRect` element in :py:class:`Region`.

        Does not return predictable results if :py:class:`Region` is empty.

        :return: part of :py:class:`Region` as :py:class:`IRect`
        )docstring")
    .def("rgn", &SkRegion::Iterator::rgn,
        R"docstring(
        Returns :py:class:`Region` if set; otherwise, returns nullptr.

        :return: iterated :py:class:`Region`
        )docstring")
    .def("__iter__",
        [] (const SkRegion::Iterator& it) { return it; })
    .def("__next__",
        [] (SkRegion::Iterator& it) {
            if (it.done())
                throw py::stop_iteration();
            auto rect = it.rect();
            it.next();
            return rect;
        })
    ;

py::class_<SkRegion::Spanerator>(region, "Spanerator", R"docstring(
    Returns the line segment ends within :py:class:`Region` that intersect a
    horizontal line.
    )docstring")
    .def(py::init<const SkRegion&, int, int, int>(),
        R"docstring(
        Sets :py:class:`Region.Spanerator` to return line segments in
        :py:class:`Region` on scan line.

        :param skia.Region region: :py:class:`Region` to iterate
        :param int y: horizontal line to intersect
        :param int left:  bounds of iteration
        :param int right: bounds of iteration
        :return: :py:class:`Region` iterator
        )docstring",
        py::arg("region"), py::arg("y"), py::arg("left"), py::arg("right"))
    .def("next", &SkRegion::Spanerator::next,
        R"docstring(
        Advances iterator to next span intersecting :py:class:`Region` within
        line segment provided in constructor.

        Returns true if interval was found.

        :param int left: pointer to span start; may be nullptr
        :param int right: pointer to span end; may be nullptr
        :return: true if interval was found
        )docstring",
        py::arg("left"), py::arg("right"))
    .def("__iter__",
        [] (const SkRegion::Spanerator& it) { return it; })
    .def("__next__",
        [] (SkRegion::Spanerator& it) {
            int left, right;
            if (it.next(&left, &right))
                return py::make_tuple(left, right);
            throw py::stop_iteration();
        })
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
    .def(py::init<>(),
        R"docstring(
        Constructs an empty :py:class:`Region`.

        :py:class:`Region` is set to empty bounds at (0, 0) with zero width and
        height.
        )docstring")
    .def(py::init<const SkRegion&>(),
        R"docstring(
        Constructs a copy of an existing region.

        Copy constructor makes two regions identical by value. Internally,
        region and the returned result share pointer values. The underlying
        :py:class:`Rect` array is copied when modified.

        Creating a :py:class:`Region` copy is very efficient and never allocates
        memory. :py:class:`Region` are always copied by value from the
        interface; the underlying shared pointers are not exposed.

        :region: :py:class:`Region` to copy by value
        )docstring",
        py::arg("region"))
    .def(py::init<const SkIRect&>(),
        R"docstring(
        Constructs a rectangular :py:class:`Region` matching the bounds of rect.

        :rect: bounds of constructed :py:class:`Region`
        )docstring",
        py::arg("rect"))
    .def(py::self == py::self,
        R"docstring(
        Compares :py:class:`Region` and other; returns true if they enclose
        exactly the same area.

        :param skia.Region other: :py:class:`Region` to compare
        :return: true if :py:class:`Region` pair are equivalent
        )docstring",
        py::arg("other"))
    .def(py::self != py::self,
        R"docstring(
        Compares :py:class:`Region` and other; returns true if they do not
        enclose the same area..

        :param skia.Region other: :py:class:`Region` to compare
        :return: true if :py:class:`Region` pair are not equivalent
        )docstring",
        py::arg("other"))
    .def("set", &SkRegion::set,
        R"docstring(
        Sets :py:class:`Region` to src, and returns true if src bounds is not
        empty.

        This makes :py:class:`Region` and src identical by value. Internally,
        :py:class:`Region` and src share pointer values. The underlying
        :py:class:`Rect` array is copied when modified.

        Creating a :py:class:`Region` copy is very efficient and never allocates
        memory. :py:class:`Region` are always copied by value from the
        interface; the underlying shared pointers are not exposed.

        :param skia.Region src: :py:class:`Region` to copy
        :return: copy of src
        )docstring",
        py::arg("src"))
    .def("swap", &SkRegion::swap,
        R"docstring(
        Exchanges :py:class:`IRect` array of :py:class:`Region` and other.

        swap() internally exchanges pointers, so it is lightweight and does not
        allocate memory.

        :py:class:`Path` do not copy their content on assignment until they are
        written to, making assignment as efficient as swap().

        :param skia.Region other: other region to swap
        )docstring",
        py::arg("other"))
    .def("isEmpty", &SkRegion::isEmpty,
        R"docstring(
        Returns true if :py:class:`Region` is empty.

        Empty :py:class:`Region` has bounds width or height less than or equal
        to zero. :py:meth:`__init__` constructs empty :py:class:`Region`;
        :py:meth:`setEmpty` and :py:meth:`setRect` with dimensionless data make
        :py:class:`Region` empty.

        :return: true if bounds has no width or height
        )docstring")
    .def("isRect", &SkRegion::isRect,
        R"docstring(
        Returns true if :py:class:`Region` is one :py:class:`IRect` with
        positive dimensions.

        :return: true if :py:class:`Region` contains one :py:class:`IRect`
        )docstring")
    .def("isComplex", &SkRegion::isComplex,
        R"docstring(
        Returns true if :py:class:`Region` is described by more than one
        rectangle.

        :return: true if :py:class:`Region` contains more than one
            :py:class:`IRect`
        )docstring")
    .def("getBounds", &SkRegion::getBounds,
        R"docstring(
        Returns minimum and maximum axes values of :py:class:`IRect` array.

        Returns (0, 0, 0, 0) if :py:class:`Region` is empty.

        :return: combined bounds of all :py:class:`IRect` elements
        )docstring")
    .def("computeRegionComplexity", &SkRegion::computeRegionComplexity,
        R"docstring(
        Returns a value that increases with the number of elements in
        :py:class:`Region`.

        Returns zero if :py:class:`Region` is empty. Returns one if
        :py:class:`Region` equals :py:class:`IRect`; otherwise, returns value
        greater than one indicating that :py:class:`Region` is complex.

        Call to compare :py:class:`Region` for relative complexity.

        :return: relative complexity
        )docstring")
    .def("getBoundaryPath", &SkRegion::getBoundaryPath,
        R"docstring(
        Appends outline of :py:class:`Region` to path.

        Returns true if :py:class:`Region` is not empty; otherwise, returns
        false, and leaves path unmodified.

        :param skia.Path path: :py:class:`Path` to append to
        :return: true if path changed
        )docstring",
        py::arg("path"))
    .def("setEmpty", &SkRegion::setEmpty,
        R"docstring(
        Constructs an empty :py:class:`Region`.

        :py:class:`Region` is set to empty bounds at (0, 0) with zero width and
        height. Always returns false.

        :return: false
        )docstring")
    .def("setRect", &SkRegion::setRect,
        R"docstring(
        Constructs a rectangular :py:class:`Region` matching the bounds of rect.

        If rect is empty, constructs empty and returns false.

        :param skia.Rect rect: bounds of constructed :py:class:`Region`
        :return: true if rect is not empty
        )docstring",
        py::arg("rect"))
    .def("setRects",
        [] (SkRegion& region, const std::vector<SkIRect>& rects) {
            return region.setRects(&rects[0], rects.size());
        },
        R"docstring(
        Constructs :py:class:`Region` as the union of :py:class:`IRect` in rects
        array.

        If count is zero, constructs empty :py:class:`Region`. Returns false if
        constructed :py:class:`Region` is empty.

        May be faster than repeated calls to :py:meth:`op`.

        :param List[skia.IRect] rects: array of :py:class:`IRect`
        :return: true if constructed :py:class:`Region` is not empty
        )docstring",
        py::arg("rects"))
    .def("setRegion", &SkRegion::setRegion,
        R"docstring(
        Constructs a copy of an existing region.

        Makes two regions identical by value. Internally, region and the
        returned result share pointer values. The underlying :py:class:`Rect`
        array is copied when modified.

        Creating a :py:class:`Region` copy is very efficient and never allocates
        memory. :py:class:`Region` are always copied by value from the
        interface; the underlying shared pointers are not exposed.

        :param skia.Region region: :py:class:`Region` to copy by value
        :return: :py:class:`Region` to copy by value
        )docstring",
        py::arg("region"))
    .def("setPath", &SkRegion::setPath,
        R"docstring(
        Constructs :py:class:`Region` to match outline of path within clip.

        Returns false if constructed :py:class:`Region` is empty.

        Constructed :py:class:`Region` draws the same pixels as path through
        clip when anti-aliasing is disabled.

        :param skia.Path path: :py:class:`Path` providing outline
        :param skia.Region clip: :py:class:`Region` containing path
        :return: true if constructed :py:class:`Region` is not empty
        )docstring",
        py::arg("path"), py::arg("clip"))
    .def("intersects",
        py::overload_cast<const SkIRect&>(&SkRegion::intersects, py::const_),
        R"docstring(
        Returns true if :py:class:`Region` intersects rect.

        Returns false if either rect or :py:class:`Region` is empty, or do not
        intersect.

        :rect: :py:class:`IRect` to intersect
        :return: true if rect and :py:class:`Region` have area in common
        )docstring",
        py::arg("rect"))
    .def("intersects",
        py::overload_cast<const SkRegion&>(&SkRegion::intersects, py::const_),
        R"docstring(
        Returns true if :py:class:`Region` intersects other.

        Returns false if either other or :py:class:`Region` is empty, or do not
        intersect.

        :other: :py:class:`Region` to intersect
        :return: true if other and :py:class:`Region` have area in common
        )docstring",
        py::arg("other"))
    .def("contains",
        py::overload_cast<int32_t, int32_t>(&SkRegion::contains, py::const_),
        R"docstring(
        Returns true if :py:class:`IPoint` (x, y) is inside :py:class:`Region`.

        Returns false if :py:class:`Region` is empty.

        :x: test :py:class:`IPoint` x-coordinate
        :y: test :py:class:`IPoint` y-coordinate
        :return: true if (x, y) is inside :py:class:`Region`
        )docstring",
        py::arg("x"), py::arg("y"))
    .def("contains",
        py::overload_cast<const SkIRect&>(&SkRegion::contains, py::const_),
        R"docstring(
        Returns true if other is completely inside :py:class:`Region`.

        Returns false if :py:class:`Region` or other is empty.

        :other: :py:class:`IRect` to contain
        :return: true if other is inside :py:class:`Region`
        )docstring",
        py::arg("other"))
    .def("contains",
        py::overload_cast<const SkRegion&>(&SkRegion::contains, py::const_),
        R"docstring(
        Returns true if other is completely inside :py:class:`Region`.

        Returns false if :py:class:`Region` or other is empty.

        :other: :py:class:`Region` to contain
        :return: true if other is inside :py:class:`Region`
        )docstring",
        py::arg("other"))
    .def("quickContains", &SkRegion::quickContains,
        R"docstring(
        Returns true if :py:class:`Region` is a single rectangle and contains r.

        May return false even though :py:class:`Region` contains r.

        :param skia.IRect r: :py:class:`IRect` to contain
        :return: true quickly if r points are equal or inside
        )docstring",
        py::arg("r"))
    .def("quickReject",
        py::overload_cast<const SkIRect&>(&SkRegion::quickReject, py::const_),
        R"docstring(
        Returns true if :py:class:`Region` does not intersect rect.

        Returns true if rect is empty or :py:class:`Region` is empty. May return
        false even though :py:class:`Region` does not intersect rect.

        :rect: :py:class:`IRect` to intersect
        :return: true if rect does not intersect
        )docstring",
        py::arg("rect"))
    .def("quickReject",
        py::overload_cast<const SkRegion&>(&SkRegion::quickReject, py::const_),
        R"docstring(
        Returns true if :py:class:`Region` does not intersect rgn.

        Returns true if rgn is empty or :py:class:`Region` is empty. May return
        false even though :py:class:`Region` does not intersect rgn.

        :rgn: :py:class:`Region` to intersect
        :return: true if rgn does not intersect
        )docstring")
    .def("translate",
        [] (const SkRegion& region, int dx, int dy) {
            SkRegion dst;
            region.translate(dx, dy, &dst);
            return dst;
        },
        R"docstring(
        Offsets :py:class:`Region` by ivector (dx, dy).

        Has no effect if :py:class:`Region` is empty. If :py:class:`Region` is
        empty, returns empty region.

        :dx: x-axis offset
        :dy: y-axis offset
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("op",
        py::overload_cast<const SkIRect&, SkRegion::Op>(&SkRegion::op),
        R"docstring(
        Replaces :py:class:`Region` with the result of :py:class:`Region` op
        rect.

        Returns true if replaced :py:class:`Region` is not empty.

        :rect: :py:class:`IRect` operand
        :return: false if result is empty
        )docstring",
        py::arg("rect"), py::arg("op"))
    .def("op",
        py::overload_cast<const SkRegion&, SkRegion::Op>(&SkRegion::op),
        R"docstring(
        Replaces :py:class:`Region` with the result of :py:class:`Region` op
        region.

        Returns true if replaced :py:class:`Region` is not empty.

        :region: :py:class:`Region` operand
        :return: false if result is empty
        )docstring",
        py::arg("region"), py::arg("op"))
    .def("op",
        py::overload_cast<const SkIRect&, const SkRegion&, SkRegion::Op>(
            &SkRegion::op),
        R"docstring(
        Replaces :py:class:`Region` with the result of rect op region.

        Returns true if replaced :py:class:`Region` is not empty.

        :rect: :py:class:`IRect` operand
        :region: :py:class:`Region` operand
        :return: false if result is empty
        )docstring",
        py::arg("rect"), py::arg("region"), py::arg("op"))
    .def("op",
        py::overload_cast<const SkRegion&, const SkIRect&, SkRegion::Op>(
            &SkRegion::op),
        R"docstring(
        Replaces :py:class:`Region` with the result of region op rect.

        Returns true if replaced :py:class:`Region` is not empty.

        :region: :py:class:`Region` operand
        :rect: :py:class:`IRect` operand
        :return: false if result is empty
        )docstring",
        py::arg("region"), py::arg("rect"), py::arg("op"))
    .def("op",
        py::overload_cast<const SkRegion&, const SkRegion&, SkRegion::Op>(
            &SkRegion::op),
        R"docstring(
        Replaces :py:class:`Region` with the result of regionA op regionB.

        Returns true if replaced :py:class:`Region` is not empty.

        :regionA: :py:class:`Region` operand
        :regionB: :py:class:`Region` operand
        :return: false if result is empty
        )docstring",
        py::arg("regionA"), py::arg("regionB"), py::arg("op"))
    .def("writeToMemory",
        [] (const SkRegion& region) {
            auto size = region.writeToMemory(nullptr);
            auto data = SkData::MakeUninitialized(size);
            region.writeToMemory(data->writable_data());
            return data;
        },
        R"docstring(
        Writes :py:class:`Region` to :py:class:`Data`.

        :return: :py:class:`Data`
        )docstring")
    .def("readFromMemory",
        [] (SkRegion& region, const SkData& data) {
            return region.readFromMemory(data.data(), data.size());
        },
        R"docstring(
        Constructs :py:class:`Region` from buffer of size length.

        Returns bytes read. Returned value will be multiple of four or zero if
        length was too small.

        :param skia.Data data: binary data
        :return: bytes read
        )docstring",
        py::arg("data"))
    .def_readonly_static("kOpCnt", &SkRegion::kOpCnt)
    .def("__iter__",
        [] (const SkRegion& region) { return SkRegion::Iterator(region); })
    .def("iterator",
        [] (const SkRegion& region) { return SkRegion::Iterator(region); },
        R"docstring(
        Creates :py:class:`~Region.Iterator` to return elements of
        :py:class:`IRect` array in region.

        Example::

            for rect in region.iterator():
                pass
        )docstring")
    .def("cliperator",
        [] (const SkRegion& region, const SkIRect& clip) {
            return SkRegion::Cliperator(region, clip);
        },
        R"docstring(
        Creates :py:class:`~Region.Cliperator` to return elements of
        :py:class:`IRect` array in :py:class:`Region` within clip.

        Example::

            for rect in region.cliperator(skia.IRect(100, 100)):
                pass

        :param skia.IRect clip: bounds of iteration
        )docstring",
        py::arg("clip"))
    .def("spanerator",
        [] (const SkRegion& region, int y, int left, int right) {
            return SkRegion::Spanerator(region, y, left, right);
        },
        R"docstring(
        Creates :py:class:`Region.Spanerator` to return line segments in
        :py:class:`Region` on scan line.

        Example::

            for left, right in region.spanerator(5, 0, 100):
                pass

        :param int y: horizontal line to intersect
        :param int left:  bounds of iteration
        :param int right: bounds of iteration
        )docstring",
        py::arg("y"), py::arg("left"), py::arg("right"))
    .def("__sub__",
        &ApplyOp2<SkRegion, SkRegion, SkRegion::kDifference_Op>,
        py::is_operator())
    .def("__and__",
        &ApplyOp2<SkRegion, SkRegion, SkRegion::kIntersect_Op>,
        py::is_operator())
    .def("__or__",
        &ApplyOp2<SkRegion, SkRegion, SkRegion::kUnion_Op>,
        py::is_operator())
    .def("__xor__",
        &ApplyOp2<SkRegion, SkRegion, SkRegion::kXOR_Op>,
        py::is_operator())
    .def("__isub__",
        &ApplyOp1<SkRegion, SkRegion::kDifference_Op>,
        py::is_operator())
    .def("__iand__",
        &ApplyOp1<SkRegion, SkRegion::kIntersect_Op>,
        py::is_operator())
    .def("__ior__",
        &ApplyOp1<SkRegion, SkRegion::kUnion_Op>,
        py::is_operator())
    .def("__ixor__",
        &ApplyOp1<SkRegion, SkRegion::kXOR_Op>,
        py::is_operator())
    .def("__sub__",
        &ApplyOp2<SkRegion, SkIRect, SkRegion::kDifference_Op>,
        py::is_operator())
    .def("__and__",
        &ApplyOp2<SkRegion, SkIRect, SkRegion::kIntersect_Op>,
        py::is_operator())
    .def("__or__",
        &ApplyOp2<SkRegion, SkIRect, SkRegion::kUnion_Op>,
        py::is_operator())
    .def("__xor__",
        &ApplyOp2<SkRegion, SkIRect, SkRegion::kXOR_Op>,
        py::is_operator())
    .def("__isub__",
        &ApplyOp1<SkIRect, SkRegion::kDifference_Op>,
        py::is_operator())
    .def("__iand__",
        &ApplyOp1<SkIRect, SkRegion::kIntersect_Op>,
        py::is_operator())
    .def("__ior__",
        &ApplyOp1<SkIRect, SkRegion::kUnion_Op>,
        py::is_operator())
    .def("__ixor__",
        &ApplyOp1<SkIRect, SkRegion::kXOR_Op>,
        py::is_operator())
    ;

py::implicitly_convertible<SkIRect,SkRegion>();
}
