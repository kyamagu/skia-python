import skia
import pytest


@pytest.mark.parametrize('args', [
    tuple(),
    ([1.0, 2.0,], 1.0),
])
def test_PathEffect_DashInfo_init(args):
    assert isinstance(skia.PathEffect.DashInfo(*args), skia.PathEffect.DashInfo)


@pytest.fixture
def patheffect_dashinfo():
    return skia.PathEffect.DashInfo()


def test_PathEffect_DashInfo_fIntervals(patheffect_dashinfo):
    patheffect_dashinfo.fIntervals = [1.0, 2.0]
    assert isinstance(patheffect_dashinfo.fIntervals, list)
    assert patheffect_dashinfo.fCount == 2


def test_PathEffect_DashInfo_fPhase(patheffect_dashinfo):
    patheffect_dashinfo.fPhase = 2.0
    assert patheffect_dashinfo.fPhase == 2.0


def test_PathEffect_PointData_init():
    assert isinstance(skia.PathEffect.PointData(), skia.PathEffect.PointData)


@pytest.fixture
def patheffect_pointdata():
    return skia.PathEffect.PointData()


def test_PathEffect_PointData_fFlags(patheffect_pointdata):
    patheffect_pointdata.fFlags = skia.PathEffect.PointData.PointFlags.kCircles
    assert (patheffect_pointdata.fFlags ==
        skia.PathEffect.PointData.PointFlags.kCircles)


def test_PathEffect_PointData_fPoints(patheffect_pointdata):
    patheffect_pointdata.fPoints = [skia.Point(0, 0)]
    assert isinstance(patheffect_pointdata.fPoints, list)
    assert patheffect_pointdata.fPoints[0] == skia.Point(0, 0)
    assert patheffect_pointdata.fNumPoints == 1


def test_PathEffect_PointData_fSize(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fSize, skia.Point)


def test_PathEffect_PointData_fClipRect(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fClipRect, skia.Rect)


def test_PathEffect_PointData_fPath(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fPath, skia.Path)


def test_PathEffect_PointData_fFirst(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fFirst, skia.Path)


def test_PathEffect_PointData_fLast(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fLast, skia.Path)


def test_DiscretePathEffect_Make():
    assert isinstance(skia.DiscretePathEffect.Make(4.0, 1.0), skia.PathEffect)


def test_DashPathEffect_Make():
    assert isinstance(skia.DashPathEffect.Make([2., 1.], 0), skia.PathEffect)


def test_CornerPathEffect_Make():
    assert isinstance(skia.CornerPathEffect.Make(4.0), skia.PathEffect)


def test_Path1DPathEffect_Make():
    path = skia.Path()
    path.addCircle(10, 10, 5)
    assert isinstance(
        skia.Path1DPathEffect.Make(
            path, 1., 0., skia.Path1DPathEffect.Style.kTranslate),
        skia.PathEffect)


def test_Path2DPathEffect_Make():
    path = skia.Path()
    path.addCircle(10, 10, 5)
    assert isinstance(
        skia.Path2DPathEffect.Make(skia.Matrix(), path), skia.PathEffect)


def test_MergePathEffect_Make():
    assert isinstance(
        skia.MergePathEffect.Make(
            skia.CornerPathEffect.Make(4.0),
            skia.DiscretePathEffect.Make(4.0, 1.0),
            skia.PathOp.kUnion), skia.PathEffect)


def test_MatrixPathEffect_Make():
    assert isinstance(
        skia.MatrixPathEffect.Make(skia.Matrix()), skia.PathEffect)


def test_MatrixPathEffect_MakeTranslate():
    assert isinstance(
        skia.MatrixPathEffect.MakeTranslate(1., 1.), skia.PathEffect)


def test_StrokePathEffect_Make():
    assert isinstance(
        skia.StrokePathEffect.Make(
            3, skia.Paint.Join.kMiter, skia.Paint.Cap.kButt), skia.PathEffect)


def test_TrimPathEffect_Make():
    assert isinstance(
        skia.TrimPathEffect.Make(0.5, 1.0), skia.PathEffect)
