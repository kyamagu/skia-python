#include "common.h"

void initSize(py::module &m) {
// ISize
py::class_<SkISize>(m, "ISize")
    .def(py::init(&SkISize::MakeEmpty))
    .def(py::init(&SkISize::Make), py::arg("width"), py::arg("height"))
    .def(py::init(
        [] (py::tuple t) {
            if (t.size() != 2)
                throw py::value_error("ISize must have exactly two elements.");
            return SkISize::Make(t[0].cast<int32_t>(), t[1].cast<int32_t>());
        }),
        py::arg("t"))
    .def("set", &SkISize::set, py::arg("width"), py::arg("height"))
    .def("isZero", &SkISize::isZero,
        R"docstring(
        Returns true iff fWidth == 0 && fHeight == 0.
        )docstring")
    .def("isEmpty", &SkISize::isEmpty,
        "Returns true if either width or height are <= 0.")
    .def("setEmpty", &SkISize::setEmpty,
        R"docstring(
        Set the width and height to 0.
        )docstring")
    .def("width", &SkISize::width)
    .def("height", &SkISize::height)
    .def("area", &SkISize::area)
    .def("equals", &SkISize::equals, py::arg("width"), py::arg("height"))
    .def("__eq__",
        [] (const SkISize& self, const SkISize& other) {
            return self.fWidth == other.fWidth && self.fHeight == other.fHeight;
        },
        py::arg("other"), py::is_operator())
    .def("__ne__",
        [] (const SkISize& self, const SkISize& other) {
            return self.fWidth != other.fWidth || self.fHeight != other.fHeight;
        },
        py::arg("other"), py::is_operator())
    .def_static("Make", &SkISize::Make, py::arg("width"), py::arg("height"))
    .def_static("MakeEmpty", &SkISize::MakeEmpty)
    .def_readwrite("fWidth", &SkISize::fWidth)
    .def_readwrite("fHeight", &SkISize::fHeight)
    .def("__iter__",
        [] (const SkISize& isize) {
            return py::make_iterator(&isize.fWidth, &isize.fWidth + 2);
        })
    .def("__len__", [] (const SkISize& isize) { return 2; })
    .def("__repr__",
        [] (const SkISize& isize) {
            std::stringstream s;
            s << "ISize(" << isize.fWidth << ", " << isize.fHeight << ")";
            return s.str();
        })
    ;

py::implicitly_convertible<py::tuple, SkISize>();

// Size
py::class_<SkSize>(m, "Size")
    .def(py::init(&SkSize::MakeEmpty))
    .def(py::init(py::overload_cast<SkScalar, SkScalar>(&SkSize::Make)),
        py::arg("width"), py::arg("height"))
    .def(py::init(py::overload_cast<const SkISize&>(&SkSize::Make)),
        py::arg("isize"))
    .def(py::init(
        [] (py::tuple t) {
            if (t.size() != 2)
                throw py::value_error("Size must have exactly two elements.");
            return SkSize::Make(t[0].cast<SkScalar>(), t[1].cast<SkScalar>());
        }),
        py::arg("t"))
    .def("set", &SkSize::set, py::arg("width"), py::arg("height"))
    .def("isZero", &SkSize::isZero,
        R"docstring(
        Returns true iff fWidth == 0 && fHeight == 0.
        )docstring")
    .def("isEmpty", &SkSize::isEmpty,
        R"docstring(
        Returns true if either width or height are <= 0.
        )docstring")
    .def("setEmpty", &SkSize::setEmpty,
        R"docstring(
        Set the width and height to 0.
        )docstring")
    .def("width", &SkSize::width)
    .def("height", &SkSize::height)
    .def("equals", &SkSize::equals, py::arg("width"), py::arg("height"))
    .def("toRound", &SkSize::toRound)
    .def("toCeil", &SkSize::toCeil)
    .def("toFloor", &SkSize::toFloor)
    .def("__eq__",
        [] (const SkSize& self, const SkSize& other) {
            return self.fWidth == other.fWidth && self.fHeight == other.fHeight;
        },
        py::arg("other"), py::is_operator())
    .def("__ne__",
        [] (const SkSize& self, const SkSize& other) {
            return self.fWidth != other.fWidth || self.fHeight != other.fHeight;
        },
        py::arg("other"), py::is_operator())
    .def_static("Make", py::overload_cast<SkScalar, SkScalar>(&SkSize::Make),
        py::arg("width"), py::arg("height"))
    .def_static("Make", py::overload_cast<const SkISize&>(&SkSize::Make),
        py::arg("isize"))
    .def_static("MakeEmpty", &SkSize::MakeEmpty)
    .def_readwrite("fWidth", &SkSize::fWidth)
    .def_readwrite("fHeight", &SkSize::fHeight)
    .def("__iter__",
        [] (const SkSize& size) {
            return py::make_iterator(&size.fWidth, &size.fWidth + 2);
        })
    .def("__len__", [] (const SkSize& size) { return 2; })
    .def("__repr__",
        [] (const SkSize& size) {
            std::stringstream s;
            s << "Size(" << size.fWidth << ", " << size.fHeight << ")";
            return s.str();
        })
    ;

py::implicitly_convertible<SkISize, SkSize>();
py::implicitly_convertible<py::tuple, SkSize>();
}
