Usage
=====

Overview
--------

Skia is organized around the :py:class:`~skia.Canvas` object. It is the host for
the "draw" calls: :py:meth:`~skia.Canvas.drawRect`,
:py:meth:`~skia.Canvas.drawPath`, :py:meth:`~skia.Canvas.drawTextBlob`, etc.
Each of these has two components: the primitive being drawn
(:py:class:`~skia.Rect`, :py:class:`~skia.Path`, etc.) and color/style
attributes (:py:class:`~skia.Paint`)::

    canvas.drawRect(rect, paint)

The paint holds much of the state describing how the rectangle (in this case) is
drawn: what color it is, if it is filled or stroked, how it should blend with
what was previously drawn.

The canvas holds relatively little state. It points to the actual pixels being
drawn, and it maintains a stack of matrices and clips. Thus in the above call,
the canvas' current matrix may transform the coordinates of the rectangle (
translation, rotation, skewing, perspective), and the canvas' current clip may
restrict where on the canvas the rectangle will be drawn, but all other
stylistic attributes of the drawing are controlled by the paint.

Some key classes are:

- :py:class:`~skia.AutoCanvasRestore` - Canvas save stack manager
- :py:class:`~skia.Bitmap` - two-dimensional raster pixel array
- :py:class:`~skia.BlendMode` - pixel color arithmetic
- :py:class:`~skia.Canvas` - drawing context
- :py:class:`~skia.Color` - color encoding using integer numbers
- :py:class:`~skia.Font` - text style and typeface
- :py:class:`~skia.Image` - two dimensional array of pixels to draw
- :py:class:`~skia.ImageInfo` - pixel dimensions and characteristics
- :py:class:`~skia.IPoint` - two integer coordinates
- :py:class:`~skia.IRect` - integer rectangle
- :py:class:`~skia.Matrix` - 3x3 transformation matrix
- :py:class:`~skia.Paint` - color, stroke, font, effects
- :py:class:`~skia.Path` - sequence of connected lines and curves
- :py:class:`~skia.Picture` - sequence of drawing commands
- :py:class:`~skia.Pixmap` - pixel map: image info and pixel address
- :py:class:`~skia.Point` - two floating point coordinates
- :py:class:`~skia.RRect` - floating point rounded rectangle
- :py:class:`~skia.Rect` - floating point rectangle
- :py:class:`~skia.Region` - compressed clipping mask
- :py:class:`~skia.Surface` - drawing destination
- :py:class:`~skia.TextBlob` - runs of glyphs
- :py:class:`~skia.TextBlobBuilder` - constructor for runs of glyphs

Examples
--------

- `Showcase`_
- `Canvas Overview`_
- `Canvas Creation`_
- `Path Overview`_
- `Paint Overview`_

.. _Showcase: https://github.com/kyamagu/skia-python/blob/master/notebooks/Showcase.ipynb
.. _Canvas Overview: https://github.com/kyamagu/skia-python/blob/master/notebooks/Canvas-Overview.ipynb
.. _Canvas Creation: https://github.com/kyamagu/skia-python/blob/master/notebooks/Canvas-Creation.ipynb
.. _Path Overview: https://github.com/kyamagu/skia-python/blob/master/notebooks/Path-Overview.ipynb
.. _Paint Overview: https://github.com/kyamagu/skia-python/blob/master/notebooks/Paint-Overview.ipynb

Python binding
--------------

`skia-python` is built upon `pybind11`_. Python bindings are mostly plain
conversion from `Skia C++ API`_. Function names remain the same.

.. _pybind11: https://github.com/pybind/pybind11
.. _Skia C++ API: https://api.skia.org/

A few differences are:

- All bindings reside in ``skia`` module.
- Removes class name prefix ``Sk``; e.g., ``SkCanvas`` is ``skia.Canvas``.
- Some method signatures adapt to Python style; e.g, :py:meth:`skia.Surface.__init__`.
