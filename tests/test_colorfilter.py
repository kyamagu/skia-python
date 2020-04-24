import skia
import pytest


@pytest.fixture
def colorfilter():
    return skia.LumaColorFilter.Make()


def test_ColorFilter_asAColorMode(colorfilter):
    result = colorfilter.asAColorMode()
    assert isinstance(result, (tuple, type(None)))
    if isinstance(result, tuple):
        assert isinstance(result[0], int)
        assert isinstance(result[1], skia.BlendMode)


def test_ColorFilter_asAColorMatrix(colorfilter):
    assert isinstance(colorfilter.asAColorMatrix(), (list, type(None)))


def test_ColorFilter_getFlags(colorfilter):
    assert isinstance(colorfilter.getFlags(), int)


def test_ColorFilter_filterColor(colorfilter):
    assert isinstance(colorfilter.filterColor(0xFF00FFFF), int)


def test_ColorFilter_filterColor4f(colorfilter):
    srcCS = skia.ColorSpace.MakeSRGB()
    dstCS = skia.ColorSpace.MakeSRGB()
    assert isinstance(
        colorfilter.filterColor4f(skia.Color4f(0xFF00FFFF), srcCS, dstCS),
        skia.Color4f)


def test_ColorFilter_affectsTransparentBlack(colorfilter):
    assert isinstance(colorfilter.affectsTransparentBlack(), bool)


def test_ColorFilter_Deserialize(colorfilter):
    assert isinstance(
        skia.ColorFilter.Deserialize(colorfilter.serialize()), skia.ColorFilter)


def test_LumaColorFilter_Make():
    assert isinstance(skia.LumaColorFilter.Make(), skia.ColorFilter)


def test_OverdrawColorFilter_MakeWithColors():
    assert isinstance(
        skia.OverdrawColorFilter.MakeWithColors([0xFF00FFFF for i in range(6)]),
        skia.ColorFilter)


def test_TableColorFilter_Make():
    assert isinstance(skia.TableColorFilter.Make(range(256)), skia.ColorFilter)


def test_TableColorFilter_MakeARGB():
    assert isinstance(skia.TableColorFilter.MakeARGB(
        range(256), range(256), range(256), range(256)), skia.ColorFilter)
