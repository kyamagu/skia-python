#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

void initAutoCanvasRestore(py::module &m) {
py::class_<SkAutoCanvasRestore>(m, "AutoCanvasRestore")
    .def(py::init<SkCanvas*, bool>(), "Preserves SkCanvas::save() count.")
    .def("restore", &SkAutoCanvasRestore::restore,
        "Restores SkCanvas to saved state immediately.")
    ;
}
