#include "common.h"
#include "modules/skunicode/include/SkUnicode_icu.h"

void initUnicode(py::module &m) {

  py::class_<SkUnicode, sk_sp<SkUnicode>, SkRefCnt> unicode(m, "Unicode");

unicode
    .def(py::init(
        [] (void) {
            return SkUnicodes::ICU::Make();
        }))
    .def_static("ICU_Make", &SkUnicodes::ICU::Make)
    ;

// SkUnicodes::ICU is a namespace
py::object SimpleNamespace = py::module_::import("types").attr("SimpleNamespace");
m.attr("Unicodes") = SimpleNamespace();
m.attr("Unicodes").attr("ICU") = SimpleNamespace();
m.attr("Unicodes").attr("ICU").attr("Make") = m.attr("Unicode").attr("ICU_Make");
}
