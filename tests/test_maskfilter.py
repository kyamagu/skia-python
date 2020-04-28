import skia
import pytest


@pytest.fixture
def maskfilter():
    return skia.MaskFilter.MakeBlur(skia.kNormal_BlurStyle, 1.)


def test_MaskFilter_MakeBlur():
    assert isinstance(
        skia.MaskFilter.MakeBlur(skia.kNormal_BlurStyle, 1.), skia.MaskFilter)


def test_MaskFilter_MakeCombine(maskfilter):
    assert isinstance(
        skia.MaskFilter.MakeCombine(
            maskfilter,
            skia.MaskFilter.MakeBlur(skia.kSolid_BlurStyle, 2.),
            skia.ConvergeMode.kUnion),
        skia.MaskFilter)


def test_MaskFilter_MakeCompose(maskfilter):
    assert isinstance(
        skia.MaskFilter.MakeCompose(
            maskfilter,
            skia.MaskFilter.MakeBlur(skia.kSolid_BlurStyle, 2.)),
        skia.MaskFilter)


def test_MaskFilter_Deserialize(maskfilter):
    assert isinstance(
        skia.MaskFilter.Deserialize(maskfilter.serialize()),
        skia.MaskFilter)


def test_ShaderMaskFilter_Make():
    assert isinstance(
        skia.ShaderMaskFilter.Make(skia.Shaders.Empty()),
        skia.MaskFilter)


def test_TableMaskFilter_MakeGammaTable():
    assert isinstance(skia.TableMaskFilter.MakeGammaTable(1.0), list)


def test_TableMaskFilter_MakeClipTable():
    assert isinstance(skia.TableMaskFilter.MakeClipTable(16, 224), list)


def test_TableMaskFilter_Create():
    assert isinstance(skia.TableMaskFilter.Create(range(256)), skia.MaskFilter)


def test_TableMaskFilter_CreateGammaTable():
    assert isinstance(skia.TableMaskFilter.CreateGamma(1.0), skia.MaskFilter)


def test_TableMaskFilter_CreateClipTable():
    assert isinstance(skia.TableMaskFilter.CreateClip(16, 224), skia.MaskFilter)

