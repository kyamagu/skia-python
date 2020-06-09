#include "common.h"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>


template <typename T>
py::tuple Iter_next(T& it) {
    std::vector<SkPoint> pts(4);
    SkPath::Verb result = it.next(&pts[0]);
    switch (result) {
        case SkPath::Verb::kDone_Verb:
            pts.erase(pts.begin(), pts.end());
            break;
        case SkPath::Verb::kMove_Verb:
        case SkPath::Verb::kClose_Verb:
            pts.erase(pts.begin() + 1, pts.end());
            break;
        case SkPath::Verb::kLine_Verb:
            pts.erase(pts.begin() + 2, pts.end());
            break;
        case SkPath::Verb::kQuad_Verb:
        case SkPath::Verb::kConic_Verb:
            pts.erase(pts.begin() + 3, pts.end());
            break;
        case SkPath::Verb::kCubic_Verb:
            break;
    }
    return py::make_tuple(result, pts);
}


void initPath(py::module &m) {
// PathTypes
py::enum_<SkPathFillType>(m, "PathFillType")
    .value("kWinding", SkPathFillType::kWinding,
        "Specifies that \"inside\" is computed by a non-zero sum of signed "
        "edge crossings.")
    .value("kEvenOdd", SkPathFillType::kEvenOdd,
        "Specifies that \"inside\" is computed by an odd number of edge "
        "crossings.")
    .value("kInverseWinding", SkPathFillType::kInverseWinding,
        "Same as Winding, but draws outside of the path, rather than inside.")
    .value("kInverseEvenOdd", SkPathFillType::kInverseEvenOdd,
        "Same as EvenOdd, but draws outside of the path, rather than inside.")
    .export_values();

py::enum_<SkPathConvexityType>(m, "PathConvexityType")
    .value("kUnknown", SkPathConvexityType::kUnknown)
    .value("kConvex", SkPathConvexityType::kConvex)
    .value("kConcave", SkPathConvexityType::kConcave)
    .export_values();

py::enum_<SkPathDirection>(m, "PathDirection")
    .value("kCW", SkPathDirection::kCW,
        "clockwise direction for adding closed contours")
    .value("kCCW", SkPathDirection::kCCW,
        "counter-clockwise direction for adding closed contours")
    .export_values();

py::enum_<SkPathSegmentMask>(m, "PathSegmentMask")
    .value("kLine_PathSegmentMask",
        SkPathSegmentMask::kLine_SkPathSegmentMask)
    .value("kQuad_PathSegmentMask",
        SkPathSegmentMask::kQuad_SkPathSegmentMask)
    .value("kConic_PathSegmentMask",
        SkPathSegmentMask::kConic_SkPathSegmentMask)
    .value("kCubic_PathSegmentMask",
        SkPathSegmentMask::kCubic_SkPathSegmentMask)
    .export_values();

py::enum_<SkPathVerb>(m, "PathVerb")
    .value("kMove", SkPathVerb::kMove, "iter.next returns 1 point")
    .value("kLine", SkPathVerb::kLine, "iter.next returns 2 points")
    .value("kQuad", SkPathVerb::kQuad, "iter.next returns 3 points")
    .value("kConic", SkPathVerb::kConic,
        "iter.next returns 3 points + iter.conicWeight()")
    .value("kCubic", SkPathVerb::kCubic, "iter.next returns 4 points")
    .value("kClose", SkPathVerb::kClose,
        "iter.next returns 1 point (contour's moveTo pt)")
    .value("kDone", SkPathVerb::kDone, "iter.next returns 0 points")
    .export_values();

m.def("PathFillType_IsEvenOdd", &SkPathFillType_IsEvenOdd);
m.def("PathFillType_IsInverse", &SkPathFillType_IsInverse);
m.def("PathFillType_ConvertToNonInverse", &SkPathFillType_ConvertToNonInverse);


py::enum_<SkPathOp>(m, "PathOp", R"docstring(
    The logical operations that can be performed when combining two paths.
    )docstring")
    .value("kDifference_PathOp",
        SkPathOp::kDifference_SkPathOp,
        "subtract the op path from the first path")
    .value("kIntersect_PathOp",
        SkPathOp::kIntersect_SkPathOp,
        "intersect the two paths")
    .value("kUnion_PathOp",
        SkPathOp::kUnion_SkPathOp,
        "union (inclusive-or) the two paths")
    .value("kXOR_PathOp",
        SkPathOp::kXOR_SkPathOp,
        "exclusive-or the two paths")
    .value("kReverseDifference_PathOp",
        SkPathOp::kReverseDifference_SkPathOp,
        "subtract the first path from the op path")
    .export_values();


// Path
py::class_<SkPath> path(m, "Path", R"docstring(
    :py:class:`Path` contain geometry.

    :py:class:`Path` may be empty, or contain one or more verbs that outline a
    figure. :py:class:`Path` always starts with a move verb to a Cartesian
    coordinate, and may be followed by additional verbs that add lines or
    curves. Adding a close verb makes the geometry into a continuous loop, a
    closed contour. :py:class:`Path` may contain any number of contours, each
    beginning with a move verb.

    :py:class:`Path` contours may contain only a move verb, or may also contain
    lines, quadratic beziers, conics, and cubic beziers. :py:class:`Path`
    contours may be open or closed.

    When used to draw a filled area, :py:class:`Path` describes whether the fill
    is inside or outside the geometry. :py:class:`Path` also describes the
    winding rule used to fill overlapping contours.

    Internally, :py:class:`Path` lazily computes metrics likes bounds and
    convexity. Call :py:meth:`Path.updateBoundsCache` to make SkPath thread
    safe.

    .. rubric:: Classes

    .. autosummary::
        :nosignatures:

        Iter
        RawIter
        ArcSize
        AddPathMode
        SegmentMask
        Verb
    )docstring");

py::enum_<SkPath::ArcSize>(path, "ArcSize")
    .value("kSmall_ArcSize", SkPath::ArcSize::kSmall_ArcSize,
        "smaller of arc pair")
    .value("kLarge_ArcSize", SkPath::ArcSize::kLarge_ArcSize,
        "larger of arc pair")
    .export_values();

py::enum_<SkPath::AddPathMode>(path, "AddPathMode")
    .value("kAppend_AddPathMode", SkPath::AddPathMode::kAppend_AddPathMode,
        "appended to destination unaltered")
    .value("kExtend_AddPathMode", SkPath::AddPathMode::kExtend_AddPathMode,
        "add line if prior contour is not closed")
    .export_values();

py::enum_<SkPath::SegmentMask>(path, "SegmentMask")
    .value("kLine_SegmentMask", SkPath::SegmentMask::kLine_SegmentMask)
    .value("kQuad_SegmentMask", SkPath::SegmentMask::kQuad_SegmentMask)
    .value("kConic_SegmentMask", SkPath::SegmentMask::kConic_SegmentMask)
    .value("kCubic_SegmentMask", SkPath::SegmentMask::kCubic_SegmentMask)
    .export_values();

py::enum_<SkPath::Verb>(path, "Verb")
    .value("kMove_Verb", SkPath::Verb::kMove_Verb)
    .value("kLine_Verb", SkPath::Verb::kLine_Verb)
    .value("kQuad_Verb", SkPath::Verb::kQuad_Verb)
    .value("kConic_Verb", SkPath::Verb::kConic_Verb)
    .value("kCubic_Verb", SkPath::Verb::kCubic_Verb)
    .value("kClose_Verb", SkPath::Verb::kClose_Verb)
    .value("kDone_Verb", SkPath::Verb::kDone_Verb)
    .export_values();

py::class_<SkPath::Iter>(path, "Iter", R"docstring(
    Iterates through verb array, and associated :py:class:`Point` array and
    conic weight.

    Provides options to treat open contours as closed, and to ignore degenerate
    data.
    )docstring")
    .def(py::init(),
        R"docstring(
        Initializes :py:class:`Iter` with an empty :py:class:`Path`.

        next() on :py:class:`Iter` returns :py:attr:`~skia.Path.kDone_Verb`.
        Call setPath to initialize :py:class:`Iter` at a later time.

        :return: :py:class:`Iter` of empty SkPath
        )docstring")
    .def(py::init<const SkPath&, bool>(),
        R"docstring(
        Sets :py:class:`Iter` to return elements of verb array,
        :py:class:`Point` array, and conic weight in path.

        If forceClose is true, :py:class:`Iter` will add
        :py:attr:`~skia.Path.kLine_Verb` and :py:attr:`~skia.Path.kClose_Verb`
        after each open contour. path is not altered.

        :path: :py:class:`Path` to iterate
        :forceClose: true if open contours generate
            :py:attr:`~skia.Path.kClose_Verb`

        :return: :py:class:`Iter` of path
        )docstring",
        py::arg("path"), py::arg("forceClose"))
    .def("setPath", &SkPath::Iter::setPath,
        R"docstring(
        Sets :py:class:`Iter` to return elements of verb array,
        :py:class:`Point` array, and conic weight in path.

        If forceClose is true, :py:class:`Iter` will add
        :py:attr:`~skia.Path.kLine_Verb` and :py:attr:`~skia.Path.kClose_Verb`
        after each open contour. path is not altered.

        :param skia.Path path: :py:class:`Path` to iterate
        :param bool forceClose: true if open contours generate
            :py:attr:`~skia.Path.kClose_Verb`
        )docstring",
        py::arg("path"), py::arg("forceClose"))
    .def("next", &Iter_next<SkPath::Iter>,
        R"docstring(
        Returns next :py:class:`~skia.Path.Verb` in verb array, and advances
        :py:class:`Iter`.

        When verb array is exhausted, returns :py:attr:`~skia.Path.kDone_Verb`.

        Zero to four :py:class:`Point` are stored in pts, depending on the
        returned :py:class:`~skia.Path.Verb`.

        :return: tuple of the following: next :py:class:`~skia.Path.Verb` from
            verb array; and storage for :py:class:`Point` data describing
            returned :py:class:`~skia.Path.Verb`
        :rtype: Tuple[skia.Path.Verb,List[skia.Point]]
        )docstring")
    .def("conicWeight", &SkPath::Iter::conicWeight,
        R"docstring(
        Returns conic weight if :py:meth:`next` returned
        :py:attr:`~skia.Path.kConic_Verb`.

        If :py:meth:`next` has not been called, or :py:meth:`next` did not
        return :py:attr:`~skia.Path.kConic_Verb`, result is undefined.

        :return: conic weight for conic :py:class:`Point` returned by
            :py:meth:`next`
        )docstring")
    .def("isCloseLine", &SkPath::Iter::isCloseLine,
        R"docstring(
        Returns true if last:py:attr:`~skia.Path.kLine_Verb` returned by
        :py:meth:`next` was generated by :py:attr:`~skia.Path.kClose_Verb`.

        When true, the end point returned by :py:meth:`next` is also the start
        point of contour.

        If :py:meth:`next` has not been called, or :py:meth:`next` did not
        return :py:attr:`~skia.Path.kLine_Verb`, result is undefined.

        :return: true if last :py:attr:`~skia.Path.kLine_Verb` was generated by
            :py:attr:`~skia.Path.kClose_Verb`
        )docstring")
    .def("isClosedContour", &SkPath::Iter::isClosedContour,
        R"docstring(
        Returns true if subsequent calls to :py:meth:`next` return
        :py:attr:`~skia.Path.kClose_Verb` before returning
        :py:attr:`~skia.Path.kMove_Verb`.

        if true, contour :py:class:`Iter` is processing may end with
        :py:attr:`~skia.Path.kClose_Verb`, or :py:class:`Iter` may have been
        initialized with force close set to true.

        :return: true if contour is closed
        )docstring")
    ;

py::class_<SkPath::RawIter>(path, "RawIter", R"docstring(
    Iterates through verb array, and associated :py:class:`Point` array and
    conic weight.

    verb array, :py:class:`Point` array, and conic weight are returned
    unaltered.
    )docstring")
    .def(py::init(),
        R"docstring(
        Initializes :py:class:`RawIter` with an empty :py:class:`Path`.

        :py:meth:`next` on :py:class:`RawIter` returns
        :py:attr:`~skia.Path.kDone_Verb`. Call :py:meth:`setPath` to initialize
        :py:class:`RawIter` at a later time.

        :return: :py:class:`RawIter` of empty :py:class:`Path`
        )docstring")
    .def(py::init<const SkPath&>(),
        R"docstring(
        Sets :py:class:`RawIter` to return elements of verb array,
        :py:class:`Point` array, and conic weight in path.

        :path: :py:class:`Path` to iterate
        :return: :py:class:`RawIter` of path
        )docstring",
        py::arg("path"))
    .def("setPath", &SkPath::RawIter::setPath,
        R"docstring(
        Sets :py:class:`RawIter` to return elements of verb array,
        :py:class:`Point` array, and conic weight in path.

        :param skia.Path path: :py:class:`Path` to iterate
        )docstring",
        py::arg("path"))
    .def("next", &Iter_next<SkPath::RawIter>,
        R"docstring(
        Returns next :py:class:`~skia.Path.Verb` in verb array, and advances
        :py:class:`RawIter`.

        When verb array is exhausted, returns :py:attr:`~skia.Path.kDone_Verb`.

        Zero to four :py:class:`Point` are stored in pts, depending on the
        returned :py:class:`~skia.Path.Verb`.

        :return: tuple of the following: next :py:class:`~skia.Path.Verb` from
            verb array; and storage for :py:class:`Point` data describing
            returned :py:class:`~skia.Path.Verb`
        :rtype: Tuple[skia.Path.Verb,List[skia.Point]]
        )docstring")
    .def("peek", &SkPath::RawIter::peek,
        R"docstring(
        Returns next :py:class:`~skia.Path.Verb`, but does not advance
        :py:class:`RawIter`.

        :return: next :py:class:`~skia.Path.Verb` from verb array
        )docstring")
    .def("conicWeight", &SkPath::RawIter::conicWeight,
        R"docstring(
        Returns conic weight if :py:meth:`next` returned
        :py:attr:`~skia.Path.kConic_Verb`.

        If :py:meth:`next` has not been called, or :py:meth:`next` did not
        return :py:attr:`~skia.Path.kConic_Verb`, result is undefined.

        :return: conic weight for conic :py:class:`Point` returned by
            :py:meth:`next`
        )docstring")
    ;

path
    .def(py::init<>(), R"docstring(
        Constructs an empty :py:class:`Path`.

        By default, :py:class:`Path` has no verbs, no :py:class:`Point`, and no
        weights. :py:class:`FillType` is set to kWinding.
        )docstring")
    .def(py::init<const SkPath&>(), R"docstring(
        Constructs a copy of an existing path.

        Copy constructor makes two paths identical by value. Internally, path
        and the returned result share pointer values. The underlying verb array,
        :py:class:`Point` array and weights are copied when modified.

        Creating a :py:class:`Path` copy is very efficient and never allocates
        memory. :py:class:`Path` are always copied by value from the interface;
        the underlying shared pointers are not exposed.

        :path: :py:class:`Path` to copy by value
        )docstring",
        py::arg("path"))
    .def("isInterpolatable", &SkPath::isInterpolatable,
        R"docstring(
        Returns true if :py:class:`Path` contain equal verbs and equal weights.

        If :py:class:`Path` contain one or more conics, the weights must match.

        :py:meth:`conicTo` may add different verbs depending on conic weight, so
        it is not trivial to interpolate a pair of :py:class:`Path` containing
        conics with different conic weight values.

        :param skia.Path compare: :py:class:`Path` to compare
        :return: true if :py:class:`Path` verb array and weights are equivalent
        )docstring",
        py::arg("compare"))
    .def("interpolate", &SkPath::interpolate,
        R"docstring(
        Interpolates between :py:class:`Path` with :py:class:`Point` array of
        equal size.

        Copy verb array and weights to out, and set out :py:class:`Point` array
        to a weighted average of this :py:class:`Point` array and ending
        :py:class:`Point` array, using the formula:
        `(Path Point * weight) + ending Point * (1 - weight)`.

        weight is most useful when between zero (ending :py:class:`Point` array)
        and one (this Point_Array); will work with values outside of this range.

        :py:meth:`interpolate` returns false and leaves out unchanged if
        :py:class:`Point` array is not the same size as ending
        :py:class:`Point` array. Call :py:meth:`isInterpolatable` to check
        :py:class:`Path` compatibility prior to calling :py:meth:`interpolate`.

        :param skia.Path ending: :py:class:`Point` array averaged with this
            :py:class:`Point` array
        :param float weight: contribution of this :py:class:`Point` array, and
            one minus contribution of ending :py:class:`Point` array
        :param skia.Path out: :py:class:`Path` replaced by interpolated averages

        :return: true if :py:class:`Path` contain same number of
            :py:class:`Point`
        )docstring")
    .def("getFillType", &SkPath::getFillType,
        R"docstring(
        Returns :py:class:`PathFillType`, the rule used to fill
        :py:class:`Path`.

        :return: current :py:class:`PathFillType` setting
        )docstring")
    .def("setFillType", &SkPath::setFillType,
        R"docstring(
        Sets FillType, the rule used to fill :py:class:`Path`.

        While there is no check that ft is legal, values outside of FillType are
        not supported.
        )docstring",
        py::arg("ft"))
    .def("isInverseFillType", &SkPath::isInverseFillType,
        R"docstring(
        Returns if FillType describes area outside :py:class:`Path` geometry.

        The inverse fill area extends indefinitely.

        :return: true if FillType is
            :py:attr:`~skia.PathFillType.kInverseWinding` or
            :py:attr:`~skia.PathFillType.kInverseEvenOdd`
        )docstring")
    .def("toggleInverseFillType", &SkPath::toggleInverseFillType,
        R"docstring(
        Replaces FillType with its inverse.

        The inverse of FillType describes the area unmodified by the original
        FillType.
        )docstring")
    .def("getConvexityType", &SkPath::getConvexityType,
        R"docstring(
        Returns the comvexity type, computing if needed.

        Never returns :py:attr:`~PathConvexityType.kUnknown`.

        :return: path's convexity type (convex or concave)
        )docstring")
    .def("getConvexityTypeOrUnknown", &SkPath::getConvexityTypeOrUnknown,
        R"docstring(
        If the path's convexity is already known, return it, else return
        :py:attr:`~PathConvexityType.kUnknown`.

        If you always want to know the convexity, even if that means having to
        compute it, call :py:meth:`getConvexityType`.

        :return: known convexity, or :py:attr:`~PathConvexityType.kUnknown`
        )docstring")
    .def("setConvexityType",
        py::overload_cast<SkPathConvexityType>(&SkPath::setConvexityType),
        R"docstring(
        Stores a convexity type for this path.

        This is what will be returned if :py:meth:`getConvexityTypeOrUnknown` is
        called. If you pass :py:attr:`~PathConvexityType.kUnknown`, then if
        :py:meth:`getContexityType` is called, the real convexity will be
        computed.
        )docstring",
        py::arg("convexity"))
    .def("isConvex", &SkPath::isConvex,
        R"docstring(
        Returns true if the path is convex.

        If necessary, it will first compute the convexity.
        )docstring")
    .def("isOval", &SkPath::isOval,
        R"docstring(
        Returns true if this path is recognized as an oval or circle.

        bounds receives bounds of oval.

        bounds is unmodified if oval is not found.

        :param skia.Rect bounds: storage for bounding :py:class:`Rect` of oval;
            may be nullptr
        :return: true if :py:class:`Path` is recognized as an oval or circle
        )docstring",
        py::arg("oval") = nullptr)
    .def("isRRect", &SkPath::isRRect,
        R"docstring(
        Returns true if path is representable as :py:class:`RRect`.

        Returns false if path is representable as oval, circle, or
        :py:class:`Rect`.

        rrect receives bounds of :py:class:`RRect`.

        rrect is unmodified if :py:class:`RRect` is not found.

        :param skia.RRect rrect: storage for bounding :py:class:`Rect` of
            :py:class:`RRect`; may be nullptr
        :return: true if :py:class:`Path` contains only :py:class:`RRect`
        )docstring",
        py::arg("rrect") = nullptr)
    .def("reset", &SkPath::reset,
        R"docstring(
        Sets :py:class:`Path` to its initial state.

        Removes verb array, :py:class:`Point` array, and weights, and sets
        FillType to kWinding. Internal storage associated with :py:class:`Path`
        is released.

        :return: reference to :py:class:`Path`
        )docstring")
    .def("rewind", &SkPath::rewind,
        R"docstring(
        Sets :py:class:`Path` to its initial state, preserving internal storage.

        Removes verb array, :py:class:`Point` array, and weights, and sets
        FillType to kWinding. Internal storage associated with :py:class:`Path`
        is retained.

        Use :py:meth:`rewind` instead of :py:meth:`reset` if :py:class:`Path`
        storage will be reused and performance is critical.

        :return: reference to :py:class:`Path`
        )docstring")
    .def("isEmpty", &SkPath::isEmpty,
        R"docstring(
        Returns if :py:class:`Path` is empty.

        Empty :py:class:`Path` may have FillType but has no :py:class:`Point`,
        :py:class:`Path.Verb`, or conic weight. :py:class:`Path`() constructs
        empty :py:class:`Path`; :py:meth:`reset` and :py:meth:`rewind` make
        :py:class:`Path` empty.

        :return: true if the path contains no :py:class:`Path.Verb` array
        )docstring")
    .def("isLastContourClosed", &SkPath::isLastContourClosed,
        R"docstring(
        Returns if contour is closed.

        Contour is closed if :py:class:`Path` :py:class:`~skia.Path.Verb` array
        was last modified by :py:meth:`close`. When stroked, closed contour
        draws :py:class:`~skia.Paint.Join` instead of
        :py:class:`~skia.Paint.Cap` at first and last :py:class:`Point`.

        :return: true if the last contour ends with a
            :py:attr:`~skia.Path.kClose_Verb`
        )docstring")
    .def("isFinite", &SkPath::isFinite,
        R"docstring(
        Returns true for finite :py:class:`Point` array values between negative
        SK_ScalarMax and positive SK_ScalarMax.

        Returns false for any :py:class:`Point` array value of
        SK_ScalarInfinity, SK_ScalarNegativeInfinity, or SK_ScalarNaN.

        :return: true if all :py:class:`Point` values are finite
        )docstring")
    .def("isVolatile", &SkPath::isVolatile,
        R"docstring(
        Returns true if the path is volatile; it will not be altered or
        discarded by the caller after it is drawn.

        :py:class:`Path` by default have volatile set false, allowing
        :py:class:`Surface` to attach a cache of data which speeds repeated
        drawing. If true, :py:class:`Surface` may not speed repeated drawing.

        :return: true if caller will alter :py:class:`Path` after drawing
        )docstring")
    .def("setIsVolatile", &SkPath::setIsVolatile,
        R"docstring(
        Specifies whether :py:class:`Path` is volatile; whether it will be
        altered or discarded by the caller after it is drawn.

        :py:class:`Path` by default have volatile set false, allowing
        :py:class:`BaseDevice` to attach a cache of data which speeds repeated
        drawing.

        Mark temporary paths, discarded or modified after use, as volatile to
        inform :py:class:`BaseDevice` that the path need not be cached.

        Mark animating :py:class:`Path` volatile to improve performance. Mark
        unchanging :py:class:`Path` non-volatile to improve repeated rendering.

        raster surface :py:class:`Path` draws are affected by volatile for some
        shadows. GPU surface :py:class:`Path` draws are affected by volatile for
        some shadows and concave geometries.

        :param bool isVolatile: true if caller will alter :py:class:`Path` after
            drawing
        )docstring",
        py::arg("isVolatile"))
    .def("isLine",
        // &SkPath::isLine,
        [] (const SkPath& path, SkPoint *p0, SkPoint *p1) {
            SkPoint line[2];
            auto result = path.isLine(line);
            if (result) {
                if (p0)
                    p0->set(line[0].x(), line[0].y());
                if (p1)
                    p1->set(line[1].x(), line[1].y());
            }
            return result;
        },
        R"docstring(
        Returns true if :py:class:`Path` contains only one line;
        :py:class:`~skia.Path.Verb` array has two entries:
        :py:attr:`~skia.Path.kMove_Verb`, :py:attr:`~skia.Path.kLine_Verb`.

        If :py:class:`Path` contains one line and line is not nullptr, line is
        set to line start point and line end point. Returns false if
        :py:class:`Path` is not one line; line is unaltered.

        :param skia.Point p0: storage for line start. May be nullptr
        :param skia.Point p1: storage for line end. May be nullptr
        :return: true if :py:class:`Path` contains exactly one line
        )docstring",
        py::arg("p0") = nullptr, py::arg("p1") =  nullptr)
    .def("countPoints", &SkPath::countPoints,
        R"docstring(
        Returns the number of points in :py:class:`Path`.

        :py:class:`Point` count is initially zero.

        :return: :py:class:`Path` :py:class:`Point` array length
        )docstring")
    .def("getPoint", &SkPath::getPoint,
        R"docstring(
        Returns :py:class:`Point` at index in :py:class:`Point` array.

        Valid range for index is 0 to countPoints() - 1. Returns (0, 0) if index
        is out of range.

        :param skia.Point index: :py:class:`Point` array element selector
        :return: :py:class:`Point` array value or (0, 0)
        )docstring",
        py::arg("index"))
    .def("getPoints",
        [] (const SkPath& path, int max) {
            std::vector<SkPoint> points((max == 0) ? path.countPoints() : max);
            auto length = path.getPoints(&points[0], max);
            if (length < max)
                points.erase(points.begin() + length, points.end());
            return points;
        },
        R"docstring(
        Returns number of points in :py:class:`Path`.

        Up to max points are copied. If max is greater than number of points,
        excess points storage is removed. If max is zero, calls
        :py:meth:`countPoints` to get max.

        :param int max: maximum to copy; must be greater than or equal to zero
        :return: List of :py:class:`skia.Point`
        :rtype: List[skia.Point]
        )docstring",
        py::arg("max") = 0)
    .def("countVerbs", &SkPath::countVerbs,
        R"docstring(
        Returns the number of verbs: :py:attr:`~Path.kMove_Verb`,
        :py:attr:`~Path.kLine_Verb`, :py:attr:`~Path.kQuad_Verb`,
        :py:attr:`~Path.kConic_Verb`, :py:attr:`~Path.kCubic_Verb`, and
        :py:attr:`~Path.kClose_Verb`; added to :py:class:`Path`.

        :return: length of verb array
        )docstring")
    .def("getVerbs",
        [] (const SkPath& path, int max) {
            std::vector<uint8_t> verbs((max == 0) ? path.countVerbs() : max);
            auto length = path.getVerbs(&verbs[0], max);
            if (length < max)
                verbs.erase(verbs.begin() + length, verbs.end());
            return verbs;
        },
        R"docstring(
        Returns verbs in the path.

        Up to max verbs are copied. The verbs are copied as one byte per verb.

        :param int max: maximum number to copy into verbs
        :return: List of :py:class:`skia.Path.Verb`
        :rtype: List[skia.Path.Verb]
        )docstring",
        py::arg("max") = 0)
    .def("approximateBytesUsed", &SkPath::approximateBytesUsed,
        R"docstring(
        Returns the approximate byte size of the SkPath in memory.

        :return: approximate size
        )docstring")
    .def("swap", &SkPath::swap,
        R"docstring(
        Exchanges the verb array, :py:class:`Point` array, weights, and
        :py:class:`PathFillType` with other.

        Cached state is also exchanged. :py:meth:`swap` internally exchanges
        pointers, so it is lightweight and does not allocate memory.

        :param skia.Path other: :py:class:`Path` exchanged by value
        )docstring",
        py::arg("other"))
    .def("getBounds", &SkPath::getBounds,
        R"docstring(
        Returns minimum and maximum axes values of :py:class:`Point` array.

        Returns (0, 0, 0, 0) if :py:class:`Path` contains no points. Returned
        bounds width and height may be larger or smaller than area affected when
        :py:class:`Path` is drawn.

        :py:class:`Rect` returned includes all :py:class:`Point` added to
        :py:class:`Path`, including :py:class:`Point` associated with
        :py:attr:`~Path.kMove_Verb` that define empty contours.

        :return: bounds of all :py:class:`Point` in :py:class:`Point` array
        )docstring")
    .def("updateBoundsCache", &SkPath::updateBoundsCache,
        R"docstring(
        Updates internal bounds so that subsequent calls to :py:meth:`getBounds`
        are instantaneous.

        Unaltered copies of :py:class:`Path` may also access cached bounds
        through :py:meth:`getBounds`.

        For now, identical to calling :py:meth:`getBounds` and ignoring the
        returned value.

        Call to prepare :py:class:`Path` subsequently drawn from multiple
        threads, to avoid a race condition where each draw separately computes
        the bounds.
        )docstring")
    .def("computeTightBounds", &SkPath::computeTightBounds,
        R"docstring(
        Returns minimum and maximum axes values of the lines and curves in
        :py:class:`Path`.

        Returns (0, 0, 0, 0) if :py:class:`Path` contains no points. Returned
        bounds width and height may be larger or smaller than area affected when
        :py:class:`Path` is drawn.

        Includes :py:class:`Point` associated with kMove_Verb that define empty
        contours.

        Behaves identically to :py:meth:`getBounds` when :py:class:`Path`
        contains only lines. If :py:class:`Path` contains curves, computed
        bounds includes the maximum extent of the quad, conic, or cubic; is
        slower than :py:meth:`getBounds`; and unlike :py:meth:`getBounds`, does
        not cache the result.

        :return: tight bounds of curves in :py:class:`Path`
        )docstring")
    .def("conservativelyContainsRect", &SkPath::conservativelyContainsRect,
        R"docstring(
        Returns true if rect is contained by :py:class:`Path`.

        May return false when rect is contained by :py:class:`Path`.

        For now, only returns true if :py:class:`Path` has one contour and is
        convex. rect may share points and edges with :py:class:`Path` and be
        contained. Returns true if rect is empty, that is, it has zero width or
        height; and the :py:class:`Point` or line described by rect is contained
        by :py:class:`Path`.

        :param skia.Rect rect: :py:class:`Rect`, line, or :py:class:`Point`
            checked for containment
        :return: true if rect is contained
        )docstring",
        py::arg("rect"))
    .def("incReserve", &SkPath::incReserve,
        R"docstring(
        Grows :py:class:`Path` verb array and :py:class:`Point` array to contain
        extraPtCount additional :py:class:`Point`.

        May improve performance and use less memory by reducing the number and
        size of allocations when creating :py:class:`Path`.

        :param int extraPtCount: number of additional :py:class:`Point` to
            allocate
        )docstring",
        py::arg("extraPtCount"))
    .def("shrinkToFit", &SkPath::shrinkToFit,
        R"docstring(
        Shrinks :py:class:`Path` verb array and :py:class:`Point` array storage
        to discard unused capacity.

        May reduce the heap overhead for :py:class:`Path` known to be fully
        constructed.
        )docstring")
    .def("moveTo",
        py::overload_cast<SkScalar, SkScalar>(&SkPath::moveTo),
        R"docstring(
        Adds beginning of contour at :py:class:`Point` (x, y).

        :x: x-axis value of contour start
        :y: y-axis value of contour start
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("x"), py::arg("y"))
    .def("moveTo",
        py::overload_cast<const SkPoint&>(&SkPath::moveTo),
        R"docstring(
        Adds beginning of contour at :py:class:`Point` p.

        :p: contour start
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("p"))
    .def("rMoveTo", &SkPath::rMoveTo,
        R"docstring(
        Adds beginning of contour relative to last point.

        If :py:class:`Path` is empty, starts contour at (dx, dy). Otherwise,
        start contour at last point offset by (dx, dy). Function name stands for
        "relative move to".

        :param int dx: offset from last point to contour start on x-axis
        :param int dy: offset from last point to contour start on y-axis
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("dx"), py::arg("dy"))
    .def("lineTo",
        py::overload_cast<SkScalar, SkScalar>(&SkPath::lineTo),
        R"docstring(
        Adds line from last point to (x, y).

        If :py:class:`Path` is empty, or last :py:class:`~Path.Verb` is
        :py:attr:`~Path.kClose_Verb`, last point is set to (0, 0) before adding
        line.

        lineTo() appends :py:attr:`~Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed. lineTo() then appends
        :py:attr:`~Path.kLine_Verb` to verb array and (x, y) to
        :py:class:`Point` array.

        :x: end of added line on x-axis
        :y: end of added line on y-axis
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("x"), py::arg("y"))
    .def("lineTo",
        py::overload_cast<const SkPoint&>(&SkPath::lineTo),
        R"docstring(
        Adds line from last point to :py:class:`Point` p.

        If :py:class:`Path` is empty, or last :py:class:`~Path.Verb` is
        :py:attr:`~Path.kClose_Verb`, last point is set to (0, 0) before adding
        line.

        lineTo() appends :py:attr:`~Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed. lineTo() then appends
        :py:attr:`~Path.kLine_Verb` to verb array and p to :py:class:`Point`
        array.

        :p: end :py:class:`Point` of added line
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("p"))
    .def("rLineTo", &SkPath::rLineTo,
        R"docstring(
        Adds line from last point to vector (dx, dy).

        If :py:class:`Path` is empty, or last :py:class:`~Path.Verb` is
        :py:attr:`~skia.Path.kClose_Verb`, last point is set to (0, 0) before
        adding line.

        Appends :py:attr:`~skia.Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.kLine_Verb` to verb array and line end to
        :py:class:`Point` array. Line end is last point plus vector (dx, dy).
        Function name stands for "relative line to".

        :param float dx: offset from last point to line end on x-axis
        :param float dy: offset from last point to line end on y-axis
        :return: reference to :py:class:`Path`
        )docstring")
    .def("quadTo",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkPath::quadTo),
        R"docstring(
        Adds quad from last point towards (x1, y1), to (x2, y2).

        If :py:class:`Path` is empty, or last :py:class:`Path.Verb` is
        :py:attr:`~skia.Path.kClose_Verb`, last point is set to (0, 0) before
        adding quad.

        Appends :py:attr:`~skia.Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.kQuad_Verb` to verb array; and (x1, y1), (x2, y2)
        to :py:class:`Point` array.

        :x1: control :py:class:`Point` of quad on x-axis
        :y1: control :py:class:`Point` of quad on y-axis
        :x2: end :py:class:`Point` of quad on x-axis
        :y2: end :py:class:`Point` of quad on y-axis

        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"))
    .def("quadTo",
        py::overload_cast<const SkPoint&, const SkPoint&>(&SkPath::quadTo),
        R"docstring(
        Adds quad from last point towards :py:class:`Point` p1, to
        :py:class:`Point` p2.

        If :py:class:`Path` is empty, or last :py:class:`Path.Verb` is
        :py:attr:`~skia.Path.kClose_Verb`, last point is set to (0, 0) before
        adding quad.

        Appends :py:attr:`~skia.Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.kQuad_Verb` to verb array; and :py:class:`Point`
        p1, p2 to :py:class:`Point` array.

        :p1: control :py:class:`Point` of added quad
        :p2: end :py:class:`Point` of added quad

        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("p1"), py::arg("p2"))
    .def("rQuadTo", &SkPath::rQuadTo,
        R"docstring(
        Adds quad from last point towards vector (dx1, dy1), to vector (dx2,
        dy2).

        If :py:class:`Path` is empty, or last :py:class:`~Path.Verb` is
        :py:attr:`~skia.Path.kClose_Verb`, last point is set to (0, 0) before
        adding quad.

        Appends :py:attr:`~skia.Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.kQuad_Verb` to verb array;and appends quad control
        and quad end to :py:class:`Point` array. Quad control is last point plus
        vector (dx1, dy1). Quad end is last point plus vector (dx2, dy2).
        Function name stands for "relative quad to".

        :param float dx1: offset from last point to quad control on x-axis
        :param float dy1: offset from last point to quad control on y-axis
        :param float dx2: offset from last point to quad end on x-axis
        :param float dy2: offset from last point to quad end on y-axis
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("dx1"), py::arg("dy1"), py::arg("dx2"), py::arg("dy2"))
    .def("conicTo",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkPath::conicTo),
        R"docstring(
        Adds conic from last point towards (x1, y1), to (x2, y2), weighted by w.

        If :py:class:`Path` is empty, or last :py:class:`Path.Verb` is
        :py:attr:`~skia.Path.kClose_Verb`, last point is set to (0, 0) before
        adding conic.

        Appends :py:attr:`~skia.Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed.

        If w is finite and not one, appends :py:attr:`~skia.Path.kConic_Verb` to
        verb array; and (x1, y1), (x2, y2) to :py:class:`Point` array; and w to
        conic weights.

        If w is one, appends :py:attr:`~skia.Path.kQuad_Verb` to verb array, and
        (x1, y1), (x2, y2) to :py:class:`Point` array.

        If w is not finite, appends :py:attr:`~skia.Path.kLine_Verb` twice to
        verb array, and (x1, y1), (x2, y2) to :py:class:`Point` array.

        :x1: control :py:class:`Point` of conic on x-axis
        :y1: control :py:class:`Point` of conic on y-axis
        :x2: end :py:class:`Point` of conic on x-axis
        :y2: end :py:class:`Point` of conic on y-axis
        :w: weight of added conic

        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"),
        py::arg("w"))
    .def("conicTo",
        py::overload_cast<const SkPoint&, const SkPoint&, SkScalar>(
            &SkPath::conicTo),
        R"docstring(
        Adds conic from last point towards :py:class:`Point` p1, to
        :py:class:`Point` p2, weighted by w.

        If :py:class:`Path` is empty, or last :py:class:`Path.Verb` is
        :py:attr:`~skia.Path.kClose_Verb`, last point is set to (0, 0) before
        adding conic.

        Appends :py:attr:`~skia.Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed.

        If w is finite and not one, appends :py:attr:`~skia.Path.kConic_Verb` to
        verb array; and :py:class:`Point` p1, p2 to :py:class:`Point` array; and
        w to conic weights.

        If w is one, appends :py:attr:`~skia.Path.kQuad_Verb` to verb array, and
        :py:class:`Point` p1, p2 to :py:class:`Point` array.

        If w is not finite, appends :py:attr:`~skia.Path.kLine_Verb` twice to
        verb array, and :py:class:`Point` p1, p2 to :py:class:`Point` array.

        :p1: control :py:class:`Point` of added conic
        :p2: end :py:class:`Point` of added conic
        :w: weight of added conic
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("p1"), py::arg("p2"), py::arg("w"))
    .def("rConicTo", &SkPath::rConicTo,
        R"docstring(
        Adds conic from last point towards vector (dx1, dy1), to vector (dx2,
        dy2), weighted by w.

        If :py:class:`Path` is empty, or last :py:class:`Path`::Verb is
        :py:attr:`~skia.Path.kClose_Verb`, last point is set to (0, 0) before
        adding conic.

        Appends :py:attr:`~skia.Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed.

        If w is finite and not one, next appends
        :py:attr:`~skia.Path.kConic_Verb` to verb array, and w is recorded as
        conic weight; otherwise, if w is one, appends
        :py:attr:`~skia.Path.kQuad_Verb` to verb array; or if w is not finite,
        appends :py:attr:`~skia.Path.kLine_Verb` twice to verb array.

        In all cases appends :py:class:`Point` control and end to
        :py:class:`Point` array. control is last point plus vector (dx1, dy1).
        end is last point plus vector (dx2, dy2).

        Function name stands for "relative conic to".

        :param float dx1: offset from last point to conic control on x-axis
        :param float dy1: offset from last point to conic control on y-axis
        :param float dx2: offset from last point to conic end on x-axis
        :param float dy2: offset from last point to conic end on y-axis
        :param float w:   weight of added conic
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("dx1"), py::arg("dy1"), py::arg("dx2"), py::arg("dy2"),
        py::arg("w"))
    .def("cubicTo",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar, SkScalar,
            SkScalar>(&SkPath::cubicTo),
        R"docstring(
        Adds cubic from last point towards (x1, y1), then towards (x2, y2),
        ending at (x3, y3).

        If :py:class:`Path` is empty, or last :py:class:`Path.Verb` is
        :py:attr:`~skia.Path.kClose_Verb`, last point is set to (0, 0) before
        adding cubic.

        Appends :py:attr:`~skia.Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.kCubic_Verb` to verb array; and (x1, y1), (x2, y2),
        (x3, y3) to :py:class:`Point` array.

        :x1: first control :py:class:`Point` of cubic on x-axis
        :y1: first control :py:class:`Point` of cubic on y-axis
        :x2: second control :py:class:`Point` of cubic on x-axis
        :y2: second control :py:class:`Point` of cubic on y-axis
        :x3: end :py:class:`Point` of cubic on x-axis
        :y3: end :py:class:`Point` of cubic on y-axis
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"),
        py::arg("x3"), py::arg("y3"))
    .def("cubicTo",
        py::overload_cast<const SkPoint&, const SkPoint&, const SkPoint&>(
            &SkPath::cubicTo),
        R"docstring(
        Adds cubic from last point towards :py:class:`Point` p1, then towards
        :py:class:`Point` p2, ending at :py:class:`Point` p3.

        If :py:class:`Path` is empty, or last :py:class:`Path.Verb` is
        :py:attr:`~skia.Path.kClose_Verb`, last point is set to (0, 0) before
        adding cubic.

        Appends :py:attr:`~skia.Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.kCubic_Verb` to verb array; and :py:class:`Point`
        p1, p2, p3 to :py:class:`Point` array.

        :p1: first control :py:class:`Point` of cubic
        :p2: second control :py:class:`Point` of cubic
        :p3: end :py:class:`Point` of cubic
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("p1"), py::arg("p2"), py::arg("p3"))
    .def("rCubicTo", &SkPath::rCubicTo,
        R"docstring(
        Adds cubic from last point towards vector (dx1, dy1), then towards
        vector (dx2, dy2), to vector (dx3, dy3).

        If :py:class:`Path` is empty, or last :py:class:`Path.Verb` is
        :py:attr:`~skia.Path.kClose_Verb`, last point is set to (0, 0) before
        adding cubic.

        Appends :py:attr:`~skia.Path.kMove_Verb` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.kCubic_Verb` to verb array; and appends cubic
        control and cubic end to :py:class:`Point` array. Cubic control is last
        point plus vector (dx1, dy1). Cubic end is last point plus vector (dx2,
        dy2). Function name stands for "relative cubic to".

        :param float dx1: offset from last point to first cubic control on
            x-axis
        :param float dy1: offset from last point to first cubic control on
            y-axis
        :param float dx2: offset from last point to second cubic control on
            x-axis
        :param float dy2: offset from last point to second cubic control on
            y-axis
        :param float dx3: offset from last point to cubic end on x-axis
        :param float dy3: offset from last point to cubic end on y-axis
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("dx1"), py::arg("dy1"), py::arg("dx2"), py::arg("dy2"),
        py::arg("dx3"), py::arg("dy3"))
    .def("arcTo",
        py::overload_cast<const SkRect&, SkScalar, SkScalar, bool>(
            &SkPath::arcTo),
        R"docstring(
        Appends arc to :py:class:`Path`.

        Arc added is part of ellipse bounded by oval, from startAngle through
        sweepAngle. Both startAngle and sweepAngle are measured in degrees,
        where zero degrees is aligned with the positive x-axis, and positive
        sweeps extends arc clockwise.

        :py:meth:`arcTo` adds line connecting :py:class:`Path` last
        :py:class:`Point` to initial arc :py:class:`Point` if forceMoveTo is
        false and :py:class:`Path` is not empty. Otherwise, added contour begins
        with first point of arc. Angles greater than -360 and less than 360 are
        treated modulo 360.

        :oval: bounds of ellipse containing arc
        :startAngle: starting angle of arc in degrees
        :sweepAngle: sweep, in degrees. Positive is clockwise; treated modulo
            360
        :forceMoveTo: true to start a new contour with arc
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("oval"), py::arg("startAngle"), py::arg("sweepAngle"),
        py::arg("forceMoveTo"))
    .def("arcTo",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkPath::arcTo),
        R"docstring(
        Appends arc to :py:class:`Path`, after appending line if needed.

        Arc is implemented by conic weighted to describe part of circle. Arc is
        contained by tangent from last :py:class:`Path` point to (x1, y1), and
        tangent from (x1, y1) to (x2, y2). Arc is part of circle sized to
        radius, positioned so it touches both tangent lines.

        If last Path Point does not start Arc, :py:meth:`arcTo` appends
        connecting Line to Path. The length of Vector from (x1, y1) to (x2, y2)
        does not affect Arc.

        Arc sweep is always less than 180 degrees. If radius is zero, or if
        tangents are nearly parallel, :py:meth:`arcTo` appends Line from last
        Path Point to (x1, y1).

        :py:meth:`arcTo` appends at most one Line and one conic.
        :py:meth:`arcTo` implements the functionality of PostScript arct and
        HTML Canvas arcTo.

        :x1: x-axis value common to pair of tangents
        :y1: y-axis value common to pair of tangents
        :x2: x-axis value end of second tangent
        :y2: y-axis value end of second tangent
        :radius: distance from arc to circle center
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"),
        py::arg("radius"))
    .def("arcTo",
        py::overload_cast<const SkPoint, const SkPoint, SkScalar>(
            &SkPath::arcTo),
        R"docstring(
        Appends arc to :py:class:`Path`, after appending line if needed.

        Arc is implemented by conic weighted to describe part of circle. Arc is
        contained by tangent from last :py:class:`Path` point to p1, and tangent
        from p1 to p2. Arc is part of circle sized to radius, positioned so it
        touches both tangent lines.

        If last :py:class:`Path` :py:class:`Point` does not start arc,
        :py:meth:`arcTo` appends connecting line to :py:class:`Path`. The length
        of vector from p1 to p2 does not affect arc.

        Arc sweep is always less than 180 degrees. If radius is zero, or if
        tangents are nearly parallel, :py:meth:`arcTo` appends line from last
        :py:class:`Path` :py:class:`Point` to p1.

        :py:meth:`arcTo` appends at most one line and one conic.
        :py:meth:`arcTo` implements the functionality of PostScript arct and
        HTML Canvas arcTo.

        :p1: :py:class:`Point` common to pair of tangents
        :p2: end of second tangent
        :radius: distance from arc to circle center
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("p1"), py::arg("p2"), py::arg("radius"))
    .def("arcTo",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkPath::ArcSize,
            SkPathDirection, SkScalar, SkScalar>(&SkPath::arcTo),
        R"docstring(
        Appends arc to :py:class:`Path`.

        Arc is implemented by one or more conics weighted to describe part of
        oval with radii (rx, ry) rotated by xAxisRotate degrees. Arc curves from
        last :py:class:`Path` :py:class:`Point` to (x, y), choosing one of four
        possible routes: clockwise or counterclockwise, and smaller or larger.

        Arc sweep is always less than 360 degrees. :py:meth:`arcTo` appends line
        to (x, y) if either radii are zero, or if last :py:class:`Path`
        :py:class:`Point` equals (x, y). :py:meth:`arcTo` scales radii (rx, ry)
        to fit last :py:class:`Path` :py:class:`Point` and (x, y) if both are
        greater than zero but too small.

        :py:meth:`arcTo` appends up to four conic curves. :py:meth:`arcTo`
        implements the functionality of SVG arc, although SVG sweep-flag value
        is opposite the integer value of sweep; SVG sweep-flag uses 1 for
        clockwise, while :py:attr:`~skia.PathDirection.kCW` cast to int is zero.

        :rx: radius on x-axis before x-axis rotation
        :ry: radius on y-axis before x-axis rotation
        :xAxisRotate: x-axis rotation in degrees; positive values are clockwise
        :largeArc: chooses smaller or larger arc
        :sweep: chooses clockwise or counterclockwise arc
        :x: end of arc
        :y: end of arc
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("rx"), py::arg("ry"), py::arg("xAxisRotate"),
        py::arg("largeArc"), py::arg("sweep"), py::arg("x"), py::arg("y"))
    .def("arcTo",
        py::overload_cast<const SkPoint, SkScalar, SkPath::ArcSize,
            SkPathDirection, const SkPoint>(&SkPath::arcTo),
        R"docstring(
        Appends arc to :py:class:`Path`.

        Arc is implemented by one or more conic weighted to describe part of
        oval with radii (r.fX, r.fY) rotated by xAxisRotate degrees. Arc curves
        from last :py:class:`Path` :py:class:`Point` to (xy.fX, xy.fY), choosing
        one of four possible routes: clockwise or counterclockwise, and smaller
        or larger.

        Arc sweep is always less than 360 degrees. :py:meth:`arcTo` appends line
        to xy if either radii are zero, or if last :py:class:`Path`
        :py:class:`Point` equals (xy.fX, xy.fY). :py:meth:`arcTo` scales radii r
        to fit last :py:class:`Path` :py:class:`Point` and xy if both are
        greater than zero but too small to describe an arc.

        :py:meth:`arcTo` appends up to four conic curves. :py:meth:`arcTo`
        implements the functionality of SVG arc, although SVG sweep-flag value
        is opposite the integer value of sweep; SVG sweep-flag uses 1 for
        clockwise, while :py:attr:`~skia.PathDirection.kCW` cast to int is zero.

        :r: radii on axes before x-axis rotation
        :xAxisRotate: x-axis rotation in degrees; positive values are clockwise
        :largeArc: chooses smaller or larger arc
        :sweep: chooses clockwise or counterclockwise arc
        :xy: end of arc
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("r"), py::arg("xAxisRotate"), py::arg("largeArc"),
        py::arg("sweep"), py::arg("xy"))
    .def("rArcTo", &SkPath::rArcTo,
        R"docstring(
        Appends arc to :py:class:`Path`, relative to last :py:class:`Path`
        :py:class:`Point`.

        Arc is implemented by one or more conic, weighted to describe part of
        oval with radii (rx, ry) rotated by xAxisRotate degrees. Arc curves from
        last :py:class:`Path` :py:class:`Point` to relative end
        :py:class:`Point`: (dx, dy), choosing one of four possible routes:
        clockwise or counterclockwise, and smaller or larger. If
        :py:class:`Path` is empty, the start arc :py:class:`Point` is (0, 0).

        Arc sweep is always less than 360 degrees. :py:meth:`arcTo` appends line
        to end :py:class:`Point` if either radii are zero, or if last
        :py:class:`Path` :py:class:`Point` equals end :py:class:`Point`.
        :py:meth:`arcTo` scales radii (rx, ry) to fit last :py:class:`Path`
        :py:class:`Point` and end :py:class:`Point` if both are greater than
        zero but too small to describe an arc.

        :py:meth:`arcTo` appends up to four conic curves. :py:meth:`arcTo`
        implements the functionality of svg arc, although SVG "sweep-flag" value
        is opposite the integer value of sweep; SVG "sweep-flag" uses 1 for
        clockwise, while :py:attr:`~skia.PathDirection.kCW` cast to int is zero.

        :param float rx:  radius before x-axis rotation
        :param float ry:  radius before x-axis rotation
        :param float xAxisRotate: x-axis rotation in degrees; positive values
            are clockwise
        :param skia.ArcSize largeArc: chooses smaller or larger arc
        :param skia.PathDirection sweep: chooses clockwise or counterclockwise
            arc
        :param float dx: x-axis offset end of arc from last :py:class:`Path`
            :py:class:`Point`
        :param float dy: y-axis offset end of arc from last :py:class:`Path`
            :py:class:`Point`
        :return: reference to :py:class:`Path`
        )docstring")
    .def("close", &SkPath::close,
        R"docstring(
        Appends :py:attr:`Path.Verb.kClose` to :py:class:`Path`.

        A closed contour connects the first and last :py:class:`Point` with
        line, forming a continuous loop. Open and closed contour draw the same
        with :py:attr:`Paint.kFill_Style`. With :py:attr:`Paint.kStroke_Style`,
        open contour draws :py:class:`Paint.Cap` at contour start and end;
        closed contour draws :py:class:`Paint.Join` at contour start and end.

        :py:meth:`close` has no effect if :py:class:`Path` is empty or last
        :py:class:`Path` :py:class:`~Path.Verb` is :py:attr:`Path.Verb.kClose`.

        :return: reference to :py:class:`Path`
        )docstring")
    .def("isRect", &SkPath::isRect,
        R"docstring(
        Returns true if :py:class:`Path` is equivalent to :py:class:`Rect` when
        filled.

        If false: rect, isClosed, and direction are unchanged. If true: rect,
        isClosed, and direction are written to if not nullptr.

        rect may be smaller than the :py:class:`Path` bounds. :py:class:`Path`
        bounds may include :py:attr:`~skia.Path.kMove_Verb` points that do not
        alter the area drawn by the returned rect.

        :param rect: storage for bounds of :py:class:`Rect`; may be nullptr
        :param isClosed: storage set to true if :py:class:`Path` is closed; may
            be nullptr
        :param direction: storage set to :py:class:`Rect` direction; may be
            nullptr
        :return: true if :py:class:`Path` contains :py:class:`Rect`
        )docstring",
        py::arg("rect") = nullptr, py::arg("isClosed") = nullptr,
        py::arg("direction") = nullptr)
    .def("addRect",
        py::overload_cast<const SkRect&, SkPathDirection>(&SkPath::addRect),
        R"docstring(
        Adds :py:class:`Rect` to :py:class:`Path`, appending
        :py:attr:`~skia.Path.kMove_Verb`, three
        :py:attr:`~skia.Path.kLine_Verb`, and :py:attr:`~skia.Path.kClose_Verb`,
        starting with top-left corner of :py:class:`Rect`; followed by
        top-right, bottom-right, and bottom-left if dir is
        :py:attr:`~skia.Direction.kCW`; or followed by bottom-left,
        bottom-right, and top-right if dir is :py:attr:`~skia._Direction.kCCW`

        :rect: :py:class:`Rect` to add as a closed contour
        :dir: :py:class:`Path`::Direction to wind added contour
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("rect"), py::arg("dir") = SkPathDirection::kCW)
    .def("addRect",
        py::overload_cast<const SkRect&, SkPathDirection, unsigned>(
            &SkPath::addRect),
        R"docstring(
        Adds :py:class:`Rect` to :py:class:`Path`, appending
        :py:attr:`~skia.Path.kMove_Verb`, three
        :py:attr:`~skia.Path.kLine_Verb`, and k:py:attr:`~skia.Path.Verb.Close`.

        If dir is :py:attr:`~skia.PathDirection.kCW`, :py:class:`Rect` corners
        are added clockwise; if dir is :py:attr:`~skia.PathDirection.kCCW`,
        :py:class:`Rect` corners are added counterclockwise. start determines
        the first corner added.

        :rect: :py:class:`Rect` to add as a closed contour
        :dir: :py:class:`Path`::Direction to wind added contour
        :start: initial corner of :py:class:`Rect` to add

        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("rect"), py::arg("dir"), py::arg("start"))
    .def("addRect",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar,
        SkPathDirection>(&SkPath::addRect),
        R"docstring(
        Adds :py:class:`Rect` (left, top, right, bottom) to :py:class:`Path`,
        appending :py:attr:`~skia.Path.kMove_Verb`, three
        :py:attr:`~skia.Path.kLine_Verb`, and :py:attr:`~skia.Path.kClose_Verb`,
        starting with top-left corner of :py:class:`Rect`; followed by
        top-right, bottom-right, and bottom-left if dir is
        :py:attr:`~skia.PathDirection.kCW`; or followed by bottom-left,
        bottom-right, and top-right if dir is
        :py:attr:`~skia.PathDirection.kCCW`.

        :left: smaller x-axis value of :py:class:`Rect`
        :top: smaller y-axis value of :py:class:`Rect`
        :right: larger x-axis value of :py:class:`Rect`
        :bottom: larger y-axis value of :py:class:`Rect`
        :dir: :py:class:`Path`::Direction to wind added contour

        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("left"), py::arg("top"), py::arg("right"), py::arg("bottom"),
        py::arg("dir") = SkPathDirection::kCW)
    .def("addOval",
        py::overload_cast<const SkRect&, SkPathDirection>(&SkPath::addOval),
        R"docstring(
        Adds oval to path, appending :py:attr:`~skia.Path.kMove_Verb`, four
        :py:attr:`~skia.Path.kConic_Verb`, and
        :py:attr:`~skia.Path.kClose_Verb`.

        Oval is upright ellipse bounded by :py:class:`Rect` oval with radii
        equal to half oval width and half oval height. Oval begins at
        (oval.fRight, oval.centerY()) and continues clockwise if dir is
        :py:attr:`~skia.PathDirection.kCW`, counterclockwise if dir is
        :py:attr:`~skia.PathDirection.kCCW`.

        :oval: bounds of ellipse added
        :dir: :py:class:`~skia.PathDirection` to wind ellipse
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("oval"), py::arg("dir") = SkPathDirection::kCW)
    .def("addOval",
        py::overload_cast<const SkRect&, SkPathDirection, unsigned>(
            &SkPath::addOval),
        R"docstring(
        Adds oval to path, appending :py:attr:`~skia.Path.kMove_Verb`, four
        :py:attr:`~skia.Path.kConic_Verb`, and
        :py:attr:`~skia.Path.kClose_Verb`.

        Oval is upright ellipse bounded by :py:class:`Rect` oval with radii
        equal to half oval width and half oval height. Oval begins at start and
        continues clockwise if dir is kCW_Direction, counterclockwise if dir is
        :py:attr:`~skia.PathDirection.kCW`, counterclockwise if dir is
        :py:attr:`~skia.PathDirection.kCCW`.

        :oval: bounds of ellipse added
        :dir: :py:class:`Path`::Direction to wind ellipse
        :start: index of initial point of ellipse
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("oval"), py::arg("dir"), py::arg("start"))
    .def("addCircle", &SkPath::addCircle,
        R"docstring(
        Adds circle centered at (x, y) of size radius to :py:class:`Path`,
        appending :py:attr:`PathVerb.kMove`, four :py:attr:`PathVerb.kConic`,
        and :py:attr:`PathVerb.kClose`.

        Circle begins at: (x + radius, y), continuing clockwise if dir is
        :py:attr:`PathDirection.kCW`, and counterclockwise if dir is
        :py:attr:`PathDirection.kCCW`.

        Has no effect if radius is zero or negative.

        :param float x: center of circle
        :param float y: center of circle
        :param float radius: distance from center to edge
        :param skia.PathDirection dir: :py:class:`PathDirection` to wind circle
        :rtype: :py:class:`Path`
        )docstring",
        py::arg("x"), py::arg("y"), py::arg("radius"),
        py::arg("dir") = SkPathDirection::kCW)
    .def("addArc", &SkPath::addArc,
        R"docstring(
        Appends arc to :py:class:`Path`, as the start of new contour.

        Arc added is part of ellipse bounded by oval, from startAngle through
        sweepAngle. Both startAngle and sweepAngle are measured in degrees,
        where zero degrees is aligned with the positive x-axis, and positive
        sweeps extends arc clockwise.

        If sweepAngle <= -360, or sweepAngle >= 360; and startAngle modulo 90 is
        nearly zero, append oval instead of arc. Otherwise, sweepAngle values
        are treated modulo 360, and arc may or may not draw depending on numeric
        rounding.

        :param skia.Rect oval: bounds of ellipse containing arc
        :param float startAngle: starting angle of arc in degrees
        :param float sweepAngle: sweep, in degrees. Positive is clockwise;
            treated modulo 360
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("oval"), py::arg("startAngle"), py::arg("sweepAngle"))
    .def("addRoundRect",
        py::overload_cast<const SkRect&, SkScalar, SkScalar, SkPathDirection>(
            &SkPath::addRoundRect),
        R"docstring(
        Appends :py:class:`RRect` to :py:class:`Path`, creating a new closed
        contour.

        :py:class:`RRect` has bounds equal to rect; each corner is 90 degrees of
        an ellipse with radii (rx, ry). If dir is
        :py:attr:`~skia.PathDirection.kCW`, :py:class:`RRect` starts at top-left
        of the lower-left corner and winds clockwise. If dir is
        :py:attr:`~skia.PathDirection.kCCW`, :py:class:`RRect` starts at the
        bottom-left of the upper-left corner and winds counterclockwise.

        If either rx or ry is too large, rx and ry are scaled uniformly until
        the corners fit. If rx or ry is less than or equal to zero,
        :py:meth:`addRoundRect` appends :py:class:`Rect` rect to
        :py:class:`Path`.

        After appending, :py:class:`Path` may be empty, or may contain:
        :py:class:`Rect`, oval, or :py:class:`RRect`.

        :rect: bounds of :py:class:`RRect`
        :rx: x-axis radius of rounded corners on the :py:class:`RRect`
        :ry: y-axis radius of rounded corners on the :py:class:`RRect`
        :dir: :py:class:`Path`::Direction to wind :py:class:`RRect`
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("rect"), py::arg("rx"), py::arg("ry"),
        py::arg("dir") = SkPathDirection::kCW)
    .def("addRoundRect",
        // py::overload_cast<const SkRect&, const SkScalar[], SkPathDirection>(
        //     &SkPath::addRoundRect),
        [] (SkPath& path, const SkRect& rect,
            py::iterable radii, SkPathDirection dir) {
            auto radii_ = radii.cast<std::vector<SkScalar>>();
            if (radii_.size() != 8) {
                std::stringstream stream;
                stream << "radii must have 8 elements (given " << radii_.size()
                    << " elements).";
                throw py::value_error(stream.str());
            }
            return path.addRoundRect(rect, &radii_[0], dir);
        },
        R"docstring(
        Appends :py:class:`RRect` to :py:class:`Path`, creating a new closed
        contour.

        :py:class:`RRect` has bounds equal to rect; each corner is 90 degrees of
        an ellipse with radii from the array.

        :rect: bounds of :py:class:`RRect`
        :radii: array of 8 :py:class:`Scalar` values, a radius pair for each
            corner
        :dir: :py:class:`Path`::Direction to wind :py:class:`RRect`
        :Returns: reference to :py:class:`Path`
        )docstring",
        py::arg("rect"), py::arg("radii"),
        py::arg("dir") = SkPathDirection::kCW)
    .def("addRRect",
        py::overload_cast<const SkRRect&, SkPathDirection>(&SkPath::addRRect),
        R"docstring(
        Adds rrect to :py:class:`Path`, creating a new closed contour.

        If dir is :py:attr:`~skia.PathDirection.kCW`, rrect starts at top-left
        of the lower-left corner and winds clockwise. If dir is
        :py:attr:`~skia.PathDirection.kCCW`, rrect starts at the bottom-left of
        the upper-left corner and winds counterclockwise.

        After appending, :py:class:`Path` may be empty, or may contain:
        :py:class:`Rect`, oval, or :py:class:`RRect`.

        :rrect: bounds and radii of rounded rectangle
        :dir: :py:class:`Path`::Direction to wind :py:class:`RRect`
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("rrect"), py::arg("dir") = SkPathDirection::kCW)
    .def("addRRect",
        py::overload_cast<const SkRRect&, SkPathDirection, unsigned>(
            &SkPath::addRRect),
        R"docstring(
        Adds rrect to :py:class:`Path`, creating a new closed contour.

        If dir is:py:attr:`~skia.PathDirection.kCW`, rrect winds clockwise; if
        dir is :py:attr:`~skia.PathDirection.kCCW`, rrect winds
        counterclockwise. start determines the first point of rrect to add.

        :rrect: bounds and radii of rounded rectangle
        :dir: :py:class:`Path`::Direction to wind :py:class:`RRect`
        :start: index of initial point of :py:class:`RRect`
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("rrect"), py::arg("dir"), py::arg("start"))
    .def("addPoly",
        [] (SkPath& path, const std::vector<SkPoint>& pts, bool close) {
            return path.addPoly(&pts[0], pts.size(), close);
        },
        R"docstring(
        Adds contour created from pts.

        Contour added starts at pts[0], then adds a line for every additional
        :py:class:`Point` in pts. If close is true, appends
        :py:attr:`~skia.Path.kClose_Verb` to :py:class:`Path`, connecting last
        and first :py:class:`Point` in pts.

        If pts is empty, append :py:attr:`~skia.Path.kMove_Verb` to path.

        :param pts: iterable of :py:class:`Point`
        :param close: true to add line connecting contour end and start
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("pts"), py::arg("close"))
    .def("addPath",
        py::overload_cast<const SkPath&, SkScalar, SkScalar,
        SkPath::AddPathMode>(&SkPath::addPath),
        R"docstring(
        Appends src to :py:class:`Path`, offset by (dx, dy).

        If mode is :py:attr:`~Path.kAppend_AddPathMode`, src verb array,
        :py:class:`Point` array, and conic weights are added unaltered. If mode
        is :py:attr:`~Path.kExtend_AddPathMode`, add line before appending
        verbs, :py:class:`Point`, and conic weights.

        :src: :py:class:`Path` verbs, :py:class:`Point`, and conic weights to
            add
        :dx: offset added to src :py:class:`Point` array x-axis coordinates
        :dy: offset added to src :py:class:`Point` array y-axis coordinates
        :mode: :py:attr:`~Path.kAppend_AddPathMode` or
            :py:attr:`~Path.kExtend_AddPathMode`

        refe:return: rence to :py:class:`Path`
        )docstring",
        py::arg("src"), py::arg("dx"), py::arg("dy"),
        py::arg("mode") = SkPath::kAppend_AddPathMode)
    .def("addPath",
        py::overload_cast<const SkPath&, SkPath::AddPathMode>(&SkPath::addPath),
        R"docstring(
        Appends src to :py:class:`Path`.

        If mode is :py:attr:`~Path.kAppend_AddPathMode`, src verb array,
        :py:class:`Point` array, and conic weights are added unaltered. If mode
        is :py:attr:`~Path.kExtend_AddPathMode`, add line before appending
        verbs, :py:class:`Point`, and conic weights.

        :src: :py:class:`Path` verbs, :py:class:`Point`, and conic weights to
            add
        :mode: :py:attr:`~Path.kAppend_AddPathMode` or
            :py:attr:`~Path.kExtend_AddPathMode`

        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("src"), py::arg("mode") = SkPath::kAppend_AddPathMode)
    .def("addPath",
        py::overload_cast<const SkPath&, const SkMatrix&, SkPath::AddPathMode>(
            &SkPath::addPath),
        R"docstring(
        Appends src to :py:class:`Path`, transformed by matrix.

        Transformed curves may have different verbs, :py:class:`Point`, and
        conic weights.

        If mode is :py:attr:`~Path.kAppend_AddPathMode`, src verb array,
        :py:class:`Point` array, and conic weights are added unaltered. If mode
        is :py:attr:`~Path.kExtend_AddPathMode`, add line before appending
        verbs, :py:class:`Point`, and conic weights.

        :src: :py:class:`Path` verbs, :py:class:`Point`, and conic weights to
            add
        :matrix: transform applied to src
        :mode: :py:attr:`~Path.kAppend_AddPathMode` or
            :py:attr:`~Path.kExtend_AddPathMode`

        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("src"), py::arg("matrix"),
        py::arg("mode") = SkPath::kAppend_AddPathMode)
    .def("reverseAddPath", &SkPath::reverseAddPath,
        R"docstring(
        Appends src to :py:class:`Path`, from back to front.

        Reversed src always appends a new contour to :py:class:`Path`.

        :param src: :py:class:`Path` verbs, :py:class:`Point`, and conic weights
            to add
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("src"))
    .def("offset",
        py::overload_cast<SkScalar, SkScalar, SkPath*>(
            &SkPath::offset, py::const_),
        R"docstring(
        Offsets :py:class:`Point` array by (dx, dy).

        Offset :py:class:`Path` replaces dst. If dst is nullptr,
        :py:class:`Path` is replaced by offset data.

        :param float dx:  offset added to :py:class:`Point` array x-axis
            coordinates
        :param float dy:  offset added to :py:class:`Point` array y-axis
            coordinates
        :param skia.Path dst: overwritten, translated copy of :py:class:`Path`;
            may be nullptr
        )docstring",
        py::arg("dx"), py::arg("dy"), py::arg("dst") = nullptr)
    // .def("offset",
    //     py::overload_cast<SkScalar, SkScalar>(&SkPath::offset),
    //     "Offsets SkPoint array by (dx, dy).")
    .def("transform",
        py::overload_cast<const SkMatrix&, SkPath*, SkApplyPerspectiveClip>(
            &SkPath::transform, py::const_),
        R"docstring(
        Transforms verb array, :py:class:`Point` array, and weight by matrix.

        transform may change verbs and increase their number. Transformed
        :py:class:`Path` replaces dst; if dst is nullptr, original data is
        replaced.

        :param skia.Matrix matrix: :py:class:`Matrix` to apply to
            :py:class:`Path`
        :param skia.Path dst: overwritten, transformed copy of :py:class:`Path`;
            may be nullptr
        :param skia.ApplyPerspectiveClip pc: whether to apply perspective
            clipping
        )docstring",
        py::arg("matrix"), py::arg("dst") = nullptr,
        py::arg("pc") = SkApplyPerspectiveClip::kYes)
    // .def("transform",
    //     py::overload_cast<const SkMatrix&, SkApplyPerspectiveClip>(
    //         &SkPath::transform),
    //     "Transforms verb array, SkPoint array, and weight by matrix.")
    .def("getLastPt", &SkPath::getLastPt,
        R"docstring(
        Returns last point on :py:class:`Path` in lastPt.

        Returns false if :py:class:`Point` array is empty, storing (0, 0) if
        lastPt is not nullptr.

        :param skia.Point lastPt: storage for final :py:class:`Point` in
            :py:class:`Point` array; may be nullptr
        :return: true if :py:class:`Point` array contains one or more
            :py:class:`Point`
        )docstring",
        py::arg("lastPt") = nullptr)
    .def("setLastPt",
        py::overload_cast<SkScalar, SkScalar>(&SkPath::setLastPt),
        R"docstring(
        Sets last point to (x, y).

        If :py:class:`Point` array is empty, append
        :py:attr:`~skia.Path.kMove_Verb` to verb array and append (x, y) to
        :py:class:`Point` array.

        :x: set x-axis value of last point
        :y: set y-axis value of last point
        )docstring",
        py::arg("x"), py::arg("y"))
    .def("setLastPt",
        py::overload_cast<const SkPoint&>(&SkPath::setLastPt),
        R"docstring(
        Sets the last point on the path.

        If :py:class:`Point` array is empty, append
        :py:attr:`~skia.Path.kMove_Verb` to verb array and append p to
        :py:class:`Point` array.

        :p: set value of last point
        )docstring",
        py::arg("p"))
    .def("getSegmentMasks", &SkPath::getSegmentMasks,
        R"docstring(
        Returns a mask, where each set bit corresponds to a
        :py:class:`~Path.SegmentMask` constant if :py:class:`Path` contains one
        or more verbs of that type.

        Returns zero if :py:class:`Path` contains no lines, or curves: quads,
        conics, or cubics.

        :py:meth:`getSegmentMasks` returns a cached result; it is very fast.

        :return: SegmentMask bits or zero
        )docstring")
    .def("contains", &SkPath::contains,
        R"docstring(
        Returns true if the point (x, y) is contained by :py:class:`Path`,
        taking into account FillType.

        :param float x: x-axis value of containment test
        :param float y: y-axis value of containment test
        :return: true if :py:class:`Point` is in :py:class:`Path`
        )docstring",
        py::arg("x"), py::arg("y"))
    // .def("dump",
    //     py::overload_cast<SkWStream*, bool, bool>(&SkPath::dump),
    //     "Writes text representation of SkPath to stream.")
    .def("dump",
        // py::overload_cast<>(&SkPath::dump, py::const_),
        [] (const SkPath& path) {
            py::scoped_ostream_redirect stream;
            path.dump();
        },
        R"docstring(
        Writes text representation of :py:class:`Path` to standard output.

        The representation may be directly compiled as C++ code. Floating point
        values are written with limited precision; it may not be possible to
        reconstruct original :py:class:`Path` from output.
        )docstring")
    .def("dumpHex",
        // &SkPath::dumpHex,
        [] (const SkPath& path) {
            py::scoped_ostream_redirect stream;
            path.dumpHex();
        },
        R"docstring(
        Writes text representation of :py:class:`Path` to standard output.

        The representation may be directly compiled as C++ code. Floating point
        values are written in hexadecimal to preserve their exact bit pattern.
        The output reconstructs the original :py:class:`Path`.

        Use instead of :py:meth:`dump` when submitting
        )docstring")
    .def("writeToMemory",
        [] (const SkPath& path) {
            size_t size = path.writeToMemory(nullptr);
            std::vector<char> buffer(size);
            path.writeToMemory(&buffer[0]);
            return py::bytes(&buffer[0], size);
        },
        R"docstring(
        Writes :py:class:`Path` to buffer, returning bytes.

        Writes :py:class:`~Path.FillType`, verb array, :py:class:`Point` array,
        conic weight, and additionally writes computed information like
        :py:class:`~Path.Convexity` and bounds.

        Use only be used in concert with :py:meth:`readFromMemory`; the format
        used for :py:class:`Path` in memory is not guaranteed.

        :return: serialized bytes
        )docstring")
    .def("serialize", &SkPath::serialize,
        R"docstring(
        Writes :py:class:`Path` to buffer, returning the buffer written to,
        wrapped in :py:class:`Data`.

        :py:meth:`serialize` writes :py:class:`~Path.FillType`, verb array,
        :py:class:`Point` array, conic weight, and additionally writes computed
        information like :py:class:`~Path.Convexity` and bounds.

        :py:meth:`serialize` should only be used in concert with
        :py:meth:`readFromMemory`. The format used for :py:class:`Path` in
        memory is not guaranteed.

        :return: :py:class:`Path` data wrapped in :py:class:`Data` buffer
        )docstring")
    .def("readFromMemory",
        [] (SkPath& path, py::buffer b) {
            auto info = b.request();
            auto length = (info.ndim > 0) ? info.shape[0] * info.strides[0] : 0;
            return path.readFromMemory(info.ptr, length);
        },
        R"docstring(
        Initializes :py:class:`Path` from buffer of size length.

        Returns zero if the buffer is data is inconsistent, or the length is too
        small.

        Reads :py:class:`Path.FillType`, verb array, :py:class:`Point` array,
        conic weight, and additionally reads computed information like
        :py:class:`Path.Convexity` and bounds.

        Used only in concert with :py:meth:`writeToMemory`; the format used for
        :py:class:`Path` in memory is not guaranteed.

        :param Union[bytes,bytearray,memoryview] buffer: storage for
            :py:class:`Path`
        :return: number of bytes read, or zero on failure
        )docstring",
        py::arg("buffer"))
    .def("getGenerationID", &SkPath::getGenerationID,
        R"docstring(
        (See Skia bug 1762.) Returns a non-zero, globally unique value.

        A different value is returned if verb array, :py:class:`Point` array, or
        conic weight changes.

        Setting :py:class:`Path.FillType` does not change generation identifier.

        Each time the path is modified, a different generation identifier will
        be returned. :py:class:`Path.FillType` does affect generation identifier
        on Android framework.

        :return: non-zero, globally unique value
        )docstring")
    .def("isValid", &SkPath::isValid,
        R"docstring(
        Returns if :py:class:`Path` data is consistent.

        Corrupt :py:class:`Path` data is detected if internal values are out of
        range or internal storage does not match array dimensions.

        :return: true if :py:class:`Path` data is consistent
        )docstring")
    .def_static("IsLineDegenerate", &SkPath::IsLineDegenerate,
        R"docstring(
        Tests if line between :py:class:`Point` pair is degenerate.

        Line with no length or that moves a very short distance is degenerate;
        it is treated as a point.

        exact changes the equality test. If true, returns true only if p1 equals
        p2. If false, returns true if p1 equals or nearly equals p2.

        :param skia.Point p1: line start point
        :param skia.Point p2: line end point
        :param bool exact: if false, allow nearly equals
        :return: true if line is degenerate; its length is effectively zero
        )docstring",
        py::arg("p1"), py::arg("p2"), py::arg("exact"))
    .def_static("IsQuadDegenerate", &SkPath::IsQuadDegenerate,
        R"docstring(
        Tests if quad is degenerate.

        Quad with no length or that moves a very short distance is degenerate;
        it is treated as a point.

        :param skia.Path p1: quad start point
        :param skia.Path p2: quad control point
        :param skia.Path p3: quad end point
        :param bool exact: if true, returns true only if p1, p2, and p3 are
            equal; if false, returns true if p1, p2, and p3 are equal or nearly
            equal
        :return: true if quad is degenerate; its length is effectively zero
        )docstring",
        py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("exact"))
    .def_static("IsCubicDegenerate", &SkPath::IsCubicDegenerate,
        R"docstring(
        Tests if cubic is degenerate.

        Cubic with no length or that moves a very short distance is degenerate;
        it is treated as a point.

        :param skia.Path p1: cubic start point
        :param skia.Path p2: cubic control point 1
        :param skia.Path p3: cubic control point 2
        :param skia.Path p4: cubic end point
        :param bool exact: if true, returns true only if p1, p2, p3, and p4 are
            equal; if false, returns true if p1, p2, p3, and p4 are equal or
            nearly equal
        :return: true if cubic is degenerate; its length is effectively zero
        )docstring",
        py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("p4"),
        py::arg("exact"))
    .def_static("ConvertConicToQuads",
        [] (const SkPoint& p0, const SkPoint& p1, const SkPoint& p2,
            SkScalar w, int pow2) {
            auto size = (1 + 2 * (1 << pow2));
            std::vector<SkPoint> pts(size);
            auto result = SkPath::ConvertConicToQuads(
                p0, p1, p2, w, &pts[0], pow2);
            if (result < size)
                pts.erase(pts.begin() + result, pts.end());
            return pts;
        },
        R"docstring(
        Approximates conic with quad array.

        Conic is constructed from start :py:class:`Point` p0, control
        :py:class:`Point` p1, end :py:class:`Point` p2, and weight w. Maximum
        quad count is 2 to the pow2. Every third point in array shares last
        :py:class:`Point` of previous quad and first :py:class:`Point` of next
        quad. Maximum possible return array size is given by:
        (1 + 2 * (1 << pow2)) * sizeof(:py:class:`Point`).

        Returns quad count used the approximation, which may be smaller than the
        number requested.

        conic weight determines the amount of influence conic control point has
        on the curve. w less than one represents an elliptical section. w
        greater than one represents a hyperbolic section. w equal to one
        represents a parabolic section.

        Two quad curves are sufficient to approximate an elliptical conic with a
        sweep of up to 90 degrees; in this case, set pow2 to one.

        :param skia.Point p0: conic start :py:class:`Point`
        :param skia.Point p1: conic control :py:class:`Point`
        :param skia.Point p2: conic end :py:class:`Point`
        :param float w: conic weight
        :param int pow2: quad count, as power of two, normally 0 to 5 (1 to 32
            quad curves)
        :return: quad array
        :rtype: List[skia.Point]
        )docstring",
        py::arg("p0"), py::arg("p1"), py::arg("p2"), py::arg("w"),
        py::arg("pow2"))
    .def(py::self == py::self,
        R"docstring(
        Compares a and b; returns true if :py:class:`Path.FillType`, verb array,
        :py:class:`Point` array, and weights are equivalent.

        :param skia.Path other: :py:class:`Path` to compare
        :return: true if :py:class:`Path` pair are equivalent
        )docstring",
        py::arg("other"))
    .def(py::self != py::self,
        R"docstring(
        Compares a and b; returns true if :py:class:`Path.FillType`, verb array,
        :py:class:`Point` array, and weights are not equivalent.

        :param skia.Path other: :py:class:`Path` to compare
        :return: true if :py:class:`Path` pair are not equivalent
        )docstring",
        py::arg("other"))
    ;
}
