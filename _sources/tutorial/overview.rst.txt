Overview
========

.. currentmodule:: skia

Skia is organized around the :py:class:`Canvas` object. It is the host for
the `draw` calls: :py:meth:`Canvas.drawRect`,
:py:meth:`Canvas.drawPath`, :py:meth:`Canvas.drawTextBlob`, etc.
Each of these has two components: the primitive being drawn
(:py:class:`Rect`, :py:class:`Path`, etc.) and color/style
attributes (:py:class:`Paint`). The following example creates a canvas,
draws a rectangle with specified paint, and writes the result to an image file::

    import skia

    surface = skia.Surface(128, 128)
    canvas = surface.getCanvas()
    rect = skia.Rect(32, 32, 96, 96)
    paint = skia.Paint(
        Color=skia.ColorBLUE,
        Style=skia.Paint.kFill_Style)
    canvas.drawRect(rect, paint)

    image = surface.makeImageSnapshot()
    with open('output.png', 'wb') as f:
        f.write(image.encodeToData())

.. image:: /_static/overview-rect-blue.png
  :width: 128
  :alt: Output rectangle image

The paint holds much of the state describing how the rectangle (in this case) is
drawn: what color it is, if it is filled or stroked, how it should blend with
what was previously drawn::

    paint.setBlendMode(skia.kScreen)

The canvas holds relatively little state. It points to the actual pixels being
drawn, and it maintains a stack of matrices and clips. Thus in the above call,
the canvas' current matrix may transform the coordinates of the rectangle (
translation, rotation, skewing, perspective), and the canvas' current clip may
restrict where on the canvas the rectangle will be drawn, but all other
stylistic attributes of the drawing are controlled by the paint.


Canvas details
--------------

:py:class:`Canvas` is the drawing context for Skia. It knows where to
direct the drawing (i.e. where the screen of offscreen pixels are), and
maintains a stack of matrices and clips. Note however, that unlike similar
contexts in other APIs like postscript, cairo, or awt, Skia does not store any
other drawing attributes in the context (e.g. color, pen size). Rather, these
are specified explicitly in each draw call, via a :py:class:`Paint`::

    import skia

    surface = skia.Surface(256, 256)
    canvas = surface.getCanvas()
    canvas.save()
    canvas.translate(128., 128.)
    canvas.rotate(45.)
    rect = skia.Rect(-90.5, -90.5, 90.5, 90.5)
    paint = skia.Paint(Color=skia.Color(0, 0, 255))
    canvas.drawRect(rect, paint)
    canvas.restore()

    image = surface.makeImageSnapshot()
    with open('output.png', 'wb') as f:
        f.write(image.encodeToData())

.. image:: /_static/overview-rotated-rect.png
    :width: 256
    :alt: Rotated rectangle


The code above will draw a rectangle rotated by 45 degrees. Exactly what color
and style the rect will be drawn in is described by the paint, not the canvas.

To begin with, we might want to erase the entire canvas. We can do this by
drawing an enormous rectangle, but there are easier ways to do it::

    paint = skia.Paint(Color=skia.ColorWHITE)
    canvas.drawPaint(paint)

This fills the entire canvas (though respecting the current clip of course) with
whatever color or shader (and xfermode) is specified by the paint. If there is a
shader in the paint, then it will respect the current matrix on the canvas as
well (see :py:class:`Shader`). If you just want to draw a color (with an
optional xfermode), you can just call :py:meth:`Canvas.drawColor`, and
save yourself having to allocate a paint::

    canvas.drawColor(skia.ColorWHITE)

More drawings
-------------

All of the other draw APIs are similar, each one ending with a paint parameter.
The following example demonstrates drawing of shape primitives,
:py:class:`Path`, external :py:class:`Image`, or
:py:class:`TextBlob` objects onto the canvas::

    import skia

    surface = skia.Surface(256, 256)
    canvas = surface.getCanvas()

    rect = skia.Rect(50, 50, 90, 110)
    paint = skia.Paint(
        AntiAlias=True,
        Style=skia.Paint.kStroke_Style,
        StrokeWidth=4,
        Color=skia.ColorRED,
    )

    canvas.drawColor(skia.ColorWHITE)
    canvas.drawRect(rect, paint)

    oval = skia.RRect.MakeOval(rect)
    oval.offset(40, 60)
    paint.setColor(skia.ColorBLUE)
    canvas.drawRRect(oval, paint)

    paint.setColor(skia.ColorCYAN)
    canvas.drawCircle(180, 50, 25, paint)

    rect.offset(80, 0)
    paint.setColor(skia.ColorYELLOW)
    canvas.drawRoundRect(rect, 10, 10, paint)

    path = skia.Path()
    path.cubicTo(768, 0, -512, 256, 256, 256)
    paint.setColor(skia.ColorGREEN)
    canvas.drawPath(path, paint)

    with open('../skia/resources/images/color_wheel.png', 'rb') as f:
        image = skia.Image.DecodeToRaster(f.read())
    canvas.drawImage(image, 128, 128, paint)

    rect2 = skia.Rect(40, 60)
    canvas.drawImageRect(image, rect2, paint)

    paint2 = skia.Paint()
    text = skia.TextBlob('Hello, Skia!', skia.Font(None, 18))
    canvas.drawTextBlob(text, 50, 25, paint2)

    image = surface.makeImageSnapshot()
    with open('output.png', 'wb') as f:
        f.write(image.encodeToData())

.. image:: /_static/overview-various-draws.png
    :width: 256
    :alt: Various drawings

Python interoperability
-----------------------

`skia-python` supports numpy array in several methods. The following directly
draws into numpy array::

    import numpy as np

    array = np.zeros((320, 240, 4), np.uint8)
    canvas = skia.Canvas(array)
    paint = skia.Paint(AntiAlias=True, Color=skia.ColorCYAN)
    canvas.drawCircle(180, 50, 25, paint)

Alternatively, :py:class:`Canvas` content can be exported to numpy array.

    array = canvas.numpy()

APIs at a glance
----------------

Skia has various APIs from low to high level. Some key class objects are:

- :py:class:`AutoCanvasRestore` - Canvas save stack manager
- :py:class:`Bitmap` - two-dimensional raster pixel array
- :py:class:`BlendMode` - pixel color arithmetic
- :py:class:`Canvas` - drawing context
- :py:class:`Color` - color encoding using integer numbers
- :py:class:`Font` - text style and typeface
- :py:class:`Image` - two dimensional array of pixels to draw
- :py:class:`ImageInfo` - pixel dimensions and characteristics
- :py:class:`IPoint` - two integer coordinates
- :py:class:`IRect` - integer rectangle
- :py:class:`Matrix` - 3x3 transformation matrix
- :py:class:`Paint` - color, stroke, font, effects
- :py:class:`Path` - sequence of connected lines and curves
- :py:class:`Picture` - sequence of drawing commands
- :py:class:`Pixmap` - pixel map: image info and pixel address
- :py:class:`Point` - two floating point coordinates
- :py:class:`RRect` - floating point rounded rectangle
- :py:class:`Rect` - floating point rectangle
- :py:class:`Region` - compressed clipping mask
- :py:class:`Surface` - drawing destination
- :py:class:`TextBlob` - runs of glyphs
- :py:class:`TextBlobBuilder` - constructor for runs of glyphs
