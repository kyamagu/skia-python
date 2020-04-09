import skia
import pytest
import numpy as np


@pytest.fixture
def canvas(surface):
    canvas = surface.getCanvas()
    yield canvas
    canvas.clear(skia.ColorWHITE)
    canvas.flush()


def check_canvas(x):
    assert isinstance(x, skia.Canvas)


@pytest.mark.parametrize('args', [
    tuple(),
    (100, 100),
    (100, 100, None),
    (100, 100, skia.SurfaceProps(skia.SurfaceProps.InitType.kLegacyFontHost)),
    (skia.Bitmap(),),
    (skia.Bitmap(),
        skia.SurfaceProps(skia.SurfaceProps.InitType.kLegacyFontHost)),
])
def test_Canvas_init(args):
    check_canvas(skia.Canvas(*args))


def test_Canvas_imageInfo(canvas):
    assert isinstance(canvas.imageInfo(), skia.ImageInfo)


def test_Canvas_getProps(canvas):
    props = skia.SurfaceProps(skia.SurfaceProps.InitType.kLegacyFontHost)
    assert isinstance(canvas.getProps(props), bool)


def test_Canvas_flush(canvas):
    canvas.flush()


def test_Canvas_getBaseLayerSize(canvas):
    assert isinstance(canvas.getBaseLayerSize(), skia.ISize)


@pytest.mark.parametrize('args', [
    tuple(),
    (None,),
    (skia.SurfaceProps(skia.SurfaceProps.InitType.kLegacyFontHost),),
])
def test_Canvas_makeSurface(canvas, args):
    assert isinstance(canvas.makeSurface(canvas.imageInfo(), *args),
        skia.Surface)


def test_Canvas_getGrContext(canvas):
    assert isinstance(canvas.getGrContext(), (skia.GrContext, type(None)))


def test_Canvas_getSurface(canvas):
    assert isinstance(canvas.getSurface(), (skia.Surface, type(None)))


# def test_Canvas_accessTopLayerPixels(canvas):
#     canvas.accessTopLayerPixels()


def test_Canvas_peekPixels(canvas):
    assert isinstance(canvas.peekPixels(skia.Pixmap()), bool)


@pytest.mark.parametrize('args', [
    (skia.Pixmap(), 0, 0),
    (np.zeros((240, 320, 4), dtype=np.uint8), 0, 0),
    (np.zeros((240, 320, 4), dtype=np.uint8),),
    (skia.Bitmap(), 0, 0),
])
def test_Canvas_readPixels(canvas, args):
    assert isinstance(canvas.readPixels(*args), bool)


@pytest.mark.parametrize('args', [
    (np.zeros((240, 320, 4), dtype=np.uint8), 0, 0),
    (np.zeros((240, 320, 4), dtype=np.uint8),),
    (skia.Bitmap(), 0, 0),
])
def test_Canvas_writePixels(canvas, args):
    canvas.writePixels(*args)


def test_Canvas_save(canvas):
    assert canvas.save() == 1
    canvas.restore()


@pytest.mark.parametrize('args', [
    (None, None),
    (skia.Rect(100, 100), skia.Paint()),
    (skia.Canvas.SaveLayerRec(),),
])
def test_Canvas_saveLayer(canvas, args):
    assert canvas.saveLayer(*args) == 1
    canvas.restore()


def test_Canvas_saveLayerAlpha(canvas):
    assert canvas.saveLayerAlpha(skia.Rect(100, 100), 255) == 1
    canvas.restore()


# def test_Canvas_experimental_saveCamera(canvas):
#     canvas.experimental_saveCamera()


def test_Canvas_getSaveCount(canvas):
    assert isinstance(canvas.getSaveCount(), int)


def test_Canvas_restoreToCount(canvas):
    canvas.restoreToCount(1)


def test_Canvas_translate(canvas):
    canvas.translate(0, 0)


def test_Canvas_scale(canvas):
    canvas.scale(1, 1)


@pytest.mark.parametrize('args', [
    (0,),
    (0, 0, 0),
])
def test_Canvas_rotate(canvas, args):
    canvas.rotate(*args)


def test_Canvas_skew(canvas):
    canvas.skew(0, 0)


def test_Canvas_concat(canvas):
    canvas.concat(skia.Matrix())


@pytest.mark.parametrize('args', [(skia.M44(),),])
def test_Canvas_concat44(canvas, args):
    canvas.concat44(*args)


def test_Canvas_setMatrix(canvas):
    canvas.setMatrix(skia.Matrix())


def test_Canvas_resetMatrix(canvas):
    canvas.resetMatrix()


@pytest.mark.parametrize('args', [
    (skia.Rect(320, 240), skia.ClipOp.kIntersect, True),
    (skia.Rect(320, 240), True),
    (skia.Rect(320, 240), skia.ClipOp.kIntersect),
    (skia.Rect(320, 240),),
])
def test_Canvas_clipRect(canvas, args):
    canvas.clipRect(*args)


def test_Canvas_androidFramework_setDeviceClipRestriction(canvas):
    canvas.androidFramework_setDeviceClipRestriction(skia.IRect(320, 240))


@pytest.mark.parametrize('args', [
    (skia.RRect.MakeRect(skia.Rect(320, 240)), skia.ClipOp.kIntersect, True),
    (skia.RRect.MakeRect(skia.Rect(320, 240)), True),
    (skia.RRect.MakeRect(skia.Rect(320, 240)), skia.ClipOp.kIntersect),
    (skia.RRect.MakeRect(skia.Rect(320, 240)),),
])
def test_Canvas_clipRRect(canvas, args):
    canvas.clipRRect(*args)


@pytest.mark.parametrize('args', [
    (skia.Path(), skia.ClipOp.kIntersect, True),
    (skia.Path(), skia.ClipOp.kIntersect),
    (skia.Path(), True),
    (skia.Path(),),
])
def test_Canvas_clipPath(canvas, args):
    canvas.clipPath(*args)


@pytest.mark.parametrize('args', [
    (skia.Region(),),
    (skia.Region(), skia.ClipOp.kIntersect,),
])
def test_Canvas_clipRegion(canvas, args):
    canvas.clipRegion(*args)


@pytest.mark.parametrize('args', [
    (skia.Rect(100, 100),),
    (skia.Path(),),
])
def test_Canvas_quickReject(canvas, args):
    assert isinstance(canvas.quickReject(*args), bool)


@pytest.mark.parametrize('args, kls', [
    (tuple(), skia.Rect),
    ((skia.Rect(100, 100),), bool),
])
def test_Canvas_getLocalClipBounds(canvas, args, kls):
    assert isinstance(canvas.getLocalClipBounds(*args), kls)


@pytest.mark.parametrize('args, kls', [
    (tuple(), skia.IRect),
    ((skia.IRect(100, 100),), bool),
])
def test_Canvas_getDeviceClipBounds(canvas, args, kls):
    assert isinstance(canvas.getDeviceClipBounds(*args), kls)


@pytest.mark.parametrize('args', [
    (skia.ColorWHITE,),
    (skia.ColorWHITE, skia.BlendMode.kSrcOver),
])
def test_Canvas_drawColor(canvas, args):
    canvas.drawColor(*args)


def test_Canvas_clear(canvas):
    canvas.clear(skia.ColorWHITE)


def test_Canvas_discard(canvas):
    canvas.discard()


def test_Canvas_drawPaint(canvas):
    canvas.drawPaint(skia.Paint())


def test_Canvas_drawPoints(canvas):
    points = [skia.Point(0, 0), skia.Point(1, 1)]
    canvas.drawPoints(skia.Canvas.PointMode.kPoints, points, skia.Paint())
