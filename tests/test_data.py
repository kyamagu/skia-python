import skia
import pytest


@pytest.fixture
def data(png_data):
    return skia.Data(png_data, True)


@pytest.mark.parametrize('args', [
    (b'\x00\x00',),
    (b'\x00\x00', True),
])
def test_Data_init(args):
    assert isinstance(skia.Data(*args), skia.Data)


def test_Data_buffer(data):
    import numpy as np
    assert isinstance(np.array(data), np.ndarray)
    assert isinstance(bytes(data), bytes)
    assert isinstance(memoryview(data), memoryview)


def test_Data_size(data):
    assert isinstance(data.size(), int)


def test_Data_isEmpty(data):
    assert not data.isEmpty()


def test_Data_data(data):
    view = data.data()
    assert isinstance(view, memoryview)
    assert view.readonly
    with pytest.raises(TypeError):
        view.cast('B')[0] = 0  # cast needed due to pybind11 issue #2223
    assert bytes(view) == bytes(data)


def test_Data_bytes(data):
    assert isinstance(data.bytes(), bytes)


def test_Data_writable_data(data):
    copy = skia.Data(data, True)
    view = copy.writable_data()
    assert isinstance(view, memoryview)
    assert not view.readonly
    view.cast('B')[0] = 0  # cast needed due to pybind11 issue #2223
    assert not data.equals(copy)


def test_Data_copyRange(data):
    buf = bytearray(16)
    assert isinstance(data.copyRange(0, 16, buf), int)


def test_Data_equals(data):
    assert isinstance(data.equals(data), bool)


def test_Data_eq(data):
    assert isinstance(data == data, bool)


def test_Data_MakeWithCopy(data):
    assert isinstance(skia.Data.MakeWithCopy(data), skia.Data)


def test_Data_MakeUninitialized():
    assert isinstance(skia.Data.MakeUninitialized(128), skia.Data)


def test_Data_MakeWithoutCopy(data):
    assert isinstance(skia.Data.MakeWithoutCopy(data), skia.Data)


def test_Data_MakeFromFileName(png_data):
    import os
    root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    image_path = os.path.join(
        root_dir, 'skia', 'resources', 'images', 'color_wheel.png')
    assert isinstance(skia.Data.MakeFromFileName(image_path), skia.Data)


def test_Data_MakeSubset(data):
    assert isinstance(skia.Data.MakeSubset(data, 0, 100), skia.Data)


def test_Data_MakeEmpty(data):
    assert isinstance(skia.Data.MakeEmpty(), skia.Data)
