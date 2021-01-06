import os
import pytest
import skia


@pytest.fixture
def svgdom(resource_path):
    stream = skia.FILEStream(os.path.join(resource_path, 'Cowboy.svg'))
    return skia.SVGDOM.MakeFromStream(stream)


def test_SVGDOM_MakeFromStream(resource_path):
    stream = skia.FILEStream(os.path.join(resource_path, 'Cowboy.svg'))
    assert isinstance(skia.SVGDOM.MakeFromStream(stream), skia.SVGDOM)


def test_SVGDOM_containerSize(svgdom):
    assert isinstance(svgdom.containerSize(), skia.Size)


def test_SVGDOM_setContainerSize(svgdom):
    svgdom.setContainerSize((100, 100))


def test_SVGDOM_render(svgdom, canvas):
    svgdom.render(canvas)
