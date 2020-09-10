#include "common.h"
#include <algorithm>


void initBitmap(py::module &m) {
py::enum_<SkTileMode>(m, "TileMode")
    .value("kClamp", SkTileMode::kClamp,
        "Replicate the edge color if the shader draws outside of its original "
        "bounds.")
    .value("kRepeat", SkTileMode::kRepeat,
        "Repeat the shader's image horizontally and vertically.")
    .value("kMirror", SkTileMode::kMirror,
        "Repeat the shader's image horizontally and vertically, alternating "
        "mirror images so that adjacent images always seam.")
    .value("kDecal", SkTileMode::kDecal,
        "Only draw within the original domain, return transparent-black "
        "everywhere else.")
    .value("kLastTileMode", SkTileMode::kLastTileMode,
        "")
    .export_values();

py::class_<SkPixelRef>(m, "PixelRef",
    R"docstring(
    This class is the smart container for pixel memory, and is used with
    :py:class:`Bitmap`.

    This class can be shared/accessed between multiple threads.
    )docstring")
    // TODO: Implement me!
    ;

py::class_<SkBitmap> bitmap(m, "Bitmap", py::buffer_protocol(),
    R"docstring(
    :py:class:`Bitmap` describes a two-dimensional raster pixel array.

    :py:class:`Bitmap` is built on :py:class:`ImageInfo`, containing integer
    width and height, :py:class:`ColorType` and :py:class:`AlphaType` describing
    the pixel format, and :py:class:`ColorSpace` describing the range of colors.
    :py:class:`Bitmap` points to :py:class:`PixelRef`, which describes the
    physical array of pixels. :py:class:`ImageInfo` bounds may be located
    anywhere fully inside :py:class:`PixelRef` bounds.

    :py:class:`Bitmap` can be drawn using :py:class:`Canvas`.
    :py:class:`Bitmap` can be a drawing destination for :py:class:`Canvas` draw
    member functions. :py:class:`Bitmap` flexibility as a pixel container limits
    some optimizations available to the target platform.

    If pixel array is primarily read-only, use :py:class:`Image` for better
    performance. If pixel array is primarily written to, use
    :py:class:`Surface` for better performance.

    Declaring :py:class:`Bitmap` const prevents altering
    :py:class:`ImageInfo`: the :py:class:`Bitmap` height, width, and so on
    cannot change. It does not affect :py:class:`PixelRef`: a caller may write
    its pixels. Declaring :py:class:`Bitmap` const affects :py:class:`Bitmap`
    configuration, not its contents.

    :py:class:`Bitmap` is not thread safe. Each thread must have its own copy of
    :py:class:`Bitmap` fields, although threads may share the underlying pixel
    array.

    :py:class:`Bitmap` supports buffer protocol. It is possible to mount
    :py:class:`Bitmap` as array::

        array = np.array(pixmap, copy=False)

    Or mount array as :py:class:`Bitmap` with :py:class:`ImageInfo`::

        buffer = np.zeros((100, 100, 4), np.uint8)
        bitmap = skia.Bitmap()
        bitmap.setInfo(skia.ImageInfo.MakeN32Premul(100, 100))
        bitmap.setPixels(buffer)

    )docstring");

py::enum_<SkBitmap::AllocFlags>(bitmap, "AllocFlags", py::arithmetic())
    .value("kZeroPixels_AllocFlag", SkBitmap::kZeroPixels_AllocFlag,
        R"docstring(
        zero pixel memory. No effect. This is the default.
        )docstring")
    .export_values();

bitmap
    .def_buffer(
        [] (const SkBitmap& bitmap) {
            CHECK_NOTNULL(bitmap.getPixels());
            return ImageInfoToBufferInfo(
                bitmap.info(), bitmap.getPixels(), bitmap.rowBytes(), false);
        })
    .def("__repr__",
        [] (const SkBitmap& bitmap) {
            return py::str("Bitmap({}, {})").format(
                bitmap.width(), bitmap.height(), bitmap.colorType(),
                bitmap.alphaType());
        })
    .def("__len__",
        [] (const SkBitmap& bitmap) {
            return bitmap.width() * bitmap.height();
        })
    .def("__getitem__",
        [] (const SkBitmap& bitmap, py::object object) {
            int x = 0;
            int y = 0;
            if (py::isinstance<py::tuple>(object)) {
                auto t = object.cast<py::tuple>();
                if (t.size() != 2)
                    throw py::index_error("Index must be two dimension.");
                x = t[0].cast<int>();
                y = t[1].cast<int>();
            }
            else {
                int offset = object.cast<int>();
                x = offset % bitmap.width();
                y = offset / bitmap.height();
            }
            if (x < 0 || bitmap.width() <= x ||
                y < 0 || bitmap.height() <= y)
                throw std::out_of_range("Index out of range.");
            return bitmap.getColor(x, y);
        })
    .def_property_readonly("__array_interface__",
        [] (const SkBitmap& bitmap) {
            return ImageInfoToArrayInterface(bitmap.info(), bitmap.rowBytes());
        })
    .def("tobytes",
        [] (const SkBitmap& bitmap) -> py::object {
            return py::module::import("builtins").attr("bytes")(bitmap);
        })
    .def(py::init<>(
        [] (const SkBitmap* src) {
            return (src) ? SkBitmap(*src) : SkBitmap();
        }),
        R"docstring(
        Creates a :py:class:`Bitmap`.

        When ``src`` is given, copies settings from src to returned
        :py:class:`Bitmap`.

        Shares pixels if src has pixels allocated, so both bitmaps reference the
        same pixels.

        When ``src`` is ``None``, creates an empty :py:class:`Bitmap` without
        pixels, with :py:attr:`~ColorType.kUnknown_ColorType`,
        :py:attr:`~AlphaType.kUnknown_AlphaType`, and with a width and height of
        zero.

        :py:class:`PixelRef` origin is set to (0, 0). :py:class:`Bitmap` is not
        volatile.

        Use :py:meth:`setInfo` to associate :py:class:`ColorType`,
        :py:class:`AlphaType`, width, and height after :py:class:`Bitmap` has
        been created.

        :param: src :py:class:`Bitmap` to copy :py:class:`ImageInfo`, and share
            :py:class:`PixelRef`
        )docstring",
        py::arg("src") = nullptr)
    .def("swap", &SkBitmap::swap,
        R"docstring(
        Swaps the fields of the two bitmaps.

        :param skia.Bitmap other: :py:class:`Bitmap` exchanged with original
        )docstring",
        py::arg("other"))
    .def("pixmap", &SkBitmap::pixmap,
        R"docstring(
        Returns a constant reference to the :py:class:`Pixmap` holding the
        :py:class:`Bitmap` pixel address, row bytes, and :py:class:`ImageInfo`.

        :return: reference to :py:class:`Pixmap` describing this
            :py:class:`Bitmap`
        )docstring")
    .def("info", &SkBitmap::info,
        R"docstring(
        Returns width, height, :py:class:`AlphaType`, :py:class:`ColorType`, and
        :py:class:`ColorSpace`.

        :return: reference to :py:class:`ImageInfo`
        )docstring")
    .def("width", &SkBitmap::width,
        R"docstring(
        Returns pixel count in each row.

        Should be equal or less than :py:meth:`rowBytes` / :py:meth:`info` .
        :py:meth:`bytesPerPixel`.

        May be less than :py:meth:`pixelRef` . :py:meth:`width`. Will not exceed
        :py:meth:`pixelRef` . :py:meth:`width` less :py:meth:`pixelRefOrigin` .
        fX.

        :return: pixel width in SkImageInfo
        )docstring")
    .def("height", &SkBitmap::height,
        R"docstring(
        Returns pixel row count.

        Maybe be less than :py:meth:`pixelRef` . :py:meth:`height`. Will not
        exceed :py:meth:`pixelRef` . :py:meth:`height` less
        :py:meth:`pixelRefOrigin` . fY.

        :return: pixel height in :py:class:`ImageInfo`
        )docstring")
    .def("colorType", &SkBitmap::colorType)
    .def("alphaType", &SkBitmap::alphaType)
    .def("colorSpace", &SkBitmap::colorSpace,
        R"docstring(
        Returns :py:class:`ColorSpace`, the range of colors, associated with
        :py:class:`ImageInfo`.

        The reference count of :py:class:`ColorSpace` is unchanged. The returned
        :py:class:`ColorSpace` is immutable.

        :return: :py:class:`ColorSpace` in :py:class:`ImageInfo`, or nullptr
        )docstring",
        py::return_value_policy::reference_internal)
    .def("refColorSpace", &SkBitmap::refColorSpace,
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
    .def("bytesPerPixel", &SkBitmap::bytesPerPixel,
        R"docstring(
        Returns number of bytes per pixel required by :py:class:`ColorType`.

        Returns zero if colorType( is :py:attr:`ColorType.kUnknown_ColorType`.

        :return: bytes in pixel
        )docstring")
    .def("rowBytesAsPixels", &SkBitmap::rowBytesAsPixels,
        R"docstring(
        Returns number of pixels that fit on row.

        Should be greater than or equal to width().

        :return: maximum pixels per row
        )docstring")
    .def("shiftPerPixel", &SkBitmap::shiftPerPixel,
        R"docstring(
        Returns bit shift converting row bytes to row pixels.

        Returns zero for :py:attr:`~ColorType.kUnknown_ColorType`.

        :return: one of: 0, 1, 2, 3; left shift to convert pixels to bytes
        )docstring")
    .def("empty", &SkBitmap::empty,
        R"docstring(
        Returns true if either :py:meth:`width` or :py:meth:`height` are zero.

        Does not check if :py:class:`PixelRef` is nullptr; call
        :py:meth:`drawsNothing` to check :py:meth:`width`, :py:meth:`height`,
        and :py:class:`PixelRef`.

        :return: true if dimensions do not enclose area
        )docstring")
    .def("isNull", &SkBitmap::isNull,
        R"docstring(
        Returns true if :py:class:`PixelRef` is nullptr.

        Does not check if :py:meth:`width` or :py:meth:`height` are zero; call
        :py:meth:`drawsNothing` to check :py:meth:`width`, :py:meth:`height`,
        and :py:class:`PixelRef`.

        :return: true if no :py:class:`PixelRef` is associated
        )docstring")
    .def("drawsNothing", &SkBitmap::drawsNothing,
        R"docstring(
        Returns true if :py:meth:`width` or :py:meth:`height` are zero, or if
        :py:class:`PixelRef` is nullptr.

        If true, :py:class:`Bitmap` has no effect when drawn or drawn into.

        :return: true if drawing has no effect
        )docstring")
    .def("rowBytes", &SkBitmap::rowBytes,
        R"docstring(
        Returns row bytes, the interval from one pixel row to the next.

        Row bytes is at least as large as: :py:meth:`width` * :py:meth:`info` .
        :py:meth:`bytesPerPixel`.

        Returns zero if :py:meth:`colorType` is
        :py:attr:`~ColorType.kUnknown_ColorType`, or if row bytes supplied to
        :py:meth:`setInfo` is not large enough to hold a row of pixels.

        :return: byte length of pixel row
        )docstring")
    .def("setAlphaType", &SkBitmap::setAlphaType,
        R"docstring(
        Sets :py:class:`AlphaType`, if alphaType is compatible with
        :py:class:`ColorType`.

        Returns true unless alphaType is
        :py:attr:`~AlphaType.kUnknown_AlphaType` and current
        :py:class:`AlphaType` is not :py:attr:`~AlphaType.kUnknown_AlphaType`.

        Returns true if :py:class:`ColorType` is
        :py:attr:`~ColorType.kUnknown_ColorType`. alphaType is ignored, and
        :py:class:`AlphaType` remains :py:attr:`~AlphaType.kUnknown_AlphaType`.

        Returns true if :py:class:`ColorType` is
        :py:attr:`~ColorType.kRGB_565_ColorType` or
        :py:attr:`~ColorType.kGray_8_ColorType`. alphaType is ignored, and
        :py:class:`AlphaType` remains :py:attr:`~AlphaType.kOpaque_AlphaType`.

        If :py:class:`ColorType` is :py:attr:`~ColorType.kARGB_4444_ColorType`,
        :py:attr:`~ColorType.kRGBA_8888_ColorType`,
        :py:attr:`~ColorType.kBGRA_8888_ColorType`, or
        :py:attr:`~ColorType.kRGBA_F16_ColorType`: returns true unless alphaType
        is :py:attr:`~AlphaType.kUnknown_AlphaType` and :py:class:`AlphaType` is
        not :py:attr:`~AlphaType.kUnknown_AlphaType`. If :py:class:`AlphaType`
        is :py:attr:`~AlphaType.kUnknown_AlphaType`, alphaType is ignored.

        If :py:class:`ColorType` is :py:attr:`~ColorType.kAlpha_8_ColorType`,
        returns true unless alphaType is
        :py:attr:`~AlphaType.kUnknown_AlphaType` and :py:class:`AlphaType` is
        not :py:attr:`~AlphaType.kUnknown_AlphaType`. If :py:class:`AlphaType`
        is :py:attr:`~AlphaType.kUnknown_AlphaType`, alphaType is ignored. If
        alphaType is :py:attr:`~AlphaType.kUnpremul_AlphaType`, it is treated as
        :py:attr:`~AlphaType.kPremul_AlphaType`.

        This changes :py:class:`AlphaType` in :py:class:`PixelRef`; all bitmaps
        sharing :py:class:`PixelRef` are affected.

        :return: true if :py:class:`AlphaType` is set
        )docstring",
        py::arg("alphaType"))
    .def("getPixels",
        [] (const SkBitmap& bitmap) -> py::object {
            CHECK_NOTNULL(bitmap.getPixels());
            return py::memoryview(ImageInfoToBufferInfo(
                bitmap.info(), bitmap.getPixels(), bitmap.rowBytes(), false));
        },
        R"docstring(
        Returns pixel address, the base address corresponding to the pixel
        origin.

        :return: pixel address
        )docstring")
    .def("computeByteSize", &SkBitmap::computeByteSize,
        R"docstring(
        Returns minimum memory required for pixel storage.

        Does not include unused memory on last row when
        :py:meth:`rowBytesAsPixels` exceeds :py:meth:`width`. Returns SIZE_MAX
        if result does not fit in size_t. Returns zero if :py:meth:`height` or
        :py:meth:`width` is 0. Returns :py:meth:`height` times
        :py:meth:`rowBytes` if :py:meth:`colorType` is
        :py:attr:`~ColorType.kUnknown_ColorType`.

        :return: size in bytes of image buffer
        )docstring")
    .def("isImmutable", &SkBitmap::isImmutable,
        R"docstring(
        Returns true if pixels can not change.

        Most immutable :py:class:`Bitmap` checks trigger an assert only on debug
        builds.

        :return: true if pixels are immutable
        )docstring")
    .def("setImmutable", &SkBitmap::setImmutable,
        R"docstring(
        Sets internal flag to mark :py:class:`Bitmap` as immutable.

        Once set, pixels can not change. Any other bitmap sharing the same
        :py:class:`PixelRef` are also marked as immutable. Once
        :py:class:`PixelRef` is marked immutable, the setting cannot be cleared.

        Writing to immutable :py:class:`Bitmap` pixels triggers an assert on
        debug builds.
        )docstring")
    .def("isOpaque", &SkBitmap::isOpaque,
        R"docstring(
        Returns true if :py:class:`AlphaType` is set to hint that all pixels are
        opaque; their alpha value is implicitly or explicitly 1.0.

        If true, and all pixels are not opaque, Skia may draw incorrectly.

        Does not check if :py:class:`ColorType` allows alpha, or if any pixel
        value has transparency.

        :return: true if :py:class:`ImageInfo` :py:class:`AlphaType` is
            :py:attr:`~AlphaType.kOpaque_AlphaType`
        )docstring")
    .def("reset", &SkBitmap::reset,
        R"docstring(
        Resets to its initial state; all fields are set to zero, as if
        :py:class:`Bitmap` had been initialized by :py:class:`Bitmap`.

        Sets width, height, row bytes to zero; pixel address to nullptr;
        :py:class:`ColorType` to :py:attr:`~ColorType.kUnknown_ColorType`; and
        :py:class:`AlphaType` to :py:attr:`~AlphaType.kUnknown_AlphaType`.

        If :py:class:`PixelRef` is allocated, its reference count is decreased
        by one, releasing its memory if :py:class:`Bitmap` is the sole owner.
        )docstring")
    .def("getBounds",
        [] (const SkBitmap& bitmap) {
            SkIRect bounds;
            bitmap.getBounds(&bounds);
            return bounds;
        },
        R"docstring(
        Returns :py:class:`IRect` { 0, 0, :py:meth:`width`, :py:meth:`height` }.

        :return: integral rectangle
        )docstring")
    .def("bounds", &SkBitmap::bounds,
        R"docstring(
        Returns :py:class:`IRect` { 0, 0, :py:meth:`width`, :py:meth:`height` }.

        :return: integral rectangle from origin to :py:meth:`width` and
            :py:meth:`height`
        )docstring")
    .def("dimensions", &SkBitmap::dimensions,
        R"docstring(
        Returns :py:class:`ISize` { :py:meth:`width`, :py:meth:`height` }.

        :return: integral size of :py:meth:`width` and :py:meth:`height`
        )docstring")
    .def("getSubset", &SkBitmap::getSubset,
        R"docstring(
        Returns the bounds of this bitmap, offset by its :py:class:`PixelRef`
        origin.

        :return: bounds within :py:class:`PixelRef` bounds
        )docstring")
    .def("setInfo", &SkBitmap::setInfo,
        R"docstring(
        Sets width, height, :py:class:`AlphaType`, :py:class:`ColorType`,
        :py:class:`ColorSpace`, and optional rowBytes.

        Frees pixels, and returns true if successful.

        imageInfo.alphaType() may be altered to a value permitted by
        imageInfo.colorSpace(). If imageInfo.colorType() is
        :py:attr:`~ColorType.kUnknown_ColorType`, imageInfo.alphaType() is set
        to :py:attr:`~AlphaType.kUnknown_AlphaType`. If imageInfo.colorType() is
        :py:attr:`~ColorType.kAlpha_8_ColorType` and imageInfo.alphaType() is
        :py:attr:`~AlphaType.kUnpremul_AlphaType`, imageInfo.alphaType() is
        replaced by :py:attr:`~AlphaType.kPremul_AlphaType`. If
        imageInfo.colorType() is :py:attr:`~ColorType.kRGB_565_ColorType` or
        :py:attr:`~ColorType.kGray_8_ColorType`, imageInfo.alphaType() is set to
        :py:attr:`~AlphaType.kOpaque_AlphaType`. If imageInfo.colorType() is
        :py:attr:`~ColorType.kARGB_4444_ColorType`,
        :py:attr:`~ColorType.kRGBA_8888_ColorType`,
        :py:attr:`~ColorType.kBGRA_8888_ColorType`, or
        :py:attr:`~ColorType.kRGBA_F16_ColorType`: imageInfo.alphaType() remains
        unchanged.

        rowBytes must equal or exceed imageInfo.minRowBytes(). If
        imageInfo.colorSpace() is :py:attr:`~ColorType.kUnknown_ColorType`,
        rowBytes is ignored and treated as zero; for all other
        :py:class:`ColorSpace` values, rowBytes of zero is treated as
        imageInfo.minRowBytes().

        Calls :py:meth:`reset` and returns false if:

        - rowBytes exceeds 31 bits
        - imageInfo.width() is negative
        - imageInfo.height() is negative
        - rowBytes is positive and less than imageInfo.width() times
            imageInfo.bytesPerPixel()

        :param skia.ImageInfo imageInfo: contains width, height,
            :py:class:`AlphaType`, :py:class:`ColorType`, :py:class:`ColorSpace`
        :param int rowBytes: imageInfo.minRowBytes() or larger; or zero
        :return: true if :py:class:`ImageInfo` set successfully
        )docstring",
        py::arg("imageInfo"), py::arg("rowBytes") = 0)
    .def("tryAllocPixelsFlags", &SkBitmap::tryAllocPixelsFlags,
        R"docstring(
        Sets :py:class:`ImageInfo` to info following the rules in
        :py:meth:`setInfo` and allocates pixel memory.

        Memory is zeroed.

        Returns false and calls :py:meth:`reset` if :py:class:`ImageInfo` could
        not be set, or memory could not be allocated, or memory could not
        optionally be zeroed.

        On most platforms, allocating pixel memory may succeed even though there
        is not sufficient memory to hold pixels; allocation does not take place
        until the pixels are written to. The actual behavior depends on the
        platform implementation of ``calloc``.

        :param skia.ImageInfo info: contains width, height,
            :py:class:`AlphaType`, :py:class:`ColorType`, :py:class:`ColorSpace`
        :param int flags: :py:attr:`kZeroPixels_AllocFlag`, or zero
        :return: true if pixels allocation is successful
        )docstring",
        py::arg("info"), py::arg("flags"))
    .def("allocPixelsFlags", &SkBitmap::allocPixelsFlags,
        R"docstring(
        Sets :py:class:`ImageInfo` to info following the rules in
        :py:meth:`setInfo` and allocates pixel memory.

        Memory is zeroed.

        Aborts execution if :py:class:`ImageInfo` could not be set, or memory
        could not be allocated, or memory could not optionally be zeroed. Abort
        steps may be provided by the user at compile time by defining SK_ABORT.

        On most platforms, allocating pixel memory may succeed even though there
        is not sufficient memory to hold pixels; allocation does not take place
        until the pixels are written to. The actual behavior depends on the
        platform implementation of ``calloc``.

        :param skia.ImageInfo info: contains width, height,
            :py:class:`AlphaType`, :py:class:`ColorType`, :py:class:`ColorSpace`
        :param int flags: :py:attr:`kZeroPixels_AllocFlag`, or zero
        )docstring",
        py::arg("info"), py::arg("flags"))
    .def("tryAllocPixels",
        [] (SkBitmap& bitmap, const SkImageInfo* info, size_t rowBytes) {
            if (!info)
                return bitmap.tryAllocPixels();
            return bitmap.tryAllocPixels(*info, rowBytes);
        },
        R"docstring(
        Sets :py:class:`ImageInfo` to info following the rules in
        :py:meth:`setInfo` and allocates pixel memory.

        rowBytes must equal or exceed info.:py:meth:`width` times
        info.bytesPerPixel, or equal zero. Pass in zero for rowBytes to compute
        the minimum valid value.

        Returns false and calls :py:meth:`reset` if :py:class:`ImageInfo` could
        not be set, or memory could not be allocated.

        On most platforms, allocating pixel memory may succeed even though there
        is not sufficient memory to hold pixels; allocation does not take place
        until the pixels are written to. The actual behavior depends on the
        platform implementation of :py:meth:`malloc`.

        :param skia.ImageInfo info: contains width, height,
            :py:class:`AlphaType`, :py:class:`ColorType`, :py:class:`ColorSpace`
        :param int rowBytes: size of pixel row or larger; may be zero
        :return: true if pixel storage is allocated
        )docstring",
        py::arg("info") = nullptr, py::arg("rowBytes") = 0)
    .def("allocPixels",
        [] (SkBitmap& bitmap, const SkImageInfo* info, size_t rowBytes) {
            if (!info)
                bitmap.allocPixels();
            else
                bitmap.allocPixels(*info, rowBytes);
        },
        R"docstring(
        Sets :py:class:`ImageInfo` to info following the rules in
        :py:meth:`setInfo` and allocates pixel memory.

        rowBytes must equal or exceed info.:py:meth:`width` times
        info.bytesPerPixel, or equal zero. Pass in zero for rowBytes to compute
        the minimum valid value.

        Aborts execution if :py:class:`ImageInfo` could not be set, or memory
        could not be allocated. Abort steps may be provided by the user at
        compile time by defining SK_ABORT.

        On most platforms, allocating pixel memory may succeed even though there
        is not sufficient memory to hold pixels; allocation does not take place
        until the pixels are written to. The actual behavior depends on the
        platform implementation of :py:meth:`malloc`.

        :param skia.ImageInfo info: contains width, height,
            :py:class:`AlphaType`, :py:class:`ColorType`, :py:class:`ColorSpace`
        :param int rowBytes: size of pixel row or larger; may be zero
        )docstring",
        py::arg("info") = nullptr, py::arg("rowBytes") = 0)
    .def("tryAllocN32Pixels", &SkBitmap::tryAllocN32Pixels,
        R"docstring(
        Sets :py:class:`ImageInfo` to width, height, and native color type; and
        allocates pixel memory.

        If isOpaque is true, sets :py:class:`ImageInfo` to
        :py:attr:`~AlphaType.kOpaque_AlphaType`; otherwise, sets to
        :py:attr:`~AlphaType.kPremul_AlphaType`.

        Calls reset() and returns false if width exceeds 29 bits or is negative,
        or height is negative.

        Returns false if allocation fails.

        Use to create :py:class:`Bitmap` that matches :py:class:`PMColor`, the
        native pixel arrangement on the platform. :py:class:`Bitmap` drawn to
        output device skips converting its pixel format.

        :param int width: pixel column count; must be zero or greater
        :param int height: pixel row count; must be zero or greater
        :param bool isOpaque: true if pixels do not have transparency
        :return: true if pixel storage is allocated
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("isOpaque"))
    .def("allocN32Pixels", &SkBitmap::allocN32Pixels,
        R"docstring(
        Sets :py:class:`ImageInfo` to width, height, and the native color type;
        and allocates pixel memory.

        If isOpaque is true, sets :py:class:`ImageInfo` to
        :py:attr:`~AlphaType.kOpaque_AlphaType`; otherwise, sets to
        :py:attr:`~AlphaType.kPremul_AlphaType`.

        Aborts if width exceeds 29 bits or is negative, or height is negative,
        or allocation fails. Abort steps may be provided by the user at compile
        time by defining SK_ABORT.

        Use to create :py:class:`Bitmap` that matches :py:class:`PMColor`, the
        native pixel arrangement on the platform. :py:class:`Bitmap` drawn to
        output device skips converting its pixel format.

        :param int width: pixel column count; must be zero or greater
        :param int height: pixel row count; must be zero or greater
        :param bool isOpaque: true if pixels do not have transparency
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("isOpaque"))
    .def("installPixels",
        [] (SkBitmap& bitmap, const SkImageInfo& imageInfo, py::object obj,
            size_t srcRowBytes) {
            if (obj.is_none())
                return bitmap.installPixels(imageInfo, nullptr, srcRowBytes);
            auto buffer = obj.cast<py::buffer>();
            auto info = buffer.request();
            auto rowBytes = ValidateBufferToImageInfo(
                imageInfo, info, srcRowBytes);
            return bitmap.installPixels(imageInfo, info.ptr, rowBytes);
        },
        R"docstring(
        Sets :py:class:`ImageInfo` to info following the rules in
        :py:meth:`setInfo`, and creates :py:class:`PixelRef` containing pixels
        and rowBytes.

        If :py:class:`ImageInfo` could not be set, or rowBytes is less than
        info.minRowBytes(): calls :py:meth:`reset`, and returns false.

        Otherwise, if pixels equals nullptr: sets :py:class:`ImageInfo`, returns
        true.

        Caller must ensure that pixels are valid for the lifetime of
        :py:class:`Bitmap` and :py:class:`PixelRef`.

        :info: contains width, height,
            :py:class:`AlphaType`, :py:class:`ColorType`, :py:class:`ColorSpace`
        :pixels: address or pixel storage buffer; may be nullptr
        :rowBytes: size of pixel row or larger
        :return: true if :py:class:`ImageInfo` is set to info
        )docstring",
        py::arg("info"), py::arg("pixels"), py::arg("rowBytes"))
    .def("installPixels",
        py::overload_cast<const SkPixmap&>(&SkBitmap::installPixels),
        R"docstring(
        Sets :py:class:`ImageInfo` to pixmap.info() following the rules in
        :py:meth:`setInfo`, and creates :py:class:`PixelRef` containing
        pixmap.addr() and pixmap.rowBytes().

        If :py:class:`ImageInfo` could not be set, or pixmap.rowBytes() is less
        than :py:meth:`ImageInfo.minRowBytes` : calls :py:meth:`reset`, and
        returns false.

        Otherwise, if pixmap.addr() equals nullptr: sets :py:class:`ImageInfo`,
        returns true.

        Caller must ensure that pixmap is valid for the lifetime of
        :py:class:`Bitmap` and :py:class:`PixelRef`.

        :pixmap: :py:class:`ImageInfo`, pixel address, and rowBytes()
        :return: true if :py:class:`ImageInfo` was set to pixmap.info()
        )docstring",
        py::arg("pixmap"))
    .def("setPixels",
        [] (SkBitmap& bitmap, py::buffer b) {
            auto info = b.request();
            ValidateBufferToImageInfo(bitmap.info(), info, bitmap.rowBytes());
            return bitmap.setPixels(info.ptr);
        },
        R"docstring(
        Replaces :py:class:`PixelRef` with pixels, preserving
        :py:class:`ImageInfo` and rowBytes().

        Sets :py:class:`PixelRef` origin to (0, 0).

        If pixels is nullptr, or if info().colorType() equals
        :py:attr:`~ColorType.kUnknown_ColorType`; release reference to
        :py:class:`PixelRef`, and set :py:class:`PixelRef` to nullptr.

        Caller is responsible for handling ownership pixel memory for the
        lifetime of :py:class:`Bitmap` and :py:class:`PixelRef`.

        :param Union[bytes,bytearray,memoryview] pixels: address of pixel
            storage, managed by caller
        )docstring",
        py::arg("pixels"))
    // .def("pixelRef", &SkBitmap::pixelRef,
    //     "Returns SkPixelRef, which contains: pixel base address; its "
    //     "dimensions; and rowBytes(), the interval from one row to the next."
    //     py::return_value_policy::reference)
    // .def("pixelRefOrigin", &SkBitmap::pixelRefOrigin,
    //     "Returns origin of pixels within SkPixelRef.")
    // .def("setPixelRef", &SkBitmap::setPixelRef,
    //     "Replaces pixelRef and origin in SkBitmap.")
    .def("readyToDraw", &SkBitmap::readyToDraw,
        R"docstring(
        Returns true if :py:class:`Bitmap` is can be drawn.

        :return: true if :py:class:`getPixels` is not nullptr
        )docstring")
    .def("getGenerationID", &SkBitmap::getGenerationID,
        R"docstring(
        Returns a unique value corresponding to the pixels in
        :py:class:`PixelRef`.

        Returns a different value after :py:meth:`notifyPixelsChanged` has been
        called. Returns zero if :py:class:`PixelRef` is nullptr.

        Determines if pixels have changed since last examined.

        :return: unique value for pixels in :py:class:`PixelRef`
        )docstring")
    .def("notifyPixelsChanged", &SkBitmap::notifyPixelsChanged,
        R"docstring(
        Marks that pixels in :py:class:`PixelRef` have changed.

        Subsequent calls to :py:meth:`getGenerationID` return a different value.
        )docstring")
    .def("eraseColor", &SkBitmap::eraseColor,
        R"docstring(
        Replaces pixel values with c, interpreted as being in the sRGB
        :py:class:`ColorSpace`.

        All pixels contained by bounds() are affected. If the colorType() is
        :py:attr:`~ColorType.kGray_8_ColorType` or
        :py:attr:`~ColorType.kRGB_565_ColorType`, then alpha is ignored; RGB is
        treated as opaque. If colorType() is
        :py:attr:`~ColorType.kAlpha_8_ColorType`, then RGB is ignored.

        :param int c: unpremultiplied color
        )docstring",
        py::arg("c"))
    .def("eraseARGB", &SkBitmap::eraseARGB,
        R"docstring(
        Replaces pixel values with unpremultiplied color built from a, r, g, and
        b, interpreted as being in the sRGB :py:class:`ColorSpace`.

        All pixels contained by bounds() are affected. If the colorType() is
        :py:attr:`~ColorType.kGray_8_ColorType` or
        :py:attr:`~ColorType.kRGB_565_ColorType`, then alpha is ignored; RGB is
        treated as opaque. If colorType() is
        :py:attr:`~ColorType.kAlpha_8_ColorType`, then RGB is ignored.

        :param int a: amount of alpha, from fully transparent (0) to fully
            opaque (255)
        :param int r: amount of red, from no red (0) to full red (255)
        :param int g: amount of green, from no green (0) to full green (255)
        :param int b: amount of blue, from no blue (0) to full blue (255)
        )docstring",
        py::arg("a"), py::arg("r"), py::arg("g"), py::arg("b"))
    .def("erase", &SkBitmap::erase,
        R"docstring(
        Replaces pixel values inside area with c.

        interpreted as being in the sRGB :py:class:`ColorSpace`. If area does
        not intersect bounds(), call has no effect.

        If the colorType() is :py:attr:`~ColorType.kGray_8_ColorType` or
        :py:attr:`~ColorType.kRGB_565_ColorType`, then alpha is ignored; RGB is
        treated as opaque. If colorType() is
        :py:attr:`~ColorType.kAlpha_8_ColorType`, then RGB is ignored.

        :param int c: unpremultiplied color
        :param skia.IRect area: rectangle to fill
        )docstring",
        py::arg("c"), py::arg("area"))
    .def("getColor", &SkBitmap::getColor,
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
    .def("getAlphaf", &SkBitmap::getAlphaf,
        R"docstring(
        Look up the pixel at (x,y) and return its alpha component, normalized to
        [0..1].

        This is roughly equivalent to :py:func:`GetColorA` of
        :py:meth:`getColor`, but can be more efficent (and more precise if the
        pixels store more than 8 bits per component).

        :param int x: column index, zero or greater, and less than
            :py:meth:`width`
        :param int y: row index, zero or greater, and less than
            :py:meth:`height`
        :return: alpha converted to normalized float
        )docstring",
        py::arg("x"), py::arg("y"))
    // .def("getAddr", &SkBitmap::getAddr, "Returns pixel address at (x, y).",
    //     py::return_value_policy::reference)
    // .def("getAddr32", &SkBitmap::getAddr32, "Returns address at (x, y).",
    //     py::return_value_policy::reference)
    // .def("getAddr16", &SkBitmap::getAddr16, "Returns address at (x, y).",
    //     py::return_value_policy::reference)
    // .def("getAddr8", &SkBitmap::getAddr8, "Returns address at (x, y).",
    //     py::return_value_policy::reference)
    .def("extractSubset", &SkBitmap::extractSubset,
        R"docstring(
        Shares :py:class:`PixelRef` with dst.

        Pixels are not copied; :py:class:`Bitmap` and dst point to the same
        pixels; dst :py:meth:`bounds` are set to the intersection of subset and
        the original :py:meth:`bounds`.

        subset may be larger than :py:meth:`bounds`. Any area outside of
        :py:meth:`bounds` is ignored.

        Any contents of dst are discarded. :py:meth:`isVolatile` setting is
        copied to dst. dst is set to :py:meth:`colorType`, :py:meth:`alphaType`,
        and :py:meth:`colorSpace`.

        Return false if:

        - dst is nullptr
        - :py:class:`PixelRef` is nullptr
        - subset does not intersect :py:meth:`bounds`

        :param dst: :py:class:`Bitmap` set to subset
        :param subset: rectangle of pixels to reference
        :return: true if dst is replaced by subset
        )docstring",
        py::arg("dst").none(false), py::arg("subset"))
    .def("readPixels", &ReadPixels<SkBitmap>,
        R"docstring(
        Copies a :py:class:`Rect` of pixels from :py:class:`Bitmap` to
        dstPixels.

        Copy starts at (srcX, srcY), and does not exceed :py:class:`Bitmap` (
        :py:meth:`width`, :py:meth:`height`).

        dstInfo specifies width, height, :py:class:`ColorType`,
        :py:class:`AlphaType`, and :py:class:`ColorSpace` of destination.
        dstRowBytes specifics the gap from one destination row to the next.
        Returns true if pixels are copied. Returns false if:

        - dstInfo has no address
        - dstRowBytes is less than dstInfo.minRowBytes()
        - :py:class:`PixelRef` is nullptr

        Pixels are copied only if pixel conversion is possible. If
        :py:class:`Bitmap` colorType() is
        :py:attr:`~ColorType.kGray_8_ColorType`, or
        :py:attr:`~ColorType.kAlpha_8_ColorType`; dstInfo.colorType() must
        match. If :py:class:`Bitmap` colorType() is
        :py:attr:`~ColorType.kGray_8_ColorType`, dstInfo.colorSpace() must
        match. If :py:class:`Bitmap` alphaType() is
        :py:attr:`~AlphaType.kOpaque_AlphaType`, dstInfo.alphaType() must match.
        If :py:class:`Bitmap` colorSpace() is nullptr, dstInfo.colorSpace() must
        match. Returns false if pixel conversion is not possible.

        srcX and srcY may be negative to copy only top or left of source.
        Returns false if width() or height() is zero or negative. Returns false
        if abs(srcX) >= Bitmap width(), or if abs(srcY) >= Bitmap height().

        :dstInfo: destination width, height, :py:class:`ColorType`,
            :py:class:`AlphaType`, :py:class:`ColorSpace`
        :dstPixels: destination pixel storage
        :dstRowBytes: destination row length
        :srcX: column index whose absolute value is less than :py:meth:`width`
        :srcY: row index whose absolute value is less than :py:meth:`height`
        :return: true if pixels are copied to dstPixels
        )docstring",
        py::arg("dstInfo"), py::arg("dstPixels"), py::arg("dstRowBytes") = 0,
        py::arg("srcX") = 0, py::arg("srcY") = 0)
    .def("readPixels",
        py::overload_cast<const SkPixmap&, int, int>(
            &SkBitmap::readPixels, py::const_),
        R"docstring(
        Copies a :py:class:`Rect` of pixels from :py:class:`Bitmap` to
        dst.

        Copy starts at (srcX, srcY), and does not exceed :py:class:`Bitmap` (
        :py:meth:`width`, :py:meth:`height`).

        dst specifies width, height, :py:class:`ColorType`,
        :py:class:`AlphaType`, :py:class:`ColorSpace`, pixel storage, and row
        bytes of destination. dst.rowBytes() specifics the gap from one
        destination row to the next. Returns true if pixels are copied. Returns
        false if:

        - dst pixel storage equals nullptr
        - dst.rowBytes is less than :py:meth:`ImageInfo.minRowBytes`
        - :py:class:`PixelRef` is nullptr

        Pixels are copied only if pixel conversion is possible. If
        :py:class:`Bitmap` colorType() is
        :py:attr:`~ColorType.kGray_8_ColorType`, or
        :py:attr:`~ColorType.kAlpha_8_ColorType`; dst :py:class:`ColorType` must
        match. If :py:class:`Bitmap` colorType() is
        :py:attr:`~ColorType.kGray_8_ColorType`, dst :py:class:`ColorSpace` must
        match. If :py:class:`Bitmap` alphaType() is
        :py:attr:`~AlphaType.kOpaque_AlphaType`, dst :py:class:`AlphaType` must
        match. If :py:class:`Bitmap` colorSpace() is nullptr, dst
        :py:class:`ColorSpace` must match. Returns false if pixel conversion is
        not possible.

        srcX and srcY may be negative to copy only top or left of source.
        Returns false if width() or height() is zero or negative. Returns false
        if abs(srcX) >= Bitmap width(), or if abs(srcY) >= Bitmap height().

        :dst: destination :py:class:`Pixmap`: :py:class:`ImageInfo`, pixels, row
            bytes
        :srcX: column index whose absolute value is less than width()
        :srcY: row index whose absolute value is less than height()
        :return: true if pixels are copied to dst
        )docstring",
        py::arg("dst"), py::arg("srcX") = 0, py::arg("srcY") = 0)
    .def("writePixels",
        py::overload_cast<const SkPixmap&, int, int>(&SkBitmap::writePixels),
        R"docstring(
        Copies a :py:class:`Rect` of pixels from src.

        Copy starts at (dstX, dstY), and does not exceed (``src.width()``,
        ``src.height()``).

        src specifies width, height, :py:class:`ColorType`,
        :py:class:`AlphaType`, :py:class:`ColorSpace`, pixel storage, and row
        bytes of source. src.rowBytes() specifics the gap from one source row to
        the next. Returns true if pixels are copied. Returns false if:

        - src pixel storage equals nullptr
        - src.rowBytes is less than :py:class:`ImageInfo`::minRowBytes()
        - :py:class:`PixelRef` is nullptr

        Pixels are copied only if pixel conversion is possible. If
        :py:class:`Bitmap` colorType() is
        :py:attr:`~ColorType.kGray_8_ColorType`, or
        :py:attr:`~ColorType.kAlpha_8_ColorType`; src :py:class:`ColorType` must
        match. If :py:class:`Bitmap` colorType() is
        :py:attr:`~ColorType.kGray_8_ColorType`, src :py:class:`ColorSpace` must
        match. If :py:class:`Bitmap` alphaType() is
        :py:attr:`~AlphaType.kOpaque_AlphaType`, src :py:class:`AlphaType` must
        match. If :py:class:`Bitmap` colorSpace() is nullptr, src
        :py:class:`ColorSpace` must match. Returns false if pixel conversion is
        not possible.

        dstX and dstY may be negative to copy only top or left of source.
        Returns false if width() or height() is zero or negative. Returns false
        if abs(dstX) >= Bitmap width(), or if abs(dstY) >= Bitmap height().

        :param skia.Pixmap src: source :py:class:`Pixmap`:
            :py:class:`ImageInfo`, pixels, row bytes
        :param int dstX: column index whose absolute value is less than width()
        :param int dstY: row index whose absolute value is less than height()
        :return: true if src pixels are copied to :py:class:`Bitmap`
        )docstring",
        py::arg("src"), py::arg("dstX") = 0, py::arg("dstY") = 0)
    .def("extractAlpha",
        py::overload_cast<SkBitmap*, const SkPaint*, SkIPoint*>(
            &SkBitmap::extractAlpha, py::const_),
        R"docstring(
        Sets dst to alpha described by pixels.

        Returns false if dst cannot be written to or dst pixels cannot be
        allocated.

        If paint is not nullptr and contains :py:class:`MaskFilter`,
        :py:class:`MaskFilter` generates mask alpha from :py:class:`Bitmap`.
        Uses HeapAllocator to reserve memory for dst :py:class:`PixelRef`. Sets
        offset to top-left position for dst for alignment with
        :py:class:`Bitmap`; (0, 0) unless :py:class:`MaskFilter` generates mask.

        :param dst: holds :py:class:`PixelRef` to fill with alpha layer
        :param paint: holds optional :py:class:`MaskFilter`; may be nullptr
        :param offset: top-left position for dst; may be nullptr
        :return: true if alpha layer was constructed in dst :py:class:`PixelRef`
        )docstring",
        py::arg("dst"), py::arg("paint") = nullptr, py::arg("offset") = nullptr)
    .def("peekPixels", &SkBitmap::peekPixels,
        R"docstring(
        Copies :py:class:`Bitmap` pixel address, row bytes, and
        :py:class:`ImageInfo` to pixmap, if address is available, and returns
        true.

        If pixel address is not available, return false and leave pixmap
        unchanged.

        pixmap contents become invalid on any future change to
        :py:class:`Bitmap`.

        :param skia.Pixmap pixmap: storage for pixel state if pixels are
            readable; otherwise, ignored
        :return: true if :py:class:`Bitmap` has direct access to pixels
        )docstring",
        py::arg("pixmap"))
    .def("makeShader",
        py::overload_cast<SkTileMode, SkTileMode, const SkMatrix*>(
            &SkBitmap::makeShader, py::const_),
        py::arg("tmx") = SkTileMode::kClamp,
        py::arg("tmy") = SkTileMode::kClamp, py::arg("localMatrix") = nullptr)
    ;


m.def("ComputeIsOpaque", &SkBitmap::ComputeIsOpaque,
    R"docstring(
    Returns true if all pixels are opaque.

    :py:class:`ColorType` determines how pixels are encoded, and whether pixel
    describes alpha. Returns true for :py:class:`ColorType` without alpha in
    each pixel; for other :py:class:`ColorType`, returns true if all pixels have
    alpha values equivalent to 1.0 or greater.

    For :py:class:`ColorType` :py:attr:`~ColorType.kRGB_565_ColorType` or
    :py:attr:`~ColorType.kGray_8_ColorType`: always returns true. For
    :py:class:`ColorType` :py:attr:`~ColorType.kAlpha_8_ColorType`,
    :py:attr:`~ColorType.kBGRA_8888_ColorType`,
    :py:attr:`~ColorType.kRGBA_8888_ColorType`: returns true if all pixel alpha
    values are 255. For :py:class:`ColorType`
    :py:attr:`~ColorType.kARGB_4444_ColorType`: returns true if all pixel alpha
    values are 15. For :py:attr:`~ColorType.kRGBA_F16_ColorType`: returns true
    if all pixel alpha values are 1.0 or greater.

    Returns false for :py:attr:`~ColorType.kUnknown_ColorType`.

    :param skia.Bitmap bm: :py:class:`Bitmap` to check
    :return: true if all pixels have opaque values or :py:class:`ColorType` is
        opaque
    )docstring",
    py::arg("bm"));
}
