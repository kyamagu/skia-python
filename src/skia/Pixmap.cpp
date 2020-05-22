#include "common.h"

template <typename T, bool readonly = true>
py::memoryview AddrN(const SkPixmap& pixmap) {
    if (pixmap.info().bytesPerPixel() != sizeof(T))
        throw std::runtime_error("Incompatible byte size.");
    return py::memoryview(
        py::buffer_info(
            reinterpret_cast<T*>(pixmap.writable_addr()),
            sizeof(T),
            py::format_descriptor<T>::format(),
            2,
            { pixmap.rowBytesAsPixels(), pixmap.height() },
            { pixmap.rowBytes(), sizeof(T) },
            readonly
        )
    );
}

template <bool readonly = true>
py::memoryview Addr(const SkPixmap& pixmap) {
    ssize_t bytesPerPixel = pixmap.info().bytesPerPixel();
    const std::string& format =
        (bytesPerPixel == 1) ? py::format_descriptor<uint8_t>::format() :
        (bytesPerPixel == 2) ? py::format_descriptor<uint16_t>::format() :
        (bytesPerPixel == 4) ? py::format_descriptor<uint32_t>::format() :
        (bytesPerPixel == 8) ? py::format_descriptor<uint64_t>::format() :
        py::format_descriptor<uint8_t>::format();
    return py::memoryview(
        py::buffer_info(
            pixmap.writable_addr(),
            bytesPerPixel,
            format,
            2,
            { pixmap.rowBytesAsPixels(), pixmap.height() },
            { ssize_t(pixmap.rowBytes()), bytesPerPixel },
            readonly
        )
    );
}

void initPixmap(py::module &m) {
py::class_<SkPixmap>(m, "Pixmap", R"docstring(
    :py:class:`Pixmap` provides a utility to pair :py:class:`ImageInfo` with
    pixels and row bytes.

    :py:class:`Pixmap` is a low level class which provides convenience functions
    to access raster destinations. :py:class:`Canvas` can not draw
    :py:class:`Pixmap`, nor does :py:class:`Pixmap` provide a direct drawing
    destination.

    Use :py:class:`Bitmap` to draw pixels referenced by :py:class:`Pixmap`; use
    :py:class:`Surface` to draw into pixels referenced by :py:class:`Pixmap`.

    :py:class:`Pixmap` does not try to manage the lifetime of the pixel memory.
    Use :py:class:`PixelRef` to manage pixel memory; :py:class:`PixelRef` is
    safe across threads.
    )docstring")
    .def(py::init<>(),
        R"docstring(
        Creates an empty :py:class:`Pixmap` without pixels, with
        :py:attr:`~ColorType.kUnknown_ColorType`, with
        :py:attr:`~AlphaType.kUnknown_AlphaType`, and with a width and height of
        zero.

        Use :py:meth:`reset` to associate pixels, :py:class:`ColorType`,
        :py:class:`AlphaType`, width, and height after :py:class:`Pixmap` has
        been created.

        :return: empty :py:class:`Pixmap`
        )docstring")
    .def(py::init(
        [] (const SkImageInfo& info, py::object data, size_t rowBytes) {
            if (data.is_none())
                return SkPixmap(info, nullptr, rowBytes);
            auto buffer = data.cast<py::buffer>().request();
            size_t size = (buffer.ndim) ?
                buffer.shape[0] * buffer.strides[0] : 0;
            if (size < info.computeByteSize(rowBytes))
                throw std::runtime_error(
                    "Buffer size is smaller than required.");
            return SkPixmap(info, buffer.ptr, rowBytes);
        }),
        R"docstring(
        Creates :py:class:`Pixmap` from info width, height,
        :py:class:`AlphaType`, and :py:class:`ColorType`.

        ``data`` points to pixels, or nullptr. rowBytes should be
        ``info.width()`` times ``info.bytesPerPixel()``, or larger.

        No parameter checking is performed; it is up to the caller to ensure
        that data and rowBytes agree with info.

        The memory lifetime of pixels is managed by the caller. When
        :py:class:`Pixmap` goes out of scope, data is unaffected.

        :py:class:`Pixmap` may be later modified by :py:meth:`reset` to change
        its size, pixel type, or storage.

        :info: width, height, :py:class:`AlphaType`,
            :py:class:`ColorType` of :py:class:`ImageInfo`
        :data: pointer to pixels
            allocated by caller; may be nullptr
        :rowBytes: size of one row of data; width times pixel size, or
            larger
        )docstring",
        py::arg("info").none(false), py::arg("data"), py::arg("rowBytes"))
    .def("reset",
        py::overload_cast<>(&SkPixmap::reset),
        R"docstring(
        Sets width, height, row bytes to zero; pixel address to nullptr;
        :py:class:`ColorType` to :py:attr:`~ColorType.kUnknown_ColorType`; and
        :py:class:`AlphaType` to :py:attr:`~Type.kUnknown_AlphaType`.

        The prior pixels are unaffected; it is up to the caller to release
        pixels memory if desired.
        )docstring")
    .def("reset",
        [] (SkPixmap& pixmap, const SkImageInfo& info, py::object data,
            size_t rowBytes) {
            if (data.is_none()) {
                pixmap.reset(info, nullptr, rowBytes);
                return;
            }
            auto buffer = data.cast<py::buffer>().request();
            size_t size = (buffer.ndim) ?
                buffer.shape[0] * buffer.strides[0] : 0;
            if (size < info.computeByteSize(rowBytes))
                throw std::runtime_error(
                    "Buffer size is smaller than required.");
            pixmap.reset(info, buffer.ptr, rowBytes);
        },
        R"docstring(
        Sets width, height, :py:class:`AlphaType`, and :py:class:`ColorType`
        from info.

        Sets pixel address from data, which may be nullptr. Sets row bytes from
        rowBytes, which should be ``info.width()`` times
        ``info.bytesPerPixel()``, or larger.

        Does not check data. Asserts if built with SK_DEBUG defined and if
        rowBytes is too small to hold one row of pixels.

        The memory lifetime pixels are managed by the caller. When
        :py:class:`Pixmap` goes out of scope, data is unaffected.

        :param skia.ImageInfo info: width, height, :py:class:`AlphaType`,
            :py:class:`ColorType` of :py:class:`ImageInfo`
        :param Union[bytes,bytearray,memoryview] data: pointer to pixels
            allocated by caller; may be nullptr
        :param int rowBytes: size of one row of data; width times pixel size, or
            larger
        )docstring",
        py::arg("info"), py::arg("data"), py::arg("rowBytes"))
    .def("setColorSpace",
        [] (SkPixmap& pixmap, const SkColorSpace* colorSpace) {
            pixmap.setColorSpace(CloneColorSpace(colorSpace));
        },
        R"docstring(
        Changes :py:class:`ColorSpace` in :py:class:`ImageInfo`; preserves
        width, height, :py:class:`AlphaType`, and :py:class:`ColorType` in
        :py:class:`Image`, and leaves pixel address and row bytes unchanged.

        :py:class:`ColorSpace` reference count is incremented.

        :param skia.ColorSpace colorSpace: :py:class:`ColorSpace` moved to
            :py:class:`ImageInfo`
        )docstring",
        py::arg("colorSpace"))
    .def("extractSubset", &SkPixmap::extractSubset,
        R"docstring(
        Sets subset width, height, pixel address to intersection of
        :py:class:`Pixmap` with area, if intersection is not empty; and return
        true.

        Otherwise, leave subset unchanged and return false.

        Failing to read the return value generates a compile time warning.

        :param skia.Pixmap subset: storage for width, height, pixel address of
            intersection
        :param skia.IRect area: bounds to intersect with :py:class:`Pixmap`
        :return: true if intersection of :py:class:`Pixmap` and area is not
            empty
        )docstring",
        py::arg("subset").none(false), py::arg("area"))
    .def("info", &SkPixmap::info,
        R"docstring(
        Returns width, height, :py:class:`AlphaType`, :py:class:`ColorType`, and
        :py:class:`ColorSpace`.

        :return: reference to :py:class:`ImageInfo`
        )docstring")
    .def("rowBytes", &SkPixmap::rowBytes,
        R"docstring(
        Returns row bytes, the interval from one pixel row to the next.

        Row bytes is at least as large as: ``width() * info().bytesPerPixel()``.

        Returns zero if :py:meth:`colorType` is
        :py:attr:`~ColorType.kUnknown_ColorType`. It is up to the
        :py:class:`Bitmap` creator to ensure that row bytes is a useful value.

        :return: byte length of pixel row
        )docstring")
    .def("addr", &Addr<true>,
        R"docstring(
        Returns pixel address, the base address corresponding to the pixel
        origin.

        It is up to the :py:class:`Pixmap` creator to ensure that pixel address
        is a useful value.

        :return: pixel address
        :rtype: memoryview
        )docstring")
    .def("width", &SkPixmap::width,
        R"docstring(
        Returns pixel count in each pixel row.

        Should be equal or less than:
        :py:meth:`rowBytes` / ``info().bytesPerPixel()``.

        :return: pixel width in :py:class:`ImageInfo`
        )docstring")
    .def("height", &SkPixmap::height,
        R"docstring(
        Returns pixel row count.

        :return: pixel height in SkImageInfo
        )docstring")
    .def("dimensions", &SkPixmap::dimensions,
        R"docstring(
        Return the dimensions of the pixmap (from its ImageInfo)
        )docstring")
    .def("colorType", &SkPixmap::colorType)
    .def("alphaType", &SkPixmap::alphaType)
    .def("colorSpace", &SkPixmap::colorSpace,
        R"docstring(
        Returns :py:class:`ColorSpace`, the range of colors, associated with
        :py:class:`ImageInfo`.

        The reference count of :py:class:`ColorSpace` is unchanged. The returned
        :py:class:`ColorSpace` is immutable.

        :return: :py:class:`ColorSpace` in :py:class:`ImageInfo`, or nullptr
        )docstring",
        py::return_value_policy::reference)
    .def("refColorSpace", &SkPixmap::refColorSpace,
        R"docstring(
        Returns smart pointer to :py:class:`ColorSpace`, the range of colors,
        associated with :py:class:`ImageInfo`.

        The smart pointer tracks the number of objects sharing this
        :py:class:`ColorSpace` reference so the memory is released when the
        owners destruct.

        The returned :py:class:`ColorSpace` is immutable.

        :return: :py:class:`ColorSpace` in :py:class:`ImageInfo` wrapped in a
            smart pointer
        )docstring")
    .def("isOpaque", &SkPixmap::isOpaque,
        R"docstring(
        Returns true if :py:class:`AlphaType` is :py:attr:`kOpaque_AlphaType`.

        Does not check if :py:class:`ColorType` allows alpha, or if any pixel
        value has transparency.

        :return: true if :py:class:`ImageInfo` has opaque :py:class:`AlphaType`
        )docstring")
    .def("bounds", &SkPixmap::bounds,
        R"docstring(
        Returns :py:class:`IRect` ( 0, 0, :py:meth:`width`, :py:meth:`height` ).

        :return: integral rectangle from origin to :py:meth:`width` and
            :py:meth:`height`
        )docstring")
    .def("rowBytesAsPixels", &SkPixmap::rowBytesAsPixels,
        R"docstring(
        Returns number of pixels that fit on row.

        Should be greater than or equal to :py:meth:`width`.

        :return: maximum pixels per row
        )docstring")
    .def("shiftPerPixel", &SkPixmap::shiftPerPixel,
        R"docstring(
        Returns bit shift converting row bytes to row pixels.

        Returns zero for :py:attr:`~ColorType.kUnknown_ColorType`.

        :return: one of: 0, 1, 2, 3; left shift to convert pixels to bytes
        )docstring")
    .def("computeByteSize", &SkPixmap::computeByteSize,
        R"docstring(
        Returns minimum memory required for pixel storage.

        Does not include unused memory on last row when
        :py:meth:`rowBytesAsPixels` exceeds width(). Returns SIZE_MAX if result
        does not fit in size_t. Returns zero if :py:meth:`height` or
        :py:meth:`width` is 0. Returns :py:meth:`height` times
        :py:meth:`rowBytes` if :py:meth:`colorType` is
        :py:attr:`~ColorType.kUnknown_ColorType`.

        :return: size in bytes of image buffer
        )docstring")
    .def("computeIsOpaque", &SkPixmap::computeIsOpaque,
        R"docstring(
        Returns true if all pixels are opaque.

        :py:class:`ColorType` determines how pixels are encoded, and whether
        pixel describes alpha. Returns true for :py:class:`ColorType` without
        alpha in each pixel; for other :py:class:`ColorType`, returns true if
        all pixels have alpha values equivalent to 1.0 or greater.

        For :py:class:`ColorType` :py:attr:`~ColorType.kRGB_565_ColorType` or
        :py:attr:`~ColorType.kGray_8_ColorType`: always returns true. For
        :py:class:`ColorType` :py:attr:`~ColorType.kAlpha_8_ColorType`,
        :py:attr:`~ColorType.kBGRA_8888_ColorType`,
        :py:attr:`~ColorType.kRGBA_8888_ColorType`: returns true if all pixel
        alpha values are 255. For :py:class:`ColorType`
        :py:attr:`~ColorType.kARGB_4444_ColorType`: returns true if all pixel
        alpha values are 15. For :py:attr:`~ColorType.kRGBA_F16_ColorType`:
        returns true if all pixel alpha values are 1.0 or greater.

        Returns false for :py:attr:`~ColorType.kUnknown_ColorType`.

        :return: true if all pixels have opaque values or :py:class:`ColorType`
            is opaque
        )docstring")
    .def("getColor", &SkPixmap::getColor,
        R"docstring(
        Returns pixel at (x, y) as unpremultiplied color.

        Returns black with alpha if :py:class:`ColorType` is
        :py:attr:`~ColorType.kAlpha_8_ColorType`.

        Input is not validated: out of bounds values of x or y trigger an
        assert() if built with SK_DEBUG defined; and returns undefined values or
        may crash if SK_RELEASE is defined. Fails if :py:class:`ColorType` is
        :py:attr:`~ColorType.kUnknown_ColorType` or pixel address is nullptr.

        :py:class:`ColorSpace` in :py:class:`ImageInfo` is ignored. Some color
        precision may be lost in the conversion to unpremultiplied color;
        original pixel data may have additional precision.

        :param int x: column index, zero or greater, and less than width()
        :param int y: row index, zero or greater, and less than height()
        :return: pixel converted to unpremultiplied color
        )docstring",
        py::arg("x"), py::arg("y"))
    .def("getAlphaf", &SkPixmap::getAlphaf,
        R"docstring(
        Look up the pixel at (x,y) and return its alpha component, normalized to
        [0..1].

        This is roughly equivalent to :py:meth:`~skia.GetColorA`(
        :py:meth:`getColor` ), but can be more efficent (and more precise if the
        pixels store more than 8 bits per component).

        :param int x: column index, zero or greater, and less than width()
        :param int y: row index, zero or greater, and less than height()
        :return: alpha converted to normalized float
        )docstring",
        py::arg("x"), py::arg("y"))
    .def("addr8", &AddrN<uint8_t>,
        R"docstring(
        Returns readable base pixel address.

        Result is addressable as unsigned 8-bit bytes. Will trigger an assert()
        if :py:class:`ColorType` is not :py:attr:`~ColorType.kAlpha_8_ColorType`
        or :py:attr:`~ColorType.kGray_8_ColorType`, and is built with SK_DEBUG
        defined.

        One byte corresponds to one pixel.

        :return: readable unsigned 8-bit pointer to pixels
        :rtype: memoryview
        )docstring")
    .def("addr16",
        &AddrN<uint16_t>,
        R"docstring(
        Returns readable base pixel address.

        Result is addressable as unsigned 16-bit words. Will trigger an assert()
        if :py:class:`ColorType` is not :py:attr:`kRGB_565_ColorType` or
        :py:attr:`kARGB_4444_ColorType`, and is built with SK_DEBUG defined.

        One word corresponds to one pixel.

        :return: readable unsigned 16-bit pointer to pixels
        :rtype: memoryview
        )docstring")
    .def("addr32",
        &AddrN<uint32_t>,
        R"docstring(
        Returns readable base pixel address.

        Result is addressable as unsigned 32-bit words. Will trigger an assert()
        if :py:class:`ColorType` is not :py:attr:`kRGBA_8888_ColorType` or
        :py:attr:`kBGRA_8888_ColorType`, and is built with SK_DEBUG defined.

        One word corresponds to one pixel.

        :return: readable unsigned 32-bit pointer to pixels
        :rtype: memoryview
        )docstring")
    .def("addr64",
        &AddrN<uint64_t>,
        R"docstring(
        Returns readable base pixel address.

        Result is addressable as unsigned 64-bit words. Will trigger an assert()
        if :py:class:`ColorType` is not :py:attr:`kRGBA_F16_ColorType` and is
        built with SK_DEBUG defined.

        One word corresponds to one pixel.

        :return: readable unsigned 64-bit pointer to pixels
        :rtype: memoryview
        )docstring")
    .def("writable_addr",
        &Addr<false>,
        R"docstring(
        Returns writable base pixel address.

        :return: writable generic base pointer to pixels
        :rtype: memoryview
        )docstring")
    .def("readPixels",
        [] (const SkPixmap& pixmap, const SkImageInfo& info, py::buffer dst,
            size_t dstRowBytes, int srcX, int srcY) {
            auto buffer = dst.request();
            size_t given = (buffer.ndim) ?
                buffer.shape[0] * buffer.strides[0] : 0;
            if (given < info.computeByteSize(dstRowBytes))
                throw std::runtime_error("Buffer is smaller than required.");
            return pixmap.readPixels(info, buffer.ptr, dstRowBytes, srcX, srcY);
        },
        R"docstring(
        Copies :py:class:`Rect` of pixels to dstPixels.

        Copy starts at offset (srcX, srcY), and does not exceed
        :py:class:`Pixmap` (:py:meth:`width`, :py:meth:`height`).

        dstInfo specifies width, height, :py:class:`ColorType`,
        :py:class:`AlphaType`, and :py:class:`ColorSpace` of destination.
        dstRowBytes specifies the gap from one destination row to the next.
        Returns true if pixels are copied. Returns false if dstInfo address
        equals nullptr, or dstRowBytes is less than ``dstInfo.minRowBytes()``.

        Pixels are copied only if pixel conversion is possible. If
        :py:class:`Pixmap` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, or
        :py:attr:`~ColorType.kAlpha_8_ColorType`; ``dstInfo.colorType``
        must match. If :py:class:`Pixmap` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, ``dstInfo.colorSpace`` must
        match. If :py:class:`Pixmap` :py:class:`AlphaType` is
        :py:attr:`~AlphaType.kOpaque_AlphaType`, ``dstInfo.alphaType`` must
        match. If :py:class:`Pixmap` :py:class:`ColorSpace` is nullptr,
        ``dstInfo.colorSpace`` must match. Returns false if pixel conversion is
        not possible.

        srcX and srcY may be negative to copy only top or left of source.
        Returns false if :py:meth:`width` or :py:meth:`height` is zero or
        negative. Returns false if abs(srcX) >= Pixmap :py:meth:`width`, or if
        abs(srcY) >= Pixmap :py:meth:`height`.

        :dstInfo: destination width, height, :py:class:`ColorType`,
            :py:class:`AlphaType`, :py:class:`ColorSpace`
        :dstPixels:   destination pixel storage
        :dstRowBytes: destination row length
        :srcX: column index whose absolute value is less than
            :py:meth:`width`
        :srcY: row index whose absolute value is less than
            :py:meth:`height`
        :return: true if pixels are copied to dstPixels
        )docstring",
        py::arg("dstInfo"), py::arg("dstPixels"), py::arg("dstRowBytes"),
        py::arg("srcX") = 0, py::arg("srcY") = 0)
    .def("readPixels",
        py::overload_cast<const SkPixmap&, int, int>(
            &SkPixmap::readPixels, py::const_),
        R"docstring(
        Copies :py:class:`Rect` of pixels to dst.

        Copy starts at offset (srcX, srcY), and does not exceed
        :py:class:`Pixmap` (:py:meth:`width`, :py:meth:`height`).

        dstInfo specifies width, height, :py:class:`ColorType`,
        :py:class:`AlphaType`, and :py:class:`ColorSpace` of destination.
        dstRowBytes specifies the gap from one destination row to the next.
        Returns true if pixels are copied. Returns false if dstInfo address
        equals nullptr, or dstRowBytes is less than ``dstInfo.minRowBytes()``.

        Pixels are copied only if pixel conversion is possible. If
        :py:class:`Pixmap` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, or
        :py:attr:`~ColorType.kAlpha_8_ColorType`; ``dstInfo.colorType``
        must match. If :py:class:`Pixmap` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, ``dstInfo.colorSpace`` must
        match. If :py:class:`Pixmap` :py:class:`AlphaType` is
        :py:attr:`~AlphaType.kOpaque_AlphaType`, ``dstInfo.alphaType`` must
        match. If :py:class:`Pixmap` :py:class:`ColorSpace` is nullptr,
        ``dstInfo.colorSpace`` must match. Returns false if pixel conversion is
        not possible.

        srcX and srcY may be negative to copy only top or left of source.
        Returns false if :py:meth:`width` or :py:meth:`height` is zero or
        negative. Returns false if abs(srcX) >= Pixmap :py:meth:`width`, or if
        abs(srcY) >= Pixmap :py:meth:`height`.

        :dst: :py:class:`ImageInfo` and pixel address to write to
        :srcX: column index whose absolute value is less than
            :py:meth:`width`
        :srcY: row index whose absolute value is less than
            :py:meth:`height`
        :return: true if pixels are copied to dst
        )docstring",
        py::arg("dst"), py::arg("srcX") = 0, py::arg("srcY") = 0)
    .def("scalePixels", &SkPixmap::scalePixels,
        R"docstring(
        Copies :py:class:`Pixmap` to dst, scaling pixels to fit ``dst.width()``
        and ``dst.height()``, and converting pixels to match ``dst.colorType()``
        and ``dst.alphaType()``.

        Returns true if pixels are copied. Returns false if ``dst.addr()`` is
        nullptr, or ``dst.rowBytes()`` is less than dst
        :py:meth:`ImageInfo.minRowBytes`.

        Pixels are copied only if pixel conversion is possible. If
        :py:class:`Pixmap` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, or
        :py:attr:`~ColorType.kAlpha_8_ColorType`; ``dst.colorType()`` must
        match. If :py:class:`Pixmap` :py:class:`ColorType` is
        :py:attr:`~ColorType.kGray_8_ColorType`, ``dst.colorSpace()`` must
        match. If :py:class:`Pixmap` :py:class:`AlphaType` is
        :py:attr:`~AlphaType.kOpaque_AlphaType`, ``dst.alphaType()`` must match.
        If :py:class:`Pixmap` :py:class:`ColorSpace` is nullptr,
        ``dst.colorSpace()`` must match. Returns false if pixel conversion is
        not possible.

        Returns false if :py:class:`Pixmap` :py:meth:`width` or
        :py:meth:`height` is zero or negative.

        Scales the image, with filterQuality, to match ``dst.width()`` and
        ``dst.height()``. filterQuality
        :py:attr:`~FilterQuality.None_FilterQuality` is fastest, typically
        implemented with nearest neighbor filter.
        :py:attr:`~FilterQuality.kLow_FilterQuality` is typically implemented
        with bilerp filter. :py:attr:`~FilterQuality.kMedium_FilterQuality` is
        typically implemented with bilerp filter, and mip-map filter when size
        is reduced. :py:attr:`~FilterQuality.kHigh_FilterQuality` is slowest,
        typically implemented with bicubic filter.

        :param skia.Pixmap dst: destination :py:class:`Pixmap`:
            :py:class:`ImageInfo`, pixels, row bytes
        :param skia.FilterQuality filterQuality: Filter quality
        :return: true if pixels are scaled to fit dst
        )docstring",
        py::arg("dst"),
        py::arg("filterQuality") = SkFilterQuality::kMedium_SkFilterQuality)
    .def("erase",
        py::overload_cast<const SkColor4f&, const SkIRect*>(
            &SkPixmap::erase, py::const_),
        R"docstring(
        Writes color to pixels bounded by subset; returns true on success.

        if subset is nullptr, writes colors pixels inside :py:meth:`bounds`.
        Returns false if :py:meth:`colorType` is
        :py:attr:`~ColorType.kUnknown_ColorType`, if subset is not nullptr and
        does not intersect :py:meth:`bounds`, or if subset is nullptr and
        :py:meth:`bounds` is empty.

        :param color: sRGB unpremultiplied color to write
        :param subset: bounding integer :py:class:`Rect` of pixels to write; may
            be nullptr
        :return: true if pixels are changed
        )docstring",
        py::arg("color"), py::arg("subset") = nullptr)
    ;
}
