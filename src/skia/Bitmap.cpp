#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initBitmap(py::module &m) {
py::class_<SkBitmap>(m, "Bitmap", py::buffer_protocol())
    .def_buffer([](SkBitmap &d) -> py::buffer_info {
        return py::buffer_info(
            const_cast<void*>(d.getPixels()),
            d.bytesPerPixel(),
            (d.bytesPerPixel() == 8) ? "B" :
            (d.bytesPerPixel() == 16) ? "H" :
            (d.bytesPerPixel() == 32) ? "I" :
            (d.bytesPerPixel() == 64) ? "Q" : "B",
            2,
            { ssize_t(d.width()), ssize_t(d.height()) },  // Transposed.
            { ssize_t(d.rowBytes()), ssize_t(d.bytesPerPixel()) }
        );
    })
    .def("info", &SkBitmap::info)
    .def("width", &SkBitmap::width)
    .def("height", &SkBitmap::height)
    .def("empty", &SkBitmap::empty)
    ;
}
