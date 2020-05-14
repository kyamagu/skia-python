import contextlib
import skia
import pytest
import sys
import logging

logger = logging.getLogger(__name__)


@contextlib.contextmanager
def glfw_context():
    import glfw
    if not glfw.init():
        raise RuntimeError('glfw.init() failed')
    glfw.window_hint(glfw.VISIBLE, glfw.FALSE)
    glfw.window_hint(glfw.STENCIL_BITS, 8)
    context = glfw.create_window(640, 480, '', None, None)
    glfw.make_context_current(context)
    yield context
    glfw.terminate()


@contextlib.contextmanager
def glut_context():
    from OpenGL.GLUT import glutInit, glutCreateWindow, glutHideWindow
    glutInit()
    context = glutCreateWindow('Hidden window for OpenGL context')
    glutHideWindow()
    yield context


@contextlib.contextmanager
def opengl_context():
    try:
        with glfw_context() as context:
            yield context
        return
    except ImportError:
        logger.warning('glfw not found, falling back to pyopengl')

    with glut_context() as context:
        yield context


def opengl_is_available():
    try:
        import glfw
        return True
    except ImportError:
        pass

    try:
        import OpenGL
        return True
    except ImportError:
        pass

    return False


@pytest.fixture(scope='session')
def context_or_none():
    if opengl_is_available():
        with opengl_context():
            yield skia.GrContext.MakeGL()
    else:
        yield None


@pytest.fixture
def context(context_or_none):
    if not context_or_none:
        pytest.skip('GrContext is not available')
    return context_or_none


@pytest.fixture(scope='session', params=[
    'raster',
    ('gpu', pytest.mark.skipif(
        not opengl_is_available(), reason='OpenGL is not available')),
])
def surface(request, context_or_none):
    if request.param == 'gpu' and context_or_none is not None:
        info = skia.ImageInfo.MakeN32Premul(320, 240)
        yield skia.Surface.MakeRenderTarget(
            context_or_none, skia.Budgeted.kNo, info)
    else:
        yield skia.Surface(320, 240)


@pytest.fixture(scope='session')
def png_data():
    import os
    root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    image_path = os.path.join(
        root_dir, 'skia', 'resources', 'images', 'color_wheel.png')
    return skia.Data.MakeFromFileName(image_path)


@pytest.fixture(scope='session')
def image(png_data):
    return skia.Image.MakeFromEncoded(png_data)
