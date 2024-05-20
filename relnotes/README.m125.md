New to m125:

* Metal-enabled on Mac OS, and added Metal APIs equivalents to Vulkan's.


Changes since m124:

* Dropping Python 3.7 support

* GrContext_Base.defaultBackendFormat and GrContext_Base.compressedBackendFormat
   re-enabled (disabled between m87 and m116)

* Corrected two typos in Vulkan code

* MemoryStream.asData and MemoryStream.skipToAlign4 now emulated
  (changed/removed upstream)