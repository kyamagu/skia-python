import skia
import pytest


@pytest.fixture
def fontstyle():
    return skia.FontStyle()


def test_FontStyle_weight(fontstyle):
    assert isinstance(fontstyle.weight(), int)


def test_FontStyle_width(fontstyle):
    assert isinstance(fontstyle.width(), int)


def test_FontStyle_slant(fontstyle):
    assert isinstance(fontstyle.slant(), skia.FontStyle.Slant)


def test_FontStyle_Normal(fontstyle):
    assert isinstance(fontstyle.Normal(), skia.FontStyle)


def test_FontStyle_Bold(fontstyle):
    assert isinstance(fontstyle.Bold(), skia.FontStyle)


def test_FontStyle_Italic(fontstyle):
    assert isinstance(fontstyle.Italic(), skia.FontStyle)


def test_FontStyle_BoldItalic(fontstyle):
    assert isinstance(fontstyle.BoldItalic(), skia.FontStyle)


@pytest.fixture
def typeface(fontmgr):
    return fontmgr.matchFamilyStyle('Arial', skia.FontStyle())


def test_Typeface_fontStyle(typeface):
    assert isinstance(typeface.fontStyle(), skia.FontStyle)


def test_Typeface_isBold(typeface):
    assert isinstance(typeface.isBold(), bool)


def test_Typeface_isItalic(typeface):
    assert isinstance(typeface.isItalic(), bool)


def test_Typeface_isFixedPitch(typeface):
    assert isinstance(typeface.isFixedPitch(), bool)


@pytest.mark.skip(reason='Fix me!')
def test_Typeface_getVariationDesignPosition(typeface):
    assert isinstance(typeface.getVariationDesignPosition(), list)


@pytest.mark.skip(reason='Fix me!')
def test_Typeface_getVariationDesignParameters(typeface):
    assert isinstance(typeface.getVariationDesignParameters(), list)


def test_Typeface_uniqueID(typeface):
    assert isinstance(typeface.uniqueID(), int)


def test_Typeface_makeClone(typeface):
    assert isinstance(typeface.makeClone(skia.FontArguments()), skia.Typeface)


def test_Typeface_serialize(typeface):
    assert isinstance(typeface.serialize(), skia.Data)


@pytest.fixture()
def fontmgr():
    return skia.FontMgr.RefDefault()


def test_FontMgr_countFamilies(fontmgr):
    assert isinstance(fontmgr.countFamilies(), int)


def test_FontMgr_getFamilyName(fontmgr):
    assert isinstance(fontmgr.getFamilyName(0), str)


def test_FontMgr_createStyleSet(fontmgr):
    assert isinstance(fontmgr.createStyleSet(0), skia.FontStyleSet)


def test_FontMgr_matchFamily(fontmgr):
    assert isinstance(fontmgr.matchFamily('monospace'), skia.FontStyleSet)


def test_FontMgr_matchFamilyStyle(fontmgr):
    assert isinstance(
        fontmgr.matchFamilyStyle('monospace', skia.FontStyle()),
        (skia.Typeface, type(None)))


def test_FontMgr_matchFamilyStyleCharacter(fontmgr):
    assert isinstance(
        fontmgr.matchFamilyStyleCharacter(
            'monospace', skia.FontStyle(), ['en'], ord('e')),
        (skia.Typeface, type(None)))


def test_FontMgr_matchFaceStyle(fontmgr):
    assert isinstance(
        fontmgr.matchFaceStyle(skia.Typeface.MakeDefault(), skia.FontStyle()),
        (skia.Typeface, type(None)))


def test_FontMgr_makeFromData(fontmgr):
    assert fontmgr.makeFromData(skia.Data.MakeEmpty(), 0) is None


def test_FontMgr_makeFromFile(fontmgr):
    assert fontmgr.makeFromFile('', 0) is None


def test_FontMgr_legacyMakeTypeface(fontmgr):
    assert isinstance(
        fontmgr.legacyMakeTypeface('monospace', skia.FontStyle()),
        skia.Typeface)


def test_FontMgr_unique(fontmgr):
    assert isinstance(fontmgr.unique(), bool)


def test_FontMgr_ref_unref(fontmgr):
    fontmgr.ref()
    fontmgr.unref()


@pytest.fixture
def fontstyleset():
    return skia.FontStyleSet.CreateEmpty()


def test_FontStyleSet_unique(fontstyleset):
    assert isinstance(fontstyleset.unique(), bool)


def test_FontStyleSet_ref_unref(fontstyleset):
    fontstyleset.ref()
    fontstyleset.unref()
