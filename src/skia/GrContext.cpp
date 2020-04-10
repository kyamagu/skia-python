#include "common.h"

void initGrContext(py::module &m) {

py::enum_<GrBackendApi>(m, "GrBackendApi", R"docstring(
    Possible 3D APIs that may be used by Ganesh.
    )docstring")
    .value("kOpenGL", GrBackendApi::kOpenGL)
    .value("kVulkan", GrBackendApi::kVulkan)
    .value("kMetal", GrBackendApi::kMetal)
    .value("kDirect3D", GrBackendApi::kDirect3D)
    .value("kDawn", GrBackendApi::kDawn)
    .value("kMock", GrBackendApi::kMock,
        "Mock is a backend that does not draw anything. It is used for unit "
        "tests and to measure CPU overhead.")
    .value("kOpenGL_GrBackend", GrBackendApi::kOpenGL_GrBackend,
        "Added here to support the legacy GrBackend enum value and clients "
        "who referenced it using GrBackend::kOpenGL_GrBackend.")
    .export_values();

py::enum_<GrMipMapped>(m, "GrMipMapped", R"docstring(
    Used to say whether a texture has mip levels allocated or not.
    )docstring")
    .value("kNo", GrMipMapped::kNo)
    .value("kYes", GrMipMapped::kYes)
    .export_values();

py::enum_<GrRenderable>(m, "GrRenderable")
    .value("kNo", GrRenderable::kNo)
    .value("kYes", GrRenderable::kYes)
    .export_values();

py::enum_<GrProtected>(m, "GrProtected")
    .value("kNo", GrProtected::kNo)
    .value("kYes", GrProtected::kYes)
    .export_values();

py::enum_<GrSurfaceOrigin>(m, "GrSurfaceOrigin", R"docstring(
    GPU SkImage and SkSurfaces can be stored such that (0, 0) in texture space
    may correspond to either the top-left or bottom-left content pixel.
    )docstring")
    .value("kTopLeft", GrSurfaceOrigin::kTopLeft_GrSurfaceOrigin)
    .value("kBottomLeft", GrSurfaceOrigin::kBottomLeft_GrSurfaceOrigin)
    .export_values();

py::enum_<GrGLBackendState>(m, "GrGLBackendState", R"docstring(
    A GrContext's cache of backend context state can be partially invalidated.

    These enums are specific to the GL backend and we'd add a new set for an
    alternative backend.
    )docstring")
    .value("kRenderTarget", GrGLBackendState::kRenderTarget_GrGLBackendState)
    .value("kTextureBinding",
        GrGLBackendState::kTextureBinding_GrGLBackendState)
    .value("kView", GrGLBackendState::kView_GrGLBackendState)
    .value("kBlend", GrGLBackendState::kBlend_GrGLBackendState)
    .value("kMSAAEnable", GrGLBackendState::kMSAAEnable_GrGLBackendState)
    .value("kVertex", GrGLBackendState::kVertex_GrGLBackendState)
    .value("kStencil", GrGLBackendState::kStencil_GrGLBackendState)
    .value("kPixelStore", GrGLBackendState::kPixelStore_GrGLBackendState)
    .value("kProgram", GrGLBackendState::kProgram_GrGLBackendState)
    .value("kFixedFunction", GrGLBackendState::kFixedFunction_GrGLBackendState)
    .value("kMisc", GrGLBackendState::kMisc_GrGLBackendState)
    .value("kPathRendering", GrGLBackendState::kPathRendering_GrGLBackendState)
    .value("kALL", GrGLBackendState::kALL_GrGLBackendState)
    .export_values();

py::enum_<GrFlushFlags>(m, "GrFlushFlags", R"docstring(
    Enum used as return value when flush with semaphores so the client knows
    whether the semaphores were submitted to GPU or not.
    )docstring")
    .value("kNone", GrFlushFlags::kNone_GrFlushFlags)
    .value("kSyncCpu", GrFlushFlags::kSyncCpu_GrFlushFlag)
    .export_values();

py::enum_<GrSemaphoresSubmitted>(m, "GrSemaphoresSubmitted")
    .value("kNo", GrSemaphoresSubmitted::kNo)
    .value("kYes", GrSemaphoresSubmitted::kYes)
    .export_values();

py::class_<GrBackendSemaphore>(m, "GrBackendSemaphore")
    .def(py::init())
    // .def("initGL", &GrBackendSemaphore::initGL)
    // .def("initVulkan", &GrBackendSemaphore::initVulkan)
    // .def("initMetal", &GrBackendSemaphore::initMetal)
    .def("isInitialized", &GrBackendSemaphore::isInitialized)
    // .def("glSync", &GrBackendSemaphore::glSync)
    // .def("vkSemaphore", &GrBackendSemaphore::vkSemaphore)
    // .def("mtlSemaphore", &GrBackendSemaphore::mtlSemaphore)
    // .def("mtlValue", &GrBackendSemaphore::mtlValue)
    ;

py::class_<GrBackendTexture>(m, "GrBackendTexture")
    .def(py::init())
    .def("isValid", &GrBackendTexture::isValid)
    ;

py::class_<sk_sp<const GrGLInterface>>(m, "GrGLInterface")
    .def(py::init([] {
        auto interface = GrGLMakeNativeInterface();
        if (!interface.get())
            throw std::runtime_error("null pointer exception.");
        return interface;
    }));

py::class_<GrContext, sk_sp<GrContext>>(m, "GrContext")
    .def("resetContext", &GrContext::resetContext,
        "The GrContext normally assumes that no outsider is setting state "
        "within the underlying 3D API's context/device/whatever.")
    .def("resetGLTextureBindings", &GrContext::resetGLTextureBindings,
        "If the backend is GrBackendApi::kOpenGL, then all texture unit/target "
        "combinations for which the GrContext has modified the bound texture "
        "will have texture id 0 bound.")
    .def("abandonContext", &GrContext::abandonContext,
        "Abandons all GPU resources and assumes the underlying backend 3D API "
        "context is no longer usable.")
    .def("releaseResourcesAndAbandonContext",
        &GrContext::releaseResourcesAndAbandonContext,
        "This is similar to abandonContext() however the underlying 3D "
        "context is not yet lost and the GrContext will cleanup all allocated "
        "resources before returning.")
    .def("getResourceCacheLimits", &GrContext::getResourceCacheLimits,
        "DEPRECATED Return the current GPU resource cache limits.")
    .def("getResourceCacheLimit", &GrContext::getResourceCacheLimit,
        "Return the current GPU resource cache limit in bytes.")
    .def("getResourceCacheUsage", &GrContext::getResourceCacheUsage,
        "Gets the current GPU resource cache usage.")
    .def("getResourceCachePurgeableBytes",
        &GrContext::getResourceCachePurgeableBytes,
        "Gets the number of bytes in the cache consumed by purgeable (e.g. "
        "unlocked) resources.")
    .def("setResourceCacheLimits", &GrContext::setResourceCacheLimits,
        "DEPRECATED Specify the GPU resource cache limits.")
    .def("setResourceCacheLimit", &GrContext::setResourceCacheLimit,
        "Specify the GPU resource cache limit.")
    .def("freeGpuResources", &GrContext::freeGpuResources,
        "Frees GPU created by the context.")
    .def("performDeferredCleanup", &GrContext::performDeferredCleanup,
        "Purge GPU resources that haven't been used in the past 'msNotUsed' "
        "milliseconds or are otherwise marked for deletion, regardless of "
        "whether the context is under budget.")
    .def("purgeResourcesNotUsedInMs", &GrContext::purgeResourcesNotUsedInMs)
    .def("purgeUnlockedResources",
        py::overload_cast<size_t, bool>(&GrContext::purgeUnlockedResources),
        "Purge unlocked resources from the cache until the the provided byte "
        "count has been reached or we have purged all unlocked resources.")
    .def("purgeUnlockedResources",
        py::overload_cast<bool>(&GrContext::purgeUnlockedResources),
        "This entry point is intended for instances where an app has been "
        "backgrounded or suspended.")
    .def("maxTextureSize", &GrContext::maxTextureSize,
        "Gets the maximum supported texture size.")
    .def("maxRenderTargetSize", &GrContext::maxRenderTargetSize,
        "Gets the maximum supported render target size.")
    .def("colorTypeSupportedAsImage", &GrContext::colorTypeSupportedAsImage,
        "Can a SkImage be created with the given color type.")
    .def("colorTypeSupportedAsSurface", &GrContext::colorTypeSupportedAsSurface,
        "Can a SkSurface be created with the given color type.")
    .def("maxSurfaceSampleCountForColorType",
        &GrContext::maxSurfaceSampleCountForColorType,
        "Gets the maximum supported sample count for a color type.")
    // .def("wait", &GrContext::wait,
    //     "Inserts a list of GPU semaphores that the current GPU-backed API "
    //     "must wait on before executing any more commands on the GPU.")
    .def("flush", py::overload_cast<>(&GrContext::flush),
        "Call to ensure all drawing to the context has been issued to the "
        "underlying 3D API.")
    // .def("flush", py::overload_cast<const GrFlushInfo&>(&GrContext::flush),
    //     "Call to ensure all drawing to the context has been issued to the "
    //     "underlying 3D API.")
    // .def("flush",
    //     py::overload_cast<const GrFlushInfo&,
    //         const GrPrepareForExternalIORequests &>(&GrContext::flush),
    //     "Call to ensure all drawing to the context has been issued to the "
    //     "underlying 3D API.")
    // .def("flush",
    //     py::overload_cast<GrFlushFlags, int, GrBackendSemaphore[],
    //         GrGpuFinishedProc, GrGpuFinishedContext>(&GrContext::flush),
    //     "Deprecated.")
    // .def("flushAndSignalSemaphores", &GrContext::flushAndSignalSemaphores,
    //     "Deprecated.")
    .def("checkAsyncWorkCompletion", &GrContext::checkAsyncWorkCompletion,
        "Checks whether any asynchronous work is complete and if so calls "
        "related callbacks.")
    // .def("priv", (GrContextPriv (GrContext::*)()) &GrContext::priv)
    // .def("priv", (const GrContextPriv (GrContext::*)() const) &GrContext::priv)
    // .def("dumpMemoryStatistics", &GrContext::dumpMemoryStatistics,
    //     "Enumerates all cached GPU resources and dumps their memory to "
    //     "traceMemoryDump.")
    .def("supportsDistanceFieldText", &GrContext::supportsDistanceFieldText)
    .def("storeVkPipelineCacheData", &GrContext::storeVkPipelineCacheData)
    // .def("defaultBackendFormat", &GrContext::defaultBackendFormat)
    // .def("createBackendTexture",
    //     (GrBackendTexture (GrContext::*)(int, int, const GrBackendFormat&,
    //         GrMipMapped, GrRenderable, GrProtected))
    //     &GrContext::createBackendTexture)
    // .def("createBackendTexture",
    //     (GrBackendTexture (GrContext::*)(int, int, SkColorType, GrMipMapped,
    //         GrRenderable, GrProtected)) &GrContext::createBackendTexture)
    // .def("createBackendTexture",
    //     (GrBackendTexture (GrContext::*)(const SkSurfaceCharacterization &))
    //     &GrContext::createBackendTexture)
    // .def("createBackendTexture",
    //     (GrBackendTexture (GrContext::*)(int, int, const GrBackendFormat&,
    //         const SkColor4f&, GrMipMapped, GrRenderable, GrProtected))
    //     &GrContext::createBackendTexture)
    // .def("createBackendTexture",
    //     (GrBackendTexture (GrContext::*)(int, int, SkColorType,
    //         const SkColor4f&, GrMipMapped, GrRenderable, GrProtected))
    //     &GrContext::createBackendTexture)
    // .def("createBackendTexture",
    //     (GrBackendTexture (GrContext::*)(const SkSurfaceCharacterization&,
    //         const SkColor4f &)) &GrContext::createBackendTexture)
    // .def("createBackendTexture",
    //     (GrBackendTexture (GrContext::*)(const SkPixmap[], int, GrRenderable,
    //         GrProtected)) &GrContext::createBackendTexture)
    // .def("createBackendTexture",
    //     (GrBackendTexture (GrContext::*)(const SkPixmap&, GrRenderable,
    //         GrProtected isProtected)) &GrContext::createBackendTexture)
    // .def("compressedBackendFormat", &GrContext::compressedBackendFormat)
    // .def("createCompressedBackendTexture",
    //     (GrBackendTexture (GrContext::*)(int, int, const GrBackendFormat&,
    //         const SkColor4f&, GrMipMapped, GrProtected))
    //     &GrContext::createCompressedBackendTexture)
    // .def("createCompressedBackendTexture",
    //     (GrBackendTexture (GrContext::*)(int, int, SkImage::CompressionType,
    //         const SkColor4f&, GrMipMapped, GrProtected))
    //     &GrContext::createCompressedBackendTexture)
    // .def("createCompressedBackendTexture",
    //     (GrBackendTexture (GrContext::*)(int, int, const GrBackendFormat&,
    //         const void*, size_t, GrMipMapped, GrProtected))
    //     &GrContext::createCompressedBackendTexture)
    // .def("createCompressedBackendTexture",
    //     (GrBackendTexture (GrContext::*)(int, int, SkImage::CompressionType,
    //         const void*, size_t, GrMipMapped, GrProtected))
    //     &GrContext::createCompressedBackendTexture)
    // .def("deleteBackendTexture", &GrContext::deleteBackendTexture)
    .def("precompileShader", &GrContext::precompileShader)
    .def_static("MakeGL",
        py::overload_cast<sk_sp<const GrGLInterface>, const GrContextOptions&>(
            &GrContext::MakeGL))
    .def_static("MakeGL",
        py::overload_cast<sk_sp<const GrGLInterface>>(&GrContext::MakeGL))
    .def_static("MakeGL",
        py::overload_cast<const GrContextOptions&>(&GrContext::MakeGL))
    .def_static("MakeGL", py::overload_cast<>(&GrContext::MakeGL))
    // .def_static("MakeVulkan",
    //     (sk_sp<GrContext> (*)(const GrVkBackendContext&,
    //         const GrContextOptions&)) &GrContext::MakeVulkan,
    //     "The Vulkan context (VkQueue, VkDevice, VkInstance) must be kept "
    //     "alive until the returned GrContext is first destroyed or abandoned.")
    // .def_static("MakeVulkan",
    //     (sk_sp<GrContext> (*)(const GrVkBackendContext&))
    //     &GrContext::MakeVulkan)
    // .def_static("MakeMock",
    //     (sk_sp<GrContext> (*)(const GrMockOptions*, const GrContextOptions&))
    //     &GrContext::MakeMock)
    // .def_static("MakeMock",
    //     (sk_sp<GrContext> (*)(const GrMockOptions*)) &GrContext::MakeMock)
    // .def_static("ComputeImageSize", &GrContext::ComputeImageSize)
    ;
}
