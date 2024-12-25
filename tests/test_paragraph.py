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


@pytest.fixture(scope='session')
def typeface_font_provider():
    return skia.textlayout.TypefaceFontProvider()

def test_textlayout_TypefaceFontProvider_init(typeface_font_provider):
    assert isinstance(typeface_font_provider, skia.textlayout_TypefaceFontProvider)
    assert typeface_font_provider.countFamilies() == 0

def test_textlayout_TypefaceFontProvider_registerTypeface0(typeface_font_provider):
    typeface_any = skia.Typeface("Text")
    assert typeface_font_provider.registerTypeface(typeface_any) == 1
    assert typeface_font_provider.countFamilies() == 1
    typeface_any = skia.Typeface("Emoji")
    assert typeface_font_provider.registerTypeface(typeface_any) == 1
    assert typeface_font_provider.countFamilies() == 2
    # TypefaceFontProvider can detect duplicates.
    typeface_any_two = skia.Typeface("Text")
    assert typeface_font_provider.registerTypeface(typeface_any_two) == 1
    assert typeface_font_provider.countFamilies() == 2

def test_textlayout_TypefaceFontProvider_registerTypeface1(typeface_font_provider):
    typeface_any = skia.Typeface("Text")
    assert typeface_font_provider.registerTypeface(typeface_any, "Not Text") == 1
    assert typeface_font_provider.countFamilies() == 3
    typeface_any = skia.Typeface("Emoji")
    assert typeface_font_provider.registerTypeface(typeface_any, "Not Emoji") == 1
    assert typeface_font_provider.countFamilies() == 4
