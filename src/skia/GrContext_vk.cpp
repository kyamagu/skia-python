#include "common.h"
#include <include/gpu/vk/GrVkBackendContext.h>

void initGrContext_vk(py::module &m) {

py::enum_<VkFormat>(m, "VkFormat", py::arithmetic())
    .export_values();

py::implicitly_convertible<int, VkFormat>();

py::enum_<VkImageLayout>(m, "VkImageLayout", py::arithmetic())
    .export_values();

py::implicitly_convertible<int, VkImageLayout>();

py::class_<GrVkAlloc>(m, "GrVkAlloc")
    .def(py::init<>())
    // TODO: Implement me!
    ;

py::class_<GrVkYcbcrConversionInfo>(m, "GrVkYcbcrConversionInfo")
    .def(py::init<>())
    // TODO: Implement me!
    ;

py::class_<GrVkImageInfo>(m, "GrVkImageInfo")
    .def(py::init<>())
    // .def(py::init(
    //     [] (VkImage image,
    //         GrVkAlloc alloc,
    //         VkImageTiling imageTiling,
    //         VkImageLayout layout,
    //         VkFormat format,
    //         uint32_t levelCount,
    //         uint32_t currentQueueFamily,
    //         GrProtected isProtected,
    //         const GrVkYcbcrConversionInfo* ycbcrConversionInfo) {
    //         return GrVkImageInfo(
    //             image, alloc, imageTiling, layout, format, levelCount,
    //             currentQueueFamily, isProtected,
    //             (ycbcrConversionInfo) ?
    //                 *ycbcrConversionInfo : GrVkYcbcrConversionInfo());
    //     }),
    //     py::arg("image"), py::arg("alloc"), py::arg("imageTiling"),
    //     py::arg("layout"), py::arg("format"), py::arg("levelCount"),
    //     py::arg("currentQueueFamily") = VK_QUEUE_FAMILY_IGNORED,
    //     py::arg("isProtected") = GrProtected::kNo,
    //     py::arg("ycbcrConversionInfo") = nullptr)
    .def(py::init<const GrVkImageInfo&, VkImageLayout, uint32_t>(),
        py::arg("info"), py::arg("layout"), py::arg("familyQueueIndex"))
    // .def_readwrite("fImage", &GrVkImageInfo::fImage)
    .def_readwrite("fAlloc", &GrVkImageInfo::fAlloc)
    // .def_readwrite("fImageTiling", &GrVkImageInfo::fImageTiling)
    // .def_readwrite("fImageLayout", &GrVkImageInfo::fImageLayout)
    // .def_readwrite("fFormat", &GrVkImageInfo::fFormat)
    .def_readwrite("fLevelCount", &GrVkImageInfo::fLevelCount)
    .def_readwrite("fCurrentQueueFamily", &GrVkImageInfo::fCurrentQueueFamily)
    .def_readwrite("fProtected", &GrVkImageInfo::fProtected)
    .def_readwrite("fYcbcrConversionInfo", &GrVkImageInfo::fYcbcrConversionInfo)
    ;

py::class_<GrVkDrawableInfo>(m, "GrVkDrawableInfo")
    // TODO: Implement me!
    ;

// GrVkBackendContext.h
py::enum_<GrVkExtensionFlags>(m, "GrVkExtensionFlags", py::arithmetic())
    .value("kEXT_debug_report_GrVkExtensionFlag",
        GrVkExtensionFlags::kEXT_debug_report_GrVkExtensionFlag)
    .value("kNV_glsl_shader_GrVkExtensionFlag",
        GrVkExtensionFlags::kNV_glsl_shader_GrVkExtensionFlag)
    .value("kKHR_surface_GrVkExtensionFlag",
        GrVkExtensionFlags::kKHR_surface_GrVkExtensionFlag)
    .value("kKHR_swapchain_GrVkExtensionFlag",
        GrVkExtensionFlags::kKHR_swapchain_GrVkExtensionFlag)
    .value("kKHR_win32_surface_GrVkExtensionFlag",
        GrVkExtensionFlags::kKHR_win32_surface_GrVkExtensionFlag)
    .value("kKHR_android_surface_GrVkExtensionFlag",
        GrVkExtensionFlags::kKHR_android_surface_GrVkExtensionFlag)
    .value("kKHR_xcb_surface_GrVkExtensionFlag",
        GrVkExtensionFlags::kKHR_xcb_surface_GrVkExtensionFlag)
    .export_values();

py::enum_<GrVkFeatureFlags>(m, "GrVkFeatureFlags", py::arithmetic())
    .value("kGeometryShader_GrVkFeatureFlag",
        GrVkFeatureFlags::kGeometryShader_GrVkFeatureFlag)
    .value("kDualSrcBlend_GrVkFeatureFlag",
        GrVkFeatureFlags::kDualSrcBlend_GrVkFeatureFlag)
    .value("kSampleRateShading_GrVkFeatureFlag",
        GrVkFeatureFlags::kSampleRateShading_GrVkFeatureFlag)
    .export_values();

py::class_<GrVkBackendContext>(m, "GrVkBackendContext",
    R"docstring(
    The BackendContext contains all of the base Vulkan objects needed by the
    GrVkGpu. The assumption is that the client will set these up and pass them
    to the GrVkGpu constructor. The VkDevice created must support at least one
    graphics queue, which is passed in as well. The QueueFamilyIndex must match
    the family of the given queue. It is needed for CommandPool creation, and
    any GrBackendObjects handed to us (e.g., for wrapped textures) needs to be
    created in or transitioned to that family. The refs held by members of this
    struct must be released (either by deleting the struct or manually releasing
    the refs) before the underlying vulkan device and instance are destroyed.
    )docstring")
    .def(py::init<>())
    // TODO: Implement me!
    ;
}
