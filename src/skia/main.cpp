#include <pybind11/pybind11.h>
#include <skia.h>
#include <stdexcept>
#include <utility>

using namespace std;
namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

// Declarations.
void AutoCanvasRestore(py::module &);
void Canvas(py::module &);
void Data(py::module &);
void Image(py::module &);
void ImageInfo(py::module &);
void IPoint(py::module &);
void IRect(py::module &);
void ISize(py::module &);
void Surface(py::module &);
void Paint(py::module &);
void Path(py::module &);

// Main entry point.
PYBIND11_MODULE(skia, m) {
    m.doc() = R"docstring(
        Pybind11 skia plugin
        --------------------
        .. currentmodule:: skia
        .. autosummary::
           :toctree: _generate
           SkSurface
    )docstring";

    AutoCanvasRestore(m);
    Canvas(m);
    Data(m);
    Image(m);
    ImageInfo(m);
    IPoint(m);
    IRect(m);
    ISize(m);
    Surface(m);
    Paint(m);
    Path(m);

    m.def("ColorSetARGB", &SkColorSetARGB,
        "Returns color value from 8-bit component values.");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
