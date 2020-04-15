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
    .def(py::init<>(), "Constructs an empty SkPath.")
    .def(py::init<const SkPath&>(), "Constructs a copy of an existing path.")
    .def("isInterpolatable", &SkPath::isInterpolatable,
        "Returns true if SkPath contain equal verbs and equal weights.")
    .def("interpolate", &SkPath::interpolate,
        "Interpolates between SkPath with SkPoint array of equal size.")
    .def("getFillType", &SkPath::getFillType,
        "Returns SkPathFillType, the rule used to fill SkPath.")
    .def("setFillType", &SkPath::setFillType,
        "Sets FillType, the rule used to fill SkPath.")
    .def("isInverseFillType", &SkPath::isInverseFillType,
        "Returns if FillType describes area outside SkPath geometry.")
    .def("toggleInverseFillType", &SkPath::toggleInverseFillType,
        "Replaces FillType with its inverse.")
    .def("getConvexityType", &SkPath::getConvexityType,
        "Returns the comvexity type, computing if needed.")
    .def("getConvexityTypeOrUnknown", &SkPath::getConvexityTypeOrUnknown,
        "If the path's convexity is already known, return it, else return "
        "kUnknown.")
    .def("setConvexityType",
        py::overload_cast<SkPathConvexityType>(&SkPath::setConvexityType),
        "Returns the comvexity type, computing if needed.")
    .def("isConvex", &SkPath::isConvex, "Returns true if the path is convex.")
    .def("isOval", &SkPath::isOval,
        "Returns true if this path is recognized as an oval or circle.")
    .def("isRRect", &SkPath::isRRect,
        "Returns true if path is representable as SkRRect.")
    .def("reset", &SkPath::reset, "Sets SkPath to its initial state.")
    .def("rewind", &SkPath::rewind,
        "Sets SkPath to its initial state, preserving internal storage.")
    .def("isEmpty", &SkPath::isEmpty, "Returns if SkPath is empty.")
    .def("isLastContourClosed", &SkPath::isLastContourClosed,
        "Returns if contour is closed.")
    .def("isFinite", &SkPath::isFinite,
        "Returns true for finite SkPoint array values between negative "
        "SK_ScalarMax and positive SK_ScalarMax.")
    .def("isVolatile", &SkPath::isVolatile,
        "Returns true if the path is volatile; it will not be altered or "
        "discarded by the caller after it is drawn.")
    .def("setIsVolatile", &SkPath::setIsVolatile,
        "Specifies whether SkPath is volatile; whether it will be altered or "
        "discarded by the caller after it is drawn.")
    .def("isLine", &SkPath::isLine,
        "Returns true if SkPath contains only one line; SkPath::Verb array "
        "has two entries: kMove_Verb, kLine_Verb.")
    .def("countPoints", &SkPath::countPoints,
        "Returns the number of points in SkPath.")
    .def("getPoint", &SkPath::getPoint,
        "Returns SkPoint at index in SkPoint array.")
    .def("getPoints", &SkPath::getPoints, "Returns number of points in SkPath.")
    .def("countVerbs", &SkPath::countVerbs,
        "Returns the number of verbs: kMove_Verb, kLine_Verb, kQuad_Verb, "
        "kConic_Verb, kCubic_Verb, and kClose_Verb; added to SkPath.")
    .def("getVerbs", &SkPath::getVerbs,
        "Returns the number of verbs in the path.")
    .def("approximateBytesUsed", &SkPath::approximateBytesUsed,
        "Returns the approximate byte size of the SkPath in memory.")
    .def("swap", &SkPath::swap,
        "Exchanges the verb array, SkPoint array, weights, and "
        "SkPath::FillType with other.")
    .def("getBounds", &SkPath::getBounds,
        "Returns minimum and maximum axes values of SkPoint array.")
    .def("updateBoundsCache", &SkPath::updateBoundsCache,
        "Updates internal bounds so that subsequent calls to getBounds() are "
        "instantaneous.")
    .def("computeTightBounds", &SkPath::computeTightBounds,
        "Returns minimum and maximum axes values of the lines and curves in "
        "SkPath.")
    .def("conservativelyContainsRect", &SkPath::conservativelyContainsRect,
        "Returns true if rect is contained by SkPath.")
    .def("incReserve", &SkPath::incReserve,
        "Grows SkPath verb array and SkPoint array to contain extraPtCount "
        "additional SkPoint.")
    .def("shrinkToFit", &SkPath::shrinkToFit,
        "Shrinks SkPath verb array and SkPoint array storage to discard "
        "unused capacity.")
    .def("moveTo",
        py::overload_cast<SkScalar, SkScalar>(&SkPath::moveTo),
        "Adds beginning of contour at SkPoint (x, y).")
    .def("moveTo",
        py::overload_cast<const SkPoint&>(&SkPath::moveTo),
        "Adds beginning of contour at SkPoint p.")
    .def("rMoveTo", &SkPath::rMoveTo,
        "Adds beginning of contour relative to last point.")
    .def("lineTo",
        py::overload_cast<SkScalar, SkScalar>(&SkPath::lineTo),
        "Adds line from last point to (x, y).")
    .def("lineTo",
        py::overload_cast<const SkPoint&>(&SkPath::lineTo),
        "Adds line from last point to SkPoint p.")
    .def("rLineTo", &SkPath::rLineTo,
        "Adds line from last point to vector (dx, dy).")
    .def("quadTo",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkPath::quadTo),
        "Adds quad from last point towards (x1, y1), to (x2, y2).")
    .def("quadTo",
        py::overload_cast<const SkPoint&, const SkPoint&>(&SkPath::quadTo),
        "Adds quad from last point towards SkPoint p1, to SkPoint p2.")
    .def("rQuadTo", &SkPath::rQuadTo,
        "Adds quad from last point towards vector (dx1, dy1), to vector (dx2, "
        "dy2).")
    .def("conicTo",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkPath::conicTo),
        "Adds conic from last point towards (x1, y1), to (x2, y2), weighted "
        "by w.")
    .def("conicTo",
        py::overload_cast<const SkPoint&, const SkPoint&, SkScalar>(
            &SkPath::conicTo),
        "Adds conic from last point towards SkPoint p1, to SkPoint p2, "
        "weighted by w.")
    .def("rConicTo", &SkPath::rConicTo,
        "Adds conic from last point towards vector (dx1, dy1), to vector "
        "(dx2, dy2), weighted by w.")
    .def("cubicTo",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar, SkScalar,
            SkScalar>(&SkPath::cubicTo),
        "Adds cubic from last point towards (x1, y1), then towards (x2, y2), "
        "ending at (x3, y3).")
    .def("cubicTo",
        py::overload_cast<const SkPoint&, const SkPoint&, const SkPoint&>(
            &SkPath::cubicTo),
        "Adds cubic from last point towards SkPoint p1, then towards SkPoint "
        "p2, ending at SkPoint p3.")
    .def("rCubicTo", &SkPath::rCubicTo,
        "Adds cubic from last point towards vector (dx1, dy1), then towards "
        "vector (dx2, dy2), to vector (dx3, dy3).")
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
