#include "common.h"
#include <pybind11/stl.h>

const int SkYUVAInfo::kMaxPlanes;

void initImageInfo(py::module &m) {
py::enum_<SkAlphaType>(m, "AlphaType")
    .value("kUnknown_AlphaType", SkAlphaType::kUnknown_SkAlphaType,
        "uninitialized")
    .value("kOpaque_AlphaType", SkAlphaType::kOpaque_SkAlphaType,
        "pixel is opaque")
    .value("kPremul_AlphaType", SkAlphaType::kPremul_SkAlphaType,
        "pixel components are premultiplied by alpha")
    .value("kUnpremul_AlphaType", SkAlphaType::kUnpremul_SkAlphaType,
        "pixel components are independent of alpha")
    .value("kLastEnum_AlphaType", SkAlphaType::kLastEnum_SkAlphaType,
        "last valid value")
    .export_values();

py::enum_<SkColorType>(m, "ColorType")
    .value("kUnknown_ColorType", SkColorType::kUnknown_SkColorType,
        "uninitialized")
    .value("kAlpha_8_ColorType", SkColorType::kAlpha_8_SkColorType,
        "pixel with alpha in 8-bit byte")
    .value("kRGB_565_ColorType", SkColorType::kRGB_565_SkColorType,
        "pixel with 5 bits red, 6 bits green, 5 bits blue, in 16-bit word")
    .value("kARGB_4444_ColorType", SkColorType::kARGB_4444_SkColorType,
        "pixel with 4 bits for alpha, red, green, blue; in 16-bit word")
    .value("kRGBA_8888_ColorType", SkColorType::kRGBA_8888_SkColorType,
        "pixel with 8 bits for red, green, blue, alpha; in 32-bit word")
    .value("kRGB_888x_ColorType", SkColorType::kRGB_888x_SkColorType,
        "pixel with 8 bits each for red, green, blue; in 32-bit word")
    .value("kBGRA_8888_ColorType", SkColorType::kBGRA_8888_SkColorType,
        "pixel with 8 bits for blue, green, red, alpha; in 32-bit word")
    .value("kRGBA_1010102_ColorType", SkColorType::kRGBA_1010102_SkColorType,
        "10 bits for red, green, blue; 2 bits for alpha; in 32-bit word")
    .value("kBGRA_1010102_ColorType", SkColorType::kBGRA_1010102_SkColorType,
        "10 bits for blue, green, red; 2 bits for alpha; in 32-bit word")
    .value("kRGB_101010x_ColorType", SkColorType::kRGB_101010x_SkColorType,
        "pixel with 10 bits each for red, green, blue; in 32-bit word")
    .value("kBGR_101010x_ColorType", SkColorType::kBGR_101010x_SkColorType,
        "pixel with 10 bits each for blue, green, red; in 32-bit word")
    .value("kGray_8_ColorType", SkColorType::kGray_8_SkColorType,
        "pixel with grayscale level in 8-bit byte")
    .value("kRGBA_F16Norm_ColorType", SkColorType::kRGBA_F16Norm_SkColorType,
        "pixel with half floats in [0,1] for red, green, blue, alpha; in "
        "64-bit word")
    .value("kRGBA_F16_ColorType", SkColorType::kRGBA_F16_SkColorType,
        "pixel with half floats for red, green, blue, alpha; in 64-bit word")
    .value("kRGBA_F32_ColorType", SkColorType::kRGBA_F32_SkColorType,
        "pixel using C float for red, green, blue, alpha; in 128-bit word")
    .value("kR8G8_unorm_ColorType", SkColorType::kR8G8_unorm_SkColorType)
    .value("kA16_float_ColorType", SkColorType::kA16_float_SkColorType)
    .value("kR16G16_float_ColorType", SkColorType::kR16G16_float_SkColorType)
    .value("kA16_unorm_ColorType", SkColorType::kA16_unorm_SkColorType)
    .value("kR16G16_unorm_ColorType",
        SkColorType::kR16G16_unorm_SkColorType,
        "pixel with a little endian uint16_t for red and green")
    .value("kR16G16B16A16_unorm_ColorType",
        SkColorType::kR16G16B16A16_unorm_SkColorType,
        "pixel with a little endian uint16_t for red, green, blue")
    .value("kN32_ColorType", SkColorType::kN32_SkColorType)
    .export_values();

py::enum_<SkYUVColorSpace>(m, "YUVColorSpace")
    .value("kJPEG_YUVColorSpace",
        SkYUVColorSpace::kJPEG_SkYUVColorSpace,
        "describes full range")
    .value("kRec601_YUVColorSpace",
        SkYUVColorSpace::kRec601_SkYUVColorSpace,
        "describes SDTV range")
    .value("kRec709_YUVColorSpace",
        SkYUVColorSpace::kRec709_SkYUVColorSpace,
        "describes HDTV range")
    .value("kBT2020_YUVColorSpace",
        SkYUVColorSpace::kBT2020_SkYUVColorSpace,
        "describes UHDTV range, non-constant-luminance")
    .value("kIdentity_YUVColorSpace",
        SkYUVColorSpace::kIdentity_SkYUVColorSpace,
        "maps Y->R, U->G, V->B")
    .value("kLastEnum_YUVColorSpace",
        SkYUVColorSpace::kLastEnum_SkYUVColorSpace,
        "last valid value")
    .export_values();

py::class_<SkColorInfo>(m, "ColorInfo",
    R"docstring(
    Describes pixel and encoding.

    :py:class:`ImageInfo` can be created from :py:class:`ColorInfo` by providing
    dimensions.

    It encodes how pixel bits describe alpha, transparency; color components
    red, blue, and green; and :py:class:`ColorSpace`, the range and linearity of
    colors.
    )docstring")
    .def("__repr__",
        [] (const SkImageInfo& info) {
            return py::str("ImageInfo({}, {}, {}, {})").format(
                info.width(), info.height(), info.colorType(),
                info.alphaType());
        })
    .def(py::init<>(),
        R"docstring(
        Creates an :py:class:`ColorInfo` with
        :py:attr:`~ColorType.kUnknown_ColorType`,
        :py:attr:`~AlphaType.kUnknown_AlphaType`, and no :py:class:`ColorSpace`.
        )docstring")
    .def(py::init<>(
        [] (SkColorType ct, SkAlphaType at, const SkColorSpace* cs) {
            return SkColorInfo(ct, at, CloneColorSpace(cs));
        }),
        R"docstring(
        Creates :py:class:`ColorInfo` from :py:class:`ColorType` ct,
        :py:class:`AlphaType` at, and optionally :py:class:`ColorSpace` cs.

        If :py:class:`ColorSpace` cs is nullptr and :py:class:`ColorInfo` is
        part of drawing source: :py:class:`ColorSpace` defaults to sRGB, mapping
        into :py:class:`Surface` :py:class:`ColorSpace`.

        Parameters are not validated to see if their values are legal, or that
        the combination is supported.
        )docstring",
        py::arg("ct"), py::arg("at"), py::arg("cs") = nullptr)
    .def(py::init<const SkColorInfo&>())
    // .def(py::init<SkColorInfo&&>())
    .def("colorSpace", &SkColorInfo::colorSpace,
        py::return_value_policy::reference_internal)
    .def("refColorSpace", &SkColorInfo::refColorSpace)
    .def("colorType", &SkColorInfo::colorType)
    .def("alphaType", &SkColorInfo::alphaType)
    .def("isOpaque", &SkColorInfo::isOpaque)
    .def("gammaCloseToSRGB", &SkColorInfo::gammaCloseToSRGB)
    .def("__eq__", &SkColorInfo::operator==,
        R"docstring(
        Does other represent the same color type, alpha type, and color space?
        )docstring",
        py::is_operator())
    .def("__ne__", &SkColorInfo::operator!=,
        R"docstring(
        Does other represent a different color type, alpha type, or color space?
        )docstring",
        py::is_operator())
    .def("makeAlphaType", &SkColorInfo::makeAlphaType,
        R"docstring(
        Creates :py:class:`ColorInfo` with same :py:class:`ColorType`,
        :py:class:`ColorSpace`, with :py:class:`AlphaType` set to newAlphaType.

        Created :py:class:`ColorInfo` contains newAlphaType even if it is
        incompatible with :py:class:`ColorType`, in which case
        :py:class:`AlphaType` in :py:class:`ColorInfo` is ignored.
        )docstring",
        py::arg("newAlphaType"))
    .def("makeColorType", &SkColorInfo::makeColorType,
        R"docstring(
        Creates new :py:class:`ColorInfo` with same :py:class:`AlphaType`,
        :py:class:`ColorSpace`, with :py:class:`ColorType` set to newColorType.
        )docstring",
        py::arg("newColorType"))
    .def("makeColorSpace",
        [] (const SkColorInfo& info, const SkColorSpace* cs) {
            return info.makeColorSpace(CloneColorSpace(cs));
        },
        R"docstring(
        Creates :py:class:`ColorInfo` with same :py:class:`AlphaType`,
        :py:class:`ColorType`, with :py:class:`ColorSpace` set to cs.

        cs may be nullptr.
        )docstring",
        py::arg("cs"))
    .def("bytesPerPixel", &SkColorInfo::bytesPerPixel,
        R"docstring(
        Returns number of bytes per pixel required by :py:class:`ColorType`.

        Returns zero if :py:meth:`colorType` is
        :py:attr:`~ColorType.kUnknown_ColorType`.

        :return: bytes in pixel
        )docstring")
    .def("shiftPerPixel", &SkColorInfo::shiftPerPixel,
        R"docstring(
        Returns bit shift converting row bytes to row pixels.

        Returns zero for :py:attr:`ColorType.kUnknown_ColorType`.

        :return: one of: 0, 1, 2, 3, 4; left shift to convert pixels to bytes
        )docstring")
    ;

py::class_<SkImageInfo>(m, "ImageInfo",
    R"docstring(
    Describes pixel dimensions and encoding.

    :py:class:`Bitmap`, :py:class:`Image`, :py:class:`Pixmap`, and
    :py:class:`Surface` can be created from :py:class:`ImageInfo`.
    :py:class:`ImageInfo` can be retrieved from :py:class:`Bitmap` and
    :py:class:`Pixmap`, but not from :py:class:`Image` and :py:class:`Surface`.
    For example, :py:class:`Image` and :py:class:`Surface` implementations may
    defer pixel depth, so may not completely specify :py:class:`ImageInfo`.

    :py:class:`ImageInfo` contains dimensions, the pixel integral width and
    height. It encodes how pixel bits describe alpha, transparency; color
    components red, blue, and green; and :py:class:`ColorSpace`, the range and
    linearity of colors.
    )docstring")
    .def(py::init<>(),
        R"docstring(
        Creates an empty :py:class:`ImageInfo` with
        :py:attr:`~skia.ColorType.kUnknown_ColorType`,
        :py:attr:`~skia.AlphaType.kUnknown_AlphaType`, a width and height of
        zero, and no :py:class:`ColorSpace`.
        )docstring")
    .def("width", &SkImageInfo::width,
        R"docstring(
        Returns pixel count in each row.

        :return: pixel width
        )docstring")
    .def("height", &SkImageInfo::height,
        R"docstring(
        Returns pixel row count.

        :return: pixel height
        )docstring")
    .def("colorType", &SkImageInfo::colorType)
    .def("alphaType", &SkImageInfo::alphaType)
    .def("colorSpace", &SkImageInfo::colorSpace,
        R"docstring(
        Returns :py:class:`ColorSpace`, the range of colors.

        The reference count of :py:class:`ColorSpace` is unchanged. The returned
        :py:class:`ColorSpace` is immutable.

        :return: :py:class:`ColorSpace`, or nullptr
        )docstring",
        py::return_value_policy::reference_internal)
    .def("refColorSpace", &SkImageInfo::refColorSpace,
        R"docstring(
        Returns smart pointer to :py:class:`ColorSpace`, the range of colors.

        The smart pointer tracks the number of objects sharing this
        :py:class:`ColorSpace` reference so the memory is released when the
        owners destruct.

        The returned :py:class:`ColorSpace` is immutable.

        :return: :py:class:`ColorSpace` wrapped in a smart pointer
        )docstring")
    .def("isEmpty", &SkImageInfo::isEmpty,
        R"docstring(
        Returns if :py:class:`ImageInfo` describes an empty area of pixels by
        checking if either width or height is zero or smaller.

        :return: true if either dimension is zero or smaller
        )docstring")
    .def("colorInfo", &SkImageInfo::colorInfo,
        R"docstring(
        Returns the dimensionless :py:class:`ColorInfo` that represents the same
        color type, alpha type, and color space as this :py:class:`ImageInfo`.
        )docstring")
    .def("isOpaque", &SkImageInfo::isOpaque,
        R"docstring(
        Returns true if :py:class:`AlphaType` is set to hint that all pixels are
        opaque; their alpha value is implicitly or explicitly 1.0.

        If true, and all pixels are not opaque, Skia may draw incorrectly.

        Does not check if :py:class:`ColorType` allows alpha, or if any pixel
        value has transparency.

        :return: true if :py:class:`AlphaType` is
            :py:attr:`~skia.AlphaType.kOpaque_AlphaType`
        )docstring")
    .def("dimensions", &SkImageInfo::dimensions,
        R"docstring(
        Returns :py:class:`ISize` (:py:meth:`width`, :py:meth:`height`).

        :return: integral size of :py:meth:`width` and :py:meth:`height`
        )docstring")
    .def("bounds", &SkImageInfo::bounds,
        R"docstring(
        Returns :py:class:`IRect` (0, 0, :py:meth:`width`, :py:meth:`height`).

        :return: integral rectangle from origin to :py:meth:`width` and
            :py:meth:`height`
        )docstring")
    .def("gammaCloseToSRGB", &SkImageInfo::gammaCloseToSRGB,
        R"docstring(
        Returns true if associated :py:class:`ColorSpace` is not nullptr, and
        :py:class:`ColorSpace` gamma is approximately the same as sRGB.

        :return: true if :py:class:`ColorSpace` gamma is approximately the same
            as sRGB
        )docstring")
    .def("makeWH", &SkImageInfo::makeWH,
        R"docstring(
        Creates :py:class:`ImageInfo` with the same :py:class:`ColorType`,
        :py:class:`ColorSpace`, and :py:class:`AlphaType`, with dimensions set
        to width and height.

        :param int newWidth:  pixel column count; must be zero or greater
        :param int newHeight: pixel row count; must be zero or greater
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("newWidth"), py::arg("newHeight"))
    .def("makeDimensions", &SkImageInfo::makeDimensions,
        R"docstring(
        Creates :py:class:`ImageInfo` with the same :py:class:`ColorType`,
        :py:class:`ColorSpace`, and :py:class:`AlphaType`, with dimensions set
        to newDimensions.

        :param skia.ISize newSize: pixel column and row count; must be zero or
            greater
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("newSize"))
    .def("makeAlphaType", &SkImageInfo::makeAlphaType,
        R"docstring(
        Creates :py:class:`ImageInfo` with same :py:class:`ColorType`,
        :py:class:`ColorSpace`, width, and height, with :py:class:`AlphaType`
        set to newAlphaType.

        Created :py:class:`ImageInfo` contains newAlphaType even if it is
        incompatible with :py:class:`ColorType`, in which case
        :py:class:`AlphaType` in :py:class:`ImageInfo` is ignored.

        :param skia.AlphaType newAlphaType: new alpha type
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("newAlphaType"))
    .def("makeColorType", &SkImageInfo::makeColorType,
        R"docstring(
        Creates :py:class:`ImageInfo` with same :py:class:`AlphaType`,
        :py:class:`ColorSpace`, width, and height, with :py:class:`ColorType`
        set to newColorType.

        :param skia.ColorType newColorType: new color type
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("newColorType"))
    .def("makeColorSpace",
        [] (const SkImageInfo& info, const SkColorSpace* cs) {
            return info.makeColorSpace(CloneColorSpace(cs));
        },
        R"docstring(
        Creates :py:class:`ImageInfo` with same :py:class:`AlphaType`,
        :py:class:`ColorType`, width, and height, with :py:class:`ColorSpace`
        set to cs.

        :param skia.ColorSpace cs: range of colors; may be nullptr
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("cs"))
    .def("bytesPerPixel", &SkImageInfo::bytesPerPixel,
        R"docstring(
        Returns number of bytes per pixel required by :py:class:`ColorType`.

        Returns zero if :py:meth:`colorType` is
        :py:attr:`~skia.ColorType.kUnknown_ColorType`.

        :return: bytes in pixel
        )docstring")
    .def("shiftPerPixel", &SkImageInfo::shiftPerPixel,
        R"docstring(
        Returns bit shift converting row bytes to row pixels.

        Returns zero for :py:attr:`skia.ColorType.kUnknown_ColorType`.

        :return: one of: 0, 1, 2, 3; left shift to convert pixels to bytes
        )docstring")
    .def("minRowBytes64", &SkImageInfo::minRowBytes64,
        R"docstring(
        Returns minimum bytes per row, computed from pixel :py:meth:`width` and
        :py:class:`ColorType`, which specifies :py:meth:`bytesPerPixel`.

        :py:class:`Bitmap` maximum value for row bytes must fit in 31 bits.

        :return: :py:meth:`width` times :py:meth:`bytesPerPixel` as unsigned
            64-bit integer
        )docstring")
    .def("minRowBytes", &SkImageInfo::minRowBytes,
        R"docstring(
        Returns minimum bytes per row, computed from pixel :py:meth:`width` and
        :py:class:`ColorType`, which specifies :py:meth:`bytesPerPixel`.

        :py:class:`Bitmap` maximum value for row bytes must fit in 31 bits.

        :return: :py:meth:`width` times :py:meth:`bytesPerPixel` as signed
            32-bit integer
        )docstring")
    .def("computeOffset", &SkImageInfo::computeOffset,
        R"docstring(
        Returns byte offset of pixel from pixel base address.

        Asserts in debug build if x or y is outside of bounds. Does not assert
        if rowBytes is smaller than :py:meth:`minRowBytes`, even though result
        may be incorrect.

        :param int x: column index, zero or greater, and less than
            :py:meth:`width`
        :param int y: row index, zero or greater, and less than
            :py:meth:`height`
        :param int rowBytes: size of pixel row or larger
        :return: offset within pixel array
        )docstring",
        py::arg("x"), py::arg("y"), py::arg("rowBytes"))
    .def("__eq__", &SkImageInfo::operator==,
        R"docstring(
        Compares :py:class:`ImageInfo` with other, and returns true if width,
        height, :py:class:`ColorType`, :py:class:`AlphaType`, and
        :py:class:`ColorSpace` are equivalent.

        :param other: :py:class:`ImageInfo` to compare
        :return: true if :py:class:`ImageInfo` equals other
        )docstring",
        py::is_operator())
    .def("__ne__", &SkImageInfo::operator!=,
        R"docstring(
        Compares :py:class:`ImageInfo` with other, and returns true if width,
        height, :py:class:`ColorType`, :py:class:`AlphaType`, and
        :py:class:`ColorSpace` are not equivalent.

        :param other: :py:class:`ImageInfo` to compare
        :return: true if :py:class:`ImageInfo` is not equal to other
        )docstring",
        py::is_operator())
    .def("computeByteSize", &SkImageInfo::computeByteSize,
        R"docstring(
        Returns storage required by pixel array, given :py:class:`ImageInfo`
        dimensions, :py:class:`ColorType`, and rowBytes.

        rowBytes is assumed to be at least as large as :py:meth:`minRowBytes`.

        Returns zero if height is zero. Returns SIZE_MAX if answer exceeds the
        range of size_t.

        :param int rowBytes: size of pixel row or larger
        :return: memory required by pixel buffer
        )docstring",
        py::arg("rowBytes"))
    .def("computeMinByteSize", &SkImageInfo::computeMinByteSize,
        R"docstring(
        Returns storage required by pixel array, given :py:class:`ImageInfo`
        dimensions, and :py:class:`ColorType`.

        Uses :py:meth:`minRowBytes` to compute bytes for pixel row.

        Returns zero if height is zero. Returns SIZE_MAX if answer exceeds the
        range of size_t.

        :return: least memory required by pixel buffer
        )docstring")
    .def("validRowBytes", &SkImageInfo::validRowBytes,
        R"docstring(
        Returns true if rowBytes is valid for this :py:class:`ImageInfo`.

        :param int rowBytes: size of pixel row including padding
        :return: true if rowBytes is large enough to contain pixel row and is
            properly aligned
        )docstring",
        py::arg("rowBytes"))
    .def("reset", &SkImageInfo::reset,
        R"docstring(
        Creates an empty :py:class:`ImageInfo` with
        :py:attr:`~skia.ColorType.kUnknown_ColorType`,
        :py:attr:`~skia.AlphaType.kUnknown_AlphaType`, a width and height of
        zero, and no :py:class:`ColorSpace`.
        )docstring")
    .def_static("Make",
        [] (int width, int height, SkColorType ct, SkAlphaType at,
            const SkColorSpace* cs) {
            return SkImageInfo::Make(
                width, height, ct, at, CloneColorSpace(cs));
        },
        R"docstring(
        Creates :py:class:`ImageInfo` from integral dimensions width and height,
        :py:class:`ColorType` ct, :py:class:`AlphaType` at, and optionally
        :py:class:`ColorSpace` cs.

        If :py:class:`ColorSpace` cs is nullptr and :py:class:`ImageInfo` is
        part of drawing source: :py:class:`ColorSpace` defaults to sRGB, mapping
        into :py:class:`Surface` :py:class:`ColorSpace`.

        Parameters are not validated to see if their values are legal, or that
        the combination is supported.

        :width:   pixel column count; must be zero or greater
        :height:  pixel row count; must be zero or greater
        :cs:  range of colors; may be nullptr
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("ct"), py::arg("at"),
        py::arg("cs") = nullptr)
    .def_static("Make",
        [] (SkISize dimensions, SkColorType ct, SkAlphaType at,
            const SkColorSpace* cs) {
            return SkImageInfo::Make(dimensions, ct, at, CloneColorSpace(cs));
        },
        py::arg("dimensions"), py::arg("ct"), py::arg("at"),
        py::arg("cs") = nullptr)
    .def_static("Make",
        py::overload_cast<SkISize, const SkColorInfo&>(&SkImageInfo::Make),
        R"docstring(
        Creates :py:class:`ImageInfo` from integral dimensions and
        :py:class:`ColorInfo` colorInfo,.

        Parameters are not validated to see if their values are legal, or that
        the combination is supported.

        :param skia.ISize dimensions: pixel column and row count; must be zeros
            or greater
        :param skia.ColorInfo colorInfo: :py:class:`ColorInfo` the pixel
            encoding consisting of :py:class:`ColorType`, :py:class:`AlphaType`,
            and :py:class:`ColorSpace` (which may be nullptr)
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("dimensions"), py::arg("colorInfo"))
    .def_static("MakeN32",
        [] (int width, int height, SkAlphaType at, const SkColorSpace* cs) {
            return SkImageInfo::MakeN32(width, height, at, CloneColorSpace(cs));
        },
        R"docstring(
        Creates :py:class:`ImageInfo` from integral dimensions width and height,
        :py:attr:`~skia.ColorType.kN32_ColorType`, :py:class:`AlphaType` at, and
        optionally :py:class:`ColorSpace` cs.

        :py:attr:`~skia.ColorType.kN32_ColorType` will equal either
        :py:attr:`~skia.ColorType.kBGRA_8888_ColorType` or
        :py:attr:`~skia.ColorType.kRGBA_8888_ColorType`, whichever is optimal.

        If :py:class:`ColorSpace` cs is nullptr and :py:class:`ImageInfo` is
        part of drawing source: :py:class:`ColorSpace` defaults to sRGB, mapping
        into :py:class:`Surface` :py:class:`ColorSpace`.

        Parameters are not validated to see if their values are legal, or that
        the combination is supported.

        :param int width:  pixel column count; must be zero or greater
        :param int height: pixel row count; must be zero or greater
        :param skia.ColorSpace cs: range of colors; may be nullptr
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("at"),
        py::arg("cs") = nullptr)
    .def_static("MakeS32", &SkImageInfo::MakeS32,
        R"docstring(
        Creates :py:class:`ImageInfo` from integral dimensions width and height,
        :py:attr:`~skia.ColorType.kN32_ColorType`, :py:class:`AlphaType` at,
        with sRGB :py:class:`ColorSpace`.

        Parameters are not validated to see if their values are legal, or that
        the combination is supported.

        :param int width:  pixel column count; must be zero or greater
        :param int height: pixel row count; must be zero or greater
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("at"))
    .def_static("MakeN32Premul",
        [] (int width, int height, const SkColorSpace* cs) {
            return SkImageInfo::MakeN32Premul(
                width, height, CloneColorSpace(cs));
        },
        R"docstring(
        Creates :py:class:`ImageInfo` from integral dimensions width and height,
        :py:attr:`~skia.ColorType.kN32_ColorType`,
        :py:attr:`~skia.AlphaType.kPremul_AlphaType`, with optional
        :py:class:`ColorSpace`.

        If :py:class:`ColorSpace` cs is nullptr and :py:class:`ImageInfo` is
        part of drawing source: :py:class:`ColorSpace` defaults to sRGB, mapping
        into :py:class:`Surface` :py:class:`ColorSpace`.

        Parameters are not validated to see if their values are legal, or that
        the combination is supported.

        :width:  pixel column count; must be zero or greater
        :height: pixel row count; must be zero or greater
        :cs: range of colors; may be nullptr
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("cs") = nullptr)
    .def_static("MakeN32Premul",
        [] (const SkISize& dimensions, const SkColorSpace* cs) {
            return SkImageInfo::MakeN32Premul(dimensions, CloneColorSpace(cs));
        },
        R"docstring(
        Creates :py:class:`ImageInfo` from integral dimensions width and height,
        :py:attr:`~skia.ColorType.kN32_ColorType`,
        :py:attr:`~skia.AlphaType.kPremul_AlphaType`, with optional
        :py:class:`ColorSpace`.

        If :py:class:`ColorSpace` cs is nullptr and :py:class:`ImageInfo` is
        part of drawing source: :py:class:`ColorSpace` defaults to sRGB, mapping
        into :py:class:`Surface` :py:class:`ColorSpace`.

        Parameters are not validated to see if their values are legal, or that
        the combination is supported.

        :dimensions: width and height, each must be zero or greater
        :cs: range of colors; may be nullptr
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("dimensions"), py::arg("cs") = nullptr)
    .def_static("MakeA8", py::overload_cast<int, int>(&SkImageInfo::MakeA8),
        R"docstring(
        Creates :py:class:`ImageInfo` from integral dimensions width and height,
        :py:attr:`~skia.ColorType.kAlpha_8_ColorType`,
        :py:attr:`~skia.AlphaType.kPremul_AlphaType`, with
        :py:class:`ColorSpace` set to nullptr.

        :width:  pixel column count; must be zero or greater
        :height: pixel row count; must be zero or greater
        )docstring",
        py::arg("width"), py::arg("height"))
    .def_static("MakeA8", py::overload_cast<SkISize>(&SkImageInfo::MakeA8),
        R"docstring(
        Creates :py:class:`ImageInfo` from integral dimensions,
        :py:attr:`~skia.ColorType.kAlpha_8_ColorType`,
        :py:attr:`~skia.AlphaType.kPremul_AlphaType`, with
        :py:class:`ColorSpace` set to nullptr.

        :dimensions: pixel row and column count; must be zero or greater
        )docstring",
        py::arg("dimensions"))
    .def_static("MakeUnknown",
        py::overload_cast<int, int>(&SkImageInfo::MakeUnknown),
        R"docstring(
        Creates :py:class:`ImageInfo` from integral dimensions width and height,
        :py:attr:`~skia.ColorType.kUnknown_ColorType`,
        :py:attr:`~skia.AlphaType.kUnknown_AlphaType`, with
        :py:class:`ColorSpace` set to nullptr.

        Returned :py:class:`ImageInfo` as part of source does not draw, and as
        part of destination can not be drawn to.

        :width:  pixel column count; must be zero or greater
        :height: pixel row count; must be zero or greater
        :return: created :py:class:`ImageInfo`
        )docstring",
        py::arg("width"), py::arg("height"))
    .def_static("MakeUnknown",
        py::overload_cast<>(&SkImageInfo::MakeUnknown),
        R"docstring(
        Creates :py:class:`ImageInfo` from integral dimensions width and height
        set to zero, :py:attr:`~skia.ColorType.kUnknown_ColorType`,
        :py:attr:`~skia.AlphaType.kUnknown_AlphaType`, with
        :py:class:`ColorSpace` set to nullptr.

        Returned :py:class:`ImageInfo` as part of source does not draw, and as
        part of destination can not be drawn to.

        :return: created :py:class:`ImageInfo`
        )docstring")
    .def_static("ByteSizeOverflowed", &SkImageInfo::ByteSizeOverflowed,
        R"docstring(
        Returns true if byteSize equals SIZE_MAX.

        :py:meth:`computeByteSize` and :py:meth:`computeMinByteSize` return
        SIZE_MAX if size_t can not hold buffer size.

        :param int byteSize: result of :py:meth:`computeByteSize` or
            :py:meth:`computeMinByteSize`
        :return: true if :py:meth:`computeByteSize` or
            :py:meth:`computeMinByteSize` result exceeds size_t
        )docstring",
        py::arg("byteSize"))
    ;

m.def("AlphaTypeIsOpaque", &SkAlphaTypeIsOpaque,
    R"docstring(
    Returns true if :py:class:`AlphaType` equals
    :py:attr:`~skia.AlphaType.kOpaque_AlphaType`.

    :py:attr:`~skia.AlphaType.kOpaque_AlphaType` is a hint that the
    :py:class:`ColorType` is opaque, or that all alpha values are set to their
    1.0 equivalent. If :py:class:`AlphaType` is
    :py:attr:`~skia.AlphaType.kOpaque_AlphaType`, and :py:class:`ColorType` is
    not opaque, then the result of drawing any pixel with a alpha value less
    than 1.0 is undefined.
    )docstring",
    py::arg("at"));
m.def("ColorTypeBytesPerPixel", &SkColorTypeBytesPerPixel,
    R"docstring(
    Returns the number of bytes required to store a pixel, including unused
    padding.

    Returns zero if ct is :py:attr:`~skia.Type.kUnknown_ColorType` or invalid.

    :return: bytes per pixel
    )docstring",
    py::arg("ct"));
m.def("ColorTypeIsAlwaysOpaque", &SkColorTypeIsAlwaysOpaque,
    R"docstring(
    Returns true if :py:class:`ColorType` always decodes alpha to 1.0, making
    the pixel fully opaque.

    If true, :py:class:`ColorType` does not reserve bits to encode alpha.

    :return: true if alpha is always set to 1.0
    )docstring",
    py::arg("ct"));
m.def("ColorTypeValidateAlphaType", &SkColorTypeValidateAlphaType,
    R"docstring(
    Returns true if canonical can be set to a valid :py:class:`AlphaType` for
    colorType.

    If there is more than one valid canonical :py:class:`AlphaType`, set to
    alphaType, if valid. If true is returned and canonical is not nullptr, store
    valid :py:class:`AlphaType`.

    Returns false only if alphaType is :py:attr:`~skia.Type.kUnknown_AlphaType`,
    color type is not :py:attr:`~skia.ColorType.kUnknown_ColorType`, and
    :py:class:`ColorType` is not always opaque. If false is returned, canonical
    is ignored.

    :param colorType: color type
    :param alphaType: alpha type
    :param canonical: output storage for :py:class:`AlphaType`
    :return: true if valid :py:class:`AlphaType` can be associated with
        colorType
    )docstring",
    py::arg("colorType"), py::arg("alphaType"), py::arg("canonical") = nullptr);


py::class_<SkYUVAInfo> yuvainfo(m, "YUVAInfo",
    R"docstring(
    Specifies the structure of planes for a YUV image with optional alpha. The
    actual planar data is not part of this structure and depending on usage is
    in external textures or pixmaps.
    )docstring");

py::enum_<SkYUVAInfo::PlaneConfig>(yuvainfo, "PlaneConfig",
    R"docstring(
    Specifies how YUV (and optionally A) are divided among planes. Planes are separated by
    underscores in the enum value names. Within each plane the pixmap/texture channels are
    mapped to the YUVA channels in the order specified, e.g. for kY_UV Y is in channel 0 of plane
    0, U is in channel 0 of plane 1, and V is in channel 1 of plane 1. Channel ordering
    within a pixmap/texture given the channels it contains:
    A:                       0:A
    Luminance/Gray:          0:Gray
    Luminance/Gray + Alpha:  0:Gray, 1:A
    RG                       0:R,    1:G
    RGB                      0:R,    1:G, 2:B
    RGBA                     0:R,    1:G, 2:B, 3:A
    )docstring")
    .value("kUnknown", SkYUVAInfo::PlaneConfig::kUnknown,
        "")
    //
    .value("kY_U_V", SkYUVAInfo::PlaneConfig::kY_U_V,
        "Plane 0: Y, Plane 1: U,  Plane 2: V")
    .value("kY_V_U", SkYUVAInfo::PlaneConfig::kY_V_U,
        "< Plane 0: Y, Plane 1: V,  Plane 2: U")
    .value("kY_UV", SkYUVAInfo::PlaneConfig::kY_UV,
        "Plane 0: Y, Plane 1: UV")
    .value("kY_VU", SkYUVAInfo::PlaneConfig::kY_VU,
        "Plane 0: Y, Plane 1: VU")
    .value("kYUV", SkYUVAInfo::PlaneConfig::kYUV,
        "Plane 0: YUV")
    .value("kUYV", SkYUVAInfo::PlaneConfig::kUYV,
        "Plane 0: UYV")
    //
    .value("kY_U_V_A", SkYUVAInfo::PlaneConfig::kY_U_V_A,
        "Plane 0: Y, Plane 1: U,  Plane 2: V, Plane 3: A")
    .value("kY_V_U_A", SkYUVAInfo::PlaneConfig::kY_V_U_A,
        "Plane 0: Y, Plane 1: V,  Plane 2: U, Plane 3: A")
    .value("kY_UV_A", SkYUVAInfo::PlaneConfig::kY_UV_A,
        "Plane 0: Y, Plane 1: UV, Plane 2: A")
    .value("kY_VU_A", SkYUVAInfo::PlaneConfig::kY_VU_A,
        "Plane 0: Y, Plane 1: VU, Plane 2: A")
    .value("kYUVA", SkYUVAInfo::PlaneConfig::kYUVA,
        "Plane 0: YUVA")
    .value("kUYVA", SkYUVAInfo::PlaneConfig::kUYVA,
        "Plane 0: UYVA")
    .export_values();


py::enum_<SkYUVAInfo::Subsampling>(yuvainfo, "Subsampling",
    R"docstring(
    UV subsampling is also specified in the enum value names using J:a:b notation (e.g. 4:2:0 is
    1/2 horizontal and 1/2 vertical resolution for U and V). If alpha is present it is not sub-
    sampled. Note that Subsampling values other than k444 are only valid with PlaneConfig values
    that have U and V in different planes than Y (and A, if present).
    )docstring")
    .value("kUnknown", SkYUVAInfo::Subsampling::kUnknown)
    //
    .value("k444", SkYUVAInfo::Subsampling::k444)
    .value("k422", SkYUVAInfo::Subsampling::k422)
    .value("k420", SkYUVAInfo::Subsampling::k420)
    .value("k440", SkYUVAInfo::Subsampling::k440)
    .value("k411", SkYUVAInfo::Subsampling::k411)
    .value("k410", SkYUVAInfo::Subsampling::k410)
    .export_values();


py::enum_<SkYUVAInfo::Siting>(yuvainfo, "Siting",
    R"docstring(
    Describes how subsampled chroma values are sited relative to luma values.

    Currently only centered siting is supported but will expand to support
    additional sitings.
    )docstring")
    .value("kCentered", SkYUVAInfo::Siting::kCentered,
        R"docstring(
        Subsampled chroma value is sited at the center of the block of
        corresponding luma values.
        )docstring")
    .export_values();

yuvainfo
    .def_readonly_static("kMaxPlanes", &SkYUVAInfo::kMaxPlanes)
    .def_static("PlaneDimensions",
            [] (SkISize imageDimensions, SkYUVAInfo::PlaneConfig config, SkYUVAInfo::Subsampling sampling,
            SkEncodedOrigin origin) {
            std::vector<SkISize> planeDimensions(SkYUVAInfo::kMaxPlanes);
            auto size = SkYUVAInfo::PlaneDimensions(
                imageDimensions, config, sampling, origin, planeDimensions.data());
            planeDimensions.erase(
                planeDimensions.begin() + size, planeDimensions.end());
            return planeDimensions;
        },
        R"docstring(
        Given image dimensions, a planar configuration, and origin, determine
        the expected size of each plane. Returns a list of expected
        planes.

        The input image dimensions are as displayed (after the planes have
        been transformed to the intended display orientation).
        )docstring",
        py::arg("imageDimensions"), py::arg("config"), py::arg("sampling"), py::arg("origin"))
    .def_static("NumPlanes", &SkYUVAInfo::NumPlanes,
        R"docstring(
        Number of planes for a given PlaneConfig.
        )docstring",
        py::arg("config"))
    .def_static("NumChannelsInPlane", &SkYUVAInfo::NumChannelsInPlane,
        R"docstring(
        Number of Y, U, V, A channels in the ith plane for a given PlaneConfig
        (or 0 if i is invalid).
        )docstring",
        py::arg("config"), py::arg("i"))
    .def_static("HasAlpha", &SkYUVAInfo::HasAlpha,
        R"docstring(
        Does the PlaneConfig have alpha values?
        )docstring",
        py::arg("config"))
    .def(py::init<>())
    .def(py::init<SkISize, SkYUVAInfo::PlaneConfig, SkYUVAInfo::Subsampling, SkYUVColorSpace,
        SkEncodedOrigin, SkYUVAInfo::Siting, SkYUVAInfo::Siting>(),
        R"docstring(
        'dimensions' should specify the size of the full resolution image (after
        planes have been oriented to how the image is displayed as indicated by
        'origin').
        )docstring",
        py::arg("dimensions"), py::arg("config"), py::arg("sampling"), py::arg("yuvColorSpace"),
        py::arg_v("origin", kTopLeft_SkEncodedOrigin, "skia.EncodedOrigin.kTopLeft_EncodedOrigin"),
        py::arg_v("sittingX", SkYUVAInfo::Siting::kCentered, "skia.SkYUVAInfo.Siting.kCentered"),
        py::arg_v("sittingY", SkYUVAInfo::Siting::kCentered, "skia.SkYUVAInfo.Siting.kCentered"))
    .def("planeConfig", &SkYUVAInfo::planeConfig)
    .def("subSampling", &SkYUVAInfo::subsampling)
    .def("dimensions", &SkYUVAInfo::dimensions,
        R"docstring(
        Dimensions of the full resolution image (after planes have been oriented
        to how the image is displayed as indicated by fOrigin).
        )docstring")
    .def("width", &SkYUVAInfo::width)
    .def("height", &SkYUVAInfo::height)
    .def("yuvColorSpace", &SkYUVAInfo::yuvColorSpace)
    .def("sitingX", &SkYUVAInfo::sitingX)
    .def("sitingY", &SkYUVAInfo::sitingY)
    .def("origin", &SkYUVAInfo::origin)
    .def("hasAlpha", &SkYUVAInfo::hasAlpha)
    .def("planeDimensions",
        [] (const SkYUVAInfo& info) {
            std::vector<SkISize> planeDimensions(SkYUVAInfo::kMaxPlanes);
            info.planeDimensions(planeDimensions.data());
            return planeDimensions;
        },
        R"docstring(
        Returns the number of planes and initializes
        planeDimensions[0]..planeDimensions[<ret>] to the expected dimensions
        for each plane. Dimensions are as stored in memory, before
        transformation to image display space as indicated by origin().
        )docstring")
    .def("computeTotalBytes",
        [] (const SkYUVAInfo& info, const std::vector<size_t>& rowBytes,
            bool returnPlaneSizes) -> py::object {
            if (rowBytes.size() < SkYUVAInfo::kMaxPlanes)
                throw py::value_error(
                    py::str("rowBytes must have {} elements.").format(
                        SkYUVAInfo::kMaxPlanes));
            std::vector<size_t> planeSizes(SkYUVAInfo::kMaxPlanes);
            auto size = info.computeTotalBytes(
                rowBytes.data(), planeSizes.data());
            if (returnPlaneSizes)
                return py::make_tuple(size, planeSizes);
            return py::cast(size);
        },
        R"docstring(
        Given a per-plane row bytes, determine size to allocate for all
        planes. Optionally retrieves the per-plane byte sizes if
        returnPlaneSizes is True. If total size overflows will return SIZE_MAX
        and set all planeSizes to SIZE_MAX.
        )docstring",
        py::arg("rowBytes"), py::arg("returnPlaneSizes") = false)
    .def("numPlanes", &SkYUVAInfo::numPlanes)
    .def("numChannelsInPlane", &SkYUVAInfo::numChannelsInPlane, py::arg("i"))
    .def("__eq__",
        [] (const SkYUVAInfo& info, const SkYUVAInfo& that) {
            return info == that;
        },
        py::is_operator())
    .def("__ne__",
        [] (const SkYUVAInfo& info, const SkYUVAInfo& that) {
            return info != that;
        },
        py::is_operator())
    ;
}
