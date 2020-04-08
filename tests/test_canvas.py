import skia
import pytest


@pytest.fixture
def canvas(surface):
    yield surface.getCanvas()


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
