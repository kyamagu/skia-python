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


@pytest.mark.parametrize('use_pixmap', [False, True])
def test_Image_readPixels(image, use_pixmap):
    info = image.imageInfo().makeWH(100, 100)
    dstRowBytes = info.minRowBytes()
    dstPixels = bytearray(info.computeByteSize(dstRowBytes))
    if use_pixmap:
        dst = skia.Pixmap(info, dstPixels, dstRowBytes)
        assert isinstance(image.readPixels(dst, 0, 0), bool)
    else:
        assert isinstance(
            image.readPixels(info, dstPixels, dstRowBytes, 0, 0), bool)


def test_Image_scalePixels(image):
    info = image.imageInfo().makeWH(100, 100)
    dstRowBytes = info.minRowBytes()
    dstPixels = bytearray(info.computeByteSize(dstRowBytes))
    dst = skia.Pixmap(info, dstPixels, dstRowBytes)
    assert isinstance(image.scalePixels(dst), bool)


@pytest.mark.parametrize('args', [
    (skia.EncodedImageFormat.kJPEG, 100),
    tuple(),
])
def test_Image_encodeToData(image, args):
    assert isinstance(image.encodeToData(*args), skia.Data)


def test_Image_refEncodedData(image):
    assert isinstance(image.refEncodedData(), skia.Data)


def test_Image_makeTextureImage(image, grcontext):
    if grcontext:
        assert isinstance(
            image.makeTextureImage(
                grcontext, skia.GrMipMapped.kNo, skia.Budgeted.kNo),
            skia.Image)


def test_Image_makeNonTextureImage(image):
    assert isinstance(image.makeNonTextureImage(), skia.Image)


def test_Image_makeRasterImage(image):
    assert isinstance(image.makeRasterImage(), skia.Image)


def test_Image_makeWithFilter(image, grcontext):
    outSubset = skia.IRect(100, 100)
    offset = skia.IPoint(0, 0)
    assert isinstance(
        image.makeWithFilter(
            grcontext,
            skia.ImageFilters.Blur(1., 1.),
            skia.IRect(100, 100),
            skia.IRect(100, 100),
            outSubset,
            offset),
        skia.Image)


def test_Image_asLegacyBitmap(image):
    bitmap = skia.Bitmap()
    assert isinstance(image.asLegacyBitmap(bitmap), bool)


def test_Image_isLazyGenerated(image):
    assert isinstance(image.isLazyGenerated(), bool)


def test_Image_makeColorSpace(image):
    assert isinstance(
        image.makeColorSpace(skia.ColorSpace.MakeSRGB()), skia.Image)


def test_Image_makeColorSpace(image):
    assert isinstance(
        image.makeColorTypeAndColorSpace(
            skia.ColorType.kRGBA_8888_ColorType,
            skia.ColorSpace.MakeSRGB()),
        skia.Image)


def test_Image_reinterpretColorSpace(image):
    assert isinstance(
        image.reinterpretColorSpace(skia.ColorSpace.MakeSRGB()), skia.Image)
