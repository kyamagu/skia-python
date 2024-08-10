#include "common.h"
#include "modules/skunicode/include/SkUnicode_icu.h"

void initUnicode(py::module &m) {

  py::class_<SkUnicode, sk_sp<SkUnicode>, SkRefCnt> unicode(m, "Unicode");

unicode
    .def(py::init(
        [] (void) {
            return SkUnicodes::ICU::Make();
        }))
    .def_static("ICU.Make", &SkUnicodes::ICU::Make)
    ;

// SkUnicodes::ICU is a namespace
m.attr("Unicodes") = m.attr("Unicode");
}
