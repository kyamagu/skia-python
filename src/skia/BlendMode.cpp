#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initBlendMode(py::module &m) {
py::enum_<SkBlendMode>(m, "BlendMode")
    .value("kClear", SkBlendMode::kClear,
        "replaces destination with zero: fully transparent")
    .value("kSrc", SkBlendMode::kSrc,
        "replaces destination")
    .value("kDst", SkBlendMode::kDst,
        "preserves destination")
    .value("kSrcOver", SkBlendMode::kSrcOver,
        "source over destination")
    .value("kDstOver", SkBlendMode::kDstOver,
        "destination over source")
    .value("kSrcIn", SkBlendMode::kSrcIn,
        "source trimmed inside destination")
    .value("kDstIn", SkBlendMode::kDstIn,
        "destination trimmed by source")
    .value("kSrcOut", SkBlendMode::kSrcOut,
        "source trimmed outside destination")
    .value("kDstOut", SkBlendMode::kDstOut,
        "destination trimmed outside source")
    .value("kSrcATop", SkBlendMode::kSrcATop,
        "source inside destination blended with destination")
    .value("kDstATop", SkBlendMode::kDstATop,
        "destination inside source blended with source")
    .value("kXor", SkBlendMode::kXor,
        "each of source and destination trimmed outside the other")
    .value("kPlus", SkBlendMode::kPlus,
        "sum of colors")
    .value("kModulate", SkBlendMode::kModulate,
        "product of premultiplied colors; darkens destination")
    .value("kScreen", SkBlendMode::kScreen,
        "multiply inverse of pixels, inverting result; brightens destination")
    .value("kLastCoeffMode", SkBlendMode::kLastCoeffMode,
        "last porter duff blend mode")
    .value("kOverlay", SkBlendMode::kOverlay,
        "multiply or screen, depending on destination")
    .value("kDarken", SkBlendMode::kDarken,
        "darker of source and destination")
    .value("kLighten", SkBlendMode::kLighten,
        "lighter of source and destination")
    .value("kColorDodge", SkBlendMode::kColorDodge,
        "brighten destination to reflect source")
    .value("kColorBurn", SkBlendMode::kColorBurn,
        "darken destination to reflect source")
    .value("kHardLight", SkBlendMode::kHardLight,
        "multiply or screen, depending on source")
    .value("kSoftLight", SkBlendMode::kSoftLight,
        "lighten or darken, depending on source")
    .value("kDifference", SkBlendMode::kDifference,
        "subtract darker from lighter with higher contrast")
    .value("kExclusion", SkBlendMode::kExclusion,
        "subtract darker from lighter with lower contrast")
    .value("kMultiply", SkBlendMode::kMultiply,
        "multiply source with destination, darkening image")
    .value("kLastSeparableMode", SkBlendMode::kLastSeparableMode,
        "last blend mode operating separately on components")
    .value("kHue", SkBlendMode::kHue,
        "hue of source with saturation and luminosity of destination")
    .value("kSaturation", SkBlendMode::kSaturation,
        "saturation of source with hue and luminosity of destination")
    .value("kColor", SkBlendMode::kColor,
        "hue and saturation of source with luminosity of destination")
    .value("kLuminosity", SkBlendMode::kLuminosity,
        "luminosity of source with hue and saturation of destination")
    .value("kLastMode", SkBlendMode::kLastMode,
        "last valid value")
    .export_values();

py::enum_<SkBlendModeCoeff>(m, "BlendModeCoeff", R"docstring(
    For Porter-Duff SkBlendModes (those <= kLastCoeffMode), these coefficients
    describe the blend equation used.

    Coefficient-based blend modes specify an equation: ('dstCoeff' * dst +
    'srcCoeff' * src), where the coefficient values are constants, functions of
    the src or dst alpha, or functions of the src or dst color.
    )docstring")
    .value("kZero", SkBlendModeCoeff::kZero,
        "0")
    .value("kOne", SkBlendModeCoeff::kOne,
        "1")
    .value("kSC", SkBlendModeCoeff::kSC,
        "src color")
    .value("kISC", SkBlendModeCoeff::kISC,
        "inverse src color")
    .value("kDC", SkBlendModeCoeff::kDC,
        "dst color")
    .value("kIDC", SkBlendModeCoeff::kIDC,
        "inverse dst color")
    .value("kSA", SkBlendModeCoeff::kSA,
        "src alpha")
    .value("kISA", SkBlendModeCoeff::kISA,
        "inverse src alpha")
    .value("kDA", SkBlendModeCoeff::kDA,
        "dst alpha")
    .value("kIDA", SkBlendModeCoeff::kIDA,
        "inverse dst alpha")
    .value("kCoeffCount", SkBlendModeCoeff::kCoeffCount)
    .export_values();
m.def("BlendMode_AsCoeff", &SkBlendMode_AsCoeff,
    "Returns true if 'mode' is a coefficient-based blend mode "
    "(<= kLastCoeffMode).");
m.def("BlendMode_Name", &SkBlendMode_Name,
    "Returns name of blendMode as null-terminated C string.");
}
