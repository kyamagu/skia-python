#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initFont(py::module &m) {
py::class_<SkFont>(m, "Font")
    ;
py::enum_<SkFontHinting>(m, "FontHinting");
py::enum_<SkTextEncoding>(m, "TextEncoding");
}
