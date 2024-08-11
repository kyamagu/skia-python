#include "common.h"
#include "modules/skparagraph/include/DartTypes.h"
#include "modules/skparagraph/include/FontCollection.h"
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include <pybind11/stl.h>

void initParagraph(py::module &m) {

py::class_<skia::textlayout::FontCollection, sk_sp<skia::textlayout::FontCollection>, SkRefCnt> font_collection(m, "textlayout_FontCollection");
py::class_<skia::textlayout::ParagraphBuilder> paragraph_builder(m, "textlayout_ParagraphBuilder");
py::class_<skia::textlayout::ParagraphStyle> paragraph_style(m, "textlayout_ParagraphStyle");
py::class_<skia::textlayout::TextStyle> text_style(m, "textlayout_TextStyle");
py::class_<skia::textlayout::Paragraph> paragraph(m, "textlayout_Paragraph");

py::enum_<skia::textlayout::TextAlign>(m, "textlayout_TextAlign", R"docstring(
    )docstring")
    .value("kLeft", skia::textlayout::TextAlign::kLeft)
    .value("kRight", skia::textlayout::TextAlign::kRight)
    .value("kCenter", skia::textlayout::TextAlign::kCenter)
    .value("kJustify", skia::textlayout::TextAlign::kJustify)
    .value("kStart", skia::textlayout::TextAlign::kStart)
    .value("kEnd", skia::textlayout::TextAlign::kEnd)
    .export_values();

py::enum_<skia::textlayout::TextDecoration>(m, "textlayout_TextDecoration", R"docstring(
    )docstring")
    .value("kNoDecoration", skia::textlayout::TextDecoration::kNoDecoration)
    .value("kUnderline", skia::textlayout::TextDecoration::kUnderline)
    .value("kOverline", skia::textlayout::TextDecoration::kOverline)
    .value("kLineThrough", skia::textlayout::TextDecoration::kLineThrough)
    .export_values();

py::enum_<skia::textlayout::TextDecorationStyle>(m, "textlayout_TextDecorationStyle", R"docstring(
    )docstring")
    .value("kSolid", skia::textlayout::TextDecorationStyle::kSolid)
    .value("kDouble", skia::textlayout::TextDecorationStyle::kDouble)
    .value("kDotted", skia::textlayout::TextDecorationStyle::kDotted)
    .value("kDashed", skia::textlayout::TextDecorationStyle::kDashed)
    .value("kWavy", skia::textlayout::TextDecorationStyle::kWavy)
    .export_values();

py::enum_<skia::textlayout::TextDecorationMode>(m, "textlayout_TextDecorationMode", R"docstring(
    )docstring")
    .value("kGaps", skia::textlayout::TextDecorationMode::kGaps)
    .value("kThrough", skia::textlayout::TextDecorationMode::kThrough)
    .export_values();

paragraph_builder
    .def(py::init(
        [] (const skia::textlayout::ParagraphStyle& style,
            sk_sp<skia::textlayout::FontCollection> fontCollection,
            sk_sp<SkUnicode> unicode) {
                return skia::textlayout::ParagraphBuilder::make(style, fontCollection, unicode);
        }),
        R"docstring(
        )docstring",
        py::arg("style"), py::arg("fontCollection"), py::arg("unicode"))
    .def_static("make",
        py::overload_cast<skia::textlayout::ParagraphStyle const&, sk_sp<skia::textlayout::FontCollection>, sk_sp<SkUnicode>>(&skia::textlayout::ParagraphBuilder::make),
        R"docstring(
        )docstring",
        py::arg("style"), py::arg("fontCollection"), py::arg("unicode"))
    .def("addText",
        py::overload_cast<const char*>(&skia::textlayout::ParagraphBuilder::addText),
        R"docstring(
        )docstring",
        py::arg("text"))
    .def("Build", &skia::textlayout::ParagraphBuilder::Build)
    ;

paragraph_style
    .def(py::init())
    .def("setTextStyle",
        py::overload_cast<const skia::textlayout::TextStyle&>(&skia::textlayout::ParagraphStyle::setTextStyle),
        R"docstring(
        )docstring",
        py::arg("textstyle"))
    .def("setTextAlign",
        py::overload_cast<skia::textlayout::TextAlign>(&skia::textlayout::ParagraphStyle::setTextAlign),
        R"docstring(
        )docstring",
        py::arg("align"))
    ;

font_collection
    .def(py::init())
    .def("setDefaultFontManager",
        py::overload_cast<sk_sp<SkFontMgr>>(&skia::textlayout::FontCollection::setDefaultFontManager),
        R"docstring(
        )docstring",
        py::arg("fontManager"))
    .def("setDefaultFontManager",
        py::overload_cast<sk_sp<SkFontMgr>, const char[]>(&skia::textlayout::FontCollection::setDefaultFontManager),
        R"docstring(
        )docstring",
        py::arg("fontManager"), py::arg("defaultFamilyName"))
    .def("setDefaultFontManager",
        py::overload_cast<sk_sp<SkFontMgr>, const std::vector<SkString>&>(&skia::textlayout::FontCollection::setDefaultFontManager),
        R"docstring(
        )docstring",
        py::arg("fontManager"), py::arg("defaultFamilyNames"))
    ;

text_style
    .def(py::init())
    .def("setForegroundColor",
        py::overload_cast<SkPaint>(&skia::textlayout::TextStyle::setForegroundColor),
        R"docstring(
        )docstring",
        py::arg("paint"))
    .def("setFontFamilies",
        py::overload_cast<std::vector<SkString>>(&skia::textlayout::TextStyle::setFontFamilies),
        R"docstring(
        )docstring",
        py::arg("families"))
    .def("setFontSize",
        py::overload_cast<SkScalar>(&skia::textlayout::TextStyle::setFontSize),
        R"docstring(
        )docstring",
        py::arg("size"))
    .def("setFontStyle",
        py::overload_cast<SkFontStyle>(&skia::textlayout::TextStyle::setFontStyle),
        R"docstring(
        )docstring",
        py::arg("fontStyle"))
    .def("setDecoration",
        py::overload_cast<skia::textlayout::TextDecoration>(&skia::textlayout::TextStyle::setDecoration),
        R"docstring(
        )docstring",
        py::arg("decoration"))
    .def("setDecorationMode",
        py::overload_cast<skia::textlayout::TextDecorationMode>(&skia::textlayout::TextStyle::setDecorationMode),
        R"docstring(
        )docstring",
        py::arg("mode"))
    .def("setDecorationStyle",
        py::overload_cast<skia::textlayout::TextDecorationStyle>(&skia::textlayout::TextStyle::setDecorationStyle),
        R"docstring(
        )docstring",
        py::arg("style"))
    .def("setDecorationColor",
        py::overload_cast<SkColor>(&skia::textlayout::TextStyle::setDecorationColor),
        R"docstring(
        )docstring",
        py::arg("color"))
    .def("setDecorationThicknessMultiplier",
        py::overload_cast<SkScalar>(&skia::textlayout::TextStyle::setDecorationThicknessMultiplier),
        R"docstring(
        )docstring",
        py::arg("m"))
    ;

paragraph
    .def("layout",
        py::overload_cast<SkScalar>(&skia::textlayout::Paragraph::layout),
        R"docstring(
        )docstring",
        py::arg("width"))
    .def("paint",
        py::overload_cast<SkCanvas*, SkScalar, SkScalar>(&skia::textlayout::Paragraph::paint),
        R"docstring(
        )docstring",
        py::arg("canvas"), py::arg("x"), py::arg("y"))
    ;

py::object SimpleNamespace = py::module_::import("types").attr("SimpleNamespace");
m.attr("textlayout") = SimpleNamespace();
m.attr("textlayout").attr("FontCollection") = m.attr("textlayout_FontCollection");
m.attr("textlayout").attr("ParagraphBuilder") = m.attr("textlayout_ParagraphBuilder");
m.attr("textlayout").attr("ParagraphStyle") = m.attr("textlayout_ParagraphStyle");
m.attr("textlayout").attr("Paragraph") = m.attr("textlayout_Paragraph");
m.attr("textlayout").attr("TextStyle") = m.attr("textlayout_TextStyle");
m.attr("textlayout").attr("TextAlign") = m.attr("textlayout_TextAlign");
m.attr("textlayout").attr("TextDecoration") = m.attr("textlayout_TextDecoration");
m.attr("textlayout").attr("TextDecorationStyle") = m.attr("textlayout_TextDecorationStyle");
m.attr("textlayout").attr("TextDecorationMode") = m.attr("textlayout_TextDecorationMode");
}
