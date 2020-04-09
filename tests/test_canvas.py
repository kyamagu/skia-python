import skia
import pytest
import numpy as np


@pytest.fixture
def canvas(surface):
    canvas = surface.getCanvas()
    yield canvas
    canvas.clear(skia.ColorWHITE)
    canvas.flush()


@pytest.fixture(scope='session')
def image():
    yield skia.Image(np.zeros((320, 240, 4), dtype=np.uint8))


@pytest.fixture(scope='session')
def bitmap():
    yield skia.Bitmap()


@pytest.fixture(scope='session')
def vertices():
    return skia.Vertices(skia.Vertices.VertexMode.kTriangles, [
        (skia.Point(0, 0), skia.Point(1, 1), skia.ColorRED),
        (skia.Point(1, 1), skia.Point(1, 0), skia.ColorRED),
        (skia.Point(1, 0), skia.Point(0, 0), skia.ColorRED),
    ])


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


@pytest.mark.parametrize('args', [
    (1, 1, skia.Paint()),
    (skia.Point(1, 1), skia.Paint()),
])
def test_Canvas_drawPoint(canvas, args):
    canvas.drawPoint(*args)


@pytest.mark.parametrize('args', [
    (0, 0, 1, 1, skia.Paint()),
    (skia.Point(0, 0), skia.Point(1, 1), skia.Paint()),
])
def test_Canvas_drawLine(canvas, args):
    canvas.drawLine(*args)


def test_Canvas_drawRect(canvas):
    canvas.drawRect(skia.Rect(10, 10), skia.Paint())


def test_Canvas_drawIRect(canvas):
    canvas.drawIRect(skia.IRect(10, 10), skia.Paint())


def test_Canvas_drawRegion(canvas):
    canvas.drawRegion(skia.Region(), skia.Paint())


def test_Canvas_drawOval(canvas):
    canvas.drawOval(skia.Rect(10, 10), skia.Paint())


def test_Canvas_drawRRect(canvas):
    canvas.drawRRect(skia.RRect(), skia.Paint())


def test_Canvas_drawDRRect(canvas):
    canvas.drawDRRect(skia.RRect(), skia.RRect(), skia.Paint())


@pytest.mark.parametrize('args', [
    (50, 50, 20, skia.Paint()),
    (skia.Point(50, 50), 20, skia.Paint()),
])
def test_Canvas_drawCircle(canvas, args):
    canvas.drawCircle(*args)


def test_Canvas_drawArc(canvas):
    canvas.drawArc(skia.Rect(30, 30), 0, 90, True, skia.Paint())


def test_Canvas_drawRoundRect(canvas):
    canvas.drawRoundRect(skia.Rect(30, 30), 3, 3, skia.Paint())


def test_Canvas_drawPath(canvas):
    canvas.drawPath(skia.Path(), skia.Paint())


@pytest.mark.parametrize('args', [
    (0, 0),
    (0, 0, skia.Paint()),
])
def test_Canvas_drawImage(canvas, image, args):
    canvas.drawImage(image, *args)


@pytest.mark.parametrize('args', [
    (skia.Rect(100, 100), skia.Rect(100, 100)),
    (skia.Rect(100, 100), skia.Rect(100, 100), skia.Paint()),
    (skia.Rect(100, 100), skia.Rect(100, 100), skia.Paint(),
        skia.Canvas.kStrict),
    (skia.IRect(100, 100), skia.Rect(100, 100)),
    (skia.IRect(100, 100), skia.Rect(100, 100), skia.Paint()),
    (skia.IRect(100, 100), skia.Rect(100, 100), skia.Paint(),
        skia.Canvas.kStrict),
    (skia.Rect(100, 100),),
    (skia.Rect(100, 100), skia.Paint()),
])
def test_Canvas_drawImageRect(canvas, image, args):
    canvas.drawImageRect(image, *args)


@pytest.mark.parametrize('args', [
    (0, 0),
    (0, 0, skia.Paint()),
])
def test_Canvas_drawBitmap(canvas, bitmap, args):
    canvas.drawBitmap(bitmap, *args)


@pytest.mark.parametrize('args', [
    (skia.Rect(100, 100), skia.Rect(100, 100)),
    (skia.Rect(100, 100), skia.Rect(100, 100), skia.Paint()),
    (skia.Rect(100, 100), skia.Rect(100, 100), skia.Paint(),
        skia.Canvas.kStrict),
    (skia.IRect(100, 100), skia.Rect(100, 100), skia.Paint()),
    (skia.IRect(100, 100), skia.Rect(100, 100), skia.Paint()),
    (skia.IRect(100, 100), skia.Rect(100, 100), skia.Paint(),
        skia.Canvas.kStrict),
    (skia.Rect(100, 100),),
    (skia.Rect(100, 100), skia.Paint()),
    (skia.Rect(100, 100), skia.Paint(), skia.Canvas.kStrict),
])
def test_Canvas_drawBitmapRect(canvas, bitmap, args):
    canvas.drawBitmapRect(bitmap, *args)


# @pytest.mark.parametrize('args', [
#     (skia.Canvas.Lattice(), skia.Rect(100, 100)),
#     (skia.Canvas.Lattice(), skia.Rect(100, 100), skia.Paint()),
# ])
# def test_Canvas_drawImageLattice(canvas, image, args):
#     canvas.drawImageLattice(image, *args)


def test_Canvas_drawSimpleText(canvas):
    canvas.drawSimpleText('foo', 0, 0, skia.Font(), skia.Paint())


def test_Canvas_drawString(canvas):
    canvas.drawString('foo', 0, 0, skia.Font(), skia.Paint())


@pytest.mark.parametrize('args', [
    (skia.TextBlob('foo', skia.Font()), 0, 0, skia.Paint(),),
    (skia.TextBlob('foo', skia.Font()), 0, 0, skia.Paint(),),
])
def test_Canvas_drawTextBlob(canvas, args):
    canvas.drawTextBlob(*args)


@pytest.mark.parametrize('args', [
    (skia.Picture(skia.Rect(100, 100)),),
    (skia.Picture(skia.Rect(100, 100)), skia.Matrix(),),
    (skia.Picture(skia.Rect(100, 100)), skia.Matrix(), skia.Paint(),),
    (skia.Picture(skia.Rect(100, 100)), None, skia.Paint(),),
])
def test_Canvas_drawPicture(canvas, args):
    canvas.drawPicture(*args)


@pytest.mark.parametrize('args', [
    (skia.Paint(),),
    (skia.Paint(), skia.BlendMode.kModulate),
])
def test_Canvas_drawVertices(canvas, vertices, args):
    canvas.drawVertices(vertices, *args)
