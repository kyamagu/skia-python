#include "common.h"
#include "modules/skparagraph/include/DartTypes.h"
#include "modules/skparagraph/include/FontCollection.h"
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/ParagraphStyle.h"

void initParagraph(py::module &m) {

py::class_<skia::textlayout::FontCollection, sk_sp<skia::textlayout::FontCollection>, SkRefCnt> font_collection(m, "textlayout.FontCollection");
py::class_<skia::textlayout::ParagraphBuilder> paragraph_builder(m, "textlayout.ParagraphBuilder");
py::class_<skia::textlayout::ParagraphStyle> paragraph_style(m, "textlayout.ParagraphStyle");
py::class_<skia::textlayout::TextStyle> text_style(m, "textlayout.TextStyle");

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
    ;

paragraph_style
    .def(py::init())
    ;

font_collection
    .def(py::init())
    .def("setDefaultFontManager",
        py::overload_cast<sk_sp<SkFontMgr>>(&skia::textlayout::FontCollection::setDefaultFontManager),
        R"docstring(
        )docstring",
        py::arg("fontManager"))
    ;
}
