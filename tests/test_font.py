import skia
import pytest


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
