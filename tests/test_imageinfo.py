import skia
import pytest
import sys


def check_colorinfo(x):
    assert isinstance(x, skia.ColorInfo)


def test_ColorInfo_init():
    check_colorinfo(skia.ColorInfo())


@pytest.fixture(scope='session')
def imageinfo():
    return skia.ImageInfo.MakeN32Premul(320, 240)


def check_imageinfo(x):
    assert isinstance(x, skia.ImageInfo)


@pytest.mark.parametrize('args', [
    (100, 100, skia.ColorType.kRGBA_8888, skia.AlphaType.kPremul),
    (skia.ISize(100, 100), skia.ColorType.kRGBA_8888, skia.AlphaType.kPremul),
    (skia.ISize(100, 100), skia.ColorInfo()),
])
def test_ImageInfo_Make(args):
    check_imageinfo(skia.ImageInfo.Make(*args))


@pytest.mark.parametrize('args', [
    (100, 100, skia.AlphaType.kPremul),
])
def test_ImageInfo_MakeN32(args):
    check_imageinfo(skia.ImageInfo.MakeN32(*args))


def test_ImageInfo_MakeS32():
    check_imageinfo(skia.ImageInfo.MakeS32(100, 100, skia.AlphaType.kPremul))


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
    check_imageinfo(imageinfo.makeAlphaType(skia.AlphaType.kPremul))


def test_ImageInfo_makeColorType(imageinfo):
    check_imageinfo(imageinfo.makeColorType(skia.ColorType.kRGBA_8888))


# def test_ImageInfo_makeColorSpace(imageinfo):
#     check_imageinfo(imageinfo.makeColorSpace())


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


def test_ImageInfo_ByteSizeOverflowed():
    assert not skia.ImageInfo.ByteSizeOverflowed(100)
