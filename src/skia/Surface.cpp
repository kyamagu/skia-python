#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

void initSurface(py::module &m) {
py::class_<SkSurface, sk_sp<SkSurface>>(m, "Surface")
    .def(py::init([](int width, int height) {
        return sk_sp<SkSurface>(
            SkSurface::MakeRasterN32Premul(width, height));
    }))
    .def("getCanvas", &SkSurface::getCanvas,
        py::return_value_policy::reference)
    .def("makeImageSnapshot",
        py::overload_cast<>(&SkSurface::makeImageSnapshot),
        "Returns SkImage capturing SkSurface contents.")
    ;
}
