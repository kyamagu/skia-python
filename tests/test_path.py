import skia
import pytest


@pytest.fixture
def path():
    path = skia.Path()
    path.addCircle(25, 25, 10)
    path.addRect((50, 60, 70, 70))
    return path


@pytest.fixture
def itr(path):
    return skia.Path.Iter(path, False)


@pytest.fixture
def rawiter(path):
    return skia.Path.RawIter(path)


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


def test_Path_Iter_iter(itr):
    for verb, points in itr:
        assert isinstance(verb, skia.Path.Verb)


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


def test_Path_RawIter_iter(rawiter):
    for verb, points in rawiter:
        assert isinstance(verb, skia.Path.Verb)


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


def test_Path_countVerbs(path):
    assert isinstance(path.countVerbs(), int)


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


@pytest.mark.parametrize('args', [
    (skia.Rect(100, 100), 0, 60, True),
    (50, 0, 0, 50, 50),
    (skia.Point(50, 0), skia.Point(0, 50), 50),
    (50, 50, 0, skia.Path.kSmall_ArcSize, skia.PathDirection.kCW, 0, 50),
    (skia.Point(50, 50), 0, skia.Path.kSmall_ArcSize, skia.PathDirection.kCW,
        skia.Point(0, 50)),
])
def test_Path_arcTo(path, args):
    assert isinstance(path.arcTo(*args), skia.Path)


def test_Path_rArcTo(path):
    assert isinstance(path.rArcTo(
        50, 50, 0, skia.Path.kSmall_ArcSize, skia.PathDirection.kCW, 0, 50),
    skia.Path)


def test_Path_close(path):
    assert isinstance(path.close(), skia.Path)


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.Rect(100, 100), False, skia.PathDirection(0)),
])
def test_Path_isRect(path, args):
    assert isinstance(path.isRect(*args), bool)


@pytest.mark.parametrize('args', [
    (skia.Rect(100, 100),),
    (skia.Rect(100, 100), skia.PathDirection.kCCW),
    (skia.Rect(100, 100), skia.PathDirection.kCW, 0),
    (0, 0, 100, 100),
    (0, 0, 100, 100, skia.PathDirection.kCW),
])
def test_Path_addRect(path, args):
    assert isinstance(path.addRect(*args), skia.Path)


@pytest.mark.parametrize('args', [
    (skia.Rect(100, 100),),
    (skia.Rect(100, 100), skia.PathDirection.kCCW),
    (skia.Rect(100, 100), skia.PathDirection.kCW, 0),
])
def test_Path_addOval(path, args):
    assert isinstance(path.addOval(*args), skia.Path)


def test_Path_addCircle(path):
    assert isinstance(path.addCircle(50, 50, 10), skia.Path)


def test_Path_addArc(path):
    assert isinstance(path.addArc(skia.Rect(100, 100), 0, 90), skia.Path)


@pytest.mark.parametrize('args', [
    (skia.Rect(100, 100), 5, 5,),
    (skia.Rect(100, 100), 5, 5, skia.PathDirection.kCCW),
    (skia.Rect(100, 100), [5] * 8,),
    (skia.Rect(100, 100), [5] * 8, skia.PathDirection.kCCW),
])
def test_Path_addRoundRect(path, args):
    assert isinstance(path.addRoundRect(*args), skia.Path)


@pytest.mark.parametrize('args', [
    (skia.RRect.MakeRect(skia.Rect(100, 100)),),
    (skia.RRect.MakeRect(skia.Rect(100, 100)), skia.PathDirection.kCCW, 0),
])
def test_Path_addRRect(path, args):
    assert isinstance(path.addRRect(*args), skia.Path)


def test_Path_addPoly(path):
    assert isinstance(path.addPoly([skia.Point(10, 10)], True), skia.Path)


@pytest.mark.parametrize('args', [
    (skia.Path(), 0, 0),
    (skia.Path(),),
    (skia.Path(), skia.Matrix(),),
])
def test_Path_addPath(path, args):
    assert isinstance(path.addPath(*args), skia.Path)


def test_Path_reverseAddPath(path):
    assert isinstance(path.reverseAddPath(skia.Path()), skia.Path)


@pytest.mark.parametrize('args', [
    (0, 0, skia.Path(),),
    (0, 0,),
])
def test_Path_offset(path, args):
    path.offset(*args)


@pytest.mark.parametrize('args', [
    (skia.Matrix(),),
    (skia.Matrix(), skia.Path(),),
    (skia.Matrix(), None, skia.ApplyPerspectiveClip.kYes),
])
def test_Path_transform(path, args):
    path.transform(*args)


def test_Path_getLastPt(path):
    assert isinstance(path.getLastPt(skia.Point(0, 0)), bool)


@pytest.mark.parametrize('args', [
    (0, 0),
    (skia.Point(0, 0),),
])
def test_Path_setLastPt(path, args):
    path.setLastPt(*args)


def test_Path_getSegmentMasks(path):
    assert isinstance(path.getSegmentMasks(), int)


def test_Path_dump(path):
    stream = skia.DynamicMemoryWStream()
    path.dump(stream, False, False)


def test_Path_dump_2(path):
    path.dump()


def test_Path_dumpHex(path):
    path.dumpHex()


def test_Path_writeToMemory_readFromMemory(path):
    buf = path.writeToMemory()
    assert isinstance(buf, bytes)
    assert isinstance(path.readFromMemory(buf), int)


def test_Path_serialize(path):
    assert isinstance(path.serialize(), skia.Data)


def test_Path_getGenerationID(path):
    assert isinstance(path.getGenerationID(), int)


def test_Path_isValid(path):
    assert isinstance(path.isValid(), bool)


def test_Path_IsLineDegenerate(path):
    assert isinstance(skia.Path.IsLineDegenerate(
        skia.Point(0, 0), skia.Point(1, 1), False), bool)


def test_Path_IsQuadDegenerate(path):
    assert isinstance(skia.Path.IsQuadDegenerate(
            skia.Point(0, 0), skia.Point(1, 1), skia.Point(1, 0), False), bool)


def test_Path_IsCubicDegenerate(path):
    assert isinstance(skia.Path.IsCubicDegenerate(
            skia.Point(0, 0), skia.Point(1, 1), skia.Point(1, 0.5),
            skia.Point(1, 0), False), bool)


def test_Path_ConvertConicToQuads(path):
    # See https://fiddle.skia.org/c/@Path_ConvertConicToQuads
    conic = [skia.Point(20, 170), skia.Point(80, 170), skia.Point(80, 230)]
    quads = skia.Path.ConvertConicToQuads(conic[0], conic[1], conic[2], .25, 1)
    assert isinstance(quads, list)
    assert len(quads) == 5


def test_Path_eq(path):
    assert path == path


def test_Path_ne(path):
    assert not (path != path)


def test_Path_iter(path):
    for verb, points in path:
        assert isinstance(verb, skia.Path.Verb)
        assert isinstance(points, list)


@pytest.fixture
def path2():
    path = skia.Path()
    path.addCircle(30, 30, 15)
    return path


def test_OpBuilder(path, path2):
    builder = skia.OpBuilder()
    builder.add(path, skia.kUnion_PathOp)
    builder.add(path2, skia.kDifference_PathOp)
    assert isinstance(builder.resolve(), skia.Path)


def test_Op(path, path2):
    assert isinstance(skia.Op(path, path2, skia.kDifference_PathOp), skia.Path)


def test_Simplify(path):
    assert isinstance(skia.Simplify(path), skia.Path)


def test_TightBounds(path):
    assert isinstance(skia.TightBounds(path), skia.Rect)


def test_AsWinding(path):
    assert isinstance(skia.AsWinding(path), skia.Path)
