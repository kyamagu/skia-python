#include "common.h"
#include <include/effects/SkImageFilters.h>
#include <include/core/SkPoint3.h>
#include <pybind11/stl.h>

#define CLONE(input) \
(input) ? CloneFlattenable(*input) : sk_sp<SkImageFilter>(nullptr)

namespace {

py::object IsColorFilterNode(const SkImageFilter& filter) {
    SkColorFilter* colorfilter;
    if (filter.isColorFilterNode(&colorfilter))
        return py::cast(sk_sp<SkColorFilter>(colorfilter));
    return py::none();
}

}  // namespace

/*
const int SkDropShadowImageFilter::kShadowModeCount;
*/

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

    See :py:class:`ImageFilters` for available filters.

    .. rubric:: Subclasses

    .. autosummary::
        :nosignatures:

        ~skia.ArithmeticImageFilter
        ~skia.BlurImageFilter
        ~skia.ColorFilterImageFilter
        ~skia.DilateImageFilter
        ~skia.DisplacementMapEffect
        ~skia.DropShadowImageFilter
        ~skia.ErodeImageFilter
        ~skia.ImageFilters
        ~skia.LightingImageFilter
        ~skia.MagnifierImageFilter
        ~skia.OffsetImageFilter
        ~skia.PaintImageFilter
        ~skia.TileImageFilter
        ~skia.XfermodeImageFilter
    )docstring");

py::class_<SkImageFilters::CropRect> croprect(imagefilter, "CropRect");

/*
py::enum_<SkImageFilters::CropRect::CropEdge>(croprect, "CropEdge")
    .value("kHasLeft_CropEdge", SkImageFilters::CropRect::kHasLeft_CropEdge)
    .value("kHasTop_CropEdge", SkImageFilters::CropRect::kHasTop_CropEdge)
    .value("kHasWidth_CropEdge", SkImageFilters::CropRect::kHasWidth_CropEdge)
    .value("kHasHeight_CropEdge", SkImageFilters::CropRect::kHasHeight_CropEdge)
    .value("kHasAll_CropEdge", SkImageFilters::CropRect::kHasAll_CropEdge)
    .export_values();
*/

croprect
    .def(py::init<>())
    .def(py::init<const SkRect&>(),
        py::arg("rect"))
/*
        py::arg("flags") = SkImageFilters::CropRect::kHasAll_CropEdge)
    .def("flags", &SkImageFilters::CropRect::flags)
    .def("rect", &SkImageFilters::CropRect::rect)
    .def("applyTo",
        [] (const SkImageFilters::CropRect& cropRect, const SkIRect& imageBounds,
            const SkMatrix& matrix, bool embiggen) {
            SkIRect cropped;
            cropRect.applyTo(imageBounds, matrix, embiggen, &cropped);
            return cropped;
        },
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
        )docstring",
        py::arg("imageBounds"), py::arg("matrix"), py::arg("embiggen"))
*/
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
        )docstring")
    .def("asColorFilter", &IsColorFilterNode)
    .def("asAColorFilter", &IsColorFilterNode,
        R"docstring(
        Returns colorfilter if this imagefilter can be completely replaced by
        the returned colorfilter. Otherwise None.

        i.e. the two effects will affect drawing in the same way.
        )docstring")
    .def("countInputs", &SkImageFilter::countInputs,
        R"docstring(
        Returns the number of inputs this filter will accept (some inputs can be
        NULL).
        )docstring")
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
        )docstring",
        py::arg("matrix"))
/*
    .def_static("MakeMatrixFilter",
        [] (const SkMatrix& matrix, SkFilterQuality quality,
            const SkImageFilter* input) {
            return SkImageFilter::MakeMatrixFilter(
                matrix, quality, CLONE(input));
        },
        R"docstring(
        Return an imagefilter which transforms its input by the given matrix.

        DEPRECATED: Use :py:meth:`ImageFilters::MatrixTransform`
        )docstring",
        py::arg("matrix"), py::arg("quality"), py::arg("input") = nullptr)
*/
    .def_static("Deserialize",
        [] (py::buffer b) {
            auto info = b.request();
            return SkImageFilter::Deserialize(
                info.ptr, info.shape[0] * info.strides[0]);
        },
        py::arg("data"))
    ;

/*
py::class_<SkAlphaThresholdFilter>(m, "AlphaThresholdFilter")
    .def_static("Make",
        [] (const SkRegion& region, SkScalar innerMin, SkScalar outerMax,
            const SkImageFilter* input,
            const SkImageFilters::CropRect *cropRect) {
            return SkAlphaThresholdFilter::Make(
                region, innerMin, outerMax, CLONE(input), cropRect);
        },
        R"docstring(
        Creates an image filter that samples a region.

        If the sample is inside the region the alpha of the image is boosted up
        to a threshold value. If it is outside the region then the alpha is
        decreased to the threshold value. The 0,0 point of the region
        corresponds to the upper left corner of the source image.
        )docstring",
        py::arg("region"), py::arg("innerMin"), py::arg("outerMax"),
        py::arg("input") = nullptr, py::arg("cropRect") = nullptr)
    ;

py::class_<SkArithmeticImageFilter>(m, "ArithmeticImageFilter")
    .def_static("Make",
        [] (float k1, float k2, float k3, float k4, bool enforcePMColor,
            const SkImageFilter& background, const SkImageFilter* foreground,
            const SkImageFilters::CropRect *cropRect) {
            return SkArithmeticImageFilter::Make(
                k1, k2, k3, k4, enforcePMColor,
                CloneFlattenable(background),
                CLONE(foreground), cropRect);
        },
        py::arg("k1"), py::arg("k2"), py::arg("k3"), py::arg("k4"),
        py::arg("enforcePMColor"), py::arg("background"),
        py::arg("foreground") = nullptr, py::arg("cropRect") = nullptr)
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
        [] (SkScalar sigmaX, SkScalar sigmaY, const SkImageFilter* input,
            const SkImageFilters::CropRect* cropRect,
            SkBlurImageFilter::TileMode tileMode) {
            return SkBlurImageFilter::Make(
                sigmaX, sigmaY, CLONE(input), cropRect, tileMode);
        },
        py::arg("sigmaX"), py::arg("sigmaY"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr,
        py::arg("tileMode") = SkBlurImageFilter::kClampToBlack_TileMode)
    ;

py::class_<SkColorFilterImageFilter>(m, "ColorFilterImageFilter")
    .def_static("Make",
        [] (const SkColorFilter& cf, const SkImageFilter* input,
            const SkImageFilters::CropRect *cropRect){
            return SkColorFilterImageFilter::Make(
                CloneFlattenable(cf), CLONE(input), cropRect);
        },
        py::arg("cf"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    ;
*/

// py::class_<SkComposeImageFilter>(m, "ComposeImageFilter")
//     .def_static("Make",
//         [] (const SkImageFilter& outer, const SkImageFilter& inner){
//             return SkComposeImageFilter::Make(
//                 CloneFlattenable(outer), CloneFlattenable(inner));
//         },
//         py::arg("outer"), py::arg("inner"))
//     ;

/*
py::class_<SkDisplacementMapEffect> displacementmapeffect(
    m, "DisplacementMapEffect");

py::enum_<SkDisplacementMapEffect::ChannelSelectorType>(
    displacementmapeffect, "ChannelSelectorType")
    .value("kUnknown_ChannelSelectorType",
        SkDisplacementMapEffect::kUnknown_ChannelSelectorType)
    .value("kR_ChannelSelectorType",
        SkDisplacementMapEffect::kR_ChannelSelectorType)
    .value("kG_ChannelSelectorType",
        SkDisplacementMapEffect::kG_ChannelSelectorType)
    .value("kB_ChannelSelectorType",
        SkDisplacementMapEffect::kB_ChannelSelectorType)
    .value("kA_ChannelSelectorType",
        SkDisplacementMapEffect::kA_ChannelSelectorType)
    .value("kLast_ChannelSelectorType",
        SkDisplacementMapEffect::kLast_ChannelSelectorType)
    .export_values();

displacementmapeffect
    .def_static("Make",
        [] (SkDisplacementMapEffect::ChannelSelectorType xChannelSelector,
            SkDisplacementMapEffect::ChannelSelectorType yChannelSelector,
            SkScalar scale,
            const SkImageFilter& displacement,
            const SkImageFilter& color,
            const SkImageFilters::CropRect* cropRect) {
            return SkDisplacementMapEffect::Make(
                xChannelSelector, yChannelSelector, scale,
                CloneFlattenable(displacement),
                CloneFlattenable(color), cropRect);
        },
        py::arg("xChannelSelector"), py::arg("yChannelSelector"),
        py::arg("scale"), py::arg("displacement"), py::arg("color"),
        py::arg("cropRect") = nullptr)
    .def_static("Make",
        [] (SkColorChannel xChannelSelector,
            SkColorChannel yChannelSelector,
            SkScalar scale,
            const SkImageFilter& displacement,
            const SkImageFilter& color,
            const SkImageFilters::CropRect* cropRect) {
            return SkDisplacementMapEffect::Make(
                xChannelSelector, yChannelSelector, scale,
                CloneFlattenable(displacement),
                CloneFlattenable(color), cropRect);
        },
        py::arg("xChannelSelector"), py::arg("yChannelSelector"),
        py::arg("scale"), py::arg("displacement"), py::arg("color"),
        py::arg("cropRect") = nullptr)
    ;

py::class_<SkDropShadowImageFilter> dropshadowimagefilter(
    m, "DropShadowImageFilter");

py::enum_<SkDropShadowImageFilter::ShadowMode>(
    dropshadowimagefilter, "ShadowMode")
    .value("kDrawShadowAndForeground_ShadowMode",
        SkDropShadowImageFilter::kDrawShadowAndForeground_ShadowMode)
    .value("kDrawShadowOnly_ShadowMode",
        SkDropShadowImageFilter::kDrawShadowOnly_ShadowMode)
    .value("kLast_ShadowMode",
        SkDropShadowImageFilter::kLast_ShadowMode)
    .export_values();

dropshadowimagefilter
    .def_static("Make",
        [] (SkScalar dx, SkScalar dy, SkScalar sigmaX, SkScalar sigmaY,
            SkColor color, SkDropShadowImageFilter::ShadowMode shadowMode,
            const SkImageFilter* input,
            const SkImageFilters::CropRect* cropRect) {
            return SkDropShadowImageFilter::Make(
                dx, dy, sigmaX, sigmaY, color, shadowMode,
                CLONE(input), cropRect);
        },
        py::arg("dx"), py::arg("dy"), py::arg("sigmaX"), py::arg("sigmaY"),
        py::arg("color"), py::arg("shadowMode"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_readonly_static("kShadowModeCount",
        &SkDropShadowImageFilter::kShadowModeCount)
    ;

py::class_<SkDilateImageFilter>(m, "DilateImageFilter")
    .def_static("Make",
        [] (SkScalar radiusX, SkScalar radiusY, const SkImageFilter* input,
            const SkImageFilters::CropRect *cropRect) {
            return SkDilateImageFilter::Make(
                radiusX, radiusY, CLONE(input), cropRect);
        },
        py::arg("radiusX"), py::arg("radiusY"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    ;

py::class_<SkErodeImageFilter>(m, "ErodeImageFilter")
    .def_static("Make",
        [] (SkScalar radiusX, SkScalar radiusY, const SkImageFilter* input,
            const SkImageFilters::CropRect *cropRect) {
            return SkErodeImageFilter::Make(
                radiusX, radiusY, CLONE(input), cropRect);
        },
        py::arg("radiusX"), py::arg("radiusY"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    ;
*/

py::class_<SkImageFilters>(m, "ImageFilters")
/* SkImageFilters::AlphaThreshold removed in m116 */
/*
    .def_static("AlphaThreshold",
        [] (const SkRegion& region, SkScalar innerMin, SkScalar outerMax,
            const SkImageFilter* input, const SkIRect* cropRect) {
            return SkImageFilters::AlphaThreshold(
                region, innerMin, outerMax, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that updates the alpha of the image based on 'region'.

        Pixels inside the region are made more opaque and pixels outside are
        made more transparent.

        Specifically, if a pixel is inside the region, its alpha will be set to
        max(innerMin, pixel's alpha). If a pixel is outside the region, its
        alpha will be updated to min(outerMax, pixel's alpha).

        :param skia.Region region: The geometric region controlling the inner
            and outer alpha thresholds.
        :param float innerMin: The minimum alpha value for pixels inside
            'region'.
        :param float outerMax: The maximum alpha value for pixels outside of
            'region'.
        :param input:  The input filter, or uses the source bitmap if this is
            null.
        :param skia.Rect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("region"), py::arg("innerMin"), py::arg("outerMax"),
        py::arg("input") = nullptr, py::arg("cropRect") = nullptr)
*/
    .def_static("Arithmetic",
        [] (SkScalar k1, SkScalar k2, SkScalar k3, SkScalar k4,
            bool enforcePMColor, const SkImageFilter& background,
            const SkImageFilter* foreground, const SkIRect* cropRect) {
            return SkImageFilters::Arithmetic(
                k1, k2, k3, k4, enforcePMColor, CloneFlattenable(background),
                CLONE(foreground), cropRect);
        },
        R"docstring(
        Create a filter that implements a custom blend mode.

        Each output pixel is the result of combining the corresponding
        background and foreground pixels using the 4 coefficients::

            k1 * foreground * background + k2 * foreground + \
            k3 * background + k4

        :param float k1: The first coefficients used to combine the foreground
            and background.
        :param float k2: The second coefficients used to combine the foreground
            and background.
        :param float k3: The third coefficients used to combine the foreground
            and background.
        :param float k4: The fourth coefficients used to combine the foreground
            and background.
        :param bool enforcePMColor:  If true, the RGB channels will be clamped
            to the calculated alpha.
        :param skia.ImageFilter background:  The background content, using the
            source bitmap when this is null.
        :param skia.ImageFilter foreground:  The foreground content, using the
            source bitmap when this is null.
        :param skia.Rect cropRect: Optional rectangle that crops the inputs and
            output.
        )docstring",
        py::arg("k1"), py::arg("k2"), py::arg("k3"), py::arg("k4"),
        py::arg("enforcePMColor"), py::arg("background"),
        py::arg("foreground") = nullptr, py::arg("cropRect") = nullptr)
    .def_static("Blur",
        [] (SkScalar sigmaX, SkScalar sigmaY, SkTileMode tileMode,
            const SkImageFilter* input, const SkIRect* cropRect) {
            return SkImageFilters::Blur(
                sigmaX, sigmaY, tileMode, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that blurs its input by the separate X and Y sigmas.

        The provided tile mode is used when the blur kernel goes outside the
        input image.

        :param float sigmaX: The Gaussian sigma value for blurring along the X
            axis.
        :param float sigmaY: The Gaussian sigma value for blurring along the Y
            axis.
        :param skia.TileMode tileMode: The tile mode applied at edges.
            TODO (michaelludwig) - kMirror is not supported yet
        :param skia.ImageFilter input: The input filter that is blurred, uses
            source bitmap if this is null.
        :param skia.Rect cropRect:    Optional rectangle that crops the input
            and output.
        )docstring",
        py::arg("sigmaX"), py::arg("sigmaY"),
        py::arg("tileMode") = SkTileMode::kDecal, py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("ColorFilter",
        [] (const SkColorFilter& cf, const SkImageFilter* input,
            const SkIRect* cropRect) {
            return SkImageFilters::ColorFilter(
                CloneFlattenable(cf), CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that applies the color filter to the input filter
        results.

        :param skia.ColorFilter cf: The color filter that transforms the input
            image.
        :param skia.ImageFilter input:  The input filter, or uses the source
            bitmap if this is null.
        :param skia.Rect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("cf"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("Compose",
        [] (const SkImageFilter& outer, const SkImageFilter& inner) {
            return SkImageFilters::Compose(
                CloneFlattenable(outer), CloneFlattenable(inner));
        },
        R"docstring(
        Create a filter that composes 'inner' with 'outer', such that the
        results of 'inner' are treated as the source bitmap passed to 'outer',
        i.e. result = outer(inner(source)).

        :param skia.ImageFilter outer: The outer filter that evaluates the
            results of inner.
        :param skia.ImageFilter inner: The inner filter that produces the input
            to outer.
        )docstring",
        py::arg("outer"), py::arg("inner"))
    .def_static("DisplacementMap",
        [] (SkColorChannel xChannelSelector, SkColorChannel yChannelSelector,
            SkScalar scale, const SkImageFilter& displacement,
            const SkImageFilter& color, const SkIRect* cropRect) {
            return SkImageFilters::DisplacementMap(
                xChannelSelector, yChannelSelector, scale,
                CloneFlattenable(displacement), CloneFlattenable(color),
                cropRect);
        },
        R"docstring(
        Create a filter that moves each pixel in its color input based on an
        (x,y) vector encoded in its displacement input filter.

        Two color components of the displacement image are mapped into a vector
        as scale * (color[xChannel], color[yChannel]), where the channel
        selectors are one of R, G, B, or A.

        :param skia.ColorChannel xChannelSelector: RGBA channel that encodes the
            x displacement per pixel.
        :param skia.ColorChannel yChannelSelector: RGBA channel that encodes the
            y displacement per pixel.
        :param float scale: Scale applied to displacement extracted from image.
        :param skia.ImageFilter displacement: The filter defining the
            displacement image, or null to use source.
        :param skia.ImageFilter color: The filter providing the color pixels to
            be displaced.
        :param skia.Rect cropRect:  Optional rectangle that crops the color
            input and output.
        )docstring",
        py::arg("xChannelSelector"), py::arg("yChannelSelector"),
        py::arg("scale"), py::arg("displacement"), py::arg("color"),
        py::arg("cropRect") = nullptr)
    .def_static("DropShadow",
        [] (SkScalar dx, SkScalar dy, SkScalar sigmaX, SkScalar sigmaY,
            SkColor color, const SkImageFilter* input,
            const SkIRect* cropRect) {
            return SkImageFilters::DropShadow(
                dx, dy, sigmaX, sigmaY, color, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that draws a drop shadow under the input content.

        This filter produces an image that includes the inputs' content.

        :param float dx: The X offset of the shadow.
        :param float dy: The Y offset of the shadow.
        :param float sigmaX: The blur radius for the shadow, along the X axis.
        :param float sigmaY: The blur radius for the shadow, along the Y axis.
        :param int color: The color of the drop shadow.
        :param skia.ImageFilter input: The input filter, or will use the source
            bitmap if this is null.
        :param skia.Rect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("dx"), py::arg("dy"), py::arg("sigmaX"), py::arg("sigmaY"),
        py::arg("color"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("DropShadowOnly",
        [] (SkScalar dx, SkScalar dy, SkScalar sigmaX, SkScalar sigmaY,
            SkColor color, const SkImageFilter* input,
            const SkIRect* cropRect) {
            return SkImageFilters::DropShadowOnly(
                dx, dy, sigmaX, sigmaY, color, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that renders a drop shadow, in exactly the same manner
        as :py:meth:`~ImageFilters.DropShadow`, except that the resulting image
        does not include the input content.

        This allows the shadow and input to be composed by a filter DAG in a
        more flexible manner.

        :param float dx: The X offset of the shadow.
        :param float dy: The Y offset of the shadow.
        :param float sigmaX: The blur radius for the shadow, along the X axis.
        :param float sigmaY: The blur radius for the shadow, along the Y axis.
        :param int color: The color of the drop shadow.
        :param skia.ImageFilter input: The input filter, or will use the source
            bitmap if this is null.
        :param skia.Rect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("dx"), py::arg("dy"), py::arg("sigmaX"), py::arg("sigmaY"),
        py::arg("color"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("Image",
        [] (const SkImage& image, const SkRect& srcRect, const SkRect& dstRect,
            const SkSamplingOptions& options) {
            return SkImageFilters::Image(
                CloneImage(image), srcRect, dstRect, options);
        },
        R"docstring(
        Create a filter that draws the 'srcRect' portion of image into 'dstRect'
        using the given filter quality.

        Similar to :py:meth:`Canvas.drawImageRect`. Returns null if 'image' is
        null.

        :image: The image that is output by the filter, subset by 'srcRect'.
        :srcRect: The source pixels sampled into 'dstRect'
        :dstRect: The local rectangle to draw the image into.
        :filterQuality: The filter quality that is used when sampling the image.
        )docstring",
        py::arg("image"), py::arg("srcRect"), py::arg("dstRect"),
        py::arg("options") = SkSamplingOptions())
    .def_static("Image",
        [] (const SkImage& image, const SkSamplingOptions& options) {
            return SkImageFilters::Image(CloneImage(image), options);
        },
        R"docstring(
        Create a filter that produces the image contents.

        :image: The image that is output by the filter.
        )docstring",
        py::arg("image"), py::arg("options") = SkSamplingOptions())
    .def_static("Magnifier",
            [] (const SkRect& srcRect, SkScalar zoomAmount, SkScalar inset,
            const SkSamplingOptions& sampling,
            const SkImageFilter* input,
            const SkIRect* cropRect) {
            return SkImageFilters::Magnifier(
                srcRect, zoomAmount, inset, sampling, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that mimics a zoom/magnifying lens effect.

        :param skia.Rect srcRect: Source rect.
        :param float inset: Inset.
        :param skia.ImageFilter input: The input filter that is magnified, if
            null the source bitmap is used.
        :param skia.Rect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("srcRect"), py::arg("zoomAmount"), py::arg("inset"), py::arg("sampling"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("MatrixConvolution",
        [] (const SkISize& kernelSize,
            const std::vector<SkScalar> kernel, SkScalar gain,
            SkScalar bias, const SkIPoint& kernelOffset,
            SkTileMode tileMode, bool convolveAlpha, const SkImageFilter* input,
            const SkIRect* cropRect) {
            size_t size = kernelSize.width() * kernelSize.height();
            if (size != kernel.size())
                throw std::runtime_error("kernel must have N*M elements.");
            return SkImageFilters::MatrixConvolution(
                kernelSize, &kernel[0], gain, bias, kernelOffset, tileMode,
                convolveAlpha, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that applies an NxM image processing kernel to the input
        image.

        This can be used to produce effects such as sharpening, blurring, edge
        detection, etc.

        :param skia.ISize kernelSize: The kernel size in pixels, in each
            dimension (N by M).
        :param List[float] kernel: The image processing kernel. Must contain
            N * M elements, in row order.
        :param float gain:   A scale factor applied to each pixel after
            convolution. This can be used to normalize the kernel, if it does
            not already sum to 1.
        :param float bias: A bias factor added to each pixel after convolution.
        :param skia.IPoint kernelOffset: An offset applied to each pixel
            coordinate before convolution. This can be used to center the kernel
            over the image (e.g., a 3x3 kernel should have an offset of {1, 1}).
        :param skia.TileMode tileMode: How accesses outside the image are
            treated. TODO (michaelludwig) - kMirror is not supported yet
        :param bool convolveAlpha:  If true, all channels are convolved. If
            false, only the RGB channels are convolved, and alpha is copied from
            the source image.
        :param skia.ImageFilter input:  The input image filter, if null the
            source bitmap is used instead.
        :param skia.Rect cropRect:   Optional rectangle to which the output
            processing will be limited.
        )docstring",
        py::arg("kernelSize"), py::arg("kernel"), py::arg("gain"),
        py::arg("bias"), py::arg("kernelOffset"), py::arg("tileMode"),
        py::arg("convolveAlpha"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("MatrixTransform",
        [] (const SkMatrix& matrix, const SkSamplingOptions& sampling,
            const SkImageFilter* input) {
            return SkImageFilters::MatrixTransform(
                matrix, sampling, CLONE(input));
        },
        R"docstring(
        Create a filter that transforms the input image by 'matrix'.

        This matrix transforms the local space, which means it effectively
        happens prior to any transformation coming from the SkCanvas initiating
        the filtering.

        :param skia.Matrix matrix: The matrix to apply to the original content.
        :param skia.SamplingOptions sampling: How the image will be sampled when it is transformed
        :param skia.ImageFilter input: The image filter to transform, or null to
            use the source image.
        )docstring",
        py::arg("matrix"), py::arg("sampling"), py::arg("input") = nullptr)
    .def_static("Merge",
        [] (py::list filters, const SkIRect* cropRect) {
            std::vector<sk_sp<SkImageFilter>> filters_(filters.size());
            for (size_t i = 0; i < filters.size(); ++i) {
                auto filter = filters[i].cast<const SkImageFilter*>();
                filters_[i] = CloneFlattenable(*filter);
            }
            return SkImageFilters::Merge(
                &filters_[0], filters.size(), cropRect);
        },
        R"docstring(
        Create a filter that merges the 'count' filters together by drawing
        their results in order with src-over blending.

        :param List[skia.ImageFilter] filters: The input filter array to merge,
            which must have 'count' elements. Any null filter pointers will use
            the source bitmap instead.
        :param skia.Rect cropRect: Optional rectangle that crops all input
            filters and the output.
        )docstring",
        py::arg("filters"), py::arg("cropRect") = nullptr)
    // .def_static("Merge")
    .def_static("Offset",
        [] (SkScalar dx, SkScalar dy, const SkImageFilter* input,
            const SkIRect* cropRect) {
            return SkImageFilters::Offset(
                dx, dy, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that offsets the input filter by the given vector.

        :param float dx: The x offset in local space that the image is shifted.
        :param float dy: The y offset in local space that the image is shifted.
        :param skia.ImageFilter input: The input that will be moved, if null the
            source bitmap is used instead.
        :param skia.Rect cropRect: Optional rectangle to crop the input and
            output.
        )docstring",
        py::arg("dx"), py::arg("dy"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
/*
    .def_static("Paint", &SkImageFilters::Paint,
        R"docstring(
        Create a filter that fills the output with the given paint.

        :param skia.Paint paint: The paint to fill
        :param skia.Rect cropRect: Optional rectangle that will be filled. If
            null, the source bitmap's bounds are filled even though the source
            bitmap itself is not used.
        )docstring",
        py::arg("paint"), py::arg("cropRect") = nullptr)
*/
    .def_static("Picture",
        [] (const SkPicture& pic, const SkRect* targetRect) {
            auto pic_ = SkPicture::MakeFromData(pic.serialize().get());
            if (targetRect)
                return SkImageFilters::Picture(pic_, *targetRect);
            return SkImageFilters::Picture(pic_);
        },
        R"docstring(
        Create a filter that produces the SkPicture as its output, drawn into
        targetRect.

        Note that the targetRect is not the same as the SkIRect cropRect that
        many filters accept. Returns null if 'pic' is null.

        :param skia.Picture pic: The picture that is drawn for the filter
            output.
        :param skia.Rect targetRect: The drawing region for the picture.
        )docstring",
        py::arg("pic"), py::arg("targetRect") = nullptr)
    .def_static("Tile",
        [] (const SkRect& src, const SkRect& dst, const SkImageFilter* input) {
            return SkImageFilters::Tile(src, dst, CLONE(input));
        },
        R"docstring(
        Create a tile image filter.

        :param skia.Rect src: Defines the pixels to tile
        :param skia.Rect dst: Defines the pixel region that the tiles will be
            drawn to
        :param skia.ImageFilter input: The input that will be tiled, if null the
            source bitmap is used instead.
        )docstring",
        py::arg("src"), py::arg("dst"), py::arg("input") = nullptr)
/*
    .def_static("Xfermode",
        [] (SkBlendMode mode, const SkImageFilter* background,
            const SkImageFilter* foreground, const SkIRect* cropRect) {
            return SkImageFilters::Xfermode(
                mode, CLONE(background), CLONE(foreground), cropRect);
        },
        R"docstring(
        This filter takes an :py:class:`BlendMode` and uses it to composite the
        two filters together.

        :param skia.ImageFilter background: The Dst pixels used in blending, if
            null the source bitmap is used.
        :param skia.ImageFilter foreground: The Src pixels used in blending, if
            null the source bitmap is used. Optional rectangle to crop input and
            output.
        )docstring",
        py::arg("mode"), py::arg("background") = nullptr,
        py::arg("foreground") = nullptr, py::arg("cropRect") = nullptr)
*/
    .def_static("Dilate",
        [] (SkScalar radiusX, SkScalar radiusY, const SkImageFilter* input,
            const SkIRect* cropRect) {
            return SkImageFilters::Dilate(
                radiusX, radiusY, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that dilates each input pixel's channel values to the
        max value within the given radii along the x and y axes.

        :param float radiusX: The distance to dilate along the x axis to either
            side of each pixel.
        :param float radiusY: The distance to dilate along the y axis to either
            side of each pixel.
        :param skia.ImageFilter input: The image filter that is dilated, using
            source bitmap if this is null.
        :param skia.Rect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("radiusX"), py::arg("radiusY"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("Erode",
        [] (SkScalar radiusX, SkScalar radiusY, const SkImageFilter* input,
            const SkIRect* cropRect) {
            return SkImageFilters::Erode(
                radiusX, radiusY, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that erodes each input pixel's channel values to the
        minimum channel value within the given radii along the x and y axes.

        :param float radiusX: The distance to erode along the x axis to either
            side of each pixel.
        :param float radiusY: The distance to erode along the y axis to either
            side of each pixel.
        :param skia.ImageFilter input: The image filter that is eroded, using
            source bitmap if this is null.
        :param skia.Rect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("radiusX"), py::arg("radiusY"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("DistantLitDiffuse",
        [] (const SkPoint3& direction, SkColor lightColor,
            SkScalar surfaceScale, SkScalar kd, const SkImageFilter* input,
            const SkIRect* cropRect) {
            return SkImageFilters::DistantLitDiffuse(
                direction, lightColor, surfaceScale, kd,
                CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that calculates the diffuse illumination from a distant
        light source, interpreting the alpha channel of the input as the height
        profile of the surface (to approximate normal vectors).

        :param skia.Point3 direction: The direction to the distance light.
        :param int lightColor: The color of the diffuse light source.
        :param float surfaceScale: Scale factor to transform from alpha values
            to physical height.
        :param float kd: Diffuse reflectance coefficient.
        :param skia.ImageFilter input: The input filter that defines surface
            normals (as alpha), or uses the source bitmap when null.
        :param skia.Rect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("direction"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("kd"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("PointLitDiffuse",
        [] (const SkPoint3& location, SkColor lightColor,
            SkScalar surfaceScale, SkScalar kd, const SkImageFilter* input,
            const SkIRect* cropRect) {
            return SkImageFilters::PointLitDiffuse(
                location, lightColor, surfaceScale, kd,
                CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that calculates the diffuse illumination from a point
        light source, using alpha channel of the input as the height profile of
        the surface (to approximate normal vectors).

        :param skia.Point3 location: The location of the point light.
        :param int lightColor:  The color of the diffuse light source.
        :param float surfaceScale: Scale factor to transform from alpha values
            to physical height.
        :param float kd: Diffuse reflectance coefficient.
        :param skia.ImageFilter input: The input filter that defines surface
            normals (as alpha), or uses the source bitmap when null.
        :param skia.IRect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("location"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("kd"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("SpotLitDiffuse",
        [] (const SkPoint3& location, const SkPoint3& target,
            SkScalar falloffExponent, SkScalar cutoffAngle, SkColor lightColor,
            SkScalar surfaceScale, SkScalar kd, const SkImageFilter* input,
            const SkIRect* cropRect) {
            return SkImageFilters::SpotLitDiffuse(
                location, target, falloffExponent, cutoffAngle, lightColor,
                surfaceScale, kd, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that calculates the diffuse illumination from a spot
        light source, using alpha channel of the input as the height profile of
        the surface (to approximate normal vectors).

        The spot light is restricted to be within 'cutoffAngle' of the vector
        between the location and target.

        :param skia.Point3 location: The location of the spot light.
        :param skia.Point3 target: The location that the spot light is point
            towards
        :param float falloffExponent: Exponential falloff parameter for
            illumination outside of cutoffAngle
        :param float cutoffAngle: Maximum angle from lighting direction that
            receives full light
        :param int lightColor:  The color of the diffuse light source.
        :param float surfaceScale: Scale factor to transform from alpha values
            to physical height.
        :param float kd:  Diffuse reflectance coefficient.
        :param skia.ImageFilter input: The input filter that defines surface
            normals (as alpha), or uses the source bitmap when null.
        :param skia.IRect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("location"), py::arg("target"), py::arg("falloffExponent"),
        py::arg("cutoffAngle"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("kd"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("DistantLitSpecular",
        [] (const SkPoint3& direction, SkColor lightColor,
            SkScalar surfaceScale, SkScalar ks, SkScalar shininess,
            const SkImageFilter* input, const SkIRect* cropRect) {
            return SkImageFilters::DistantLitSpecular(
                direction, lightColor, surfaceScale, ks, shininess,
                CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that calculates the specular illumination from a distant
        light source, interpreting the alpha channel of the input as the height
        profile of the surface (to approximate normal vectors).

        :param skia.Point3 direction: The direction to the distance light.
        :param int lightColor: The color of the diffuse light source.
        :param float surfaceScale: Scale factor to transform from alpha values
            to physical height.
        :param float ks: Specular reflectance coefficient.
        :param float shininess: The specular exponent determining how shiny the
            surface is.
        :param skia.ImageFilter input: The input filter that defines surface
            normals (as alpha), or uses the source bitmap when null.
        :param skia.Rect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("direction"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("ks"), py::arg("shininess"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("PointLitSpecular",
        [] (const SkPoint3& location, SkColor lightColor,
            SkScalar surfaceScale, SkScalar ks, SkScalar shininess,
            const SkImageFilter* input, const SkIRect* cropRect) {
            return SkImageFilters::PointLitSpecular(
                location, lightColor, surfaceScale, ks, shininess,
                CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that calculates the specular illumination from a point
        light source, using alpha channel of the input as the height profile of
        the surface (to approximate normal vectors).

        :param skia.Point3 location: The location of the point light.
        :param int lightColor:  The color of the diffuse light source.
        :param float surfaceScale: Scale factor to transform from alpha values
            to physical height.
        :param float ks: Specular reflectance coefficient.
        :param float shininess: The specular exponent determining how shiny the
            surface is.
        :param skia.ImageFilter input: The input filter that defines surface
            normals (as alpha), or uses the source bitmap when null.
        :param skia.IRect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("location"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("ks"), py::arg("shininess"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("SpotLitSpecular",
        [] (const SkPoint3& location, const SkPoint3& target,
            SkScalar falloffExponent, SkScalar cutoffAngle, SkColor lightColor,
            SkScalar surfaceScale, SkScalar ks, SkScalar shininess,
            const SkImageFilter* input, const SkIRect* cropRect) {
            return SkImageFilters::SpotLitSpecular(
                location, target, falloffExponent, cutoffAngle, lightColor,
                surfaceScale, ks, shininess, CLONE(input), cropRect);
        },
        R"docstring(
        Create a filter that calculates the diffuse illumination from a spot
        light source, using alpha channel of the input as the height profile of
        the surface (to approximate normal vectors).

        The spot light is restricted to be within 'cutoffAngle' of the vector
        between the location and target.

        :param skia.Point3 location: The location of the spot light.
        :param skia.Point3 target: The location that the spot light is point
            towards
        :param float falloffExponent: Exponential falloff parameter for
            illumination outside of cutoffAngle
        :param float cutoffAngle: Maximum angle from lighting direction that
            receives full light
        :param int lightColor:  The color of the diffuse light source.
        :param float surfaceScale: Scale factor to transform from alpha values
            to physical height.
        :param float ks: Specular reflectance coefficient.
        :param float shininess: The specular exponent determining how shiny the
            surface is.
        :param skia.ImageFilter input: The input filter that defines surface
            normals (as alpha), or uses the source bitmap when null.
        :param skia.IRect cropRect: Optional rectangle that crops the input and
            output.
        )docstring",
        py::arg("location"), py::arg("target"), py::arg("falloffExponent"),
        py::arg("cutoffAngle"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("ks"), py::arg("shininess"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    ;


/*
py::class_<SkLightingImageFilter>(m, "LightingImageFilter")
    .def_static("MakeDistantLitDiffuse",
        [] (const SkPoint3& direction, SkColor lightColor,
            SkScalar surfaceScale, SkScalar kd, const SkImageFilter* input,
            const SkImageFilters::CropRect* cropRect) {
            return SkLightingImageFilter::MakeDistantLitDiffuse(
                direction, lightColor, surfaceScale, kd,
                CLONE(input), cropRect);
        },
        py::arg("direction"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("kd"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("MakePointLitDiffuse",
        [] (const SkPoint3& location, SkColor lightColor,
            SkScalar surfaceScale, SkScalar kd, const SkImageFilter* input,
            const SkImageFilters::CropRect* cropRect) {
            return SkLightingImageFilter::MakePointLitDiffuse(
                location, lightColor, surfaceScale, kd,
                CLONE(input), cropRect);
        },
        py::arg("location"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("kd"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("MakeSpotLitDiffuse",
        [] (const SkPoint3& location, const SkPoint3& target,
            SkScalar falloffExponent, SkScalar cutoffAngle, SkColor lightColor,
            SkScalar surfaceScale, SkScalar kd, const SkImageFilter* input,
            const SkImageFilters::CropRect* cropRect) {
            return SkLightingImageFilter::MakeSpotLitDiffuse(
                location, target, falloffExponent, cutoffAngle, lightColor,
                surfaceScale, kd, CLONE(input), cropRect);
        },
        py::arg("location"), py::arg("target"), py::arg("falloffExponent"),
        py::arg("cutoffAngle"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("kd"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("MakeDistantLitSpecular",
        [] (const SkPoint3& direction, SkColor lightColor,
            SkScalar surfaceScale, SkScalar ks, SkScalar shininess,
            const SkImageFilter* input,
            const SkImageFilters::CropRect* cropRect) {
            return SkLightingImageFilter::MakeDistantLitSpecular(
                direction, lightColor, surfaceScale, ks, shininess,
                CLONE(input), cropRect);
        },
        py::arg("direction"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("ks"), py::arg("shininess"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("MakePointLitSpecular",
        [] (const SkPoint3& location, SkColor lightColor,
            SkScalar surfaceScale, SkScalar ks, SkScalar shininess,
            const SkImageFilter* input,
            const SkImageFilters::CropRect* cropRect) {
            return SkLightingImageFilter::MakePointLitSpecular(
                location, lightColor, surfaceScale, ks, shininess,
                CLONE(input), cropRect);
        },
        py::arg("location"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("ks"), py::arg("shininess"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    .def_static("MakeSpotLitSpecular",
        [] (const SkPoint3& location, const SkPoint3& target,
            SkScalar falloffExponent, SkScalar cutoffAngle, SkColor lightColor,
            SkScalar surfaceScale, SkScalar ks, SkScalar shininess,
            const SkImageFilter* input,
            const SkImageFilters::CropRect* cropRect) {
            return SkLightingImageFilter::MakeSpotLitSpecular(
                location, target, falloffExponent, cutoffAngle, lightColor,
                surfaceScale, ks, shininess, CLONE(input), cropRect);
        },
        py::arg("location"), py::arg("target"), py::arg("falloffExponent"),
        py::arg("cutoffAngle"), py::arg("lightColor"), py::arg("surfaceScale"),
        py::arg("ks"), py::arg("shininess"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    ;

py::class_<SkMagnifierImageFilter>(m, "MagnifierImageFilter")
    .def_static("Make",
        [] (const SkRect& srcRect, SkScalar inset, const SkImageFilter* input,
            const SkImageFilters::CropRect* cropRect) {
            return SkMagnifierImageFilter::Make(
                srcRect, inset, CLONE(input), cropRect);
        },
        py::arg("srcRect"), py::arg("inset"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    ;
*/

// py::class_<SkMatrixConvolutionImageFilter>(m, "MatrixConvolutionImageFilter")
//     .def_static("Make",
//         py::overload_cast<const SkISize&, const SkScalar*, SkScalar, SkScalar,
//             const SkIPoint&, SkTileMode, bool, sk_sp<SkImageFilter>,
//             const SkImageFilters::CropRect*>(
//                 &SkMatrixConvolutionImageFilter::Make))
//     ;

// py::class_<SkMergeImageFilter>(m, "SkMergeImageFilter")
//     .def_static("Make", &SkMergeImageFilter::Make)
//     ;

/*
py::class_<SkOffsetImageFilter>(m, "OffsetImageFilter")
    .def_static("Make",
        [] (SkScalar dx, SkScalar dy, const SkImageFilter* input,
            const SkImageFilters::CropRect* cropRect) {
            return SkOffsetImageFilter::Make(
                dx, dy, CLONE(input), cropRect);
        },
        py::arg("dx"), py::arg("dy"), py::arg("input") = nullptr,
        py::arg("cropRect") = nullptr)
    ;

py::class_<SkPaintImageFilter>(m, "PaintImageFilter")
    .def_static("Make", &SkPaintImageFilter::Make,
        py::arg("paint"), py::arg("cropRect") = nullptr)
    ;
*/

// py::class_<SkPictureImageFilter>(m, "PictureImageFilter")
//     .def_static("Make", &SkPictureImageFilter::Make)
//     ;

/*
py::class_<SkTileImageFilter>(m, "TileImageFilter")
    .def_static("Make",
        [] (const SkRect& src, const SkRect& dst, const SkImageFilter* input) {
            return SkTileImageFilter::Make(src, dst, CLONE(input));
        },
        R"docstring(
        Create a tile image filter.

        :param skia.Rect src: Defines the pixels to tile
        :param skia.Rect dst: Defines the pixels where tiles are drawn
        :param skia.ImageFilter input: Input from which the subregion defined by
            srcRect will be tiled
        )docstring",
        py::arg("src"), py::arg("dst"), py::arg("input") = nullptr)
    ;

py::class_<SkXfermodeImageFilter>(m, "XfermodeImageFilter",
    R"docstring(
    This filter takes a :py:class:`BlendMode`, and uses it to composite the
    foreground over the background.

    If foreground or background is NULL, the input bitmap (src) is used instead.
    DEPRECATED: Use :py:meth:`ImageFilters.XferMode`
    )docstring")
    .def_static("Make",
        [] (SkBlendMode mode, const SkImageFilter* background,
            const SkImageFilter* foreground,
            const SkImageFilters::CropRect* cropRect) {
            return SkXfermodeImageFilter::Make(
                mode, CLONE(background), CLONE(foreground), cropRect);
        },
        py::arg("mode"), py::arg("background") = nullptr,
        py::arg("foreground") = nullptr, py::arg("cropRect") = nullptr)
    ;
*/
}
