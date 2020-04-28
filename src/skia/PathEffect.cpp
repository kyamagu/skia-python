#include "common.h"
#include <pybind11/stl.h>
#include "include/effects/SkOpPathEffect.h"
#include "include/effects/SkTrimPathEffect.h"


const int SkStrokeRec::kStyleCount;


void initPathEffect(py::module &m) {
// StrokeRec
py::class_<SkStrokeRec> strokerec(m, "StrokeRec",
    R"docstring(

    .. rubric:: Classes

    .. autosummary::
        :nosignatures:

        ~skia.StrokeRec.InitStyle
        ~skia.StrokeRec.Style
    )docstring");

py::enum_<SkStrokeRec::InitStyle>(strokerec, "InitStyle")
    .value("kHairline_InitStyle", SkStrokeRec::InitStyle::kHairline_InitStyle)
    .value("kFill_InitStyle", SkStrokeRec::InitStyle::kFill_InitStyle)
    .export_values();

py::enum_<SkStrokeRec::Style>(strokerec, "Style")
    .value("kHairline_Style", SkStrokeRec::Style::kHairline_Style)
    .value("kFill_Style", SkStrokeRec::Style::kFill_Style)
    .value("kStroke_Style", SkStrokeRec::Style::kStroke_Style)
    .value("kStrokeAndFill_Style", SkStrokeRec::Style::kStrokeAndFill_Style)
    .export_values();

strokerec
    .def(py::init<SkStrokeRec::InitStyle>(), py::arg("style"))
    .def(py::init<const SkPaint&, SkPaint::Style, SkScalar>(),
        py::arg("paint"), py::arg("style"), py::arg("resScale") = 1)
    .def(py::init<const SkPaint&, SkScalar>(),
        py::arg("paint"), py::arg("resScale") = 1)
    .def("getStyle", &SkStrokeRec::getStyle)
    .def("getWidth", &SkStrokeRec::getWidth)
    .def("getMiter", &SkStrokeRec::getMiter)
    .def("getCap", &SkStrokeRec::getCap)
    .def("getJoin", &SkStrokeRec::getJoin)
    .def("isHairlineStyle", &SkStrokeRec::isHairlineStyle)
    .def("isFillStyle", &SkStrokeRec::isFillStyle)
    .def("setFillStyle", &SkStrokeRec::setFillStyle)
    .def("setHairlineStyle", &SkStrokeRec::setHairlineStyle)
    .def("setStrokeStyle", &SkStrokeRec::setStrokeStyle,
        R"docstring(
        Specify the strokewidth, and optionally if you want stroke + fill.

        Note, if width==0, then this request is taken to mean:
        strokeAndFill==true -> new style will be Fill strokeAndFill==false ->
        new style will be Hairline
        )docstring",
        py::arg("width"), py::arg("strokeAndFill") = false)
    .def("setStrokeParams", &SkStrokeRec::setStrokeParams,
        py::arg("cap"), py::arg("join"), py::arg("miterLimit"))
    .def("getResScale", &SkStrokeRec::getResScale)
    .def("setResScale", &SkStrokeRec::setResScale, py::arg("rs"))
    .def("needToApply", &SkStrokeRec::needToApply,
        R"docstring(
        Returns true if this specifes any thick stroking, i.e.

        :py:meth:`applyToPath` will return true.
        )docstring")
    .def("applyToPath", &SkStrokeRec::applyToPath,
        R"docstring(
        Apply these stroke parameters to the src path, returning the result in
        dst.

        If there was no change (i.e. style == hairline or fill) this returns
        false and dst is unchanged. Otherwise returns true and the result is
        stored in dst.

        src and dst may be the same path.
        )docstring",
        py::arg("dst"), py::arg("src"))
    .def("applyToPaint", &SkStrokeRec::applyToPaint,
        R"docstring(
        Apply these stroke parameters to a paint.
        )docstring",
        py::arg("paint"))
    .def("getInflationRadius", &SkStrokeRec::getInflationRadius,
        R"docstring(
        Gives a conservative value for the outset that should applied to a
        geometries bounds to account for any inflation due to applying this
        strokeRec to the geometry.
        )docstring")
    .def("hasEqualEffect", &SkStrokeRec::hasEqualEffect,
        R"docstring(
        Compare if two SkStrokeRecs have an equal effect on a path.

        Equal SkStrokeRecs produce equal paths. Equality of produced paths does
        not take the ResScale parameter into account.
        )docstring",
        py::arg("other"))
    .def_static("GetInflationRadius",
        py::overload_cast<const SkPaint&, SkPaint::Style>(
            &SkStrokeRec::GetInflationRadius),
        R"docstring(
        Equivalent to: :py:class:`StrokeRec` rec(paint, style);
        rec.getInflationRadius(); This does not account for other effects on the
        paint (i.e. path effect).
        )docstring",
        py::arg("paint"), py::arg("style"))
    .def_static("GetInflationRadius",
        py::overload_cast<SkPaint::Join, SkScalar, SkPaint::Cap, SkScalar>(
            &SkStrokeRec::GetInflationRadius),
        py::arg("join"), py::arg("miterLimit"), py::arg("cap"),
        py::arg("strokeWidth"))
    .def_readonly_static("kStyleCount", &SkStrokeRec::kStyleCount)
    ;


// PathEffect
py::class_<SkPathEffect, sk_sp<SkPathEffect>, SkFlattenable>
    patheffect(m, "PathEffect",
    R"docstring(
    :py:class:`PathEffect` is the base class for objects in the
    :py:class:`Paint` that affect the geometry of a drawing primitive before it
    is transformed by the canvas' matrix and drawn.

    Dashing is implemented as a subclass of :py:class:`PathEffect`.

    .. rubric:: Classes

    .. autosummary::
        :nosignatures:

        ~PathEffect.DashInfo
        ~PathEffect.PointData

    .. rubric:: Subclasses

    .. autosummary::
        :nosignatures:

        ~skia.DiscretePathEffect
        ~skia.DashPathEffect
        ~skia.CornerPathEffect
        ~skia.Path1DPathEffect
        ~skia.Path2DPathEffect
        ~skia.MergePathEffect
        ~skia.MatrixPathEffect
        ~skia.StrokePathEffect
        ~skia.TrimPathEffect
    )docstring");

py::class_<SkPathEffect::DashInfo>(patheffect, "DashInfo")
    .def(py::init<>())
    .def_property_readonly("fIntervals",
        [] (const SkPathEffect::DashInfo& info) {
            return std::vector<SkScalar>(
                info.fIntervals, info.fIntervals + info.fCount);
        },
        R"docstring(
        Length of on/off intervals for dashed lines.
        )docstring")
    .def_readonly("fCount", &SkPathEffect::DashInfo::fCount,
        R"docstring(
        Number of intervals in the dash. Should be even number.
        )docstring")
    .def_readonly("fPhase", &SkPathEffect::DashInfo::fPhase,
        R"docstring(
        Offset into the dashed interval pattern.
        )docstring")
    ;

py::class_<SkPathEffect::PointData> pointdata(patheffect, "PointData",
    R"docstring(
    :py:class:`PointData` aggregates all the information needed to draw the
    point primitives returned by an asPoints call.

    .. rubric:: Classes

    .. autosummary::
        :nosignatures:

        ~skia.PathEffect.PointData.PointFlags
    )docstring");

py::enum_<SkPathEffect::PointData::PointFlags>(
    pointdata, "PointFlags", py::arithmetic())
    .value("kCircles_PointFlag",
        SkPathEffect::PointData::PointFlags::kCircles_PointFlag)
    .value("kUsePath_PointFlag",
        SkPathEffect::PointData::PointFlags::kUsePath_PointFlag)
    .value("kUseClip_PointFlag",
        SkPathEffect::PointData::PointFlags::kUseClip_PointFlag)
    .export_values();

pointdata
    .def(py::init<>())
    .def_readonly("fFlags", &SkPathEffect::PointData::fFlags)
    .def_property_readonly("fPoints",
        [] (const SkPathEffect::PointData& data) {
            return std::vector<SkPoint>(
                data.fPoints, data.fPoints + data.fNumPoints);
        })
    .def_readonly("fNumPoints", &SkPathEffect::PointData::fNumPoints)
    .def_readonly("fSize", &SkPathEffect::PointData::fSize)
    .def_readonly("fClipRect", &SkPathEffect::PointData::fClipRect)
    .def_readonly("fPath", &SkPathEffect::PointData::fPath)
    .def_readonly("fFirst", &SkPathEffect::PointData::fFirst)
    .def_readonly("fLast", &SkPathEffect::PointData::fLast)
    ;

py::enum_<SkPathEffect::DashType>(patheffect, "DashType",
    R"docstring(
    If the :py:class:`PathEffect` can be represented as a dash pattern, asADash
    will return kDash_DashType and None otherwise.

    If a non NULL info is passed in, the various
    :py:class:`~PathEffect.DashInfo` will be filled in if the
    :py:class:`PathEffect` can be a dash pattern. If passed in info has an
    fCount equal or greater to that of the effect, it will memcpy the values of
    the dash intervals into the info. Thus the general approach will be call
    asADash once with default info to get DashType and fCount. If effect can be
    represented as a dash pattern, allocate space for the intervals in info,
    then call asADash again with the same info and the intervals will get copied
    in.
    )docstring")
    .value("kNone_DashType", SkPathEffect::DashType::kNone_DashType,
        "ignores the info parameter")
    .value("kDash_DashType", SkPathEffect::DashType::kDash_DashType,
        "fills in all of the info parameter")
    .export_values();

patheffect
    .def("filterPath", &SkPathEffect::filterPath,
        R"docstring(
        Given a src path (input) and a stroke-rec (input and output), apply this
        effect to the src path, returning the new path in dst, and return true.

        If this effect cannot be applied, return false and ignore dst and
        stroke-rec.

        The stroke-rec specifies the initial request for stroking (if any). The
        effect can treat this as input only, or it can choose to change the rec
        as well. For example, the effect can decide to change the stroke's width
        or join, or the effect can change the rec from stroke to fill (or fill
        to stroke) in addition to returning a new (dst) path.

        If this method returns true, the caller will apply (as needed) the
        resulting stroke-rec to dst and then draw.
        )docstring",
        py::arg("dst"), py::arg("dst"), py::arg("stroke_rec"), py::arg("cullR"))
    .def("computeFastBounds", &SkPathEffect::computeFastBounds,
        R"docstring(
        Compute a conservative bounds for its effect, given the src bounds.

        The baseline implementation just assigns src to dst.
        )docstring",
        py::arg("dst"), py::arg("src"))
    .def("asPoints", &SkPathEffect::asPoints,
        R"docstring(
        Does applying this path effect to 'src' yield a set of points? If so,
        optionally return the points in 'results'.
        )docstring",
        py::arg("results"), py::arg("src"), py::arg("stroke_rec"),
        py::arg("matrix"), py::arg("cullR"))
    .def("asADash", &SkPathEffect::asADash, py::arg("info"))
    .def_static("MakeSum",
        [] (const SkPathEffect& first, const SkPathEffect& second) {
            auto first_ = first.serialize();
            auto second_ = second.serialize();
            return SkPathEffect::MakeSum(
                SkPathEffect::Deserialize(first_->data(), first_->size()),
                SkPathEffect::Deserialize(second_->data(), second_->size()));
        },
        R"docstring(
        Returns a patheffect that apples each effect (first and second) to the
        original path, and returns a path with the sum of these.

        result = first(path) + second(path)
        )docstring")
    .def_static("MakeCompose",
        [] (const SkPathEffect& outer, const SkPathEffect& inner) {
            auto outer_ = outer.serialize();
            auto inner_ = inner.serialize();
            return SkPathEffect::MakeCompose(
                SkPathEffect::Deserialize(outer_->data(), outer_->size()),
                SkPathEffect::Deserialize(inner_->data(), inner_->size()));
        },
        R"docstring(
        Returns a patheffect that applies the inner effect to the path, and then
        applies the outer effect to the result of the inner's.

        result = outer(inner(path))
        )docstring")
    .def_static("RegisterFlattenables", &SkPathEffect::RegisterFlattenables)
    .def_static("GetFlattenableType", &SkPathEffect::GetFlattenableType)
    .def_static("Deserialize",
        [] (py::buffer b) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.shape[0] * info.strides[0] : 0;
            return SkPathEffect::Deserialize(info.ptr, size);
        },
        py::arg("data"))
    ;

py::class_<SkDiscretePathEffect, SkPathEffect, sk_sp<SkDiscretePathEffect>>(
    m, "DiscretePathEffect")
    .def_static("Make", &SkDiscretePathEffect::Make,
        R"docstring(
        Break the path into segments of segLength length, and randomly move the
        endpoints away from the original path by a maximum of deviation.

        Note: works on filled or framed paths

        :param segLength: Segment length
        :param dev: Deviation
        :param seedAssist: This is a caller-supplied seedAssist that modifies
            the seed value that is used to randomize the path segments'
            endpoints. If not supplied it defaults to 0, in which case filtering
            a path multiple times will result in the same set of segments (this
            is useful for testing). If a caller does not want this behaviour
            they can pass in a different seedAssist to get a different set of
            path segments.
        )docstring",
        py::arg("segLength"), py::arg("dev"), py::arg("seedAssist") = 0)
    ;

py::class_<SkDashPathEffect>(m, "DashPathEffect")
    .def_static("Make",
        [] (const std::vector<SkScalar>& intervals, SkScalar phase) {
            return SkDashPathEffect::Make(
                &intervals[0], intervals.size(), phase);
        },
        R"docstring(
        For example: if intervals[] = {10, 20}, count = 2, and phase = 25, this
        will set up a dashed path like so: 5 pixels off 10 pixels on 20 pixels
        off 10 pixels on 20 pixels off ... A phase of -5, 25, 55, 85, etc. would
        all result in the same path, because the sum of all the intervals is 30.

        Note: only affects stroked paths.

        :param List[skia.Point] intervals: array containing an even number of
            entries (>=2), with the even indices specifying the length of "on"
            intervals, and the odd indices specifying the length of "off"
            intervals.
        :param float phase: offset into the intervals array (mod the sum of all
            of the intervals).
        )docstring",
        py::arg("intervals"), py::arg("phase"))
    ;

py::class_<SkCornerPathEffect, SkPathEffect, sk_sp<SkCornerPathEffect>>(
    m, "CornerPathEffect",
    R"docstring(
    :py:class:`CornerPathEffect` is a subclass of :py:class:`PathEffect` that
    can turn sharp corners into various treatments (e.g. rounded corners)
    )docstring")
    .def_static("Make", &SkCornerPathEffect::Make,
        R"docstring(
        radius must be > 0 to have an effect.

        It specifies the distance from each corner that should be "rounded".
        )docstring",
        py::arg("radius"))
    ;

py::class_<SkPath1DPathEffect, SkPathEffect, sk_sp<SkPath1DPathEffect>>
    path1dpatheffect(m, "Path1DPathEffect",
        R"docstring(
        .. rubric:: Classes

        .. autosummary::
            :nosignatures:

            ~Path1DPathEffect.Style
        )docstring");

py::enum_<SkPath1DPathEffect::Style>(path1dpatheffect, "Style")
    .value("kTranslate_Style", SkPath1DPathEffect::Style::kTranslate_Style)
    .value("kRotate_Style", SkPath1DPathEffect::Style::kRotate_Style)
    .value("kMorph_Style", SkPath1DPathEffect::Style::kMorph_Style)
    .value("kLastEnum_Style", SkPath1DPathEffect::Style::kLastEnum_Style)
    .export_values();

path1dpatheffect
    .def_static("Make",
        &SkPath1DPathEffect::Make,
        R"docstring(
        Dash by replicating the specified path.

        :param skia.Path path: The path to replicate (dash)
        :param float advance: The space between instances of path
        :param float phase: distance (mod advance) along path for its initial
            position
        :param skia.Path1DPathEffect.Style style: how to transform path at each
            point (based on the current position and tangent)
        )docstring",
        py::arg("path"), py::arg("advance"), py::arg("phase"), py::arg("style"))
    ;

py::class_<SkPath2DPathEffect, SkPathEffect, sk_sp<SkPath2DPathEffect>>(
    m, "Path2DPathEffect")
    .def_static("Make", &SkPath2DPathEffect::Make,
        R"docstring(
        Stamp the specified path to fill the shape, using the matrix to define
        the latice.
        )docstring",
        py::arg("matrix"), py::arg("path"))
    ;

py::class_<SkMergePathEffect>(m, "MergePathEffect")
    .def_static("Make",
        [] (const SkPathEffect& one, const SkPathEffect& two, SkPathOp op) {
            auto one_ = one.serialize();
            auto two_ = two.serialize();
            return SkMergePathEffect::Make(
                SkPathEffect::Deserialize(one_->data(), one_->size()),
                SkPathEffect::Deserialize(two_->data(), two_->size()),
                op);
        },
        py::arg("one"), py::arg("two"), py::arg("op"))
    ;

py::class_<SkMatrixPathEffect>(m, "MatrixPathEffect")
    .def_static("Make", &SkMatrixPathEffect::Make, py::arg("matrix"))
    .def_static("MakeTranslate", &SkMatrixPathEffect::MakeTranslate,
        py::arg("dx"), py::arg("dy"))
    ;

py::class_<SkStrokePathEffect>(m, "StrokePathEffect")
    .def_static("Make", &SkStrokePathEffect::Make,
        py::arg("width"), py::arg("join"), py::arg("cap"), py::arg("miter") = 4)
    ;

py::class_<SkTrimPathEffect> trimpatheffect(m, "TrimPathEffect",
    R"docstring(
    .. rubric:: Classes

    .. autosummary::
        :nosignatures:

        ~TrimPathEffect.Mode
    )docstring");

py::enum_<SkTrimPathEffect::Mode>(trimpatheffect, "Mode")
    .value("kNormal", SkTrimPathEffect::Mode::kNormal)
    .value("kInverted", SkTrimPathEffect::Mode::kInverted)
    .export_values();

trimpatheffect
    .def_static("Make", &SkTrimPathEffect::Make,
        R"docstring(
        Take start and stop "t" values (values between 0...1), and return a path
        that is that subset of the original path.

        e.g. Make(0.5, 1.0) –> return the 2nd half of the path Make(0.33333,
        0.66667) –> return the middle third of the path

        The trim values apply to the entire path, so if it contains several
        contours, all of them are including in the calculation.

        startT and stopT must be 0..1 inclusive. If they are outside of that
        interval, they will be pinned to the nearest legal value. If either is
        NaN, null will be returned.

        Note: for Mode::kNormal, this will return one (logical) segment (even if
        it is spread across multiple contours). For Mode::kInverted, this will
        return 2 logical segments: stopT..1 and 0...startT, in this order.
        )docstring",
        py::arg("startT"), py::arg("stopT"),
        py::arg("mode") = SkTrimPathEffect::Mode::kNormal)
    ;
}
