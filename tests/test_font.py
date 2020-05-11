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


def test_Typeface_unicharsToGlyphs(typeface):
    assert isinstance(typeface.unicharsToGlyphs([ord('b'), ord('x')]), list)


def test_Typeface_unicharToGlyph(typeface):
    assert isinstance(typeface.unicharToGlyph(ord('b')), int)


def test_Typeface_countGlyphs(typeface):
    assert isinstance(typeface.countGlyphs(), int)


def test_Typeface_countTables(typeface):
    assert isinstance(typeface.countTables(), int)


def test_Typeface_getTableTags(typeface):
    assert isinstance(typeface.getTableTags(), list)


def test_Typeface_getTableSize(typeface):
    assert isinstance(typeface.getTableSize(0), int)


def test_Typeface_getTableData(typeface):
    assert isinstance(typeface.getTableData(0), bytes)


def test_Typeface_copyTableData(typeface):
    assert isinstance(typeface.copyTableData(0), (skia.Data, type(None)))


def test_Typeface_getUnitsPerEm(typeface):
    assert isinstance(typeface.getUnitsPerEm(), int)


def test_Typeface_getKerningPairAdjustments(typeface):
    assert isinstance(
        typeface.getKerningPairAdjustments([0]), (list, type(None)))


def test_Typeface_getFamilyNames(typeface):
    result = typeface.getFamilyNames()
    assert isinstance(result, list)
    for item in result:
        assert isinstance(item, tuple)


def test_Typeface_getFamilyName(typeface):
    assert isinstance(typeface.getFamilyName(), str)


def test_Typeface_getBounds(typeface):
    assert isinstance(typeface.getBounds(), skia.Rect)


def test_Typeface_UniqueID(typeface):
    assert isinstance(skia.Typeface.UniqueID(typeface), int)


def test_Typeface_Equal(typeface):
    assert isinstance(skia.Typeface.Equal(typeface, typeface), bool)


def test_Typeface_MakeDefault(typeface):
    assert isinstance(skia.Typeface.MakeDefault(), skia.Typeface)


def test_Typeface_MakeFromName(typeface):
    assert isinstance(
        skia.Typeface.MakeFromName('Arial', skia.FontStyle()), skia.Typeface)


@pytest.mark.parametrize('args', [
    ('foo',),
    ('foo', 0,),
])
def test_Typeface_MakeFromFile(typeface, args):
    assert isinstance(
        skia.Typeface.MakeFromFile(*args), (skia.Typeface, type(None)))


@pytest.mark.parametrize('args', [
    (skia.Data.MakeEmpty(),),
    (skia.Data.MakeEmpty(), 0,),
])
def test_Typeface_MakeFromData(typeface, args):
    assert isinstance(
        skia.Typeface.MakeFromData(*args), (skia.Typeface, type(None)))


def test_Typeface_MakeDeserialize(typeface):
    assert isinstance(
        skia.Typeface.MakeDeserialize(typeface.serialize()), skia.Typeface)


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
