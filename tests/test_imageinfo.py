import skia
import pytest
import sys


def check_colorinfo(x):
    assert isinstance(x, skia.ColorInfo)


def test_ColorInfo_init():
    check_colorinfo(skia.ColorInfo())


@pytest.fixture(scope='session')
def imageinfo():
    return skia.ImageInfo.MakeN32Premul(320, 240)


def check_imageinfo(x):
    assert isinstance(x, skia.ImageInfo)


@pytest.mark.parametrize('args', [
    (100, 100, skia.ColorType.kRGBA_8888, skia.AlphaType.kPremul),
    (skia.ISize(100, 100), skia.ColorType.kRGBA_8888, skia.AlphaType.kPremul),
    (skia.ISize(100, 100), skia.ColorInfo()),
])
def test_ImageInfo_Make(args):
    check_imageinfo(skia.ImageInfo.Make(*args))


@pytest.mark.parametrize('args', [
    (100, 100, skia.AlphaType.kPremul),
])
def test_ImageInfo_MakeN32(args):
    check_imageinfo(skia.ImageInfo.MakeN32(*args))


def test_ImageInfo_MakeS32():
    check_imageinfo(skia.ImageInfo.MakeS32(100, 100, skia.AlphaType.kPremul))


@pytest.mark.parametrize('args', [
    (100, 100),
    (skia.ISize(100, 100),),
])
def test_ImageInfo_MakeN32Premul(args):
    check_imageinfo(skia.ImageInfo.MakeN32Premul(*args))


@pytest.mark.parametrize('args', [
    (100, 100),
    (skia.ISize(100, 100),),
])
def test_ImageInfo_MakeA8(args):
    check_imageinfo(skia.ImageInfo.MakeA8(*args))


@pytest.mark.parametrize('args', [
    tuple(),
    (100, 100),
])
def test_ImageInfo_MakeUnknown(args):
    check_imageinfo(skia.ImageInfo.MakeUnknown(*args))
