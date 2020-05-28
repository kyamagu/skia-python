import skia
import pytest


@pytest.fixture(params=[
    'color_wheel.gif',
    'color_wheel.ico',
    'color_wheel.jpg',
    'color_wheel.png',
    'color_wheel.webp',
    'alphabetAnim.gif',
])
def image_path(request):
    import os
    root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    return os.path.join(
        root_dir, 'skia', 'resources', 'images', request.param)


@pytest.fixture
def codec(image_path):
    with open(image_path, 'rb') as f:
        yield skia.Codec(f.read())


def test_Codec_init(codec):
    assert isinstance(codec, skia.Codec)


def test_Codec_ResultToString():
    assert isinstance(
        skia.Codec.ResultToString(skia.Codec.Result.kSuccess), str)


def test_Codec_MakeFromData(image_path):
    data = skia.Data.MakeFromFileName(image_path)
    assert isinstance(skia.Codec.MakeFromData(data), skia.Codec)


def test_Codec_getInfo(codec):
    assert isinstance(codec.getInfo(), skia.ImageInfo)


def test_Codec_dimensions(codec):
    assert isinstance(codec.dimensions(), skia.ISize)


def test_Codec_bounds(codec):
    assert isinstance(codec.bounds(), skia.IRect)


def test_Codec_getOrigin(codec):
    assert isinstance(codec.getOrigin(), skia.EncodedOrigin)


def test_Codec_getScaledDimensions(codec):
    assert isinstance(codec.getScaledDimensions(1.0), skia.ISize)


def test_Codec_getValidSubset(codec):
    assert isinstance(codec.getValidSubset(skia.IRect(128, 128)), bool)


def test_Codec_getEncodedFormat(codec):
    assert isinstance(codec.getEncodedFormat(), skia.EncodedImageFormat)


@pytest.mark.parametrize('use_pixmap', [False, True])
def test_Codec_getPixels(codec, use_pixmap):
    info = skia.ImageInfo.MakeN32Premul(128, 128)
    data = bytearray(info.computeMinByteSize())
    rowBytes = info.minRowBytes()
    if use_pixmap:
        pixmap = skia.Pixmap(info, data, rowBytes)
        assert isinstance(codec.getPixels(pixmap), skia.Codec.Result)
    else:
        assert isinstance(
            codec.getPixels(info, data, rowBytes), skia.Codec.Result)


def test_Codec_incrementalDecode(codec):
    info = skia.ImageInfo.MakeN32Premul(128, 128)
    data = bytearray(info.computeMinByteSize())
    rowBytes = info.minRowBytes()
    result = codec.startIncrementalDecode(info, data, rowBytes)
    if result == skia.Codec.kSuccess:
        rows = 0
        while codec.incrementalDecode(rows) == skia.Codec.kIncompleteInput:
            assert isinstance(rows, int)
            break


@pytest.fixture
def scanline(codec):
    info = skia.ImageInfo.MakeN32Premul(128, 128)
    rowBytes = info.minRowBytes()
    result = codec.startScanlineDecode(info)
    if result != skia.Codec.kSuccess:
        pytest.skip('Scanline is not supported')
    yield codec, info, rowBytes


def test_Codec_getScanlines(scanline):
    codec, info, rowBytes = scanline
    row = bytearray(rowBytes)
    assert isinstance(codec.getScanlines(row, 1, rowBytes), int)


def test_Codec_skipScanlines(scanline):
    codec, info, rowBytes = scanline
    row = bytearray(rowBytes)
    assert isinstance(codec.skipScanlines(1), bool)


def test_Codec_getScanlineOrder(scanline):
    codec, info, rowBytes = scanline
    assert isinstance(codec.getScanlineOrder(), skia.Codec.ScanlineOrder)


def test_Codec_nextScanline(scanline):
    codec, info, rowBytes = scanline
    assert isinstance(codec.nextScanline(), int)


def test_Codec_outputScanline(scanline):
    codec, info, rowBytes = scanline
    assert isinstance(codec.outputScanline(0), int)


def test_Codec_getFrameCount(codec):
    count = codec.getFrameCount()
    assert isinstance(count, int)
    if count > 1:
        frame_info = skia.Codec.FrameInfo()
        assert isinstance(codec.getFrameInfo(0, frame_info), bool)
        assert isinstance(codec.getFrameInfo(), list)
        assert isinstance(codec.getRepetitionCount(), int)
