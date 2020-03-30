import skia
import pytest


def draw(canvas):
    from math import cos, sin
    scale = 256.
    R = 0.45 * scale
    TAU = 6.2831853

    path = skia.Path()
    path.moveTo(R, 0.)
    for i in range(7):
        theta = 3 * i * TAU / 7
        path.lineTo(R * cos(theta), R * sin(theta))
    path.close()

    paint = skia.Paint()
    paint.setAntiAlias(True)

    canvas.clear(0xFFFFFFFF)
    canvas.translate(0.5 * scale, 0.5 * scale)
    canvas.drawPath(path, paint)


def test_skia():
    surface = skia.Surface(256, 256)
    canvas = surface.getCanvas()
    draw(canvas)
    image = surface.makeImageSnapshot()
    data = image.encodeToData()
    encoded = bytes(data)


def test_numpy():
    import numpy as np
    array = np.zeros((240, 320, 4), dtype=np.uint8)
    surface = skia.Surface(array)
    draw(surface.getCanvas())
