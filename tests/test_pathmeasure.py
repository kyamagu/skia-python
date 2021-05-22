import math
import skia
import pytest


@pytest.fixture
def path():
    return skia.Path()


def postan(path_measure, offset):
    postan = path_measure.getPosTan(offset)
    assert postan is not None
    return postan


def test_PathMeasure_init(path):
    assert isinstance(skia.PathMeasure(path, False), skia.PathMeasure)


def test_forceclosed(path):
    path.moveTo(0, 0)
    path.lineTo(1, 0)
    path.lineTo(1, 1)
    path.lineTo(0, 1)
    meas = skia.PathMeasure(path, True)
    assert meas.getLength() == 4


def test_345(path):
    path.moveTo(0, 0)
    path.lineTo(3, 4)
    meas = skia.PathMeasure(path, False)
    assert meas.getLength() == 5


def test_circle(path):
    path.addCircle(0, 0, 1)
    meas = skia.PathMeasure(path, False)
    # Test is also commented in skia/tests/PathMeasureTest.cpp
    # assert meas.getLength() == pytest.approx(2 * math.pi)


def test_close_without_move(path):
    path.lineTo(1, 0)
    path.lineTo(1, 1)
    path.lineTo(0, 1)
    path.close()
    path.lineTo(-1, 0)
    meas = skia.PathMeasure(path, False)
    assert meas.getLength() == 4
    meas.nextContour()
    assert meas.getLength() == 1
    (pos, tan) = postan(meas, 0.5)
    assert pos.x() == pytest.approx(-0.5)
    assert pos.y() == 0.0
    assert tan.x() == -1.0
    assert tan.y() == 0.0


def test_degenerate(path):
    path.moveTo(0, 0)
    path.lineTo(0, 0)
    path.lineTo(1, 0)
    path.quadTo(1, 0, 1, 0)
    path.quadTo(1, 1, 1, 1 * 2)
    path.cubicTo(1, 2, 1, 2, 1, 2)
    path.cubicTo(2, 2, 3, 2, 4, 2)
    meas = skia.PathMeasure(path, False)
    assert meas.getLength() == 6

    (pos, tan) = postan(meas, 0.5)
    assert pos.x() == pytest.approx(0.5)
    assert pos.y() == 0
    assert tan.x() == 1
    assert tan.y() == 0

    (pos, tan) = postan(meas, 2.5)
    assert pos.x() == pytest.approx(1.0)
    assert pos.y() == pytest.approx(1.5)
    assert tan.x() == 0
    assert tan.y() == 1

    (pos, tan) = postan(meas, 4.5)
    assert pos.x() == pytest.approx(2.5)
    assert pos.y() == pytest.approx(2.0)
    assert tan.x() == 1
    assert tan.y() == 0


def test_degenerate2(path):
    path.moveTo(0, 0)
    path.lineTo(1, 0)
    path.moveTo(1, 1)
    path.moveTo(2, 2)
    path.lineTo(1, 2)
    meas = skia.PathMeasure(path, False)
    assert meas.getLength() == 1
    (pos, tan) = postan(meas, 0.5)
    assert pos.x() == pytest.approx(0.5)
    assert pos.y() == 0
    assert tan.x() == 1
    assert tan.y() == 0

    meas.nextContour()
    assert meas.getLength() == 1
    (pos, tan) = postan(meas, 0.5)
    assert pos.x() == pytest.approx(1.5)
    assert pos.y() == pytest.approx(2.0)
    assert tan.x() == -1
    assert tan.y() == 0


def test_conic(path):
    pt = skia.Point(100, 0)
    path.moveTo(0, 0)
    path.conicTo(pt, pt, 1)
    meas = skia.PathMeasure(path, False)
    (stdP, tan) = postan(meas, 20)

    path.reset()
    path.moveTo(0, 0)
    path.conicTo(pt, pt, 10)
    meas.setPath(path, False)
    (hiP, tan) = postan(meas, 20)
    assert 19.5 < stdP.x() and stdP.x() < 20.5
    assert 19.5 < hiP.x() and hiP.x() < 20.5


# Regression test for b/26425223
def test_nextctr(path):
    path.moveTo(0, 0)
    path.lineTo(100, 0)

    meas = skia.PathMeasure(path, False)
    # only expect 1 contour, even if we didn't explicitly call getLength() ourselves
    assert not meas.nextContour()
