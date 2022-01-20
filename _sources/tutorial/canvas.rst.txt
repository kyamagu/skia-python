Canvas Creation
===============

.. currentmodule:: skia

Skia has multiple backends which receive :py:class:`Canvas` drawing
commands, including:

- `Raster`_ - CPU-only.
- `GPU`_ - Skia's GPU-accelerated backend.
- `PDF`_ - PDF document creation.
- `Picture`_ - Skia's display list format.
- `SVG`_ - Experimental SVG backend.

Each backend has a unique way of creating a :py:class:`Canvas`. This page
gives an example for each.

Raster
------

The raster backend draws to a block of memory. This memory can be managed by
Skia or by the client.

The recommended way of creating a canvas for the Raster and Ganesh backends is
to use a :py:class:`Surface`, which is an object that manages the memory
into which the canvas commands are drawn.

.. code-block:: python

    import skia

    width, height = 200, 200
    surface = skia.Surface(width, height)

    with surface as canvas:
        canvas.drawCircle(100, 100, 40, skia.Paint(Color=skia.ColorGREEN))

    image = surface.makeImageSnapshot()
    image.save('output.png', skia.kPNG)


Alternatively, we could have specified the memory for the surface explicitly,
instead of asking Skia to manage it. The following example directly draws
into numpy array.

.. code-block:: python

    import skia
    import numpy as np
    import matplotlib.pyplot as plt

    width, height = 200, 200
    array = np.zeros((height, width, 4), dtype=np.uint8)

    with skia.Surface(array) as canvas:
        canvas.drawCircle(100, 100, 40, skia.Paint(Color=skia.ColorGREEN))

    plt.imshow(array)
    plt.show()

GPU
---

GPU Surfaces must have a :py:class:`GrDirectContext` object which manages the
GPU context, and related caches for textures and fonts. GrDirectContexts are matched
one to one with OpenGL contexts or Vulkan devices. That is, all
:py:class:`Surface` that will be rendered to using the same OpenGL context
or Vulkan device should share a :py:class:`GrDirectContext`. Skia does not
create a OpenGL context or Vulkan device for you. In OpenGL mode it also assumes
that the correct OpenGL context has been made current to the current thread when
Skia calls are made.

The following example uses glfw package to create an OpenGL context. Install
``glfw`` via pip to try.

.. code-block:: bash

    pip install glfw

.. code-block:: python

    import skia
    import glfw
    import contextlib


    @contextlib.contextmanager
    def glfw_context():
        if not glfw.init():
            raise RuntimeError('glfw.init() failed')
        glfw.window_hint(glfw.VISIBLE, glfw.FALSE)
        glfw.window_hint(glfw.STENCIL_BITS, 8)
        window = glfw.create_window(640, 480, '', None, None)
        glfw.make_context_current(window)
        yield window
        glfw.terminate()


    width, height = 200, 200
    with glfw_context():
        context = skia.GrDirectContext.MakeGL()
        info = skia.ImageInfo.MakeN32Premul(width, height)
        surface = skia.Surface.MakeRenderTarget(context, skia.Budgeted.kNo, info)
        assert surface is not None

        with surface as canvas:
            canvas.drawCircle(100, 100, 40, skia.Paint(Color=skia.ColorGREEN))

        image = surface.makeImageSnapshot()
        assert image is not None
        image.save('output.png', skia.kPNG)

OpenGL window
-------------

In the previous example, even though the rendering is executed on the GPU,
nothing is actually shown on the screen. It is possible to use Skia to draw
directly to a window on the screen. To do this:

1.  You have to use GLFW or other similar library to create and manage
    the window.
2.  :py:class:`GrBackendRenderTarget` has to be created with a surface attached
    to it.
3.  The rendered image has to be flushed to the screen by calling
    :py:meth:`Surface.flushAndSubmit`.

Here's a complete example:

.. code-block:: python

    import contextlib, glfw, skia
    from OpenGL import GL

    WIDTH, HEIGHT = 640, 480

    @contextlib.contextmanager
    def glfw_window():
        if not glfw.init():
            raise RuntimeError('glfw.init() failed')
        glfw.window_hint(glfw.STENCIL_BITS, 8)
        window = glfw.create_window(WIDTH, HEIGHT, '', None, None)
        glfw.make_context_current(window)
        yield window
        glfw.terminate()

    @contextlib.contextmanager
    def skia_surface(window):
        context = skia.GrDirectContext.MakeGL()
        (fb_width, fb_height) = glfw.get_framebuffer_size(window)
        backend_render_target = skia.GrBackendRenderTarget(
            fb_width,
            fb_height,
            0,  # sampleCnt
            0,  # stencilBits
            skia.GrGLFramebufferInfo(0, GL.GL_RGBA8))
        surface = skia.Surface.MakeFromBackendRenderTarget(
            context, backend_render_target, skia.kBottomLeft_GrSurfaceOrigin,
            skia.kRGBA_8888_ColorType, skia.ColorSpace.MakeSRGB())
        assert surface is not None
        yield surface
        context.abandonContext()

    with glfw_window() as window:
        GL.glClear(GL.GL_COLOR_BUFFER_BIT)

        with skia_surface(window) as surface:
            with surface as canvas:
                canvas.drawCircle(100, 100, 40, skia.Paint(Color=skia.ColorGREEN))
            surface.flushAndSubmit()
            glfw.swap_buffers(window)

            while (glfw.get_key(window, glfw.KEY_ESCAPE) != glfw.PRESS
                and not glfw.window_should_close(window)):
                glfw.wait_events()

PDF
---

The :py:class:`PDF` backend uses :py:class:`Document` instead of
:py:class:`Surface`, since a document must include multiple pages. The
following demonstrates how to write a single-page PDF document to a file::

    import skia

    width, height = 200, 200
    stream = skia.FILEWStream('output.pdf')
    with skia.PDF.MakeDocument(stream) as document:
        with document.page(width, height) as canvas:
            canvas.drawCircle(100, 100, 40, skia.Paint(Color=skia.ColorGREEN))


Picture
-------

The :py:class:`Picture` backend uses :py:class:`PictureRecorder`
instead of :py:class:`Surface`.

.. code-block:: python

    import skia

    width, height = 200, 200
    recorder = skia.PictureRecorder()
    canvas = recorder.beginRecording(width, height)
    canvas.drawCircle(100, 100, 40, skia.Paint(Color=skia.ColorGREEN))
    picture = recorder.finishRecordingAsPicture()
    data = picture.serialize()


Recorded picture can later be drawn onto another canvas::

    picture.playback(canvas)


SVG
---

The (still experimental) :py:class:`SVGCanvas` writes into an SVG document.

.. code-block:: python

    import skia

    width, height = 200, 200
    stream = skia.FILEWStream('output.svg')
    canvas = skia.SVGCanvas.Make((width, height), stream)
    draw(canvas)
    del canvas
    stream.flush()
