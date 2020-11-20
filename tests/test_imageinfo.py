import skia
import pytest
import sys


@pytest.fixture
def colorinfo():
    return skia.ColorInfo(
        skia.ColorType.kRGBA_8888_ColorType,
        skia.AlphaType.kPremul_AlphaType,
        skia.ColorSpace.MakeSRGB())


@pytest.mark.parametrize('args', [
    tuple(),
    (
        skia.ColorType.kUnknown_ColorType,
        skia.AlphaType.kUnknown_AlphaType,
        skia.ColorSpace.MakeSRGB(),
    ),
])
def test_ColorInfo_init(args):
    assert isinstance(skia.ColorInfo(*args), skia.ColorInfo)


def test_ColorInfo_colorSpace(colorinfo):
    assert isinstance(colorinfo.colorSpace(), (skia.ColorSpace, type(None)))


def test_ColorInfo_refColorSpace(colorinfo):
    assert isinstance(colorinfo.refColorSpace(), (skia.ColorSpace, type(None)))


def test_ColorInfo_colorType(colorinfo):
    assert isinstance(colorinfo.colorType(), skia.ColorType)


def test_ColorInfo_alphaType(colorinfo):
    assert isinstance(colorinfo.alphaType(), skia.AlphaType)


def test_ColorInfo_isOpaque(colorinfo):
    assert isinstance(colorinfo.isOpaque(), bool)


def test_ColorInfo_gammaCloseToSRGB(colorinfo):
    assert isinstance(colorinfo.gammaCloseToSRGB(), bool)


def test_ColorInfo_eq(colorinfo):
    assert colorinfo == colorinfo


def test_ColorInfo_ne(colorinfo):
    assert colorinfo != skia.ColorInfo()


def test_ColorInfo_makeAlphaType(colorinfo):
    assert isinstance(
        colorinfo.makeAlphaType(skia.AlphaType.kOpaque_AlphaType),
        skia.ColorInfo)


def test_ColorInfo_makeColorType(colorinfo):
    assert isinstance(
        colorinfo.makeColorType(skia.ColorType.kAlpha_8_ColorType),
        skia.ColorInfo)


def test_ColorInfo_makeColorSpace(colorinfo):
    assert isinstance(
        colorinfo.makeColorSpace(skia.ColorSpace.MakeSRGBLinear()),
        skia.ColorInfo)


def test_ColorInfo_bytesPerPixel(colorinfo):
    assert isinstance(colorinfo.bytesPerPixel(), int)


def test_ColorInfo_shiftPerPixel(colorinfo):
    assert isinstance(colorinfo.shiftPerPixel(), int)


@pytest.fixture(scope='session')
def imageinfo():
    return skia.ImageInfo.MakeN32Premul(320, 240)


def check_imageinfo(x):
    assert isinstance(x, skia.ImageInfo)


def test_ImageInfo_init():
    check_imageinfo(skia.ImageInfo())


def test_ImageInfo_repr(imageinfo):
    assert isinstance(repr(imageinfo), str)


def test_ImageInfo_width(imageinfo):
    assert imageinfo.width() == 320


def test_ImageInfo_height(imageinfo):
    assert imageinfo.height() == 240


def test_ImageInfo_colorType(imageinfo):
    assert isinstance(imageinfo.colorType(), skia.ColorType)


def test_ImageInfo_alphaType(imageinfo):
    assert isinstance(imageinfo.alphaType(), skia.AlphaType)


def test_ImageInfo_colorSpace(imageinfo):
    assert isinstance(imageinfo.colorSpace(), (skia.ColorSpace, type(None)))


def test_ImageInfo_refColorSpace(imageinfo):
    assert isinstance(imageinfo.refColorSpace(), (skia.ColorSpace, type(None)))


def test_ImageInfo_isEmpty(imageinfo):
    assert not imageinfo.isEmpty()


def test_ImageInfo_colorInfo(imageinfo):
    assert isinstance(imageinfo.colorInfo(), skia.ColorInfo)


def test_ImageInfo_isOpaque(imageinfo):
    assert not imageinfo.isOpaque()


def test_ImageInfo_dimensions(imageinfo):
    assert imageinfo.dimensions().equals(320, 240)


def test_ImageInfo_bounds(imageinfo):
    assert isinstance(imageinfo.bounds(), skia.IRect)


def test_ImageInfo_gammaCloseToSRGB(imageinfo):
    assert not imageinfo.gammaCloseToSRGB()


def test_ImageInfo_makeWH(imageinfo):
    check_imageinfo(imageinfo.makeWH(100, 100))


def test_ImageInfo_makeDimensions(imageinfo):
    check_imageinfo(imageinfo.makeDimensions(skia.ISize(100, 100)))


def test_ImageInfo_makeAlphaType(imageinfo):
    check_imageinfo(imageinfo.makeAlphaType(skia.AlphaType.kPremul_AlphaType))


def test_ImageInfo_makeColorType(imageinfo):
    check_imageinfo(
        imageinfo.makeColorType(skia.ColorType.kRGBA_8888_ColorType))


def test_ImageInfo_makeColorSpace(imageinfo):
    check_imageinfo(imageinfo.makeColorSpace(skia.ColorSpace.MakeSRGBLinear()))


def test_ImageInfo_bytesPerPixel(imageinfo):
    assert imageinfo.bytesPerPixel() == 4


def test_ImageInfo_shiftPerPixel(imageinfo):
    assert imageinfo.shiftPerPixel() == 2


def test_ImageInfo_minRowBytes64(imageinfo):
    assert isinstance(imageinfo.minRowBytes64(), int)


def test_ImageInfo_minRowBytes(imageinfo):
    assert isinstance(imageinfo.minRowBytes(), int)


def test_ImageInfo_computeOffset(imageinfo):
    assert isinstance(imageinfo.computeOffset(0, 0, 4), int)


def test_ImageInfo_operators(imageinfo):
    assert imageinfo == imageinfo
    assert imageinfo != skia.ImageInfo()


def test_ImageInfo_computeByteSize(imageinfo):
    assert isinstance(imageinfo.computeByteSize(64), int)


def test_ImageInfo_computeMinByteSize(imageinfo):
    assert isinstance(imageinfo.computeMinByteSize(), int)


def test_ImageInfo_validRowBytes(imageinfo):
    assert imageinfo.validRowBytes(1280)


def test_ImageInfo_reset():
    imageinfo = skia.ImageInfo()
    imageinfo.reset()


@pytest.mark.parametrize('args', [
    (100, 100, skia.ColorType.kRGBA_8888_ColorType,
        skia.AlphaType.kPremul_AlphaType),
    (100, 100, skia.ColorType.kRGBA_8888_ColorType,
        skia.AlphaType.kPremul_AlphaType, skia.ColorSpace.MakeSRGBLinear()),
    (skia.ISize(100, 100), skia.ColorType.kRGBA_8888_ColorType,
        skia.AlphaType.kPremul_AlphaType),
    (skia.ISize(100, 100), skia.ColorInfo()),
])
def test_ImageInfo_Make(args):
    check_imageinfo(skia.ImageInfo.Make(*args))


@pytest.mark.parametrize('args', [
    (100, 100, skia.AlphaType.kPremul_AlphaType),
])
def test_ImageInfo_MakeN32(args):
    check_imageinfo(skia.ImageInfo.MakeN32(*args))


def test_ImageInfo_MakeS32():
    check_imageinfo(
        skia.ImageInfo.MakeS32(100, 100, skia.AlphaType.kPremul_AlphaType))


@pytest.mark.parametrize('args', [
    (100, 100),
    (skia.ISize(100, 100),),
])
def test_ImageInfo_MakeN32Premul(args):
    check_imageinfo(skia.ImageInfo.MakeN32Premul(*args))


@pytest.mark.parametrize('args', [
    (100, 100),
    (skia.ISize(100, 100),),
])
def test_ImageInfo_MakeA8(args):
    check_imageinfo(skia.ImageInfo.MakeA8(*args))


@pytest.mark.parametrize('args', [
    tuple(),
    (100, 100),
])
def test_ImageInfo_MakeUnknown(args):
    check_imageinfo(skia.ImageInfo.MakeUnknown(*args))


def test_ImageInfo_ByteSizeOverflowed():
    assert not skia.ImageInfo.ByteSizeOverflowed(100)


def test_AlphaTypeIsOpaque():
    assert skia.AlphaTypeIsOpaque(skia.AlphaType.kOpaque_AlphaType)


def test_ColorTypeBytesPerPixel():
    assert skia.ColorTypeBytesPerPixel(skia.ColorType.kRGBA_8888_ColorType) == 4


def test_ColorTypeIsAlwaysOpaque():
    assert skia.ColorTypeIsAlwaysOpaque(skia.ColorType.kRGB_888x_ColorType)


def test_ColorTypeIsAlwaysOpaque():
    assert skia.ColorTypeValidateAlphaType(
        skia.ColorType.kRGBA_8888_ColorType,
        skia.AlphaType.kOpaque_AlphaType)


@pytest.fixture
def yuva_info():
    return skia.YUVAInfo()


def test_YUVAInfo_kMaxPlanes():
    assert isinstance(skia.YUVAInfo.kMaxPlanes, int)


def test_YUVAInfo_PlaneDimensions():
    assert isinstance(
        skia.YUVAInfo.PlaneDimensions(
            (100, 100),
            skia.YUVAInfo.kY_U_V_444,
            skia.kTopLeft_EncodedOrigin),
        list)


def test_YUVAInfo_NumPlanes():
    assert isinstance(skia.YUVAInfo.NumPlanes(skia.YUVAInfo.kY_U_V_444), int)


def test_YUVAInfo_NumChannelsInPlane():
    assert isinstance(
        skia.YUVAInfo.NumChannelsInPlane(skia.YUVAInfo.kY_U_V_444, 0), int)


def test_YUVAInfo_HasAlpha():
    assert isinstance(skia.YUVAInfo.HasAlpha(skia.YUVAInfo.kY_U_V_444), bool)


def test_YUVAInfo_init():
    assert isinstance(skia.YUVAInfo(), skia.YUVAInfo)
    assert isinstance(
        skia.YUVAInfo(
            (100, 100),
            skia.YUVAInfo.kY_U_V_444,
            skia.kJPEG_YUVColorSpace),
        skia.YUVAInfo)


def test_YUVAInfo_planarConfig(yuva_info):
    assert isinstance(yuva_info.planarConfig(), skia.YUVAInfo.PlanarConfig)


def test_YUVAInfo_dimensions(yuva_info):
    assert isinstance(yuva_info.dimensions(), skia.ISize)


def test_YUVAInfo_width(yuva_info):
    assert isinstance(yuva_info.width(), int)


def test_YUVAInfo_height(yuva_info):
    assert isinstance(yuva_info.height(), int)


def test_YUVAInfo_yuvColorSpace(yuva_info):
    assert isinstance(yuva_info.yuvColorSpace(), skia.YUVColorSpace)


def test_YUVAInfo_sitingX(yuva_info):
    assert isinstance(yuva_info.sitingX(), skia.YUVAInfo.Siting)


def test_YUVAInfo_sitingY(yuva_info):
    assert isinstance(yuva_info.sitingY(), skia.YUVAInfo.Siting)


def test_YUVAInfo_origin(yuva_info):
    assert isinstance(yuva_info.origin(), skia.EncodedOrigin)


def test_YUVAInfo_hasAlpha(yuva_info):
    assert isinstance(yuva_info.hasAlpha(), bool)


def test_YUVAInfo_planeDimensions(yuva_info):
    assert isinstance(yuva_info.planeDimensions(), list)


def test_YUVAInfo_computeTotalBytes(yuva_info):
    row_bytes = [100, 100, 100, 100]
    assert isinstance(yuva_info.computeTotalBytes(row_bytes), int)
    _, plane_sizes = yuva_info.computeTotalBytes(row_bytes, True)
    assert isinstance(plane_sizes, list)


def test_YUVAInfo_numPlanes(yuva_info):
    assert isinstance(yuva_info.numPlanes(), int)


def test_YUVAInfo_numChannelsInPlane(yuva_info):
    assert isinstance(yuva_info.numChannelsInPlane(0), int)


def test_YUVAInfo___eq__(yuva_info):
    assert isinstance(yuva_info == yuva_info, bool)


def test_YUVAInfo___ne__(yuva_info):
    assert isinstance(yuva_info != yuva_info, bool)
