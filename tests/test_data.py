import skia
import pytest


@pytest.fixture
def data(png_data):
    return skia.Data(png_data)


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
    assert isinstance(data.data(), memoryview)


def test_Data_bytes(data):
    assert isinstance(data.bytes(), bytes)


def test_Data_writable_data(data):
    assert isinstance(data.writable_data(), memoryview)


def test_Data_copyRange(data):
    buf = bytearray(16)
    assert isinstance(data.copyRange(0, 16, buf), int)


def test_Data_equals(data):
    assert isinstance(data.equals(data), bool)


def test_Data_eq(data):
    assert isinstance(data == data, bool)
