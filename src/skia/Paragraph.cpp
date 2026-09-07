#include "common.h"
#include "modules/skparagraph/include/DartTypes.h"
#include "modules/skparagraph/include/FontCollection.h"
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/TypefaceFontProvider.h"
#include <pybind11/stl.h>

using namespace skia::textlayout;

void initParagraph(py::module &m) {

py::class_<FontCollection, sk_sp<FontCollection>, SkRefCnt> font_collection(m, "textlayout_FontCollection");
py::class_<ParagraphBuilder> paragraph_builder(m, "textlayout_ParagraphBuilder");
py::class_<ParagraphStyle> paragraph_style(m, "textlayout_ParagraphStyle");
py::class_<StrutStyle> strut_style(m, "textlayout_StrutStyle");
py::class_<TextStyle> text_style(m, "textlayout_TextStyle");
py::class_<Paragraph> paragraph(m, "textlayout_Paragraph");
py::class_<TypefaceFontProvider, sk_sp<TypefaceFontProvider>, SkFontMgr> typeface_font_provider(m, "textlayout_TypefaceFontProvider");

py::enum_<TextAlign>(m, "textlayout_TextAlign", R"docstring(
    How lines are aligned horizontally within the layout width.
    )docstring")
    .value("kLeft", TextAlign::kLeft)
    .value("kRight", TextAlign::kRight)
    .value("kCenter", TextAlign::kCenter)
    .value("kJustify", TextAlign::kJustify)
    .value("kStart", TextAlign::kStart)
    .value("kEnd", TextAlign::kEnd)
    .export_values();

py::enum_<TextDecoration>(m, "textlayout_TextDecoration", R"docstring(
    Lines drawn on the text.

    Values can be combined, so underline together with overline is
    ``kUnderline | kOverline``.
    )docstring")
    .value("kNoDecoration", TextDecoration::kNoDecoration)
    .value("kUnderline", TextDecoration::kUnderline)
    .value("kOverline", TextDecoration::kOverline)
    .value("kLineThrough", TextDecoration::kLineThrough)
    .value("kUnderlineOverline", TextDecoration(TextDecoration::kUnderline | TextDecoration::kOverline))
    .value("kUnderlineLineThrough", TextDecoration(TextDecoration::kUnderline | TextDecoration::kLineThrough))
    .value("kOverlineLineThrough", TextDecoration(TextDecoration::kOverline | TextDecoration::kLineThrough))
    .value("kUnderlineOverlineLineThrough", TextDecoration(TextDecoration::kUnderline | TextDecoration::kOverline | TextDecoration::kLineThrough))
    .export_values();

py::enum_<TextDecorationStyle>(m, "textlayout_TextDecorationStyle", R"docstring(
    The line style used to draw text decorations.
    )docstring")
    .value("kSolid", TextDecorationStyle::kSolid)
    .value("kDouble", TextDecorationStyle::kDouble)
    .value("kDotted", TextDecorationStyle::kDotted)
    .value("kDashed", TextDecorationStyle::kDashed)
    .value("kWavy", TextDecorationStyle::kWavy)
    .export_values();

py::enum_<TextDecorationMode>(m, "textlayout_TextDecorationMode", R"docstring(
    Whether an underline is interrupted where descenders cross it
    (``kGaps``) or drawn straight through them (``kThrough``).
    )docstring")
    .value("kGaps", TextDecorationMode::kGaps)
    .value("kThrough", TextDecorationMode::kThrough)
    .export_values();

paragraph_builder
    .def(py::init(
        [] (const ParagraphStyle& style,
            sk_sp<FontCollection> fontCollection,
            sk_sp<SkUnicode> unicode) {
                return ParagraphBuilder::make(style, fontCollection, unicode);
        }),
        R"docstring(
        Creates a paragraph builder.

        :param skia.textlayout.ParagraphStyle style: default style for the
            paragraph
        :param skia.textlayout.FontCollection fontCollection: collection used
            to resolve the fonts named by the text styles
        :param skia.Unicodes unicode: Unicode implementation used for word and
            line breaking, for example ``skia.Unicodes.ICU.Make()``
        :return: the new :py:class:`textlayout.ParagraphBuilder`
        )docstring",
        py::arg("style"), py::arg("fontCollection"), py::arg("unicode"))
    .def_static("make",
        py::overload_cast<ParagraphStyle const&, sk_sp<FontCollection>, sk_sp<SkUnicode>>(&ParagraphBuilder::make),
        R"docstring(
        Creates a paragraph builder.

        :param skia.textlayout.ParagraphStyle style: default style for the
            paragraph
        :param skia.textlayout.FontCollection fontCollection: collection used
            to resolve the fonts named by the text styles
        :param skia.Unicodes unicode: Unicode implementation used for word and
            line breaking, for example ``skia.Unicodes.ICU.Make()``
        :return: the new :py:class:`textlayout.ParagraphBuilder`
        )docstring",
        py::arg("style"), py::arg("fontCollection"), py::arg("unicode"))
    .def("addText",
        py::overload_cast<const char*>(&ParagraphBuilder::addText),
        R"docstring(
        Adds UTF-8 encoded text to the builder.

        The text is laid out with the top-most style on the style stack.

        :param str text: text to append
        )docstring",
        py::arg("text"))
    .def("pop", &ParagraphBuilder::pop,
        R"docstring(
        Removes the top-most style from the style stack.

        Text added afterwards uses the style beneath it, so pushing and popping
        lets a single paragraph mix styles::

            builder.pushStyle(normal)
            builder.addText('Hello this is normal. ')
            builder.pushStyle(bold)
            builder.addText('And this is BOLD. ')
            builder.pop()
            builder.addText(' Back to normal again.')
        )docstring")
    .def("pushStyle",
        py::overload_cast<const TextStyle&>(&ParagraphBuilder::pushStyle),
        R"docstring(
        Pushes a style onto the style stack.

        Text added afterwards with :py:meth:`addText` uses the top-most style
        on the stack. Use :py:meth:`pop` to return to the previous style.

        :param skia.textlayout.TextStyle style: style to apply to following text
        )docstring",
        py::arg("style"))
    .def("Build", &ParagraphBuilder::Build,
        R"docstring(
        Builds the paragraph from the text and styles added so far.

        :return: the resulting :py:class:`textlayout.Paragraph`. Call
            :py:meth:`Paragraph.layout` on it before querying its metrics or
            painting it.
        )docstring")
    ;

strut_style
    .def(py::init())
    .def("setStrutEnabled",
        py::overload_cast<const bool>(&StrutStyle::setStrutEnabled),
        R"docstring(
        Enables the strut.

        The strut is a minimum line box: when enabled, every line is at least
        as tall as the strut, regardless of the fonts actually used on it.

        :param bool strutenabled: whether the strut is applied
        )docstring",
        py::arg("strutenabled"))
    .def("setLeading",
        py::overload_cast<const SkScalar>(&StrutStyle::setLeading),
        R"docstring(
        Sets the strut's leading as a multiple of the font size.

        A negative value is treated as no extra leading.

        :param float leading: leading as a multiple of the font size
        )docstring",
        py::arg("leading"))
    .def("setHeight",
        py::overload_cast<const SkScalar>(&StrutStyle::setHeight),
        R"docstring(
        Sets the strut's line height as a multiple of the font size.

        Only applied when the height override is enabled with
        :py:meth:`setHeightOverride`; otherwise the strut font's own metrics
        determine its height and this value is ignored.

        :param float height: line height as a multiple of the font size
        )docstring",
        py::arg("height"))
    .def("setHeightOverride",
        py::overload_cast<const bool>(&StrutStyle::setHeightOverride),
        R"docstring(
        Enables the line height set by :py:meth:`setHeight`.

        When disabled, the strut font's own ascent and descent determine the
        strut height, together with the leading set by :py:meth:`setLeading`.

        :param bool heightoverride: whether to use the height multiple
        )docstring",
        py::arg("heightoverride"))
    ;

paragraph_style
    .def(py::init())
    .def("setTextStyle",
        py::overload_cast<const TextStyle&>(&ParagraphStyle::setTextStyle),
        R"docstring(
        Sets the default text style for the paragraph.

        Text added without an explicit :py:meth:`ParagraphBuilder.pushStyle`
        uses this style.

        :param skia.textlayout.TextStyle textstyle: the default text style
        )docstring",
        py::arg("textstyle"))
    .def("setTextAlign",
        py::overload_cast<TextAlign>(&ParagraphStyle::setTextAlign),
        R"docstring(
        Sets how lines are aligned horizontally within the layout width.

        :param skia.textlayout.TextAlign align: the alignment to apply
        )docstring",
        py::arg("align"))
    .def("setStrutStyle",
        py::overload_cast<StrutStyle>(&ParagraphStyle::setStrutStyle),
        R"docstring(
        Sets the strut, a minimum line box applied to every line.

        The strut only takes effect if it is enabled with
        :py:meth:`StrutStyle.setStrutEnabled`.

        :param skia.textlayout.StrutStyle strutstyle: the strut to apply
        )docstring",
        py::arg("strutstyle"))
    ;

font_collection
    .def(py::init())
    .def("setDefaultFontManager",
        py::overload_cast<sk_sp<SkFontMgr>>(&FontCollection::setDefaultFontManager),
        R"docstring(
        Sets the font manager used to resolve font families.

        :param skia.FontMgr fontManager: font manager to resolve families and
            supply fallback fonts
        )docstring",
        py::arg("fontManager"))
    .def("setDefaultFontManager",
        py::overload_cast<sk_sp<SkFontMgr>, const char[]>(&FontCollection::setDefaultFontManager),
        R"docstring(
        Sets the font manager and the family to fall back on.

        :param skia.FontMgr fontManager: font manager to resolve families and
            supply fallback fonts
        :param str defaultFamilyName: family used when a requested family
            cannot be resolved
        )docstring",
        py::arg("fontManager"), py::arg("defaultFamilyName"))
    .def("setDefaultFontManager",
        py::overload_cast<sk_sp<SkFontMgr>, const std::vector<SkString>&>(&FontCollection::setDefaultFontManager),
        R"docstring(
        Sets the font manager and the families to fall back on.

        :param skia.FontMgr fontManager: font manager to resolve families and
            supply fallback fonts
        :param List[str] defaultFamilyNames: families tried in order when a
            requested family cannot be resolved
        )docstring",
        py::arg("fontManager"), py::arg("defaultFamilyNames"))
    ;

text_style
    .def(py::init())
    .def("cloneForPlaceholder", &TextStyle::cloneForPlaceholder,
        R"docstring(
        Returns a copy of this style marked as a placeholder.

        Only some attributes are carried over, among them the color, font
        size, font families and decoration; others such as the locale, letter
        spacing and word spacing are left at their defaults.
        )docstring")
    .def("setColor",
        py::overload_cast<SkColor>(&TextStyle::setColor),
        R"docstring(
        Sets the color the text is filled with.

        :param int color: the text color
        )docstring",
        py::arg("color"))
    .def("setForegroundColor",
        py::overload_cast<SkPaint>(&TextStyle::setForegroundColor),
        R"docstring(
        DEPRECATED: Use :py:meth:`setForegroundPaint`
        )docstring",
        py::arg("paint"))
    .def("setForegroundPaint",
        py::overload_cast<SkPaint>(&TextStyle::setForegroundPaint),
        R"docstring(
        Sets the paint the text is drawn with.

        Use this instead of :py:meth:`setColor` when the text needs a shader,
        a stroke style or any other paint attribute.

        :param skia.Paint paint: paint used to draw the glyphs
        )docstring",
        py::arg("paint"))
    .def("getFontFamilies",
        &TextStyle::getFontFamilies,
        R"docstring(
        Returns the font families this style asks for, in priority order.

        :return: the requested family names
        )docstring")
    .def("setFontFamilies",
        py::overload_cast<std::vector<SkString>>(&TextStyle::setFontFamilies),
        R"docstring(
        Sets the font families to use, in priority order.

        The first family that can supply a glyph is used for it, so later
        entries act as fallbacks.

        :param List[str] families: family names in priority order
        )docstring",
        py::arg("families"))
    .def("getFontSize",
        &TextStyle::getFontSize,
        R"docstring(
        Returns the font size.

        :return: the em size, in the same units as the layout
        )docstring")
    .def("setFontSize",
        py::overload_cast<SkScalar>(&TextStyle::setFontSize),
        R"docstring(
        Sets the font size.

        :param float size: the em size, in the same units as the layout
        )docstring",
        py::arg("size"))
    .def("setFontStyle",
        py::overload_cast<SkFontStyle>(&TextStyle::setFontStyle),
        R"docstring(
        Sets the weight, width and slant used to select a typeface.

        :param skia.FontStyle fontStyle: the style to match
        )docstring",
        py::arg("fontStyle"))
    .def("setLocale",
        py::overload_cast<const SkString&>(&TextStyle::setLocale),
        R"docstring(
        Sets the locale used when a fallback font is chosen.

        Affects, for example, which font supplies the Han characters shared
        between Chinese, Japanese and Korean.

        :param str locale: BCP 47 language tag
        )docstring",
        py::arg("locale"))
    .def("setLetterSpacing",
        py::overload_cast<SkScalar>(&TextStyle::setLetterSpacing),
        R"docstring(
        Sets extra space added after every glyph, in pixels.

        Has no effect on cursive scripts such as Arabic or Syriac, whose
        glyphs are joined; other right-to-left scripts such as Hebrew are
        spaced normally.

        :param float letterspacing: extra space per glyph; may be negative to
            tighten the text
        )docstring",
        py::arg("letterspacing"))
    .def("setWordSpacing",
        py::overload_cast<SkScalar>(&TextStyle::setWordSpacing),
        R"docstring(
        Sets extra space added at every word boundary, in pixels.

        :param float wordspacing: extra space per word break; may be negative
            to tighten the text
        )docstring",
        py::arg("wordspacing"))
    .def("setDecoration",
        py::overload_cast<TextDecoration>(&TextStyle::setDecoration),
        R"docstring(
        Sets which decorations are drawn, such as underline or overline.

        Decorations can be combined, so underline and overline together is
        ``kUnderline | kOverline``.

        :param skia.textlayout.TextDecoration decoration: decorations to draw
        )docstring",
        py::arg("decoration"))
    .def("setDecorationMode",
        py::overload_cast<TextDecorationMode>(&TextStyle::setDecorationMode),
        R"docstring(
        Sets whether an underline is interrupted where descenders cross it.

        Only affects underlines; other decorations are drawn the same way
        either way.

        :param skia.textlayout.TextDecorationMode mode: kGaps or kThrough
        )docstring",
        py::arg("mode"))
    .def("setDecorationStyle",
        py::overload_cast<TextDecorationStyle>(&TextStyle::setDecorationStyle),
        R"docstring(
        Sets the line style used to draw decorations.

        :param skia.textlayout.TextDecorationStyle style: solid, double,
            dotted, dashed or wavy
        )docstring",
        py::arg("style"))
    .def("setDecorationColor",
        py::overload_cast<SkColor>(&TextStyle::setDecorationColor),
        R"docstring(
        Sets the color decorations are drawn in.

        :param int color: the decoration color
        )docstring",
        py::arg("color"))
    .def("setDecorationThicknessMultiplier",
        py::overload_cast<SkScalar>(&TextStyle::setDecorationThicknessMultiplier),
        R"docstring(
        Scales the decoration line thickness.

        The value multiplies the font's own default thickness, so 1.0 leaves
        it unchanged.

        :param float m: multiplier applied to the default thickness
        )docstring",
        py::arg("m"))
    .def("getHeight",
        &TextStyle::getHeight,
        R"docstring(
        Returns the line height as a multiple of the font size.

        :return: zero when the height override is disabled, otherwise the
            multiple set by :py:meth:`setHeight`
        )docstring")
    .def("setHeight",
        py::overload_cast<SkScalar>(&TextStyle::setHeight),
        R"docstring(
        Sets the line height as a multiple of the font size.

        Only applied when the height override is enabled with
        :py:meth:`setHeightOverride`; otherwise the font's own metrics
        determine the line height and this value is ignored. Together they
        give the CSS ``line-height`` behaviour.

        :param float height: line height as a multiple of the font size
        )docstring",
        py::arg("height"))
    .def("getHeightOverride",
        &TextStyle::getHeightOverride,
        R"docstring(
        Returns whether the line height set by :py:meth:`setHeight` is used.

        :return: True if the height multiple overrides the font's metrics
        )docstring")
    .def("setHeightOverride",
        py::overload_cast<bool>(&TextStyle::setHeightOverride),
        R"docstring(
        Enables the line height set by :py:meth:`setHeight`.

        When disabled, the font's own ascent, descent and leading determine
        the line height.

        :param bool heightoverride: whether to use the height multiple
        )docstring",
        py::arg("heightoverride"))
    .def("getHalfLeading",
        &TextStyle::getHalfLeading,
        R"docstring(
        Returns whether leading is split evenly above and below the text.

        :return: True if half leading is enabled
        )docstring")
    .def("setHalfLeading",
        py::overload_cast<bool>(&TextStyle::setHalfLeading),
        R"docstring(
        Splits the extra leading evenly above and below the text.

        Only takes effect when the height override is enabled with
        :py:meth:`setHeightOverride`. When disabled, the ascent and descent
        are scaled by the height multiple instead, which shifts the text
        within the line box.

        :param bool halfleading: whether to split the leading evenly
        )docstring",
        py::arg("halfleading"))
    .def("getBaselineShift",
        &TextStyle::getBaselineShift,
        R"docstring(
        Returns the baseline shift in pixels.

        :return: the shift set by :py:meth:`setBaselineShift`
        )docstring")
    .def("setBaselineShift",
        py::overload_cast<SkScalar>(&TextStyle::setBaselineShift),
        R"docstring(
        Shifts the text off its baseline, in pixels.

        Positive values lower the text and negative values raise it, so a
        superscript takes a negative shift and a subscript a positive one.
        The line box grows to keep the shifted text inside it.

        :param float baselineshift: distance to lower the text
        )docstring",
        py::arg("baselineshift"))
    ;

paragraph
    .def_property_readonly("Width", &Paragraph::getMaxWidth,
        R"docstring(
        The width the paragraph was laid out to.

        This is the value passed to :py:meth:`layout`, not the width of the
        text; use :py:attr:`LongestLine` for that.
        )docstring")
    .def_property_readonly("Height", &Paragraph::getHeight,
        R"docstring(
        The total height of the laid-out paragraph.
        )docstring")
    .def_property_readonly("MinIntrinsicWidth", &Paragraph::getMinIntrinsicWidth,
        R"docstring(
        The narrowest width the text can be laid out to without breaking a
        word.
        )docstring")
    .def_property_readonly("MaxIntrinsicWidth", &Paragraph::getMaxIntrinsicWidth,
        R"docstring(
        The width the text would occupy on a single unwrapped line.
        )docstring")
    .def_property_readonly("AlphabeticBaseline", &Paragraph::getAlphabeticBaseline,
        R"docstring(
        The distance from the top of the paragraph to the alphabetic
        baseline of the first line.
        )docstring")
    .def_property_readonly("IdeographicBaseline", &Paragraph::getIdeographicBaseline,
        R"docstring(
        The distance from the top of the paragraph to the ideographic
        baseline of the first line.
        )docstring")
    .def_property_readonly("LongestLine", &Paragraph::getLongestLine,
        R"docstring(
        The width of the widest line actually laid out.

        Never more than :py:attr:`Width`; long words are broken to fit.
        )docstring")
    .def_property_readonly("ExceedMaxLines", &Paragraph::didExceedMaxLines,
        R"docstring(
        Whether the text did not fit within the paragraph style's maximum
        number of lines and was truncated.
        )docstring")
    .def("layout",
        py::overload_cast<SkScalar>(&Paragraph::layout),
        R"docstring(
        Lays the paragraph out to the given width.

        Must be called before the paragraph's metrics are read or it is
        painted, and again after any change that affects layout.

        :param float width: width to wrap the text to
        )docstring",
        py::arg("width"))
    .def("paint",
        py::overload_cast<SkCanvas*, SkScalar, SkScalar>(&Paragraph::paint),
        R"docstring(
        Draws the laid-out paragraph onto a canvas.

        :param skia.Canvas canvas: canvas to draw on
        :param float x: x offset of the paragraph's left edge
        :param float y: y offset of the paragraph's top edge
        )docstring",
        py::arg("canvas"), py::arg("x"), py::arg("y"))
    ;

typeface_font_provider
    .def(py::init())
    .def("registerTypeface",
        py::overload_cast<sk_sp<SkTypeface>>(&TypefaceFontProvider::registerTypeface),
        R"docstring(
        Registers a typeface under its own family name.

        :param skia.Typeface typeface: typeface to make available
        :return: 1 on success, or 0 if the typeface is null or reports no
            family name
        )docstring",
        py::arg("typeface"))
    .def("registerTypeface",
        py::overload_cast<sk_sp<SkTypeface>, const SkString&>(&TypefaceFontProvider::registerTypeface),
        R"docstring(
        Registers a typeface under an alternative family name.

        :param skia.Typeface typeface: typeface to make available
        :param str alias: family name to register it under
        :return: 1 on success, or 0 if the alias is empty
        )docstring",
        py::arg("typeface"), py::arg("alias"))
    ;

py::object SimpleNamespace = py::module_::import("types").attr("SimpleNamespace");
m.attr("textlayout") = SimpleNamespace();
m.attr("textlayout").attr("FontCollection") = m.attr("textlayout_FontCollection");
m.attr("textlayout").attr("ParagraphBuilder") = m.attr("textlayout_ParagraphBuilder");
m.attr("textlayout").attr("ParagraphStyle") = m.attr("textlayout_ParagraphStyle");
m.attr("textlayout").attr("Paragraph") = m.attr("textlayout_Paragraph");
m.attr("textlayout").attr("TypefaceFontProvider") = m.attr("textlayout_TypefaceFontProvider");
m.attr("textlayout").attr("StrutStyle") = m.attr("textlayout_StrutStyle");
m.attr("textlayout").attr("TextStyle") = m.attr("textlayout_TextStyle");
m.attr("textlayout").attr("TextAlign") = m.attr("textlayout_TextAlign");
m.attr("textlayout").attr("TextDecoration") = m.attr("textlayout_TextDecoration");
m.attr("textlayout").attr("TextDecorationStyle") = m.attr("textlayout_TextDecorationStyle");
m.attr("textlayout").attr("TextDecorationMode") = m.attr("textlayout_TextDecorationMode");
}
