import skia
import pytest
import os


@pytest.fixture
def file_stream(png_path):
    stream = skia.FILEStream(png_path)
    yield stream
    stream.close()


@pytest.fixture
def memory_stream():
    buf = b'\x00\x01\x02\x03'
    yield skia.MemoryStream.MakeCopy(buf)


@pytest.fixture(params=[
    'file_stream', 'memory_stream',
])
def stream(request):
    yield request.getfixturevalue(request.param)


@pytest.fixture
def dynamic_memory_wstream():
    return skia.DynamicMemoryWStream()


@pytest.fixture
def null_wstream():
    return skia.NullWStream()


@pytest.fixture
def file_wstream(tmp_path):
    yield skia.FILEWStream(tmp_path.name)
    try:
        os.remove(tmp_path.name)
    except PermissionError:
        pass


@pytest.fixture(params=[
    'null_wstream', 'file_wstream', 'dynamic_memory_wstream',
])
def wstream(request):
    yield request.getfixturevalue(request.param)


def test_Stream_MakeFromFile(png_path):
    assert isinstance(skia.Stream.MakeFromFile(png_path), skia.StreamAsset)


def test_Stream_read(stream):
    buf = bytearray(256)
    assert isinstance(stream.read(buf), int)


def test_Stream_skip(stream):
    assert isinstance(stream.skip(4), int)


def test_Stream_peek(stream):
    buf = bytearray(256)
    assert isinstance(stream.peek(buf), int)


def test_Stream_isAtEnd(stream):
    assert isinstance(stream.isAtEnd(), bool)


def test_Stream_readS8(stream):
    assert isinstance(stream.readS8(), int)


def test_Stream_readS16(stream):
    assert isinstance(stream.readS16(), int)


def test_Stream_readS32(stream):
    assert isinstance(stream.readS32(), int)


def test_Stream_readU8(stream):
    assert isinstance(stream.readU8(), int)


def test_Stream_readU16(stream):
    assert isinstance(stream.readU16(), int)


def test_Stream_readU32(stream):
    assert isinstance(stream.readU32(), int)


def test_Stream_readBool(stream):
    assert isinstance(stream.readBool(), bool)


def test_Stream_readScalar(stream):
    assert isinstance(stream.readScalar(), float)


def test_Stream_readPackedUInt(stream):
    assert isinstance(stream.readPackedUInt(), int)


def test_Stream_rewind(stream):
    assert isinstance(stream.rewind(), bool)


def test_Stream_duplicate(stream):
    assert isinstance(stream.duplicate(), skia.Stream)


def test_Stream_fork(stream):
    assert isinstance(stream.fork(), skia.Stream)


def test_Stream_hasPosition(stream):
    assert isinstance(stream.hasPosition(), bool)


def test_Stream_getPosition(stream):
    assert isinstance(stream.getPosition(), int)


def test_Stream_seek(stream):
    assert isinstance(stream.seek(0), bool)


def test_Stream_move(stream):
    assert isinstance(stream.move(0), bool)


def test_Stream_hasLength(stream):
    assert isinstance(stream.hasLength(), bool)


def test_Stream_getLength(stream):
    assert isinstance(stream.getLength(), int)


def test_Stream_getMemoryBase(stream):
    stream.getMemoryBase()


def test_WStream_write(wstream):
    wstream.write(b'\x00\x01\x02\x03')


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


# def test_WStream_writeStream(wstream, stream):
#     assert isinstance(wstream.writeStream(stream), bool)


def test_WStream_SizeOfPackedUInt():
    assert isinstance(skia.WStream.SizeOfPackedUInt(0x10), int)


def test_NullWStream_init(null_wstream):
    assert isinstance(null_wstream, skia.NullWStream)


def test_FILEStream_init(png_path):
    assert isinstance(skia.FILEStream(png_path), skia.FILEStream)


def test_FILEStream_Make(png_path):
    assert isinstance(skia.FILEStream.Make(png_path), skia.FILEStream)


def test_FILEStream_isValid(file_stream):
    assert file_stream.isValid()


def test_FILEStream_context(png_path):
    with skia.FILEStream(png_path) as f:
        assert isinstance(f, skia.FILEStream)


@pytest.mark.parametrize('args', [
    tuple(),
    (128,),
    (b'foo bar', True),
    (skia.Data.MakeUninitialized(128),),
])
def test_MemoryStream_init(args):
    assert isinstance(skia.MemoryStream(*args), skia.MemoryStream)


def test_MemoryStream_MakeCopy():
    assert isinstance(skia.MemoryStream.MakeCopy(b'\x00'), skia.MemoryStream)


def test_MemoryStream_MakeDirect():
    buf = b'\x00\x00'
    assert isinstance(skia.MemoryStream.MakeDirect(buf), skia.MemoryStream)


def test_MemoryStream_Make():
    buf = skia.Data(b'\x00\x00')
    assert isinstance(skia.MemoryStream.MakeDirect(buf), skia.MemoryStream)


def test_MemoryStream_setMemory(memory_stream):
    memory_stream.setMemory(b'\x00\x00')


def test_MemoryStream_asData(memory_stream):
    assert isinstance(memory_stream.asData(), skia.Data)


def test_MemoryStream_setData(memory_stream):
    data = skia.Data.MakeUninitialized(10)
    memory_stream.setData(data)


@pytest.mark.skip(reason='m125:REVISIT; Apparently replaced by SkRBuffer::skipToAlign4')
def test_MemoryStream_skipToAlign4(memory_stream):
    memory_stream.skipToAlign4()


def test_MemoryStream_getAtPos(memory_stream):
    memory_stream.getAtPos()


def test_FILEWStream_init(file_wstream):
    assert isinstance(file_wstream, skia.FILEWStream)


def test_FILEWStream_isValid(file_wstream):
    assert file_wstream.isValid()


def test_FILEWStream_fsync(file_wstream):
    file_wstream.fsync()


def test_DynamicMemoryWStream_init(dynamic_memory_wstream):
    assert isinstance(dynamic_memory_wstream, skia.DynamicMemoryWStream)


def test_DynamicMemoryWStream_read(dynamic_memory_wstream):
    buf = bytearray(dynamic_memory_wstream.bytesWritten())
    assert isinstance(dynamic_memory_wstream.read(buf), bool)


def test_DynamicMemoryWStream_copyTo(dynamic_memory_wstream):
    buf = bytearray(dynamic_memory_wstream.bytesWritten())
    dynamic_memory_wstream.copyTo(buf)


def test_DynamicMemoryWStream_writeToStream(dynamic_memory_wstream):
    wstream = skia.NullWStream()
    assert isinstance(dynamic_memory_wstream.writeToStream(wstream), bool)


def test_DynamicMemoryWStream_copyToAndReset(dynamic_memory_wstream):
    buf = bytearray(dynamic_memory_wstream.bytesWritten())
    dynamic_memory_wstream.copyToAndReset(buf)


@pytest.mark.parametrize('WStream', ['NullWStream', 'DynamicMemoryWStream'])
def test_DynamicMemoryWStream_writeToAndReset(dynamic_memory_wstream, WStream):
    wstream = getattr(skia, WStream)()
    assert isinstance(dynamic_memory_wstream.writeToAndReset(wstream), bool)


def test_DynamicMemoryWStream_prependToAndReset(dynamic_memory_wstream):
    dynamic_memory_wstream.prependToAndReset(skia.DynamicMemoryWStream())


def test_DynamicMemoryWStream_detachAsData(dynamic_memory_wstream):
    assert isinstance(dynamic_memory_wstream.detachAsData(), skia.Data)


def test_DynamicMemoryWStream_reset(dynamic_memory_wstream):
    dynamic_memory_wstream.reset()


def test_DynamicMemoryWStream_padToAlign4(dynamic_memory_wstream):
    dynamic_memory_wstream.padToAlign4()
