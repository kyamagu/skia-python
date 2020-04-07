import contextlib
import skia
import pytest
import numpy as np
import sys
import logging

logger = logging.getLogger(__name__)


@contextlib.contextmanager
def opengl_context():
    if sys.platform in ('linux', 'win32'):
        # Seems macos VM fails to create OpenGL context via glfw.
        import glfw
        if not glfw.init():
            raise RuntimeError('glfw.init() failed')
        glfw.window_hint(glfw.VISIBLE, glfw.FALSE)
        glfw.window_hint(glfw.STENCIL_BITS, 8)
        context = glfw.create_window(640, 480, '', None, None)
        glfw.make_context_current(context)
        yield context
        glfw.terminate()
    elif sys.platform in ('darwin',):
        from OpenGL.GLUT import glutInit, glutCreateWindow, glutHideWindow
        glutInit()
        context = glutCreateWindow('Hidden window for OpenGL context')
        glutHideWindow()
        yield context
    else:
        logger.warning('Platform not supported: %s' % sys.platform)
        yield None


@pytest.fixture(scope='session', params=[
    'raster',
    ('gpu', pytest.mark.skipif(
        sys.platform == 'win32',
        reason='Windows VMs on Github Actions do not support capable drivers.',
        )),
])
def surface(request):
    if request.param == 'gpu':
        with opengl_context():
            context = skia.GrContext.MakeGL()
            info = skia.ImageInfo.MakeN32Premul(320, 240)
            yield skia.Surface.MakeRenderTarget(
                context, skia.Budgeted.kNo, info)
    else:
        yield skia.Surface(320, 240)


def check_surface(x):
    assert isinstance(x, skia.Surface)


@pytest.mark.parametrize('args', [
    (np.zeros((240, 320, 4), dtype=np.uint8),),
])
def test_Surface_init(args):
    check_surface(skia.Surface(*args))


@pytest.mark.parametrize('args', [
    (320, 240),
    (320, 240, None),
    (320, 240, skia.SurfaceProps(skia.SurfaceProps.InitType.kLegacyFontHost)),
])
def test_Surface_MakeRasterN32Premul(args):
    check_surface(skia.Surface.MakeRasterN32Premul(*args))


def test_Surface_width(surface):
    assert surface.width() == 320


def test_Surface_height(surface):
    assert surface.height() == 240


def test_Surface_getCanvas(surface):
    assert isinstance(surface.imageInfo(), skia.ImageInfo)


def test_Surface_generationID(surface):
    assert isinstance(surface.getCanvas(), skia.Canvas)


def test_Surface_generationID(surface):
    assert isinstance(surface.generationID(), int)


@pytest.mark.parametrize('args', [
    (skia.ImageInfo.MakeN32Premul(120, 120),),
    (120, 120),
])
def test_Surface_makeSurface(surface, args):
    check_surface(surface.makeSurface(*args))


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.IRect(100, 100),),
])
def test_Surface_makeImageSnapshot(surface, args):
    assert isinstance(surface.makeImageSnapshot(*args), skia.Image)


def test_Surface_draw(surface):
    surface.draw(surface.getCanvas(), 0, 0, None)


def test_Surface_peekPixels(surface):
    assert surface.peekPixels(skia.Pixmap())


@pytest.mark.parametrize('args', [
    (skia.Pixmap(), 0, 0),
    # (skia.ImageInfo(), None, 0, 0, 0),
    (skia.Bitmap(), 0, 0),
])
def test_Surface_readPixels(surface, args):
    assert isinstance(surface.readPixels(*args), bool)


@pytest.mark.parametrize('args', [
    (skia.Pixmap(), 0, 0),
    (skia.Bitmap(), 0, 0),
])
def test_Surface_writePixels(surface, args):
    surface.writePixels(*args)


def test_Surface_props(surface):
    assert isinstance(surface.props(), skia.SurfaceProps)


def test_Surface_flush(surface):
    surface.flush()


def test_Surface_characterize(surface):
    characterization = skia.SurfaceCharacterization()
    assert isinstance(surface.characterize(characterization), bool)


# def test_Surface_draw(surface):
#     surface.draw()


def test_Surface_unique(surface):
    assert surface.unique()


def test_Surface_ref_unref(surface):
    surface.ref()
    surface.unref()
