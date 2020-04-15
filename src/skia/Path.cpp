#include "common.h"
#include <pybind11/operators.h>
#include <pybind11/stl.h>


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
    .value("kLine", SkPathSegmentMask::kLine_SkPathSegmentMask)
    .value("kQuad", SkPathSegmentMask::kQuad_SkPathSegmentMask)
    .value("kConic", SkPathSegmentMask::kConic_SkPathSegmentMask)
    .value("kCubic", SkPathSegmentMask::kCubic_SkPathSegmentMask)
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
    .value("kSmall", SkPath::ArcSize::kSmall_ArcSize, "smaller of arc pair")
    .value("kLarge", SkPath::ArcSize::kLarge_ArcSize, "larger of arc pair")
    .export_values();

py::enum_<SkPath::AddPathMode>(path, "AddPathMode")
    .value("kAppend", SkPath::AddPathMode::kAppend_AddPathMode,
        "appended to destination unaltered")
    .value("kExtend", SkPath::AddPathMode::kExtend_AddPathMode,
        "add line if prior contour is not closed")
    .export_values();

py::enum_<SkPath::SegmentMask>(path, "SegmentMask")
    .value("kLine", SkPath::SegmentMask::kLine_SegmentMask)
    .value("kQuad", SkPath::SegmentMask::kQuad_SegmentMask)
    .value("kConic", SkPath::SegmentMask::kConic_SegmentMask)
    .value("kCubic", SkPath::SegmentMask::kCubic_SegmentMask)
    .export_values();

py::enum_<SkPath::Verb>(path, "Verb")
    .value("kMove", SkPath::Verb::kMove_Verb)
    .value("kLine", SkPath::Verb::kLine_Verb)
    .value("kQuad", SkPath::Verb::kQuad_Verb)
    .value("kConic", SkPath::Verb::kConic_Verb)
    .value("kCubic", SkPath::Verb::kCubic_Verb)
    .value("kClose", SkPath::Verb::kClose_Verb)
    .value("kDone", SkPath::Verb::kDone_Verb)
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

        next() on :py:class:`Iter` returns :py:attr:`~skia.Path.Verb.kDone`. Call
        setPath to initialize :py:class:`Iter` at a later time.

        :return: :py:class:`Iter` of empty SkPath
        )docstring")
    .def(py::init<const SkPath&, bool>(),
        R"docstring(
        Sets :py:class:`Iter` to return elements of verb array,
        :py:class:`Point` array, and conic weight in path.

        If forceClose is true, :py:class:`Iter` will add
        :py:attr:`~skia.Path.Verb.kLine` and :py:attr:`~skia.Path.Verb.kClose`
        after each open contour. path is not altered.

        :path: :py:class:`Path` to iterate
        :forceClose: true if open contours generate
            :py:attr:`~skia.Path.Verb.kClose`

        :return: :py:class:`Iter` of path
        )docstring",
        py::arg("path"), py::arg("forceClose"))
    .def("setPath", &SkPath::Iter::setPath,
        R"docstring(
        Sets :py:class:`Iter` to return elements of verb array,
        :py:class:`Point` array, and conic weight in path.

        If forceClose is true, :py:class:`Iter` will add
        :py:attr:`~skia.Path.Verb.kLine` and :py:attr:`~skia.Path.Verb.kClose`
        after each open contour. path is not altered.

        :param skia.Path path: :py:class:`Path` to iterate
        :param bool forceClose: true if open contours generate
            :py:attr:`~skia.Path.Verb.kClose`
        )docstring",
        py::arg("path"), py::arg("forceClose"))
    .def("next", &Iter_next<SkPath::Iter>,
        R"docstring(
        Returns next :py:class:`~skia.Path.Verb` in verb array, and advances
        :py:class:`Iter`.

        When verb array is exhausted, returns :py:attr:`~skia.Path.Verb.kDone`.

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
        :py:attr:`~skia.Path.Verb.kConic`.

        If :py:meth:`next` has not been called, or :py:meth:`next` did not
        return :py:attr:`~skia.Path.Verb.kConic`, result is undefined.

        :return: conic weight for conic :py:class:`Point` returned by
            :py:meth:`next`
        )docstring")
    .def("isCloseLine", &SkPath::Iter::isCloseLine,
        R"docstring(
        Returns true if last:py:attr:`~skia.Path.Verb.kLine` returned by
        :py:meth:`next` was generated by :py:attr:`~skia.Path.Verb.kClose`.

        When true, the end point returned by :py:meth:`next` is also the start
        point of contour.

        If :py:meth:`next` has not been called, or :py:meth:`next` did not
        return :py:attr:`~skia.Path.Verb.kLine`, result is undefined.

        :return: true if last :py:attr:`~skia.Path.Verb.kLine` was generated by
            :py:attr:`~skia.Path.Verb.kClose`
        )docstring")
    .def("isClosedContour", &SkPath::Iter::isClosedContour,
        R"docstring(
        Returns true if subsequent calls to :py:meth:`next` return
        :py:attr:`~skia.Path.Verb.kClose` before returning
        :py:attr:`~skia.Path.Verb.kMove`.

        if true, contour :py:class:`Iter` is processing may end with
        :py:attr:`~skia.Path.Verb.kClose`, or :py:class:`Iter` may have been
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
        :py:attr:`~skia.Path.Verb.kDone`. Call :py:meth:`setPath` to initialize
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

        When verb array is exhausted, returns :py:attr:`~skia.Path.Verb.kDone`.

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
        :py:attr:`~skia.Path.Verb.kConic`.

        If :py:meth:`next` has not been called, or :py:meth:`next` did not
        return :py:attr:`~skia.Path.Verb.kConic`, result is undefined.

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
        py::arg("oval"))
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
        py::arg("rrect"))
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
            :py:attr:`~skia.Path.Verb.kClose`
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
        :py:attr:`~skia.Path.Verb.kMove`, :py:attr:`~skia.Path.Verb.kLine`.

        If :py:class:`Path` contains one line and line is not nullptr, line is
        set to line start point and line end point. Returns false if
        :py:class:`Path` is not one line; line is unaltered.

        :param skia.Point p0: storage for line start. May be nullptr
        :param skia.Point p1: storage for line end. May be nullptr
        :return: true if :py:class:`Path` contains exactly one line
        )docstring",
        py::arg("p0"), py::arg("p1"))
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
        Returns the number of verbs: :py:attr:`~Path.Verb.kMove`,
        :py:attr:`~Path.Verb.kLine`, :py:attr:`~Path.Verb.kQuad`,
        :py:attr:`~Path.Verb.kConic`, :py:attr:`~Path.Verb.kCubic`, and
        :py:attr:`~Path.Verb.kClose`; added to :py:class:`Path`.

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
        :py:attr:`~Path.Verb.kMove` that define empty contours.

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
        :py:attr:`~Path.Verb.kClose`, last point is set to (0, 0) before adding
        line.

        lineTo() appends :py:attr:`~Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed. lineTo() then appends
        :py:attr:`~Path.Verb.kLine` to verb array and (x, y) to
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
        :py:attr:`~Path.Verb.kClose`, last point is set to (0, 0) before adding
        line.

        lineTo() appends :py:attr:`~Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed. lineTo() then appends
        :py:attr:`~Path.Verb.kLine` to verb array and p to :py:class:`Point`
        array.

        :p: end :py:class:`Point` of added line
        :return: reference to :py:class:`Path`
        )docstring",
        py::arg("p"))
    .def("rLineTo", &SkPath::rLineTo,
        R"docstring(
        Adds line from last point to vector (dx, dy).

        If :py:class:`Path` is empty, or last :py:class:`~Path.Verb` is
        :py:attr:`~skia.Path.Verb.kClose`, last point is set to (0, 0) before
        adding line.

        Appends :py:attr:`~skia.Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.Verb.kLine` to verb array and line end to
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
        :py:attr:`~skia.Path.Verb.kClose`, last point is set to (0, 0) before
        adding quad.

        Appends :py:attr:`~skia.Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.Verb.kQuad` to verb array; and (x1, y1), (x2, y2)
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
        :py:attr:`~skia.Path.Verb.kClose`, last point is set to (0, 0) before
        adding quad.

        Appends :py:attr:`~skia.Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.Verb.kQuad` to verb array; and :py:class:`Point`
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
        :py:attr:`~skia.Path.Verb.kClose`, last point is set to (0, 0) before
        adding quad.

        Appends :py:attr:`~skia.Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.Verb.kQuad` to verb array;and appends quad control
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
        :py:attr:`~skia.Path.Verb.kClose`, last point is set to (0, 0) before
        adding conic.

        Appends :py:attr:`~skia.Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed.

        If w is finite and not one, appends :py:attr:`~skia.Path.Verb.kConic` to
        verb array; and (x1, y1), (x2, y2) to :py:class:`Point` array; and w to
        conic weights.

        If w is one, appends :py:attr:`~skia.Path.Verb.kQuad` to verb array, and
        (x1, y1), (x2, y2) to :py:class:`Point` array.

        If w is not finite, appends :py:attr:`~skia.Path.Verb.kLine` twice to
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
        :py:attr:`~skia.Path.Verb.kClose`, last point is set to (0, 0) before
        adding conic.

        Appends :py:attr:`~skia.Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed.

        If w is finite and not one, appends :py:attr:`~skia.Path.Verb.kConic` to
        verb array; and :py:class:`Point` p1, p2 to :py:class:`Point` array; and
        w to conic weights.

        If w is one, appends :py:attr:`~skia.Path.Verb.kQuad` to verb array, and
        :py:class:`Point` p1, p2 to :py:class:`Point` array.

        If w is not finite, appends :py:attr:`~skia.Path.Verb.kLine` twice to
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
        :py:attr:`~skia.Path.Verb.kClose`, last point is set to (0, 0) before
        adding conic.

        Appends :py:attr:`~skia.Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed.

        If w is finite and not one, next appends
        :py:attr:`~skia.Path.Verb.kConic` to verb array, and w is recorded as
        conic weight; otherwise, if w is one, appends
        :py:attr:`~skia.Path.Verb.kQuad` to verb array; or if w is not finite,
        appends :py:attr:`~skia.Path.Verb.kLine` twice to verb array.

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
        :py:attr:`~skia.Path.Verb.kClose`, last point is set to (0, 0) before
        adding cubic.

        Appends :py:attr:`~skia.Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.Verb.kCubic` to verb array; and (x1, y1), (x2, y2),
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
        :py:attr:`~skia.Path.Verb.kClose`, last point is set to (0, 0) before
        adding cubic.

        Appends :py:attr:`~skia.Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.Verb.kCubic` to verb array; and :py:class:`Point`
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
        :py:attr:`~skia.Path.Verb.kClose`, last point is set to (0, 0) before
        adding cubic.

        Appends :py:attr:`~skia.Path.Verb.kMove` to verb array and (0, 0) to
        :py:class:`Point` array, if needed; then appends
        :py:attr:`~skia.Path.Verb.kCubic` to verb array; and appends cubic
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
        "Appends arc to SkPath.")
    .def("arcTo",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkPath::arcTo),
        "Appends arc to SkPath, after appending line if needed.")
    .def("arcTo",
        py::overload_cast<const SkPoint, const SkPoint, SkScalar>(
            &SkPath::arcTo),
        "Appends arc to SkPath, after appending line if needed.")
    .def("arcTo",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkPath::ArcSize,
            SkPathDirection, SkScalar, SkScalar>(&SkPath::arcTo),
        "Appends arc to SkPath.")
    .def("arcTo",
        py::overload_cast<const SkPoint, SkScalar, SkPath::ArcSize,
            SkPathDirection, const SkPoint>(&SkPath::arcTo),
        "Appends arc to SkPath.")
    .def("rArcTo", &SkPath::rArcTo,
        "Appends arc to SkPath, relative to last SkPath SkPoint.")
    .def("close", &SkPath::close,
        "Appends kClose_Verb to SkPath.")
    .def("isRect", &SkPath::isRect,
        "Returns true if SkPath is equivalent to SkRect when filled.")
    .def("addRect",
        py::overload_cast<const SkRect&, SkPathDirection>(&SkPath::addRect),
        "Adds SkRect to SkPath, appending kMove_Verb, three kLine_Verb, and "
        "kClose_Verb, starting with top-left corner of SkRect; followed by "
        "top-right, bottom-right, and bottom-left if dir is kCW_Direction; "
        "or followed by bottom-left, bottom-right, and top-right if dir is "
        "kCCW_Direction.")
    .def("addRect",
        py::overload_cast<const SkRect&, SkPathDirection, unsigned>(
            &SkPath::addRect),
        "Adds SkRect to SkPath, appending kMove_Verb, three kLine_Verb, and "
        "kClose_Verb.")
    .def("addRect",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar,
        SkPathDirection>(&SkPath::addRect),
        "Adds SkRect (left, top, right, bottom) to SkPath, appending "
        "kMove_Verb, three kLine_Verb, and kClose_Verb, starting with "
        "top-left corner of SkRect; followed by top-right, bottom-right, "
        "and bottom-left if dir is kCW_Direction; or followed by bottom-left, "
        "bottom-right, and top-right if dir is kCCW_Direction.")
    .def("addOval",
        py::overload_cast<const SkRect&, SkPathDirection>(&SkPath::addOval),
        "Adds oval to path, appending kMove_Verb, four kConic_Verb, and "
        "kClose_Verb.")
    .def("addOval",
        py::overload_cast<const SkRect&, SkPathDirection, unsigned>(
            &SkPath::addOval),
        "Adds oval to SkPath, appending kMove_Verb, four kConic_Verb, and "
        "kClose_Verb.")
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
        "Appends arc to SkPath, as the start of new contour.")
    .def("addRoundRect",
        py::overload_cast<const SkRect&, SkScalar, SkScalar, SkPathDirection>(
            &SkPath::addRoundRect),
        "Appends SkRRect to SkPath, creating a new closed contour.")
    .def("addRoundRect",
        py::overload_cast<const SkRect&, const SkScalar[], SkPathDirection>(
            &SkPath::addRoundRect),
        "Appends SkRRect to SkPath, creating a new closed contour.")
    .def("addRRect",
        py::overload_cast<const SkRRect&, SkPathDirection>(&SkPath::addRRect),
        "Adds rrect to SkPath, creating a new closed contour.")
    .def("addRRect",
        py::overload_cast<const SkRRect&, SkPathDirection, unsigned>(
            &SkPath::addRRect),
        "Adds rrect to SkPath, creating a new closed contour.")
    .def("addPoly",
        py::overload_cast<const SkPoint[], int, bool>(&SkPath::addPoly),
        "Adds contour created from line array, adding (count - 1) line "
        "segments.")
    .def("addPoly",
        py::overload_cast<const std::initializer_list<SkPoint>&, bool>(
            &SkPath::addPoly),
        "Adds contour created from list.")
    .def("addPath",
        py::overload_cast<const SkPath&, SkScalar, SkScalar,
        SkPath::AddPathMode>(&SkPath::addPath),
        "Appends src to SkPath, offset by (dx, dy).")
    .def("addPath",
        py::overload_cast<const SkPath&, SkPath::AddPathMode>(&SkPath::addPath),
        "Appends src to SkPath.")
    .def("addPath",
        py::overload_cast<const SkPath&, const SkMatrix&, SkPath::AddPathMode>(
            &SkPath::addPath),
        "Appends src to SkPath, transformed by matrix.")
    .def("reverseAddPath", &SkPath::reverseAddPath,
        "Appends src to SkPath, from back to front.")
    .def("offset",
        py::overload_cast<SkScalar, SkScalar, SkPath*>(
            &SkPath::offset, py::const_),
        "Offsets SkPoint array by (dx, dy).")
    .def("offset",
        py::overload_cast<SkScalar, SkScalar>(&SkPath::offset),
        "Offsets SkPoint array by (dx, dy).")
    .def("transform",
        py::overload_cast<const SkMatrix&, SkPath*, SkApplyPerspectiveClip>(
            &SkPath::transform, py::const_),
        "Transforms verb array, SkPoint array, and weight by matrix.")
    .def("transform",
        py::overload_cast<const SkMatrix&, SkApplyPerspectiveClip>(
            &SkPath::transform),
        "Transforms verb array, SkPoint array, and weight by matrix.")
    .def("getLastPt", &SkPath::getLastPt,
        "Returns last point on SkPath in lastPt.")
    .def("setLastPt",
        py::overload_cast<SkScalar, SkScalar>(&SkPath::setLastPt),
        "Sets last point to (x, y).")
    .def("setLastPt",
        py::overload_cast<const SkPoint&>(&SkPath::setLastPt),
        "Sets the last point on the path.")
    .def("getSegmentMasks", &SkPath::getSegmentMasks,
        "Returns a mask, where each set bit corresponds to a SegmentMask "
        "constant if SkPath contains one or more verbs of that type.")
    .def("contains", &SkPath::contains,
        "Returns true if the point (x, y) is contained by SkPath, taking into "
        "account FillType.")
    // .def("dump",
    //     py::overload_cast<SkWStream*, bool, bool>(&SkPath::dump),
    //     "Writes text representation of SkPath to stream.")
    .def("dump",
        py::overload_cast<>(&SkPath::dump, py::const_),
        "Writes text representation of SkPath to standard output.")
    .def("dumpHex", &SkPath::dumpHex,
        "Writes text representation of SkPath to standard output.")
    .def("writeToMemory", &SkPath::writeToMemory,
        "Writes SkPath to buffer, returning the number of bytes written.")
    .def("serialize", &SkPath::serialize,
        "Writes SkPath to buffer, returning the buffer written to, wrapped in "
        "SkData.")
    .def("readFromMemory", &SkPath::readFromMemory,
        "Initializes SkPath from buffer of size length.")
    .def("getGenerationID", &SkPath::getGenerationID,
        "(See Skia bug 1762.) Returns a non-zero, globally unique value.")
    .def("isValid", &SkPath::isValid,
        "Returns if SkPath data is consistent.")
    .def_static("IsLineDegenerate", &SkPath::IsLineDegenerate,
        "Tests if line between SkPoint pair is degenerate.")
    .def_static("IsQuadDegenerate", &SkPath::IsQuadDegenerate,
        "Tests if quad is degenerate.")
    .def_static("IsCubicDegenerate", &SkPath::IsCubicDegenerate,
        "Tests if cubic is degenerate.")
    .def_static("ConvertConicToQuads", &SkPath::ConvertConicToQuads,
        "Approximates conic with quad array.")
    .def(py::self == py::self,
        "Compares a and b; returns true if SkPath::FillType, verb array, "
        "SkPoint array, and weights are equivalent.")
    .def(py::self != py::self,
        "Compares a and b; returns true if SkPath::FillType, verb array, "
        "SkPoint array, and weights are not equivalent.")
    ;
}
