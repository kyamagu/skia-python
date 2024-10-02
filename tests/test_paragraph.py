import skia
import pytest

def test_FontCollection_init0():
    assert isinstance(skia.textlayout.FontCollection(), skia.textlayout_FontCollection)

def test_ParagraphStyle_init0():
    assert isinstance(skia.textlayout.ParagraphStyle(),skia.textlayout_ParagraphStyle)

def test_TextStyle_init0():
    assert isinstance(skia.textlayout.TextStyle(), skia.textlayout_TextStyle)

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
