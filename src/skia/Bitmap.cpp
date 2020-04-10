#include "common.h"

void initBitmap(py::module &m) {
py::class_<SkBitmap>(m, "Bitmap", py::buffer_protocol(), R"docstring(
        SkBitmap describes a two-dimensional raster pixel array.

        SkBitmap is built on SkImageInfo, containing integer width and height,
        SkColorType and SkAlphaType describing the pixel format, and
        SkColorSpace describing the range of colors. SkBitmap points to
        SkPixelRef, which describes the physical array of pixels. SkImageInfo
        bounds may be located anywhere fully inside SkPixelRef bounds.

        SkBitmap can be drawn using SkCanvas. SkBitmap can be a drawing
        destination for SkCanvas draw member functions. SkBitmap flexibility
        as a pixel container limits some optimizations available to the target
        platform.

        If pixel array is primarily read-only, use SkImage for better
        performance. If pixel array is primarily written to, use SkSurface for
        better performance.

        Declaring SkBitmap const prevents altering SkImageInfo: the SkBitmap
        height, width, and so on cannot change. It does not affect SkPixelRef:
        a caller may write its pixels. Declaring SkBitmap const affects
        SkBitmap configuration, not its contents.

        SkBitmap is not thread safe. Each thread must have its own copy of
        SkBitmap fields, although threads may share the underlying pixel array.
    )docstring")
    .def_buffer([](SkBitmap &d) -> py::buffer_info {
        return py::buffer_info(
            const_cast<void*>(d.getPixels()),
            d.bytesPerPixel(),
            (d.bytesPerPixel() == 8) ? "B" :
            (d.bytesPerPixel() == 16) ? "H" :
            (d.bytesPerPixel() == 32) ? "I" :
            (d.bytesPerPixel() == 64) ? "Q" : "B",
            2,
            { ssize_t(d.width()), ssize_t(d.height()) },  // Transposed.
            { ssize_t(d.rowBytes()), ssize_t(d.bytesPerPixel()) }
        );
    })
    .def(py::init<>(),
        "Creates an empty SkBitmap without pixels, with kUnknown_SkColorType, "
        "kUnknown_SkAlphaType, and with a width and height of zero.")
    .def(py::init<const SkBitmap&>(),
        "Copies settings from src to returned SkBitmap.")
    .def("swap", &SkBitmap::swap, "Swaps the fields of the two bitmaps.")
    .def("pixmap", &SkBitmap::pixmap,
        "Returns a constant reference to the SkPixmap holding the SkBitmap "
        "pixel address, row bytes, and SkImageInfo.")
    .def("info", &SkBitmap::info,
        "Returns width, height, SkAlphaType, SkColorType, and SkColorSpace.")
    .def("width", &SkBitmap::width,
        "Returns pixel count in each row.")
    .def("height", &SkBitmap::height,
        "Returns pixel row count.")
    .def("colorType", &SkBitmap::colorType)
    .def("alphaType", &SkBitmap::alphaType)
    .def("colorSpace", &SkBitmap::colorSpace,
        "Returns SkColorSpace, the range of colors, associated with "
        "SkImageInfo.",
        py::return_value_policy::reference)
    .def("refColorSpace", &SkBitmap::refColorSpace,
        "Returns smart pointer to SkColorSpace, the range of colors, associated"
        " with SkImageInfo.")
    .def("bytesPerPixel", &SkBitmap::bytesPerPixel,
        "Returns number of bytes per pixel required by SkColorType.")
    .def("rowBytesAsPixels", &SkBitmap::rowBytesAsPixels,
        "Returns number of pixels that fit on row.")
    .def("shiftPerPixel", &SkBitmap::shiftPerPixel,
        "Returns bit shift converting row bytes to row pixels.")
    .def("empty", &SkBitmap::empty,
        "Returns true if either width() or height() are zero.")
    .def("isNull", &SkBitmap::isNull,
        "Returns true if SkPixelRef is nullptr.")
    .def("drawsNothing", &SkBitmap::drawsNothing,
        "Returns true if width() or height() are zero, or if SkPixelRef is "
        "nullptr.")
    .def("rowBytes", &SkBitmap::rowBytes,
        "Returns row bytes, the interval from one pixel row to the next.")
    .def("setAlphaType", &SkBitmap::setAlphaType,
        "Sets SkAlphaType, if alphaType is compatible with SkColorType.")
    .def("getPixels", &SkBitmap::getPixels,
        "Returns pixel address, the base address corresponding to the pixel "
        "origin.",
        py::return_value_policy::reference)
    .def("computeByteSize", &SkBitmap::computeByteSize,
        "Returns minimum memory required for pixel storage.")
    .def("isImmutable", &SkBitmap::isImmutable,
        "Returns true if pixels can not change.")
    .def("setImmutable", &SkBitmap::setImmutable,
        "Sets internal flag to mark SkBitmap as immutable.")
    .def("isOpaque", &SkBitmap::isOpaque,
        "Returns true if SkAlphaType is set to hint that all pixels are opaque;"
        " their alpha value is implicitly or explicitly 1.0.")
    .def("isVolatile", &SkBitmap::isVolatile,
        "Provides a hint to caller that pixels should not be cached.")
    .def("setIsVolatile", &SkBitmap::setIsVolatile,
        "Sets if pixels should be read from SkPixelRef on every access.")
    .def("reset", &SkBitmap::reset,
        "Resets to its initial state; all fields are set to zero, as if "
        "SkBitmap had been initialized by SkBitmap().")
    .def("getBounds",
        (void (SkBitmap::*)(SkRect *bounds) const) &SkBitmap::getBounds,
        "Returns SkRect { 0, 0, width(), height() }.")
    .def("getBounds",
        (void (SkBitmap::*)(SkIRect *bounds) const) &SkBitmap::getBounds,
        "Returns SkIRect { 0, 0, width(), height() }.")
    .def("bounds", &SkBitmap::bounds,
        "Returns SkIRect { 0, 0, width(), height() }.")
    .def("dimensions", &SkBitmap::dimensions,
        "Returns SkISize { width(), height() }.")
    .def("getSubset", &SkBitmap::getSubset,
        "Returns the bounds of this bitmap, offset by its SkPixelRef origin.")
    .def("setInfo", &SkBitmap::setInfo,
        "Sets width, height, SkAlphaType, SkColorType, SkColorSpace, and "
        "optional rowBytes.")
    .def("tryAllocPixelsFlags", &SkBitmap::tryAllocPixelsFlags,
        "Sets SkImageInfo to info following the rules in setInfo() and "
        "allocates pixel memory.")
    .def("allocPixelsFlags", &SkBitmap::allocPixelsFlags,
        "Sets SkImageInfo to info following the rules in setInfo() and "
        "allocates pixel memory.")
    .def("tryAllocPixels",
        (bool (SkBitmap::*)(const SkImageInfo&, size_t))
        &SkBitmap::tryAllocPixels,
        "Sets SkImageInfo to info following the rules in setInfo() and "
        "allocates pixel memory.")
    .def("allocPixels",
        (void (SkBitmap::*)(const SkImageInfo&, size_t))
        &SkBitmap::allocPixels,
        "Sets SkImageInfo to info following the rules in setInfo() and "
        "allocates pixel memory.")
    .def("tryAllocPixels",
        (bool (SkBitmap::*)(const SkImageInfo&))
        &SkBitmap::tryAllocPixels,
        "Sets SkImageInfo to info following the rules in setInfo() and "
        "allocates pixel memory.")
    .def("allocPixels",
        (void (SkBitmap::*)(const SkImageInfo&))
        &SkBitmap::allocPixels,
        "Sets SkImageInfo to info following the rules in setInfo() and "
        "allocates pixel memory.")
    .def("tryAllocN32Pixels", &SkBitmap::tryAllocN32Pixels,
        "Sets SkImageInfo to info following the rules in setInfo() and "
        "allocates pixel memory.")
    .def("allocN32Pixels", &SkBitmap::allocN32Pixels,
        "Sets SkImageInfo to info following the rules in setInfo() and "
        "allocates pixel memory.")
    .def("installPixels",
        (bool (SkBitmap::*)(const SkImageInfo&, void*, size_t, void(*)(
            void *addr, void *context), void*)) &SkBitmap::installPixels,
        "Sets SkImageInfo to info following the rules in setInfo(), and creates"
        " SkPixelRef containing pixels and rowBytes.")
    .def("installPixels",
        (bool (SkBitmap::*)(const SkImageInfo&, void*, size_t))
        &SkBitmap::installPixels,
        "Sets SkImageInfo to info following the rules in setInfo(), and creates"
        " SkPixelRef containing pixels and rowBytes.")
    .def("installPixels",
        (bool (SkBitmap::*)(const SkPixmap&)) &SkBitmap::installPixels,
        "Sets SkImageInfo to pixmap.info() following the rules in setInfo(), "
        "and creates SkPixelRef containing pixmap.addr() and "
        "pixmap.rowBytes().")
    // .def("installMaskPixels", &SkBitmap::installMaskPixels, "Deprecated.")
    .def("setPixels", &SkBitmap::setPixels,
        "Replaces SkPixelRef with pixels, preserving SkImageInfo and "
        "rowBytes().")
    .def("tryAllocPixels", (bool (SkBitmap::*)()) &SkBitmap::tryAllocPixels,
        "Allocates pixel memory with HeapAllocator, and replaces existing "
        "SkPixelRef.")
    .def("allocPixels", (void (SkBitmap::*)()) &SkBitmap::allocPixels,
        "Allocates pixel memory with HeapAllocator, and replaces existing "
        "SkPixelRef.")
    // .def("tryAllocPixels",
    //     (bool (SkBitmap::*)(Allocator*)) &SkBitmap::tryAllocPixels,
    //     "Allocates pixel memory with allocator, and replaces existing "
    //     "SkPixelRef.")
    // .def("allocPixels",
    //     (void (SkBitmap::*)(Allocator*)) &SkBitmap::allocPixels,
    //     "Allocates pixel memory with allocator, and replaces existing "
    //     "SkPixelRef.")
    // .def("pixelRef", &SkBitmap::pixelRef,
    //     "Returns SkPixelRef, which contains: pixel base address; its "
    //     "dimensions; and rowBytes(), the interval from one row to the next."
    //     py::return_value_policy::reference)
    .def("pixelRefOrigin", &SkBitmap::pixelRefOrigin,
        "Returns origin of pixels within SkPixelRef.")
    // .def("setPixelRef", &SkBitmap::setPixelRef,
    //     "Replaces pixelRef and origin in SkBitmap.")
    .def("readyToDraw", &SkBitmap::readyToDraw,
        "Returns true if SkBitmap is can be drawn.")
    .def("getGenerationID", &SkBitmap::getGenerationID,
        "Returns a unique value corresponding to the pixels in SkPixelRef.")
    .def("notifyPixelsChanged", &SkBitmap::notifyPixelsChanged,
        "Marks that pixels in SkPixelRef have changed.")
    .def("eraseColor", &SkBitmap::eraseColor, "Replaces pixel values with c.")
    .def("eraseARGB", &SkBitmap::eraseARGB,
        "Replaces pixel values with unpremultiplied color built from a, r, g, "
        "and b.")
    .def("erase", &SkBitmap::erase, "Replaces pixel values inside area with c.")
    .def("eraseArea", &SkBitmap::eraseArea, "Deprecated")
    .def("getColor", &SkBitmap::getColor,
        "Returns pixel at (x, y) as unpremultiplied color.")
    .def("getAlphaf", &SkBitmap::getAlphaf,
        "Look up the pixel at (x,y) and return its alpha component, normalized "
        "to [0..1].")
    .def("getAddr", &SkBitmap::getAddr, "Returns pixel address at (x, y).",
        py::return_value_policy::reference)
    .def("getAddr32", &SkBitmap::getAddr32, "Returns address at (x, y).",
        py::return_value_policy::reference)
    .def("getAddr16", &SkBitmap::getAddr16, "Returns address at (x, y).",
        py::return_value_policy::reference)
    .def("getAddr8", &SkBitmap::getAddr8, "Returns address at (x, y).",
        py::return_value_policy::reference)
    .def("extractSubset", &SkBitmap::extractSubset,
        "Shares SkPixelRef with dst.")
    .def("readPixels",
        (bool (SkBitmap::*)(const SkImageInfo&, void*, size_t, int, int) const)
        &SkBitmap::readPixels,
        "Copies a SkRect of pixels from SkBitmap to dstPixels.")
    .def("readPixels",
        (bool (SkBitmap::*)(const SkPixmap&, int, int) const)
        &SkBitmap::readPixels,
        "Copies a SkRect of pixels from SkBitmap to dst.")
    .def("readPixels",
        (bool (SkBitmap::*)(const SkPixmap&) const) &SkBitmap::readPixels,
        "Copies a SkRect of pixels from SkBitmap to dst.")
    .def("writePixels",
        (bool (SkBitmap::*)(const SkPixmap&, int, int))
        &SkBitmap::writePixels,
        "Copies a SkRect of pixels from src.")
    .def("writePixels",
        (bool (SkBitmap::*)(const SkPixmap&)) &SkBitmap::writePixels,
        "Copies a SkRect of pixels from src.")
    .def("extractAlpha",
        (bool (SkBitmap::*)(SkBitmap *dst) const) &SkBitmap::extractAlpha,
        "Sets dst to alpha described by pixels.")
    .def("extractAlpha",
        (bool (SkBitmap::*)(SkBitmap *dst, const SkPaint*, SkIPoint*) const)
        &SkBitmap::extractAlpha,
        "Sets dst to alpha described by pixels.")
    // .def("extractAlpha",
    //     (bool (SkBitmap::*)(SkBitmap *dst, const SkPaint*, Allocator*,
    //         SkIPoint*)) &SkBitmap::extractAlpha,
    //     "Sets dst to alpha described by pixels.")
    .def("peekPixels", &SkBitmap::peekPixels,
        "Copies SkBitmap pixel address, row bytes, and SkImageInfo to pixmap, "
        "if address is available, and returns true.")
    // .def("makeShader",
    //     (sk_sp<SkShader> (SkBitmap::*)(SkTileMode, SkTileMode, const
    //         SkMatrix*) const) &SkBitmap::makeShader)
    // .def("makeShader",
    //     (sk_sp<SkShader> (SkBitmap::*)(const SkMatrix*) const)
    //     &SkBitmap::makeShader)
    ;

m.def("ComputeIsOpaque", &SkBitmap::ComputeIsOpaque,
    "Returns true if all pixels are opaque.");
}
