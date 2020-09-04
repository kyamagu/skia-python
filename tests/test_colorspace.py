import skia
import pytest


@pytest.fixture
def colorspace():
    return skia.ColorSpace.MakeSRGB()


def test_ColorSpace_toProfile(colorspace):
    assert isinstance(colorspace.toProfile(), skia.cms.ICCProfile)


def test_ColorSpace_gammaCloseToSRGB(colorspace):
    assert isinstance(colorspace.gammaCloseToSRGB(), bool)


def test_ColorSpace_gammaIsLinear(colorspace):
    assert isinstance(colorspace.gammaIsLinear(), bool)


def test_ColorSpace_isNumericalTransferFn(colorspace):
    transfer_fn = skia.cms.TransferFunction([1, 0, 0, 0, 0, 0, 0.5])
    assert isinstance(colorspace.isNumericalTransferFn(transfer_fn), bool)


def test_ColorSpace_toXYZD50(colorspace):
    m = skia.cms.Matrix3x3([1, 0, 0, 0, 1, 0, 0, 0, 1])
    assert isinstance(colorspace.toXYZD50(m), bool)


def test_ColorSpace_toXYZD50Hash(colorspace):
    assert isinstance(colorspace.toXYZD50Hash(), int)


def test_ColorSpace_makeLinearGamma(colorspace):
    assert isinstance(colorspace.makeLinearGamma(), skia.ColorSpace)


def test_ColorSpace_makeSRGBGamma(colorspace):
    assert isinstance(colorspace.makeSRGBGamma(), skia.ColorSpace)


def test_ColorSpace_makeColorSpin(colorspace):
    assert isinstance(colorspace.makeColorSpin(), skia.ColorSpace)


def test_ColorSpace_isSRGB(colorspace):
    assert isinstance(colorspace.isSRGB(), bool)
