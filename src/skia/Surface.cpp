#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

const SkSurfaceProps::Flags SkSurfaceProps::kUseDistanceFieldFonts_Flag;

template<typename T>
using NumPy = py::array_t<T, py::array::c_style | py::array::forcecast>;

void initSurface(py::module &m) {
py::enum_<SkPixelGeometry>(m, "PixelGeometry")
    .value("kUnknown", SkPixelGeometry::kUnknown_SkPixelGeometry)
    .value("kRGB_H", SkPixelGeometry::kRGB_H_SkPixelGeometry)
    .value("kBGR_H", SkPixelGeometry::kBGR_H_SkPixelGeometry)
    .value("kRGB_V", SkPixelGeometry::kRGB_V_SkPixelGeometry)
    .value("kBGR_V", SkPixelGeometry::kBGR_V_SkPixelGeometry)
    .export_values();

py::class_<SkSurfaceProps> surfaceprops(m, "SurfaceProps", R"docstring(
    Describes properties and constraints of a given SkSurface.

    The rendering engine can parse these during drawing, and can sometimes
    optimize its performance (e.g. disabling an expensive feature).
    )docstring");

py::enum_<SkSurfaceProps::Flags>(surfaceprops, "Flags")
    .value("kUseDeviceIndependentFonts",
        SkSurfaceProps::Flags::kUseDeviceIndependentFonts_Flag)
    .export_values();

py::enum_<SkSurfaceProps::InitType>(surfaceprops, "InitType")
    .value("kLegacyFontHost",
        SkSurfaceProps::InitType::kLegacyFontHost_InitType)
    .export_values();

surfaceprops
    .def(py::init<uint32_t, SkPixelGeometry>())
    .def(py::init<SkSurfaceProps::InitType>())
    .def(py::init<uint32_t, SkSurfaceProps::InitType>())
    .def(py::init<const SkSurfaceProps&>())
    .def("flags", &SkSurfaceProps::flags)
    .def("pixelGeometry", &SkSurfaceProps::pixelGeometry)
    .def("isUseDeviceIndependentFonts",
        &SkSurfaceProps::isUseDeviceIndependentFonts)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def_readonly_static("kUseDistanceFieldFonts_Flag",
        &SkSurfaceProps::kUseDistanceFieldFonts_Flag)
    ;

py::class_<SkSurfaceCharacterization>(m, "SurfaceCharacterization")
    .def(py::init())
    .def("createResized", &SkSurfaceCharacterization::createResized)
    .def("createColorSpace", &SkSurfaceCharacterization::createColorSpace)
    .def("createBackendFormat", &SkSurfaceCharacterization::createBackendFormat)
    .def("createFBO0", &SkSurfaceCharacterization::createFBO0)
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
    SkSurface is responsible for managing the pixels that a canvas draws into.

    The pixels can be allocated either in CPU memory (a raster surface) or on
    the GPU (a GrRenderTarget surface). SkSurface takes care of allocating a
    SkCanvas that will draw into the surface. Call surface->getCanvas() to use
    that canvas (but don't delete it, it is owned by the surface). SkSurface
    always has non-zero dimensions. If there is a request for a new surface, and
    either of the requested dimensions are zero, then nullptr will be returned.
    )docstring");

py::class_<SkSurface::AsyncReadResult>(surface, "AsyncReadResult", R"docstring(
    The result from asyncRescaleAndReadPixels() or
    asyncRescaleAndReadPixelsYUV420().
    )docstring");

py::enum_<SkSurface::ContentChangeMode>(surface, "ContentChangeMode")
    .value("kDiscard", SkSurface::ContentChangeMode::kDiscard_ContentChangeMode,
        "discards surface on change")
    .value("kRetain", SkSurface::ContentChangeMode::kRetain_ContentChangeMode,
        "preserves surface on change")
    .export_values();

py::enum_<SkSurface::BackendHandleAccess>(surface, "BackendHandleAccess")
    .value("kFlushRead",
        SkSurface::BackendHandleAccess::kFlushRead_BackendHandleAccess,
        "back-end object is readable")
    .value("kFlushWrite",
        SkSurface::BackendHandleAccess::kFlushWrite_BackendHandleAccess,
        "back-end object is writable")
    .value("kDiscardWrite",
        SkSurface::BackendHandleAccess::kDiscardWrite_BackendHandleAccess,
        "back-end object must be overwritten")
    .export_values();

py::enum_<SkSurface::RescaleGamma>(surface, "RescaleGamma")
    .value("kSrc", SkSurface::RescaleGamma::kSrc)
    .value("kLinear", SkSurface::RescaleGamma::kLinear)
    .export_values();

py::enum_<SkSurface::BackendSurfaceAccess>(surface, "BackendSurfaceAccess")
    .value("kNoAccess", SkSurface::BackendSurfaceAccess::kNoAccess,
        "back-end object will not be used by client")
    .value("kPresent", SkSurface::BackendSurfaceAccess::kPresent,
        "back-end surface will be used for presenting to screen")
    .export_values();

py::enum_<SkSurface::FlushFlags>(surface, "FlushFlags")
    .value("kNone_FlushFlags", SkSurface::FlushFlags::kNone_FlushFlags)
    .value("kSyncCpu_FlushFlag", SkSurface::FlushFlags::kSyncCpu_FlushFlag)
    .export_values();

surface
    .def(py::init(&SkSurface::MakeRasterN32Premul),
        py::arg("width"), py::arg("height"), py::arg("surfaceProps") = nullptr)
    .def(py::init([](NumPy<uint8_t> array) {
        py::buffer_info info = array.request();
        if (info.ndim <= 1)
            throw std::runtime_error("Number of dimensions must be 2 or more.");
        if (info.shape[0] == 0 || info.shape[1] == 0)
            throw std::runtime_error(
                "Width and height must be greater than 0.");
        return SkSurface::MakeRasterDirect(
            SkImageInfo::MakeN32Premul(info.shape[1], info.shape[0], nullptr),
            info.ptr, info.strides[0]);
    }))
    // .def("isCompatible", &SkSurface::isCompatible,
    //     "Is this surface compatible with the provided characterization?")
    .def("width", &SkSurface::width,
        "Returns pixel count in each row; may be zero or greater.")
    .def("height", &SkSurface::height,
        "Returns pixel row count; may be zero or greater.")
    .def("imageInfo", &SkSurface::imageInfo,
        "Returns an ImageInfo describing the surface.")
    .def("generationID", &SkSurface::generationID,
        "Returns unique value identifying the content of SkSurface.")
    // .def("notifyContentWillChange", &SkSurface::notifyContentWillChange,
    //     "Notifies that SkSurface contents will be changed by code outside of "
    //     "Skia.")
    // .def("getBackendTexture", &SkSurface::getBackendTexture,
    //     "Retrieves the back-end texture.")
    // .def("getBackendRenderTarget", &SkSurface::getBackendRenderTarget,
    //     "Retrieves the back-end render target.")
    // .def("replaceBackendTexture", &SkSurface::replaceBackendTexture,
    //     "If the surface was made via MakeFromBackendTexture then it's "
    //     "backing texture may be substituted with a different texture.")
    .def("getCanvas", &SkSurface::getCanvas,
        py::return_value_policy::reference)
    .def("makeSurface",
        py::overload_cast<const SkImageInfo&>(&SkSurface::makeSurface),
        "Returns a compatible SkSurface, or nullptr.")
    .def("makeSurface",
        py::overload_cast<int, int>(&SkSurface::makeSurface),
        "Calls makeSurface(ImageInfo) with the same ImageInfo as this surface, "
        "but with the specified width and height.")
    .def("makeImageSnapshot",
        py::overload_cast<>(&SkSurface::makeImageSnapshot),
        "Returns SkImage capturing SkSurface contents.")
    .def("makeImageSnapshot",
        py::overload_cast<const SkIRect &>(&SkSurface::makeImageSnapshot),
        "Like the no-parameter version, this returns an image of the current "
        "surface contents.")
    .def("draw",
        py::overload_cast<SkCanvas*, SkScalar, SkScalar, const SkPaint*>(
            &SkSurface::draw),
        "Draws SkSurface contents to canvas, with its top-left corner at "
        "(x, y).")
    .def("peekPixels", &SkSurface::peekPixels,
        "Copies SkSurface pixel address, row bytes, and SkImageInfo to "
        "SkPixmap, if address is available, and returns true.")
    .def("readPixels",
        py::overload_cast<const SkPixmap&, int, int>(&SkSurface::readPixels),
        "Copies SkRect of pixels to dst.")
    .def("readPixels",
        py::overload_cast<const SkImageInfo&, void*, size_t, int, int>(
            &SkSurface::readPixels),
        "Copies SkRect of pixels from SkCanvas into dstPixels.")
    .def("readPixels",
        py::overload_cast<const SkBitmap&, int, int>(&SkSurface::readPixels),
        "Copies SkRect of pixels from SkSurface into bitmap.")
    // .def("asyncRescaleAndReadPixels", &SkSurface::asyncRescaleAndReadPixels,
    //     "Makes surface pixel data available to caller, possibly "
    //     "asynchronously.")
    // .def("asyncRescaleAndReadPixelsYUV420",
    //     &SkSurface::asyncRescaleAndReadPixelsYUV420,
    //     "Similar to asyncRescaleAndReadPixels but performs an additional "
    //     "conversion to YUV.")
    .def("writePixels",
        py::overload_cast<const SkPixmap&, int, int>(&SkSurface::writePixels),
        "Copies SkRect of pixels from the src SkPixmap to the SkSurface.")
    .def("writePixels",
        py::overload_cast<const SkBitmap&, int, int>(&SkSurface::writePixels),
        "Copies SkRect of pixels from the src SkBitmap to the SkSurface.")
    .def("props", &SkSurface::props,
        "Returns SkSurfaceProps for surface.")
    .def("flush",
        py::overload_cast<>(&SkSurface::flush),
        "Issues pending SkSurface commands to the GPU-backed API and resolves "
        "any SkSurface MSAA.")
    .def("flush",
        py::overload_cast<>(&SkSurface::flush),
        "Issues pending SkSurface commands to the GPU-backed API and resolves "
        "any SkSurface MSAA.")
    // .def("flush",
    //     py::overload_cast<BackendSurfaceAccess, const GrFlushInfo&>(
    //         &SkSurface::flush),
    //     "Issues pending SkSurface commands to the GPU-backed API and resolves "
    //     "any SkSurface MSAA.")
    // .def("flush",
    //     py::overload_cast<BackendSurfaceAccess, GrFlushFlags, int,
    //     GrBackendSemaphore[], GrGpuFinishedProc, GrGpuFinishedContext>(
    //         &SkSurface::flush),
    //     "Deprecated.")
    // .def("flush",
    //     py::overload_cast<BackendSurfaceAccess, FlushFlags, int,
    //     GrBackendSemaphore[]>(&SkSurface::flush))
    // .def("flushAndSignalSemaphores", &SkSurface::flushAndSignalSemaphores,
    //     "Deprecated.")
    // .def("wait", &SkSurface::wait,
    //     "Inserts a list of GPU semaphores that the current GPU-backed API "
    //     "must wait on before executing any more commands on the GPU for this "
    //     "surface.")
    .def("characterize", &SkSurface::characterize,
        "Initializes SkSurfaceCharacterization that can be used to perform "
        "GPU back-end processing in a separate thread.")
    .def("draw",
        py::overload_cast<SkDeferredDisplayList*>(&SkSurface::draw),
        "Draws deferred display list created using "
        "SkDeferredDisplayListRecorder.")
    .def("unique", &SkSurface::unique,
        "May return true if the caller is the only owner.")
    .def("ref", &SkSurface::ref,
        "Increment the reference count.")
    .def("unref", &SkSurface::unref,
        "Decrement the reference count.")
    .def_static("MakeRasterDirect",
        [](const SkImageInfo& image_info, NumPy<uint8_t> array,
            const SkSurfaceProps* surfaceProps) {
            py::buffer_info info = array.request();
            if (info.format != py::format_descriptor<uint8_t>::format())
                throw std::runtime_error("Incompatible format.");
            return SkSurface::MakeRasterDirect(
                image_info, info.ptr, info.strides[0], surfaceProps);
        }, "Allocates raster SkSurface.",
        py::arg("info"), py::arg("buf"), py::arg("surfaceProps") = nullptr)
    // .def_static("MakeRasterDirectReleaseProc",
    //     &SkSurface::MakeRasterDirectReleaseProc,
    //     "Allocates raster SkSurface.")
    .def_static("MakeRaster",
        py::overload_cast<const SkImageInfo&, size_t, const SkSurfaceProps*>(
            &SkSurface::MakeRaster),
        "Allocates raster SkSurface.")
    .def_static("MakeRaster",
        py::overload_cast<const SkImageInfo&, const SkSurfaceProps*>(
            &SkSurface::MakeRaster),
        "Allocates raster SkSurface.")
    .def_static("MakeRasterN32Premul", &SkSurface::MakeRasterN32Premul,
        "Allocates raster SkSurface.")
    // .def_static("MakeFromBackendTexture", &SkSurface::MakeFromBackendTexture,
    //     "Wraps a GPU-backed texture into SkSurface.")
    // .def_static("MakeFromBackendRenderTarget",
    //     &SkSurface::MakeFromBackendRenderTarget,
    //     "Wraps a GPU-backed buffer into SkSurface.")
    // .def_static("MakeFromBackendTextureAsRenderTarget",
    //     &SkSurface::MakeFromBackendTextureAsRenderTarget,
    //     "Wraps a GPU-backed texture into SkSurface.")
    // .def_static("MakeRenderTarget",
    //     py::overload_cast<GrContext*, SkBudgeted, const SkImageInfo&, int,
    //     GrSurfaceOrigin, const SkSurfaceProps*, bool>(
    //         &SkSurface::MakeRenderTarget),
    //     "Returns SkSurface on GPU indicated by context.")
    // .def_static("MakeRenderTarget",
    //     py::overload_cast<GrContext*, SkBudgeted, const SkImageInfo&, int,
    //     const SkSurfaceProps*>(&SkSurface::MakeRenderTarget),
    //     "Returns SkSurface on GPU indicated by context.")
    // .def_static("MakeRenderTarget",
    //     py::overload_cast<GrContext*, SkBudgeted, const SkImageInfo&>(
    //         &SkSurface::MakeRenderTarget),
    //     "Returns SkSurface on GPU indicated by context.")
    // .def_static("MakeRenderTarget",
    //     py::overload_cast<GrRecordingContext*,
    //     const SkSurfaceCharacterization&, SkBudgeted>(
    //         &SkSurface::MakeRenderTarget),
    //     "Returns SkSurface on GPU indicated by context that is compatible with "
    //     "the provided characterization.")
    // .def_static("MakeFromBackendTexture", &SkSurface::MakeFromBackendTexture,
    //     "Wraps a backend texture in an SkSurface - setting up the surface to "
    //     "match the provided characterization.")
    .def_static("MakeNull", &SkSurface::MakeNull,
        "Returns SkSurface without backing pixels.")
    ;
}
