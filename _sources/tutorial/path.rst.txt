Path Overview
=============

.. currentmodule:: skia

:py:class:`Path` contains Lines and Curves which can be stroked or filled.
Contour is composed of a series of connected Lines and Curves. :py:class:`Path`
may contain zero, one, or more Contours. Each Line and Curve are described by
:py:class:`~Path.Verb`, list of :py:class:`Point`, and optional
Path_Conic_Weight.

Each pair of connected Lines and Curves share common :py:class:`Point`; for
instance, :py:class:`Path` containing two connected Lines are described the
Path_Verb sequence: :py:attr:`Path.kMove`, :py:attr:`Path.kLine`,
:py:attr:`Path.kLine`; and a Point sequence with three entries, sharing the
middle entry as the end of the first Line and the start of the second Line.

:py:class:`Path` components Arc, Rect, Round_Rect, Circle, and Oval are
composed of Lines and Curves with as many Verbs and Points required for an exact
description. Once added to :py:class:`Path`, these components may lose their
identity; although :py:class:`Path` can be inspected to determine if it
describes a single Rect, Oval, Round_Rect, and so on.

`Example`::

    paint = skia.Paint(AntiAlias=True)
    path = skia.Path()
    path.moveTo(124, 108)
    path.lineTo(172, 24)
    path.addCircle(50, 50, 30)
    path.moveTo(36, 148)
    path.quadTo(66, 188, 120, 136)
    canvas.drawPath(path, paint)

    paint.setStyle(skia.Paint.kStroke_Style);
    paint.setColor(skia.ColorBLUE)
    paint.setStrokeWidth(3)
    canvas.drawPath(path, paint)


Path contains three Contours: Line, Circle, and Quad. Line is stroked but not
filled. Circle is stroked and filled; Circle stroke forms a loop. Quad is
stroked and filled, but since it is not closed, Quad does not stroke a loop.

.. image:: /_static/path-1.png
    :alt: Path 1

Path contains a Path_Fill_Type which determines whether overlapping Contours
form fills or holes. Path_Fill_Type also determines whether area inside or
outside Lines and Curves is filled::

    paint = skia.Paint(AntiAlias=True)
    path = skia.Path()
    path.moveTo(36, 48)
    path.quadTo(66, 88, 120, 36)
    canvas.drawPath(path, paint)

    paint.setStyle(skia.Paint.kStroke_Style)
    paint.setColor(skia.ColorBLUE)
    paint.setStrokeWidth(8)
    canvas.translate(0, 50)
    canvas.drawPath(path, paint)

    paint.setStyle(skia.Paint.kStrokeAndFill_Style)
    paint.setColor(skia.ColorRED)
    canvas.translate(0, 50)
    canvas.drawPath(path, paint)

Path is drawn filled, then stroked, then stroked and filled.

.. image:: /_static/path-2.png
    :alt: Path 2

:py:class:`Path` contents are never shared. Copying :py:class:`Path` by value
effectively creates a new :py:class:`Path` independent of the original.
Internally, the copy does not duplicate its contents until it is edited, to
reduce memory use and improve performance.

-----

Contour contains one or more Verbs, and as many Points as are required to
satisfy Path_Verb_Array. First Path_Verb in :py:class:`Path` is always
:py:attr:`PathVerb.kMove`; each :py:attr:`PathVerb.kMove` that follows starts a
new Contour.

`Example`::

    font = skia.Font()
    paint = skia.Paint(AntiAlias=True)
    canvas.drawString("1st contour", 150, 100, font, paint)
    canvas.drawString("2nd contour", 130, 160, font, paint)
    canvas.drawString("3rd contour", 40, 30, font, paint)
    paint.setStyle(skia.Paint.kStroke_Style)
    path = skia.Path()
    path.moveTo(124, 108)
    path.lineTo(172, 24)
    path.moveTo(36, 148)
    path.quadTo(66, 188, 120, 136)
    path.close()
    path.conicTo(70, 20, 110, 40, 0.6)
    canvas.drawPath(path, paint)

Each :py:meth:`~Path.moveTo` starts a new Contour, and content after
:py:meth:`~Path.close` also starts a new Contour. Since :py:meth:`~Path.conicTo`
is not preceded by :py:meth:`~Path.moveTo`, the first Point of the third Contour
starts at the last Point of the second Contour.

.. image:: /_static/path-3.png
    :alt: Path 3

If final :py:class:`PathVerb` in Contour is :py:attr:`~PathVerb.kClose`, Line
connects Path_Last_Point in Contour with first Point. A closed Contour, stroked,
draws Paint_Stroke_Join at Path_Last_Point and first Point. Without
:py:attr:`~PathVerb.kClose` as final Verb, Path_Last_Point and first Point are
not connected; Contour remains open. An open Contour, stroked, draws
Paint_Stroke_Cap at Path_Last_Point and first Point.

`Example`::

    paint = skia.Paint(
        AntiAlias=True,
        Style=skia.Paint.kStroke_Style,
        StrokeWidth=8)
    path = skia.Path()
    path.moveTo(36, 48)
    path.quadTo(66, 88, 120, 36)
    canvas.drawPath(path, paint)
    path.close()
    canvas.translate(0, 50)
    canvas.drawPath(path, paint)

Path is drawn stroked, with an open Contour and a closed Contour.

.. image:: /_static/path-4.png
    :alt: Path 4

-----

Contour length is distance traveled from first Point to Path_Last_Point, plus,
if Contour is closed, distance from Path_Last_Point to first Point. Even if
Contour length is zero, stroked Lines are drawn if Paint_Stroke_Cap makes them
visible.

`Example`::

    paint = skia.Paint(
        AntiAlias=True,
        Style=skia.Paint.kStroke_Style,
        StrokeWidth=8,
        StrokeCap=skia.Paint.kRound_Cap)
    path = skia.Path()
    path.moveTo(36, 48)
    path.lineTo(36, 48)
    canvas.drawPath(path, paint)
    path.reset()
    paint.setStrokeCap(skia.Paint.kSquare_Cap)
    path.moveTo(56, 48)
    path.close()
    canvas.drawPath(path, paint)

.. image:: /_static/path-5.png
    :alt: Path 5



