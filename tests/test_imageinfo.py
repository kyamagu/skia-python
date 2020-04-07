import skia
import pytest


def test_ColorInfo_init():
    def _check(x):
        assert isinstance(x, skia.ColorInfo)
    _check(skia.ColorInfo())


@pytest.fixture(scope='session')
def imageinfo():
    return skia.ImageInfo.MakeN32Premul(320, 240)


def check(x):
    assert isinstance(x, skia.ImageInfo)


@pytest.mark.parametrize('args', [
    (100, 100, skia.ColorType.kRGBA_8888, skia.AlphaType.kPremul),
    (100, 100, skia.ColorType.kRGBA_8888, skia.AlphaType.kPremul,
        skia.ColorSpace()),
    (100, 100, skia.ColorType.kRGBA_8888, skia.AlphaType.kPremul,
        skia.ColorSpace.MakeSRGB()),
    (skia.ISize(100, 100), skia.ColorType.kRGBA_8888, skia.AlphaType.kPremul),
    (skia.ISize(100, 100), skia.ColorType.kRGBA_8888, skia.AlphaType.kPremul,
        skia.ColorSpace.MakeSRGB()),
    (skia.ISize(100, 100), skia.ColorInfo()),
])
def test_ImageInfo_Make(args):
    check(skia.ImageInfo.Make(*args))


@pytest.mark.parametrize('args', [
    (100, 100, skia.AlphaType.kPremul),
    # (100, 100, skia.AlphaType.kPremul, skia.ColorSpace.MakeSRGB()),
])
def test_ImageInfo_MakeN32(args):
    check(skia.ImageInfo.MakeN32(*args))


def test_ImageInfo_MakeS32():
    check(skia.ImageInfo.MakeS32(100, 100, skia.AlphaType.kPremul))


@pytest.mark.parametrize('args', [
    (100, 100),
    (100, 100, skia.ColorSpace()),
    (100, 100, skia.ColorSpace.MakeSRGB()),
    (skia.ISize(100, 100),),
    (skia.ISize(100, 100), skia.ColorSpace.MakeSRGB()),
])
def test_ImageInfo_MakeN32Premul(args):
    check(skia.ImageInfo.MakeN32Premul(*args))


@pytest.mark.parametrize('args', [
    (100, 100),
    (skia.ISize(100, 100),),
])
def test_ImageInfo_MakeA8(args):
    check(skia.ImageInfo.MakeA8(*args))


@pytest.mark.parametrize('args', [
    tuple(),
    (100, 100),
])
def test_ImageInfo_MakeUnknown(args):
    check(skia.ImageInfo.MakeUnknown(*args))
