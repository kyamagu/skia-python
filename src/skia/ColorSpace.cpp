#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

void initColorSpace(py::module &m) {
py::class_<SkColorSpace, sk_sp<SkColorSpace>>(m, "ColorSpace")
    // .def("toProfile", &SkColorSpace::toProfile,
    //     "Convert this color space to an skcms ICC profile struct.")
    .def("gammaCloseToSRGB", &SkColorSpace::gammaCloseToSRGB,
        "Returns true if the color space gamma is near enough to be "
        "approximated as sRGB.")
    .def("gammaIsLinear", &SkColorSpace::gammaIsLinear,
        "Returns true if the color space gamma is linear.")
    // .def("isNumericalTransferFn", &SkColorSpace::isNumericalTransferFn,
    //     "Sets |fn| to the transfer function from this color space.")
    // .def("toXYZD50", &SkColorSpace::toXYZD50,
    //     "Returns true and sets |toXYZD50| if the color gamut can be "
    //     "described as a matrix.")
    .def("toXYZD50Hash", &SkColorSpace::toXYZD50Hash,
        "Returns a hash of the gamut transformation to XYZ D50.")
    .def("makeLinearGamma", &SkColorSpace::makeLinearGamma,
        "Returns a color space with the same gamut as this one, but with a "
        "linear gamma.")
    .def("makeSRGBGamma", &SkColorSpace::makeSRGBGamma,
        "Returns a color space with the same gamut as this one, with with the "
        "sRGB transfer function.")
    .def("makeColorSpin", &SkColorSpace::makeColorSpin,
        "Returns a color space with the same transfer function as this one, "
        "but with the primary colors rotated.")
    .def("isSRGB", &SkColorSpace::isSRGB,
        "Returns true if the color space is sRGB.")
    .def("serialize", &SkColorSpace::serialize, "Returns nullptr on failure.")
    .def("writeToMemory", &SkColorSpace::writeToMemory,
        "If |memory| is nullptr, returns the size required to serialize.")
    .def("transferFn",
        (void (SkColorSpace::*)(float[7]) const) &SkColorSpace::transferFn)
    // .def("transferFn",
    //     (void (SkColorSpace::*)(skcms_TransferFunction*) const)
    //     &SkColorSpace::transferFn)
    // .def("invTransferFn", &SkColorSpace::invTransferFn)
    // .def("gamutTransformTo", &SkColorSpace::gamutTransformTo)
    .def("transferFnHash", &SkColorSpace::transferFnHash)
    .def("hash", &SkColorSpace::hash)
    .def("unique", &SkColorSpace::unique)
    .def("ref", &SkColorSpace::ref)
    .def("unref", &SkColorSpace::unref)
    .def("deref", &SkColorSpace::deref)
    .def("refCntGreaterThan", &SkColorSpace::refCntGreaterThan)
    .def_static("MakeSRGB", &SkColorSpace::MakeSRGB)
    .def_static("MakeSRGBLinear", &SkColorSpace::MakeSRGBLinear)
    // .def_static("MakeRGB", &SkColorSpace::MakeRGB)
    // .def_static("Make", &SkColorSpace::Make)
    .def_static("Deserialize", &SkColorSpace::Deserialize)
    .def_static("Equals", &SkColorSpace::Equals)
    ;
}
