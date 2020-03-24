#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initImageInfo(py::module &m) {
py::class_<SkColorInfo>(m, "ColorInfo")
    .def(py::init<>(),
        "Creates an SkColorInfo with kUnknown_SkColorType, "
        "kUnknown_SkAlphaType, and no SkColorSpace.")
    .def(py::init<SkColorType, SkAlphaType, sk_sp<SkColorSpace>>(),
        "Creates SkColorInfo from SkColorType ct, SkAlphaType at, and "
        "optionally SkColorSpace cs.")
    .def(py::init<const SkColorInfo&>())
    // .def(py::init<SkColorInfo&&>())
    .def("colorSpace", &SkColorInfo::colorSpace,
        py::return_value_policy::reference)
    .def("refColorSpace", &SkColorInfo::refColorSpace)
    .def("colorType", &SkColorInfo::colorType)
    .def("alphaType", &SkColorInfo::alphaType)
    .def("isOpaque", &SkColorInfo::isOpaque)
    .def("gammaCloseToSRGB", &SkColorInfo::gammaCloseToSRGB)
    .def("__eq__", &SkColorInfo::operator==,
        "Does other represent the same color type, alpha type, and color "
        "space?",
        py::is_operator())
    .def("__ne__", &SkColorInfo::operator!=,
        "Does other represent a different color type, alpha type, or color "
        "space?",
        py::is_operator())
    .def("makeAlphaType", &SkColorInfo::makeAlphaType,
        "Creates SkColorInfo with same SkColorType, SkColorSpace, with "
        "SkAlphaType set to newAlphaType.")
    .def("makeColorType", &SkColorInfo::makeColorType,
        "Creates new SkColorInfo with same SkAlphaType, SkColorSpace, with "
        "SkColorType set to newColorType.")
    .def("makeColorSpace", &SkColorInfo::makeColorSpace,
        "Creates SkColorInfo with same SkAlphaType, SkColorType, with "
        "SkColorSpace set to cs.")
    .def("bytesPerPixel", &SkColorInfo::bytesPerPixel,
        "Returns number of bytes per pixel required by SkColorType.")
    .def("shiftPerPixel", &SkColorInfo::shiftPerPixel,
        "Returns bit shift converting row bytes to row pixels.")
    ;
py::class_<SkImageInfo>(m, "ImageInfo")
    .def(py::init<>())
    .def("width", &SkImageInfo::width, "Returns pixel count in each row.")
    .def("height", &SkImageInfo::height, "Returns pixel row count.")
    .def("colorType", &SkImageInfo::colorType)
    .def("alphaType", &SkImageInfo::alphaType)
    .def("colorSpace", &SkImageInfo::colorSpace,
        "Returns SkColorSpace, the range of colors.",
        py::return_value_policy::reference)
    .def("refColorSpace", &SkImageInfo::refColorSpace,
        "Returns smart pointer to SkColorSpace, the range of colors.")
    .def("isEmpty", &SkImageInfo::isEmpty,
        "Returns if SkImageInfo describes an empty area of pixels by checking "
        "if either width or height is zero or smaller.")
    .def("colorInfo", &SkImageInfo::colorInfo,
        "Returns the dimensionless SkColorInfo that represents the same color "
        "type, alpha type, and color space as this SkImageInfo.")
    .def("isOpaque", &SkImageInfo::isOpaque, "Returns true if SkAlphaType is "
        "set to hint that all pixels are opaque; their alpha value is "
        "implicitly or explicitly 1.0.")
    .def("dimensions", &SkImageInfo::dimensions,
        "Returns SkISize { width(), height() }.")
    .def("bounds", &SkImageInfo::bounds,
        "Returns SkIRect { 0, 0, width(), height() }.")
    .def("gammaCloseToSRGB", &SkImageInfo::gammaCloseToSRGB,
        "Returns true if associated SkColorSpace is not nullptr, and "
        "SkColorSpace gamma is approximately the same as sRGB.")
    .def("makeWH", &SkImageInfo::makeWH,
        "Creates SkImageInfo with the same SkColorType, SkColorSpace, and "
        "SkAlphaType, with dimensions set to width and height.")
    .def("makeDimensions", &SkImageInfo::makeDimensions,
        "Creates SkImageInfo with the same SkColorType, SkColorSpace, and "
        "SkAlphaType, with dimensions set to newDimensions.")
    .def("makeAlphaType", &SkImageInfo::makeAlphaType,
        "Creates SkImageInfo with same SkColorType, SkColorSpace, width, and "
        "height, with SkAlphaType set to newAlphaType.")
    .def("makeColorType", &SkImageInfo::makeColorType,
        "Creates SkImageInfo with same SkAlphaType, SkColorSpace, width, and "
        "height, with SkColorType set to newColorType.")
    .def("makeColorSpace", &SkImageInfo::makeColorSpace,
        "Creates SkImageInfo with same SkAlphaType, SkColorType, width, and "
        "height, with SkColorSpace set to cs.")
    .def("bytesPerPixel", &SkImageInfo::bytesPerPixel,
        "Returns number of bytes per pixel required by SkColorType.")
    .def("shiftPerPixel", &SkImageInfo::shiftPerPixel,
        "Returns bit shift converting row bytes to row pixels.")
    .def("minRowBytes64", &SkImageInfo::minRowBytes64,
        "Returns minimum bytes per row, computed from pixel width() and "
        "SkColorType, which specifies bytesPerPixel().")
    .def("minRowBytes", &SkImageInfo::minRowBytes,
        "Returns minimum bytes per row, computed from pixel width() and "
        "SkColorType, which specifies bytesPerPixel().")
    .def("computeOffset", &SkImageInfo::computeOffset,
        "Returns byte offset of pixel from pixel base address.")
    .def("__eq__", &SkImageInfo::operator==,
        "Compares SkImageInfo with other, and returns true if width, height, "
        "SkColorType, SkAlphaType, and SkColorSpace are equivalent.",
        py::is_operator())
    .def("__ne__", &SkImageInfo::operator!=,
        "Compares SkImageInfo with other, and returns true if width, height, "
        "SkColorType, SkAlphaType, and SkColorSpace are not equivalent.",
        py::is_operator())
    .def("computeByteSize", &SkImageInfo::computeByteSize,
        "Returns storage required by pixel array, given SkImageInfo dimensions,"
        " SkColorType, and rowBytes.")
    .def("computeMinByteSize", &SkImageInfo::computeMinByteSize,
        "Returns storage required by pixel array, given SkImageInfo dimensions,"
        " and SkColorType.")
    .def("validRowBytes", &SkImageInfo::validRowBytes,
        "Returns true if rowBytes is valid for this SkImageInfo.")
    .def("reset", &SkImageInfo::reset,
        "Creates an empty SkImageInfo with kUnknown_SkColorType, "
        "kUnknown_SkAlphaType, a width and height of zero, and no "
        "SkColorSpace.")
    .def_static("Make",
        (SkImageInfo (*)(int, int, SkColorType, SkAlphaType,
            sk_sp<SkColorSpace>)) &SkImageInfo::Make,
        "Creates SkImageInfo from integral dimensions width and height, "
        "SkColorType ct, SkAlphaType at, and optionally SkColorSpace cs.")
    .def_static("Make",
        (SkImageInfo (*)(SkISize, SkColorType, SkAlphaType,
            sk_sp<SkColorSpace>)) &SkImageInfo::Make)
    .def_static("Make",
        (SkImageInfo (*)(SkISize, const SkColorInfo&)) &SkImageInfo::Make,
        "Creates SkImageInfo from integral dimensions and SkColorInfo "
        "colorInfo.")
    // .def_static("Make",
    //     (SkImageInfo (*)(SkISize, SkColorInfo&&)) &SkImageInfo::Make)
    .def_static("MakeN32", &SkImageInfo::MakeN32,
        "Creates SkImageInfo from integral dimensions width and height, "
        "kN32_SkColorType, SkAlphaType at, and optionally SkColorSpace cs.")
    .def_static("MakeN32Premul",
        (SkImageInfo (*)(int, int, sk_sp<SkColorSpace>))
        &SkImageInfo::MakeN32Premul,
        "Creates SkImageInfo from integral dimensions width and height, "
        "kN32_SkColorType, kPremul_SkAlphaType, with optional SkColorSpace.")
    .def_static("MakeN32Premul",
        (SkImageInfo (*)(SkISize, sk_sp<SkColorSpace>))
        &SkImageInfo::MakeN32Premul,
        "Creates SkImageInfo from integral dimensions width and height, "
        "kN32_SkColorType, kPremul_SkAlphaType, with SkColorSpace set to "
        "nullptr.")
    .def_static("MakeA8", (SkImageInfo (*)(int, int)) &SkImageInfo::MakeA8,
        "Creates SkImageInfo from integral dimensions width and height, "
        "kAlpha_8_SkColorType, kPremul_SkAlphaType, with SkColorSpace set to "
        "nullptr.")
    .def_static("MakeA8", (SkImageInfo (*)(SkISize)) &SkImageInfo::MakeA8,
        "Creates SkImageInfo from integral dimensions, kAlpha_8_SkColorType, "
        "kPremul_SkAlphaType, with SkColorSpace set to nullptr.")
    .def_static("MakeUnknown",
        (SkImageInfo (*)(int, int)) &SkImageInfo::MakeUnknown,
        "Creates SkImageInfo from integral dimensions width and height, "
        "kUnknown_SkColorType, kUnknown_SkAlphaType, with SkColorSpace set to "
        "nullptr.")
    .def_static("MakeUnknown",
        (SkImageInfo (*)()) &SkImageInfo::MakeUnknown,
        "Creates SkImageInfo from integral dimensions width and height set to "
        "zero, kUnknown_SkColorType, kUnknown_SkAlphaType, with SkColorSpace "
        "set to nullptr.")
    .def_static("ByteSizeOverflowed", &SkImageInfo::ByteSizeOverflowed,
        "Returns true if byteSize equals SIZE_MAX.")
    ;
py::enum_<SkAlphaType>(m, "AlphaType");
py::enum_<SkColorType>(m, "ColorType");
py::enum_<SkYUVColorSpace>(m, "YUVColorSpace");
m.def("AlphaTypeIsOpaque", &SkAlphaTypeIsOpaque);
m.def("ColorTypeBytesPerPixel", &SkColorTypeBytesPerPixel);
m.def("ColorTypeIsAlwaysOpaque", &SkColorTypeIsAlwaysOpaque);
m.def("ColorTypeValidateAlphaType", &SkColorTypeValidateAlphaType);
}
