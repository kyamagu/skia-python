import skia
import pytest
import numpy as np


@pytest.fixture
def info():
    return skia.ImageInfo.MakeN32Premul(120, 80, skia.ColorSpace.MakeSRGB())


@pytest.fixture
def bitmap(info):
    bitmap = skia.Bitmap()
    bitmap.allocPixels(info)
    assert not bitmap.isNull()
    yield bitmap


def test_Bitmap_repr(bitmap):
    assert isinstance(repr(bitmap), str)


def test_Bitmap_buffer(bitmap):
    assert isinstance(memoryview(bitmap), memoryview)
    array = np.array(bitmap, copy=False)
    assert array.ndim == 3
    assert array.shape[0] == 80
    assert array.shape[1] == 120
    assert array.shape[2] == 4


@pytest.mark.parametrize('color_type, shape, dtype', [
     (skia.kAlpha_8_ColorType, (32, 16,), np.uint8),
     (skia.kRGB_565_ColorType, (32, 16,), np.uint16),
     (skia.kARGB_4444_ColorType, (32, 16,), np.uint16),
     (skia.kRGBA_8888_ColorType, (32, 16, 4), np.uint8),
     (skia.kRGB_888x_ColorType, (32, 16, 4), np.uint8),
     (skia.kBGRA_8888_ColorType, (32, 16, 4), np.uint8),
     (skia.kRGBA_1010102_ColorType, (32, 16,), np.uint32),
     (skia.kBGRA_1010102_ColorType, (32, 16,), np.uint32),
     (skia.kRGB_101010x_ColorType, (32, 16,), np.uint32),
     (skia.kBGR_101010x_ColorType, (32, 16,), np.uint32),
     (skia.kGray_8_ColorType, (32, 16,), np.uint8),
     (skia.kRGBA_F16Norm_ColorType, (32, 16, 4), np.float16),
     (skia.kRGBA_F16_ColorType, (32, 16, 4), np.float16),
     (skia.kRGBA_F32_ColorType, (32, 16, 4), np.float32),
     (skia.kR8G8_unorm_ColorType, (32, 16, 2), np.uint8),
     (skia.kA16_float_ColorType, (32, 16,), np.float16),
     (skia.kR16G16_float_ColorType, (32, 16, 2), np.float16),
     (skia.kA16_unorm_ColorType, (32, 16,), np.uint16),
     (skia.kR16G16_unorm_ColorType, (32, 16, 2), np.uint16),
     (skia.kR16G16B16A16_unorm_ColorType, (32, 16, 4), np.uint16),
])
def test_Bitmap_buffer_shape_dtype(color_type, shape, dtype):
    bitmap = skia.Bitmap()
    bitmap.allocPixels(
        skia.ImageInfo.Make(16, 32, color_type, skia.kUnpremul_AlphaType))
    assert not bitmap.isNull()
    array = np.array(bitmap, copy=False)
    assert array.shape == shape
    assert array.dtype == dtype


def test_Bitmap_len(bitmap):
    assert len(bitmap) == bitmap.width() * bitmap.height()


@pytest.mark.parametrize('index', [0, (0, 0)])
def test_Bitmap_getitem(bitmap, index):
    assert isinstance(bitmap[index], int)


@pytest.mark.parametrize('color_type, shape, dtype', [
     (skia.kAlpha_8_ColorType, (32, 16,), np.uint8),
     (skia.kRGBA_8888_ColorType, (32, 16, 4), np.uint8),
     (skia.kGray_8_ColorType, (32, 16,), np.uint8),
     (skia.kA16_unorm_ColorType, (32, 16,), np.uint16),
])
def test_Bitmap_array_interface(color_type, shape, dtype):
    from PIL import Image
    bitmap = skia.Bitmap()
    bitmap.allocPixels(
        skia.ImageInfo.Make(16, 32, color_type, skia.kUnpremul_AlphaType))
    assert not bitmap.isNull()
    assert isinstance(Image.fromarray(bitmap), Image.Image)


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.Bitmap(),),
])
def test_Bitmap_init(args):
    assert isinstance(skia.Bitmap(*args), skia.Bitmap)


def test_Bitmap_swap(bitmap):
    other = skia.Bitmap()
    bitmap.swap(other)


def test_Bitmap_pixmap(bitmap):
    assert isinstance(bitmap.pixmap(), skia.Pixmap)


def test_Bitmap_info(bitmap):
    assert isinstance(bitmap.info(), skia.ImageInfo)


def test_Bitmap_width(bitmap):
    assert isinstance(bitmap.width(), int)


def test_Bitmap_height(bitmap):
    assert isinstance(bitmap.height(), int)


def test_Bitmap_colorType(bitmap):
    assert isinstance(bitmap.colorType(), skia.ColorType)


def test_Bitmap_alphaType(bitmap):
    assert isinstance(bitmap.alphaType(), skia.AlphaType)


def test_Bitmap_colorSpace(bitmap):
    assert isinstance(bitmap.colorSpace(), skia.ColorSpace)


def test_Bitmap_refColorSpace(bitmap):
    assert isinstance(bitmap.refColorSpace(), skia.ColorSpace)


def test_Bitmap_bytesPerPixel(bitmap):
    assert isinstance(bitmap.bytesPerPixel(), int)


def test_Bitmap_rowBytesAsPixels(bitmap):
    assert isinstance(bitmap.rowBytesAsPixels(), int)


def test_Bitmap_shiftPerPixel(bitmap):
    assert isinstance(bitmap.shiftPerPixel(), int)


def test_Bitmap_empty(bitmap):
    assert isinstance(bitmap.empty(), bool)


def test_Bitmap_isNull(bitmap):
    assert isinstance(bitmap.isNull(), bool)


def test_Bitmap_drawsNothing(bitmap):
    assert isinstance(bitmap.drawsNothing(), bool)


def test_Bitmap_rowBytes(bitmap):
    assert isinstance(bitmap.rowBytes(), int)


def test_Bitmap_setAlphaType(bitmap):
    assert isinstance(
        bitmap.setAlphaType(skia.AlphaType.kPremul_AlphaType), bool)


def test_Bitmap_getPixels(bitmap):
    view = bitmap.getPixels()
    assert isinstance(view, memoryview)
    assert view.shape == (80, 120, 4)


def test_Bitmap_computeByteSize(bitmap):
    assert isinstance(bitmap.computeByteSize(), int)


def test_Bitmap_isImmutable(bitmap):
    assert isinstance(bitmap.isImmutable(), bool)


def test_Bitmap_setImmutable(bitmap):
    bitmap.setImmutable()


def test_Bitmap_isOpaque(bitmap):
    assert isinstance(bitmap.isOpaque(), bool)


def test_Bitmap_reset(bitmap):
    bitmap.reset()


def test_Bitmap_getBounds(bitmap):
    assert isinstance(bitmap.getBounds(), skia.IRect)


def test_Bitmap_bounds(bitmap):
    assert isinstance(bitmap.bounds(), skia.IRect)


def test_Bitmap_dimensions(bitmap):
    assert isinstance(bitmap.dimensions(), skia.ISize)


def test_Bitmap_getSubset(bitmap):
    assert isinstance(bitmap.getSubset(), skia.IRect)


def test_Bitmap_setInfo(bitmap):
    info = skia.ImageInfo.MakeN32Premul(120, 80, skia.ColorSpace.MakeSRGB())
    assert bitmap.setInfo(info)


def test_Bitmap_tryAllocPixelsFlags(info):
    bitmap = skia.Bitmap()
    assert isinstance(
        bitmap.tryAllocPixelsFlags(info, skia.Bitmap.kZeroPixels_AllocFlag),
        bool)


def test_Bitmap_allocPixelsFlags(info):
    bitmap = skia.Bitmap()
    bitmap.allocPixelsFlags(info, skia.Bitmap.kZeroPixels_AllocFlag)


def test_Bitmap_tryAllocPixels(bitmap):
    assert isinstance(bitmap.tryAllocPixels(), bool)


def test_Bitmap_tryAllocPixels2(bitmap, info):
    assert isinstance(bitmap.tryAllocPixels(info), bool)


def test_Bitmap_allocPixels(bitmap):
    bitmap.allocPixels()


def test_Bitmap_allocPixels2(bitmap, info):
    bitmap.allocPixels(info)


def test_Bitmap_tryAllocN32Pixels():
    bitmap = skia.Bitmap()
    assert isinstance(bitmap.tryAllocN32Pixels(100, 100, True), bool)


def test_Bitmap_allocN32Pixels():
    bitmap = skia.Bitmap()


def test_Bitmap_installPixels(bitmap, info):
    assert isinstance(bitmap.installPixels(info, None, 0), bool)
    pixels = bytearray(info.computeMinByteSize())
    assert isinstance(
        bitmap.installPixels(info, pixels, info.minRowBytes()), bool)


def test_Bitmap_installPixels2(bitmap, pixmap):
    assert isinstance(bitmap.installPixels(pixmap), bool)


def test_Bitmap_setPixels(bitmap, info):
    pixels = bytearray(bitmap.computeByteSize())
    bitmap.setPixels(pixels)


def test_Bitmap_readyToDraw(bitmap):
    assert isinstance(bitmap.readyToDraw(), bool)


def test_Bitmap_getGenerationID(bitmap):
    assert isinstance(bitmap.getGenerationID(), int)


def test_Bitmap_notifyPixelsChanged(bitmap):
    bitmap.notifyPixelsChanged()


def test_Bitmap_eraseColor(bitmap):
    bitmap.eraseColor(0xFFFFFFFF)


def test_Bitmap_eraseARGB(bitmap):
    bitmap.eraseARGB(0xFF, 0xFF, 0xFF, 0xFF)


def test_Bitmap_erase(bitmap):
    bitmap.erase(0xFFFFFFFF, skia.IRect(100, 100))


def test_Bitmap_extractSubset(bitmap):
    dst = skia.Bitmap()
    assert isinstance(bitmap.extractSubset(dst, skia.IRect(50, 50)), bool)


@pytest.mark.parametrize('use_pixmap', [False, True])
def test_Bitmap_readPixels(bitmap, info, use_pixmap):
    dst = bytearray(info.computeMinByteSize())
    if use_pixmap:
        pixmap = skia.Pixmap(info, dst, info.minRowBytes())
        assert isinstance(bitmap.readPixels(pixmap), bool)
    else:
        assert isinstance(
            bitmap.readPixels(info, dst, info.minRowBytes()), bool)


def test_Bitmap_writePixels(bitmap, pixmap):
    assert isinstance(bitmap.writePixels(pixmap), bool)


def test_Bitmap_extractAlpha(bitmap):
    dst = skia.Bitmap()
    assert isinstance(bitmap.extractAlpha(dst), bool)


def test_Bitmap_peekPixels(bitmap):
    assert isinstance(bitmap.peekPixels(skia.Pixmap()), bool)


def test_Bitmap_makeShader(bitmap):
    assert isinstance(bitmap.makeShader(), skia.Shader)


def test_Bitmap_ComputeIsOpaque(bitmap):
    assert isinstance(skia.ComputeIsOpaque(bitmap), bool)
