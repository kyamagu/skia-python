import contextlib
import skia
import pytest
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


def test_Surface_init():
    import numpy as np
    array = np.zeros((240, 320, 4), dtype=np.uint8)
    assert isinstance(skia.Surface(array), skia.Surface)
    assert isinstance(skia.Surface.MakeRasterN32Premul(320, 240), skia.Surface)
    assert isinstance(
        skia.Surface.MakeRasterN32Premul(320, 240, None), skia.Surface)
    assert isinstance(
        skia.Surface.MakeRasterN32Premul(320, 240,
            skia.SurfaceProps(skia.SurfaceProps.InitType.kLegacyFontHost)),
        skia.Surface)


def test_Surface_methods(surface):
    assert surface.width() == 320
    assert surface.height() == 240
    assert isinstance(surface.imageInfo(), skia.ImageInfo)
    assert isinstance(surface.getCanvas(), skia.Canvas)
    assert isinstance(surface.generationID(), int)
    # assert isinstance(surface.makeSurface(
    #     skia.ImageInfo.MakeN32Premul(120, 120)), skia.Surface)
    assert isinstance(surface.makeSurface(120, 120), skia.Surface)
    assert isinstance(surface.makeImageSnapshot(), skia.Image)
    assert isinstance(
        surface.makeImageSnapshot(skia.IRect(100, 100)), skia.Image)
    surface.draw(surface.getCanvas(), 0, 0, None)
