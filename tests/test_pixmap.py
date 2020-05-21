import skia
import pytest


@pytest.fixture
def pixmap():
    info = skia.ImageInfo.MakeN32Premul(100, 100)
    data = bytearray(info.computeMinByteSize())
    yield skia.Pixmap(info, data, info.minRowBytes())


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.ImageInfo.MakeN32Premul(100, 100), None, 400),
])
def test_Pixmap_init(args):
    assert isinstance(skia.Pixmap(*args), skia.Pixmap)


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.ImageInfo.MakeN32Premul(100, 100), None, 400),
])
def test_Pixmap_reset(pixmap, args):
    pixmap.reset(*args)


def test_Pixmap_setColorSpace(pixmap):
    pixmap.setColorSpace(skia.ColorSpace.MakeSRGB())


def test_Pixmap_extractSubset(pixmap):
    assert isinstance(
        pixmap.extractSubset(skia.Pixmap(), skia.IRect.MakeWH(100, 100)),
        bool)


def test_Pixmap_info(pixmap):
    assert isinstance(pixmap.info(), skia.ImageInfo)


def test_Pixmap_rowBytes(pixmap):
    assert isinstance(pixmap.rowBytes(), int)


def test_Pixmap_addr(pixmap):
    assert isinstance(pixmap.addr(), memoryview)


def test_Pixmap_width(pixmap):
    assert isinstance(pixmap.width(), int)


def test_Pixmap_height(pixmap):
    assert isinstance(pixmap.height(), int)


def test_Pixmap_colorType(pixmap):
    assert isinstance(pixmap.colorType(), skia.ColorType)


def test_Pixmap_alphaType(pixmap):
    assert isinstance(pixmap.alphaType(), skia.AlphaType)


def test_Pixmap_colorSpace(pixmap):
    assert isinstance(pixmap.colorSpace(), (skia.ColorSpace, type(None)))


def test_Pixmap_refColorSpace(pixmap):
    assert isinstance(pixmap.refColorSpace(), (skia.ColorSpace, type(None)))


def test_Pixmap_isOpaque(pixmap):
    assert isinstance(pixmap.isOpaque(), bool)


def test_Pixmap_bounds(pixmap):
    assert isinstance(pixmap.bounds(), skia.IRect)


def test_Pixmap_rowBytesAsPixels(pixmap):
    assert isinstance(pixmap.rowBytesAsPixels(), int)


def test_Pixmap_shiftPerPixel(pixmap):
    assert isinstance(pixmap.shiftPerPixel(), int)


def test_Pixmap_computeByteSize(pixmap):
    assert isinstance(pixmap.computeByteSize(), int)


def test_Pixmap_computeIsOpaque(pixmap):
    assert isinstance(pixmap.computeIsOpaque(), bool)


def test_Pixmap_getColor(pixmap):
    assert isinstance(pixmap.getColor(0, 0), int)


def test_Pixmap_getAlphaf(pixmap):
    assert isinstance(pixmap.getAlphaf(0, 0), float)


def test_Pixmap_addr8():
    info = skia.ImageInfo.MakeA8(100, 100)
    data = bytearray(info.computeMinByteSize())
    pixmap = skia.Pixmap(info, data, info.minRowBytes())
    assert isinstance(pixmap.addr8(), memoryview)


def test_Pixmap_addr16():
    info = skia.ImageInfo.Make(
        100, 100, skia.ColorType.kARGB_4444_ColorType,
        skia.AlphaType.kPremul_AlphaType)
    data = bytearray(info.computeMinByteSize())
    pixmap = skia.Pixmap(info, data, info.minRowBytes())
    assert isinstance(pixmap.addr16(), memoryview)


def test_Pixmap_addr32(pixmap):
    assert isinstance(pixmap.addr32(), memoryview)


def test_Pixmap_addr64(pixmap):
    info = skia.ImageInfo.Make(
        100, 100, skia.ColorType.kRGBA_F16_ColorType,
        skia.AlphaType.kPremul_AlphaType)
    data = bytearray(info.computeMinByteSize())
    pixmap = skia.Pixmap(info, data, info.minRowBytes())
    assert isinstance(pixmap.addr64(), memoryview)


@pytest.mark.parametrize('use_pixmap', [False, True])
def test_Pixmap_readPixels(pixmap, use_pixmap):
    info = pixmap.info().makeWH(100, 100)
    dstRowBytes = info.minRowBytes()
    dstPixels = bytearray(info.computeByteSize(dstRowBytes))
    if use_pixmap:
        dst = skia.Pixmap(info, dstPixels, dstRowBytes)
        assert isinstance(pixmap.readPixels(dst, 0, 0), bool)
    else:
        assert isinstance(
            pixmap.readPixels(info, dstPixels, dstRowBytes, 0, 0), bool)


def test_Pixmap_scalePixels(pixmap):
    info = pixmap.info().makeWH(100, 100)
    dstRowBytes = info.minRowBytes()
    dstPixels = bytearray(info.computeByteSize(dstRowBytes))
    dst = skia.Pixmap(info, dstPixels, dstRowBytes)
    assert isinstance(pixmap.scalePixels(dst), bool)


def test_Pixmap_erase(pixmap):
    assert isinstance(pixmap.erase(0xFFFFFFFF), bool)
