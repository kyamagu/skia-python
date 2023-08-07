import skia
import pytest


@pytest.fixture
def imagefilter():
    return skia.ImageFilters.Blur(1.0, 1.0)


@pytest.fixture
def croprect():
    return skia.ImageFilter.CropRect(skia.Rect(100, 100))


@pytest.mark.parametrize('args', [tuple(), (skia.Rect(100, 100),)])
def test_ImageFilter_CropRect_init(args):
    assert isinstance(
        skia.ImageFilter.CropRect(*args), skia.ImageFilter.CropRect)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ImageFilter_CropRect_flags(croprect):
    assert isinstance(croprect.flags(), int)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ImageFilter_CropRect_rect(croprect):
    assert isinstance(croprect.rect(), skia.Rect)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ImageFilter_CropRect_applyTo(croprect):
    assert isinstance(
        croprect.applyTo(skia.IRect(100, 100), skia.Matrix(), False),
        skia.IRect)


@pytest.mark.parametrize('args', [
    (skia.IRect(100, 100), skia.Matrix(),
        skia.ImageFilter.kForward_MapDirection),
    (skia.IRect(100, 100), skia.Matrix(),
        skia.ImageFilter.kReverse_MapDirection, skia.IRect(100, 100))
])
def test_ImageFilter_filterBounds(imagefilter, args):
    assert isinstance(imagefilter.filterBounds(*args), skia.IRect)


def test_ImageFilter_isColorFilterNode(imagefilter):
    assert isinstance(
        imagefilter.isColorFilterNode(), (skia.ColorFilter, type(None)))


def test_ImageFilter_countInputs(imagefilter):
    assert isinstance(imagefilter.countInputs(), int)


def test_ImageFilter_getInput(imagefilter):
    assert isinstance(imagefilter.getInput(0), (skia.ImageFilter, type(None)))


def test_ImageFilter_computeFastBounds(imagefilter):
    assert isinstance(
        imagefilter.computeFastBounds(skia.Rect(100, 100)), skia.Rect)


def test_ImageFilter_canComputeFastBounds(imagefilter):
    assert isinstance(imagefilter.canComputeFastBounds(), bool)


def test_ImageFilter_makeWithLocalMatrix(imagefilter):
    assert isinstance(
        imagefilter.makeWithLocalMatrix(skia.Matrix()), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ImageFilter_MakeMatrixFilter(imagefilter):
    assert isinstance(
        skia.ImageFilter.MakeMatrixFilter(
            skia.Matrix(), skia.FilterQuality.kHigh_FilterQuality,
            imagefilter), skia.ImageFilter)


def test_ImageFilter_Deserialize(imagefilter):
    assert isinstance(
        skia.ImageFilter.Deserialize(imagefilter.serialize()),
        skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_AlphaThresholdFilter_Make():
    assert isinstance(
        skia.AlphaThresholdFilter.Make(
            skia.Region(), 0, 1), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ArithmeticImageFilter_Make(imagefilter):
    assert isinstance(
        skia.ArithmeticImageFilter.Make(1, 0, 0, 0, False, imagefilter),
        skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_BlurImageFilter_Make():
    assert isinstance(
        skia.BlurImageFilter.Make(1, 1), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ColorFilterImageFilter_Make():
    assert isinstance(
        skia.ColorFilterImageFilter.Make(skia.ColorFilters.LinearToSRGBGamma()),
        skia.ImageFilter)


# def test_ComposeImageFilter_Make(imagefilter):
#     assert isinstance(
#         skia.ComposeImageFilter.Make(imagefilter, imagefilter),
#         skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_DisplacementMapEffect_Make(imagefilter):
    assert isinstance(
        skia.DisplacementMapEffect.Make(
            skia.DisplacementMapEffect.kR_ChannelSelectorType,
            skia.DisplacementMapEffect.kG_ChannelSelectorType,
            1.0, imagefilter, imagefilter),
        skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_DropShadowImageFilter_Make():
    assert isinstance(
        skia.DropShadowImageFilter.Make(
            1, 1, 1., 1., 0xFF000000,
            skia.DropShadowImageFilter.kDrawShadowAndForeground_ShadowMode),
        skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_DilateImageFilter_Make():
    assert isinstance(skia.DilateImageFilter.Make(2, 2), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ErodeImageFilter_Make():
    assert isinstance(skia.ErodeImageFilter.Make(2, 2), skia.ImageFilter)


@pytest.mark.xfail(reason='SkImageFilters::AlphaThreshold removed in m116')
def test_ImageFilters_AlphaThreshold():
    assert isinstance(skia.ImageFilters.AlphaThreshold(
        skia.Region(), 0, 1), skia.ImageFilter)


def test_ImageFilters_Arithmetic(imagefilter):
    assert isinstance(
        skia.ImageFilters.Arithmetic(1, 0, 0, 0, False, imagefilter),
        skia.ImageFilter)


def test_ImageFilters_Blur():
    assert isinstance(skia.ImageFilters.Blur(1., 1.), skia.ImageFilter)


def test_ImageFilters_ColorFilter():
    assert isinstance(
        skia.ImageFilters.ColorFilter(skia.ColorFilters.LinearToSRGBGamma()),
        skia.ImageFilter)


def test_ImageFilters_Compose(imagefilter):
    assert isinstance(
        skia.ImageFilters.Compose(imagefilter, imagefilter), skia.ImageFilter)


def test_ImageFilters_DisplacementMap(imagefilter):
    assert isinstance(skia.ImageFilters.DisplacementMap(
            skia.ColorChannel.kR, skia.ColorChannel.kG, 1.0, imagefilter,
            imagefilter), skia.ImageFilter)


def test_ImageFilters_DropShadow():
    assert isinstance(
        skia.ImageFilters.DropShadow(1., 1., 4., 4., 0xFF000000),
        skia.ImageFilter)


def test_ImageFilters_DropShadowOnly():
    assert isinstance(
        skia.ImageFilters.DropShadowOnly(1., 1., 4., 4., 0xFF000000),
        skia.ImageFilter)


@pytest.mark.parametrize('args', [
    (skia.Rect(100, 100), skia.Rect(100, 100),),
    tuple(),
])
def test_ImageFilters_Image(image, args):
    assert isinstance(skia.ImageFilters.Image(image, *args), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ImageFilters_Magnifier():
    assert isinstance(
        skia.ImageFilters.Magnifier(skia.Rect(100, 100), 1., 1., skia.SamplingOptions()), skia.ImageFilter)


def test_ImageFilters_MatrixConvolution():
    assert isinstance(skia.ImageFilters.MatrixConvolution(
        skia.ISize(5, 5), [1. for i in range(25)], 1. / 25., 0,
        skia.IPoint(2, 2), skia.TileMode.kClamp, False), skia.ImageFilter)


def test_ImageFilters_MatrixTransform():
    assert isinstance(skia.ImageFilters.MatrixTransform(
        skia.Matrix(), skia.SamplingOptions()), skia.ImageFilter)


def test_ImageFilters_Merge(imagefilter):
    assert isinstance(skia.ImageFilters.Merge(
        [imagefilter, imagefilter]), skia.ImageFilter)


def test_ImageFilters_Offset():
    assert isinstance(skia.ImageFilters.Offset(0, 0), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ImageFilters_Paint():
    assert isinstance(skia.ImageFilters.Paint(skia.Paint()), skia.ImageFilter)


def test_ImageFilters_Picture():
    picture = skia.Picture.MakePlaceholder(skia.Rect(100, 100))
    assert isinstance(skia.ImageFilters.Picture(picture), skia.ImageFilter)


def test_ImageFilters_Tile():
    assert isinstance(
        skia.ImageFilters.Tile(skia.Rect(10, 10), skia.Rect(100, 100)),
        skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_ImageFilters_Xfermode():
    assert isinstance(
        skia.ImageFilters.Xfermode(skia.BlendMode.kSrc), skia.ImageFilter)


def test_ImageFilters_Dilate():
    assert isinstance(skia.ImageFilters.Dilate(1, 1), skia.ImageFilter)


def test_ImageFilters_Erode():
    assert isinstance(skia.ImageFilters.Erode(1, 1), skia.ImageFilter)


def test_ImageFilters_DistantLitDiffuse():
    assert isinstance(skia.ImageFilters.DistantLitDiffuse(
        skia.Point3(1, 0, 1), 0xFFFFFFFF, 1., 1.), skia.ImageFilter)


def test_ImageFilters_PointLitDiffuse():
    assert isinstance(skia.ImageFilters.PointLitDiffuse(
        skia.Point3(1, 0, 1), 0xFFFFFFFF, 1., 1.), skia.ImageFilter)


def test_ImageFilters_SpotLitDiffuse():
    assert isinstance(skia.ImageFilters.SpotLitDiffuse(
        skia.Point3(1, 0, 1), skia.Point3(-1, 0, -1), 1.2, 60, 0xFFFFFFFF, 1.,
        1.), skia.ImageFilter)


def test_ImageFilters_DistantLitSpecular():
    assert isinstance(skia.ImageFilters.DistantLitSpecular(
        skia.Point3(1, 0, 1), 0xFFFFFFFF, 1., 1., 1.), skia.ImageFilter)


def test_ImageFilters_PointLitSpecular():
    assert isinstance(skia.ImageFilters.PointLitSpecular(
        skia.Point3(1, 0, 1), 0xFFFFFFFF, 1., 1., 1.), skia.ImageFilter)


def test_ImageFilters_SpotLitSpecular():
    assert isinstance(skia.ImageFilters.SpotLitSpecular(
        skia.Point3(1, 0, 1), skia.Point3(-1, 0, -1), 1.2, 60, 0xFFFFFFFF, 1.,
        1., 1.), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_LightingImageFilter_MakeDistantLitDiffuse():
    assert isinstance(skia.LightingImageFilter.MakeDistantLitDiffuse(
        skia.Point3(1, 0, 1), 0xFFFFFFFF, 1., 1.), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_LightingImageFilter_MakePointLitDiffuse():
    assert isinstance(skia.LightingImageFilter.MakePointLitDiffuse(
        skia.Point3(1, 0, 1), 0xFFFFFFFF, 1., 1.), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_LightingImageFilter_MakeSpotLitDiffuse():
    assert isinstance(skia.LightingImageFilter.MakeSpotLitDiffuse(
        skia.Point3(1, 0, 1), skia.Point3(-1, 0, -1), 1.2, 60, 0xFFFFFFFF, 1.,
        1.), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_LightingImageFilter_MakeDistantLitSpecular():
    assert isinstance(skia.LightingImageFilter.MakeDistantLitSpecular(
        skia.Point3(1, 0, 1), 0xFFFFFFFF, 1., 1., 1.), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_LightingImageFilter_MakePointLitSpecular():
    assert isinstance(skia.LightingImageFilter.MakePointLitSpecular(
        skia.Point3(1, 0, 1), 0xFFFFFFFF, 1., 1., 1.), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_LightingImageFilter_MakeSpotLitSpecular():
    assert isinstance(skia.LightingImageFilter.MakeSpotLitSpecular(
        skia.Point3(1, 0, 1), skia.Point3(-1, 0, -1), 1.2, 60, 0xFFFFFFFF, 1.,
        1., 1.), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_MagnifierImageFilter_Make():
    assert isinstance(
        skia.MagnifierImageFilter.Make(skia.Rect(100, 100), 1.),
        skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_OffsetImageFilter_Make():
    assert isinstance(skia.OffsetImageFilter.Make(0, 0), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_PaintImageFilter_Make():
    assert isinstance(
        skia.PaintImageFilter.Make(skia.Paint()), skia.ImageFilter)


# def test_PictureImageFilter_Make():
#     picture = skia.Picture.MakePlaceholder(skia.Rect(100, 100))
#     assert isinstance(skia.PictureImageFilter.Make(picture), skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_TileImageFilter_Make():
    assert isinstance(
        skia.TileImageFilter.Make(skia.Rect(10, 10), skia.Rect(100, 100)),
        skia.ImageFilter)


@pytest.mark.skip(reason='m116:REVISIT')
def test_XfermodeImageFilter_Make():
    assert isinstance(
        skia.XfermodeImageFilter.Make(skia.BlendMode.kSrc), skia.ImageFilter)
