import skia
import pytest


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.ImageInfo.MakeN32Premul(100, 100), None, 400),
])
def test_Pixmap_init(args):
    assert isinstance(skia.Pixmap(*args), skia.Pixmap)
