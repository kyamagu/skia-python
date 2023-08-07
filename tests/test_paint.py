import skia
import pytest


@pytest.mark.parametrize('args', [
    tuple(),
    (0xFF00FF00,),
    (skia.Color4f.FromColor(0xFF00FF00), skia.ColorSpace.MakeSRGB()),
    (skia.Paint(),),
    (
        {
            'Alpha': 255,
            'Alphaf': 1.0,
            'AntiAlias': True,
            'Color': 0xFFFFFFFF,
            'Color4f': skia.Color4f.FromColor(0xFF00FF00),
            'ColorFilter': skia.LumaColorFilter.Make(),
            'Dither': False,
#            'FilterQuality': skia.kMedium_FilterQuality,
            'ImageFilter': skia.ImageFilters.Blur(1.0, 1.0),
            'MaskFilter': skia.MaskFilter.MakeBlur(skia.kNormal_BlurStyle, 1.),
            'PathEffect': skia.DashPathEffect.Make([2., 1.], 0),
            'Shader': skia.Shaders.Empty(),
            'StrokeCap': skia.Paint.kButt_Cap,
            'StrokeJoin': skia.Paint.kMiter_Join,
            'StrokeMiter': 0,
            'StrokeWidth': 2,
            'Style': skia.Paint.kStroke_Style,
        },
    ),
])
def test_Paint_init(args):
    assert isinstance(skia.Paint(*args), skia.Paint)


def test_Paint_kwargs():
    assert isinstance(skia.Paint(Color=0xFFFFFFFF), skia.Paint)


@pytest.fixture
def paint():
    return skia.Paint()


@pytest.mark.skip(reason='m116:REVISIT')
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


@pytest.mark.skip(reason='m116:REVISIT')
def test_Paint_getFilterQuality(paint):
    assert isinstance(paint.getFilterQuality(), skia.FilterQuality)


@pytest.mark.skip(reason='m116:REVISIT')
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


@pytest.mark.skip(reason='m116:REVISIT')
def test_Paint_getBlendMode(paint):
    assert isinstance(paint.getBlendMode(), skia.BlendMode)


def test_Paint_isSrcOver(paint):
    assert isinstance(paint.isSrcOver(), bool)


def test_Paint_setBlendMode(paint):
    paint.setBlendMode(skia.BlendMode.kOverlay)


def test_Paint_getPathEffect(paint):
    assert isinstance(paint.getPathEffect(), (skia.PathEffect, type(None)))


def test_Paint_refPathEffect(paint):
    assert isinstance(paint.refPathEffect(), (skia.PathEffect, type(None)))


def test_Paint_setPathEffect(paint):
    paint.setPathEffect(skia.CornerPathEffect.Make(4.0))


def test_Paint_getMaskFilter(paint):
    assert isinstance(paint.getMaskFilter(), (skia.MaskFilter, type(None)))


def test_Paint_refMaskFilter(paint):
    assert isinstance(paint.refMaskFilter(), (skia.MaskFilter, type(None)))


def test_Paint_setMaskFilter(paint):
    paint.setMaskFilter(skia.MaskFilter.MakeBlur(skia.kNormal_BlurStyle, 1.))


def test_Paint_getImageFilter(paint):
    assert isinstance(paint.getImageFilter(), (skia.ImageFilter, type(None)))


def test_Paint_refImageFilter(paint):
    assert isinstance(paint.refImageFilter(), (skia.ImageFilter, type(None)))


def test_Paint_setImageFilter(paint):
    paint.setImageFilter(skia.ImageFilters.Blur(1.0, 1.0))


def test_Paint_nothingToDraw(paint):
    assert isinstance(paint.nothingToDraw(), bool)


def test_Paint_canComputeFastBounds(paint):
    assert isinstance(paint.canComputeFastBounds(), bool)


def test_Paint_computeFastBounds(paint):
    paint.setStyle(skia.Paint.kStroke_Style)
    paint.setStrokeWidth(4)
    paint.setColor(0xFFFFFFFF)
    if paint.canComputeFastBounds():
        assert isinstance(paint.computeFastBounds(skia.Rect(10, 10)), skia.Rect)


def test_Paint_computeFastStrokeBounds(paint):
    paint.setStyle(skia.Paint.kStroke_Style)
    paint.setStrokeWidth(4)
    paint.setColor(0xFFFFFFFF)
    if paint.canComputeFastBounds():
        assert isinstance(
            paint.computeFastStrokeBounds(skia.Rect(10, 10)), skia.Rect)


def test_Paint_doComputeFastBounds(paint):
    paint.setStyle(skia.Paint.kStroke_Style)
    paint.setStrokeWidth(4)
    paint.setColor(0xFFFFFFFF)
    if paint.canComputeFastBounds():
        assert isinstance(paint.doComputeFastBounds(
            skia.Rect(10, 10), skia.Paint.kStroke_Style), skia.Rect)


def test_Paint_eq(paint):
    assert paint == paint


def test_Paint_ne(paint):
    paint2 = skia.Paint()
    paint2.setColor(0xFFFFFFFF)
    assert paint != paint2


def test_Paint_convertible(canvas):
    canvas.drawPaint({'Color': 0xFFFFFFFF})
    canvas.drawLine((0, 0), (1, 1), {'Color': 0xFF0000FF})
