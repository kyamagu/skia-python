#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void IRect(py::module &m) {
py::class_<SkIRect>(m, "IRect")
    .def("left", &SkIRect::left)
    .def("top", &SkIRect::top)
    .def("right", &SkIRect::right)
    .def("bottom", &SkIRect::bottom)
    .def("x", &SkIRect::x)
    .def("y", &SkIRect::y)
    .def("topLeft", &SkIRect::topLeft)
    .def("width", &SkIRect::width)
    .def("height", &SkIRect::height)
    .def("size", &SkIRect::size)
    .def("isEmpty", &SkIRect::isEmpty)
    .def("setEmpty", &SkIRect::setEmpty)
    .def("setLTRB", &SkIRect::setLTRB)
    .def("setXYWH", &SkIRect::setXYWH)
    .def("setWH", &SkIRect::setWH)
    .def("makeOffset",
        (SkIRect (SkIRect::*)(int32_t, int32_t) const) &SkIRect::makeOffset)
    // .def("makeOffset",
    //     (SkIRect (SkIRect::*)(SkIVector) const) &SkIRect::makeOffset)
    .def("makeInset", &SkIRect::makeInset)
    .def("makeOutset", &SkIRect::makeOutset)
    .def("offset", py::overload_cast<int32_t, int32_t>(&SkIRect::offset))
    .def("offsetTo", &SkIRect::offsetTo)
    .def("inset", &SkIRect::inset)
    .def("outset", &SkIRect::outset)
    .def("adjust", &SkIRect::adjust)
    .def("contains",
        (bool (SkIRect::*)(int32_t, int32_t) const) &SkIRect::contains)
    .def("contains",
        (bool (SkIRect::*)(const SkIRect&) const) &SkIRect::contains)
    // .def("contains",
    //     (bool (SkIRect::*)(const SkRect&) const) &SkIRect::contains)
    .def("containsNoEmptyCheck", &SkIRect::containsNoEmptyCheck)
    .def("intersect",
        (bool (SkIRect::*)(const SkIRect&)) &SkIRect::intersect)
    .def("intersect",
        (bool (SkIRect::*)(const SkIRect&, const SkIRect&)) &SkIRect::intersect)
    .def("join", &SkIRect::join)
    .def("sort", &SkIRect::sort)
    .def("makeSorted", &SkIRect::makeSorted)
    .def_static("MakeEmpty", &SkIRect::MakeEmpty)
    .def_static("MakeWH", &SkIRect::MakeWH)
    .def_static("MakeSize", &SkIRect::MakeSize)
    .def_static("MakeLTRB", &SkIRect::MakeLTRB)
    .def_static("MakeXYWH", &SkIRect::MakeXYWH)
    .def_static("Intersects", &SkIRect::Intersects)
    .def_readwrite("fLeft", &SkIRect::fLeft)
    .def_readwrite("fTop", &SkIRect::fTop)
    .def_readwrite("fRight", &SkIRect::fRight)
    .def_readwrite("fBottom", &SkIRect::fBottom)
    ;
}
