#include "common.h"
#include <pybind11/stl.h>

py::object IsColorFilterNode(const SkImageFilter& filter) {
    SkColorFilter* colorfilter;
    if (filter.isColorFilterNode(&colorfilter))
        return py::cast(colorfilter);
    return py::none();
}

void initImageFilter(py::module &m) {
py::class_<SkImageFilter, sk_sp<SkImageFilter>, SkFlattenable> imagefilter(
    m, "ImageFilter",
    R"docstring(
    Base class for image filters.

    If one is installed in the paint, then all drawing occurs as usual, but it
    is as if the drawing happened into an offscreen (before the xfermode is
    applied). This offscreen bitmap will then be handed to the imagefilter, who
    in turn creates a new bitmap which is what will finally be drawn to the
    device (using the original xfermode).

    The local space of image filters matches the local space of the drawn
    geometry. For instance if there is rotation on the canvas, the blur will be
    computed along those rotated axes and not in the device space. In order to
    achieve this result, the actual drawing of the geometry may happen in an
    unrotated coordinate system so that the filtered image can be computed more
    easily, and then it will be post transformed to match what would have been
    produced if the geometry were drawn with the total canvas matrix to begin
    with.

    .. rubric:: Subclasses

    .. autosummary::
        :nosignatures:

        ~skia.ArithmeticImageFilter
    )docstring");

py::class_<SkImageFilter::CropRect> croprect(imagefilter, "CropRect");

py::enum_<SkImageFilter::CropRect::CropEdge>(croprect, "CropEdge")
    .value("kHasLeft_CropEdge", SkImageFilter::CropRect::kHasLeft_CropEdge)
    .value("kHasTop_CropEdge", SkImageFilter::CropRect::kHasTop_CropEdge)
    .value("kHasWidth_CropEdge", SkImageFilter::CropRect::kHasWidth_CropEdge)
    .value("kHasHeight_CropEdge", SkImageFilter::CropRect::kHasHeight_CropEdge)
    .value("kHasAll_CropEdge", SkImageFilter::CropRect::kHasAll_CropEdge)
    .export_values();

croprect
    .def(py::init<>())
    .def(py::init<const SkRect&, uint32_t>(),
        py::arg("rect"),
        py::arg("flags") = SkImageFilter::CropRect::kHasAll_CropEdge)
    .def("flags", &SkImageFilter::CropRect::flags)
    .def("rect", &SkImageFilter::CropRect::rect)
    .def("applyTo", &SkImageFilter::CropRect::applyTo,
        R"docstring(
        Apply this cropRect to the imageBounds.

        If a given edge of the cropRect is not set, then the corresponding edge
        from imageBounds will be used. If "embiggen" is true, the crop rect is
        allowed to enlarge the size of the rect, otherwise it may only reduce
        the rect. Filters that can affect transparent black should pass "true",
        while all other filters should pass "false".

        Note: imageBounds is in "device" space, as the output cropped rectangle
        will be, so the matrix is ignored for those. It is only applied to the
        cropRect's bounds.
        )docstring")
    ;

py::enum_<SkImageFilter::MapDirection>(imagefilter, "MapDirection")
    .value("kForward_MapDirection", SkImageFilter::kForward_MapDirection)
    .value("kReverse_MapDirection", SkImageFilter::kReverse_MapDirection)
    .export_values();

imagefilter
    .def("filterBounds", &SkImageFilter::filterBounds,
        R"docstring(
        Map a device-space rect recursively forward or backward through the
        filter DAG.

        :py:attr:`kForward_MapDirection` is used to determine which pixels of
        the destination canvas a source image rect would touch after filtering.
        :py:attr:`kReverse_MapDirection` is used to determine which rect of the
        source image would be required to fill the given rect (typically, clip
        bounds). Used for clipping and temp-buffer allocations, so the result
        need not be exact, but should never be smaller than the real answer. The
        default implementation recursively unions all input bounds, or returns
        the source rect if no inputs.

        In kReverse mode, 'inputRect' is the device-space bounds of the input
        pixels. In kForward mode it should always be null. If 'inputRect' is
        null in kReverse mode the resulting answer may be incorrect.
        )docstring",
        py::arg("src"), py::arg("ctm"), py::arg("direction"),
        py::arg("inputRect") = nullptr)
    .def("isColorFilterNode", &IsColorFilterNode,
        R"docstring(
        Returns :py:class:`ColorFilter` if it can. Otherwise returns None.
        )docstring",
        py::return_value_policy::reference)
    .def("asColorFilter", &IsColorFilterNode,
        py::return_value_policy::reference)
    .def("asAColorFilter", &IsColorFilterNode,
        R"docstring(
        Returns colorfilter if this imagefilter can be completely replaced by
        the returned colorfilter. Otherwise None.

        i.e. the two effects will affect drawing in the same way.
        )docstring",
        py::return_value_policy::reference)
    .def("getInput", &SkImageFilter::getInput,
        R"docstring(
        Returns the input filter at a given index, or NULL if no input is
        connected.

        The indices used are filter-specific.
        )docstring",
        py::arg("i"))
    .def("computeFastBounds", &SkImageFilter::computeFastBounds,
        py::arg("bounds"))
    .def("canComputeFastBounds", &SkImageFilter::canComputeFastBounds)
    .def("makeWithLocalMatrix", &SkImageFilter::makeWithLocalMatrix,
        R"docstring(
        If this filter can be represented by another filter + a localMatrix,
        return that filter, else return null.
        )docstring")
    .def_static("MakeMatrixFilter",
        [] (const SkMatrix& matrix, SkFilterQuality quality,
            const SkImageFilter& input) {
            auto data = input.serialize();
            auto clone = SkImageFilter::Deserialize(data->data(), data->size());
            return SkImageFilter::MakeMatrixFilter(matrix, quality, clone);
        },
        R"docstring(
        Return an imagefilter which transforms its input by the given matrix.

        DEPRECATED: Use :py:meth:`ImageFilters::MatrixTransform`
        )docstring",
        py::arg("matrix"), py::arg("quality"), py::arg("input"))
    .def_static("Deserialize",
        [] (py::buffer b) {
            auto info = b.request();
            return SkImageFilter::Deserialize(
                info.ptr, info.shape[0] * info.strides[0]);
        },
        py::arg("data"))
    ;

py::class_<SkArithmeticImageFilter>(m, "ArithmeticImageFilter")
    .def_static("Make",
        [] (float k1, float k2, float k3, float k4, bool enforcePMColor,
            const SkImageFilter& background, const SkImageFilter& foreground,
            const SkImageFilter::CropRect *cropRect) {
            auto background_data = background.serialize();
            auto foreground_data = foreground.serialize();
            auto background_ = SkImageFilter::Deserialize(
                background_data->data(), background_data->size());
            auto foreground_ = SkImageFilter::Deserialize(
                foreground_data->data(), foreground_data->size());
            return SkArithmeticImageFilter::Make(
                k1, k2, k3, k4, enforcePMColor, background_, foreground_,
                cropRect);
        },
        py::arg("k1"), py::arg("k2"), py::arg("k3"), py::arg("k4"),
        py::arg("enforcePMColor"), py::arg("background"), py::arg("foreground"),
        py::arg("cropRect"))
    ;

py::class_<SkBlurImageFilter> blurimagefilter(m, "BlurImageFilter");

py::enum_<SkBlurImageFilter::TileMode>(blurimagefilter, "TileMode")
    .value("kClamp_TileMode", SkBlurImageFilter::kClamp_TileMode,
        R"docstring(
        Clamp to the image's edge pixels. This re-weights the filter so samples
        outside have no effect
        )docstring")
    .value("kRepeat_TileMode", SkBlurImageFilter::kRepeat_TileMode,
        R"docstring(
        Wrap around to the image's opposite edge.
        )docstring")
    .value("kClampToBlack_TileMode", SkBlurImageFilter::kClampToBlack_TileMode,
        R"docstring(
        Fill with transparent black.
        )docstring")
    .value("kLast_TileMode", SkBlurImageFilter::kLast_TileMode)
    .value("kMax_TileMode", SkBlurImageFilter::kMax_TileMode)
    .export_values();

blurimagefilter
    .def_static("Make",
        [] (SkScalar sigmaX, SkScalar sigmaY, const SkImageFilter& input,
            const SkImageFilter::CropRect* cropRect,
            SkBlurImageFilter::TileMode tileMode) {
            auto data = input.serialize();
            auto input_ = SkImageFilter::Deserialize(
                data->data(), data->size());
            return SkBlurImageFilter::Make(
                sigmaX, sigmaY, input_, cropRect, tileMode);
        },
        py::arg("sigmaX"), py::arg("sigmaY"), py::arg("input"),
        py::arg("cropRect") = nullptr,
        py::arg("tileMode") = SkBlurImageFilter::kClampToBlack_TileMode)
    ;

}
