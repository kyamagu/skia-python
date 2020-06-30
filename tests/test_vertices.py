import skia
import pytest


def test_Vertices_init(vertices):
    assert isinstance(vertices, skia.Vertices)


def test_Vertices_uniqueID(vertices):
    assert isinstance(vertices.uniqueID(), int)


def test_Vertices_bounds(vertices):
    assert isinstance(vertices.bounds(), skia.Rect)


def test_Vertices_approximateSize(vertices):
    assert isinstance(vertices.approximateSize(), int)


@pytest.mark.parametrize('args', [
    (
        skia.Vertices.kTriangles_VertexMode,
        [skia.Point(0, 0), skia.Point(1, 1), skia.Point(1, 0)],
        [skia.Point(1, 1), skia.Point(1, 0), skia.Point(0, 0)],
        [skia.ColorRED, skia.ColorRED, skia.ColorRED],
        [1],
    ),
    (
        skia.Vertices.kTriangles_VertexMode,
        [skia.Point(0, 0), skia.Point(1, 1), skia.Point(1, 0)],
        [skia.Point(1, 1), skia.Point(1, 0), skia.Point(0, 0)],
        [skia.ColorRED, skia.ColorRED, skia.ColorRED],
    ),
    (
        skia.Vertices.kTriangles_VertexMode,
        [skia.Point(0, 0), skia.Point(1, 1), skia.Point(1, 0)],
    ),
])
def test_Vertices_MakeCopy(args):
    assert isinstance(skia.Vertices.MakeCopy(*args), skia.Vertices)
