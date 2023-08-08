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


@pytest.mark.skip(reason='m116:REVISIT')
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


@pytest.mark.xfail(reason='TableColorFilter class removed in m116')
def test_ColorFilter_makeComposed(colorfilter):
    assert isinstance(colorfilter.makeComposed(
        skia.TableColorFilter.Make(range(256))), skia.ColorFilter)


def test_ColorFilter_Deserialize(colorfilter):
    assert isinstance(
        skia.ColorFilter.Deserialize(colorfilter.serialize()), skia.ColorFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ColorFilters_Compose(colorfilter):
    assert isinstance(
        skia.ColorFilters.Compose(colorfilter, colorfilter),
        skia.ColorFilter)


def test_ColorFilters_Blend():
    assert isinstance(
        skia.ColorFilters.Blend(0xFF000000, skia.BlendMode.kSrc),
        skia.ColorFilter)


def test_ColorFilters_Matrix():
    assert isinstance(
        skia.ColorFilters.Matrix(range(20)),
        skia.ColorFilter)


def test_ColorFilters_HSLAMatrix():
    assert isinstance(
        skia.ColorFilters.HSLAMatrix(range(20)),
        skia.ColorFilter)


def test_ColorFilters_LinearToSRGBGamma():
    assert isinstance(
        skia.ColorFilters.LinearToSRGBGamma(),
        skia.ColorFilter)


def test_ColorFilters_SRGBToLinearGamma():
    assert isinstance(
        skia.ColorFilters.SRGBToLinearGamma(),
        skia.ColorFilter)


def test_ColorFilters_Lerp(colorfilter):
    assert isinstance(
        skia.ColorFilters.Lerp(0.5, colorfilter, colorfilter),
        skia.ColorFilter)


def test_ColorMatrixFilter_MakeLightingFilter():
    assert isinstance(
        skia.ColorMatrixFilter.MakeLightingFilter(0xFF010101, 0xFF000000),
        skia.ColorFilter)


def test_LumaColorFilter_Make():
    assert isinstance(skia.LumaColorFilter.Make(), skia.ColorFilter)


def test_OverdrawColorFilter_MakeWithColors():
    assert isinstance(
        skia.OverdrawColorFilter.MakeWithColors([0xFF00FFFF for i in range(6)]),
        skia.ColorFilter)


@pytest.mark.xfail(reason='TableColorFilter class removed in m116')
def test_TableColorFilter_Make():
    assert isinstance(skia.TableColorFilter.Make(range(256)), skia.ColorFilter)


@pytest.mark.parametrize('args', [
    (range(256), range(256), range(256), range(256)),
    (range(256), None, None, None),
])
@pytest.mark.xfail(reason='TableColorFilter class removed in m116')
def test_TableColorFilter_MakeARGB(args):
    assert isinstance(skia.TableColorFilter.MakeARGB(*args), skia.ColorFilter)
