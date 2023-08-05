import skia
import pytest


@pytest.fixture
def strokerec():
    return skia.StrokeRec(skia.StrokeRec.kHairline_InitStyle)


@pytest.mark.parametrize('args', [
    (skia.StrokeRec.kHairline_InitStyle,),
    (skia.Paint(), skia.Paint.kStroke_Style),
    (skia.Paint(), skia.Paint.kStroke_Style, 1),
    (skia.Paint(),),
    (skia.Paint(), 1),
])
def test_StrokeRec_init(args):
    assert isinstance(skia.StrokeRec(*args), skia.StrokeRec)


def test_StrokeRec_getStyle(strokerec):
    assert isinstance(strokerec.getStyle(), skia.StrokeRec.Style)


def test_StrokeRec_getWidth(strokerec):
    assert isinstance(strokerec.getWidth(), float)


def test_StrokeRec_getMiter(strokerec):
    assert isinstance(strokerec.getMiter(), float)


def test_StrokeRec_getCap(strokerec):
    assert isinstance(strokerec.getCap(), skia.Paint.Cap)


def test_StrokeRec_getJoin(strokerec):
    assert isinstance(strokerec.getJoin(), skia.Paint.Join)


def test_StrokeRec_isHairlineStyle(strokerec):
    assert isinstance(strokerec.isHairlineStyle(), bool)


def test_StrokeRec_isFillStyle(strokerec):
    assert isinstance(strokerec.isFillStyle(), bool)


def test_StrokeRec_setFillStyle(strokerec):
    strokerec.setFillStyle()


def test_StrokeRec_setHairlineStyle(strokerec):
    strokerec.setHairlineStyle()


def test_StrokeRec_setStrokeStyle(strokerec):
    strokerec.setStrokeStyle(1., True)


def test_StrokeRec_setStrokeParams(strokerec):
    strokerec.setStrokeParams(skia.Paint.kButt_Cap, skia.Paint.kMiter_Join, 1)


def test_StrokeRec_getResScale(strokerec):
    assert isinstance(strokerec.getResScale(), float)


def test_StrokeRec_setResScale(strokerec):
    strokerec.setResScale(1.)


def test_StrokeRec_needToApply(strokerec):
    assert isinstance(strokerec.needToApply(), bool)


def test_StrokeRec_applyToPath(strokerec):
    assert isinstance(strokerec.applyToPath(skia.Path(), skia.Path()), bool)


def test_StrokeRec_applyToPaint(strokerec):
    strokerec.applyToPaint(skia.Paint())


def test_StrokeRec_getInflationRadius(strokerec):
    assert isinstance(strokerec.getInflationRadius(), float)


def test_StrokeRec_hasEqualEffect(strokerec):
    assert isinstance(strokerec.hasEqualEffect(
        skia.StrokeRec(skia.StrokeRec.kHairline_InitStyle)), bool)


@pytest.mark.parametrize('args', [
    (skia.Paint(), skia.Paint.kStroke_Style,),
    (skia.Paint.kMiter_Join, 1, skia.Paint.kButt_Cap, 1),
])
def test_StrokeRec_GetInflationRadius(args):
    assert isinstance(skia.StrokeRec.GetInflationRadius(*args), float)


@pytest.fixture
def patheffect_dashinfo():
    return skia.PathEffect.DashInfo()


def test_PathEffect_DashInfo_fIntervals(patheffect_dashinfo):
    assert isinstance(patheffect_dashinfo.fIntervals, list)


def test_PathEffect_DashInfo_fCount(patheffect_dashinfo):
    assert isinstance(patheffect_dashinfo.fCount, int)


def test_PathEffect_DashInfo_fPhase(patheffect_dashinfo):
    assert isinstance(patheffect_dashinfo.fPhase, float)


@pytest.mark.skip(reason='"PointData inner class in PathEffect. Gone in m116. May not need REVISIT')
@pytest.fixture
def patheffect_pointdata():
    return skia.PathEffect.PointData()


@pytest.mark.skip(reason='"PointData inner class in PathEffect. Gone in m116. May not need REVISIT')
def test_PathEffect_PointData_fFlags(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fFlags, int)


@pytest.mark.skip(reason='"PointData inner class in PathEffect. Gone in m116. May not need REVISIT')
def test_PathEffect_PointData_fPoints(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fPoints, list)


@pytest.mark.skip(reason='"PointData inner class in PathEffect. Gone in m116. May not need REVISIT')
def test_PathEffect_PointData_fNumPoints(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fNumPoints, int)


@pytest.mark.skip(reason='"PointData inner class in PathEffect. Gone in m116. May not need REVISIT')
def test_PathEffect_PointData_fSize(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fSize, skia.Point)


@pytest.mark.skip(reason='"PointData inner class in PathEffect. Gone in m116. May not need REVISIT')
def test_PathEffect_PointData_fClipRect(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fClipRect, skia.Rect)


@pytest.mark.skip(reason='"PointData inner class in PathEffect. Gone in m116. May not need REVISIT')
def test_PathEffect_PointData_fPath(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fPath, skia.Path)


@pytest.mark.skip(reason='"PointData inner class in PathEffect. Gone in m116. May not need REVISIT')
def test_PathEffect_PointData_fFirst(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fFirst, skia.Path)


@pytest.mark.skip(reason='"PointData inner class in PathEffect. Gone in m116. May not need REVISIT')
def test_PathEffect_PointData_fLast(patheffect_pointdata):
    assert isinstance(patheffect_pointdata.fLast, skia.Path)


@pytest.fixture
def patheffect():
    return skia.DashPathEffect.Make([2., 1.], 0)


def test_PathEffect_filterPath(patheffect):
    dst = skia.Path()
    src = skia.Path()
    src.addCircle(10, 10, 5)
    rec = skia.StrokeRec(skia.StrokeRec.kHairline_InitStyle)
    assert isinstance(patheffect.filterPath(dst, src, rec, None), bool)


@pytest.mark.skip(reason='m116:REVISIT')
def test_PathEffect_computeFastBounds(patheffect):
    patheffect.computeFastBounds(skia.Rect(100, 100), skia.Rect(100, 100))


@pytest.mark.skip(reason='"PointData inner class in PathEffect. Gone in m116. May not need REVISIT')
def test_PathEffect_asPoints(patheffect):
    results = skia.PathEffect.PointData()
    path = skia.Path()
    path.addCircle(10, 10, 5)
    rec = skia.StrokeRec(skia.StrokeRec.kHairline_InitStyle)
    matrix = skia.Matrix()
    assert isinstance(
        patheffect.asPoints(results, path, rec, matrix, None), bool)


def test_PathEffect_asADash(patheffect):
    dashinfo = skia.PathEffect.DashInfo()
    assert isinstance(patheffect.asADash(dashinfo), skia.PathEffect.DashType)


def test_PathEffect_Deserialize(patheffect):
    data = patheffect.serialize()
    assert isinstance(data, skia.Data)
    effect = skia.PathEffect.Deserialize(data)
    assert isinstance(effect, skia.PathEffect)


def test_PathEffect_GetFlattenableType():
    assert isinstance(
        skia.PathEffect.GetFlattenableType(), skia.Flattanable.Type)


def test_DashPathEffect_Make():
    assert isinstance(skia.DashPathEffect.Make([2., 1.], 0), skia.PathEffect)


def test_DiscretePathEffect_Make():
    assert isinstance(skia.DiscretePathEffect.Make(4.0, 1.0), skia.PathEffect)


def test_CornerPathEffect_Make():
    assert isinstance(skia.CornerPathEffect.Make(4.0), skia.PathEffect)


def test_Path1DPathEffect_Make():
    path = skia.Path()
    path.addCircle(10, 10, 5)
    assert isinstance(
        skia.Path1DPathEffect.Make(
            path, 1., 0., skia.Path1DPathEffect.kTranslate_Style),
        skia.PathEffect)


def test_Line2DPathEffect_Make():
    assert isinstance(
        skia.Line2DPathEffect.Make(1, skia.Matrix()), skia.PathEffect)


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
            skia.PathOp.kUnion_PathOp), skia.PathEffect)


def test_MatrixPathEffect_Make():
    assert isinstance(
        skia.MatrixPathEffect.Make(skia.Matrix()), skia.PathEffect)


def test_MatrixPathEffect_MakeTranslate():
    assert isinstance(
        skia.MatrixPathEffect.MakeTranslate(1., 1.), skia.PathEffect)


def test_StrokePathEffect_Make():
    assert isinstance(
        skia.StrokePathEffect.Make(
            3, skia.Paint.kMiter_Join, skia.Paint.kButt_Cap), skia.PathEffect)


def test_TrimPathEffect_Make():
    assert isinstance(
        skia.TrimPathEffect.Make(0.5, 1.0), skia.PathEffect)
