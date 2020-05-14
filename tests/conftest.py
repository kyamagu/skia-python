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
    try:
        context = glfw.create_window(640, 480, '', None, None)
        glfw.make_context_current(context)
        yield context
        glfw.destroy_window(context)
    except glfw.GLFWError:
        logger.exception('GLFW error')
    glfw.terminate()


@contextlib.contextmanager
def glut_context():
    from OpenGL.GLUT import glutInit, glutCreateWindow, glutHideWindow
    glutInit()
    context = glutCreateWindow('Hidden window for OpenGL context')
    glutHideWindow()
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
def opengl_context():
    try:
        with glfw_context() as context:
            yield context
        return
    except ImportError:
        logger.warning('glfw not found, falling back to pyopengl')

    try:
        with glut_context() as context:
            yield context
    except ImportError:
        pass

    pytest.skip('OpenGL is not available')


@pytest.fixture(scope='session')
def context(opengl_context):
    yield skia.GrContext.MakeGL()


@pytest.fixture(scope='module', params=[
    'raster',
    ('gpu', pytest.mark.skipif(
        not opengl_is_available(), reason='OpenGL is not available')),
])
def surface(request):
    if request.param == 'gpu':
        context = request.getfixturevalue('context')
        info = skia.ImageInfo.MakeN32Premul(320, 240)
        return skia.Surface.MakeRenderTarget(
            context, skia.Budgeted.kNo, info)
    else:
        return skia.Surface(320, 240)


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
