import skia
import pytest


@pytest.fixture(scope='session')
def path():
    return skia.Path()


@pytest.fixture(scope='session')
def itr():
    return skia.Path.Iter()


@pytest.fixture(scope='session')
def rawiter():
    return skia.Path.RawIter()


@pytest.mark.parametrize('args', [
    tuple(),
])
def test_Path_init(args):
    assert isinstance(skia.Path(*args), skia.Path)


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.Path(), True),
])
def test_Path_Iter_init(args):
    assert isinstance(skia.Path.Iter(*args), skia.Path.Iter)


def test_Path_Iter_setPath(itr):
    itr.setPath(skia.Path(), True)


def test_Path_Iter_next(itr):
    result = itr.next()
    assert isinstance(result, tuple)
    assert isinstance(result[0], skia.Path.Verb)
    assert isinstance(result[1], list)


# def test_Path_Iter_conicWeight(itr):
#     assert isinstance(itr.conicWeight())


def test_Path_Iter_isCloseLine(itr):
    assert isinstance(itr.isCloseLine(), bool)


def test_Path_Iter_isClosedContour(itr):
    assert isinstance(itr.isClosedContour(), bool)


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.Path(),),
])
def test_Path_RawIter_init(args):
    assert isinstance(skia.Path.RawIter(*args), skia.Path.RawIter)


def test_Path_RawIter_setPath(rawiter):
    rawiter.setPath(skia.Path())


def test_Path_RawIter_next(rawiter):
    result = rawiter.next()
    assert isinstance(result, tuple)
    assert isinstance(result[0], skia.Path.Verb)
    assert isinstance(result[1], list)


def test_Path_RawIter_peek(rawiter):
    assert isinstance(rawiter.peek(), skia.Path.Verb)


# def test_Path_RawIter_conicWeight(rawiter):
#     assert isinstance(rawiter.conicWeight())


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.Path(),),
])
def test_Path_init(args):
    assert isinstance(skia.Path(*args), skia.Path)


def test_Path_isInterpolatable(path):
    assert isinstance(path.isInterpolatable(path), bool)


def test_Path_interpolate(path):
    out = skia.Path()
    assert isinstance(path.interpolate(path, .5, out), bool)


def test_Path_getFillType(path):
    assert isinstance(path.getFillType(), skia.PathFillType)


def test_Path_setFillType(path):
    path.setFillType(skia.PathFillType.kWinding)


def test_Path_isInverseFillType(path):
    assert isinstance(path.isInverseFillType(), bool)


def test_Path_toggleInverseFillType(path):
    path.toggleInverseFillType()
    path.toggleInverseFillType()


def test_Path_getConvexityType(path):
    assert isinstance(path.getConvexityType(), skia.PathConvexityType)


def test_Path_getConvexityTypeOrUnknown(path):
    assert isinstance(path.getConvexityTypeOrUnknown(), skia.PathConvexityType)


def test_Path_setConvexityType(path):
    path.setConvexityType(skia.PathConvexityType.kUnknown)


def test_Path_isConvex(path):
    assert isinstance(path.isConvex(), bool)


def test_Path_isOval(path):
    assert isinstance(path.isOval(skia.Rect()), bool)


def test_Path_isRRect(path):
    assert isinstance(path.isRRect(skia.RRect()), bool)


def test_Path_reset(path):
    assert isinstance(path.reset(), skia.Path)


def test_Path_rewind(path):
    assert isinstance(path.rewind(), skia.Path)


def test_Path_isEmpty(path):
    assert isinstance(path.isEmpty(), bool)


def test_Path_isLastContourClosed(path):
    assert isinstance(path.isLastContourClosed(), bool)


def test_Path_isFinite(path):
    assert isinstance(path.isFinite(), bool)


def test_Path_isVolatile(path):
    assert isinstance(path.isVolatile(), bool)


def test_Path_setIsVolatile(path):
    path.setIsVolatile(False)


def test_Path_isLine(path):
    p0 = skia.Point(0, 0)
    p1 = skia.Point(0, 0)
    assert isinstance(path.isLine(p0, p1), bool)


def test_Path_countPoints(path):
    assert isinstance(path.countPoints(), int)


def test_Path_getPoint(path):
    assert isinstance(path.getPoint(0), skia.Point)


@pytest.mark.parametrize('args', [
    tuple(),
    (10,),
])
def test_Path_getPoints(path, args):
    assert isinstance(path.getPoints(*args), list)


@pytest.mark.parametrize('args', [
    tuple(),
    (10,),
])
def test_Path_getVerbs(path, args):
    assert isinstance(path.getVerbs(*args), list)


def test_Path_approximateBytesUsed(path):
    assert isinstance(path.approximateBytesUsed(), int)


def test_Path_swap(path):
    path.swap(skia.Path())


def test_Path_getBounds(path):
    assert isinstance(path.getBounds(), skia.Rect)


def test_Path_updateBoundsCache(path):
    path.updateBoundsCache()


def test_Path_computeTightBounds(path):
    assert isinstance(path.computeTightBounds(), skia.Rect)


def test_Path_conservativelyContainsRect(path):
    assert isinstance(path.conservativelyContainsRect(skia.Rect()), bool)


def test_Path_incReserve(path):
    path.incReserve(0)


def test_Path_shrinkToFit(path):
    path.shrinkToFit()


@pytest.mark.parametrize('args', [
    (0, 0),
    (skia.Point(0, 0),),
])
def test_Path_moveTo(path, args):
    assert isinstance(path.moveTo(*args), skia.Path)


def test_Path_rMoveTo(path):
    assert isinstance(path.rMoveTo(0, 0), skia.Path)


@pytest.mark.parametrize('args', [
    (1, 1),
    (skia.Point(1, 0),),
])
def test_Path_lineTo(path, args):
    assert isinstance(path.lineTo(*args), skia.Path)


def test_Path_rLineTo(path):
    assert isinstance(path.rLineTo(0, 0), skia.Path)


@pytest.mark.parametrize('args', [
    (1, 1, 1, 0),
    (skia.Point(1, 1), skia.Point(1, 0),),
])
def test_Path_quadTo(path, args):
    assert isinstance(path.quadTo(*args), skia.Path)


def test_Path_rQuadTo(path):
    assert isinstance(path.rQuadTo(1, 1, 1, 0), skia.Path)


@pytest.mark.parametrize('args', [
    (1, 1, 1, 0, 1),
    (skia.Point(1, 1), skia.Point(1, 0), 1),
])
def test_Path_conicTo(path, args):
    assert isinstance(path.conicTo(*args), skia.Path)


def test_Path_rConicTo(path):
    assert isinstance(path.rConicTo(1, 1, 1, 0, 1), skia.Path)


@pytest.mark.parametrize('args', [
    (1, 1, 1, 0, 0, 0),
    (skia.Point(1, 1), skia.Point(1, 0), skia.Point(0, 0)),
])
def test_Path_cubicTo(path, args):
    assert isinstance(path.cubicTo(*args), skia.Path)


def test_Path_rCubicTo(path):
    assert isinstance(path.rCubicTo(1, 1, 1, 0, 0, 0), skia.Path)
