#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

void initGrContext(py::module &m) {
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
