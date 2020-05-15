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
