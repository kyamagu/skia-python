import skia
import pytest


# def test_Stream_read(stream):
#     stream.read()


@pytest.fixture
def wstream():
    return skia.DynamicMemoryWStream()


def test_WStream_flush(wstream):
    wstream.flush()


def test_WStream_bytesWritten(wstream):
    assert isinstance(wstream.bytesWritten(), int)


def test_WStream_write8(wstream):
    assert isinstance(wstream.write8(0), bool)


def test_WStream_write16(wstream):
    assert isinstance(wstream.write16(0), bool)


def test_WStream_write32(wstream):
    assert isinstance(wstream.write32(0), bool)


def test_WStream_writeText(wstream):
    assert isinstance(wstream.writeText('foo'), bool)


def test_WStream_newline(wstream):
    wstream.newline()


def test_WStream_writeDecAsText(wstream):
    assert isinstance(wstream.writeDecAsText(0), bool)


def test_WStream_writeBigDecAsText(wstream):
    assert isinstance(wstream.writeBigDecAsText(0), bool)


def test_WStream_writeHexAsText(wstream):
    assert isinstance(wstream.writeHexAsText(0), bool)


def test_WStream_writeScalarAsText(wstream):
    assert isinstance(wstream.writeScalarAsText(0.0), bool)


def test_WStream_writeBool(wstream):
    assert isinstance(wstream.writeBool(True), bool)


def test_WStream_writeScalar(wstream):
    assert isinstance(wstream.writeScalar(1.0), bool)


def test_WStream_writePackedUInt(wstream):
    assert isinstance(wstream.writePackedUInt(0x00), bool)


# def test_WStream_writeStream(wstream):
#     assert isinstance(wstream.writePackedUInt(), bool)


def test_WStream_SizeOfPackedUInt():
    assert isinstance(skia.WStream.SizeOfPackedUInt(0x10), int)
