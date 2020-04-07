#include <pybind11/pybind11.h>
#include <skia.h>

#define CONST_CALL(T, method) [](const sk_sp<T>& t){\
    if (!t.get()) throw std::runtime_error("null pointer exception."); \
    return t->method();\
}

namespace py = pybind11;

using ColorSpace = sk_sp<SkColorSpace>;

template class sk_sp<SkColorSpace>;  // Explicit instantiation.


void initColorSpace(py::module &m) {
py::class_<ColorSpace>(m, "ColorSpace")
    .def(py::init([] () { return ColorSpace(nullptr); }),
        "Create a null color space.")
    // .def("toProfile", &SkColorSpace::toProfile,
    //     "Convert this color space to an skcms ICC profile struct.")
    .def("gammaCloseToSRGB", CONST_CALL(SkColorSpace, gammaCloseToSRGB),
        "Returns true if the color space gamma is near enough to be "
        "approximated as sRGB.")
    .def("gammaIsLinear", CONST_CALL(SkColorSpace, gammaIsLinear),
        "Returns true if the color space gamma is linear.")
    // .def("isNumericalTransferFn", &SkColorSpace::isNumericalTransferFn,
    //     "Sets |fn| to the transfer function from this color space.")
    // .def("toXYZD50", &SkColorSpace::toXYZD50,
    //     "Returns true and sets |toXYZD50| if the color gamut can be "
    //     "described as a matrix.")
    .def("toXYZD50Hash", CONST_CALL(SkColorSpace, toXYZD50Hash),
        "Returns a hash of the gamut transformation to XYZ D50.")
    .def("makeLinearGamma", CONST_CALL(SkColorSpace, makeLinearGamma),
        "Returns a color space with the same gamut as this one, but with a "
        "linear gamma.")
    .def("makeSRGBGamma", CONST_CALL(SkColorSpace, makeSRGBGamma),
        "Returns a color space with the same gamut as this one, with with the "
        "sRGB transfer function.")
    .def("makeColorSpin", CONST_CALL(SkColorSpace, makeColorSpin),
        "Returns a color space with the same transfer function as this one, "
        "but with the primary colors rotated.")
    .def("isSRGB", CONST_CALL(SkColorSpace, isSRGB),
        "Returns true if the color space is sRGB.")
    .def("serialize", CONST_CALL(SkColorSpace, serialize),
        "Returns nullptr on failure.")
    .def("writeToMemory", [](const ColorSpace& cs, void* memory) {
        return cs->writeToMemory(memory);
    },
        "If memory is nullptr, returns the size required to serialize.")
    .def("transferFn", [] (const ColorSpace& cs, float gabcdef[7]) {
        cs->transferFn(gabcdef);
    })
    // .def("transferFn",
    //     (void (SkColorSpace::*)(skcms_TransferFunction*) const)
    //     &SkColorSpace::transferFn)
    // .def("invTransferFn", &SkColorSpace::invTransferFn)
    // .def("gamutTransformTo", &SkColorSpace::gamutTransformTo)
    .def("transferFnHash", CONST_CALL(SkColorSpace, transferFnHash))
    .def("hash", CONST_CALL(SkColorSpace, hash))
    .def("unique", CONST_CALL(SkColorSpace, unique))
    .def("ref", CONST_CALL(SkColorSpace, ref))
    .def("unref", CONST_CALL(SkColorSpace, unref))
    .def("deref", CONST_CALL(SkColorSpace, deref))
    .def("refCntGreaterThan",
        [] (const ColorSpace& cs, int32_t threadIsolatedTestCnt) {
            return cs->refCntGreaterThan(threadIsolatedTestCnt);
        })
    .def_static("MakeSRGB", &SkColorSpace::MakeSRGB,
        "Create the sRGB color space.")
    .def_static("MakeSRGBLinear", &SkColorSpace::MakeSRGBLinear,
        "Create the sRGB color space.")
    // .def("MakeRGB", &SkColorSpace::MakeRGB,
    //     "Create an SkColorSpace from a transfer function and a row-major 3x3 "
    //     "transformation to XYZ.")
    // .def("Make", &SkColorSpace::Make,
    //     "Create an SkColorSpace from a parsed (skcms) ICC profile.")
    .def_static("Deserialize", &SkColorSpace::Deserialize)
    .def_static("Equals", &SkColorSpace::Equals,
        "If both are null, we return true.")
    ;
}
