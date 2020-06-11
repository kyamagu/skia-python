import skia
import pytest
from datetime import timedelta


@pytest.fixture(scope='module')
def backend_semaphore():
    return skia.GrBackendSemaphore()


def test_GrBackendSemaphore_isInitialized(backend_semaphore):
    assert isinstance(backend_semaphore.isInitialized(), bool)


@pytest.fixture(scope='module')
def backend_format():
    return skia.GrBackendFormat()


# def test_GrBackendFormat_MakeGL(backend_format):
#     assert isinstance(backend_format.MakeGL(), skia.GrBackendFormat)


def test_GrBackendFormat_MakeMock(backend_format):
    assert isinstance(
        backend_format.MakeMock(
            skia.GrColorType.kBGRA_8888, skia.Image.kBC1_RGBA8_UNORM),
        skia.GrBackendFormat)


def test_GrBackendFormat_backend(backend_format):
    assert isinstance(backend_format.backend(), skia.GrBackendApi)


def test_GrBackendFormat_textureType(backend_format):
    assert isinstance(backend_format.textureType(), skia.GrTextureType)


def test_GrBackendFormat_isValid(backend_format):
    assert isinstance(backend_format.isValid(), bool)


@pytest.fixture(scope='module')
def backend_texture(context):
    gl_info = skia.GrGLTextureInfo()
    return skia.GrBackendTexture(256, 256, skia.GrMipMapped.kNo, gl_info)


def test_GrBackendTexture_init_glInfo():
    gl_info = skia.GrGLTextureInfo()
    assert isinstance(
        skia.GrBackendTexture(128, 128, skia.GrMipMapped.kNo, gl_info),
        skia.GrBackendTexture)


def test_GrBackendTexture_init_mockInfo():
    mock_info = skia.GrMockTextureInfo()
    assert isinstance(
        skia.GrBackendTexture(128, 128, skia.GrMipMapped.kNo, mock_info),
        skia.GrBackendTexture)


def test_GrBackendTexture_dimensions(backend_texture):
    assert isinstance(backend_texture.dimensions(), skia.ISize)


def test_GrBackendTexture_width(backend_texture):
    assert isinstance(backend_texture.width(), int)


def test_GrBackendTexture_height(backend_texture):
    assert isinstance(backend_texture.height(), int)


def test_GrBackendTexture_hasMipMaps(backend_texture):
    assert isinstance(backend_texture.hasMipMaps(), bool)


def test_GrBackendTexture_getGLTextureInfo(backend_texture):
    gl_info = skia.GrGLTextureInfo()
    assert isinstance(backend_texture.getGLTextureInfo(gl_info), bool)


def test_GrBackendTexture_glTextureParametersModified(backend_texture):
    backend_texture.glTextureParametersModified()


def test_GrBackendTexture_getBackendFormat(backend_texture):
    assert isinstance(backend_texture.getBackendFormat(), skia.GrBackendFormat)


def test_GrBackendTexture_getMockTextureInfo(backend_texture):
    mock_info = skia.GrMockTextureInfo()
    assert isinstance(backend_texture.getMockTextureInfo(mock_info), bool)


def test_GrBackendTexture_isProtected(backend_texture):
    assert isinstance(backend_texture.isProtected(), bool)


def test_GrBackendTexture_isValid(backend_texture):
    assert isinstance(backend_texture.isValid(), bool)


def test_GrBackendTexture_isSameTexture(backend_texture):
    assert isinstance(backend_texture.isSameTexture(backend_texture), bool)


@pytest.fixture
def grflushinfo():
    return skia.GrFlushInfo()


def test_GrFlushInfo_fFlag(grflushinfo):
    assert isinstance(grflushinfo.fFlags, skia.GrFlushFlags)


def test_GrFlushInfo_fNumSemaphores(grflushinfo):
    assert isinstance(grflushinfo.fNumSemaphores, int)


def test_GrFlushInfo_semaphores(grflushinfo, backend_semaphore):
    grflushinfo.semaphores = [backend_semaphore]
    assert isinstance(grflushinfo.semaphores, list)


@pytest.fixture
def backend_render_target():
    return skia.GrBackendRenderTarget()


def test_GrBackendRenderTarget_init(backend_render_target):
    assert isinstance(backend_render_target, skia.GrBackendRenderTarget)


def test_GrBackendRenderTarget_isValid(backend_render_target):
    assert isinstance(backend_render_target.isValid(), bool)


def test_GrBackendRenderTarget_isFramebufferOnly(backend_render_target):
    assert isinstance(backend_render_target.isFramebufferOnly(), bool)


def test_GrContext_resetContext(context):
    context.resetContext()


def test_GrContext_resetGLTextureBindings(context):
    context.resetGLTextureBindings()


@pytest.mark.skip(reason='This destroys the context')
def test_GrContext_abandonContext(context):
    context.abandonContext()


@pytest.mark.skip(reason='This destroys the context')
def test_GrContext_releaseResourcesAndAbandonContext(context):
    context.releaseResourcesAndAbandonContext()


def test_GrContext_getResourceCacheLimit(context):
    assert isinstance(context.getResourceCacheLimit(), int)


def test_GrContext_getResourceCacheUsage(context):
    resource_count, max_resource_bytes = 0, 0
    context.getResourceCacheUsage(resource_count, max_resource_bytes)


def test_GrContext_getResourceCachePurgeableBytes(context):
    assert isinstance(context.getResourceCachePurgeableBytes(), int)


def test_GrContext_setResourceCacheLimit(context):
    context.setResourceCacheLimit((1 << 16))


def test_GrContext_freeGpuResources(context):
    context.freeGpuResources()


def test_GrContext_performDeferredCleanup(context):
    context.performDeferredCleanup(timedelta(milliseconds=1000))


def test_GrContext_purgeResourcesNotUsedInMs(context):
    context.purgeResourcesNotUsedInMs(timedelta(milliseconds=1000))


@pytest.mark.parametrize('args', [
    (1 << 16, True),
    (True,),
])
def test_GrContext_purgeUnlockedResources(context, args):
    context.purgeUnlockedResources(*args)


def test_GrContext_maxTextureSize(context):
    assert isinstance(context.maxTextureSize(), int)


def test_GrContext_maxRenderTargetSize(context):
    assert isinstance(context.maxRenderTargetSize(), int)


def test_GrContext_colorTypeSupportedAsImage(context):
    assert isinstance(context.colorTypeSupportedAsImage(
        skia.ColorType.kRGBA_8888_ColorType), bool)


def test_GrContext_colorTypeSupportedAsSurface(context):
    assert isinstance(context.colorTypeSupportedAsSurface(
        skia.ColorType.kRGBA_8888_ColorType), bool)


def test_GrContext_maxSurfaceSampleCountForColorType(context):
    assert isinstance(context.maxSurfaceSampleCountForColorType(
        skia.ColorType.kRGBA_8888_ColorType), int)


def test_GrContext_wait(context, backend_semaphore):
    assert isinstance(context.wait([backend_semaphore]), bool)


def test_GrContext_flush(context, grflushinfo):
    assert isinstance(context.flush(grflushinfo), skia.GrSemaphoresSubmitted)


def test_GrContext_flush2(context):
    context.flush()


def test_GrContext_flushAndSubmit(context):
    context.flushAndSubmit()


def test_GrContext_submit(context):
    context.submit()


def test_GrContext_checkAsyncWorkCompletion(context):
    context.checkAsyncWorkCompletion()


def test_GrContext_supportsDistanceFieldText(context):
    assert isinstance(context.supportsDistanceFieldText(), bool)


def test_GrContext_storeVkPipelineCacheData(context):
    context.storeVkPipelineCacheData()


def test_GrContext_defaultBackendFormat(context):
    assert isinstance(
        context.defaultBackendFormat(
            skia.ColorType.kRGBA_8888_ColorType, skia.GrRenderable.kNo),
        skia.GrBackendFormat)


@pytest.mark.parametrize('args', [
    (64, 64, skia.GrBackendFormat(), skia.GrMipMapped.kNo,
        skia.GrRenderable.kNo),
    (64, 64, skia.ColorType.kRGBA_8888_ColorType, skia.GrMipMapped.kNo,
        skia.GrRenderable.kNo),
    pytest.param((skia.SurfaceCharacterization(),), marks=pytest.mark.skip),
    (64, 64, skia.GrBackendFormat(), 0xFFFFFFFF, skia.GrMipMapped.kNo,
        skia.GrRenderable.kNo),
    (64, 64, skia.ColorType.kRGBA_8888_ColorType, 0xFFFFFFFF,
        skia.GrMipMapped.kNo, skia.GrRenderable.kNo),
    pytest.param(
        (skia.SurfaceCharacterization(), 0xFFFFFFFF),
        marks=pytest.mark.skip),
])
def test_GrContext_createBackendTexture(context, args, request):
    assert isinstance(
        context.createBackendTexture(*args), skia.GrBackendTexture)


def test_GrContext_createBackendTexture_2(context, pixmap):
    assert isinstance(
        context.createBackendTexture([pixmap], skia.GrRenderable.kNo),
        skia.GrBackendTexture)


def test_GrContext_createBackendTexture_3(context, pixmap):
    assert isinstance(
        context.createBackendTexture(pixmap, skia.GrRenderable.kNo),
        skia.GrBackendTexture)


def test_GrContext_updateBackendTexture_1(context):
    backend_texture = context.createBackendTexture(
        64, 64, skia.ColorType.kRGBA_8888_ColorType, 0xFFFFFFFF,
        skia.GrMipMapped.kNo, skia.GrRenderable.kNo)
    assert isinstance(
        context.updateBackendTexture(backend_texture, skia.ColorBLACK), bool)


def test_GrContext_updateBackendTexture_2(context, pixmap):
    backend_texture = context.createBackendTexture(
        64, 64, skia.ColorType.kRGBA_8888_ColorType, 0xFFFFFFFF,
        skia.GrMipMapped.kNo, skia.GrRenderable.kNo)
    assert isinstance(
        context.updateBackendTexture(backend_texture, [pixmap]), bool)


def test_GrContext_compressedBackendFormat(context):
    assert isinstance(
        context.compressedBackendFormat(skia.Image.kBC1_RGBA8_UNORM),
        skia.GrBackendFormat)


@pytest.mark.parametrize('args', [
    (64, 64, skia.GrBackendFormat(), 0xFFFFFFFF, skia.GrMipMapped.kNo),
    (64, 64, skia.Image.kBC1_RGBA8_UNORM, 0xFFFFFFFF, skia.GrMipMapped.kNo),
    (16, 16, skia.GrBackendFormat(), bytearray(256), skia.GrMipMapped.kNo),
    (16, 16, skia.Image.kBC1_RGBA8_UNORM, bytearray(256), skia.GrMipMapped.kNo),
])
def test_GrContext_createCompressedBackendTexture(context, args):
    assert isinstance(
        context.createCompressedBackendTexture(*args),
        skia.GrBackendTexture)


def test_GrContext_deleteBackendTexture(context):
    backend_texture = context.createBackendTexture(
        64, 64, skia.ColorType.kRGBA_8888_ColorType, 0xFFFFFFFF,
        skia.GrMipMapped.kNo, skia.GrRenderable.kNo)
    context.deleteBackendTexture(backend_texture)


@pytest.mark.skip()
def test_GrContext_precompileShader(context):
    assert isinstance(context.precompileShader(b'', b''), bool)


def test_GrContext_ComputeImageSize(image):
    assert isinstance(
        skia.GrContext.ComputeImageSize(image, skia.GrMipMapped.kYes),
        int)


def test_GrContext_MakeGL(context):
    assert isinstance(context, skia.GrContext)


def test_GrContext_MakeMock():
    assert isinstance(
        skia.GrContext.MakeMock(skia.GrMockOptions(), skia.GrContextOptions()),
        skia.GrContext)
