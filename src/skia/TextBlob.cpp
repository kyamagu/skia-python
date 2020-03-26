#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

template<>
struct py::detail::has_operator_delete<SkTextBlob, void> : std::false_type {};

void initTextBlob(py::module &m) {
    py::class_<SkTextBlob, sk_sp<SkTextBlob>>(m, "TextBlob")
        ;
}
