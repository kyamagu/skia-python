#include "common.h"
#include <pybind11/stl.h>

const int SkOverdrawColorFilter::kNumColors;

py::object ColorFilterAsAColorMode(SkColorFilter& colorFilter) {
    SkColor color;
    SkBlendMode mode;
    auto result = colorFilter.asColorMode(&color, &mode);
    if (result)
        return py::make_tuple(color, mode);
    else
        return py::none();
}

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

    .. rubric:: Classes

    .. autosummary::
        :nosignatures:

        ~ColorFilter.Flags

    )docstring");

py::enum_<SkColorFilter::Flags>(colorfilter, "Flags", py::arithmetic())
    .value("kAlphaUnchanged_Flag", SkColorFilter::kAlphaUnchanged_Flag)
    .export_values();

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
    .def("getFlags", &SkColorFilter::getFlags,
        R"docstring(
        Returns the flags for this filter.

        Override in subclasses to return custom flags.
        )docstring")
    .def("filterColor", &SkColorFilter::filterColor, py::arg("color"))
    .def("filterColor4f", &SkColorFilter::filterColor4f,
        R"docstring(
        Converts the src color (in src colorspace), into the dst colorspace,
        then applies this filter to it, returning the filtered color in the dst
        colorspace.
        )docstring",
        py::arg("srcColor"), py::arg("srcCS"), py::arg("dstCS"))
    // .def("makeComposed", &SkColorFilter::makeComposed)
    .def("affectsTransparentBlack", &SkColorFilter::affectsTransparentBlack)
    .def_static("Deserialize",
        [] (py::buffer b) {
            auto info = b.request();
            return SkColorFilter::Deserialize(
                info.ptr, info.shape[0] * info.strides[0]);
        })
    ;

py::class_<SkLumaColorFilter, sk_sp<SkLumaColorFilter>, SkColorFilter>(
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

py::class_<SkOverdrawColorFilter, sk_sp<SkOverdrawColorFilter>, SkColorFilter>(
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
        [] (const std::vector<uint8_t>& tableA,
            const std::vector<uint8_t>& tableR,
            const std::vector<uint8_t>& tableG,
            const std::vector<uint8_t>& tableB) {
            if (tableA.size() != 256 || tableR.size() != 256 ||
                tableG.size() != 256 || tableB.size() != 256)
                throw std::runtime_error("each table must have 256 elements");
            return SkTableColorFilter::MakeARGB(
                &tableA[0], &tableR[0], &tableG[0], &tableB[0]);
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

}
