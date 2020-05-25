import skia
import pytest


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
def backend_texture():
    return skia.GrBackendTexture()


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
