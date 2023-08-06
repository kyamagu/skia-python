#include "common.h"

template <class T = SkStream> class PyStream : public T {
public:
    using T::T;
    size_t read(void* buffer, size_t size) override {
        PYBIND11_OVERLOAD_PURE(size_t, T, read, buffer, size);
    }
    size_t peek(void* buffer, size_t size) const override {
        PYBIND11_OVERLOAD(size_t, T, peek, buffer, size);
    }
    bool isAtEnd() const override {
        PYBIND11_OVERLOAD_PURE(bool, T, isAtEnd);
    }
    bool rewind() override { PYBIND11_OVERLOAD(bool, T, rewind); }
    bool hasPosition() const override {
        PYBIND11_OVERLOAD(bool, T, hasPosition);
    }
    size_t getPosition() const override {
        PYBIND11_OVERLOAD(size_t, T, getPosition);
    }
    bool seek(size_t position) override {
        PYBIND11_OVERLOAD(bool, T, seek, position);
    }
    bool move(long offset) override {
        PYBIND11_OVERLOAD(bool, T, move, offset);
    }
    bool hasLength() const override {
        PYBIND11_OVERLOAD(bool, T, hasLength);
    }
    size_t getLength() const override {
        PYBIND11_OVERLOAD(size_t, T, getLength);
    }
    const void* getMemoryBase() override {
        PYBIND11_OVERLOAD(const void*, T, getMemoryBase);
    }
};

template <class T = SkStreamRewindable> class PyStreamRewindable : public T {
public:
    using T::T;
    size_t read(void* buffer, size_t size) override {
        PYBIND11_OVERLOAD(size_t, T, read, buffer, size);
    }
    size_t peek(void* buffer, size_t size) const override {
        PYBIND11_OVERLOAD(size_t, T, peek, buffer, size);
    }
    bool isAtEnd() const override {
        PYBIND11_OVERLOAD(bool, T, isAtEnd);
    }
    bool rewind() override { PYBIND11_OVERLOAD_PURE(bool, T, rewind); }
    bool hasPosition() const override {
        PYBIND11_OVERLOAD(bool, T, hasPosition);
    }
    size_t getPosition() const override {
        PYBIND11_OVERLOAD(size_t, T, getPosition);
    }
    bool seek(size_t position) override {
        PYBIND11_OVERLOAD(bool, T, seek, position);
    }
    bool move(long offset) override {
        PYBIND11_OVERLOAD(bool, T, move, offset);
    }
    bool hasLength() const override {
        PYBIND11_OVERLOAD(bool, T, hasLength);
    }
    size_t getLength() const override {
        PYBIND11_OVERLOAD(size_t, T, getLength);
    }
    const void* getMemoryBase() override {
        PYBIND11_OVERLOAD(const void*, T, getMemoryBase);
    }
};

template <class T = SkStreamSeekable> class PyStreamSeekable : public T {
public:
    using T::T;
    size_t read(void* buffer, size_t size) override {
        PYBIND11_OVERLOAD(size_t, T, read, buffer, size);
    }
    size_t peek(void* buffer, size_t size) const override {
        PYBIND11_OVERLOAD(size_t, T, peek, buffer, size);
    }
    bool isAtEnd() const override {
        PYBIND11_OVERLOAD(bool, T, isAtEnd);
    }
    bool rewind() override { PYBIND11_OVERLOAD(bool, T, rewind); }
    bool hasPosition() const override {
        PYBIND11_OVERLOAD_PURE(bool, T, hasPosition);
    }
    size_t getPosition() const override {
        PYBIND11_OVERLOAD_PURE(size_t, T, getPosition);
    }
    bool seek(size_t position) override {
        PYBIND11_OVERLOAD_PURE(bool, T, seek, position);
    }
    bool move(long offset) override {
        PYBIND11_OVERLOAD_PURE(bool, T, move, offset);
    }
    bool hasLength() const override {
        PYBIND11_OVERLOAD(bool, T, hasLength);
    }
    size_t getLength() const override {
        PYBIND11_OVERLOAD(size_t, T, getLength);
    }
    const void* getMemoryBase() override {
        PYBIND11_OVERLOAD(const void*, T, getMemoryBase);
    }
};

template <class T = SkStreamAsset> class PyStreamAsset : public T {
public:
    using T::T;
    size_t read(void* buffer, size_t size) override {
        PYBIND11_OVERLOAD(size_t, T, read, buffer, size);
    }
    size_t peek(void* buffer, size_t size) const override {
        PYBIND11_OVERLOAD(size_t, T, peek, buffer, size);
    }
    bool isAtEnd() const override {
        PYBIND11_OVERLOAD(bool, T, isAtEnd);
    }
    bool rewind() override { PYBIND11_OVERLOAD(bool, T, rewind); }
    bool hasPosition() const override {
        PYBIND11_OVERLOAD(bool, T, hasPosition);
    }
    size_t getPosition() const override {
        PYBIND11_OVERLOAD(size_t, T, getPosition);
    }
    bool seek(size_t position) override {
        PYBIND11_OVERLOAD(bool, T, seek, position);
    }
    bool move(long offset) override {
        PYBIND11_OVERLOAD(bool, T, move, offset);
    }
    bool hasLength() const override {
        PYBIND11_OVERLOAD(bool, T, hasLength);
    }
    size_t getLength() const override {
        PYBIND11_OVERLOAD_PURE(size_t, T, getLength);
    }
    const void* getMemoryBase() override {
        PYBIND11_OVERLOAD(const void*, T, getMemoryBase);
    }
};

template <class T = SkStreamMemory> class PyStreamMemory : public T {
public:
    using T::T;
    size_t read(void* buffer, size_t size) override {
        PYBIND11_OVERLOAD(size_t, T, read, buffer, size);
    }
    size_t peek(void* buffer, size_t size) const override {
        PYBIND11_OVERLOAD(size_t, T, peek, buffer, size);
    }
    bool isAtEnd() const override {
        PYBIND11_OVERLOAD(bool, T, isAtEnd);
    }
    bool rewind() override { PYBIND11_OVERLOAD(bool, T, rewind); }
    bool hasPosition() const override {
        PYBIND11_OVERLOAD(bool, T, hasPosition);
    }
    size_t getPosition() const override {
        PYBIND11_OVERLOAD(size_t, T, getPosition);
    }
    bool seek(size_t position) override {
        PYBIND11_OVERLOAD(bool, T, seek, position);
    }
    bool move(long offset) override {
        PYBIND11_OVERLOAD(bool, T, move, offset);
    }
    bool hasLength() const override {
        PYBIND11_OVERLOAD(bool, T, hasLength);
    }
    size_t getLength() const override {
        PYBIND11_OVERLOAD(size_t, T, getLength);
    }
    const void* getMemoryBase() override {
        PYBIND11_OVERLOAD_PURE(const void*, T, getMemoryBase);
    }
};

template <class T = SkWStream> class PyWStream : public T {
public:
    using T::T;
    bool write(const void* buffer, size_t size) override {
        PYBIND11_OVERLOAD_PURE(bool, T, write, buffer, size);
    }
    void flush() override { PYBIND11_OVERLOAD(void, T, flush); };
    size_t bytesWritten() const override {
        PYBIND11_OVERLOAD_PURE(size_t, T, bytesWritten);
    };
};

template <class T = SkNullWStream> class PyWStreamImpl : public T {
public:
    using T::T;
    bool write(const void* buffer, size_t size) override {
        PYBIND11_OVERLOAD(bool, T, write, buffer, size);
    }
    void flush() override { PYBIND11_OVERLOAD(void, T, flush); };
    size_t bytesWritten() const override {
        PYBIND11_OVERLOAD(size_t, T, bytesWritten);
    };
};

template <class T = SkFILEStream> class PyStreamImpl : public T {
public:
    using T::T;
    size_t read(void* buffer, size_t size) override {
        PYBIND11_OVERLOAD(size_t, T, read, buffer, size);
    }
    size_t peek(void* buffer, size_t size) const override {
        PYBIND11_OVERLOAD(size_t, T, peek, buffer, size);
    }
    bool isAtEnd() const override {
        PYBIND11_OVERLOAD(bool, T, isAtEnd);
    }
    bool rewind() override { PYBIND11_OVERLOAD(bool, T, rewind); }
    bool hasPosition() const override {
        PYBIND11_OVERLOAD(bool, T, hasPosition);
    }
    size_t getPosition() const override {
        PYBIND11_OVERLOAD(size_t, T, getPosition);
    }
    bool seek(size_t position) override {
        PYBIND11_OVERLOAD(bool, T, seek, position);
    }
    bool move(long offset) override {
        PYBIND11_OVERLOAD(bool, T, move, offset);
    }
    bool hasLength() const override {
        PYBIND11_OVERLOAD(bool, T, hasLength);
    }
    size_t getLength() const override {
        PYBIND11_OVERLOAD(size_t, T, getLength);
    }
    const void* getMemoryBase() override {
        PYBIND11_OVERLOAD(const void*, T, getMemoryBase);
    }
};

template <class T = SkMemoryStream> class PyMemoryStream : public T {
public:
    using T::T;
    size_t read(void* buffer, size_t size) override {
        PYBIND11_OVERLOAD(size_t, T, read, buffer, size);
    }
    size_t peek(void* buffer, size_t size) const override {
        PYBIND11_OVERLOAD(size_t, T, peek, buffer, size);
    }
    bool isAtEnd() const override {
        PYBIND11_OVERLOAD(bool, T, isAtEnd);
    }
    bool rewind() override { PYBIND11_OVERLOAD(bool, T, rewind); }
    bool hasPosition() const override {
        PYBIND11_OVERLOAD(bool, T, hasPosition);
    }
    size_t getPosition() const override {
        PYBIND11_OVERLOAD(size_t, T, getPosition);
    }
    bool seek(size_t position) override {
        PYBIND11_OVERLOAD(bool, T, seek, position);
    }
    bool move(long offset) override {
        PYBIND11_OVERLOAD(bool, T, move, offset);
    }
    bool hasLength() const override {
        PYBIND11_OVERLOAD(bool, T, hasLength);
    }
    size_t getLength() const override {
        PYBIND11_OVERLOAD(size_t, T, getLength);
    }
    const void* getMemoryBase() override {
        PYBIND11_OVERLOAD(const void*, T, getMemoryBase);
    }
    void setMemory(
        const void* data, size_t length, bool copyData = false) override {
        PYBIND11_OVERLOAD(void, T, setMemory, data, length, copyData);
    }
};


void initStream(py::module &m) {

py::class_<SkStream, PyStream<>>(m, "Stream",
    R"docstring(
    :py:class:`Stream` – abstraction for a source of bytes.

    Subclasses can be backed by memory, or a file, or something else.

    NOTE:

    Classic "streams" APIs are sort of async, in that on a request for N bytes,
    they may return fewer than N bytes on a given call, in which case the caller
    can "try again" to get more bytes, eventually (modulo an error) receiving
    their total N bytes.

    Skia streams behave differently. They are effectively synchronous, and will
    always return all N bytes of the request if possible. If they return fewer
    (the read() call returns the number of bytes read) then that means there is
    no more data (at EOF or hit an error). The caller should not call again in
    hopes of fulfilling more of the request.

    .. rubric:: Subclasses

    .. autosummary::
        :nosignatures:

        ~FILEStream
        ~MemoryStream
    )docstring")
    .def("read",
        [] (SkStream& stream, py::buffer b, size_t size) {
            auto info = b.request(true);
            size_t given = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            if (size == 0 || size >= given)
                size = given;
            return stream.read(info.ptr, size);
        },
        R"docstring(
        Reads or skips size number of bytes.

        If buffer == NULL, skip size bytes, return how many were skipped. If
        buffer != NULL, copy size bytes into buffer, return how many were
        copied.

        :param buffer:  when NULL skip size bytes, otherwise copy size bytes
            into buffer
        :param size:    the number of bytes to skip or copy; may be nullptr
        :return: the number of bytes actually read.

        Implemented in :py:class:`MemoryStream`, and :py:class:`FILEStream`.
        )docstring",
        py::arg("buffer"), py::arg("size") = 0)
    .def("skip", &SkStream::skip,
        R"docstring(
        Skip size number of bytes.

        :return: the actual number bytes that could be skipped.
        )docstring",
        py::arg("size"))
    .def("peek",
        [] (SkStream& stream, py::buffer b) {
            auto info = b.request(true);
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return stream.peek(info.ptr, size);
        },
        R"docstring(
        Attempt to peek at size bytes.

        If this stream supports peeking, copy min(size, peekable bytes) into
        buffer, and return the number of bytes copied. If the stream does not
        support peeking, or cannot peek any bytes, return 0 and leave buffer
        unchanged. The stream is guaranteed to be in the same visible state
        after this call, regardless of success or failure.

        :param buffer:  Must not be NULL, and must be at least size bytes.
            Destination to copy bytes.
        :param size:  Number of bytes to copy.
        :return: The number of bytes peeked/copied.

        Reimplemented in :py:class:`MemoryStream`.
        )docstring",
        py::arg("buffer"))
    .def("isAtEnd", &SkStream::isAtEnd,
        R"docstring(
        Returns true when all the bytes in the stream have been read.

        This may return true early (when there are no more bytes to be read) or
        late (after the first unsuccessful read).

        Implemented in :py:class:`MemoryStream`, and :py:class:`FILEStream`.
        )docstring")
    .def("readS8",
        [] (SkStream& stream) {
            int8_t value;
            if (stream.readS8(&value))
                return value;
            throw std::runtime_error("Failed to read");
        })
    .def("readS16",
        [] (SkStream& stream) {
            int16_t value;
            if (stream.readS16(&value))
                return value;
            throw std::runtime_error("Failed to read");
        })
    .def("readS32",
        [] (SkStream& stream) {
            int32_t value;
            if (stream.readS32(&value))
                return value;
            throw std::runtime_error("Failed to read");
        })
    .def("readU8",
        [] (SkStream& stream) {
            uint8_t value;
            if (stream.readU8(&value))
                return value;
            throw std::runtime_error("Failed to read");
        })
    .def("readU16",
        [] (SkStream& stream) {
            uint16_t value;
            if (stream.readU16(&value))
                return value;
            throw std::runtime_error("Failed to read");
        })
    .def("readU32",
        [] (SkStream& stream) {
            uint32_t value;
            if (stream.readU32(&value))
                return value;
            throw std::runtime_error("Failed to read");
        })
    .def("readBool",
        [] (SkStream& stream) {
            bool value;
            if (stream.readBool(&value))
                return value;
            throw std::runtime_error("Failed to read");
        })
    .def("readScalar",
        [] (SkStream& stream) {
            SkScalar value;
            if (stream.readScalar(&value))
                return value;
            throw std::runtime_error("Failed to read");
        })
    .def("readPackedUInt",
        [] (SkStream& stream) {
            size_t value;
            if (stream.readPackedUInt(&value))
                return value;
            throw std::runtime_error("Failed to read");
        })
    .def("rewind", &SkStream::rewind,
        R"docstring(
        Rewinds to the beginning of the stream.

        Returns true if the stream is known to be at the beginning after this
        call returns.

        Reimplemented in :py:class:`MemoryStream`, :py:class:`FILEStream`, and
        :py:class:`StreamRewindable`.
        )docstring")
    .def("duplicate", &SkStream::duplicate,
        R"docstring(
        Duplicates this stream.

        If this cannot be done, returns NULL. The returned stream will be
        positioned at the beginning of its data.
        )docstring")
    .def("fork", &SkStream::fork,
        R"docstring(
        Duplicates this stream.

        If this cannot be done, returns NULL. The returned stream will be
        positioned the same as this stream.
        )docstring")
    .def("hasPosition", &SkStream::hasPosition,
        R"docstring(
        Returns true if this stream can report it's current position.

        Reimplemented in :py:class:`StreamSeekable`.
        )docstring")
    .def("getPosition", &SkStream::getPosition,
        R"docstring(
        Returns the current position in the stream.

        If this cannot be done, returns 0.

        Reimplemented in :py:class:`MemoryStream`, :py:class:`FILEStream`, and
        :py:class:`StreamSeekable`.
        )docstring")
    .def("seek", &SkStream::seek,
        R"docstring(
        Seeks to an absolute position in the stream.

        If this cannot be done, returns false. If an attempt is made to seek
        past the end of the stream, the position will be set to the end of the
        stream.

        Reimplemented in :py:class:`MemoryStream`, :py:class:`FILEStream`, and
        :py:class:`StreamSeekable`.
        )docstring",
        py::arg("position"))
    .def("move", &SkStream::move,
        R"docstring(
        Seeks to an relative offset in the stream.

        If this cannot be done, returns false. If an attempt is made to move to
        a position outside the stream, the position will be set to the closest
        point within the stream (beginning or end).

        Reimplemented in :py:class:`MemoryStream`, :py:class:`FILEStream`, and
        :py:class:`StreamSeekable`.
        )docstring",
        py::arg("offset"))
    .def("hasLength", &SkStream::hasLength,
        R"docstring(
        Returns true if this stream can report it's total length.

        Reimplemented in :py:class:`StreamAsset`.
        )docstring")
    .def("getLength", &SkStream::getLength,
        R"docstring(
        Returns the total length of the stream.

        If this cannot be done, returns 0.

        Reimplemented in :py:class:`MemoryStream`, :py:class:`FILEStream`, and
        :py:class:`StreamAsset`.
        )docstring")
    .def("getMemoryBase", &SkStream::getMemoryBase,
        R"docstring(
        Returns the starting address for the data.

        If this cannot be done, returns NULL.

        Reimplemented in :py:class:`MemoryStream`, and :py:class:`StreamMemory`.
        )docstring")
    .def_static("MakeFromFile",
        [] (const std::string& path) {
            return SkStream::MakeFromFile(path.c_str());
        },
        R"docstring(
        Attempts to open the specified file as a stream, returns nullptr on
        failure.
        )docstring",
        py::arg("path"))
    ;

py::class_<SkStreamRewindable, PyStreamRewindable<>, SkStream>(
    m, "StreamRewindable")
    .def("duplicate", &SkStreamRewindable::duplicate)
    ;

py::class_<SkStreamSeekable, PyStreamSeekable<>, SkStreamRewindable>(
    m, "StreamSeekable")
    .def("fork", &SkStreamSeekable::fork)
    ;

py::class_<SkStreamAsset, PyStreamAsset<>, SkStreamSeekable>(
    m, "StreamAsset")
    .def("duplicate", &SkStreamSeekable::duplicate)
    .def("fork", &SkStreamSeekable::fork)
    ;

py::class_<SkStreamMemory, PyStreamMemory<>, SkStreamAsset>(
    m, "StreamMemory")
    .def("duplicate", &SkStreamSeekable::duplicate)
    .def("fork", &SkStreamSeekable::fork)
    ;

py::class_<SkWStream, PyWStream<>>(m, "WStream",
    R"docstring(
    .. rubric:: Subclasses

    .. autosummary::
        :nosignatures:

        ~FILEWStream
        ~DynamicMemoryWStream
    )docstring")
    .def("write",
        [] (SkWStream& stream, py::buffer b) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return stream.write(info.ptr, size);
        },
        R"docstring(
        Called to write bytes to a :py:class:`WStream`.

        Returns true on success

        :param buffer:  the address of at least size bytes to be written to the
            stream
        :param size:    The number of bytes in buffer to write to the stream
        :return: true on success

        Implemented in :py:class:`DynamicMemoryWStream`,
            :py:class:`FILEWStream`, and :py:class:`NullWStream`.
        )docstring",
        py::arg("buffer"))
    .def("flush", &SkWStream::flush,
        R"docstring(
        Reimplemented in :py:class:`FILEWStream`, and :py:class:`NullWStream`.
        )docstring")
    .def("bytesWritten", &SkWStream::bytesWritten)
    .def("write8", &SkWStream::write8, py::arg("value"))
    .def("write16", &SkWStream::write16, py::arg("value"))
    .def("write32", &SkWStream::write32, py::arg("value"))
    .def("writeText",
        [] (SkWStream& stream, const std::string& text) {
            return stream.writeText(text.c_str());
        },
        py::arg("text"))
    .def("newline", &SkWStream::newline)
    .def("writeDecAsText", &SkWStream::writeDecAsText, py::arg("value"))
    .def("writeBigDecAsText", &SkWStream::writeBigDecAsText,
        py::arg("value"), py::arg("minDigits") = 0)
    .def("writeHexAsText", &SkWStream::writeHexAsText,
        py::arg("value"), py::arg("minDigits") = 0)
    .def("writeScalarAsText", &SkWStream::writeScalarAsText, py::arg("value"))
    .def("writeBool", &SkWStream::writeBool, py::arg("value"))
    .def("writeBool", &SkWStream::writeBool, py::arg("value"))
    .def("writeScalar", &SkWStream::writeScalar, py::arg("value"))
    .def("writePackedUInt", &SkWStream::writePackedUInt, py::arg("value"))
    .def("writeStream", &SkWStream::writeStream,
        py::arg("input"), py::arg("length"))
    .def_static("SizeOfPackedUInt", &SkWStream::SizeOfPackedUInt,
        py::arg("value"))
    ;

py::class_<SkNullWStream, PyWStreamImpl<SkNullWStream>, SkWStream>(
    m, "NullWStream")
    .def(py::init<>())
    ;

py::class_<SkFILEStream, PyStreamImpl<SkFILEStream>, SkStreamAsset>(
    m, "FILEStream")
    .def("__enter__", [] (const SkFILEStream* stream) { return stream; })
    .def("__exit__",
        [] (SkFILEStream& stream, py::object exc_type, py::object exc_value,
            py::object traceback) { stream.close(); })
    .def(py::init(
        [] (const std::string& path) {
            const char* path_ = path.c_str();
            return SkFILEStream::Make(path_);
        }),
        py::arg("path"))
    .def_static("Make",
        [] (const std::string& path) {
            const char* path_ = path.c_str();
            return SkFILEStream::Make(path_);
        },
        py::arg("path"))
    .def("isValid", &SkFILEStream::isValid)
    .def("close", &SkFILEStream::close)
    ;

py::class_<SkMemoryStream, PyMemoryStream<>, SkStreamMemory>(m, "MemoryStream")
    .def(py::init<>())
    .def(py::init<size_t>(), py::arg("length"))
    .def(py::init(
        [] (py::buffer b, bool copyData) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return std::unique_ptr<SkMemoryStream>(
                new SkMemoryStream(info.ptr, size, copyData));
        }),
        py::arg("data"), py::arg("copyData") = false)
    .def(py::init<const void*, size_t, bool>(), py::arg("data"), py::arg("length"), py::arg("copyData") = false)
    .def(py::init<sk_sp<SkData>>(), py::arg("data"))
    .def_static("MakeCopy",
        [] (py::buffer b) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return SkMemoryStream::MakeCopy(info.ptr, size);
        },
        py::arg("data"))
    .def_static("MakeDirect",
        [] (py::buffer b) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return SkMemoryStream::MakeDirect(info.ptr, size);
        },
        py::arg("data"))
    .def_static("Make", &SkMemoryStream::Make, py::arg("data"))
    .def("setMemory",
        [] (SkMemoryStream& stream, py::buffer b, bool copyData) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            stream.setMemory(info.ptr, size, copyData);
        },
        py::arg("data"), py::arg("copyData") = false)
    .def("asData", &SkMemoryStream::asData)
    .def("setData", &SkMemoryStream::setData, py::arg("data"))
    .def("skipToAlign4", &SkMemoryStream::skipToAlign4)
    .def("getAtPos", &SkMemoryStream::getAtPos)
    ;

py::class_<SkFILEWStream, PyWStreamImpl<SkFILEWStream>, SkWStream>(
    m, "FILEWStream")
    .def(py::init(
        [] (const std::string& path) {
            return std::unique_ptr<SkFILEWStream>(
                new SkFILEWStream(path.c_str()));
        }),
        py::arg("path"))
    .def("isValid", &SkFILEWStream::isValid)
    .def("fsync", &SkFILEWStream::fsync)
    ;

py::class_<SkDynamicMemoryWStream, PyWStreamImpl<SkDynamicMemoryWStream>,
    SkWStream>(m, "DynamicMemoryWStream")
    .def(py::init<>())
    .def("read",
        [] (SkDynamicMemoryWStream& stream, py::buffer b, size_t offset) {
            auto info = b.request(true);
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return stream.read(info.ptr, offset, size);
        },
        py::arg("data"), py::arg("offset") = 0)
    .def("copyTo",
        [] (SkDynamicMemoryWStream& stream, py::buffer b) {
            auto info = b.request(true);
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            if (size < stream.bytesWritten())
                throw py::value_error("Buffer is smaller than required");
            stream.copyTo(info.ptr);
        },
        R"docstring(
        More efficient version of read(dst, 0, :py:meth:`bytesWritten`).
        )docstring",
        py::arg("dst"))
    .def("writeToStream", &SkDynamicMemoryWStream::writeToStream,
        py::arg("dst"))
    .def("copyToAndReset",
        [] (SkDynamicMemoryWStream& stream, py::buffer b) {
            auto info = b.request(true);
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            if (size < stream.bytesWritten())
                throw py::value_error("Buffer is smaller than required");
            stream.copyToAndReset(info.ptr);
        },
        R"docstring(
        Equivalent to :py:meth:`copyTo` followed by :py:meth:`reset`, but may
        save memory use.
        )docstring",
        py::arg("dst"))
    .def("writeToAndReset",
        py::overload_cast<SkWStream*>(&SkDynamicMemoryWStream::writeToAndReset),
        R"docstring(
        Equivalent to :py:meth:`writeToStream` followed by :py:meth:`reset`, but
        may save memory use.
        )docstring",
        py::arg("dst"))
    .def("writeToAndReset",
        py::overload_cast<SkDynamicMemoryWStream*>(
            &SkDynamicMemoryWStream::writeToAndReset),
        R"docstring(
        Equivalent to :py:meth:`writeToStream` followed by :py:meth:`reset`, but
        may save memory use.

        When the dst is also a :py:class:`DynamicMemoryWStream`, the
        implementation is constant time.
        )docstring",
        py::arg("dst"))
    .def("prependToAndReset", &SkDynamicMemoryWStream::prependToAndReset,
        R"docstring(
        Prepend this stream to dst, resetting this.
        )docstring",
        py::arg("dst"))
    .def("detachAsData", &SkDynamicMemoryWStream::detachAsData,
        R"docstring(
        Return the contents as :py:class:`Data`, and then reset the stream.
        )docstring")
    .def("detachAsStream", &SkDynamicMemoryWStream::detachAsStream,
        R"docstring(
        Reset, returning a reader stream with the current content.
        )docstring")
    .def("reset", &SkDynamicMemoryWStream::reset)
    .def("padToAlign4", &SkDynamicMemoryWStream::padToAlign4)
    ;

}
