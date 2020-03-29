#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

void initSurface(py::module &m) {
py::class_<SkSurfaceProps>(m, "SurfaceProps")
    ;
py::class_<SkSurface, sk_sp<SkSurface>>(m, "Surface", py::buffer_protocol())
    .def(py::init([](int width, int height) -> sk_sp<SkSurface> {
        auto surface = SkSurface::MakeRasterN32Premul(width, height);
        if (surface.get() == nullptr)
            throw std::runtime_error("Failed to allocate surface.");
        return surface;
    }))
    .def("getCanvas", &SkSurface::getCanvas,
        py::return_value_policy::reference)
    .def("makeImageSnapshot",
        py::overload_cast<>(&SkSurface::makeImageSnapshot),
        "Returns SkImage capturing SkSurface contents.")
    .def_static("MakeRasterDirect", [](const SkImageInfo& info, py::buffer b) {
        py::buffer_info binfo = b.request();
        if (binfo.format != py::format_descriptor<uint8_t>::format())
            throw std::runtime_error("Incompatible format.");
        return SkSurface::MakeRasterDirect(info, binfo.ptr, binfo.strides[0]);
    })
    ;
}
