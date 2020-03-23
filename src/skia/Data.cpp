#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

void Data(py::module &m) {
py::class_<SkData, sk_sp<SkData>>(m, "Data", py::buffer_protocol())
    .def_buffer([](SkData &d) -> py::buffer_info {
        return py::buffer_info(
            d.writable_data(),
            sizeof(uint8_t),
            py::format_descriptor<uint8_t>::format(),
            1,
            { d.size() },
            { sizeof(uint8_t) }
        );
    })
    .def("size", &SkData::size)
    .def("isEmpty", &SkData::isEmpty)
    ;
}
