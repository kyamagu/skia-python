#include "common.h"
#include <include/effects/SkColorMatrix.h>
#include <include/effects/SkOverdrawColorFilter.h>
#include <include/effects/SkColorMatrixFilter.h>
#include <include/effects/SkLumaColorFilter.h>
#include <pybind11/stl.h>
#include <include/effects/SkHighContrastFilter.h>

const int SkOverdrawColorFilter::kNumColors;

namespace {

py::object ColorFilterAsAColorMode(SkColorFilter& colorFilter) {
    SkColor color;
    SkBlendMode mode;
    auto result = colorFilter.asAColorMode(&color, &mode);
    if (result)
        return py::make_tuple(color, mode);
    else
        return py::none();
}

void CopyTableIfValid(py::object obj, std::vector<uint8_t>* table) {
    if (!obj.is_none()) {
        auto v = obj.cast<std::vector<uint8_t>>();
        if (v.size() != 256)
            throw py::value_error("Table must have 256 elements");
        table->assign(v.begin(), v.end());
    }
}

}  // namespace

void initColorFilter(py::module &m) {
py::class_<SkColorFilter, sk_sp<SkColorFilter>, SkFlattenable> colorfilter(
    m, "ColorFilter",
    R"docstring(
    ColorFilters are optional objects in the drawing pipeline.

    When present in a paint, they are called with the "src" colors, and return
    new colors, which are then passed onto the next stage (either ImageFilter or
    Xfermode).

    All subclasses are required to be reentrant-safe : it must be legal to share
    the same instance between several threads.

    .. rubric:: Subclasses

    .. autosummary::
        :nosignatures:

        ~skia.ColorFilters
        ~skia.ColorMatrixFilter
        ~skia.HighContrastFilter
        ~skia.LumaColorFilter
        ~skia.OverdrawColorFilter
        ~skia.TableColorFilter
    )docstring");

/*
py::enum_<SkColorFilter::Flags>(colorfilter, "Flags", py::arithmetic())
    .value("kAlphaUnchanged_Flag", SkColorFilter::kAlphaUnchanged_Flag)
    .export_values();
*/

colorfilter
    .def("asColorMode", &ColorFilterAsAColorMode)
    .def("asAColorMode", &ColorFilterAsAColorMode,
        R"docstring(
        If the filter can be represented by a source color plus Mode, this
        returns color and mode appropriately.

        If not, this returns None.

        :rtype: Tuple[int,skia.BlendMode] or None
        )docstring")
    .def("asAColorMatrix",
        [] (SkColorFilter& colorFilter) -> py::object {
            std::vector<float> matrix(20);
            if (colorFilter.asAColorMatrix(&matrix[0]))
                return py::cast(matrix);
            else
                return py::none();
        },
        R"docstring(
        If the filter can be represented by a 5x4 matrix, this returns list of
        floats appropriately.

        If not, this returns None.

        :rtype: List[float] or None
        )docstring")
    // .def("appendStages", &SkColorFilter::appendStages)
    // .def("program", &SkColorFilter::program)
/*
    .def("getFlags", &SkColorFilter::getFlags,
        R"docstring(
        Returns the flags for this filter.

        Override in subclasses to return custom flags.
        )docstring")
*/
    .def("filterColor", &SkColorFilter::filterColor, py::arg("color"))
    .def("filterColor4f", &SkColorFilter::filterColor4f,
        R"docstring(
        Converts the src color (in src colorspace), into the dst colorspace,
        then applies this filter to it, returning the filtered color in the dst
        colorspace.
        )docstring",
        py::arg("srcColor"), py::arg("srcCS"), py::arg("dstCS"))
    .def("makeComposed",
        [] (SkColorFilter& colorFilter, const SkColorFilter& inner) {
            return colorFilter.makeComposed(
                CloneFlattenable<SkColorFilter>(inner));
        },
        R"docstring(
        Construct a colorfilter whose effect is to first apply the inner filter
        and then apply this filter, applied to the output of the inner filter.

        result = this(inner(...))
        )docstring",
        py::arg("inner"))
    .def_static("Deserialize",
        [] (py::buffer b) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return SkColorFilter::Deserialize(info.ptr, size);
        },
        py::arg("data"))
    ;

py::class_<SkColorMatrix>(m, "ColorMatrix")
    .def(py::init<>())
    // TODO: Implement me!
    ;

py::class_<SkColorFilters>(m, "ColorFilters")
/*
    .def_static("Compose",
        [] (const SkColorFilter& outer, const SkColorFilter& inner) {
            return SkColorFilters::Compose(
                CloneFlattenable<SkColorFilter>(outer),
                CloneFlattenable<SkColorFilter>(inner));
        },
        py::arg("outer"), py::arg("inner"))
*/
    .def_static("Blend", py::overload_cast<const SkColor4f&, sk_sp<SkColorSpace>,
        SkBlendMode>(&SkColorFilters::Blend),
        py::arg("c"), py::arg("colorspace"), py::arg("mode"))
    .def_static("Blend", py::overload_cast<SkColor,
        SkBlendMode>(&SkColorFilters::Blend),
        py::arg("c"), py::arg("mode"))
    // .def_static("Matrix",
    //     py::overload_cast<const SkColorMatrix&>(&SkColorFilters::Matrix))
    .def_static("Matrix",
        [] (const std::vector<float>& rowMajor) {
            if (rowMajor.size() != 20)
                throw std::runtime_error("Input must have 20 elements.");
            return SkColorFilters::Matrix(&rowMajor[0]);
        },
        py::arg("rowMajor"))
    .def_static("HSLAMatrix",
        [] (const std::vector<float>& rowMajor) {
            if (rowMajor.size() != 20)
                throw std::runtime_error("Input must have 20 elements.");
            return SkColorFilters::HSLAMatrix(&rowMajor[0]);
        },
        py::arg("rowMajor"))
    .def_static("HSLAMatrix",
        py::overload_cast<const SkColorMatrix&>(&SkColorFilters::HSLAMatrix),
        R"docstring(
        A version of Matrix which operates in HSLA space instead of RGBA.
        i.e. ``HSLA-to-RGBA(Matrix(RGBA-to-HSLA(input)))``.
        )docstring",
        py::arg("colorMatrix"))
    .def_static("LinearToSRGBGamma", &SkColorFilters::LinearToSRGBGamma)
    .def_static("SRGBToLinearGamma", &SkColorFilters::SRGBToLinearGamma)
    .def_static("Lerp",
        [] (float t, const SkColorFilter& dst, const SkColorFilter& src) {
            return SkColorFilters::Lerp(
                t, CloneFlattenable<SkColorFilter>(dst),
                CloneFlattenable<SkColorFilter>(src));
        },
        py::arg("t"), py::arg("dst"), py::arg("src"))
    ;

py::class_<SkColorMatrixFilter, sk_sp<SkColorMatrixFilter>, SkColorFilter>(
    m, "ColorMatrixFilter")
    .def_static("MakeLightingFilter", &SkColorMatrixFilter::MakeLightingFilter,
        R"docstring(
        Create a colorfilter that multiplies the RGB channels by one color, and
        then adds a second color, pinning the result for each component to
        [0..255].

        The alpha components of the mul and add arguments are ignored.
        )docstring",
        py::arg("mul"), py::arg("add"))
    ;

py::class_<SkHighContrastConfig> highcontrastconfig(m, "HighContrastConfig",
    R"docstring(
    Configuration struct for :py:class:`HighContrastFilter`.

    Provides transformations to improve contrast for users with low vision.
    )docstring");

py::enum_<SkHighContrastConfig::InvertStyle>(highcontrastconfig, "InvertStyle")
    .value("kNoInvert",
        SkHighContrastConfig::InvertStyle::kNoInvert)
    .value("kInvertBrightness",
        SkHighContrastConfig::InvertStyle::kInvertBrightness)
    .value("kInvertLightness",
        SkHighContrastConfig::InvertStyle::kInvertLightness)
    .value("kLast",
        SkHighContrastConfig::InvertStyle::kLast)
    .export_values();

highcontrastconfig
    .def(py::init<>())
    .def(py::init<bool, SkHighContrastConfig::InvertStyle, SkScalar>(),
        py::arg("grayscale"), py::arg("invertStyle"), py::arg("contrast"))
    .def("isValid", &SkHighContrastConfig::isValid)
    .def_readwrite("fGrayscale", &SkHighContrastConfig::fGrayscale)
    .def_readwrite("fInvertStyle", &SkHighContrastConfig::fInvertStyle)
    .def_readwrite("fContrast", &SkHighContrastConfig::fContrast)
    ;

py::class_<SkHighContrastFilter>(m, "HighContrastFilter",
    R"docstring(
    Color filter that provides transformations to improve contrast for users
    with low vision.

    Applies the following transformations in this order. Each of these can be
    configured using :py:class:`HighContrastConfig`.

     - Conversion to grayscale
     - Color inversion (either in RGB or HSL space)
     - Increasing the resulting contrast.

    Calling :py:meth:`HighContrastFilter.Make` will return nullptr if the config
    is not valid, e.g. if you try to call it with a contrast outside the range
    of -1.0 to 1.0.
    )docstring")
    .def_static("Make", &SkHighContrastFilter::Make, py::arg("config"))
    ;

py::class_<SkLumaColorFilter>(
    m, "LumaColorFilter", R"docstring(
    :py:class:`LumaColorFilter` multiplies the luma of its input into the alpha
    channel, and sets the red, green, and blue channels to zero.

    :py:class:`LumaColorFilter` (r,g,b,a) = {0,0,0, a * luma(r,g,b)}

    This is similar to a luminanceToAlpha feColorMatrix, but note how this
    filter folds in the previous alpha, something an feColorMatrix cannot do.

    feColorMatrix(luminanceToAlpha; r,g,b,a) = {0,0,0, luma(r,g,b)}

    (Despite its name, an feColorMatrix using luminanceToAlpha does actually
    compute luma, a dot-product of gamma-encoded color channels, not luminance,
    a dot-product of linear color channels. So at least
    :py:class:`LumaColorFilter` and feColorMatrix+luminanceToAlpha agree there.)
    )docstring")
    .def_static("Make", &SkLumaColorFilter::Make);

py::class_<SkOverdrawColorFilter>(
    m, "OverdrawColorFilter", R"docstring(
    Uses the value in the src alpha channel to set the dst pixel.

    0 -> colors[0] 1 -> colors[1] ... 5 (or larger) -> colors[5]
    )docstring")
    .def_static("MakeWithColors",
        [] (const std::vector<SkColor>& colors) {
            if (colors.size() != 6)
                throw std::runtime_error("colors must have 6 elements");
            return SkOverdrawColorFilter::MakeWithSkColors(&colors[0]);
        },
        py::arg("colors"))
    .def_readonly_static("kNumColors", &SkOverdrawColorFilter::kNumColors)
    ;

/* SkTableColorFilter class removed in m116 */
/*
py::class_<SkTableColorFilter>(
    m, "TableColorFilter")
    .def_static("Make",
        [] (const std::vector<uint8_t>& table) {
            if (table.size() != 256)
                throw std::runtime_error("table must have 256 elements");
            return SkTableColorFilter::Make(&table[0]);
        },
        R"docstring(
        Create a table colorfilter, copying the table into the filter, and
        applying it to all 4 components.

        a' = table[a]; r' = table[r]; g' = table[g]; b' = table[b]; Compoents
        are operated on in unpremultiplied space. If the incomming colors are
        premultiplied, they are temporarily unpremultiplied, then the table is
        applied, and then the result is remultiplied.
        )docstring",
        py::arg("table"))
    .def_static("MakeARGB",
        [] (py::object tableA, py::object tableR, py::object tableG,
            py::object tableB) {
            std::vector<uint8_t> tableA_, tableR_, tableG_, tableB_;
            CopyTableIfValid(tableA, &tableA_);
            CopyTableIfValid(tableR, &tableR_);
            CopyTableIfValid(tableG, &tableG_);
            CopyTableIfValid(tableB, &tableB_);
            return SkTableColorFilter::MakeARGB(
                (tableA_.empty()) ? nullptr : &tableA_[0],
                (tableR_.empty()) ? nullptr : &tableR_[0],
                (tableG_.empty()) ? nullptr : &tableG_[0],
                (tableB_.empty()) ? nullptr : &tableB_[0]);
        },
        R"docstring(
        Create a table colorfilter, with a different table for each component
        [A, R, G, B].

        If a given table is NULL, then it is treated as identity, with the
        component left unchanged. If a table is not null, then its contents are
        copied into the filter.
        )docstring",
        py::arg("tableA"), py::arg("tableR"), py::arg("tableG"),
        py::arg("tableB"))
    ;
*/

}
