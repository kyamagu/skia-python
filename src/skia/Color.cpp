#include "common.h"
#include <pybind11/operators.h>
#include <pybind11/stl.h>

void initColor(py::module &m) {
py::class_<SkColor4f>(m, "Color4f", R"docstring(
    RGBA color value, holding four floating point components.

    Color components are always in a known order, and are unpremultiplied.

    Skia's public API always uses unpremultiplied colors.

    Example::

        color4f = skia.Color4f(0xFFFFFFFF)            # From int format
        color4f = skia.Color4f(1.0, 0.0, 0.0, 1.0)    # From elements
        color4f = skia.Color4f((1.0, 0.0, 0.0, 1.0))  # From tuple
        color = color4f.toColor()                     # Convert to int format
        color = int(color4f)                          # Convert to int format
        r, g, b, a = tuple(color4f)                   # Convert to tuple
    )docstring")
    .def(py::init(&SkColor4f::FromColor),
        R"docstring(
        Returns closest :py:class:`Color4f` to ARGB Color.
        See :py:meth:`FromColor`.
        )docstring",
        py::arg("color"))
    .def(py::init(
        [] (float r, float g, float b, float a) {
            SkColor4f color4f = {r, g, b, a};
            return color4f;
        }),
        R"docstring(
        Returns a new Color4f instance initialized with the given values.

        :red: red components
        :green: green components
        :blue: blue components
        :alpha: alpha components
        )docstring",
        py::arg("red"), py::arg("green"), py::arg("blue"),
        py::arg("alpha") = 1.0f)
    .def(py::init(
        [] (py::tuple tuple) {
            SkColor4f color4f = {1.0f, 1.0f, 1.0f, 1.0f};
            if (tuple.size() == 3 || tuple.size() == 4)
                for (size_t i = 0; i < tuple.size(); ++i)
                    color4f[i] = tuple[i].cast<float>();
            else
                throw py::value_error("Tuple must have 3 or 4 elements");
            return color4f;
        }),
        R"docstring(
        Returns a new Color4f instance given (R, G, B) or (R, G, B, A) tuple.
        )docstring",
        py::arg("t"))
    .def(py::self == py::self,
        R"docstring(
        Compares :py:class:`Color4f` with other, and returns true if all
        components are equal.

        :param other: :py:class:`Color4f` to compare
        :return: true if :py:class:`Color4f` equals other
        )docstring",
        py::arg("other"))
    .def(py::self != py::self,
        R"docstring(
        Compares :py:class:`Color4f` with other, and returns true if not all
        components are equal.

        :param other: :py:class:`Color4f` to compare
        :return: true if :py:class:`Color4f` is not equal to other
        )docstring",
        py::arg("other"))
    .def(py::self * float(),
        R"docstring(
        Returns :py:class:`Color4f` multiplied by scale.
        )docstring")
    .def(py::self * py::self,
        R"docstring(
        Returns :py:class:`Color4f` multiplied component-wise by scale.
        )docstring")
    .def("vec",
        [] (SkColor4f& color4f) {
            return py::memoryview::from_buffer(
                color4f.vec(),
                sizeof(float),
                py::format_descriptor<float>::value,
                { 4 },
                { sizeof(float) },
                false
            );
        },
        R"docstring(
        Returns a memoryview to components of :py:class:`Color4f`, for array
        access.
        )docstring")
    .def("__getitem__",
        [] (const SkColor4f& c, int index) {
            if (index < 0 || 4 <= index)
                throw py::index_error("Index out of range.");
            return c[index];
        },
        R"docstring(
        Returns one component.

        Asserts if index is out of range and SK_DEBUG is defined.

        :param index: one of: 0 (fR), 1 (fG), 2 (fB), 3 (fA)
        :return: value corresponding to index
        )docstring",
        py::arg("index"))
    .def("__setitem__",
        [] (SkColor4f& c, int index, float value) {
            if (index < 0 || 4 <= index)
                throw py::index_error("Index out of range.");
            c[index] = value;
        },
        R"docstring(
        Sets one component.

        Asserts if index is out of range and SK_DEBUG is defined.

        :param index: one of: 0 (fR), 1 (fG), 2 (fB), 3 (fA)
        :param value: float value
        )docstring",
        py::arg("index"), py::arg("value"))
    .def("isOpaque", &SkColor4f::isOpaque,
        R"docstring(
        Returns true if :py:class:`Color4f` is an opaque color.

        Asserts if fA is out of range and SK_DEBUG is defined.

        :return: true if :py:class:`Color4f` is opaque
        )docstring")
    .def("fitsInBytes", &SkColor4f::fitsInBytes,
        R"docstring(
        Returns true if all channels are in [0, 1].
        )docstring")
    .def("toColor", &SkColor4f::toSkColor,
        R"docstring(
        Returns closest Color to :py:class:`Color4f`.

        :return: color as Color int
        )docstring")
    .def("__int__", &SkColor4f::toSkColor)
    .def("__len__", [] (const SkColor4f& color4f) { return 4; })
    .def("__repr__",
        [] (const SkColor4f& color4f) {
            std::stringstream s;
            s << "Color4f(" << color4f.fR << ", " << color4f.fG << ", "
                << color4f.fB << ", " << color4f.fA << ")";
            return s.str();
        })
    // .def("premul", &SkColor4f::premul,
    //     "Returns SkRGBA4f premultiplied by alpha.")
    // .def("unpremul", &SkColor4f::unpremul,
    //     "Returns SkRGBA4f unpremultiplied by alpha.")
    .def("toBytes_RGBA", &SkColor4f::toBytes_RGBA)
    .def("makeOpaque", &SkColor4f::makeOpaque)
    .def_static("FromColor", &SkColor4f::FromColor,
        R"docstring(
        Returns closest :py:class:`Color4f` to Color int.

        :param int color: Color with Alpha, red, blue, and green components
            (e.g., 0xFFFFFFFF)
        :return: Color int as :py:class:`Color4f`
        )docstring",
        py::arg("color"))
    // .def_static("FromPMColor", &SkColor4f::FromPMColor,
    //     R"docstring(
    //     Returns closest :py:class:`RGBA4f` to :py:class:`PMColor`.

    //     Only allowed if :py:class:`RGBA4f` is premultiplied.

    //     :return: :py:class:`PMColor` as :py:class:`RGBA4f`
    //     )docstring",
    //     py::arg("pmcolor"))
    .def_static("FromBytes_RGBA", &SkColor4f::FromBytes_RGBA,
        py::arg("rgba"))
    .def_readwrite("fR", &SkColor4f::fR, "red component")
    .def_readwrite("fG", &SkColor4f::fG, "green component")
    .def_readwrite("fB", &SkColor4f::fB, "blue component")
    .def_readwrite("fA", &SkColor4f::fA, "alpha component")
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
    .value("kRed_ColorChannelFlag",
        SkColorChannelFlag::kRed_SkColorChannelFlag)
    .value("kGreen_ColorChannelFlag",
        SkColorChannelFlag::kGreen_SkColorChannelFlag)
    .value("kBlue_ColorChannelFlag",
        SkColorChannelFlag::kBlue_SkColorChannelFlag)
    .value("kAlpha_ColorChannelFlag",
        SkColorChannelFlag::kAlpha_SkColorChannelFlag)
    .value("kGray_ColorChannelFlag",
        SkColorChannelFlag::kGray_SkColorChannelFlag)
    .value("kRG_ColorChannelFlags",
        SkColorChannelFlag::kRG_SkColorChannelFlags)
    .value("kRGB_ColorChannelFlags",
        SkColorChannelFlag::kRGB_SkColorChannelFlags)
    .value("kRGBA_ColorChannelFlags",
        SkColorChannelFlag::kRGBA_SkColorChannelFlags)
    .export_values();


m.def("Color",
    [] (U8CPU r, U8CPU g, U8CPU b, U8CPU a) {
        return SkColorSetARGB(a, r, g, b);
    },
    R"docstring(
    Returns color value from 8-bit component values.

    In Skia, colors are represented as 32-bit ARGB word, where each component
    has 8-bit. Colors can be specified by Python literal or constructed from the
    functions.

    Floating point representation is available via :py:class:`Color4f`.

    Example::

        c = 0xFFFF0000  # ARGB
        c = skia.Color(255, 0, 0)  # RGB
        c = skia.Color(255, 0, 0, 255)  # RGBA
        c = skia.ColorSetRGB(255, 255, 0, 0)  # RGB
        c = skia.ColorSetARGB(255, 255, 0, 0)  # ARGB

    Asserts if SK_DEBUG is defined if a, r, g, or b exceed 255. Since color is
    unpremultiplied, a may be smaller than the largest of r, g, and b.

    :param int r: amount of red, from no red (0) to full red (255)
    :param int g: amount of green, from no green (0) to full green (255)
    :param int b: amount of blue, from no blue (0) to full blue (255)
    :param int a: amount of alpha, from fully transparent (0) to fully opaque
        (255)
    :return: color and alpha, unpremultiplied
    )docstring",
    py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a") = 255);
m.def("ColorSetRGB",
    [] (U8CPU r, U8CPU g, U8CPU b) {
        return SkColorSetARGB(0xFF, r, g, b);
    },
    R"docstring(
    Returns color value from 8-bit component values, with alpha set fully opaque
    to 255.
    )docstring",
    py::arg("r"), py::arg("g"), py::arg("b"));
m.def("ColorGetA",
    [] (SkColor color) { return SkColorGetA(color); },
    R"docstring(
    Returns alpha byte from color value.
    )docstring",
    py::arg("color"));
m.def("ColorGetR",
    [] (SkColor color) { return SkColorGetR(color); },
    R"docstring(
    Returns red component of color, from zero to 255.
    )docstring",
    py::arg("color"));
m.def("ColorGetG",
    [] (SkColor color) { return SkColorGetG(color); },
    R"docstring(
    Returns green component of color, from zero to 255.
    )docstring",
    py::arg("color"));
m.def("ColorGetB",
    [] (SkColor color) { return SkColorGetB(color); },
    R"docstring(
    Returns blue component of color, from zero to 255.
    )docstring",
    py::arg("color"));
m.def("ColorSetARGB", &SkColorSetARGB,
    R"docstring(
    Returns color value from 8-bit component values.

    Asserts if SK_DEBUG is defined if a, r, g, or b exceed 255. Since color is
    unpremultiplied, a may be smaller than the largest of r, g, and b.

    :param int a: amount of alpha, from fully transparent (0) to fully opaque
        (255)
    :param int r: amount of red, from no red (0) to full red (255)
    :param int g: amount of green, from no green (0) to full green (255)
    :param int b: amount of blue, from no blue (0) to full blue (255)
    :return: color and alpha, unpremultiplied
    )docstring",
    py::arg("a"), py::arg("r"), py::arg("g"), py::arg("b"));
m.def("ColorSetA", &SkColorSetA,
    R"docstring(
    Returns unpremultiplied color with red, blue, and green set from c; and
    alpha set from a.

    Alpha component of c is ignored and is replaced by a in result.

    :param int c: packed RGB, eight bits per component
    :param int a: alpha: transparent at zero, fully opaque at 255
    :return: color with transparency
    )docstring",
    py::arg("c"), py::arg("a"));
m.def("RGBToHSV",
    [] (U8CPU red, U8CPU green, U8CPU blue) {
        std::vector<SkScalar> hsv(3);
        SkRGBToHSV(red, green, blue, &hsv[0]);
        return hsv;
    },
    R"docstring(
    Converts RGB to its HSV components.

    hsv[0] contains hsv hue, a value from zero to less than 360. hsv[1] contains
    hsv saturation, a value from zero to one. hsv[2] contains hsv value, a value
    from zero to one.

    :param int red:   red component value from zero to 255
    :param int green: green component value from zero to 255
    :param int blue:  blue component value from zero to 255
    :return: three element array which holds the resulting HSV components
    )docstring",
    py::arg("red"), py::arg("green"), py::arg("blue"));
m.def("ColorToHSV",
    [] (SkColor color) {
        std::vector<SkScalar> hsv(3);
        SkColorToHSV(color, &hsv[0]);
        return hsv;
    },
    R"docstring(
    Converts ARGB to its HSV components.

    Alpha in ARGB is ignored. hsv[0] contains hsv hue, and is assigned a value
    from zero to less than 360. hsv[1] contains hsv saturation, a value from
    zero to one. hsv[2] contains hsv value, a value from zero to one.

    :param int color: ARGB color to convert
    :return: three element array which holds the resulting HSV components
    )docstring",
    py::arg("color"));
m.def("HSVToColor",
    [] (const std::vector<SkScalar>& hsv, U8CPU alpha) {
        if (hsv.size() != 3)
            throw py::value_error("hsv must be 3 element array");
        return SkHSVToColor(alpha, &hsv[0]);
    },
    R"docstring(
    Converts HSV components to an ARGB color.

    Alpha is passed through unchanged. hsv[0] represents hsv hue, an angle from
    zero to less than 360. hsv[1] represents hsv saturation, and varies from
    zero to one. hsv[2] represents hsv value, and varies from zero to one.

    Out of range hsv values are pinned.

    :param hsv: three element array which holds the input HSV components
    :param alpha: alpha component of the returned ARGB color
    :return: ARGB equivalent to HSV
    )docstring",
    py::arg("hsv"), py::arg("alpha") = 255);
m.def("PreMultiplyARGB", &SkPreMultiplyARGB,
    R"docstring(
    Returns a SkPMColor value from unpremultiplied 8-bit component values.

    :param a: amount of alpha, from fully transparent (0) to fully opaque (255)
    :param r: amount of red, from no red (0) to full red (255)
    :param g: amount of green, from no green (0) to full green (255)
    :param b: amount of blue, from no blue (0) to full blue (255)
    :return: premultiplied color
    )docstring",
    py::arg("a"), py::arg("r"), py::arg("g"), py::arg("b"));
m.def("PreMultiplyColor", &SkPreMultiplyColor,
    R"docstring(
    Returns pmcolor closest to color c.

    Multiplies c RGB components by the c alpha, and arranges the bytes to match
    the format of kN32_ColorType.

    :param c: unpremultiplied ARGB color
    :return: premultiplied color
    )docstring",
    py::arg("c"));

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
m.attr("ColorMAGENTA") = SK_ColorMAGENTA;
}
