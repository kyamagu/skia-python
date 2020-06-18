#include "common.h"
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

const SkSurfaceProps::Flags SkSurfaceProps::kUseDistanceFieldFonts_Flag;

void initSurface(py::module &m) {

py::enum_<SkBackingFit>(m, "BackingFit", R"docstring(
    Indicates whether a backing store needs to be an exact match or can be
    larger than is strictly necessary.
    )docstring")
    .value("kApprox", SkBackingFit::kApprox)
    .value("kExact", SkBackingFit::kExact)
    .export_values();

py::enum_<SkPixelGeometry>(m, "PixelGeometry", R"docstring(
    Description of how the LCD strips are arranged for each pixel.

    If this is unknown, or the pixels are meant to be "portable" and/or
    transformed before showing (e.g. rotated, scaled) then use
    :py:attr:`PixelGeometry.kUnknown`.
    )docstring")
    .value("kUnknown_PixelGeometry", SkPixelGeometry::kUnknown_SkPixelGeometry)
    .value("kRGB_H_PixelGeometry", SkPixelGeometry::kRGB_H_SkPixelGeometry)
    .value("kBGR_H_PixelGeometry", SkPixelGeometry::kBGR_H_SkPixelGeometry)
    .value("kRGB_V_PixelGeometry", SkPixelGeometry::kRGB_V_SkPixelGeometry)
    .value("kBGR_V_PixelGeometry", SkPixelGeometry::kBGR_V_SkPixelGeometry)
    .export_values();

py::class_<SkSurfaceProps> surfaceprops(m, "SurfaceProps", R"docstring(
    Describes properties and constraints of a given :py:class:`Surface`.

    The rendering engine can parse these during drawing, and can sometimes
    optimize its performance (e.g. disabling an expensive feature).
    )docstring");

py::enum_<SkSurfaceProps::Flags>(surfaceprops, "Flags", py::arithmetic())
    .value("kUseDeviceIndependentFonts_Flag",
        SkSurfaceProps::Flags::kUseDeviceIndependentFonts_Flag)
    .export_values();

py::enum_<SkSurfaceProps::InitType>(surfaceprops, "InitType")
    .value("kLegacyFontHost_InitType",
        SkSurfaceProps::InitType::kLegacyFontHost_InitType)
    .export_values();

surfaceprops
    .def(py::init<uint32_t, SkPixelGeometry>(),
        py::arg("flags"), py::arg("geometry"))
    .def(py::init<SkSurfaceProps::InitType>(),
        py::arg("initType"))
    .def(py::init<uint32_t, SkSurfaceProps::InitType>(),
        py::arg("flags"), py::arg("initType"))
    .def(py::init<const SkSurfaceProps&>(),
        py::arg("props"))
    .def("flags", &SkSurfaceProps::flags)
    .def("pixelGeometry", &SkSurfaceProps::pixelGeometry)
    .def("isUseDeviceIndependentFonts",
        &SkSurfaceProps::isUseDeviceIndependentFonts)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def_readonly_static("kUseDistanceFieldFonts_Flag",
        &SkSurfaceProps::kUseDistanceFieldFonts_Flag,
        R"docstring(
        Deprecated alias used by Chromium.

        Will be removed.
        )docstring")
    ;

py::class_<SkSurfaceCharacterization>(m, "SurfaceCharacterization")
    .def(py::init())
    .def("createResized", &SkSurfaceCharacterization::createResized,
        py::arg("width"), py::arg("height"))
    .def("createColorSpace", &SkSurfaceCharacterization::createColorSpace,
        py::arg("cs"))
    .def("createBackendFormat", &SkSurfaceCharacterization::createBackendFormat,
        py::arg("colorType"), py::arg("backendFormat"))
    .def("createFBO0", &SkSurfaceCharacterization::createFBO0,
        py::arg("usesGLFBO0"))
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def("cacheMaxResourceBytes",
        &SkSurfaceCharacterization::cacheMaxResourceBytes)
    .def("isValid", &SkSurfaceCharacterization::isValid)
    .def("width", &SkSurfaceCharacterization::width)
    .def("height", &SkSurfaceCharacterization::height)
    // .def("stencilCount", &SkSurfaceCharacterization::stencilCount)
    .def("isTextureable", &SkSurfaceCharacterization::isTextureable)
    .def("isMipMapped", &SkSurfaceCharacterization::isMipMapped)
    .def("usesGLFBO0", &SkSurfaceCharacterization::usesGLFBO0)
    .def("vulkanSecondaryCBCompatible",
        &SkSurfaceCharacterization::vulkanSecondaryCBCompatible)
    .def("colorSpace", &SkSurfaceCharacterization::colorSpace,
        py::return_value_policy::reference)
    .def("refColorSpace", &SkSurfaceCharacterization::refColorSpace)
    .def("surfaceProps", &SkSurfaceCharacterization::surfaceProps)
    ;

py::class_<SkSurface, sk_sp<SkSurface>> surface(
    m, "Surface", py::buffer_protocol(), R"docstring(
    :py:class:`Surface` is responsible for managing the pixels that a canvas
    draws into.

    The pixels can be allocated either in CPU memory (a raster surface) or on
    the GPU (a GrRenderTarget surface). :py:class:`Surface` takes care of
    allocating a :py:class:`Canvas` that will draw into the surface. Call
    :py:meth:`getCanvas` to use that canvas (but don't delete it, it is owned
    by the surface). :py:class:`Surface` always has non-zero dimensions. If
    there is a request for a new surface, and either of the requested dimensions
    are zero, then nullptr will be returned.
    )docstring");

py::class_<SkSurface::AsyncReadResult>(surface, "AsyncReadResult", R"docstring(
    The result from :py:meth:`Surface.asyncRescaleAndReadPixels` or
    :py:meth:`Surface.asyncRescaleAndReadPixelsYUV420`.
    )docstring");

py::enum_<SkSurface::ContentChangeMode>(surface, "ContentChangeMode")
    .value("kDiscard_ContentChangeMode",
        SkSurface::ContentChangeMode::kDiscard_ContentChangeMode,
        "discards surface on change")
    .value("kRetain_ContentChangeMode",
        SkSurface::ContentChangeMode::kRetain_ContentChangeMode,
        "preserves surface on change")
    .export_values();

py::enum_<SkSurface::BackendHandleAccess>(surface, "BackendHandleAccess")
    .value("kFlushRead_BackendHandleAccess",
        SkSurface::BackendHandleAccess::kFlushRead_BackendHandleAccess,
        "back-end object is readable")
    .value("kFlushWrite_BackendHandleAccess",
        SkSurface::BackendHandleAccess::kFlushWrite_BackendHandleAccess,
        "back-end object is writable")
    .value("kDiscardWrite_BackendHandleAccess",
        SkSurface::BackendHandleAccess::kDiscardWrite_BackendHandleAccess,
        "back-end object must be overwritten")
    .export_values();

py::enum_<SkSurface::RescaleGamma>(surface, "RescaleGamma", R"docstring(
    Controls the gamma that rescaling occurs in for
    :py:meth:`Surface.asyncRescaleAndReadPixels` and
    :py:meth:`Surface.asyncRescaleAndReadPixelsYUV420`.
    )docstring")
    .value("kSrc", SkSurface::RescaleGamma::kSrc)
    .value("kLinear", SkSurface::RescaleGamma::kLinear)
    .export_values();

py::enum_<SkSurface::BackendSurfaceAccess>(surface, "BackendSurfaceAccess")
    .value("kNoAccess", SkSurface::BackendSurfaceAccess::kNoAccess,
        "back-end object will not be used by client")
    .value("kPresent", SkSurface::BackendSurfaceAccess::kPresent,
        "back-end surface will be used for presenting to screen")
    .export_values();

py::enum_<SkSurface::FlushFlags>(surface, "FlushFlags", py::arithmetic())
    .value("kNone_FlushFlags", SkSurface::FlushFlags::kNone_FlushFlags)
    .value("kSyncCpu_FlushFlag", SkSurface::FlushFlags::kSyncCpu_FlushFlag)
    .export_values();

surface
    .def("__repr__",
        [] (SkSurface& surface) {
            return py::str("Surface({}, {}, '{}')").format(
                surface.width(), surface.height(),
                (surface.getContext()) ? "GPU" : "Raster");
        })
    .def("_repr_png_",
        [] (SkSurface& surface) {
            auto image = surface.makeImageSnapshot();
            if (!image)
                throw std::runtime_error("Failed to make an image snapshot.");
            auto data = image->encodeToData();
            if (!data)
                throw std::runtime_error("Failed to encode an image.");
            return py::bytes(
                static_cast<const char*>(data->data()), data->size());
        })
    .def("numpy", &ReadToNumpy<SkSurface>,
        R"docstring(
        Exports a ``numpy.ndarray``.

        :param srcX: offset into readable pixels on x-axis; may be negative
        :param srcY: offset into readable pixels on y-axis; may be negative
        :param colorType: target :py:class:`ColorType`
        :param alphaType: target :py:class:`AlphaType`
        :param colorSpace: target :py:class:`ColorSpace`
        :return: numpy.ndarray
        )docstring",
        py::arg("srcX") = 0, py::arg("srcY") = 0,
        py::arg("colorType") = kUnknown_SkColorType,
        py::arg("alphaType") = kUnpremul_SkAlphaType,
        py::arg("colorSpace") = nullptr)
    .def(py::init(&SkSurface::MakeRasterN32Premul),
        R"docstring(
        See :py:meth:`~MakeRasterN32Premul`
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("surfaceProps") = nullptr)
    .def(py::init(
        [] (py::array array, SkColorType ct, SkAlphaType at,
            const SkColorSpace* cs, const SkSurfaceProps *surfaceProps) {
            auto imageInfo = NumPyToImageInfo(array, ct, at, cs);
            auto surface = SkSurface::MakeRasterDirect(
                imageInfo, array.mutable_data(), array.strides(0),
                surfaceProps);
            if (!surface)
                throw std::runtime_error("Failed to create Canvas");
            return surface;
        }),
        R"docstring(
        Create a raster surface on numpy array.

        Input array must have compatible shape with the given color type.

        Do not destroy numpy array while using this surface.

        :array: numpy ndarray of shape=(height, width, channels) and appropriate
            dtype. Must have non-zero width and height, and the valid number of
            channels for the specified color type.
        :colorType: color type of the array
        :alphaType: alpha type of the array
        :colorSpace: range of colors; may be nullptr
        )docstring",
        py::arg("array"), py::arg("colorType") = kN32_SkColorType,
        py::arg("alphaType") = kUnpremul_SkAlphaType,
        py::arg("colorSpace") = nullptr, py::arg("surfaceProps") = nullptr)
    .def("isCompatible", &SkSurface::isCompatible,
        R"docstring(
        Is this surface compatible with the provided characterization?

        This method can be used to determine if an existing :py:class:`Surface`
        is a viable destination for an :py:class:`DeferredDisplayList`.

        :param skia.SurfaceCharacterization characterization: The
            characterization for which a compatibility check is desired
        :return: true if this surface is compatible with the characterization;
            false otherwise
        )docstring",
        py::arg("characterization"))
    .def("width", &SkSurface::width,
        R"docstring(
        Returns pixel count in each row; may be zero or greater.

        :return: number of pixel columns
        )docstring")
    .def("height", &SkSurface::height,
        R"docstring(
        Returns pixel row count; may be zero or greater.

        :return: number of pixel rows
        )docstring")
    .def("imageInfo", &SkSurface::imageInfo,
        R"docstring(
        Returns an :py:class:`ImageInfo` describing the surface.
        )docstring")
    .def("generationID", &SkSurface::generationID,
        R"docstring(
        Returns unique value identifying the content of :py:class:`Surface`.

        Returned value changes each time the content changes. Content is changed
        by drawing, or by calling :py:meth:`notifyContentWillChange`.

        :return: unique content identifier
        )docstring")
    // .def("notifyContentWillChange", &SkSurface::notifyContentWillChange,
    //     "Notifies that SkSurface contents will be changed by code outside of "
    //     "Skia.")
    .def("getContext", &SkSurface::getContext,
        R"docstring(
        Returns the GPU context of the GPU surface.

        :return: GPU context, if available; nullptr otherwise
        )docstring",
        py::return_value_policy::reference)
    .def("getBackendTexture", &SkSurface::getBackendTexture,
        R"docstring(
        Retrieves the back-end texture.

        If :py:class:`Surface` has no back-end texture, an invalid object is
        returned. Call :py:meth:`GrBackendTexture.isValid` to determine if the
        result is valid.

        The returned :py:class:`GrBackendTexture` should be discarded if the
        :py:class:`Surface` is drawn to or deleted.

        :return: GPU texture reference; invalid on failure
        )docstring",
        py::arg("backendHandleAccess"))
    .def("getBackendRenderTarget", &SkSurface::getBackendRenderTarget,
        R"docstring(
        Retrieves the back-end render target.

        If :py:class:`Surface` has no back-end render target, an invalid object
        is returned. Call :py:meth:`GrBackendRenderTarget.isValid` to determine
        if the result is valid.

        The returned :py:class:`GrBackendRenderTarget` should be discarded if
        the :py:class:`Surface` is drawn to or deleted.

        :return: GPU render target reference; invalid on failure
        )docstring",
        py::arg("backendHandleAccess"))
    .def("replaceBackendTexture",
        [] (SkSurface& surface, const GrBackendTexture& backendTexture,
            GrSurfaceOrigin origin, SkSurface::ContentChangeMode mode) {
            return surface.replaceBackendTexture(backendTexture, origin, mode);
        },
        R"docstring(
        If the surface was made via :py:meth:`MakeFromBackendTexture` then it's
        backing texture may be substituted with a different texture.

        The contents of the previous backing texture are copied into the new
        texture. :py:class:`Canvas` state is preserved. The original sample
        count is used. The :py:class:`GrBackendFormat` and dimensions of
        replacement texture must match that of the original.

        :param backendTexture:  the new backing texture for the surface
        :param mode:    Retain or discard current Content
        )docstring",
        py::arg("backendTexture"), py::arg("origin"),
        py::arg("mode") = SkSurface::kRetain_ContentChangeMode)
    .def("getCanvas", &SkSurface::getCanvas,
        R"docstring(
        Returns :py:class:`Canvas` that draws into :py:class:`Surface`.

        Subsequent calls return the same :py:class:`Canvas`. :py:class:`Canvas`
        returned is managed and owned by :py:class:`Surface`, and is deleted
        when :py:class:`Surface` is deleted.

        :return: drawing :py:class:`Canvas` for :py:class:`Surface`
        )docstring",
        py::return_value_policy::reference)
    .def("makeSurface",
        py::overload_cast<const SkImageInfo&>(&SkSurface::makeSurface),
        R"docstring(
        Returns a compatible :py:class:`Surface`, or nullptr.

        Returned :py:class:`Surface` contains the same raster, GPU, or null
        properties as the original. Returned :py:class:`Surface` does not share
        the same pixels.

        Returns nullptr if imageInfo width or height are zero, or if imageInfo
        is incompatible with :py:class:`Surface`.

        :param skia.ImageInfo imageInfo: width, height, :py:class:`ColorType`,
            :py:class:`AlphaType`, :py:class:`ColorSpace`, of
            :py:class:`Surface`; width and height must be greater than zero
        :return: compatible :py:class:`Surface` or nullptr
        )docstring",
        py::arg("imageInfo"))
    .def("makeSurface",
        py::overload_cast<int, int>(&SkSurface::makeSurface),
        R"docstring(
        Calls :py:meth:`makeSurface()` with the same ImageInfo as this surface,
        but with the specified width and height.
        )docstring",
        py::arg("width"), py::arg("height"))
    .def("makeImageSnapshot",
        py::overload_cast<>(&SkSurface::makeImageSnapshot),
        R"docstring(
        Returns :py:class:`Image` capturing :py:class:`Surface` contents.

        Subsequent drawing to :py:class:`Surface` contents are not captured.
        :py:class:`Image` allocation is accounted for if :py:class:`Surface` was
        created with :py:attr:`Budgeted.kYes`.

        :return: :py:class:`Image` initialized with :py:class:`Surface` contents
        )docstring")
    .def("makeImageSnapshot",
        py::overload_cast<const SkIRect &>(&SkSurface::makeImageSnapshot),
        R"docstring(
        Like the no-parameter version, this returns an image of the current
        surface contents.

        This variant takes a rectangle specifying the subset of the surface that
        is of interest. These bounds will be sanitized before being used.

        - If bounds extends beyond the surface, it will be trimmed to just the
            intersection of it and the surface.
        - If bounds does not intersect the surface, then this returns nullptr.
        - If bounds == the surface, then this is the same as calling the
            no-parameter variant.
        )docstring",
        py::arg("bounds"))
    .def("draw",
        py::overload_cast<SkCanvas*, SkScalar, SkScalar, const SkPaint*>(
            &SkSurface::draw),
        R"docstring(
        Draws :py:class:`Surface` contents to canvas, with its top-left corner
        at (x, y).

        If :py:class:`Paint` paint is not nullptr, apply
        :py:class:`ColorFilter`, alpha, :py:class:`ImageFilter`,
        :py:class:`BlendMode`, and :py:class:`DrawLooper`.

        :canvas: :py:class:`Canvas` drawn into
        :x: horizontal offset in :py:class:`Canvas`
        :y: vertical offset in :py:class:`Canvas`
        :paint: :py:class:`Paint` containing :py:class:`BlendMode`,
            :py:class:`ColorFilter`, :py:class:`ImageFilter`, and so on; or
            nullptr
        )docstring",
        py::arg("canvas"), py::arg("x"), py::arg("y"),
        py::arg("paint") = nullptr)
    .def("peekPixels", &SkSurface::peekPixels,
        R"docstring(
        Copies :py:class:`Surface` pixel address, row bytes, and
        :py:class:`ImageInfo` to :py:class:`Pixmap`, if address is available,
        and returns true.

        If pixel address is not available, return false and leave
        :py:class:`Pixmap` unchanged.

        pixmap contents become invalid on any future change to
        :py:class:`Surface`.

        :param skia.Pixmap pixmap: storage for pixel state if pixels are
            readable; otherwise, ignored
        :return: true if :py:class:`Surface` has direct access to pixels
        )docstring",
        py::arg("pixmap"))
    .def("readPixels",
        py::overload_cast<const SkPixmap&, int, int>(&SkSurface::readPixels),
        R"docstring(
        Copies :py:class:`Rect` of pixels to dst.

        Source :py:class:`Rect` corners are (srcX, srcY) and :py:class:`Surface`
        (width(), height()). Destination :py:class:`Rect` corners are (0, 0) and
        (dst.width(), dst.height()). Copies each readable pixel intersecting
        both rectangles, without scaling, converting to dst.colorType() and
        dst.alphaType() if required.

        Pixels are readable when :py:class:`Surface` is raster, or backed by a
        GPU.

        The destination pixel storage must be allocated by the caller.

        Pixel values are converted only if :py:class:`ColorType` and
        :py:class:`AlphaType` do not match. Only pixels within both source and
        destination rectangles are copied. dst contents outside :py:class:`Rect`
        intersection are unchanged.

        Pass negative values for srcX or srcY to offset pixels across or down
        destination.

        Does not copy, and returns false if:

        - Source and destination rectangles do not intersect.
        - :py:class:`Pixmap` pixels could not be allocated.
        - dst.rowBytes() is too small to contain one row of pixels.

        :dst: storage for pixels copied from :py:class:`Surface`
        :srcX: offset into readable pixels on x-axis; may be negative
        :srcY: offset into readable pixels on y-axis; may be negative
        :return: true if pixels were copied
        )docstring",
        py::arg("dst"), py::arg("srcX") = 0, py::arg("srcY") = 0)
    .def("readPixels", &ReadPixels<SkSurface>,
        R"docstring(
        Copies :py:class:`Rect` of pixels from :py:class:`Canvas` into array.

        :py:class:`Matrix` and clip are ignored.

        Source :py:class:`Rect` corners are (srcX, srcY) and :py:class:`Surface`
        (width(), height()). Destination :py:class:`Rect` corners are (0, 0) and
        (array.shape[1], array.shape[0]). Copies each readable pixel
        intersecting both rectangles, without scaling, converting to
        :py:attr:`ColorType.kN32` and :py:attr:`AlphaType.kPremul` if required.

        Pixels are readable when :py:class:`BaseDevice` is raster, or backed by
        a GPU.

        The destination pixel storage must be allocated by the caller.

        Pixel values are converted only if :py:class:`ColorType` and
        :py:class:`AlphaType` do not match. Only pixels within both source and
        destination rectangles are copied. array contents outside
        :py:class:`Rect` intersection are unchanged.

        Pass negative values for srcX or srcY to offset pixels across or down
        destination.

        Does not copy, and returns false if:

        - Source and destination rectangles do not intersect.
        - :py:class:`Surface` pixels could not be converted to
            :py:attr:`ColorType.kN32` or :py:attr:`AlphaType.kPremul`.

        :dstInfo: width, height, :py:class:`ColorType`, and
            :py:class:`AlphaType` of dstPixels
        :dstPixels: storage for pixels; dstInfo.height() times dstRowBytes, or
            larger
        :dstRowBytes: size of one destination row; dstInfo.width() times pixel
            size, or larger. Ignored when dstPixels has more than one-dimension.
        :srcX: offset into readable pixels on x-axis; may be negative
        :srcY: offset into readable pixels on y-axis; may be negative
        :return: true if pixels were copied
        )docstring",
        py::arg("dstInfo"), py::arg("dstPixels"), py::arg("dstRowBytes") = 0,
        py::arg("srcX") = 0, py::arg("srcY") = 0)
    .def("readPixels",
        py::overload_cast<const SkBitmap&, int, int>(&SkSurface::readPixels),
        R"docstring(
        Copies :py:class:`Rect` of pixels from :py:class:`Surface` into bitmap.

        Source :py:class:`Rect` corners are (srcX, srcY) and :py:class:`Surface`
        (width(), height()). Destination :py:class:`Rect` corners are (0, 0) and
        (bitmap.width(), bitmap.height()). Copies each readable pixel
        intersecting both rectangles, without scaling, converting to
        bitmap.colorType() and bitmap.alphaType() if required.

        Pixels are readable when :py:class:`Surface` is raster, or backed by a
        GPU.

        The destination pixel storage must be allocated by the caller.

        Pixel values are converted only if :py:class:`ColorType` and
        :py:class:`AlphaType` do not match. Only pixels within both source and
        destination rectangles are copied. dst contents outside :py:class:`Rect`
        intersection are unchanged.

        Pass negative values for srcX or srcY to offset pixels across or down
        destination.

        Does not copy, and returns false if:

        - Source and destination rectangles do not intersect.
        - :py:class:`Surface` pixels could not be converted to dst.colorType()
            or dst.alphaType().
        - dst pixels could not be allocated.
        - dst.rowBytes() is too small to contain one row of pixels.

        :dst: storage for pixels copied from :py:class:`Surface`
        :srcX: offset into readable pixels on x-axis; may be negative
        :srcY: offset into readable pixels on y-axis; may be negative
        :return: true if pixels were copied
        )docstring",
        py::arg("dst"), py::arg("srcX"), py::arg("srcY"))
    // .def("asyncRescaleAndReadPixels", &SkSurface::asyncRescaleAndReadPixels,
    //     "Makes surface pixel data available to caller, possibly "
    //     "asynchronously.")
    // .def("asyncRescaleAndReadPixelsYUV420",
    //     &SkSurface::asyncRescaleAndReadPixelsYUV420,
    //     "Similar to asyncRescaleAndReadPixels but performs an additional "
    //     "conversion to YUV.")
    .def("writePixels",
        py::overload_cast<const SkPixmap&, int, int>(&SkSurface::writePixels),
        R"docstring(
        Copies :py:class:`Rect` of pixels from the src :py:class:`Pixmap` to the
        :py:class:`Surface`.

        Source :py:class:`Rect` corners are (0, 0) and (src.width(),
        src.height()). Destination :py:class:`Rect` corners are (dstX, dstY) and
        (dstX + Surface width(), dstY + Surface height()).

        Copies each readable pixel intersecting both rectangles, without
        scaling, converting to :py:class:`Surface` colorType() and
        :py:class:`Surface` alphaType() if required.

        :src: storage for pixels to copy to :py:class:`Surface`
        :dstX: x-axis position relative to :py:class:`Surface` to begin copy;
            may be negative
        :dstY: y-axis position relative to :py:class:`Surface` to begin copy;
            may be negative
        )docstring",
        py::arg("src"), py::arg("dstX") = 0, py::arg("dstY") = 0)
    .def("writePixels",
        py::overload_cast<const SkBitmap&, int, int>(&SkSurface::writePixels),
        R"docstring(
        Copies :py:class:`Rect` of pixels from the src :py:class:`Bitmap` to the
        :py:class:`Surface`.

        Source :py:class:`Rect` corners are (0, 0) and (src.width(),
        src.height()). Destination :py:class:`Rect` corners are (dstX, dstY) and
        (dstX + Surface width(), dstY + Surface height()).

        Copies each readable pixel intersecting both rectangles, without
        scaling, converting to :py:class:`Surface` colorType() and
        :py:class:`Surface` alphaType() if required.

        :src: storage for pixels to copy to :py:class:`Surface`
        :dstX: x-axis position relative to :py:class:`Surface` to begin copy;
            may be negative
        :dstY: y-axis position relative to :py:class:`Surface` to begin copy;
            may be negative
        )docstring",
        py::arg("src"), py::arg("dstX") = 0, py::arg("dstY") = 0)
    .def("props", &SkSurface::props,
        R"docstring(
        Returns :py:class:`SurfaceProps` for surface.

        :return: LCD striping orientation and setting for device independent
            fonts
        )docstring")
    .def("flushAndSubmit",
        py::overload_cast<>(&SkSurface::flushAndSubmit),
        R"docstring(
        Call to ensure all reads/writes of the surface have been issued to the
        underlying 3D API.

        Skia will correctly order its own draws and pixel operations. This must
        to be used to ensure correct ordering when the surface backing store is
        accessed outside Skia (e.g. direct use of the 3D API or a windowing
        system). :py:class:`GrContext` has additional flush and submit methods
        that apply to all surfaces and images created from a
        :py:class:`GrContext`. This is equivalent to calling :py:meth:`flush`
        with a default :py:class:`GrFlushInfo` followed by
        :py:meth:`GrContext.submit`.
        )docstring")
    .def("flush",
        py::overload_cast<SkSurface::BackendSurfaceAccess, const GrFlushInfo&>(
            &SkSurface::flush),
        R"docstring(
        Issues pending :py:class:`Surface` commands to the GPU-backed API
        objects and resolves any :py:class:`Surface` MSAA.

        A call to :py:meth:`GrContext.submit` is always required to ensure work
        is actually sent to the gpu. Some specific API details:

        :GL: Commands are actually sent to the driver, but glFlush is never
            called. Thus some sync objects from the flush will not be valid
            until a submission occurs.

        :Vulkan/Metal/D3D/Dawn: Commands are recorded to the backend APIs
            corresponding command buffer or encoder objects. However, these
            objects are not sent to the gpu until a submission occurs.

        The work that is submitted to the GPU will be dependent on the
        :py:class:`BackendSurfaceAccess` that is passed in.

        If :py:attr:`BackendSurfaceAccess.kNoAccess` is passed in all commands
        will be issued to the GPU.

        If :py:attr:`BackendSurfaceAccess.kPresent` is passed in and the backend
        API is not Vulkan, it is treated the same as kNoAccess. If the backend
        API is Vulkan, the VkImage that backs the :py:class:`Surface` will be
        transferred back to its original queue. If the :py:class:`Surface` was
        created by wrapping a VkImage, the queue will be set to the queue which
        was originally passed in on the GrVkImageInfo. Additionally, if the
        original queue was not external or foreign the layout of the VkImage
        will be set to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR.

        The :py:class:`GrFlushInfo` describes additional options to flush.
        Please see documentation at :py:class:`GrFlushInfo` for more info.

        If the return is :py:attr:`GrSemaphoresSubmitted.kYes`, only initialized
        :py:class:`GrBackendSemaphores` will be submitted to the gpu during the
        next submit call (it is possible Skia failed to create a subset of the
        semaphores). The client should not wait on these semaphores until after
        submit has been called, but must keep them alive until then. If a submit
        flag was passed in with the flush these valid semaphores can we waited
        on immediately. If this call returns
        :py:attr:`GrSemaphoresSubmitted.kNo`, the GPU backend will not submit
        any semaphores to be signaled on the GPU. Thus the client should not
        have the GPU wait on any of the semaphores passed in with the
        :py:class:`GrFlushInfo`. Regardless of whether semaphores were submitted
        to the GPU or not, the client is still responsible for deleting any
        initialized semaphores. Regardleess of semaphore submission the context
        will still be flushed. It should be emphasized that a return value of
        :py:attr:`GrSemaphoresSubmitted.kNo` does not mean the flush did not
        happen. It simply means there were no semaphores submitted to the GPU. A
        caller should only take this as a failure if they passed in semaphores
        to be submitted.

        Pending surface commands are flushed regardless of the return result.

        :param access:  type of access the call will do on the backend object
            after flush
        :param info:    flush options
        )docstring",
        py::arg("access"), py::arg("info"))
    .def("characterize", &SkSurface::characterize,
        R"docstring(
        Initializes :py:class:`SurfaceCharacterization` that can be used to
        perform GPU back-end processing in a separate thread.

        Typically this is used to divide drawing into multiple tiles.
        :py:class:`DeferredDisplayListRecorder` records the drawing commands for
        each tile.

        Return true if :py:class:`Surface` supports characterization. raster
        surface returns false.

        :param skia.SurfaceCharacterization characterization: properties for
            parallel drawing
        :return: true if supported
        )docstring",
        py::arg("characterization"))
    .def("draw",
        py::overload_cast<SkDeferredDisplayList*>(&SkSurface::draw),
        R"docstring(
        Draws deferred display list created using
        :py:class:`DeferredDisplayListRecorder`.

        Has no effect and returns false if :py:class:`SurfaceCharacterization`
        stored in deferredDisplayList is not compatible with
        :py:class:`Surface`.

        raster surface returns false.

        :deferredDisplayList: drawing commands
        :return: false if deferredDisplayList is not compatible
        )docstring",
        py::arg("deferredDisplayList"))
    .def("unique", &SkSurface::unique,
        R"docstring(
        May return true if the caller is the only owner.

        Ensures that all previous owner's actions are complete.
        )docstring")
    .def("ref", &SkSurface::ref,
        R"docstring(
        Increment the reference count.

        Must be balanced by a call to unref().
        )docstring")
    .def("unref", &SkSurface::unref,
        R"docstring(
        Decrement the reference count.

        If the reference count is 1 before the decrement, then delete the
        object. Note that if this is the case, then the object needs to have
        been allocated via new, and not on the stack.
        )docstring")
    .def_static("MakeRasterDirect",
        [](const SkImageInfo& image_info, py::buffer pixels, size_t rowBytes,
            const SkSurfaceProps* surfaceProps) {
            py::buffer_info info = pixels.request();
            size_t given_size = (info.ndim > 0) ?
                info.shape[0] * info.strides[0] : 0;
            rowBytes = (rowBytes == 0) ? image_info.minRowBytes() : rowBytes;
            auto required = rowBytes * image_info.height();
            if (given_size < required)
                throw std::runtime_error("Buffer is smaller than required");
            auto surface = SkSurface::MakeRasterDirect(
                image_info, info.ptr, rowBytes, surfaceProps);
            if (!surface)
                throw std::runtime_error("Failed to create Surface");
            return surface;
        },
        R"docstring(
        Allocates raster :py:class:`Surface`.

        :py:class:`Canvas` returned by :py:class:`Surface` draws directly into
        pixels.

        :py:class:`Surface` is returned if all parameters are valid. Valid
        parameters include: info dimensions are greater than zero; info contains
        :py:class:`ColorType` and :py:class:`AlphaType` supported by raster
        surface; pixels is not nullptr; rowBytes is large enough to contain info
        width pixels of :py:class:`ColorType`.

        Pixel buffer size should be info height times computed rowBytes. Pixels
        are not initialized. To access pixels after drawing, peekPixels() or
        readPixels().

        :param skia.ImageInfo imageInfo: width, height, :py:class:`ColorType`,
            :py:class:`AlphaType`, :py:class:`ColorSpace`, of raster surface;
            width and height must be greater than zero
        :param Union[bytes,bytearray,memoryview] pixels: pointer to destination
            pixels buffer
        :param rowBytes: interval from one :py:class:`Surface` row to the next
        :param surfaceProps: LCD striping orientation and setting for device
            independent fonts; may be nullptr

        :return: :py:class:`Surface` if all parameters are valid; otherwise,
            nullptr
        )docstring",
        py::arg("info"), py::arg("pixels"), py::arg("rowBytes") = 0,
        py::arg("surfaceProps") = nullptr)
    // .def_static("MakeRasterDirectReleaseProc",
    //     &SkSurface::MakeRasterDirectReleaseProc,
    //     "Allocates raster SkSurface.")
    .def_static("MakeRaster",
        py::overload_cast<const SkImageInfo&, size_t, const SkSurfaceProps*>(
            &SkSurface::MakeRaster),
        R"docstring(
        Allocates raster :py:class:`Surface`.

        :py:class:`Canvas` returned by :py:class:`Surface` draws directly into
        pixels. Allocates and zeroes pixel memory. Pixel memory size is
        imageInfo.height() times rowBytes, or times imageInfo.minRowBytes() if
        rowBytes is zero. Pixel memory is deleted when :py:class:`Surface` is
        deleted.

        :py:class:`Surface` is returned if all parameters are valid. Valid
        parameters include: info dimensions are greater than zero; info contains
        :py:class:`ColorType` and :py:class:`AlphaType` supported by raster
        surface; rowBytes is large enough to contain info width pixels of
        :py:class:`ColorType`, or is zero.

        If rowBytes is zero, a suitable value will be chosen internally.

        :imageInfo: width, height, :py:class:`ColorType`,
            :py:class:`AlphaType`, :py:class:`ColorSpace`, of raster surface;
            width and height must be greater than zero
        :rowBytes: interval from one :py:class:`Surface` row to the
            next; may be zero
        :skia.SurfaceProps surfaceProps: LCD striping orientation and
            setting for device independent fonts; may be nullptr
        :return: :py:class:`Surface` if all parameters are valid; otherwise,
            nullptr
        )docstring",
        py::arg("imageInfo"), py::arg("rowBytes") = 0,
        py::arg("surfaceProps") = nullptr)
    .def_static("MakeRasterN32Premul", &SkSurface::MakeRasterN32Premul,
        R"docstring(
        Allocates raster :py:class:`Surface`.

        :py:class:`Canvas` returned by :py:class:`Surface` draws directly into
        pixels. Allocates and zeroes pixel memory. Pixel memory size is height
        times width times four. Pixel memory is deleted when :py:class:`Surface`
        is deleted.

        Internally, sets :py:class:`ImageInfo` to width, height, native color
        type, and :py:attr:`AlphaType.kPremul`.

        :py:class:`Surface` is returned if width and height are greater than
        zero.

        Use to create :py:class:`Surface` that matches :py:class:`PMColor`, the
        native pixel arrangement on the platform. :py:class:`Surface` drawn to
        output device skips converting its pixel format.

        :param int width: pixel column count; must be greater than zero
        :param int height: pixel row count; must be greater than zero
        :param skia.SurfaceProps surfaceProps: LCD striping orientation and
            setting for device independent fonts; may be nullptr
        :return: :py:class:`Surface` if all parameters are valid; otherwise,
            nullptr
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("surfaceProps") = nullptr)
    .def_static("MakeFromBackendTexture",
        [] (GrContext* context, const GrBackendTexture& backendTexture,
            GrSurfaceOrigin origin, int sampleCnt, SkColorType colorType,
            sk_sp<SkColorSpace> colorSpace,
            const SkSurfaceProps* surfaceProps) {
            return SkSurface::MakeFromBackendTexture(
                context, backendTexture, origin, sampleCnt, colorType,
                colorSpace, surfaceProps);
        },
        R"docstring(
        Wraps a GPU-backed texture into :py:class:`Surface`.

        Caller must ensure the texture is valid for the lifetime of returned
        :py:class:`Surface`. If sampleCnt greater than zero, creates an
        intermediate MSAA :py:class:`Surface` which is used for drawing
        backendTexture.

        :py:class:`Surface` is returned if all parameters are valid.
        backendTexture is valid if its pixel configuration agrees with
        colorSpace and context; for instance, if backendTexture has an sRGB
        configuration, then context must support sRGB, and colorSpace must be
        present. Further, backendTexture width and height must not exceed
        context capabilities, and the context must be able to support back-end
        textures.

        Upon success textureReleaseProc is called when it is safe to delete the
        texture in the backend API (accounting only for use of the texture by
        this surface). If :py:class:`Surface` creation fails textureReleaseProc
        is called before this function returns.

        If SK_SUPPORT_GPU is defined as zero, has no effect and returns nullptr.

        :context: GPU context
        :backendTexture:  texture residing on GPU
        :sampleCnt:  samples per pixel, or 0 to disable full scene anti-aliasing
        :colorSpace:  range of colors; may be nullptr
        :surfaceProps:    LCD striping orientation and setting for device
            independent fonts; may be nullptr
        :textureReleaseProc:  function called when texture can be released
        :releaseContext:  state passed to textureReleaseProc
        :return: :py:class:`Surface` if all parameters are valid; otherwise,
            nullptr
        )docstring",
        py::arg("context"), py::arg("backendTexture"), py::arg("origin"),
        py::arg("sampleCnt"), py::arg("colorType"), py::arg("colorSpace"),
        py::arg("surfaceProps"))
    .def_static("MakeFromBackendTexture",
        [] (GrContext* context,
            const SkSurfaceCharacterization& characterzation,
            const GrBackendTexture& backendTexture) {
            return SkSurface::MakeFromBackendTexture(
                context, characterzation, backendTexture);
        },
        R"docstring(
        Wraps a backend texture in an :py:class:`Surface` - setting up the
        surface to match the provided characterization.

        The caller must ensure the texture is valid for the lifetime of returned
        :py:class:`Surface`.

        If the backend texture and surface characterization are incompatible
        then null will be returned.

        Usually, the :py:meth:`GrContext.createBackendTexture` variant that
        takes a surface characterization should be used to create the backend
        texture. If not, :py:meth:`SurfaceCharacterization.isCompatible` can be
        used to determine if a given backend texture is compatible with a
        specific surface characterization.

        Upon success textureReleaseProc is called when it is safe to delete the
        texture in the backend API (accounting only for use of the texture by
        this surface). If :py:class:`Surface` creation fails textureReleaseProc
        is called before this function returns.

        :context: GPU context
        :characterization:    characterization of the desired surface
        :backendTexture:  texture residing on GPU
        :return: :py:class:`Surface` if all parameters are compatible;
            otherwise, nullptr
        )docstring",
        py::arg("context"), py::arg("characterization"),
        py::arg("backendTexture"))
    .def_static("MakeFromBackendRenderTarget",
        [] (GrContext* context, const GrBackendRenderTarget& target,
            GrSurfaceOrigin origin, SkColorType colorType,
            sk_sp<SkColorSpace> colorSpace,
            const SkSurfaceProps* surfaceProps) {
            return SkSurface::MakeFromBackendRenderTarget(
                context, target, origin, colorType, colorSpace, surfaceProps);
        },
        R"docstring(
        Wraps a GPU-backed buffer into :py:class:`Surface`.

        Caller must ensure backendRenderTarget is valid for the lifetime of
        returned :py:class:`Surface`.

        :py:class:`Surface` is returned if all parameters are valid.
        backendRenderTarget is valid if its pixel configuration agrees with
        colorSpace and context; for instance, if backendRenderTarget has an sRGB
        configuration, then context must support sRGB, and colorSpace must be
        present. Further, backendRenderTarget width and height must not exceed
        context capabilities, and the context must be able to support back-end
        render targets.

        Upon success releaseProc is called when it is safe to delete the render
        target in the backend API (accounting only for use of the render target
        by this surface). If :py:class:`Surface` creation fails releaseProc is
        called before this function returns.

        If SK_SUPPORT_GPU is defined as zero, has no effect and returns nullptr.

        :param context: GPU context
        :param backendRenderTarget: GPU intermediate memory buffer
        :param colorSpace:  range of colors
        :param surfaceProps:    LCD striping orientation and setting for device
            independent fonts; may be nullptr
        :return: :py:class:`Surface` if all parameters are valid; otherwise,
            nullptr
        )docstring",
        py::arg("context"), py::arg("backendRenderTarget"), py::arg("origin"),
        py::arg("colorType"), py::arg("colorSpace"),
        py::arg("surfaceProps") = nullptr)
    .def_static("MakeRenderTarget",
        py::overload_cast<GrContext*, SkBudgeted, const SkImageInfo&, int,
        GrSurfaceOrigin, const SkSurfaceProps*, bool>(
            &SkSurface::MakeRenderTarget),
        R"docstring(
        Returns :py:class:`Surface` on GPU indicated by context.

        Allocates memory for pixels, based on the width, height, and
        :py:class:`ColorType` in :py:class:`ImageInfo`. budgeted selects whether
        allocation for pixels is tracked by context. imageInfo describes the
        pixel format in :py:class:`ColorType`, and transparency in
        :py:class:`AlphaType`, and color matching in :py:class:`ColorSpace`.

        sampleCount requests the number of samples per pixel. Pass zero to
        disable multi-sample anti-aliasing. The request is rounded up to the
        next supported count, or rounded down if it is larger than the maximum
        supported count.

        surfaceOrigin pins either the top-left or the bottom-left corner to the
        origin.

        shouldCreateWithMips hints that :py:class:`Image` returned by
        makeImageSnapshot() is mip map.

        If SK_SUPPORT_GPU is defined as zero, has no effect and returns nullptr.

        :param skia.GrContext context: GPU context
        :param skia.ImageInfo imageInfo: width, height, :py:class:`ColorType`,
            :py:class:`AlphaType`, :py:class:`ColorSpace`; width, or height, or
            both, may be zero
        :param int sampleCount: samples per pixel, or 0 to disable full scene
            anti-aliasing
        :param skia.GrSurfaceOrigin surfaceOrigin: kTopLeft or kBottomLeft
        :param skia.SurfaceProps surfaceProps: LCD striping orientation and
            setting for device independent fonts; may be nullptr
        :param shouldCreateWithMips: hint that :py:class:`Surface` will host mip
            map images
        :return: :py:class:`Surface` if all parameters are valid; otherwise,
            nullptr
        )docstring",
        py::arg("context"), py::arg("budgeted"), py::arg("imageInfo"),
        py::arg("sampleCount") = 0,
        py::arg("surfaceOrigin") = GrSurfaceOrigin::kBottomLeft_GrSurfaceOrigin,
        py::arg("surfaceProps") = nullptr,
        py::arg("shouldCreateWithMips") = false)
    .def_static("MakeRenderTarget",
        py::overload_cast<GrContext*, SkBudgeted, const SkImageInfo&, int,
            const SkSurfaceProps*>(&SkSurface::MakeRenderTarget),
        R"docstring(
        Returns :py:class:`Surface` on GPU indicated by context.

        Allocates memory for pixels, based on the width, height, and
        :py:class:`ColorType` in :py:class:`ImageInfo`. budgeted selects whether
        allocation for pixels is tracked by context. imageInfo describes the
        pixel format in :py:class:`ColorType`, and transparency in
        :py:class:`AlphaType`, and color matching in :py:class:`ColorSpace`.

        sampleCount requests the number of samples per pixel. Pass zero to
        disable multi-sample anti-aliasing. The request is rounded up to the
        next supported count, or rounded down if it is larger than the maximum
        supported count.

        :py:class:`Surface` bottom-left corner is pinned to the origin.

        :context: GPU context
        :imageInfo:   width, height, :py:class:`ColorType`,
            :py:class:`AlphaType`, :py:class:`ColorSpace`; width, or height, or
            both, may be zero
        :sampleCount: samples per pixel, or 0 to disable full scene
            anti-aliasing
        :surfaceProps:    LCD striping orientation and setting for device
            independent fonts; may be nullptr
        :return: :py:class:`Surface` if all parameters are valid; otherwise,
            nullptr
        )docstring",
        py::arg("context"), py::arg("budgeted"), py::arg("imageInfo"),
        py::arg("sampleCount"), py::arg("surfaceProps"))
    .def_static("MakeRenderTarget",
        py::overload_cast<GrContext*, SkBudgeted, const SkImageInfo&>(
            &SkSurface::MakeRenderTarget),
        R"docstring(
        Returns :py:class:`Surface` on GPU indicated by context.

        Allocates memory for pixels, based on the width, height, and
        :py:class:`ColorType` in :py:class:`ImageInfo`. budgeted selects whether
        allocation for pixels is tracked by context. imageInfo describes the
        pixel format in :py:class:`ColorType`, and transparency in
        :py:class:`AlphaType`, and color matching in :py:class:`ColorSpace`.

        :py:class:`Surface` bottom-left corner is pinned to the origin.

        :context: GPU context
        :imageInfo:   width, height, :py:class:`ColorType`,
            :py:class:`AlphaType`, :py:class:`ColorSpace`; width, or height, or
            both, may be zero
        )docstring",
        py::arg("context"), py::arg("budgeted"), py::arg("imageInfo"))
    .def_static("MakeRenderTarget",
        py::overload_cast<GrRecordingContext*,
        const SkSurfaceCharacterization&, SkBudgeted>(
            &SkSurface::MakeRenderTarget),
        R"docstring(
        Returns :py:class:`Surface` on GPU indicated by context that is
        compatible with the provided characterization.

        budgeted selects whether allocation for pixels is tracked by context.

        :context: GPU context
        :characterization: description of the desired :py:class:`Surface`
        :return: :py:class:`Surface` if all parameters are valid; otherwise,
            nullptr
        )docstring",
        py::arg("context"), py::arg("characterization"), py::arg("budgeted"))
    .def_static("MakeNull", &SkSurface::MakeNull,
        R"docstring(
        Returns :py:class:`Surface` without backing pixels.

        Drawing to :py:class:`Canvas` returned from :py:class:`Surface` has no
        effect. Calling makeImageSnapshot() on returned :py:class:`Surface`
        returns nullptr.

        :param int width: one or greater
        :param int height: one or greater

        :return: :py:class:`Surface` if width and height are positive;
            otherwise, nullptr
        )docstring",
        py::arg("width"), py::arg("height"))
    ;
}
