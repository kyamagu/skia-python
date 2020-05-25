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
        R"docstring(
        Mock is a backend that does not draw anything. It is used for unit tests
        and to measure CPU overhead.
        )docstring")
    .value("kOpenGL_GrBackend", GrBackendApi::kOpenGL_GrBackend,
        R"docstring(
        Added here to support the legacy GrBackend enum value and clients who
        referenced it using :py:attr:`~skia.GrBackend.kOpenGL_GrBackend`.
        )docstring")
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
    .value("kTopLeft_GrSurfaceOrigin",
        GrSurfaceOrigin::kTopLeft_GrSurfaceOrigin)
    .value("kBottomLeft_GrSurfaceOrigin",
        GrSurfaceOrigin::kBottomLeft_GrSurfaceOrigin)
    .export_values();

py::enum_<GrGLBackendState>(m, "GrGLBackendState", R"docstring(
    A GrContext's cache of backend context state can be partially invalidated.

    These enums are specific to the GL backend and we'd add a new set for an
    alternative backend.
    )docstring")
    .value("kRenderTarget_GrGLBackendState",
        GrGLBackendState::kRenderTarget_GrGLBackendState)
    .value("kTextureBinding_GrGLBackendState",
        GrGLBackendState::kTextureBinding_GrGLBackendState)
    .value("kView_GrGLBackendState",
        GrGLBackendState::kView_GrGLBackendState)
    .value("kBlend_GrGLBackendState",
        GrGLBackendState::kBlend_GrGLBackendState)
    .value("kMSAAEnable_GrGLBackendState",
        GrGLBackendState::kMSAAEnable_GrGLBackendState)
    .value("kVertex_GrGLBackendState",
        GrGLBackendState::kVertex_GrGLBackendState)
    .value("kStencil_GrGLBackendState",
        GrGLBackendState::kStencil_GrGLBackendState)
    .value("kPixelStore_GrGLBackendState",
        GrGLBackendState::kPixelStore_GrGLBackendState)
    .value("kProgram_GrGLBackendState",
        GrGLBackendState::kProgram_GrGLBackendState)
    .value("kFixedFunction_GrGLBackendState",
        GrGLBackendState::kFixedFunction_GrGLBackendState)
    .value("kMisc_GrGLBackendState",
        GrGLBackendState::kMisc_GrGLBackendState)
    .value("kPathRendering_GrGLBackendState",
        GrGLBackendState::kPathRendering_GrGLBackendState)
    .value("kALL_GrGLBackendState",
        GrGLBackendState::kALL_GrGLBackendState)
    .export_values();

py::enum_<GrFlushFlags>(m, "GrFlushFlags", R"docstring(
    Enum used as return value when flush with semaphores so the client knows
    whether the semaphores were submitted to GPU or not.
    )docstring")
    .value("kNone_GrFlushFlags", GrFlushFlags::kNone_GrFlushFlags)
    .value("kSyncCpu_GrFlushFlag", GrFlushFlags::kSyncCpu_GrFlushFlag)
    .export_values();

py::enum_<GrSemaphoresSubmitted>(m, "GrSemaphoresSubmitted")
    .value("kNo", GrSemaphoresSubmitted::kNo)
    .value("kYes", GrSemaphoresSubmitted::kYes)
    .export_values();

py::enum_<GrColorType>(m, "GrColorType")
    .value("kUnknown", GrColorType::kUnknown)
    .value("kAlpha_8", GrColorType::kAlpha_8)
    .value("kBGR_565", GrColorType::kBGR_565)
    .value("kABGR_4444", GrColorType::kABGR_4444)
    .value("kRGBA_8888", GrColorType::kRGBA_8888)
    .value("kRGBA_8888_SRGB", GrColorType::kRGBA_8888_SRGB)
    .value("kRGB_888x", GrColorType::kRGB_888x)
    .value("kRG_88", GrColorType::kRG_88)
    .value("kBGRA_8888", GrColorType::kBGRA_8888)
    .value("kRGBA_1010102", GrColorType::kRGBA_1010102)
    .value("kGray_8", GrColorType::kGray_8)
    .value("kAlpha_F16", GrColorType::kAlpha_F16)
    .value("kRGBA_F16", GrColorType::kRGBA_F16)
    .value("kRGBA_F16_Clamped", GrColorType::kRGBA_F16_Clamped)
    .value("kRGBA_F32", GrColorType::kRGBA_F32)
    .value("kAlpha_16", GrColorType::kAlpha_16)
    .value("kRG_1616", GrColorType::kRG_1616)
    .value("kRG_F16", GrColorType::kRG_F16)
    .value("kRGBA_16161616", GrColorType::kRGBA_16161616)
    .value("kAlpha_8xxx", GrColorType::kAlpha_8xxx)
    .value("kAlpha_F32xxx", GrColorType::kAlpha_F32xxx)
    .value("kGray_8xxx", GrColorType::kGray_8xxx)
    .value("kRGB_888", GrColorType::kRGB_888)
    .value("kR_8", GrColorType::kR_8)
    .value("kR_16", GrColorType::kR_16)
    .value("kR_F16", GrColorType::kR_F16)
    .value("kGray_F16", GrColorType::kGray_F16)
    .value("kLast", GrColorType::kLast)
    .export_values();

py::enum_<GrTextureType>(m, "GrTextureType")
    .value("kNone", GrTextureType::kNone)
    .value("k2D", GrTextureType::k2D)
    .value("kRectangle", GrTextureType::kRectangle)
    .value("kExternal", GrTextureType::kExternal)
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

py::class_<GrBackendFormat>(m, "GrBackendFormat")
    .def(py::init<>())
    .def(py::init<const GrBackendFormat&>())
    .def_static("MakeGL", &GrBackendFormat::MakeGL)
    .def_static("MakeMock", &GrBackendFormat::MakeMock)
    .def("backend", &GrBackendFormat::backend)
    .def("textureType", &GrBackendFormat::textureType)
    // .def("channelMask", &GrBackendFormat::channelMask)
    .def("asGLFormat", &GrBackendFormat::asGLFormat)
    .def("asMockColorType", &GrBackendFormat::asMockColorType)
    .def("asMockCompressionType", &GrBackendFormat::asMockCompressionType)
    .def("makeTexture2D", &GrBackendFormat::makeTexture2D)
    .def("isValid", &GrBackendFormat::isValid)
    ;

py::class_<GrGLTextureInfo>(m, "GrGLTextureInfo")
    .def(py::init<>())
    .def_readwrite("fTarget", &GrGLTextureInfo::fTarget)
    .def_readwrite("fID", &GrGLTextureInfo::fID)
    .def_readwrite("fFormat", &GrGLTextureInfo::fFormat)
    .def("__eq__", &GrGLTextureInfo::operator==, py::is_operator())
    ;

py::class_<GrMockTextureInfo>(m, "GrMockTextureInfo")
    .def(py::init<>())
    .def(py::init<GrColorType, SkImage::CompressionType, int>())
    .def("__eq__", &GrMockTextureInfo::operator==, py::is_operator())
    ;

py::class_<GrBackendTexture>(m, "GrBackendTexture")
    .def(py::init<>())
    .def(py::init<int, int, GrMipMapped, const GrGLTextureInfo&>(),
        py::arg("width"), py::arg("height"), py::arg("mipMapped"),
        py::arg("glInfo"))
    .def(py::init<int, int, GrMipMapped, const GrMockTextureInfo&>(),
        py::arg("width"), py::arg("height"), py::arg("mipMapped"),
        py::arg("mockInfo"))
    .def("dimensions", &GrBackendTexture::dimensions)
    .def("width", &GrBackendTexture::width)
    .def("height", &GrBackendTexture::height)
    .def("hasMipMaps", &GrBackendTexture::hasMipMaps)
    .def("backend", &GrBackendTexture::backend)
    .def("getGLTextureInfo", &GrBackendTexture::getGLTextureInfo)
    .def("glTextureParametersModified",
        &GrBackendTexture::glTextureParametersModified)
    .def("getBackendFormat", &GrBackendTexture::getBackendFormat)
    .def("getMockTextureInfo", &GrBackendTexture::getMockTextureInfo)
    .def("isProtected", &GrBackendTexture::isProtected)
    .def("isValid", &GrBackendTexture::isValid)
    .def("isSameTexture", &GrBackendTexture::isSameTexture)
    ;

py::class_<GrGLInterface, sk_sp<GrGLInterface>, SkRefCnt>(
    m, "GrGLInterface")
    .def(py::init([] {
        sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
        if (!interface.get())
            throw std::runtime_error("null pointer exception.");
        const GrGLInterface* ptr = interface.release();
        return const_cast<GrGLInterface*>(ptr);
    }));

py::class_<GrContext, sk_sp<GrContext>, SkRefCnt>(m, "GrContext")
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
    .def("defaultBackendFormat", &GrContext::defaultBackendFormat,
        R"docstring(
        Retrieve the default GrBackendFormat for a given :py:class:`ColorType`
        and renderability.

        It is guaranteed that this backend format will be the one used by the
        following :py:class:`ColorType` and SurfaceCharacterization-based
        :py:meth:`createBackendTexture` methods.

        The caller should check that the returned format is valid.
        )docstring",
        py::arg("colorType"), py::arg("renderable") = GrRenderable::kNo)
    // .def("createBackendTexture",
    //     (GrBackendTexture (GrContext::*)(int, int, const GrBackendFormat&,
    //         GrMipMapped, GrRenderable, GrProtected))
    //     &GrContext::createBackendTexture)
    .def("createBackendTexture",
        py::overload_cast<int, int, SkColorType, GrMipMapped,
            GrRenderable, GrProtected>(&GrContext::createBackendTexture),
        R"docstring(
        If possible, create an uninitialized backend texture.

        The client should ensure that the returned backend texture is valid. If
        successful, the created backend texture will be compatible with the
        provided :py:class:`ColorType`. For the Vulkan backend the layout of the
        created VkImage will be: VK_IMAGE_LAYOUT_UNDEFINED.
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("colorType"),
        py::arg("mipMapped"), py::arg("renderable"),
        py::arg("protected") = GrProtected::kNo)
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
    .def("compressedBackendFormat", &GrContext::compressedBackendFormat,
        R"docstring(
        Retrieve the :py:class:`GrBackendFormat` for a given
        :py:class:`Image.CompressionType`.

        This is guaranteed to match the backend format used by the following
        :py:meth:`createCompressedsBackendTexture` methods that take a
        CompressionType. The caller should check that the returned format is
        valid.
        )docstring",
        py::arg("compressionType"))

    .def("createCompressedBackendTexture",
        [] (GrContext& context, int width, int height,
            const GrBackendFormat& backendFormat, const SkColor4f& color,
            GrMipMapped mipMapped) {
            return context.createCompressedBackendTexture(
                width, height, backendFormat, color, mipMapped);
        },
        py::arg("width"), py::arg("height"), py::arg("backendFormat"),
        py::arg("color"), py::arg("mipMapped"))
    .def("createCompressedBackendTexture",
        [] (GrContext& context, int width, int height,
            SkImage::CompressionType type, const SkColor4f& color,
            GrMipMapped mipMapped) {
            return context.createCompressedBackendTexture(
                width, height, type, color, mipMapped);
        },
        py::arg("width"), py::arg("height"), py::arg("type"), py::arg("color"),
        py::arg("mipMapped"))
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
