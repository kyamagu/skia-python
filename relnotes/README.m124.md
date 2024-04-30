New to m124:

* The skia.SamplingOptions class has been fleshed out.

  To migrate from m87 SkFilterQuality-based code:

  FilterQuality.kHigh_FilterQuality   -> SamplingOptions(CubicResampler.Mitchell())
  FilterQuality.kMedium_FilterQuality -> SamplingOptions(FilterMode.kLinear, MipmapMode.kNearest)    // cpu
                                         or SamplingOptions(FilterMode.kLinear, MipmapMode.kLinear)  // gpu
  FilterQuality.kLow_FilterQuality    -> SamplingOptions(FilterMode.kLinear, MipmapMode.kNone)
  FilterQuality.kNone_FilterQuality   -> SamplingOptions(FilterMode.kNearest, MipmapMode.kNone)

Since m123:

* Vulkan initialization has changed. In m123, one does:

      vkinstance = GrBackendSemaphore()
      vkinstance.initVulkan(vksemaphore)

  In m124, `vkinstance = GrBackendSemaphore.MakeVk(vksemaphore)` is used instead.
