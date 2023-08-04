import skia
import pytest
import numpy as np


@pytest.fixture
def mipmap_builder(image):
    return skia.MipmapBuilder(image.imageInfo())


def test_MipmapBuilder_countLevels(mipmap_builder):
    assert isinstance(mipmap_builder.countLevels(), int)


def test_MipmapBuilder_level(mipmap_builder):
    assert isinstance(mipmap_builder.level(0), skia.Pixmap)


def test_MipmapBuilder_attachTo(mipmap_builder, image):
    assert isinstance(mipmap_builder.attachTo(image), skia.Image)


def test_Image_buffer(image):
    assert isinstance(memoryview(image.makeRasterImage()), memoryview)


@pytest.mark.parametrize(['args', 'color_type', 'error'], [
    (((100, 100), np.uint8), skia.kAlpha_8_ColorType, None),
    (((100, 100), np.uint8), skia.kGray_8_ColorType, None),
    (((100, 100, 4), np.uint8), skia.kRGB_888x_ColorType, None),
    (((100, 100, 4), np.uint8), skia.kBGRA_8888_ColorType, None),
    (((100, 100, 4), np.uint8), skia.kRGBA_8888_ColorType, None),
    (((100, 100, 1), np.float16), skia.kA16_float_ColorType, None),
    (((100, 100, 4), np.float16), skia.kRGBA_F16_ColorType, None),
    (((100, 100, 4), np.float32), skia.kRGBA_F32_ColorType, None),
    (((100, 100), np.uint8), skia.kRGBA_8888_ColorType, ValueError),
    (((100, 100, 1), np.uint8), skia.kRGBA_8888_ColorType, ValueError),
    (((0, 100, 4), np.uint8), skia.kRGBA_8888_ColorType, ValueError),
    (((100, 0, 4), np.uint8), skia.kRGBA_8888_ColorType, ValueError),
    (((10, 10, 3), np.float64), skia.kRGBA_8888_ColorType, ValueError),
])
def test_Image_fromarray(args, color_type, error):
    if error is not None:
        with pytest.raises(error):
            skia.Image.fromarray(np.zeros(*args), color_type)
    else:
        assert isinstance(
            skia.Image.fromarray(np.zeros(*args), color_type), skia.Image)


def test_Image_frombytes(png_path):
    from PIL import Image
    pil_image = Image.open(png_path)
    image = skia.Image.frombytes(
        pil_image.tobytes(), pil_image.size, skia.kRGBA_8888_ColorType)
    assert isinstance(image, skia.Image)


def test_Image_tobytes(image):
    assert isinstance(image.tobytes(), bytes)
    assert isinstance(image.makeRasterImage().tobytes(), bytes)


def test_Image_open(png_path):
    assert isinstance(skia.Image.open(png_path), skia.Image)
    with open(png_path, 'rb') as f:
        assert isinstance(skia.Image.open(f), skia.Image)


def test_Image_save(image):
    import io
    import os
    import tempfile

    with io.BytesIO() as f:
        image.save(f)

    with tempfile.NamedTemporaryFile(dir=os.path.dirname(__file__)) as t:
        image.save(t.name)


def test_Image_toarray(image):
    assert isinstance(image.toarray(), np.ndarray)


def test_Image_bitmap(image):
    assert isinstance(image.bitmap(), skia.Bitmap)


def test_Image_convert(image):
    converted = image.convert(alphaType=skia.kUnpremul_AlphaType)
    assert isinstance(converted, skia.Image)
    assert converted.alphaType() == skia.kUnpremul_AlphaType


def test_Image_resize(image):
    resized = image.resize(50, 40)
    assert isinstance(resized, skia.Image)
    assert resized.width() == 50
    assert resized.height() == 40


def test_Image_repr(image):
    assert isinstance(repr(image), str)


def test_Image_repr_png(image):
    assert isinstance(image._repr_png_(), bytes)


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
    (skia.TileMode.kRepeat, skia.TileMode.kRepeat),
    (skia.TileMode.kRepeat, skia.TileMode.kRepeat, skia.SamplingOptions()),
    (skia.TileMode.kRepeat, skia.TileMode.kRepeat, skia.SamplingOptions(), None),
])
def test_Image_makeShader(image, args):
    assert isinstance(image.makeShader(*args), skia.Shader)


def test_Image_peekPixels(image):
    assert isinstance(image.peekPixels(skia.Pixmap()), bool)


def test_Image_isTextureBacked(image):
    assert isinstance(image.isTextureBacked(), bool)


def test_Image_isValid(image):
    assert isinstance(image.isValid(), bool)


def test_Image_flush(image, context):
    image.flush(context)


@pytest.mark.parametrize('args', [
    (
        skia.ImageInfo.MakeN32Premul(320, 240),
        bytearray(240 * 320 * 4),
        320 * 4,
    ),
    (
        skia.ImageInfo.MakeN32Premul(320, 240),
        np.zeros((240, 320, 4), np.uint8),
        320 * 4,
    ),
])
def test_Image_readPixels(context, image, args):
    assert isinstance(image.readPixels(*args), bool)
    assert isinstance(image.readPixels(context, *args), bool)
    assert isinstance(image.readPixels(None, *args), bool)


@pytest.mark.parametrize('use_context', [True, False])
def test_Image_readPixels2(context, use_context, image):
    info = image.imageInfo().makeWH(100, 100)
    dstRowBytes = info.minRowBytes()
    dstPixels = bytearray(info.computeByteSize(dstRowBytes))
    dst = skia.Pixmap(info, dstPixels, dstRowBytes)
    if use_context:
        assert isinstance(image.readPixels(context, dst, 0, 0), bool)
    else:
        assert isinstance(image.readPixels(dst, 0, 0), bool)


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

def test_Image_makeSubset(image):
    assert isinstance(image.makeSubset((10, 10)), skia.Image)

def test_Image_hasMipmaps(image):
    assert isinstance(image.hasMipmaps(), bool)

def test_Image_withDefaultMipmaps(context, image):
    assert isinstance(image.withDefaultMipmaps(), (type(None), skia.Image))

def test_Image_makeTextureImage(image, context):
    assert isinstance(
        image.makeTextureImage(
            context, skia.GrMipmapped.kNo, skia.Budgeted.kNo),
        skia.Image)


def test_Image_makeNonTextureImage(image):
    assert isinstance(image.makeNonTextureImage(), skia.Image)


def test_Image_makeRasterImage(image):
    assert isinstance(image.makeRasterImage(), skia.Image)


def test_Image_makeWithFilter(image, context):
    outSubset = skia.IRect(100, 100)
    offset = skia.IPoint(0, 0)
    assert isinstance(
        image.makeWithFilter(
            context,
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


@pytest.fixture
def pixmap():
    info = skia.ImageInfo().MakeN32Premul(100, 100)
    row_bytes = info.minRowBytes()
    pixels = bytearray(info.computeByteSize(row_bytes))
    return skia.Pixmap(info, pixels, row_bytes)


def test_Image_MakeRasterCopy(pixmap):
    assert isinstance(skia.Image.MakeRasterCopy(pixmap), skia.Image)


def test_Image_MakeRasterData():
    info = skia.ImageInfo().MakeN32Premul(100, 100)
    row_bytes = info.minRowBytes()
    pixels = bytearray(info.computeByteSize(row_bytes))
    assert isinstance(
        skia.Image.MakeRasterData(info, pixels, row_bytes), skia.Image)


def test_Image_MakeFromRaster(pixmap):
    assert isinstance(skia.Image.MakeFromRaster(pixmap), skia.Image)


def test_Image_MakeFromBitmap():
    bitmap = skia.Bitmap()
    bitmap.allocPixels(skia.ImageInfo().MakeN32Premul(100, 100))
    assert isinstance(skia.Image.MakeFromBitmap(bitmap), skia.Image)


def test_Image_MakeFromEncoded(png_data):
    assert isinstance(skia.Image.MakeFromEncoded(png_data), skia.Image)


@pytest.fixture(scope='session')
def compressed_data():
    return skia.Data.MakeUninitialized(128 * 128 * 32)


def test_Image_MakeTextureFromCompressed(context, compressed_data):
    assert isinstance(
        skia.Image.MakeTextureFromCompressed(
            context, compressed_data, 128, 128,
            skia.Image.CompressionType.kETC2_RGB8_UNORM),
        skia.Image)


def test_Image_MakeRasterFromCompressed(compressed_data):
    assert isinstance(
        skia.Image.MakeRasterFromCompressed(
            compressed_data, 128, 128,
            skia.Image.CompressionType.kETC2_RGB8_UNORM),
        skia.Image)


@pytest.fixture
def texture(context):
    backend_texture = context.createBackendTexture(
        128, 128, skia.ColorType.kRGBA_8888_ColorType,
        skia.GrMipmapped.kNo, skia.GrRenderable.kNo)
    assert backend_texture.isValid()
    return backend_texture


def test_Image_MakeFromTexture(context, texture):
    assert isinstance(
        skia.Image.MakeFromTexture(
            context,
            texture,
            skia.GrSurfaceOrigin.kTopLeft_GrSurfaceOrigin,
            skia.ColorType.kRGBA_8888_ColorType,
            skia.AlphaType.kPremul_AlphaType,
            None),
        skia.Image)


@pytest.fixture
def compressed_texture(context):
    backend_format = context.compressedBackendFormat(
        skia.Image.CompressionType.kBC1_RGBA8_UNORM)
    if not backend_format.isValid():
        pytest.skip('Backend format is invalid.')
    backend_texture = context.createCompressedBackendTexture(
        128, 128, backend_format, skia.Color4f.kRed, skia.GrMipmapped.kYes)
    if not backend_texture.isValid():
        pytest.skip('Backend texture is invalid.')
    return backend_texture


def test_Image_MakeFromCompressedTexture(context, compressed_texture):
    assert isinstance(
        skia.Image.MakeFromCompressedTexture(
            context,
            compressed_texture,
            skia.GrSurfaceOrigin.kTopLeft_GrSurfaceOrigin,
            skia.AlphaType.kOpaque_AlphaType,
            None),
        skia.Image)


def test_Image_MakeCrossContextFromPixmap(context, pixmap):
    assert isinstance(
        skia.Image.MakeCrossContextFromPixmap(context, pixmap, False),
        skia.Image)


def test_Image_MakeFromAdoptedTexture(context, texture):
    assert isinstance(
        skia.Image.MakeFromAdoptedTexture(
            context, texture, skia.GrSurfaceOrigin.kTopLeft_GrSurfaceOrigin,
            skia.ColorType.kRGBA_8888_ColorType),
        skia.Image)


@pytest.mark.xfail(reason='Not implemented')
def test_Image_MakeFromYUVATexturesCopy(context, texture):
    raise NotImplementedError


@pytest.mark.xfail(reason='Not implemented')
def test_Image_MakeFromYUVATexturesCopyWithExternalBackend(context, texture):
    raise NotImplementedError


@pytest.mark.xfail(reason='Not implemented')
def test_Image_MakeFromYUVATextures(context, texture):
    raise NotImplementedError


@pytest.mark.xfail(reason='Not implemented')
def test_Image_MakeFromYUVAPixmaps(context, texture):
    raise NotImplementedError


@pytest.mark.xfail(reason='Not implemented')
def test_Image_MakeFromNV12TexturesCopy(context, texture):
    raise NotImplementedError


@pytest.mark.xfail(reason='Not implemented')
def test_Image_MakeFromNV12TexturesCopyWithExternalBackend(context, texture):
    raise NotImplementedError


def test_Image_MakeFromPicture(picture):
    assert isinstance(
        skia.Image.MakeFromPicture(picture, (100, 100)),
        (skia.Image, type(None)))


def test_Imag_MakeBackendTextureFromImage(context, image):
    backendTexture = skia.GrBackendTexture()
    assert isinstance(
        skia.Image.MakeBackendTextureFromImage(context, image, backendTexture),
        bool)
