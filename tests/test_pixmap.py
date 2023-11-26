import skia
import pytest
import numpy as np

@pytest.mark.parametrize('args', [
    tuple(),
    (skia.ImageInfo.MakeN32Premul(100, 100), None, 400),
    (np.zeros((100, 100, 4), np.uint8),),
])
def test_Pixmap_init(args):
    assert isinstance(skia.Pixmap(*args), skia.Pixmap)


def test_Pixmap_repr(pixmap):
    assert isinstance(repr(pixmap), str)


def test_Pixmap_buffer(pixmap):
    array = np.array(pixmap, copy=False)
    assert array.shape[0] == pixmap.height()
    assert array.shape[1] == pixmap.width()


def test_Pixmap_len(pixmap):
    assert len(pixmap) == pixmap.width() * pixmap.height()


@pytest.mark.parametrize('index', [0, (0, 0)])
def test_Pixmap_getitem(pixmap, index):
    assert isinstance(pixmap[index], int)


def test_Pixmap_array_interface(pixmap):
    from PIL import Image
    assert isinstance(Image.fromarray(pixmap), Image.Image)


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


@pytest.fixture
def supported_data_types():
    return skia.YUVAPixmapInfo.SupportedDataTypes.All()


@pytest.mark.xfail(reason='m116: The `SkYUVAPixmapInfo::SupportedDataTypes(const GrImageContext&)` constructor has been removed from the public API.')
def test_YUVAPixmapInfo_SupportedDataTypes___init__(context):
    assert isinstance(
        skia.YUVAPixmapInfo.SupportedDataTypes(context),
        skia.YUVAPixmapInfo.SupportedDataTypes)


def test_YUVAPixmapInfo_SupportedDataTypes_All():
    assert isinstance(
        skia.YUVAPixmapInfo.SupportedDataTypes.All(),
        skia.YUVAPixmapInfo.SupportedDataTypes)


def test_YUVAPixmapInfo_SupportedDataTypes_supported(supported_data_types):
    assert isinstance(
        supported_data_types.supported(
            skia.YUVAInfo.kY_U_V, skia.YUVAPixmapInfo.kUnorm8),
        bool)


def test_YUVAPixmapInfo_SupportedDataTypes_enableDataType(supported_data_types):
    supported_data_types.enableDataType(skia.YUVAPixmapInfo.kUnorm8, 3)


@pytest.fixture
def yuva_pixmap_info():
    return skia.YUVAPixmapInfo(
        skia.YUVAInfo(
            (100, 100),
            skia.YUVAInfo.PlaneConfig.kY_U_V,
            skia.YUVAInfo.Subsampling.k444,
            skia.kJPEG_YUVColorSpace),
        skia.YUVAPixmapInfo.kUnorm8)


def test_YUVAPixmapInfo_DefaultColorTypeForDataType():
    assert isinstance(
        skia.YUVAPixmapInfo.DefaultColorTypeForDataType(
            skia.YUVAPixmapInfo.kUnorm8, 4),
        skia.ColorType)


def test_YUVAPixmapInfo_NumChannelsAndDataType():
    assert isinstance(
        skia.YUVAPixmapInfo.NumChannelsAndDataType(skia.kRGBA_8888_ColorType),
        tuple)


@pytest.mark.parametrize('args', [
    (),
    (skia.YUVAInfo(), [
        skia.kRGBA_8888_ColorType,
        skia.kRGBA_8888_ColorType,
        skia.kRGBA_8888_ColorType,
        skia.kRGBA_8888_ColorType]),
    (skia.YUVAInfo(), skia.YUVAPixmapInfo.kUnorm8),
])
def test_YUVAPixmapInfo___init__(args):
    assert isinstance(skia.YUVAPixmapInfo(*args), skia.YUVAPixmapInfo)


def test_YUVAPixmapInfo___eq__(yuva_pixmap_info):
    assert isinstance(yuva_pixmap_info == yuva_pixmap_info, bool)


def test_YUVAPixmapInfo___ne__(yuva_pixmap_info):
    assert isinstance(yuva_pixmap_info != yuva_pixmap_info, bool)


def test_YUVAPixmapInfo_yuvaInfo(yuva_pixmap_info):
    assert isinstance(yuva_pixmap_info.yuvaInfo(), skia.YUVAInfo)


def test_YUVAPixmapInfo_yuvColorSpace(yuva_pixmap_info):
    assert isinstance(yuva_pixmap_info.yuvColorSpace(), skia.YUVColorSpace)


def test_YUVAPixmapInfo_numPlanes(yuva_pixmap_info):
    assert isinstance(yuva_pixmap_info.numPlanes(), int)


def test_YUVAPixmapInfo_dataType(yuva_pixmap_info):
    assert isinstance(yuva_pixmap_info.dataType(), skia.YUVAPixmapInfo.DataType)


def test_YUVAPixmapInfo_rowBytes(yuva_pixmap_info):
    assert isinstance(yuva_pixmap_info.rowBytes(0), int)


def test_YUVAPixmapInfo_planeInfo(yuva_pixmap_info):
    assert isinstance(yuva_pixmap_info.planeInfo(0), skia.ImageInfo)


def test_YUVAPixmapInfo_computeTotalBytes(yuva_pixmap_info):
    assert isinstance(yuva_pixmap_info.computeTotalBytes(), int)
    assert isinstance(yuva_pixmap_info.computeTotalBytes(True), tuple)


def test_YUVAPixmapInfo_initPixmapsFromSingleAllocation(yuva_pixmap_info):
    memory = bytearray(yuva_pixmap_info.computeTotalBytes())
    assert isinstance(
        yuva_pixmap_info.initPixmapsFromSingleAllocation(memory),
        list)


def test_YUVAPixmapInfo_isValid(yuva_pixmap_info):
    assert isinstance(yuva_pixmap_info.isValid(), bool)


def test_YUVAPixmapInfo_isSupported(yuva_pixmap_info, supported_data_types):
    assert isinstance(yuva_pixmap_info.isSupported(supported_data_types), bool)


@pytest.fixture
def yuva_pixmaps(yuva_pixmap_info):
    return skia.YUVAPixmaps.Allocate(yuva_pixmap_info)


def test_YUVAPixmaps_Allocate(yuva_pixmap_info):
    assert isinstance(
        skia.YUVAPixmaps.Allocate(yuva_pixmap_info), skia.YUVAPixmaps)


def test_YUVAPixmaps_FromData(yuva_pixmap_info):
    data = skia.Data.MakeUninitialized(yuva_pixmap_info.computeTotalBytes())
    assert isinstance(
        skia.YUVAPixmaps.FromData(yuva_pixmap_info, data),
        skia.YUVAPixmaps)


def test_YUVAPixmaps_FromExternalMemory(yuva_pixmap_info):
    memory = bytearray(yuva_pixmap_info.computeTotalBytes())
    assert isinstance(
        skia.YUVAPixmaps.FromExternalMemory(yuva_pixmap_info, memory),
        skia.YUVAPixmaps)


def test_YUVAPixmaps_FromExternalPixmaps():
    info = skia.YUVAInfo()
    pixmaps = [
        skia.Pixmap(),
        skia.Pixmap(),
        skia.Pixmap(),
        skia.Pixmap(),
    ]
    assert isinstance(
        skia.YUVAPixmaps.FromExternalPixmaps(info, pixmaps),
        skia.YUVAPixmaps)


def test_YUVAPixmaps_isValid(yuva_pixmaps):
    assert isinstance(yuva_pixmaps.isValid(), bool)


def test_YUVAPixmaps_yuvaInfo(yuva_pixmaps):
    assert isinstance(yuva_pixmaps.yuvaInfo(), skia.YUVAInfo)


def test_YUVAPixmaps_numPlanes(yuva_pixmaps):
    assert isinstance(yuva_pixmaps.numPlanes(), int)


def test_YUVAPixmaps_planes(yuva_pixmaps):
    assert isinstance(yuva_pixmaps.planes(), list)


def test_YUVAPixmaps_plane(yuva_pixmaps):
    assert isinstance(yuva_pixmaps.plane(0), skia.Pixmap)


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_YUVAPixmaps_toLegacy(yuva_pixmaps):
    info, indices = yuva_pixmaps.toLegacy()
    assert isinstance(info, skia.YUVASizeInfo)
    assert isinstance(indices, list)


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
@pytest.fixture
def yuva_index(yuva_pixmaps):
    _, indices = yuva_pixmaps.toLegacy()
    if len(indices) == 0:
        pytest.skip('YUVAIndex unavailable.')
    return indices[0]


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVAIndex___eq__(yuva_index):
    assert yuva_index == yuva_index


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVAIndex___ne__(yuva_index):
    assert isinstance(yuva_index != yuva_index, bool)


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVAIndex_kIndexCount(yuva_index):
    assert isinstance(yuva_index.kIndexCount, int)


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVAIndex_fIndex(yuva_index):
    assert isinstance(yuva_index.fIndex, int)


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVAIndex_fChannel(yuva_index):
    assert isinstance(yuva_index.fChannel, skia.ColorChannel)


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVAIndex_AreValidIndices(yuva_index):
    assert isinstance(
        skia.YUVAIndex.AreValidIndices(
            [yuva_index, yuva_index, yuva_index, yuva_index]),
        bool)
    assert isinstance(
        skia.YUVAIndex.AreValidIndices(
            [yuva_index, yuva_index, yuva_index, yuva_index],
            True),
        tuple)


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
@pytest.fixture
def yuva_size_info(yuva_pixmaps):
    info, _ = yuva_pixmaps.toLegacy()
    return info


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVASizeInfo_fSizes(yuva_size_info):
    value = yuva_size_info.fSizes
    assert isinstance(value, list)
    yuva_size_info.fSizes = value


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVASizeInfo_fWidthBytes(yuva_size_info):
    value = yuva_size_info.fWidthBytes
    assert isinstance(value, list)
    yuva_size_info.fWidthBytes = value


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVASizeInfo_fOrigin(yuva_size_info):
    assert isinstance(yuva_size_info.fOrigin, skia.EncodedOrigin)
    yuva_size_info.fOrigin = skia.kDefault_EncodedOrigin


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVASizeInfo___eq__(yuva_size_info):
    assert yuva_size_info == yuva_size_info


@pytest.mark.skip(reason='"toLegacy()" method. Gone in m116. May not need REVISIT')
def test_SkYUVASizeInfo_computeTotalBytes(yuva_size_info):
    assert isinstance(yuva_size_info.computeTotalBytes(), int)
