import skia
import pytest


def test_ColorInfo_init():
    def check(x):
        assert isinstance(x, skia.ColorInfo)
    check(skia.ColorInfo())


@pytest.fixture(scope='session')
def imageinfo():
    return skia.ImageInfo.MakeN32Premul(320, 240)


def test_ImageInfo_init():
    def check(x):
        assert isinstance(x, skia.ImageInfo)

    check(skia.ImageInfo.Make(100, 100, skia.ColorType.kRGBA_8888,
        skia.AlphaType.kPremul))
    check(skia.ImageInfo.Make(100, 100, skia.ColorType.kRGBA_8888,
        skia.AlphaType.kPremul, skia.ColorSpace()))
    check(skia.ImageInfo.Make(skia.ISize(100, 100), skia.ColorType.kRGBA_8888,
        skia.AlphaType.kPremul))
    check(skia.ImageInfo.Make(skia.ISize(100, 100), skia.ColorType.kRGBA_8888,
        skia.AlphaType.kPremul, skia.ColorSpace.MakeSRGB()))
    check(skia.ImageInfo.Make(skia.ISize(100, 100), skia.ColorInfo()))

    check(skia.ImageInfo.MakeN32(100, 100, skia.AlphaType.kPremul,
        skia.ColorSpace.MakeSRGB()))
    check(skia.ImageInfo.MakeN32(100, 100, skia.AlphaType.kPremul))

    check(skia.ImageInfo.MakeS32(100, 100, skia.AlphaType.kPremul))

    check(skia.ImageInfo.MakeN32Premul(320, 240))
    check(skia.ImageInfo.MakeN32Premul(320, 240, skia.ColorSpace()))
    check(skia.ImageInfo.MakeN32Premul(320, 240, skia.ColorSpace.MakeSRGB()))
    check(skia.ImageInfo.MakeN32Premul(skia.ISize(320, 240)))
    check(skia.ImageInfo.MakeN32Premul(skia.ISize(320, 240),
        skia.ColorSpace.MakeSRGB()))

    check(skia.ImageInfo.MakeA8(100, 100))
    check(skia.ImageInfo.MakeA8(skia.ISize(100, 100)))

    check(skia.ImageInfo.MakeUnknown())
