import skia
import pytest


@pytest.mark.parametrize('args', [
    tuple(),
    (0xFF00FF00,),
    (skia.Color4f.FromColor(0xFF00FF00), skia.ColorSpace.MakeSRGB()),
    (skia.Paint(),),
])
def test_Paint_init(args):
    assert isinstance(skia.Paint(*args), skia.Paint)


@pytest.fixture
def paint():
    return skia.Paint()


def test_Paint_getHash(paint):
    assert isinstance(paint.getHash(), int)


def test_Paint_reset(paint):
    paint.reset()


def test_Paint_isAntiAlias(paint):
    assert isinstance(paint.isAntiAlias(), bool)


def test_Paint_setAntiAlias(paint):
    paint.setAntiAlias(True)


def test_Paint_isDither(paint):
    assert isinstance(paint.isDither(), bool)


def test_Paint_setDither(paint):
    paint.setDither(True)


def test_Paint_getFilterQuality(paint):
    assert isinstance(paint.getFilterQuality(), skia.FilterQuality)


def test_Paint_setFilterQuality(paint):
    paint.setFilterQuality(skia.FilterQuality.kLow_FilterQuality)


def test_Paint_getStyle(paint):
    assert isinstance(paint.getStyle(), skia.Paint.Style)


def test_Paint_setStyle(paint):
    paint.setStyle(skia.Paint.kStroke_Style)


def test_Paint_getColor(paint):
    assert isinstance(paint.getColor(), int)


def test_Paint_setColor(paint):
    paint.setColor(0xFFFFFFFF)


def test_Paint_getColor4f(paint):
    assert isinstance(paint.getColor4f(), skia.Color4f)


def test_Paint_setColor4f(paint):
    paint.setColor4f(skia.Color4f.FromColor(0xFFFFFFFF))


def test_Paint_getAlphaf(paint):
    assert isinstance(paint.getAlphaf(), float)


def test_Paint_setAlphaf(paint):
    paint.setAlphaf(0.0)


def test_Paint_getAlpha(paint):
    assert isinstance(paint.getAlpha(), int)


def test_Paint_setAlpha(paint):
    paint.setAlpha(255)


def test_Paint_getStrokeWidth(paint):
    assert isinstance(paint.getStrokeWidth(), float)


def test_Paint_setStrokeWidth(paint):
    paint.setStrokeWidth(4)


def test_Paint_getStrokeMiter(paint):
    assert isinstance(paint.getStrokeMiter(), float)


def test_Paint_setStrokeMiter(paint):
    paint.setStrokeMiter(4)


def test_Paint_getStrokeCap(paint):
    assert isinstance(paint.getStrokeCap(), skia.Paint.Cap)


def test_Paint_setStrokeCap(paint):
    paint.setStrokeCap(skia.Paint.kButt_Cap)


def test_Paint_getStrokeJoin(paint):
    assert isinstance(paint.getStrokeJoin(), skia.Paint.Join)


def test_Paint_setStrokeJoin(paint):
    paint.setStrokeJoin(skia.Paint.kMiter_Join)


@pytest.mark.parametrize('args', [
    (skia.Path(), skia.Path(),),
    (skia.Path(), skia.Path(), skia.Rect(100, 100), 1),
])
def test_Paint_getFillPath(paint, args):
    assert isinstance(paint.getFillPath(*args), bool)


def test_Paint_getShader(paint):
    assert isinstance(paint.getShader(), (skia.Shader, type(None)))


def test_Paint_refShader(paint):
    assert isinstance(paint.refShader(), (skia.Shader, type(None)))


def test_Paint_setShader(paint):
    paint.setShader(skia.Shaders.Empty())


def test_Paint_getColorFilter(paint):
    assert isinstance(paint.getColorFilter(), (skia.ColorFilter, type(None)))


def test_Paint_refColorFilter(paint):
    assert isinstance(paint.refColorFilter(), (skia.ColorFilter, type(None)))


def test_Paint_setColorFilter(paint):
    paint.setColorFilter(skia.LumaColorFilter.Make())




