#include "common.h"

namespace {

py::str StringToPyStr(const SkString& string) {
    return py::str(string.c_str(), string.size());
}

}  // namespace


void initString(py::module &m) {

py::class_<SkString>(m, "String")
    .def("__repr__", &StringToPyStr)
    .def("__str__", &StringToPyStr)
    .def(py::init<>())
    .def(py::init<size_t>(), py::arg("size"))
    .def(py::init(
        [] (py::str str) {
            py::ssize_t length = 0;
            auto buffer = PyUnicode_AsUTF8AndSize(str.ptr(), &length);
            if (!buffer)
                throw py::value_error("Failed to get UTF-8 str.");
            return SkString(buffer, length);
        }),
        py::arg("text"))
    .def(py::init<const char*>(), py::arg("text"))
    .def(py::init<const char*, size_t>(), py::arg("text"), py::arg("size"))
    .def(py::init<const SkString&>(), py::arg("other"))
    .def("isEmpty", &SkString::isEmpty)
    .def("size", &SkString::size)
    .def("c_str", &SkString::c_str)
    .def("__getitem__",
        [] (const SkString& self, size_t index) {
            if (index < 0 || self.size() <= index)
                throw std::out_of_range("Invalid index.");
            return self[index];
        })
    .def("__setitem__",
        [] (SkString& self, size_t index, char value) {
            if (index < 0 || self.size() <= index)
                throw std::out_of_range("Invalid index.");
            self[index] = value;
            return self;
        })
    .def("__len__", &SkString::size)
    .def("equals",
        py::overload_cast<const SkString&>(&SkString::equals, py::const_),
        py::arg("text"))
    .def("equals",
        py::overload_cast<const char*>(&SkString::equals, py::const_),
        py::arg("text"))
    .def("equals",
        py::overload_cast<const char*, size_t>(&SkString::equals, py::const_),
        py::arg("text"), py::arg("size"))
    .def("__eq__",
        py::overload_cast<const SkString&>(&SkString::equals, py::const_),
        py::arg("text"), py::is_operator())
    .def("startsWith",
        py::overload_cast<const char*>(&SkString::startsWith, py::const_),
        py::arg("prefixStr"))
    .def("startsWith",
        py::overload_cast<const char>(&SkString::startsWith, py::const_),
        py::arg("prefixChar"))
    .def("endsWith",
        py::overload_cast<const char*>(&SkString::endsWith, py::const_),
        py::arg("suffixStr"))
    .def("endsWith",
        py::overload_cast<const char>(&SkString::endsWith, py::const_),
        py::arg("suffixChar"))
    .def("contains",
        py::overload_cast<const char*>(&SkString::contains, py::const_),
        py::arg("subStr"))
    .def("contains",
        py::overload_cast<const char>(&SkString::contains, py::const_),
        py::arg("subChar"))
    .def("__contains__",
        py::overload_cast<const char*>(&SkString::contains, py::const_),
        py::arg("subStr"))
    .def("find", &SkString::find, py::arg("substring"))
    .def("findLastOf", &SkString::findLastOf, py::arg("subchar"))
    // .def("writable_str", &SkString::writable_str)
    .def("reset", &SkString::reset)
    .def("resize", &SkString::resize, py::arg("size"))
    .def("set", py::overload_cast<const SkString&>(&SkString::set),
        py::arg("src"))
    .def("set", py::overload_cast<const char*>(&SkString::set),
        py::arg("text"))
    .def("set", py::overload_cast<const char*, size_t>(&SkString::set),
        py::arg("text"), py::arg("size"))
    .def("insert",
        py::overload_cast<size_t, const SkString&>(&SkString::insert),
        py::arg("offset"), py::arg("src"))
    .def("insert", py::overload_cast<size_t, const char*>(&SkString::insert),
        py::arg("offset"), py::arg("text"))
    .def("insert",
        py::overload_cast<size_t, const char*, size_t>(&SkString::insert),
        py::arg("offset"), py::arg("text"), py::arg("size"))
    .def("insertUnichar", &SkString::insertUnichar,
        py::arg("offset"), py::arg("value"))
    .def("insertS32", &SkString::insertS32,
        py::arg("offset"), py::arg("value"))
    .def("insertS64", &SkString::insertS64,
        py::arg("offset"), py::arg("value"), py::arg("minDigits") = 0)
    .def("insertU32", &SkString::insertU32,
        py::arg("offset"), py::arg("value"))
    .def("insertU64", &SkString::insertU64,
        py::arg("offset"), py::arg("value"), py::arg("minDigits") = 0)
    .def("insertHex", &SkString::insertHex,
        py::arg("offset"), py::arg("value"), py::arg("minDigits") = 0)
    .def("insertScalar", &SkString::insertScalar,
        py::arg("offset"), py::arg("value"))
    .def("append",
        py::overload_cast<const SkString&>(&SkString::append),
        py::arg("str"))
    .def("append",
        py::overload_cast<const char*>(&SkString::append),
        py::arg("text"))
    .def("append",
        py::overload_cast<const char*, size_t>(&SkString::append),
        py::arg("text"), py::arg("size"))
    .def("appendUnichar", &SkString::appendUnichar,
        py::arg("value"))
    .def("appendS32", &SkString::appendS32,
        py::arg("value"))
    .def("appendS64", &SkString::appendS64,
        py::arg("value"), py::arg("minDigits") = 0)
    .def("appendU32", &SkString::appendU32,
        py::arg("value"))
    .def("appendU64", &SkString::appendU64,
        py::arg("value"), py::arg("minDigits") = 0)
    .def("appendHex", &SkString::appendHex,
        py::arg("value"), py::arg("minDigits") = 0)
    .def("appendScalar", &SkString::appendScalar,
        py::arg("value"))
    .def("prepend",
        py::overload_cast<const SkString&>(&SkString::prepend),
        py::arg("str"))
    .def("prepend",
        py::overload_cast<const char*>(&SkString::prepend),
        py::arg("text"))
    .def("prepend",
        py::overload_cast<const char*, size_t>(&SkString::prepend),
        py::arg("text"), py::arg("size"))
    .def("prependUnichar", &SkString::prependUnichar,
        py::arg("value"))
    .def("prependS32", &SkString::prependS32,
        py::arg("value"))
    .def("prependS64", &SkString::prependS64,
        py::arg("value"), py::arg("minDigits") = 0)
    .def("prependHex", &SkString::prependHex,
        py::arg("value"), py::arg("minDigits") = 0)
    .def("prependScalar", &SkString::prependScalar,
        py::arg("value"))
    .def("remove", &SkString::remove, py::arg("offset"), py::arg("size"))
    .def("__iadd__",
        [] (SkString& s, const SkString& other) { return s += other; },
        py::arg("other"), py::is_operator())
    .def("__iadd__",
        [] (SkString& s, const char* other) { return s += other; },
        py::arg("other"), py::is_operator())
    .def("__iadd__",
        [] (SkString& s, const char other) { return s += other; },
        py::arg("other"), py::is_operator())
    .def("swap", &SkString::swap,
        R"docstring(
        Swap contents between this and other.

        This function is guaranteed to never fail or throw.
        )docstring",
        py::arg("other"))
    ;

py::implicitly_convertible<py::str, SkString>();

}
