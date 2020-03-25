#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initPath(py::module &m) {
// PathTypes
py::enum_<SkPathFillType>(m, "PathFillType");
py::enum_<SkPathConvexityType>(m, "PathConvexityType");
py::enum_<SkPathDirection>(m, "PathDirection");
py::enum_<SkPathSegmentMask>(m, "PathSegmentMask");
py::enum_<SkPathVerb>(m, "PathVerb");
m.def("SkPathFillType_IsEvenOdd", &SkPathFillType_IsEvenOdd);
m.def("SkPathFillType_IsInverse", &SkPathFillType_IsInverse);
m.def("SkPathFillType_ConvertToNonInverse",
    &SkPathFillType_ConvertToNonInverse);
// Path
py::class_<SkPath> path(m, "Path");
py::enum_<SkPath::ArcSize>(path, "ArcSize");
py::enum_<SkPath::AddPathMode>(path, "AddPathMode");
py::enum_<SkPath::SegmentMask>(path, "SegmentMask");
py::enum_<SkPath::Verb>(path, "Verb");
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
    ;
}
