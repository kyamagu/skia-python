import skia
import pytest

def test_SamplingOptions_init():
    assert isinstance(skia.SamplingOptions(), skia.SamplingOptions)
