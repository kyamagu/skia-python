import skia
import pytest
import numpy as np


def check_surface(x):
    assert isinstance(x, skia.Surface)


@pytest.mark.parametrize('args', [
    (np.zeros((240, 320, 4), dtype=np.uint8),),
])
def test_Surface_init(args):
    check_surface(skia.Surface(*args))


@pytest.mark.parametrize('args', [
    (320, 240),
    (320, 240, None),
    (320, 240, skia.SurfaceProps(skia.SurfaceProps.InitType.kLegacyFontHost)),
])
def test_Surface_MakeRasterN32Premul(args):
    check_surface(skia.Surface.MakeRasterN32Premul(*args))


def test_Surface_width(surface):
    assert surface.width() == 320


def test_Surface_height(surface):
    assert surface.height() == 240


def test_Surface_getCanvas(surface):
    assert isinstance(surface.imageInfo(), skia.ImageInfo)


def test_Surface_generationID(surface):
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
    assert surface.peekPixels(skia.Pixmap())


@pytest.mark.parametrize('args', [
    (skia.Pixmap(), 0, 0),
    # (skia.ImageInfo(), None, 0, 0, 0),
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


def test_Surface_flush(surface):
    surface.flush()


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
