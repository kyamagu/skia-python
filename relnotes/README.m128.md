Since m127:

* Vulkan symbols are renamed to skgpu::* . We use the older GrVk* forms,
  but also alias them to skgpu.Vulkan* , under the skia.skgpu namespace
  (i.e. similar to upstream's skgpu::, with python syntax), so that both
  m87-like and current continue to work.
