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

font_collection
    .def(py::init())
    .def("setDefaultFontManager",
        py::overload_cast<sk_sp<SkFontMgr>>(&skia::textlayout::FontCollection::setDefaultFontManager),
        R"docstring(
        )docstring",
        py::arg("fontManager"))
    ;
}
