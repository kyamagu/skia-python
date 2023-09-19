#include "common.h"
#include <include/core/SkTextureCompressionType.h>
#include <include/gpu/GrBackendSemaphore.h>
#include <include/gpu/GrBackendSurface.h>
#include <include/gpu/GrContextThreadSafeProxy.h>
#include <include/gpu/GpuTypes.h>
#include <include/gpu/mock/GrMockTypes.h>
#include <include/gpu/gl/GrGLInterface.h>
#include <include/gpu/vk/GrVkBackendContext.h>
#include <include/gpu/MutableTextureState.h>
#include <pybind11/chrono.h>
#include <pybind11/stl.h>
#include <pybind11/cast.h>

void initGrContext_gl(py::module&);
void initGrContext_mock(py::module&);
void initGrContext_vk(py::module&);

void initGrContext(py::module &m) {

py::enum_<GrBackendApi>(m, "GrBackendApi",
    R"docstring(
    Possible 3D APIs that may be used by Ganesh.
    )docstring",
    py::arithmetic())
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

py::enum_<skgpu::BackendApi>(m, "gpuBackendApi",
    R"docstring(
    Possible 3D APIs that may be used by Graphite.
    )docstring",
    py::arithmetic())
    .value("kDawn", skgpu::BackendApi::kDawn)
    .value("kMetal", skgpu::BackendApi::kMetal)
    .value("kVulkan", skgpu::BackendApi::kVulkan)
    .value("kMock", skgpu::BackendApi::kMock)
    .export_values();

py::enum_<GrMipmapped>(m, "GrMipmapped",
    R"docstring(
    Used to say whether a texture has mip levels allocated or not.
    )docstring",
    py::arithmetic())
    .value("kNo", GrMipmapped::kNo)
    .value("kYes", GrMipmapped::kYes)
    .export_values();

py::enum_<GrRenderable>(m, "GrRenderable", py::arithmetic())
    .value("kNo", GrRenderable::kNo)
    .value("kYes", GrRenderable::kYes)
    .export_values();

py::enum_<GrProtected>(m, "GrProtected", py::arithmetic())
    .value("kNo", GrProtected::kNo)
    .value("kYes", GrProtected::kYes)
    .export_values();

py::enum_<GrSurfaceOrigin>(m, "GrSurfaceOrigin",
    R"docstring(
    GPU :py:class:`Image` and :py:class:`Surfaces` can be stored such that
    (0, 0) in texture space may correspond to either the top-left or bottom-left
    content pixel.
    )docstring",
    py::arithmetic())
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
/*
    .value("kPathRendering_GrGLBackendState",
        GrGLBackendState::kPathRendering_GrGLBackendState)
*/
    .value("kALL_GrGLBackendState",
        GrGLBackendState::kALL_GrGLBackendState)
    .export_values();

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

    If a submittedProc is provided, the submittedProc will be called when all
    work from this flush call is submitted to the GPU. If the flush call fails
    due to an error and nothing will get sent to the GPU, the submitted proc is
    called immediately. It is possibly that when work is finally submitted, that
    the submission actual fails. In this case we will not reattempt to do the
    submission. Skia notifies the client of these via the success bool passed
    into the submittedProc. The submittedProc is useful to the client to know
    when semaphores that were sent with the flush have actually been submitted
    to the GPU so that they can be waited on (or deleted if the submit fails).

    Note about GL: In GL work gets sent to the driver immediately during the
    flush call, but we don't really know when the driver sends the work to the
    GPU. Therefore, we treat the submitted proc as we do in other backends. It
    will be called when the next GrDirectContext::submit is called after the flush (or
    possibly during the flush if there is no work to be done for the flush). The
    main use case for the submittedProc is to know when semaphores have been
    sent to the GPU and even in GL it is required to call GrDirectContext::submit to
    flush them. So a client should be able to treat all backend APIs the same in
    terms of how the submitted procs are treated.
    )docstring")
    .def(py::init<>())
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
    // .def_readwrite("fFinishedProc", &GrFlushInfo::fFinishedProc)
    // .def_readwrite("fFinishedContext", &GrFlushInfo::fFinishedContext)
    // .def_readwrite("fSubmittedProc", &GrFlushInfo::fSubmittedProc)
    // .def_readwrite("fSubmittedContext", &GrFlushInfo::fSubmittedContext)
    ;

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
    .def("initGL",
        [] (GrBackendSemaphore& semaphore, void* glsync) {
            semaphore.initGL(reinterpret_cast<GrGLsync>(glsync));
        },
        py::arg("glsync"))
/*
    .def("initVulkan",
        [] (GrBackendSemaphore& semaphore, void* vksemaphore) {
            semaphore.initVulkan(reinterpret_cast<VkSemaphore>(vksemaphore));
        },
        py::arg("semaphore"))
*/
    // .def("initMetal", &GrBackendSemaphore::initMetal)
    .def("isInitialized", &GrBackendSemaphore::isInitialized)
    .def("glSync",
        [] (GrBackendSemaphore& semaphore) {
            return reinterpret_cast<void*>(semaphore.glSync());
        })
/*
    .def("vkSemaphore",
        [] (GrBackendSemaphore& semaphore) {
            return reinterpret_cast<void*>(semaphore.vkSemaphore());
        })
*/
    // .def("mtlSemaphore", &GrBackendSemaphore::mtlSemaphore)
    // .def("mtlValue", &GrBackendSemaphore::mtlValue)
    ;

py::class_<GrBackendFormat>(m, "GrBackendFormat")
    .def(py::init<>())
    .def(py::init<const GrBackendFormat&>())
    .def_static("MakeGL", &GrBackendFormat::MakeGL,
        py::arg("format"), py::arg("target"))
/*
    .def_static("MakeVk", py::overload_cast<VkFormat>(&GrBackendFormat::MakeVk),
        py::arg("format"))
    .def_static("MakeVk",
        py::overload_cast<const GrVkYcbcrConversionInfo&>(
            &GrBackendFormat::MakeVk),
        py::arg("ycbcrInfo"))
*/
    .def_static("MakeMock", &GrBackendFormat::MakeMock,
        py::arg("colorType"), py::arg("compression"), py::arg("isStencilFormat") = false)
    .def("__eq__", &GrBackendFormat::operator==, py::arg("other"),
        py::is_operator())
    .def("__ne__", &GrBackendFormat::operator!=, py::arg("other"),
        py::is_operator())
    .def("backend", &GrBackendFormat::backend)
    .def("textureType", &GrBackendFormat::textureType)
    .def("channelMask", &GrBackendFormat::channelMask)
    .def("asGLFormat", &GrBackendFormat::asGLFormat)
/*
    .def("asVkFormat", &GrBackendFormat::asVkFormat, py::arg("format"))
    .def("getVkYcbcrConversionInfo",
        &GrBackendFormat::getVkYcbcrConversionInfo)
*/
    .def("asMockColorType", &GrBackendFormat::asMockColorType)
    .def("asMockCompressionType", &GrBackendFormat::asMockCompressionType)
    .def("makeTexture2D", &GrBackendFormat::makeTexture2D)
    .def("isValid", &GrBackendFormat::isValid)
    ;

py::class_<GrBackendTexture>(m, "GrBackendTexture")
    .def(py::init<>())
    .def(py::init<int, int, GrMipmapped, const GrGLTextureInfo&>(),
        py::arg("width"), py::arg("height"), py::arg("mipMapped"),
        py::arg("glInfo"))
#ifdef SK_VULKAN
    .def(py::init<int, int, const GrVkImageInfo&>(),
        py::arg("width"), py::arg("height"), py::arg("vkInfo"))
#endif
    .def(py::init<int, int, GrMipmapped, const GrMockTextureInfo&>(),
        py::arg("width"), py::arg("height"), py::arg("mipMapped"),
        py::arg("mockInfo"))
    .def(py::init<const GrBackendTexture&>(), py::arg("that"))
    .def("dimensions", &GrBackendTexture::dimensions)
    .def("width", &GrBackendTexture::width)
    .def("height", &GrBackendTexture::height)
    .def("hasMipmaps", &GrBackendTexture::hasMipmaps)
    .def("backend", &GrBackendTexture::backend)
    .def("getGLTextureInfo", &GrBackendTexture::getGLTextureInfo,
        py::arg("info"))
    .def("glTextureParametersModified",
        &GrBackendTexture::glTextureParametersModified)
/*
    .def("getVkImageInfo", &GrBackendTexture::getVkImageInfo,
        py::arg("info"))
    .def("setVkImageLayout", &GrBackendTexture::setVkImageLayout,
        py::arg("layout"))
*/
    .def("getBackendFormat", &GrBackendTexture::getBackendFormat)
    .def("getMockTextureInfo", &GrBackendTexture::getMockTextureInfo,
        py::arg("info"))
    .def("setMutableState", &GrBackendTexture::setMutableState,
        py::arg("state"))
    .def("isProtected", &GrBackendTexture::isProtected)
    .def("isValid", &GrBackendTexture::isValid)
    .def("isSameTexture", &GrBackendTexture::isSameTexture,
        py::arg("texture"))
    ;

py::class_<GrContextOptions>(m, "GrContextOptions")
    .def(py::init<>())
    // TODO: Implement me!
    ;

py::class_<GrBackendSurfaceMutableState>(m, "GrBackendSurfaceMutableState",
    R"docstring(
    Since Skia and clients can both modify gpu textures and their connected
    state, Skia needs a way for clients to inform us if they have modifiend any
    of this state.

    In order to not need setters for every single API and state, we use this
    class to be a generic wrapper around all the mutable state. This class is
    used for calls that inform Skia of these texture/image state changes by the
    client as well as for requesting state changes to be done by Skia. The
    backend specific state that is wrapped by this class are:

        Vulkan: VkImageLayout and QueueFamilyIndex
    )docstring")
    .def(py::init<>())
#ifdef SK_VULKAN
    .def(py::init<VkImageLayout, uint32_t>(),
        py::arg("layout"), py::arg("queueFamilyIndex"))
    .def("getVkImageLayout",
        &GrBackendSurfaceMutableState::getVkImageLayout,
        R"docstring(
        If this class is not Vulkan backed it will return value of
        VK_IMAGE_LAYOUT_UNDEFINED. Otherwise it will return the VkImageLayout.
        )docstring")
    .def("getQueueFamilyIndex",
        &GrBackendSurfaceMutableState::getQueueFamilyIndex,
        R"docstring(
        If this class is not Vulkan backed it will return value of
        VK_QUEUE_FAMILY_IGNORED. Otherwise it will return the VkImageLayout.
        )docstring")
#endif
    .def("isValid", &GrBackendSurfaceMutableState::isValid)
    .def("backend", &GrBackendSurfaceMutableState::backend)
    ;

py::class_<GrBackendRenderTarget>(m, "GrBackendRenderTarget")
    .def(py::init<>())
    .def(py::init<int, int, int, int, const GrGLFramebufferInfo&>(),
        py::arg("width"), py::arg("height"), py::arg("sampleCnt"),
        py::arg("stencilBits"), py::arg("glInfo"))
#ifdef SK_VULKAN
    .def(py::init<int, int, int, const GrVkImageInfo&>(),
        py::arg("width"), py::arg("height"), py::arg("vkInfo"))
    .def(py::init<int, int, int, int, const GrVkImageInfo&>(),
        py::arg("width"), py::arg("height"), py::arg("sampleCnt"),
        py::arg("vkInfo"))
#endif
    .def(py::init<int, int, int, int, const GrMockRenderTargetInfo&>(),
        py::arg("width"), py::arg("height"), py::arg("sampleCnt"),
        py::arg("stencilBits"), py::arg("mockInfo"))
    .def("dimensions", &GrBackendRenderTarget::dimensions)
    .def("width", &GrBackendRenderTarget::width)
    .def("height", &GrBackendRenderTarget::height)
    .def("sampleCnt", &GrBackendRenderTarget::sampleCnt)
    .def("stencilBits", &GrBackendRenderTarget::stencilBits)
    .def("backend", &GrBackendRenderTarget::backend)
    .def("isFramebufferOnly", &GrBackendRenderTarget::isFramebufferOnly)
    .def("getGLFramebufferInfo", &GrBackendRenderTarget::getGLFramebufferInfo,
        R"docstring(
        If the backend API is GL, copies a snapshot of the GrGLFramebufferInfo
        struct into the passed in pointer and returns true. Otherwise returns
        false if the backend API is not GL.
        )docstring",
        py::arg("info"))
/*
    .def("getVkImageInfo", &GrBackendRenderTarget::getVkImageInfo,
        R"docstring(
        If the backend API is Vulkan, copies a snapshot of the GrVkImageInfo
        struct into the passed in pointer and returns true. This snapshot will
        set the fImageLayout to the current layout state. Otherwise returns
        false if the backend API is not Vulkan.
        )docstring",
        py::arg("info"))
    .def("setVkImageLayout", &GrBackendRenderTarget::setVkImageLayout,
        R"docstring(
        Anytime the client changes the VkImageLayout of the VkImage captured by
        this GrBackendRenderTarget, they must call this function to notify Skia
        of the changed layout.
        )docstring",
        py::arg("layout"))
*/
    .def("getBackendFormat", &GrBackendRenderTarget::getBackendFormat,
        R"docstring(
        Get the GrBackendFormat for this render target (or an invalid format if
        this is not valid).
        )docstring")
    .def("getMockRenderTargetInfo",
        &GrBackendRenderTarget::getMockRenderTargetInfo,
        R"docstring(
        If the backend API is Mock, copies a snapshot of the GrMockTextureInfo
        struct into the passed in pointer and returns true. Otherwise returns
        false if the backend API is not Mock.
        )docstring",
        py::arg("info"))
    // .def("setMutableState", &GrBackendRenderTarget::setMutableState)
    .def("isProtected", &GrBackendRenderTarget::isProtected)
    .def("isValid", &GrBackendRenderTarget::isValid)
    ;

py::class_<GrContext_Base, sk_sp<GrContext_Base>, SkRefCnt>(m, "GrContext_Base")
    .def("asDirectContext", &GrContext_Base::asDirectContext,
        R"docstring(
        Safely downcast to a :py:class:`GrDirectContext`.
        )docstring")
    .def("backend", &GrContext_Base::backend,
        R"docstring(
        The 3D API backing this context.
        )docstring")
/*
    .def("defaultBackendFormat", &GrContext_Base::defaultBackendFormat,
        R"docstring(
        Retrieve the default :py:class:`GrBackendFormat` for a given
        :py:class:`ColorType` and renderability. It is guaranteed that this
        backend format will be the one used by the :py:class:`GrContext`
        :py:class:`ColorType` and SurfaceCharacterization-based
        :py:meth:`createBackendTexture` methods.

        The caller should check that the returned format is valid.
        )docstring")
    .def("compressedBackendFormat", &GrContext_Base::compressedBackendFormat)
*/
    .def("threadSafeProxy", &GrContext_Base::threadSafeProxy)
    // .def("priv", py::overload_cast<>(&GrContext_Base::priv))
    // .def("priv", py::overload_cast<>(&GrContext_Base::priv, py::const_))
    ;

py::class_<GrImageContext, sk_sp<GrImageContext>, GrContext_Base>(
    m, "GrImageContext")
    ;

py::class_<GrRecordingContext, sk_sp<GrRecordingContext>, GrImageContext>(
    m, "GrRecordingContext")
    .def("defaultBackendFormat", &GrRecordingContext::defaultBackendFormat,
        R"docstring(
        Retrieve the default :py:class:`GrBackendFormat` for a given
        :py:class:`ColorType` and renderability.

        It is guaranteed that this backend format will be the one used by the
        following :py:class:`ColorType` and SurfaceCharacterization-based
        :py:meth:`createBackendTexture` methods.

        The caller should check that the returned format is valid.
        )docstring",
        py::arg("colorType"), py::arg("renderable") = GrRenderable::kNo)
    .def("abandoned", &GrRecordingContext::abandoned,
        R"docstring(
        Reports whether the :py:class:`GrDirectContext` associated with this
        :py:class:`GrRecordingContext` is abandoned.

        When called on a :py:class:`GrDirectContext` it may actively check
        whether the underlying 3D API device/context has been disconnected
        before reporting the status. If so, calling this method will transition
        the :py:class:`GrDirectContext` to the abandoned state.
        )docstring")
    .def("colorTypeSupportedAsSurface",
        &GrRecordingContext::colorTypeSupportedAsSurface,
        R"docstring(
        Can a :py:class:`Surface` be created with the given color type. To check
        whether MSAA is supported use
        :py:meth:`~GrRecordingContext.maxSurfaceSampleCountForColorType`.
        )docstring",
        py::arg("colorType"))
    .def("maxTextureSize", &GrRecordingContext::maxTextureSize,
        R"docstring(
        Gets the maximum supported texture size.
        )docstring")
    .def("maxRenderTargetSize", &GrRecordingContext::maxRenderTargetSize,
        R"docstring(
        Gets the maximum supported render target size.
        )docstring")
    .def("colorTypeSupportedAsImage",
        &GrRecordingContext::colorTypeSupportedAsImage,
        R"docstring(
        Can a :py:class:`Image` be created with the given color type.
        )docstring")
    .def("maxSurfaceSampleCountForColorType",
        &GrRecordingContext::maxSurfaceSampleCountForColorType,
        R"docstring(
        Gets the maximum supported sample count for a color type.

        1 is returned if only non-MSAA rendering is supported for the color
        type. 0 is returned if rendering to this color type is not supported at
        all.
        )docstring")
    // .def("priv",
    //     py::overload_cast<>(&GrRecordingContext::priv))
    // .def("priv",
    //     py::overload_cast<>(&GrRecordingContext::priv, py::const_))
    ;

py::class_<GrDirectContext, sk_sp<GrDirectContext>, GrRecordingContext>(m, "GrDirectContext")
    .def("resetContext", &GrDirectContext::resetContext,
        R"docstring(
        The :py:class:`GrContext` normally assumes that no outsider is setting
        state within the underlying 3D API's context/device/whatever.

        This call informs the context that the state was modified and it should
        resend. Shouldn't be called frequently for good performance. The flag
        bits, state, is dependent on which backend is used by the context,
        either GL or D3D (possible in future).
        )docstring",
        py::arg("state") = kAll_GrBackendState)
    .def("resetGLTextureBindings", &GrDirectContext::resetGLTextureBindings,
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
    .def("abandonContext", &GrDirectContext::abandonContext,
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
    .def("abandoned", &GrDirectContext::abandoned,
        R"docstring(
        Returns true if the context was abandoned or if the if the backend
        specific context has gotten into an unrecoverarble, lost state (e.g.

        in Vulkan backend if we've gotten a VK_ERROR_DEVICE_LOST). If the
        backend context is lost, this call will also abandon the GrContext.
        )docstring")
    .def("oomed", &GrDirectContext::oomed,
        R"docstring(
        Checks if the underlying 3D API reported an out-of-memory error.

        If this returns true it is reset and will return false until another
        out-of-memory error is reported by the 3D API. If the context is
        abandoned then this will report false.

        Currently this is implemented for:

        OpenGL [ES] - Note that client calls to glGetError() may swallow
        GL_OUT_OF_MEMORY errors and therefore hide the error from Skia. Also, it
        is not advised to use this in combination with enabling
        GrContextOptions::fSkipGLErrorChecks. That option may prevent GrContext
        from ever checking the GL context for OOM.

        Vulkan - Reports true if VK_ERROR_OUT_OF_HOST_MEMORY or
        VK_ERROR_OUT_OF_DEVICE_MEMORY has occurred.
        )docstring")
    .def("releaseResourcesAndAbandonContext",
        &GrDirectContext::releaseResourcesAndAbandonContext,
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
    .def("getResourceCacheLimit", &GrDirectContext::getResourceCacheLimit,
        R"docstring(
        Return the current GPU resource cache limit in bytes.
        )docstring")
    .def("getResourceCacheUsage", &GrDirectContext::getResourceCacheUsage,
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
        &GrDirectContext::getResourceCachePurgeableBytes,
        R"docstring(
        Gets the number of bytes in the cache consumed by purgeable (e.g.
        unlocked) resources.
        )docstring")
    .def("setResourceCacheLimit", &GrDirectContext::setResourceCacheLimit,
        R"docstring(
        Specify the GPU resource cache limit.

        If the cache currently exceeds this limit, it will be purged (LRU) to
        keep the cache within the limit.

        :param maxResourceBytes: The maximum number of bytes of video memory
            that can be held in the cache.
        )docstring",
        py::arg("maxResourceBytes"))
    .def("freeGpuResources", &GrDirectContext::freeGpuResources,
        R"docstring(
        Frees GPU created by the context.

        Can be called to reduce GPU memory pressure.
        )docstring")
/*
    .def("performDeferredCleanup", &GrDirectContext::performDeferredCleanup,
        R"docstring(
        Purge GPU resources that haven't been used in the past 'msNotUsed'
        milliseconds or are otherwise marked for deletion, regardless of whether
        the context is under budget.
        )docstring",
        py::arg("msNotUsed"))
*/
    .def("purgeResourcesNotUsedInMs", &GrDirectContext::purgeResourcesNotUsedInMs,
        py::arg("msNotUsed"))
    .def("purgeUnlockedResources",
        py::overload_cast<size_t, bool>(&GrDirectContext::purgeUnlockedResources),
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
        py::overload_cast<bool>(&GrDirectContext::purgeUnlockedResources),
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
    .def("maxTextureSize", &GrDirectContext::maxTextureSize,
        R"docstring(
        Gets the maximum supported texture size.
        )docstring")
    .def("maxRenderTargetSize", &GrDirectContext::maxRenderTargetSize,
        R"docstring(
        Gets the maximum supported render target size.
        )docstring")
    .def("colorTypeSupportedAsImage", &GrDirectContext::colorTypeSupportedAsImage,
        R"docstring(
        Can a :py:class:`Image` be created with the given color type.
        )docstring",
        py::arg("colorType"))
    .def("colorTypeSupportedAsSurface", &GrDirectContext::colorTypeSupportedAsSurface,
        R"docstring(
        Can a SkSurface be created with the given color type.

        To check whether MSAA is supported use
        :py:meth:`maxSurfaceSampleCountForColorType`.
        )docstring",
        py::arg("colorType"))
    .def("maxSurfaceSampleCountForColorType",
        &GrDirectContext::maxSurfaceSampleCountForColorType,
        R"docstring(
        Gets the maximum supported sample count for a color type.

        1 is returned if only non-MSAA rendering is supported for the color
        type. 0 is returned if rendering to this color type is not supported at
        all.
        )docstring",
        py::arg("colorType"))
    .def("wait",
        [] (GrDirectContext& context,
            const std::vector<GrBackendSemaphore>& semaphores,
            bool deleteSemaphoresAfterWait) {
            return context.wait(
                semaphores.size(), &semaphores[0], deleteSemaphoresAfterWait);
        },
        R"docstring(
        Inserts a list of GPU semaphores that the current GPU-backed API must
        wait on before executing any more commands on the GPU.

        Skia will take ownership of the underlying semaphores and delete them
        once they have been signaled and waited on. If this call returns false,
        then the GPU back-end will not wait on any passed in semaphores, and the
        client will still own the semaphores.
        )docstring",
        py::arg("semaphores"), py::arg("deleteSemaphoresAfterWait") = true)
    .def("flushAndSubmit", py::overload_cast<bool>(&GrDirectContext::flushAndSubmit),
        R"docstring(
        Call to ensure all drawing to the context has been flushed and submitted
        to the underlying 3D API. This is equivalent to calling :py:meth:`flush`
        with a default :py:class:`GrFlushInfo` followed by :py:meth:`submit`.
        )docstring",
        py::arg("syncCpu") = false)
    .def("flush", py::overload_cast<const GrFlushInfo&>(&GrDirectContext::flush),
        R"docstring(
        Call to ensure all drawing to the context has been flushed to underlying
        3D API specific objects. A call to :py:meth:`GrContext.submit` is always
        required to ensure work is actually sent to the gpu. Some specific API
        details:

        :GL: Commands are actually sent to the driver, but glFlush is never
            called. Thus some sync objects from the flush will not be valid
            until a submission occurs.

        :Vulkan/Metal/D3D/Dawn: Commands are recorded to the backend APIs
            corresponding command buffer or encoder objects. However, these
            objects are not sent to the gpu until a submission occurs.

        If the return is GrSemaphoresSubmitted::kYes, only initialized
        GrBackendSemaphores will be submitted to the gpu during the next submit
        call (it is possible Skia failed to create a subset of the semaphores).
        The client should not wait on these semaphores until after submit has
        been called, but must keep them alive until then. If a submit flag was
        passed in with the flush these valid semaphores can we waited on
        immediately. If this call returns GrSemaphoresSubmitted::kNo, the GPU
        backend will not submit any semaphores to be signaled on the GPU. Thus
        the client should not have the GPU wait on any of the semaphores passed
        in with the GrFlushInfo. Regardless of whether semaphores were submitted
        to the GPU or not, the client is still responsible for deleting any
        initialized semaphores.

        Regardleess of semaphore submission the context will still be flushed.
        It should be emphasized that a return value of
        GrSemaphoresSubmitted::kNo does not mean the flush did not happen. It
        simply means there were no semaphores submitted to the GPU. A caller
        should only take this as a failure if they passed in semaphores to be
        submitted.
        )docstring",
        py::arg("info"))
    .def("flush", py::overload_cast<>(&GrDirectContext::flush))
    .def("submit", &GrDirectContext::submit,
        R"docstring(
        Submit outstanding work to the gpu from all previously un-submitted
        flushes. The return value of the submit will indicate whether or not the
        submission to the GPU was successful.

        If the call returns true, all previously passed in semaphores in flush
        calls will have been submitted to the GPU and they can safely be waited
        on. The caller should wait on those semaphores or perform some other
        global synchronization before deleting the semaphores.

        If it returns false, then those same semaphores will not have been
        submitted and we will not try to submit them again. The caller is free
        to delete the semaphores at any time.

        If the syncCpu flag is true this function will return once the gpu has
        finished with all submitted work.
        )docstring",
        py::arg("syncCpu") = false)
    .def("checkAsyncWorkCompletion", &GrDirectContext::checkAsyncWorkCompletion,
        R"docstring(
        Checks whether any asynchronous work is complete and if so calls related
        callbacks.
        )docstring")
    // .def("priv", (GrContextPriv (GrDirectContext::*)()) &GrContext::priv)
    // .def("priv", (const GrContextPriv (GrDirectContext::*)() const) &GrContext::priv)
    // .def("dumpMemoryStatistics", &GrDirectContext::dumpMemoryStatistics,
    //     "Enumerates all cached GPU resources and dumps their memory to "
    //     "traceMemoryDump.")
    .def("supportsDistanceFieldText", &GrDirectContext::supportsDistanceFieldText)
    .def("storeVkPipelineCacheData", &GrDirectContext::storeVkPipelineCacheData)
    .def_static("ComputeImageSize",
        [] (sk_sp<SkImage> image, GrMipmapped mapped, bool useNextPow2) {
            // REVISIT: process GrMipmapped and useNextPow2 = true
            return image->textureSize();
        },
        py::arg("image"), py::arg("mipMapped"), py::arg("useNextPow2") = false)
    .def("defaultBackendFormat", &GrDirectContext::defaultBackendFormat,
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
        py::overload_cast<int, int, const GrBackendFormat&, GrMipmapped,
            GrRenderable, GrProtected, std::string_view>(&GrDirectContext::createBackendTexture),
        R"docstring(
        If possible, create an uninitialized backend texture.

        The client should ensure that the returned backend texture is valid.
        For the Vulkan backend the layout of the created VkImage will be:
             VK_IMAGE_LAYOUT_UNDEFINED.
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("backendFormat"),
        py::arg("mipMapped"), py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo, py::arg("view") = std::string_view{})
    .def("createBackendTexture",
        py::overload_cast<int, int, SkColorType, GrMipmapped,
            GrRenderable, GrProtected, std::string_view>(&GrDirectContext::createBackendTexture),
        R"docstring(
        If possible, create an uninitialized backend texture.

        The client should ensure that the returned backend texture is valid. If
        successful, the created backend texture will be compatible with the
        provided :py:class:`ColorType`. For the Vulkan backend the layout of the
        created VkImage will be: VK_IMAGE_LAYOUT_UNDEFINED.
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("colorType"),
        py::arg("mipMapped"), py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo, py::arg("view") = std::string_view{})
    .def("createBackendTexture",
        [] (GrDirectContext& context, int width, int height,
            const GrBackendFormat& backendFormat, const SkColor4f& color,
            GrMipmapped mipMapped, GrRenderable renderable,
            GrProtected isProtected) {
            return context.createBackendTexture(
                width, height, backendFormat, color, mipMapped, renderable,
                isProtected);
        },
        R"docstring(
        If possible, create a backend texture initialized to a particular color.

        The client should ensure that the returned backend texture is valid. The
        client can pass in a finishedProc to be notified when the data has been
        uploaded by the gpu and the texture can be deleted. The client is
        required to call GrDirectContext::submit to send the upload work to the gpu.
        The finishedProc will always get called even if we failed to create the
        GrBackendTexture.
        For the Vulkan backend the layout of the created VkImage will be:
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("backendFormat"),
        py::arg("color"), py::arg("mipMapped"), py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo)
    .def("createBackendTexture",
        [] (GrDirectContext& context, int width, int height,
            SkColorType colorType, const SkColor4f& color,
            GrMipmapped mipMapped, GrRenderable renderable,
            GrProtected isProtected) {
            return context.createBackendTexture(
                width, height, colorType, color, mipMapped, renderable,
                isProtected);
        },
        R"docstring(
        If possible, create a backend texture initialized to a particular color.

        The client should ensure that the returned backend texture is valid.
        If successful, the created backend texture will be compatible with the
        provided :py:class:`~skia.ColorType`. For the Vulkan backend the layout
        of the created VkImage will be: VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("colorType"),
        py::arg("color"), py::arg("mipMapped"), py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo)
    .def("createBackendTexture",
        [] (GrDirectContext& context, const std::vector<SkPixmap>& srcData,
            GrRenderable renderable, GrProtected isProtected) {
            return context.createBackendTexture(
                (srcData.empty()) ? nullptr : srcData.data(),
                srcData.size(),
                renderable,
                isProtected);
        },
        R"docstring(
        If possible, create a backend texture initialized with the provided
        pixmap data. The client should ensure that the returned backend texture
        is valid. The client can pass in a finishedProc to be notified when the
        data has been uploaded by the gpu and the texture can be deleted. The
        client is required to call GrDirectContext::submit to send the upload work to
        the gpu. The finishedProc will always get called even if we failed to
        create the GrBackendTexture. If successful, the created backend texture
        will be compatible with the provided pixmap(s). Compatible, in this
        case, means that the backend format will be the result of calling
        defaultBackendFormat on the base pixmap's colortype. The src data can be
        deleted when this call returns.

        If numLevels is 1 a non-mipMapped texture will result. If a mipMapped
        texture is desired the data for all the mipmap levels must be
        provided. In the mipmapped case all the colortypes of the provided
        pixmaps must be the same. Additionally, all the miplevels must be
        sized correctly (please see SkMipmap::ComputeLevelSize and
        ComputeLevelCount). Note: the pixmap's alphatypes and colorspaces are
        ignored. For the Vulkan backend the layout of the created VkImage will
        be: VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        )docstring",
        py::arg("srcData"),  py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo)
    .def("createBackendTexture",
        [] (GrDirectContext& context, const SkPixmap& pixmap, GrRenderable renderable,
            GrProtected isProtected) {
            return context.createBackendTexture(
                pixmap, renderable, isProtected);
        },
        py::arg("pixmap"), py::arg("renderable"),
        py::arg("isProtected") = GrProtected::kNo)
    .def("updateBackendTexture",
        [] (GrDirectContext& context, const GrBackendTexture& texture,
            const SkColor4f& color) {
            return context.updateBackendTexture(
                texture, color, nullptr, nullptr);
        },
        R"docstring(
        If possible, updates a backend texture to be filled to a particular
        color.

        The client should check the return value to see if the update was
        successful. The client can pass in a finishedProc to be notified when
        the data has been uploaded by the gpu and the texture can be deleted.
        The client is required to call :py:meth:`GrContext.submit` to send the
        upload work to the gpu. The finishedProc will always get called even if
        we failed to update the :py:class:`GrBackendTexture`. For the Vulkan
        backend after a successful update the layout of the created VkImage will
        be: VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        )docstring",
        py::arg("backendTexture"), py::arg("color"))
    .def("updateBackendTexture",
        [] (GrDirectContext& context, const GrBackendTexture& texture,
            const std::vector<SkPixmap>& srcData) {
            return context.updateBackendTexture(
                texture, &srcData[0], srcData.size(), nullptr, nullptr);
        },
        R"docstring(
        If possible, updates a backend texture to be filled to a particular
        color.

        The client should check the return value to see if the update was
        successful. The client can pass in a finishedProc to be notified when
        the data has been uploaded by the gpu and the texture can be deleted.
        The client is required to call :py:meth:`GrContext.submit` to send the
        upload work to the gpu. The finishedProc will always get called even if
        we failed to update the :py:class:`GrBackendTexture`. The backend
        texture must be compatible with the provided pixmap(s). Compatible, in
        this case, means that the backend format is compatible with the base
        pixmap's colortype. The src data can be deleted when this call returns.
        If the backend texture is mip mapped, the data for all the mipmap levels
        must be provided. In the mipmapped case all the colortypes of the
        provided pixmaps must be the same. Additionally, all the miplevels must
        be sized correctly (please see SkMipmap::ComputeLevelSize and
        ComputeLevelCount). Note: the pixmap's alphatypes and colorspaces are
        ignored. For the Vulkan backend after a successful update the layout of
        the created VkImage will be: VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        )docstring",
        py::arg("backendTexture"), py::arg("srcData"))
    .def("compressedBackendFormat", &GrDirectContext::compressedBackendFormat,
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
        [] (GrDirectContext& context, int width, int height,
            const GrBackendFormat& backendFormat, const SkColor4f& color,
            GrMipmapped mipMapped, GrProtected isProtected) {
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
        [] (GrDirectContext& context, int width, int height,
            SkTextureCompressionType type, const SkColor4f& color,
            GrMipmapped mipMapped, GrProtected isProtected) {
            return context.createCompressedBackendTexture(
                width, height, type, color, mipMapped, isProtected);
        },
        py::arg("width"), py::arg("height"), py::arg("type"), py::arg("color"),
        py::arg("mipMapped"), py::arg("isProtected") = GrProtected::kNo)
    .def("createCompressedBackendTexture",
        [] (GrDirectContext& context, int width, int height,
            const GrBackendFormat& backendFormat, py::buffer b,
            GrMipmapped mipMapped, GrProtected isProtected) {
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
        [] (GrDirectContext& context, int width, int height,
            SkTextureCompressionType type, py::buffer b,
            GrMipmapped mipMapped, GrProtected isProtected) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            return context.createCompressedBackendTexture(
                width, height, type, info.ptr, size, mipMapped, isProtected);
        },
        py::arg("width"), py::arg("height"), py::arg("type"), py::arg("data"),
        py::arg("mipMapped"), py::arg("isProtected") = GrProtected::kNo)
    .def("setBackendTextureState",
        [] (GrDirectContext& context, const GrBackendTexture& texture,
            const GrBackendSurfaceMutableState& mutableState,
            GrBackendSurfaceMutableState* previousState) {
            return context.setBackendTextureState(
                texture, mutableState, previousState, nullptr, nullptr);
        },
        R"docstring(
        Updates the state of the GrBackendTexture/RenderTarget to have the
        passed in GrBackendSurfaceMutableState.

        All objects that wrap the backend surface (i.e. :py:class:`Surface` and
        :py:class:`Image`) will also be aware of this state change. This call
        does not submit the state change to the gpu, but requires the client to
        call :py:meth:`GrContext.submit` to send it to the GPU. The work for
        this call is ordered linearly with all other calls that require
        :py:meth:`GrContext.submit` to be called (e.g
        :py:meth:`~updateBackendTexture` and :py:meth:`~flush`).

        See :py:class:`GrBackendSurfaceMutableState` to see what state can be
        set via this call.

        If the backend API is Vulkan, the caller can set the
        GrBackendSurfaceMutableState's VkImageLayout to
        VK_IMAGE_LAYOUT_UNDEFINED or queueFamilyIndex to VK_QUEUE_FAMILY_IGNORED
        to tell Skia to not change those respective states.

        If previousState is not null and this returns true, then Skia will have
        filled in previousState to have the values of the state before this
        call.
        )docstring",
        py::arg("texture"), py::arg("mutableState"),
        py::arg("previousState") = nullptr)
    .def("setBackendRenderTargetState",
        [] (GrDirectContext& context, const GrBackendRenderTarget& target,
            const GrBackendSurfaceMutableState& mutableState,
            GrBackendSurfaceMutableState* previousState) {
            return context.setBackendRenderTargetState(
                target, mutableState, previousState, nullptr, nullptr);
        },
        py::arg("target"), py::arg("mutableState"),
        py::arg("previousState") = nullptr)
    .def("deleteBackendTexture", &GrDirectContext::deleteBackendTexture,
        py::arg("texture"))
    .def("precompileShader", &GrDirectContext::precompileShader,
        py::arg("key"), py::arg("data"))
#ifdef SK_GL
    .def_static("MakeGL",
        py::overload_cast<sk_sp<const GrGLInterface>, const GrContextOptions&>(
            &GrDirectContext::MakeGL),
        R"docstring(
        Creates a :py:class:`GrDirectContext` for a backend context. If no
        GrGLInterface is provided then the result of GrGLMakeNativeInterface()
        is used if it succeeds.
        )docstring",
        py::arg("interface"), py::arg("options"))
    .def_static("MakeGL",
        py::overload_cast<sk_sp<const GrGLInterface>>(&GrDirectContext::MakeGL),
        py::arg("interface"))
    .def_static("MakeGL",
        py::overload_cast<const GrContextOptions&>(&GrDirectContext::MakeGL),
        py::arg("options"))
    .def_static("MakeGL", py::overload_cast<>(&GrDirectContext::MakeGL))
#endif

#ifdef SK_VULKAN
    .def_static("MakeVulkan",
        py::overload_cast<const GrVkBackendContext&, const GrContextOptions&>(
            &GrDirectContext::MakeVulkan),
        R"docstring(
        The Vulkan context (VkQueue, VkDevice, VkInstance) must be kept alive
        until the returned GrDirectContext is destroyed. This also means that
        any objects created with this GrDirectContext (e.g. Surfaces, Images,
        etc.) must also be released as they may hold refs on the
        GrDirectContext. Once all these objects and the GrDirectContext are
        released, then it is safe to delete the vulkan objects.
        )docstring",
        py::arg("backendContext"), py::arg("options"))
    .def_static("MakeVulkan",
        py::overload_cast<const GrVkBackendContext&>(
            &GrDirectContext::MakeVulkan),
        py::arg("backendContext"))
#endif

    .def_static("MakeMock",
        py::overload_cast<const GrMockOptions*, const GrContextOptions&>(
            &GrDirectContext::MakeMock),
        py::arg("mockOptions"), py::arg("options"))
    .def_static("MakeMock",
        py::overload_cast<const GrMockOptions*>(&GrDirectContext::MakeMock),
        py::arg("mockOptions"))
    .def("abandonContext", &GrDirectContext::abandonContext,
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
        &GrDirectContext::releaseResourcesAndAbandonContext,
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
    .def("freeGpuResources", &GrDirectContext::freeGpuResources,
        R"docstring(
        Frees GPU created by the context.

        Can be called to reduce GPU memory pressure.
        )docstring")
    ;

m.attr("GrContext") = m.attr("GrDirectContext");

initGrContext_gl(m);
initGrContext_vk(m);
initGrContext_mock(m);
}
