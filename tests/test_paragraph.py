import skia
import pytest

@pytest.fixture(scope='session')
def textlayout_font_collection():
    return skia.textlayout.FontCollection()

def test_FontCollection_init0(textlayout_font_collection):
    assert isinstance(textlayout_font_collection, skia.textlayout_FontCollection)


@pytest.fixture(scope='session')
def paragraph_style():
    return skia.textlayout.ParagraphStyle()

def test_ParagraphStyle_init0(paragraph_style):
    assert isinstance(paragraph_style, skia.textlayout_ParagraphStyle)


@pytest.fixture(scope='session')
def strut_style():
    return skia.textlayout.StrutStyle()

def test_StrutStyle_init0(strut_style):
    assert isinstance(strut_style, skia.textlayout_StrutStyle)


@pytest.fixture(scope='session')
def textlayout_text_style():
    return skia.textlayout.TextStyle()

def test_TextStyle_init0(textlayout_text_style):
    assert isinstance(textlayout_text_style, skia.textlayout_TextStyle)


@pytest.fixture(scope='session')
def paragraph_builder():
    return skia.textlayout.ParagraphBuilder.make(skia.textlayout.ParagraphStyle(),
                                                 skia.textlayout.FontCollection(),
                                                 skia.Unicodes.ICU.Make())

def test_ParagraphBuilder_init0(paragraph_builder):
    assert isinstance(paragraph_builder, skia.textlayout_ParagraphBuilder)

def test_Paragraph_init0(paragraph_builder):
    paragraph_builder.addText("")
    paragraph = paragraph_builder.Build()
    assert isinstance(paragraph, skia.textlayout_Paragraph)


# Adapted from #278, to make sure that "\n" results in a line break  (rather that .notdef).
# Height is larger than twice width, if a break happens.
def test_Paragraph_linebreak(paragraph_builder, textlayout_text_style, textlayout_font_collection, paragraph_style):
    paint = skia.Paint()
    paint.setColor(skia.ColorBLACK)
    paint.setAntiAlias(True)

    textlayout_text_style.setFontSize(50)
    textlayout_text_style.setForegroundPaint(paint)

    textlayout_font_collection.setDefaultFontManager(skia.FontMgr())

    builder = skia.textlayout.ParagraphBuilder.make(
        paragraph_style, textlayout_font_collection, skia.Unicodes.ICU.Make()
    )
    builder.pushStyle(textlayout_text_style)

    builder.addText("o\no")
    paragraph = builder.Build()
    paragraph.layout(300)
    assert (paragraph.Height > 0) and (paragraph.Height > paragraph.LongestLine * 2)


def test_Paragraph_strutHeight(paragraph_builder, textlayout_text_style, textlayout_font_collection, paragraph_style, strut_style):
    paint = skia.Paint()
    paint.setColor(skia.ColorBLACK)
    paint.setAntiAlias(True)

    textlayout_text_style.setFontSize(50)
    textlayout_text_style.setForegroundPaint(paint)

    textlayout_font_collection.setDefaultFontManager(skia.FontMgr())

    def graf_with_strut(enabled, leading_factor):
        strut_style.setStrutEnabled(enabled)
        strut_style.setLeading(leading_factor)
        paragraph_style.setStrutStyle(strut_style)

        builder = skia.textlayout.ParagraphBuilder.make(
            paragraph_style, textlayout_font_collection, skia.Unicodes.ICU.Make()
        )
        builder.pushStyle(textlayout_text_style)

        builder.addText("o\no")
        paragraph = builder.Build()
        paragraph.layout(300)

        return paragraph
    
    nostrut_height = graf_with_strut(False, 1.0).Height
    assert nostrut_height > 0

    two_x_strut_height = graf_with_strut(True, 2.0).Height
    assert two_x_strut_height > nostrut_height

    three_x_strut_height = graf_with_strut(True, 3.0).Height
    assert three_x_strut_height > two_x_strut_height

    # Anything < 1 seems to have no effect

    half_strut_height = graf_with_strut(True, 0.5).Height
    assert half_strut_height == nostrut_height

    zero_x_strut_height = graf_with_strut(True, 0).Height
    assert zero_x_strut_height == nostrut_height


def test_Paragraph_letterSpacing(paragraph_builder, textlayout_text_style, textlayout_font_collection, paragraph_style, strut_style):
    paint = skia.Paint()
    paint.setColor(skia.ColorBLACK)
    paint.setAntiAlias(True)

    textlayout_font_collection.setDefaultFontManager(skia.FontMgr())

    def graf_with_letterspacing(letterspacing):
        textlayout_text_style.setFontSize(50)
        textlayout_text_style.setForegroundPaint(paint)
        textlayout_text_style.setLetterSpacing(letterspacing)

        builder = skia.textlayout.ParagraphBuilder.make(
            paragraph_style, textlayout_font_collection, skia.Unicodes.ICU.Make()
        )
        builder.pushStyle(textlayout_text_style)

        builder.addText("ooo")
        paragraph = builder.Build()
        paragraph.layout(300)

        return paragraph
    
    negative_one_x_letter_spacing = graf_with_letterspacing(-1.0).LongestLine
    zero_x_letter_spacing = graf_with_letterspacing(0.0).LongestLine
    one_x_letter_spacing = graf_with_letterspacing(1.0).LongestLine
    two_x_letter_spacing = graf_with_letterspacing(2.0).LongestLine
    three_x_letter_spacing = graf_with_letterspacing(3.0).LongestLine

    assert zero_x_letter_spacing > negative_one_x_letter_spacing
    assert one_x_letter_spacing > zero_x_letter_spacing
    assert two_x_letter_spacing > one_x_letter_spacing
    assert three_x_letter_spacing > two_x_letter_spacing


def test_Paragraph_wordSpacing(paragraph_builder, textlayout_text_style, textlayout_font_collection, paragraph_style, strut_style):
    paint = skia.Paint()
    paint.setColor(skia.ColorBLACK)
    paint.setAntiAlias(True)

    textlayout_font_collection.setDefaultFontManager(skia.FontMgr())

    def graf_with_word_spacing(letterspacing):
        textlayout_text_style.setFontSize(50)
        textlayout_text_style.setForegroundPaint(paint)
        textlayout_text_style.setWordSpacing(letterspacing)

        builder = skia.textlayout.ParagraphBuilder.make(
            paragraph_style, textlayout_font_collection, skia.Unicodes.ICU.Make()
        )
        builder.pushStyle(textlayout_text_style)

        builder.addText("word word word")
        paragraph = builder.Build()
        paragraph.layout(300)

        return paragraph
    
    negative_one_x_word_spacing = graf_with_word_spacing(-1.0).LongestLine
    zero_x_word_spacing = graf_with_word_spacing(0.0).LongestLine
    one_x_word_spacing = graf_with_word_spacing(1.0).LongestLine
    two_x_word_spacing = graf_with_word_spacing(2.0).LongestLine
    three_x_word_spacing = graf_with_word_spacing(3.0).LongestLine

    assert zero_x_word_spacing > negative_one_x_word_spacing
    assert one_x_word_spacing > zero_x_word_spacing
    assert two_x_word_spacing > one_x_word_spacing
    assert three_x_word_spacing > two_x_word_spacing