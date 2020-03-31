import pytest
import skia


@pytest.fixture()
def ipoint():
    return skia.IPoint.Make(4, 3)


def test_IPoint_init():
    assert isinstance(skia.IPoint(4, 3), skia.IPoint)
    assert isinstance(skia.IPoint.Make(4, 3), skia.IPoint)


def test_IPoint_methods(ipoint):
    assert ipoint.x() == 4
    assert ipoint.y() == 3
    assert not ipoint.isZero()
    ipoint.set(4, 3)
    assert (-ipoint.x()) == -4
    ipoint += skia.IPoint.Make(1, 1)
    assert ipoint.equals(5, 4)
    ipoint -= skia.IPoint.Make(1, 1)
    assert ipoint.equals(4, 3)

    ipoint.fX = 4
    ipoint.fY = 3
    assert ipoint.fX == 4
    assert ipoint.fY == 3
    assert ipoint == skia.IPoint.Make(4, 3)
    assert ipoint != skia.IPoint.Make(4, 0)
    assert isinstance(ipoint + ipoint, skia.IPoint)
    assert isinstance(ipoint - ipoint, skia.IPoint)


@pytest.fixture()
def point():
    return skia.Point.Make(4, 3)


def test_Point_init():
    assert isinstance(skia.Point(4, 3), skia.Point)
    assert isinstance(skia.Point.Make(4, 3), skia.Point)


def test_Point_methods(point):
    assert point.x() == 4
    assert point.y() == 3
    assert not point.isZero()
    point.set(4, 3)
    point.iset(4, 3)
    point.iset(skia.IPoint.Make(4, 3))
    point.setAbs(-point)
    assert point.x() == 4
    point.offset(0, 0)
    point.length() == 5
    point.distanceToOrigin() == 5
    assert point.normalize()
    assert point.setNormalize(4, 3)
    assert point.setLength(5)
    assert point.setLength(4, 3, 5)
    point.scale(1, skia.Point.Make(1, 1))
    point.scale(1)
    point.set(4, 3)
    point.negate()
    assert point.x() == -4
    point.negate()
    point += skia.Point.Make(1, 1)
    assert point.equals(5, 4)
    point -= skia.Point.Make(1, 1)
    assert point.equals(4, 3)
    assert (point * 1).x() == 4
    point *= 1
    assert point.x() == 4
    assert point.isFinite()
    assert point.equals(4, 3)
    assert point.cross(point) == 0.0
    assert point.dot(point) == 25

    # skia.Point.Offset()
    # skia.Point.Offset()
    assert skia.Point.Length(4, 3) == 5
    assert skia.Point.Normalize(skia.Point.Make(4, 3)) == 5
    assert skia.Point.Distance(point, skia.Point.Make(0, 0)) == 5
    assert skia.Point.DotProduct(point, point) == 25
    assert skia.Point.CrossProduct(point, point) == 0

    point.fX = 4
    point.fY = 3
    assert point.fX == 4
    assert point.fY == 3
    assert point == skia.Point.Make(4, 3)
    assert point != skia.Point.Make(4, 0)
    assert isinstance(point + point, skia.Point)
    assert isinstance(point - point, skia.Point)


@pytest.fixture()
def point3():
    return skia.Point3.Make(4, 3, 0)


def test_Point3_init():
    assert isinstance(skia.Point3(4, 3, 0), skia.Point3)
    assert isinstance(skia.Point3.Make(4, 3, 0), skia.Point3)


def test_Point3_methods(point3):
    assert point3.x() == 4
    assert point3.y() == 3
    assert point3.z() == 0
    assert point3.length() == 5
    assert point3.normalize()
    point3.set(4, 3, 0)
    assert point3.makeScale(1.0).x() == 4
    point3.scale(1.0)
    assert point3.x() == 4
    assert (-point3).x() == -4
    point3 += skia.Point3.Make(1, 1, 1)
    assert point3.x() == 5
    point3 -= skia.Point3.Make(1, 1, 1)
    assert point3.x() == 4
    assert point3.isFinite()
    assert point3.dot(point3) == 25
    assert isinstance(point3.cross(point3), skia.Point3)

    assert skia.Point3.Length(3, 4, 0) == 5
    assert skia.Point3.DotProduct(point3, point3) == 25
    assert isinstance(skia.Point3.CrossProduct(point3, point3), skia.Point3)

    point3.fX = 4
    point3.fY = 3
    point3.fZ = 0
    assert point3.fX == 4
    assert point3.fY == 3
    assert point3.fZ == 0

    assert point3 == skia.Point3.Make(4, 3, 0)
    assert point3 != skia.Point3.Make(4, 1, 1)
    assert isinstance(point3 + point3, skia.Point3)
    assert isinstance(point3 - point3, skia.Point3)
    assert isinstance(1 * point3, skia.Point3)

