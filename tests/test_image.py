import skia
import pytest


def test_Image_imageInfo(image):
    assert isinstance(image.imageInfo(), skia.ImageInfo)


def test_Image_width(image):
    assert isinstance(image.width(), int)


def test_Image_height(image):
    assert isinstance(image.height(), int)


def test_Image_dimensions(image):
    assert isinstance(image.dimensions(), skia.ISize)


def test_Image_bounds(image):
    assert isinstance(image.bounds(), skia.IRect)


def test_Image_uniqueID(image):
    assert isinstance(image.uniqueID(), int)


def test_Image_alphaType(image):
    assert isinstance(image.alphaType(), skia.AlphaType)


def test_Image_colorType(image):
    assert isinstance(image.colorType(), skia.ColorType)


def test_Image_colorSpace(image):
    assert isinstance(image.colorSpace(), skia.ColorSpace)


def test_Image_refColorSpace(image):
    assert isinstance(image.refColorSpace(), skia.ColorSpace)


def test_Image_isAlphaOnly(image):
    assert isinstance(image.isAlphaOnly(), bool)


def test_Image_isOpaque(image):
    assert isinstance(image.isOpaque(), bool)


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.TileMode.kRepeat, skia.TileMode.kRepeat, None),
])
def test_Image_makeShader(image, args):
    assert isinstance(image.makeShader(*args), skia.Shader)


def test_Image_peekPixels(image):
    pixmap = skia.Pixmap()
    assert isinstance(image.peekPixels(pixmap), bool)


def test_Image_isTextureBacked(image):
    assert isinstance(image.isTextureBacked(), bool)


def test_Image_isValid(image):
    assert isinstance(image.isValid(), bool)


def test_Image_flush(image, grcontext):
    if grcontext is not None:
        image.flush(grcontext)


def test_Image_readPixels(image):
    info = image.imageInfo().makeWH(100, 100)
    dstRowBytes = info.minRowBytes()
    dstPixels = bytearray(info.computeByteSize(dstRowBytes))
    assert isinstance(
        image.readPixels(info, dstPixels, dstRowBytes, 0, 0), bool)


def test_Image_makeColorSpace(image):
    assert isinstance(
        image.makeColorSpace(skia.ColorSpace.MakeSRGB()), skia.Image)
