#include "common.h"
#include <pybind11/operators.h>

void initColor(py::module &m) {
py::class_<SkColor4f>(m, "Color4f", R"docstring(
    RGBA color value, holding four floating point components.

    Color components are always in a known order, and are unpremultiplied.
    )docstring")
    .def(py::init(&SkColor4f::FromColor), py::arg("color"))
    .def(py::self == py::self,
        "Compares SkRGBA4f with other, and returns true if all components "
        "are equal.")
    .def(py::self != py::self,
        "Compares SkRGBA4f with other, and returns true if not all "
        "components are equal.")
    .def(py::self * float(), "Returns SkRGBA4f multiplied by scale.")
    .def(py::self * py::self,
        "Returns SkRGBA4f multiplied component-wise by scale.")
    .def("vec", py::overload_cast<>(&SkColor4f::vec, py::const_),
        py::return_value_policy::reference)
    .def("vec", py::overload_cast<>(&SkColor4f::vec),
        py::return_value_policy::reference)
    .def("__getitem__",
        [] (const SkColor4f& c, int index) {
            if (index < 0 || 4 <= index)
                throw py::index_error("Index out of range.");
            return c[index];
        },
        "Returns one component.")
    .def("__setitem__",
        [] (SkColor4f& c, int index, float value) {
            if (index < 0 || 4 <= index)
                throw py::index_error("Index out of range.");
            c[index] = value;
        },
        "Sets one component.")
    .def("isOpaque", &SkColor4f::isOpaque,
        "Returns true if SkRGBA4f is an opaque color.")
    .def("fitsInBytes", &SkColor4f::fitsInBytes,
        "Returns true if all channels are in [0, 1].")
    .def("toColor", &SkColor4f::toSkColor,
        "Returns closest SkColor to SkRGBA4f.")
    // .def("premul", &SkColor4f::premul,
    //     "Returns SkRGBA4f premultiplied by alpha.")
    // .def("unpremul", &SkColor4f::unpremul,
    //     "Returns SkRGBA4f unpremultiplied by alpha.")
    .def("toBytes_RGBA", &SkColor4f::toBytes_RGBA)
    .def("makeOpaque", &SkColor4f::makeOpaque)
    .def_static("FromColor", &SkColor4f::FromColor,
        R"docstring(
        Returns closest :py:class:`RGBA4f` to :py:class:`Color`.

        Only allowed if :py:class:`RGBA4f` is unpremultiplied.

        :color: Color with Alpha, red, blue, and green components
        :return: :py:class:`Color` as :py:class:`RGBA4f`
        )docstring",
        py::arg("color"))
    // .def_static("FromPMColor", &SkColor4f::FromPMColor,
    //     R"docstring(
    //     Returns closest :py:class:`RGBA4f` to :py:class:`PMColor`.

    //     Only allowed if :py:class:`RGBA4f` is premultiplied.

    //     :return: :py:class:`PMColor` as :py:class:`RGBA4f`
    //     )docstring",
    //     py::arg("pmcolor"))
    .def_static("FromBytes_RGBA", &SkColor4f::FromBytes_RGBA)

    .def_readonly_static("kTransparent", &SkColors::kTransparent)
    .def_readonly_static("kBlack", &SkColors::kBlack)
    .def_readonly_static("kDkGray", &SkColors::kDkGray)
    .def_readonly_static("kGray", &SkColors::kGray)
    .def_readonly_static("kLtGray", &SkColors::kLtGray)
    .def_readonly_static("kWhite", &SkColors::kWhite)
    .def_readonly_static("kRed", &SkColors::kRed)
    .def_readonly_static("kGreen", &SkColors::kGreen)
    .def_readonly_static("kBlue", &SkColors::kBlue)
    .def_readonly_static("kYellow", &SkColors::kYellow)
    .def_readonly_static("kCyan", &SkColors::kCyan)
    .def_readonly_static("kMagenta", &SkColors::kMagenta)
    ;

py::implicitly_convertible<SkColor, SkColor4f>();

py::enum_<SkColorChannel>(m, "ColorChannel",
    R"docstring(
    Describes different color channels one can manipulate.
    )docstring")
    .value("kR", SkColorChannel::kR)
    .value("kG", SkColorChannel::kG)
    .value("kB", SkColorChannel::kB)
    .value("kA", SkColorChannel::kA)
    .value("kLastEnum", SkColorChannel::kLastEnum)
    .export_values();

py::enum_<SkColorChannelFlag>(m, "ColorChannelFlag", py::arithmetic(),
    R"docstring(
    Used to represent the channels available in a color type or texture format
    as a mask.
    )docstring")
    .value("kRed_SkColorChannelFlag",
        SkColorChannelFlag::kRed_SkColorChannelFlag)
    .value("kGreen_SkColorChannelFlag",
        SkColorChannelFlag::kGreen_SkColorChannelFlag)
    .value("kBlue_SkColorChannelFlag",
        SkColorChannelFlag::kBlue_SkColorChannelFlag)
    .value("kAlpha_SkColorChannelFlag",
        SkColorChannelFlag::kAlpha_SkColorChannelFlag)
    .value("kGray_SkColorChannelFlag",
        SkColorChannelFlag::kGray_SkColorChannelFlag)
    .value("kRG_SkColorChannelFlags",
        SkColorChannelFlag::kRG_SkColorChannelFlags)
    .value("kRGB_SkColorChannelFlags",
        SkColorChannelFlag::kRGB_SkColorChannelFlags)
    .value("kRGBA_SkColorChannelFlags",
        SkColorChannelFlag::kRGBA_SkColorChannelFlags)
    .export_values();


m.def("ColorSetRGB",
    [] (U8CPU r, U8CPU g, U8CPU b) {
        return SkColorSetARGB(0xFF, r, g, b);
    },
    "Returns color value from 8-bit component values, with alpha set "
    "fully opaque to 255.");
m.def("ColorGetA",
    [] (SkColor color) { return SkColorGetA(color); },
    "Returns alpha byte from color value.");
m.def("ColorGetR",
    [] (SkColor color) { return SkColorGetR(color); },
    "Returns red component of color, from zero to 255.");
m.def("ColorGetG",
    [] (SkColor color) { return SkColorGetG(color); },
    "Returns green component of color, from zero to 255.");
m.def("ColorGetB",
    [] (SkColor color) { return SkColorGetB(color); },
    "Returns blue component of color, from zero to 255.");
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
}
