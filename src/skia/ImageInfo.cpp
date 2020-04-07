#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

#pragma optimize("", off)
void initImageInfo(py::module &m) {
py::enum_<SkAlphaType>(m, "AlphaType")
    .value("kUnknown", SkAlphaType::kUnknown_SkAlphaType,
        "uninitialized")
    .value("kOpaque", SkAlphaType::kOpaque_SkAlphaType,
        "pixel is opaque")
    .value("kPremul", SkAlphaType::kPremul_SkAlphaType,
        "pixel components are premultiplied by alpha")
    .value("kUnpremul", SkAlphaType::kUnpremul_SkAlphaType,
        "pixel components are independent of alpha")
    .value("kLastEnum", SkAlphaType::kLastEnum_SkAlphaType,
        "last valid value")
    .export_values();

py::enum_<SkColorType>(m, "ColorType")
    .value("kUnknown", SkColorType::kUnknown_SkColorType, "uninitialized")
    .value("kAlpha_8", SkColorType::kAlpha_8_SkColorType,
        "pixel with alpha in 8-bit byte")
    .value("kRGB_565", SkColorType::kRGB_565_SkColorType,
        "pixel with 5 bits red, 6 bits green, 5 bits blue, in 16-bit word")
    .value("kARGB_4444", SkColorType::kARGB_4444_SkColorType,
        "pixel with 4 bits for alpha, red, green, blue; in 16-bit word")
    .value("kRGBA_8888", SkColorType::kRGBA_8888_SkColorType,
        "pixel with 8 bits for red, green, blue, alpha; in 32-bit word")
    .value("kRGB_888x", SkColorType::kRGB_888x_SkColorType,
        "pixel with 8 bits each for red, green, blue; in 32-bit word")
    .value("kBGRA_8888", SkColorType::kBGRA_8888_SkColorType,
        "pixel with 8 bits for blue, green, red, alpha; in 32-bit word")
    .value("kRGBA_1010102", SkColorType::kRGBA_1010102_SkColorType,
        "10 bits for red, green, blue; 2 bits for alpha; in 32-bit word")
    .value("kBGRA_1010102", SkColorType::kBGRA_1010102_SkColorType,
        "10 bits for blue, green, red; 2 bits for alpha; in 32-bit word")
    .value("kRGB_101010x", SkColorType::kRGB_101010x_SkColorType,
        "pixel with 10 bits each for red, green, blue; in 32-bit word")
    .value("kBGR_101010x", SkColorType::kBGR_101010x_SkColorType,
        "pixel with 10 bits each for blue, green, red; in 32-bit word")
    .value("kGray_8", SkColorType::kGray_8_SkColorType,
        "pixel with grayscale level in 8-bit byte")
    .value("kRGBA_F16Norm", SkColorType::kRGBA_F16Norm_SkColorType,
        "pixel with half floats in [0,1] for red, green, blue, alpha; in "
        "64-bit word")
    .value("kRGBA_F16", SkColorType::kRGBA_F16_SkColorType,
        "pixel with half floats for red, green, blue, alpha; in 64-bit word")
    .value("kRGBA_F32", SkColorType::kRGBA_F32_SkColorType,
        "pixel using C float for red, green, blue, alpha; in 128-bit word")
    .value("kR8G8_unorm", SkColorType::kR8G8_unorm_SkColorType)
    .value("kA16_float", SkColorType::kA16_float_SkColorType)
    .value("kR16G16_float", SkColorType::kR16G16_float_SkColorType)
    .value("kA16_unorm", SkColorType::kA16_unorm_SkColorType)
    .export_values();

py::enum_<SkYUVColorSpace>(m, "YUVColorSpace")
    .value("kJPEG", SkYUVColorSpace::kJPEG_SkYUVColorSpace,
        "describes full range")
    .value("kRec601", SkYUVColorSpace::kRec601_SkYUVColorSpace,
        "describes SDTV range")
    .value("kRec709", SkYUVColorSpace::kRec709_SkYUVColorSpace,
        "describes HDTV range")
    .value("kBT2020", SkYUVColorSpace::kBT2020_SkYUVColorSpace,
        "describes UHDTV range, non-constant-luminance")
    .value("kIdentity", SkYUVColorSpace::kIdentity_SkYUVColorSpace,
        "maps Y->R, U->G, V->B")
    .value("kLastEnum", SkYUVColorSpace::kLastEnum_SkYUVColorSpace,
        "last valid value")
    .export_values();

py::class_<SkColorInfo>(m, "ColorInfo", R"docstring(
    Describes pixel and encoding.

    SkImageInfo can be created from SkColorInfo by providing dimensions.

    It encodes how pixel bits describe alpha, transparency; color components
    red, blue, and green; and SkColorSpace, the range and linearity of colors.
    )docstring")
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

py::class_<SkImageInfo>(m, "ImageInfo", R"docstring(
    Describes pixel dimensions and encoding.

    SkBitmap, SkImage, PixMap, and SkSurface can be created from SkImageInfo.
    SkImageInfo can be retrieved from SkBitmap and SkPixmap, but not from
    SkImage and SkSurface. For example, SkImage and SkSurface implementations
    may defer pixel depth, so may not completely specify SkImageInfo.

    SkImageInfo contains dimensions, the pixel integral width and height. It
    encodes how pixel bits describe alpha, transparency; color components red,
    blue, and green; and SkColorSpace, the range and linearity of colors.
    )docstring")
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
        py::overload_cast<int, int, SkColorType, SkAlphaType,
            sk_sp<SkColorSpace>>(&SkImageInfo::Make),
        "Creates SkImageInfo from integral dimensions width and height, "
        "SkColorType ct, SkAlphaType at, and optionally SkColorSpace cs.",
        py::arg("width"), py::arg("height"), py::arg("ct"), py::arg("at"),
        py::arg_v("cs", sk_sp<SkColorSpace>(nullptr), "skia.ColorSpace()"))
    .def_static("Make",
        py::overload_cast<SkISize, SkColorType, SkAlphaType,
            sk_sp<SkColorSpace>>(&SkImageInfo::Make),
        py::arg("dimensions"), py::arg("ct"), py::arg("at"),
        py::arg_v("cs", sk_sp<SkColorSpace>(nullptr), "skia.ColorSpace()"))
    .def_static("Make",
        py::overload_cast<SkISize, const SkColorInfo&>(&SkImageInfo::Make),
        "Creates SkImageInfo from integral dimensions and SkColorInfo "
        "colorInfo.",
        py::arg("dimensions"), py::arg("colorInfo"))
    // .def_static("Make",
    //     (SkImageInfo (*)(SkISize, SkColorInfo&&)) &SkImageInfo::Make)
    .def_static("MakeN32", &SkImageInfo::MakeN32,
        "Creates SkImageInfo from integral dimensions width and height, "
        "kN32_SkColorType, SkAlphaType at, and optionally SkColorSpace cs.",
        py::arg("width"), py::arg("height"), py::arg("at"),
        py::arg_v("cs", sk_sp<SkColorSpace>(nullptr), "skia.ColorSpace()"))
    .def_static("MakeS32", &SkImageInfo::MakeS32,
        "Creates SkImageInfo from integral dimensions width and height, "
        "kN32_SkColorType, SkAlphaType at, with sRGB SkColorSpace.",
        py::arg("width"), py::arg("height"), py::arg("at"))
    .def_static("MakeN32Premul",
        py::overload_cast<int, int, sk_sp<SkColorSpace>>(
            &SkImageInfo::MakeN32Premul),
        "Creates SkImageInfo from integral dimensions width and height, "
        "kN32_SkColorType, kPremul_SkAlphaType, with optional SkColorSpace.",
        py::arg("width"), py::arg("height"),
        py::arg_v("cs", sk_sp<SkColorSpace>(nullptr), "skia.ColorSpace()"))
    .def_static("MakeN32Premul",
        py::overload_cast<SkISize, sk_sp<SkColorSpace>>(
            &SkImageInfo::MakeN32Premul),
        "Creates SkImageInfo from integral dimensions width and height, "
        "kN32_SkColorType, kPremul_SkAlphaType, with SkColorSpace set to "
        "nullptr.",
        py::arg("dimensions"),
        py::arg_v("cs", sk_sp<SkColorSpace>(nullptr), "skia.ColorSpace()"))
    .def_static("MakeA8", py::overload_cast<int, int>(&SkImageInfo::MakeA8),
        "Creates SkImageInfo from integral dimensions width and height, "
        "kAlpha_8_SkColorType, kPremul_SkAlphaType, with SkColorSpace set to "
        "nullptr.")
    .def_static("MakeA8", py::overload_cast<SkISize>(&SkImageInfo::MakeA8),
        "Creates SkImageInfo from integral dimensions, kAlpha_8_SkColorType, "
        "kPremul_SkAlphaType, with SkColorSpace set to nullptr.")
    .def_static("MakeUnknown",
        py::overload_cast<int, int>(&SkImageInfo::MakeUnknown),
        "Creates SkImageInfo from integral dimensions width and height, "
        "kUnknown_SkColorType, kUnknown_SkAlphaType, with SkColorSpace set to "
        "nullptr.")
    .def_static("MakeUnknown",
        py::overload_cast<>(&SkImageInfo::MakeUnknown),
        "Creates SkImageInfo from integral dimensions width and height set to "
        "zero, kUnknown_SkColorType, kUnknown_SkAlphaType, with SkColorSpace "
        "set to nullptr.")
    .def_static("ByteSizeOverflowed", &SkImageInfo::ByteSizeOverflowed,
        "Returns true if byteSize equals SIZE_MAX.")
    ;

m.def("AlphaTypeIsOpaque", &SkAlphaTypeIsOpaque);
m.def("ColorTypeBytesPerPixel", &SkColorTypeBytesPerPixel);
m.def("ColorTypeIsAlwaysOpaque", &SkColorTypeIsAlwaysOpaque);
m.def("ColorTypeValidateAlphaType", &SkColorTypeValidateAlphaType);
}
#pragma optimize("", on)
