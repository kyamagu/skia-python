#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initColor(py::module &m) {
    m.def("ColorSetARGB", &SkColorSetARGB,
        "Returns color value from 8-bit component values.");
    m.def("ColorSetA", &SkColorSetA,
        "Returns unpremultiplied color with red, blue, and green set from c; "
        "and alpha set from a.");
    m.def("RGBToHSV", &SkRGBToHSV, "Converts RGB to its HSV components.");
    m.def("ColorToHSV", &SkColorToHSV, "Converts ARGB to its HSV components.");
    m.def("HSVToColor",
        py::overload_cast<U8CPU, const SkScalar[3]>(&SkHSVToColor),
        "Converts HSV components to an ARGB color.");
    m.def("HSVToColor",
        py::overload_cast<const SkScalar[3]>(&SkHSVToColor));
    m.def("PreMultiplyARGB", &SkPreMultiplyARGB,
        "Returns a SkPMColor value from unpremultiplied 8-bit component "
        "values.");
    m.def("PreMultiplyColor", &SkPreMultiplyColor,
        "Returns pmcolor closest to color c.");

    m.attr("AlphaTRANSPARENT") = SK_AlphaTRANSPARENT;
    m.attr("AlphaOPAQUE") = SK_AlphaOPAQUE;
    m.attr("ColorTRANSPARENT") = SK_ColorTRANSPARENT;
    m.attr("ColorBLACK") = SK_ColorBLACK;
    m.attr("ColorDKGRAY") = SK_ColorDKGRAY;
    m.attr("ColorGRAY") = SK_ColorGRAY;
    m.attr("ColorLTGRAY") = SK_ColorLTGRAY;
    m.attr("ColorWHITE") = SK_ColorWHITE;
    m.attr("ColorRED") = SK_ColorRED;
    m.attr("ColorGREEN") = SK_ColorGREEN;
    m.attr("ColorBLUE") = SK_ColorBLUE;
    m.attr("ColorYELLOW") = SK_ColorYELLOW;
    m.attr("ColorCYAN") = SK_ColorCYAN;
    m.attr("ColorYELLOW") = SK_ColorYELLOW;

    py::class_<SkColor4f>(m, "Color4f");
}
