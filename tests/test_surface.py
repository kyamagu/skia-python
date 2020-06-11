import skia
import pytest
import numpy as np
import sys


def check_surface(x):
    assert isinstance(x, skia.Surface)


@pytest.mark.parametrize('args', [
    (np.zeros((240, 320, 4), dtype=np.uint8),),
])
def test_Surface_init(args):
    check_surface(skia.Surface(*args))


def test_Surface_width(surface):
    assert surface.width() == 320


def test_Surface_height(surface):
    assert surface.height() == 240


def test_Surface_imageInfo(surface):
    assert isinstance(surface.imageInfo(), skia.ImageInfo)


def test_Surface_getContext(surface):
    assert isinstance(surface.getContext(), (type(None), skia.GrContext))


def test_Surface_getBackendTexture(surface):
    assert isinstance(
        surface.getBackendTexture(skia.Surface.kFlushRead_BackendHandleAccess),
        skia.GrBackendTexture)


def test_Surface_getBackendRenderTarget(surface):
    assert isinstance(
        surface.getBackendRenderTarget(
            skia.Surface.kFlushRead_BackendHandleAccess),
        skia.GrBackendRenderTarget)


def test_Surface_replaceBackendTexture(surface):
    assert isinstance(
        surface.replaceBackendTexture(
            skia.GrBackendTexture(), skia.kTopLeft_GrSurfaceOrigin),
        bool)


def test_Surface_getCanvas(surface):
    assert isinstance(surface.getCanvas(), skia.Canvas)


def test_Surface_generationID(surface):
    assert isinstance(surface.generationID(), int)


@pytest.mark.parametrize('args', [
    (skia.ImageInfo.MakeN32Premul(120, 120),),
    (120, 120),
])
def test_Surface_makeSurface(surface, args):
    check_surface(surface.makeSurface(*args))


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.IRect(100, 100),),
])
def test_Surface_makeImageSnapshot(surface, args):
    assert isinstance(surface.makeImageSnapshot(*args), skia.Image)


def test_Surface_draw(surface):
    surface.draw(surface.getCanvas(), 0, 0, None)


def test_Surface_peekPixels(surface):
    assert isinstance(surface.peekPixels(skia.Pixmap()), bool)


@pytest.mark.parametrize('args', [
    (skia.Pixmap(), 0, 0),
    (np.zeros((240, 320, 4), dtype=np.uint8), 0, 0),
    (np.zeros((240, 320, 4), dtype=np.uint8),),
    (skia.Bitmap(), 0, 0),
])
def test_Surface_readPixels(surface, args):
    assert isinstance(surface.readPixels(*args), bool)


@pytest.mark.parametrize('args', [
    (skia.Pixmap(), 0, 0),
    (skia.Bitmap(), 0, 0),
])
def test_Surface_writePixels(surface, args):
    surface.writePixels(*args)


def test_Surface_props(surface):
    assert isinstance(surface.props(), skia.SurfaceProps)


def test_Surface_flushAndSubmit(surface):
    surface.flushAndSubmit()


def test_Surface_flush(surface):
    surface.flush(skia.Surface.kNoAccess, skia.GrFlushInfo())


def test_Surface_characterize(surface):
    characterization = skia.SurfaceCharacterization()
    assert isinstance(surface.characterize(characterization), bool)


# def test_Surface_draw(surface):
#     surface.draw()


def test_Surface_unique(surface):
    assert surface.unique()


def test_Surface_ref_unref(surface):
    surface.ref()
    surface.unref()


@pytest.mark.parametrize('args', [
    (skia.ImageInfo.MakeN32Premul(16, 16), bytearray(16 * 16 * 4)),
    (skia.ImageInfo.MakeN32Premul(16, 16), bytearray(16 * 16 * 4), 16 * 4),
    (
        skia.ImageInfo.MakeN32Premul(16, 16), bytearray(16 * 16 * 4),
        16 * 4,
        skia.SurfaceProps(skia.SurfaceProps.kLegacyFontHost_InitType),),
])
def test_Surface_MakeRasterDirect(args):
    check_surface(skia.Surface.MakeRasterDirect(*args))


@pytest.mark.parametrize('args', [
    (skia.ImageInfo.MakeN32Premul(16, 16),),
    (skia.ImageInfo.MakeN32Premul(16, 16), 16 * 4),
    (
        skia.ImageInfo.MakeN32Premul(16, 16),
        16 * 4,
        skia.SurfaceProps(skia.SurfaceProps.kLegacyFontHost_InitType),),
])
def test_Surface_MakeRaster(args):
    check_surface(skia.Surface.MakeRaster(*args))


@pytest.mark.parametrize('args', [
    (320, 240),
    (320, 240, skia.SurfaceProps(skia.SurfaceProps.kLegacyFontHost_InitType)),
])
def test_Surface_MakeRasterN32Premul(args):
    check_surface(skia.Surface.MakeRasterN32Premul(*args))


def test_Surface_MakeFromBackendTexture(context):
    texture = skia.GrBackendTexture()
    assert isinstance(
        skia.Surface.MakeFromBackendTexture(
            context, texture, skia.kTopLeft_GrSurfaceOrigin, 0,
            skia.kRGBA_8888_ColorType, skia.ColorSpace.MakeSRGB(), None),
        (type(None), skia.Surface))


def test_Surface_MakeFromBackendTexture_2(context):
    characterization = skia.SurfaceCharacterization()
    texture = skia.GrBackendTexture()
    assert isinstance(
        skia.Surface.MakeFromBackendTexture(context, characterization, texture),
        (type(None), skia.Surface))


def test_Surface_MakeFromBackendRenderTarget(context):
    target = skia.GrBackendRenderTarget()
    assert isinstance(
        skia.Surface.MakeFromBackendRenderTarget(
            context, target, skia.kTopLeft_GrSurfaceOrigin,
            skia.kRGBA_8888_ColorType, skia.ColorSpace.MakeSRGB()),
        (type(None), skia.Surface))


@pytest.mark.parametrize('args', [
    tuple(),
    (
        0,
        skia.GrSurfaceOrigin.kBottomLeft_GrSurfaceOrigin,
        None,
        False,
    ),
    (
        0,
        None,
    ),
])
def test_Surface_MakeRenderTarget(args, context):
    info = skia.ImageInfo.MakeN32Premul(320, 240)
    check_surface(skia.Surface.MakeRenderTarget(
        context, skia.Budgeted.kNo, info, *args))


@pytest.mark.skip(reason='GrRecordingContext not implemented')
def test_Surface_MakeRenderTarget_2(context):
    characterization = skia.SurfaceCharacterization()
    check_surface(skia.Surface.MakeRenderTarget(
        context, characterization, skia.Budgeted.kNo))


def test_Surface_MakeNull():
    check_surface(skia.Surface.MakeNull(100, 100))
