#include "common.h"

py::buffer_info GetBuffer(SkData &d, bool readonly = true) {
    return py::buffer_info(
        d.writable_data(),
        sizeof(uint8_t),
        py::format_descriptor<uint8_t>::format(),
        1,
        { d.size() },
        { sizeof(uint8_t) },
        readonly
    );
}

template<>
struct py::detail::has_operator_delete<SkData, void> : std::false_type {};

void initData(py::module &m) {
py::class_<SkData, sk_sp<SkData>>(m, "Data", py::buffer_protocol(),
    R"docstring(
    :py:class:`Data` holds an immutable data buffer.

    Not only is the data immutable, but the actual ptr that is returned (by
    :py:meth:`data` or :py:meth:`bytes`) is guaranteed to always be the same for
    the life of this instance.

    :py:class:`Data` supports Python buffer protocol, meaning that
    :py:class:`Data` can be converted to Python buffer types without copy::

        bytes(data)
        memoryview(data)
        np.array(data)
    )docstring")
    .def_buffer([] (SkData& data) { return GetBuffer(data); })
    .def("__repr__",
        [] (const SkData& data) {
            return py::str("Data(size={})").format(data.size());
        })
    .def(py::init([] (py::buffer b, bool copy) {
        py::buffer_info info = b.request();
        auto length = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
        if (copy)
            return SkData::MakeWithCopy(info.ptr, length);
        else
            return SkData::MakeWithoutCopy(info.ptr, length);
    }),
    R"docstring(
    Create a new :py:class:`Data`.

    :param Union[bytes,bytearray,memoryview] buf: Buffer object
    :param bool copy: Whether to copy data, default `False`.
    )docstring",
    py::arg("buf"), py::arg("copy") = false)
    .def("size", &SkData::size,
        R"docstring(
        Returns the number of bytes stored.
        )docstring")
    .def("isEmpty", &SkData::isEmpty)
    .def("data",
        [] (SkData& data) { return py::memoryview(GetBuffer(data)); },
        R"docstring(
        Returns the read-only memoryview to the data.
        )docstring")
    .def("bytes",
        [] (SkData& data) {
            return py::bytes(
                reinterpret_cast<const char*>(data.bytes()),
                data.size());
        },
        R"docstring(
        Like data(), returns a read-only ptr into the data, but in this case it
        is cast to ``bytes``.
        )docstring")
    .def("writable_data",
        [] (SkData& data) { return py::memoryview(GetBuffer(data, false)); },
        R"docstring(
        USE WITH CAUTION.

        Returns the read-write memoryview to the data.

        This call will assert that the refcnt is 1, as a precaution against
        modifying the contents when another client/thread has access to the
        data.
        )docstring")
    .def("copyRange",
        [] (const SkData& data, size_t offset, size_t length, py::object b) {
            if (b.is_none())
                return data.copyRange(offset, length, nullptr);
            auto info = b.cast<py::buffer>().request();
            size_t given = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            if (given < length)
                throw std::runtime_error("Buffer is smaller than required");
            return data.copyRange(offset, length, info.ptr);
        },
        R"docstring(
        Helper to copy a range of the data into a caller-provided buffer.

        Returns the actual number of bytes copied, after clamping offset and
        length to the size of the data. If buffer is NULL, it is ignored, and
        only the computed number of bytes is returned.
        )docstring",
        py::arg("offset"), py::arg("length"), py::arg("buffer"))
    .def("equals", &SkData::equals,
        R"docstring(
        Returns true if these two objects have the same length and contents,
        effectively returning 0 == memcmp(...)
        )docstring",
        py::arg("other"))
    .def("__eq__", &SkData::equals,
        R"docstring(
        Returns true if these two objects have the same length and contents,
        effectively returning 0 == memcmp(...)
        )docstring",
        py::arg("other"), py::is_operator())
    .def("unique", &SkData::unique)
    .def("ref", &SkData::ref)
    .def("unref", &SkData::unref)
    .def("deref", &SkData::deref)
    .def("refCntGreaterThan", &SkData::refCntGreaterThan)
    .def_static("MakeWithCopy",
        [] (py::buffer b) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return SkData::MakeWithCopy(info.ptr, size);
        },
        R"docstring(
        Create a new dataref by copying the specified data.
        )docstring",
        py::arg("data"))
    .def_static("MakeUninitialized", &SkData::MakeUninitialized,
        R"docstring(
        Create a new data with uninitialized contents.

        The caller should call :py:meth:`writable_data` to write into the
        buffer, but this must be done before another :py:meth:`ref` is made.
        )docstring",
        py::arg("length"))
    .def_static("MakeWithoutCopy",
        [] (py::buffer b) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return SkData::MakeWithoutCopy(info.ptr, size);
        },
        R"docstring(
        Call this when the data parameter is already const and will outlive the
        lifetime of the :py:class:`Data`.

        Suitable for with const globals.
        )docstring",
        py::arg("data"))
    .def_static("MakeFromFileName",
        [] (const std::string& path) {
            return SkData::MakeFromFileName(path.c_str());
        },
        R"docstring(
        Create a new dataref the file with the specified path.

        If the file cannot be opened, this returns NULL.
        )docstring",
        py::arg("path"))
    .def_static("MakeSubset", &SkData::MakeSubset,
        R"docstring(
        Create a new dataref using a subset of the data in the specified src
        dataref.
        )docstring",
        py::arg("src"), py::arg("offset"), py::arg("length"))
    .def_static("MakeEmpty", &SkData::MakeEmpty,
        R"docstring(
        Returns a new empty dataref (or a reference to a shared empty dataref).

        New or shared, the caller must see that unref() is eventually called.
        )docstring")
    ;

py::implicitly_convertible<py::buffer, SkData>();
}
