#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initPixmap(py::module &m) {
py::class_<SkPixmap>(m, "Pixmap")
    .def(py::init<>(),
        "Creates an empty SkPixmap without pixels, with "
        "kUnknown_SkColorType, with kUnknown_SkAlphaType, and with a "
        "width and height of zero.")
    .def(py::init<const SkImageInfo&, const void*, size_t>(),
        "Creates SkPixmap from info width, height, SkAlphaType, and "
        "SkColorType.")
    .def("reset",
        py::overload_cast<>(&SkPixmap::reset),
        "Sets width, height, row bytes to zero; pixel address to nullptr; "
        "SkColorType to kUnknown_SkColorType; and SkAlphaType to "
        "kUnknown_SkAlphaType.")
    .def("reset",
        py::overload_cast<const SkImageInfo&, const void*, size_t>(
            &SkPixmap::reset),
        "Sets width, height, SkAlphaType, and SkColorType from info.")
    .def("setColorSpace", &SkPixmap::setColorSpace,
        "Changes SkColorSpace in SkImageInfo; preserves width, height, "
        "SkAlphaType, and SkColorType in SkImage, and leaves pixel "
        "address and row bytes unchanged.")
    // .def("reset", py::overload_cast<const SkMask&>(&SkPixmap::reset),
    //     "Deprecated.")
    .def("extractSubset", &SkPixmap::extractSubset,
        "Sets subset width, height, pixel address to intersection of "
        "SkPixmap with area, if intersection is not empty; and return "
        "true.")
    .def("info", &SkPixmap::info,
        "Returns width, height, SkAlphaType, SkColorType, and "
        "SkColorSpace.")
    .def("rowBytes", &SkPixmap::rowBytes,
        "Returns row bytes, the interval from one pixel row to the next.")
    .def("addr", py::overload_cast<>(&SkPixmap::addr, py::const_),
        "Returns pixel address, the base address corresponding to the "
        "pixel origin.")
    .def("width", &SkPixmap::width,
        "Returns pixel count in each pixel row.")
    .def("height", &SkPixmap::height, "Returns pixel row count.")
    .def("dimensions", &SkPixmap::dimensions,
        "Return the dimensions of the pixmap (from its ImageInfo)")
    .def("colorType", &SkPixmap::colorType)
    .def("alphaType", &SkPixmap::alphaType)
    .def("colorSpace", &SkPixmap::colorSpace,
        "Returns SkColorSpace, the range of colors, associated with "
        "SkImageInfo.",
        py::return_value_policy::reference)
    .def("refColorSpace", &SkPixmap::refColorSpace,
        "Returns smart pointer to SkColorSpace, the range of colors, "
        "associated with SkImageInfo.")
    .def("isOpaque", &SkPixmap::isOpaque,
        "Returns true if SkAlphaType is kOpaque_SkAlphaType.")
    .def("bounds", &SkPixmap::bounds,
        "Returns SkIRect { 0, 0, width(), height() }.")
    .def("rowBytesAsPixels", &SkPixmap::rowBytesAsPixels,
        "Returns number of pixels that fit on row.")
    .def("shiftPerPixel", &SkPixmap::shiftPerPixel,
        "Returns bit shift converting row bytes to row pixels.")
    .def("computeByteSize", &SkPixmap::computeByteSize,
        "Returns minimum memory required for pixel storage.")
    .def("computeIsOpaque", &SkPixmap::computeIsOpaque,
        "Returns true if all pixels are opaque.")
    .def("getColor", &SkPixmap::getColor,
        "Returns pixel at (x, y) as unpremultiplied color.")
    .def("getAlphaf", &SkPixmap::getAlphaf,
        "Look up the pixel at (x,y) and return its alpha component, "
        "normalized to [0..1].")
    .def("addr",
        py::overload_cast<int, int>(&SkPixmap::addr, py::const_),
        "Returns readable pixel address at (x, y).")
    .def("addr8",
        py::overload_cast<>(&SkPixmap::addr8, py::const_),
        "Returns readable base pixel address.",
        py::return_value_policy::reference)
    .def("addr16",
        py::overload_cast<>(&SkPixmap::addr16, py::const_),
        "Returns readable base pixel address.",
        py::return_value_policy::reference)
    .def("addr32",
        py::overload_cast<>(&SkPixmap::addr32, py::const_),
        "Returns readable base pixel address.",
        py::return_value_policy::reference)
    .def("addr64",
        py::overload_cast<>(&SkPixmap::addr64, py::const_),
        "Returns readable base pixel address.",
        py::return_value_policy::reference)
    .def("addrF16",
        py::overload_cast<>(&SkPixmap::addrF16, py::const_),
        "Returns readable base pixel address.",
        py::return_value_policy::reference)
    .def("addr8",
        py::overload_cast<int, int>(&SkPixmap::addr8, py::const_),
        "Returns readable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("addr16",
        py::overload_cast<int, int>(&SkPixmap::addr16, py::const_),
        "Returns readable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("addr32",
        py::overload_cast<int, int>(&SkPixmap::addr32, py::const_),
        "Returns readable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("addr64",
        py::overload_cast<int, int>(&SkPixmap::addr64, py::const_),
        "Returns readable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("addrF16",
        py::overload_cast<int, int>(&SkPixmap::addrF16, py::const_),
        "Returns readable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("writable_addr",
        py::overload_cast<>(&SkPixmap::writable_addr, py::const_),
        "Returns writable base pixel address.",
        py::return_value_policy::reference)
    .def("writable_addr",
        py::overload_cast<int, int>(&SkPixmap::writable_addr, py::const_),
        "Returns writable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("writable_addr8",
        py::overload_cast<int, int>(&SkPixmap::writable_addr8, py::const_),
        "Returns writable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("writable_addr16",
        py::overload_cast<int, int>(&SkPixmap::writable_addr16, py::const_),
        "Returns writable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("writable_addr32",
        py::overload_cast<int, int>(&SkPixmap::writable_addr32, py::const_),
        "Returns writable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("writable_addr64",
        py::overload_cast<int, int>(&SkPixmap::writable_addr64, py::const_),
        "Returns writable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("writable_addrF16",
        py::overload_cast<int, int>(
            &SkPixmap::writable_addrF16, py::const_),
        "Returns writable pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("readPixels",
        py::overload_cast<const SkImageInfo&, void*, size_t>(
            &SkPixmap::readPixels, py::const_),
        "Copies a SkRect of pixels to dstPixels.")
    .def("readPixels",
        py::overload_cast<const SkImageInfo&, void*, size_t, int, int>(
            &SkPixmap::readPixels, py::const_),
        "Copies a SkRect of pixels to dstPixels.")
    .def("readPixels",
        py::overload_cast<const SkPixmap&, int, int>(
            &SkPixmap::readPixels, py::const_),
        "Copies a SkRect of pixels to dstPixels.")
    .def("readPixels",
        py::overload_cast<const SkPixmap&>(
            &SkPixmap::readPixels, py::const_),
        "Copies pixels inside bounds() to dst.")
    .def("scalePixels", &SkPixmap::scalePixels,
        "Copies SkBitmap to dst, scaling pixels to fit dst.width() and "
        "dst.height(), and converting pixels to match dst.colorType() "
        "and dst.alphaType().")
    .def("erase",
        py::overload_cast<SkColor, const SkIRect&>(
            &SkPixmap::erase, py::const_),
        "Writes color to pixels bounded by subset; returns true on "
        "success.")
    .def("erase",
        py::overload_cast<SkColor>(&SkPixmap::erase, py::const_),
        "Writes color to pixels inside bounds(); returns true on success.")
    .def("erase",
        py::overload_cast<const SkColor4f&, const SkIRect*>(
            &SkPixmap::erase, py::const_),
        "Writes color to pixels bounded by subset; returns true on "
        "success.")
    ;
}
