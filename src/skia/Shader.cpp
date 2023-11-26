#include "common.h"
#include <include/effects/SkRuntimeEffect.h>
#include <include/effects/SkGradientShader.h>
#include <include/effects/SkPerlinNoiseShader.h>
#include <include/effects/SkBlenders.h>
#include <pybind11/stl.h>

#define GET_SKSCALAR_PTR(pos) \
((pos.is_none()) ? nullptr : &(pos.cast<std::vector<SkScalar>>())[0])

void initShader(py::module &m) {
py::class_<SkShader, sk_sp<SkShader>, SkFlattenable> shader(
    m, "Shader", R"docstring(
    Shaders specify the source color(s) for what is being drawn.

    If a paint has no shader, then the paint's color is used. If the paint has a
    shader, then the shader's color(s) are use instead, but they are modulated
    by the paint's alpha. This makes it easy to create a shader once (e.g.
    bitmap tiling or gradient) and then change its transparency w/o having to
    modify the original shader... only the paint's alpha needs to be modified.

    .. rubric:: Subclasses

    .. autosummary::
        :nosignatures:

        ~skia.Shaders
        ~skia.GradientShader
        ~skia.PerlinNoiseShader
    )docstring");

/*
py::class_<SkShader::GradientInfo>(shader, "GradientInfo")
    .def(py::init<>())
    .def_readwrite("fColorCount", &SkShader::GradientInfo::fColorCount,
        R"docstring(
        In-out parameter, specifies passed size.
        )docstring")
    .def_property_readonly("fColors",
        [] (const SkShader::GradientInfo& info) {
            return std::vector<SkColor>(
                info.fColors, info.fColors + info.fColorCount);
        },
        R"docstring(
        The colors in the gradient.
        )docstring")
    .def_property_readonly("fColorOffsets",
        [] (const SkShader::GradientInfo& info) {
            return std::vector<SkColor>(
                info.fColorOffsets, info.fColorOffsets + info.fColorCount);
        },
        R"docstring(
        The unit offset for color transitions.
        )docstring")
    .def_property_readonly("fPoint",
        [] (const SkShader::GradientInfo& info) {
            return py::make_tuple(info.fPoint[0], info.fPoint[1]);
        },
        R"docstring(
        Type specific, see above.
        )docstring")
    .def_property_readonly("fRadius",
        [] (const SkShader::GradientInfo& info) {
            return py::make_tuple(info.fRadius[0], info.fRadius[1]);
        },
        R"docstring(
        Type specific, see above.
        )docstring")
    .def_readonly("fTileMode", &SkShader::GradientInfo::fTileMode)
    .def_readonly("fGradientFlags", &SkShader::GradientInfo::fGradientFlags,
        R"docstring(
        see :py:class:`GradientShader.Flags`
        )docstring")
    ;

py::enum_<SkShader::GradientType>(shader, "GradientType", R"docstring(
    If the shader subclass can be represented as a gradient, asAGradient returns
    the matching GradientType enum (or kNone_GradientType if it cannot).

    Also, if info is not null, asAGradient populates info with the relevant (see
    below) parameters for the gradient. fColorCount is both an input and output
    parameter. On input, it indicates how many entries in fColors and
    fColorOffsets can be used, if they are non-NULL. After asAGradient has run,
    fColorCount indicates how many color-offset pairs there are in the gradient.
    If there is insufficient space to store all of the color-offset pairs,
    fColors and fColorOffsets will not be altered. fColorOffsets specifies where
    on the range of 0 to 1 to transition to the given color. The meaning of
    fPoint and fRadius is dependant on the type of gradient.

    - None: info is ignored. Color: fColorOffsets[0] is meaningless.
    - Linear: fPoint[0] and fPoint[1] are the end-points of the gradient
    - Radial: fPoint[0] and fRadius[0] are the center and radius
    - Conical: fPoint[0] and fRadius[0] are the center and radius of the 1st
        circle fPoint[1] and fRadius[1] are the center and radius of the 2nd
        circle Sweep: fPoint[0] is the center of the sweep.
    )docstring")
    .value("kNone_GradientType",
        SkShader::GradientType::kNone_GradientType)
    .value("kColor_GradientType",
        SkShader::GradientType::kColor_GradientType)
    .value("kLinear_GradientType",
        SkShader::GradientType::kLinear_GradientType)
    .value("kRadial_GradientType",
        SkShader::GradientType::kRadial_GradientType)
    .value("kSweep_GradientType",
        SkShader::GradientType::kSweep_GradientType)
    .value("kConical_GradientType",
        SkShader::GradientType::kConical_GradientType)
    .value("kLast_GradientType",
        SkShader::GradientType::kLast_GradientType)
    .export_values();
*/

shader
    .def("isOpaque", &SkShader::isOpaque,
        R"docstring(
        Returns true if the shader is guaranteed to produce only opaque colors,
        subject to the :py:class:`Paint` using the shader to apply an opaque
        alpha value.

        Subclasses should override this to allow some optimizations.
        )docstring")
    .def("isAImage",
        [] (const SkShader& shader, SkMatrix* localMatrix,
            std::vector<SkTileMode>& xy) {
            if (xy.size() != 2)
                throw std::runtime_error("xy must have two elements.");
            return sk_sp<SkImage>(shader.isAImage(localMatrix, &xy[0]));
        },
        R"docstring(
        Iff this shader is backed by a single :py:class:`Image`, return its ptr
        (the caller must ref this if they want to keep it longer than the
        lifetime of the shader).

        If not, return nullptr.
        )docstring",
        py::arg("localMatrix"), py::arg("xy") = nullptr)
    .def("isAImage", py::overload_cast<>(&SkShader::isAImage, py::const_))
/*
    .def("asAGradient", &SkShader::asAGradient, py::arg("info"))
*/
    .def("makeWithLocalMatrix", &SkShader::makeWithLocalMatrix,
        R"docstring(
        Return a shader that will apply the specified localMatrix to this
        shader.

        The specified matrix will be applied before any matrix associated with
        this shader.
        )docstring",
        py::arg("matrix"))
    .def("makeWithColorFilter", &SkShader::makeWithColorFilter,
        R"docstring(
        Create a new shader that produces the same colors as invoking this
        shader and then applying the colorfilter.
        )docstring",
        py::arg("colorFilter"))
/*
    .def_static("Deserialize",
        [] (py::buffer b) {
            auto info = b.request();
            auto shader = SkShader::Deserialize(
                SkFlattenable::Type::kSkShaderBase_Type, info.ptr,
                info.shape[0] * info.strides[0]);
            return sk_sp<SkShader>(
                reinterpret_cast<SkShader*>(shader.release()));
        },
        py::arg("data"))
*/
    ;

py::class_<std::unique_ptr<int32_t>>(m, "Shaders")
    .def_static("Empty", &SkShaders::Empty)
    .def_static("Color", py::overload_cast<SkColor>(&SkShaders::Color),
        py::arg("color"))
    .def_static("Color",
        [] (const SkColor4f& color, const SkColorSpace& cs) {
            return SkShaders::Color(color, CloneFlattenable(cs));
        },
        py::arg("color"), py::arg("cs") = nullptr)
    .def_static("Blend",
        [] (SkBlendMode mode, const SkShader& dst,
            const SkShader& src) {
            return SkShaders::Blend(
                mode, CloneFlattenable(dst), CloneFlattenable(src));
        },
        py::arg("mode"), py::arg("dst"), py::arg("src"))
    .def_static("Blend",
        [] (sk_sp<SkBlender> blender, sk_sp<SkShader> dst,
            sk_sp<SkShader> src) {
            return SkShaders::Blend(
                blender, dst, src);
        },
        py::arg("blender"), py::arg("dst"), py::arg("src"))
    .def_static("Lerp",
        [] (SkScalar t, const SkShader& dst,
            const SkShader& src) {
            return SkShaders::Blend(
                SkBlenders::Arithmetic(0, t, 1-t, 0, false),
                CloneFlattenable(dst), CloneFlattenable(src));
        },
        py::arg("t"), py::arg("dst"), py::arg("src"))
    ;

py::class_<SkGradientShader> gradientshader(m, "GradientShader");

py::enum_<SkGradientShader::Flags>(gradientshader, "Flags", py::arithmetic())
    .value("kInterpolateColorsInPremul_Flag",
        SkGradientShader::Flags::kInterpolateColorsInPremul_Flag)
    .export_values();

gradientshader
    .def_static("MakeLinear",
        [] (const std::vector<SkPoint>& pts, const std::vector<SkColor>& colors,
            py::object pos, SkTileMode mode, uint32_t flags,
            const SkMatrix* localMatrix) {
            if (pts.size() != 2)
                throw std::runtime_error("pts must have two elements.");
            if (colors.size() < 2)
                throw std::runtime_error("length of colors must be 2 or more.");
            return SkGradientShader::MakeLinear(
                &pts[0], &colors[0], GET_SKSCALAR_PTR(pos), colors.size(), mode,
                flags, localMatrix);
        },
        R"docstring(
        Returns a shader that generates a linear gradient between the two
        specified points.

        :param List[skia.Point] points: The start and end points for the
            gradient.
        :param List[int] colors: The array of colors, to be distributed
            between the two points
        :param List[float] positions: May be empty list. array of Scalars, or
            NULL, of the relative position of each corresponding color in the
            colors array. If this is NULL, the the colors are distributed evenly
            between the start and end point. If this is not null, the values
            must begin with 0, end with 1.0, and intermediate values must be
            strictly increasing.
        :param skia.TileMode mode: The tiling mode
        :param localMatrix: Local matrix
        )docstring",
        py::arg("points"), py::arg("colors"), py::arg("positions") = nullptr,
        py::arg("mode") = SkTileMode::kClamp, py::arg("flags") = 0,
        py::arg("localMatrix") = nullptr)
    .def_static("MakeRadial",
        [] (const SkPoint& center, SkScalar radius,
            const std::vector<SkColor>& colors,
            py::object pos, SkTileMode mode, uint32_t flags,
            const SkMatrix* localMatrix) {
            if (colors.size() < 2)
                throw std::runtime_error("length of colors must be 2 or more.");
            return SkGradientShader::MakeRadial(
                center, radius, &colors[0], GET_SKSCALAR_PTR(pos),
                colors.size(), mode, flags, localMatrix);
        },
        R"docstring(
        Returns a shader that generates a radial gradient given the center and
        radius.

        :param skia.Point center:  The center of the circle for this gradient
        :param float radius:  Must be positive. The radius of the circle for
            this gradient
        :param List[int] colors:  The array of colors, to be distributed between
            the center and edge of the circle
        :param List[float] positions: May be empty list. The array of Scalars,
            or NULL, of the relative position of each corresponding color in the
            colors array. If this is NULL, the the colors are distributed evenly
            between the center and edge of the circle. If this is not null, the
            values must begin with 0, end with 1.0, and intermediate values must
            be strictly increasing.
        :param skia.TileMode mode: The tiling mode
        :param localMatrix: Local matrix
        )docstring",
        py::arg("center"), py::arg("radius"), py::arg("colors"),
        py::arg("positions") = nullptr, py::arg("mode") = SkTileMode::kClamp,
        py::arg("flags") = 0, py::arg("localMatrix") = nullptr)
    .def_static("MakeTwoPointConical",
        [] (const SkPoint& start, SkScalar startRadius, const SkPoint& end,
            SkScalar endRadius, const std::vector<SkColor>& colors,
            py::object pos, SkTileMode mode, uint32_t flags,
            const SkMatrix *localMatrix) {
            if (colors.size() < 2)
                throw std::runtime_error("length of colors must be 2 or more.");
            return SkGradientShader::MakeTwoPointConical(
                start, startRadius, end, endRadius, &colors[0],
                GET_SKSCALAR_PTR(pos), colors.size(), mode, flags,
                localMatrix);
        },
        R"docstring(
        Returns a shader that generates a conical gradient given two circles, or
        returns NULL if the inputs are invalid.

        The gradient interprets the two circles according to the following HTML
        spec.
        http://dev.w3.org/html5/2dcontext/#dom-context-2d-createradialgradient
        )docstring",
        py::arg("start"), py::arg("startRadius"), py::arg("end"),
        py::arg("endRadius"), py::arg("colors"), py::arg("positions") = nullptr,
        py::arg("mode") = SkTileMode::kClamp, py::arg("flags") = 0,
        py::arg("localMatrix") = nullptr)
    .def_static("MakeSweep",
        [] (SkScalar cx, SkScalar cy, const std::vector<SkColor>& colors,
            py::object pos, SkTileMode mode, SkScalar startAngle,
            SkScalar endAngle, uint32_t flags,
            const SkMatrix *localMatrix) {
            if (colors.size() < 2)
                throw std::runtime_error("length of colors must be 2 or more.");
            return SkGradientShader::MakeSweep(
                cx, cy, &colors[0], GET_SKSCALAR_PTR(pos),
                colors.size(), mode, startAngle, endAngle, flags, localMatrix);
        },
        R"docstring(
        Returns a shader that generates a conical gradient given two circles, or
        returns NULL if the inputs are invalid.

        The gradient interprets the two circles according to the following HTML
        spec.
        http://dev.w3.org/html5/2dcontext/#dom-context-2d-createradialgradient
        )docstring",
        py::arg("cx"), py::arg("cy"), py::arg("colors"),
        py::arg("positions") = nullptr, py::arg("mode") = SkTileMode::kClamp,
        py::arg("startAngle") = 0, py::arg("endAngle") = 360,
        py::arg("flags") = 0, py::arg("localMatrix") = nullptr)
    ;

py::class_<std::unique_ptr<uint32_t>>(m, "PerlinNoiseShader",
    R"docstring(
    :py:class:`PerlinNoiseShader` creates an image using the Perlin turbulence
    function.

    It can produce tileable noise if asked to stitch tiles and provided a tile
    size. In order to fill a large area with repeating noise, set the
    stitchTiles flag to true, and render exactly a single tile of noise. Without
    this flag, the result will contain visible seams between tiles.

    The algorithm used is described here:
    http://www.w3.org/TR/SVG/filters.html#feTurbulenceElement
    )docstring")
    .def_static("MakeFractalNoise", &SkShaders::MakeFractalNoise,
        R"docstring(
        This will construct Perlin noise of the given type (Fractal Noise or
        Turbulence).

        Both base frequencies (X and Y) have a usual range of (0..1) and must be
        non-negative.

        The number of octaves provided should be fairly small, with a limit of
        255 enforced. Each octave doubles the frequency, so 10 octaves would
        produce noise from baseFrequency * 1, * 2, * 4, ..., * 512, which
        quickly yields insignificantly small periods and resembles regular
        unstructured noise rather than Perlin noise.

        If tileSize isn't NULL or an empty size, the tileSize parameter will be
        used to modify the frequencies so that the noise will be tileable for
        the given tile size. If tileSize is NULL or an empty size, the
        frequencies will be used as is without modification.
        )docstring",
        py::arg("baseFrequencyX"), py::arg("baseFrequencyY"),
        py::arg("numOctaves"), py::arg("seed"), py::arg("tileSize") = nullptr)
    .def_static("MakeTurbulence", &SkShaders::MakeTurbulence,
        py::arg("baseFrequencyX"), py::arg("baseFrequencyY"),
        py::arg("numOctaves"), py::arg("seed"), py::arg("tileSize") = nullptr)
/*
    .def_static("MakeImprovedNoise", &SkShaders::MakeImprovedNoise,
        R"docstring(
        Creates an Improved Perlin Noise shader.

        The z value is roughly equivalent to the seed of the other two types,
        but minor variations to z will only slightly change the noise.
        )docstring",
        py::arg("baseFrequencyX"), py::arg("baseFrequencyY"),
        py::arg("numOctaves"), py::arg("z"))
*/
    ;
}
