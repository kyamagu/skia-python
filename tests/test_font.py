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
def fontarguments():
    return skia.FontArguments()


def test_FontArguments_setCollectionIndex(fontarguments):
    fontarguments.setCollectionIndex(0)


def test_FontArguments_setVariationDesignPosition(fontarguments):
    coordinates = skia.FontArguments.VariationPosition.Coordinates([
        skia.FontArguments.VariationPosition.Coordinate(0x00, 0.),
        ])
    fontarguments.setVariationDesignPosition(
        skia.FontArguments.VariationPosition(coordinates))


def test_FontArguments_getCollectionIndex(fontarguments):
    assert isinstance(fontarguments.getCollectionIndex(), int)


def test_FontArguments_getVariationDesignPosition(fontarguments):
    assert isinstance(
        fontarguments.getVariationDesignPosition(),
        skia.FontArguments.VariationPosition)


@pytest.fixture
def typeface():
    return skia.Typeface.MakeDefault()


def test_Typeface_fontStyle(typeface):
    assert isinstance(typeface.fontStyle(), skia.FontStyle)


def test_Typeface_isBold(typeface):
    assert isinstance(typeface.isBold(), bool)


def test_Typeface_isItalic(typeface):
    assert isinstance(typeface.isItalic(), bool)


def test_Typeface_isFixedPitch(typeface):
    assert isinstance(typeface.isFixedPitch(), bool)


@pytest.mark.xfail(reason='Fix me!')
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


@pytest.fixture
def fontstyleset():
    return skia.FontStyleSet.CreateEmpty()


def test_FontStyleSet_unique(fontstyleset):
    assert isinstance(fontstyleset.unique(), bool)


def test_FontStyleSet_ref_unref(fontstyleset):
    fontstyleset.ref()
    fontstyleset.unref()


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


@pytest.fixture(scope='session')
def font():
    return skia.Font()


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.Typeface.MakeDefault(), 10),
    (skia.Typeface.MakeDefault(),),
    (skia.Typeface.MakeDefault(), 10, 1, 0),
])
def test_Font_init(args):
    assert isinstance(skia.Font(*args), skia.Font)


def test_Font_eq(font):
    assert font == font


def test_Font_ne(font):
    assert not (font != font)


def test_Font_isForceAutoHinting(font):
    assert isinstance(font.isForceAutoHinting(), bool)


def test_Font_isEmbeddedBitmaps(font):
    assert isinstance(font.isEmbeddedBitmaps(), bool)


def test_Font_isSubpixel(font):
    assert isinstance(font.isSubpixel(), bool)


def test_Font_isLinearMetrics(font):
    assert isinstance(font.isLinearMetrics(), bool)


def test_Font_isEmbolden(font):
    assert isinstance(font.isEmbolden(), bool)


def test_Font_isBaselineSnap(font):
    assert isinstance(font.isBaselineSnap(), bool)


def test_Font_setForceAutoHinting(font):
    font.setForceAutoHinting(True)


def test_Font_setEmbeddedBitmaps(font):
    font.setEmbeddedBitmaps(True)


def test_Font_setSubpixel(font):
    font.setSubpixel(True)


def test_Font_setLinearMetrics(font):
    font.setLinearMetrics(True)


def test_Font_setEmbolden(font):
    font.setEmbolden(True)


def test_Font_setBaselineSnap(font):
    font.setBaselineSnap(True)


def test_Font_getEdging(font):
    assert isinstance(font.getEdging(), skia.Font.Edging)


def test_Font_setEdging(font):
    font.setEdging(skia.Font.kAntiAlias)


def test_Font_getHinting(font):
    assert isinstance(font.getHinting(), skia.FontHinting)


def test_Font_setHinting(font):
    font.setHinting(skia.FontHinting.kNone)


def test_Font_makeWithSize(font):
    assert isinstance(font.makeWithSize(10), skia.Font)


def test_Font_getTypeface(font):
    assert isinstance(font.getTypeface(), (skia.Typeface, type(None)))


def test_Font_getTypefaceOrDefault(font):
    assert isinstance(font.getTypefaceOrDefault(), skia.Typeface)


def test_Font_getSize(font):
    assert isinstance(font.getSize(), float)


def test_Font_getScaleX(font):
    assert isinstance(font.getScaleX(), float)


def test_Font_getSkewX(font):
    assert isinstance(font.getSkewX(), float)


def test_Font_getSize(font):
    assert isinstance(font.getSize(), float)


def test_Font_refTypeface(font):
    assert isinstance(font.refTypeface(), (skia.Typeface, type(None)))


def test_Font_refTypefaceOrDefault(font):
    assert isinstance(font.refTypefaceOrDefault(), skia.Typeface)


def test_Font_setSize(font):
    font.setSize(10)


def test_Font_setScaleX(font):
    font.setScaleX(1)


def test_Font_setSkewX(font):
    font.setSkewX(0)


def test_Font_textToGlyphs(font):
    glyphs = font.textToGlyphs('abcde')
    assert isinstance(glyphs, list)
    assert len(glyphs) == 5


@pytest.fixture
def glyphs(font):
    return font.textToGlyphs('abcde')


def test_Font_unicharToGlyph(font):
    assert isinstance(font.unicharToGlyph(ord('a')), int)


def test_Font_unicharsToGlyphs(font):
    assert isinstance(font.unicharsToGlyphs([0x40, 0x41]), list)


def test_Font_countText(font):
    assert isinstance(font.countText('abcde'), int)


def test_Font_measureText(font):
    assert isinstance(font.measureText('abcde'), float)


def test_Font_getWidths(font, glyphs):
    assert isinstance(font.getWidths(glyphs), list)


def test_Font_getWidthsBounds(font, glyphs):
    assert isinstance(font.getWidthsBounds(glyphs), tuple)


def test_Font_getBounds(font, glyphs):
    assert isinstance(font.getBounds(glyphs), list)


def test_Font_getPos(font, glyphs):
    assert isinstance(font.getPos(glyphs), list)


def test_Font_getXPos(font, glyphs):
    assert isinstance(font.getXPos(glyphs), list)


def test_Font_getPath(font, glyphs):
    path = skia.Path()
    assert isinstance(font.getPath(glyphs[0], path), bool)


def test_Font_getMetrics(font):
    assert isinstance(font.getMetrics(), skia.FontMetrics)


def test_Font_getSpacing(font):
    assert isinstance(font.getSpacing(), float)


def test_Font_dump(font):
    font.dump()


@pytest.fixture
def fontmetrics(font):
    return font.getMetrics()


def test_FontMetrics_hasUnderlineThickness(fontmetrics):
    thickness = 0.
    assert isinstance(fontmetrics.hasUnderlineThickness(thickness), bool)


def test_FontMetrics_hasUnderlinePosition(fontmetrics):
    position = 0.
    assert isinstance(fontmetrics.hasUnderlinePosition(position), bool)


def test_FontMetrics_hasStrikeoutThickness(fontmetrics):
    thickness = 0.
    assert isinstance(fontmetrics.hasStrikeoutThickness(thickness), bool)


def test_FontMetrics_hasStrikeoutPosition(fontmetrics):
    position = 0.
    assert isinstance(fontmetrics.hasStrikeoutPosition(position), bool)
