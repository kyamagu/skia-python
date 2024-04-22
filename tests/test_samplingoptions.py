import skia
import pytest

@pytest.mark.parametrize('args', [
    (),
    # FilterQuality enum Equivalents
    (skia.FilterMode.kLinear,  skia.MipmapMode.kNearest),  # FilterQuality.kMedium_SkFilterQuality CPU
    (skia.FilterMode.kLinear,  skia.MipmapMode.kLinear),   # FilterQuality.kMedium_SkFilterQuality GPU
    (skia.FilterMode.kLinear,  skia.MipmapMode.kNone),     # FilterQuality.kLow_SkFilterQuality
    (skia.FilterMode.kNearest, skia.MipmapMode.kNone)      # FilterQuality.kNone_SkFilterQuality
])
def test_SamplingOptions_init(args):
    assert isinstance(skia.SamplingOptions(*args), skia.SamplingOptions)

def test_SamplingOptions_init_cubic1():
    # FilterQuality.kHigh_SkFilterQuality
    assert isinstance(skia.SamplingOptions(skia.CubicResampler.Mitchell()), skia.SamplingOptions)


def test_SamplingOptions_init_cubic2():
    # No FilterQuality Equivalent
    assert isinstance(skia.SamplingOptions(skia.CubicResampler.CatmullRom()), skia.SamplingOptions)
