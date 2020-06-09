#include "common.h"
#include <pybind11/chrono.h>
#include <pybind11/stl.h>

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
    )docstring", py::arithmetic())
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

py::class_<GrFlushInfo>(m, "GrFlushInfo",
    R"docstring(
    Struct to supply options to flush calls.

    After issuing all commands, fNumSemaphore semaphores will be signaled by the
    gpu. The client passes in an array of fNumSemaphores GrBackendSemaphores. In
    general these :py:class:`GrBackendSemaphore`'s can be either initialized or
    not. If they are initialized, the backend uses the passed in semaphore. If
    it is not initialized, a new semaphore is created and the
    :py:class:`GrBackendSemaphore` object is initialized with that semaphore.

    The client will own and be responsible for deleting the underlying
    semaphores that are stored and returned in initialized
    :py:class:`GrBackendSemaphore` objects. The :py:class:`GrBackendSemaphore`
    objects themselves can be deleted as soon as this function returns.

    If a finishedProc is provided, the finishedProc will be called when all work
    submitted to the gpu from this flush call and all previous flush calls has
    finished on the GPU. If the flush call fails due to an error and nothing
    ends up getting sent to the GPU, the finished proc is called immediately.
    )docstring")
    .def(py::init<>())
    .def_readwrite("fFlags", &GrFlushInfo::fFlags)
    .def_readonly("fNumSemaphores", &GrFlushInfo::fNumSemaphores)
    .def_property("semaphores",
        [] (const GrFlushInfo& info) -> py::object {
            auto begin = info.fSignalSemaphores;
            if (!begin)
                return py::none();
            return py::cast(std::vector<GrBackendSemaphore>(
                begin, begin + info.fNumSemaphores));
        },
        [] (GrFlushInfo& info,
            std::vector<GrBackendSemaphore>& semaphores) {
            info.fNumSemaphores = semaphores.size();
            info.fSignalSemaphores = (semaphores.size()) ?
                &semaphores[0] : (GrBackendSemaphore*) nullptr;
        })
    ;

py::class_<GrContextOptions>(m, "GrContextOptions")
    .def(py::init<>())
    // TODO: Implement me!
    ;

py::class_<GrMockOptions>(m, "GrMockOptions")
    .def(py::init<>())
    // TODO: Implement me!
    ;

py::class_<GrContext, sk_sp<GrContext>, SkRefCnt>(m, "GrContext")
    .def("resetContext", &GrContext::resetContext,
        R"docstring(
        The :py:class:`GrContext` normally assumes that no outsider is setting
        state within the underlying 3D API's context/device/whatever.

        This call informs the context that the state was modified and it should
        resend. Shouldn't be called frequently for good performance. The flag
        bits, state, is dependent on which backend is used by the context,
        either GL or D3D (possible in future).
        )docstring",
        py::arg("state") = kAll_GrBackendState)
    .def("resetGLTextureBindings", &GrContext::resetGLTextureBindings,
        R"docstring(
        If the backend is :py:attr:`~GrBackendApi.kOpenGL`, then all texture
        unit/target combinations for which the GrContext has modified the bound
        texture will have texture id 0 bound.

        This does not flush the :py:class:`GrContext`. Calling
        :py:meth:`resetContext` does not change the set that will be bound to
        texture id 0 on the next call to :py:meth:`resetGLTextureBindings`.
        After this is called all unit/target combinations are considered to have
        unmodified bindings until the :py:class:`GrContext` subsequently
        modifies them (meaning if this is called twice in a row with no
        intervening :py:class:`GrContext` usage then the second call is a
        no-op.)
        )docstring")
    .def("abandonContext", &GrContext::abandonContext,
        R"docstring(
        Abandons all GPU resources and assumes the underlying backend 3D API
        context is no longer usable.

        Call this if you have lost the associated GPU context, and thus internal
        texture, buffer, etc. references/IDs are now invalid. Calling this
        ensures that the destructors of the :py:class:`GrContext` and any of its
        created resource objects will not make backend 3D API calls. Content
        rendered but not previously flushed may be lost. After this function is
        called all subsequent calls on the GrContext will fail or be no-ops.

        The typical use case for this function is that the underlying 3D context
        was lost and further API calls may crash.

        For Vulkan, even if the device becomes lost, the VkQueue, VkDevice, or
        VkInstance used to create the GrContext must be alive before calling
        abandonContext.
        )docstring")
    .def("releaseResourcesAndAbandonContext",
        &GrContext::releaseResourcesAndAbandonContext,
        R"docstring(
        This is similar to :py:meth:`abandonContext` however the underlying 3D
        context is not yet lost and the :py:class:`GrContext` will cleanup all
        allocated resources before returning.

        After returning it will assume that the underlying context may no longer
        be valid.

        The typical use case for this function is that the client is going to
        destroy the 3D context but can't guarantee that :py:class:`GrContext`
        will be destroyed first (perhaps because it may be ref'ed elsewhere by
        either the client or Skia objects).

        For Vulkan, even if the device becomes lost, the VkQueue, VkDevice, or
        VkInstance used to create the GrContext must be alive before calling
        :py:meth:`releaseResourcesAndAbandonContext`.
        )docstring")
    .def("getResourceCacheLimit", &GrContext::getResourceCacheLimit,
        R"docstring(
        Return the current GPU resource cache limit in bytes.
        )docstring")
    .def("getResourceCacheUsage", &GrContext::getResourceCacheUsage,
        R"docstring(
        Gets the current GPU resource cache usage.

        :param int resourceCount: If non-null, returns the number of resources
            that are held in the cache.
        :param int maxResourceBytes: If non-null, returns the total number of
            bytes of video memory held in the cache.
        )docstring",
        py::arg("resourceCount") = nullptr,
        py::arg("maxResourceBytes") = nullptr)
    .def("getResourceCachePurgeableBytes",
        &GrContext::getResourceCachePurgeableBytes,
        R"docstring(
        Gets the number of bytes in the cache consumed by purgeable (e.g.
        unlocked) resources.
        )docstring")
    .def("setResourceCacheLimit", &GrContext::setResourceCacheLimit,
        R"docstring(
        Specify the GPU resource cache limit.

        If the cache currently exceeds this limit, it will be purged (LRU) to
        keep the cache within the limit.

        :param maxResourceBytes: The maximum number of bytes of video memory
            that can be held in the cache.
        )docstring",
        py::arg("maxResourceBytes"))
    .def("freeGpuResources", &GrContext::freeGpuResources,
        R"docstring(
        Frees GPU created by the context.

        Can be called to reduce GPU memory pressure.
        )docstring")
    .def("performDeferredCleanup", &GrContext::performDeferredCleanup,
        R"docstring(
        Purge GPU resources that haven't been used in the past 'msNotUsed'
        milliseconds or are otherwise marked for deletion, regardless of whether
        the context is under budget.
        )docstring",
        py::arg("msNotUsed"))
    .def("purgeResourcesNotUsedInMs", &GrContext::purgeResourcesNotUsedInMs,
        py::arg("msNotUsed"))
    .def("purgeUnlockedResources",
        py::overload_cast<size_t, bool>(&GrContext::purgeUnlockedResources),
        R"docstring(
        Purge unlocked resources from the cache until the the provided byte
        count has been reached or we have purged all unlocked resources.

        The default policy is to purge in LRU order, but can be overridden to
        prefer purging scratch resources (in LRU order) prior to purging other
        resource types.

        :maxBytesToPurge: the desired number of bytes to be purged.
        :preferScratchResources:  If true scratch resources will be purged prior
            to other resource types.
        )docstring",
        py::arg("maxBytesToPurge"), py::arg("preferScratchResources"))
    .def("purgeUnlockedResources",
        py::overload_cast<bool>(&GrContext::purgeUnlockedResources),
        R"docstring(
        This entry point is intended for instances where an app has been
        backgrounded or suspended.

        If 'scratchResourcesOnly' is true all unlocked scratch resources will be
        purged but the unlocked resources with persistent data will remain. If
        'scratchResourcesOnly' is false then all unlocked resources will be
        purged. In either case, after the unlocked resources are purged a
        separate pass will be made to ensure that resource usage is under budget
        (i.e., even if 'scratchResourcesOnly' is true some resources with
        persistent data may be purged to be under budget).

        :scratchResourcesOnly: If true only unlocked scratch resources will be
            purged prior enforcing the budget requirements.
        )docstring",
        py::arg("scratchResourcesOnly"))
    .def("maxTextureSize", &GrContext::maxTextureSize,
        R"docstring(
        Gets the maximum supported texture size.
        )docstring")
    .def("maxRenderTargetSize", &GrContext::maxRenderTargetSize,
        R"docstring(
        Gets the maximum supported render target size.
        )docstring")
    .def("colorTypeSupportedAsImage", &GrContext::colorTypeSupportedAsImage,
        R"docstring(
        Can a :py:class:`Image` be created with the given color type.
        )docstring",
        py::arg("colorType"))
    .def("colorTypeSupportedAsSurface", &GrContext::colorTypeSupportedAsSurface,
        R"docstring(
        Can a SkSurface be created with the given color type.

        To check whether MSAA is supported use
        :py:meth:`maxSurfaceSampleCountForColorType`.
        )docstring",
        py::arg("colorType"))
    .def("maxSurfaceSampleCountForColorType",
        &GrContext::maxSurfaceSampleCountForColorType,
        R"docstring(
        Gets the maximum supported sample count for a color type.

        1 is returned if only non-MSAA rendering is supported for the color
        type. 0 is returned if rendering to this color type is not supported at
        all.
        )docstring",
        py::arg("colorType"))
    .def("wait",
        [] (GrContext& context,
            const std::vector<GrBackendSemaphore>& semaphores) {
            return context.wait(semaphores.size(), &semaphores[0]);
        },
        R"docstring(
        Inserts a list of GPU semaphores that the current GPU-backed API must
        wait on before executing any more commands on the GPU.

        Skia will take ownership of the underlying semaphores and delete them
        once they have been signaled and waited on. If this call returns false,
        then the GPU back-end will not wait on any passed in semaphores, and the
        client will still own the semaphores.
        )docstring",
        py::arg("semaphores"))
    .def("flush", py::overload_cast<const GrFlushInfo&>(&GrContext::flush),
        R"docstring(
        Call to ensure all drawing to the context has been flushed to underlying
        3D API specific objects.

        A call to :py:meth:`submit` is always required to ensure work is
        actually sent to the gpu. Some specific API details: GL: Commands are
        actually sent to the driver, but glFlush is never called. Thus some sync
        objects from the flush will not be valid until a submission occurs.

        Vulkan/Metal/D3D/Dawn: Commands are recorded to the backend APIs
        corresponding command buffer or encoder objects. However, these objects
        are not sent to the gpu until a submission occurs.

        If the return is :py:attr:`GrSemaphoresSubmitted.kYes`, only
        initialized GrBackendSemaphores will be submitted to the gpu during the
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
        )docstring",
        py::arg("info"))
    .def("flush", py::overload_cast<>(&GrContext::flush),
        R"docstring(
        Call to ensure all drawing to the context has been flushed to underlying
        3D API specific objects.

        This is equivalent to calling :py:meth:`flush` with a default
        :py:class:`GrFlushInfo`.
        )docstring")
    // .def("flushAndSubmit", &GrContext::flushAndSubmit,
    //     R"docstring(
    //     Call to ensure all drawing to the context has been flushed and submitted
    //     to the underlying 3D API.

    //     This is equivalent to calling :py:meth:`flush` with a default
    //     :py:class:`GrFlushInfo` followed by :py:meth:`submit`.
    //     )docstring")
    // .def("submit", &GrContext::submit,
    //     R"docstring(
    //     Submit outstanding work to the gpu from all previously un-submitted
    //     flushes.

    //     The return value of the submit will indicate whether or not the
    //     submission to the GPU was successful.

    //     If the call returns true, all previously passed in semaphores in flush
    //     calls will have been submitted to the GPU and they can safely be waited
    //     on. The caller should wait on those semaphores or perform some other
    //     global synchronization before deleting the semaphores.

    //     If it returns false, then those same semaphores will not have been
    //     submitted and we will not try to submit them again. The caller is free
    //     to delete the semaphores at any time.

    //     If the syncCpu flag is true this function will return once the gpu has
    //     finished with all submitted work.
    //     )docstring",
    //     py::arg("syncCpu") = false)
    .def("checkAsyncWorkCompletion", &GrContext::checkAsyncWorkCompletion,
        R"docstring(
        Checks whether any asynchronous work is complete and if so calls related
        callbacks.
        )docstring")
    // .def("priv", (GrContextPriv (GrContext::*)()) &GrContext::priv)
    // .def("priv", (const GrContextPriv (GrContext::*)() const) &GrContext::priv)
    // .def("dumpMemoryStatistics", &GrContext::dumpMemoryStatistics,
    //     "Enumerates all cached GPU resources and dumps their memory to "
    //     "traceMemoryDump.")
    .def("supportsDistanceFieldText", &GrContext::supportsDistanceFieldText)
    .def("storeVkPipelineCacheData", &GrContext::storeVkPipelineCacheData)
    .def("defaultBackendFormat", &GrContext::defaultBackendFormat,
        R"docstring(
        Retrieve the default :py:class:`GrBackendFormat` for a given
        :py:class:`ColorType` and renderability.

        It is guaranteed that this backend format will be the one used by the
        following :py:class:`ColorType` and SurfaceCharacterization-based
        :py:meth:`createBackendTexture` methods.

        The caller should check that the returned format is valid.
        )docstring",
        py::arg("colorType"), py::arg("renderable") = GrRenderable::kNo)
    .def("createBackendTexture",
        py::overload_cast<int, int, const GrBackendFormat&, GrMipMapped,
            GrRenderable, GrProtected>(&GrContext::createBackendTexture),
        R"docstring(
        The explicitly allocated backend texture API allows clients to use Skia
        to create backend objects outside of Skia proper (i.e., Skia's caching
        system will not know about them.)

        It is the client's responsibility to delete all these objects (using
        :py:meth:`deleteBackendTexture`) before deleting the
        :py:class:`GrContext` used to create them. If the backend is Vulkan, the
        textures must be deleted before abandoning the :py:class:`GrContext` as
        well. Additionally, clients should only delete these objects on the
        thread for which that :py:class:`GrContext` is active.

        The client is responsible for ensuring synchronization between different
        uses of the backend object (i.e., wrapping it in a surface, rendering to
        it, deleting the surface, rewrapping it in a image and drawing the image
        will require explicit sychronization on the client's part). If possible,
        create an uninitialized backend texture. The client should ensure that
        the returned backend texture is valid. For the Vulkan backend the layout
        of the created VkImage will be: VK_IMAGE_LAYOUT_UNDEFINED.
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("backendFormat"),
        py::arg("mipMapped"), py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo)
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
        py::arg("isProtected") = GrProtected::kNo)
    .def("createBackendTexture",
        py::overload_cast<const SkSurfaceCharacterization &>(
            &GrContext::createBackendTexture),
        R"docstring(
        If possible, create an uninitialized backend texture that is compatible
        with the provided characterization.

        The client should ensure that the returned backend texture is valid. For
        the Vulkan backend the layout of the created VkImage will be:
        VK_IMAGE_LAYOUT_UNDEFINED.
        )docstring",
        py::arg("characterization"))
    .def("createBackendTexture",
        [] (GrContext& context, int width, int height,
            const GrBackendFormat& backendFormat, const SkColor4f& color,
            GrMipMapped mipMapped, GrRenderable renderable,
            GrProtected isProtected) {
            return context.createBackendTexture(
                width, height, backendFormat, color, mipMapped, renderable,
                isProtected);
        },
        R"docstring(
        If possible, create a backend texture initialized to a particular color.

        The client should ensure that the returned backend texture is valid.
        For the Vulkan backend the layout of the created VkImage will be:
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("backendFormat"),
        py::arg("color"), py::arg("mipMapped"), py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo)
    .def("createBackendTexture",
        [] (GrContext& context, int width, int height,
            SkColorType colorType, const SkColor4f& color,
            GrMipMapped mipMapped, GrRenderable renderable,
            GrProtected isProtected) {
            return context.createBackendTexture(
                width, height, colorType, color, mipMapped, renderable,
                isProtected);
        },
        R"docstring(
        If possible, create a backend texture initialized to a particular color.

        The client should ensure that the returned backend texture is valid.
        If successful, the created backend texture will be compatible with the
        provided SkColorType. For the Vulkan backend the layout of the created
        VkImage will be: VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("colorType"),
        py::arg("color"), py::arg("mipMapped"), py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo)
    .def("createBackendTexture",
        [] (GrContext& context,
            const SkSurfaceCharacterization& characterization,
            const SkColor4f& color) {
            return context.createBackendTexture(characterization, color);
        },
        R"docstring(
        If possible, create a backend texture initialized to a particular color
        that is compatible with the provided characterization.

        The client should ensure that the returned backend texture is valid.
        For the Vulkan backend the layout of the created VkImage will be:
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL if texturaeble
        VK_IMAGE_LAYOUT_UNDEFINED if not textureable
        )docstring",
        py::arg("characterization"), py::arg("color"))
    .def("createBackendTexture",
        [] (GrContext& context, const std::vector<SkPixmap>& srcData,
            GrRenderable renderable, GrProtected isProtected) {
            return context.createBackendTexture(
                (srcData.empty()) ? nullptr : &srcData[0],
                srcData.size(),
                renderable,
                isProtected);
        },
        py::arg("srcData"),  py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo)
    .def("createBackendTexture",
        [] (GrContext& context, const SkPixmap& pixmap, GrRenderable renderable,
            GrProtected isProtected) {
            return context.createBackendTexture(
                pixmap, renderable, isProtected);
        },
        py::arg("pixmap"), py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo)
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
            GrMipMapped mipMapped, GrProtected isProtected) {
            return context.createCompressedBackendTexture(
                width, height, backendFormat, color, mipMapped, isProtected);
        },
        R"docstring(
        If possible, create a compressed backend texture initialized to a
        particular color.

        The client should ensure that the returned backend texture is valid.
        For the Vulkan backend the layout of the created VkImage will be:
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("backendFormat"),
        py::arg("color"), py::arg("mipMapped"),
        py::arg("isProtected") = GrProtected::kNo)
    .def("createCompressedBackendTexture",
        [] (GrContext& context, int width, int height,
            SkImage::CompressionType type, const SkColor4f& color,
            GrMipMapped mipMapped, GrProtected isProtected) {
            return context.createCompressedBackendTexture(
                width, height, type, color, mipMapped, isProtected);
        },
        py::arg("width"), py::arg("height"), py::arg("type"), py::arg("color"),
        py::arg("mipMapped"), py::arg("isProtected") = GrProtected::kNo)
    .def("createCompressedBackendTexture",
        [] (GrContext& context, int width, int height,
            const GrBackendFormat& backendFormat, py::buffer b,
            GrMipMapped mipMapped, GrProtected isProtected) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return context.createCompressedBackendTexture(
                width, height, backendFormat, info.ptr, size, mipMapped,
                isProtected);
        },
        py::arg("width"), py::arg("height"), py::arg("backendFormat"),
        py::arg("data"), py::arg("mipMapped"),
        py::arg("isProtected") = GrProtected::kNo)
    .def("createCompressedBackendTexture",
        [] (GrContext& context, int width, int height,
            SkImage::CompressionType type, py::buffer b,
            GrMipMapped mipMapped, GrProtected isProtected) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return context.createCompressedBackendTexture(
                width, height, type, info.ptr, size, mipMapped, isProtected);
        },
        py::arg("width"), py::arg("height"), py::arg("type"), py::arg("data"),
        py::arg("mipMapped"), py::arg("isProtected") = GrProtected::kNo)
    .def("deleteBackendTexture", &GrContext::deleteBackendTexture,
        py::arg("texture"))
    .def("precompileShader", &GrContext::precompileShader,
        py::arg("key"), py::arg("data"))
    .def_static("ComputeImageSize", &GrContext::ComputeImageSize,
        py::arg("image"), py::arg("mipMapped"), py::arg("useNextPow2") = false)
    .def_static("MakeGL",
        py::overload_cast<sk_sp<const GrGLInterface>, const GrContextOptions&>(
            &GrContext::MakeGL),
        py::arg("interface"), py::arg("options"))
    .def_static("MakeGL",
        py::overload_cast<sk_sp<const GrGLInterface>>(&GrContext::MakeGL),
        py::arg("interface"))
    .def_static("MakeGL",
        py::overload_cast<const GrContextOptions&>(&GrContext::MakeGL),
        py::arg("options"))
    .def_static("MakeGL", py::overload_cast<>(&GrContext::MakeGL))
    // .def_static("MakeVulkan",
    //     (sk_sp<GrContext> (*)(const GrVkBackendContext&,
    //         const GrContextOptions&)) &GrContext::MakeVulkan,
    //     "The Vulkan context (VkQueue, VkDevice, VkInstance) must be kept "
    //     "alive until the returned GrContext is first destroyed or abandoned.")
    // .def_static("MakeVulkan",
    //     (sk_sp<GrContext> (*)(const GrVkBackendContext&))
    //     &GrContext::MakeVulkan)
    .def_static("MakeMock",
        py::overload_cast<const GrMockOptions*, const GrContextOptions&>(
            &GrContext::MakeMock),
        py::arg("mockOptions"), py::arg("options"))
    .def_static("MakeMock",
        py::overload_cast<const GrMockOptions*>(&GrContext::MakeMock),
        py::arg("mockOptions"))
    ;
}
