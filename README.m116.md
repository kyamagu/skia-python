This is a partial port of [skia-python](https://github.com/kyamagu/skia-python/)
from m87 to m116, possibly disabling any API that's not working.

It concentrates on OT-SVG, and fixing these two issues:

[SkSVGDOM::renderNode() is not exposed in python](https://github.com/kyamagu/skia-python/issues/192)
[three-args contructor to SkMemoryStream not exposed.](https://github.com/kyamagu/skia-python/issues/194)

Some COLRv1-related Skia internals are also exposed for access.


General overview of changes between m87 and m116
================================================

* Be WARN'ed: some m87 APIs are removed when there are no obvious
  new-equivalents, or possible emulations with m116.

* Where it is possible, when m87 APIs disappear, emulations with m116
  is done. So these are "new emulations of old APIs" and while they work,
  they might be withdrawn/changed later:
       Image I/O and decoding routines -
       "encodeToData", "MakeRasterCopy",
       "MakeFromRaster", "MakeFromBitmap", "MakeFromEncoded",
       "MakeTextureFromCompressed", "MakeRasterFromCompressed",
       "MakeFromTexture", "MakeFromCompressedTexture",
       "MakeCrossContextFromPixmap", "MakeFromAdoptedTexture",
       "MakeFromYUVATexturesCopy", "MakeFromYUVATexturesCopyWithExternalBackend",
       "MakeFromYUVATextures", "MakeFromYUVAPixmaps", "MakeFromYUVAPixmaps",
       "MakeFromPicture", "MakeBackendTextureFromSkImage",
       "MakeBackendTextureFromSkImage"
       - these are emulated in m116. In particular, upstream recommends
         using the graphic format encoding/decoding routines directly.

       Surface methods -
       "Surface.MakeRasterN32Premul", "Surface.MakeRasterDirect",
       "Surface.getBackendTexture", "Surface.getBackendRenderTarget",
       "Surface.MakeRaster", "Surface.MakeFromBackendTexture",
       "Surface.MakeFromBackendRenderTarget", "Surface::MakeRenderTarget",
       "Surface.MakeNull".
       - these are simple renamings; we might add the new names and remove
         the old names, and document them as renamed.

* Most "GrContext" class methods were merged into "GrDirectContext" class
  methods. For now, we have an alias between those two classes, and
  internally, use "GrContext" as it is shorter. In the long term,
  people should use "GrDirectContext", to align with upstream documentation.

* New "SamplingOptions" class; there is only a default constructor for now.
  Many APIs changes from m87 to m116 concern adding new options with it, or
  changing from "FilterQuality" enum to "SamplingOptions". If you use any
  value other than the default "FilterQuality.kMedium_SkFilterQuality"
  enum, you'll need to request addition to the "SamplingOptions" class.

* "yuvainfo.PlanarConfig" enums were splitted into a combination of
  "yuvainfo.PlaneConfig" and "yuvainfo.Subsampling" enums. Where the former
  enum was used in m87, it is the equivalent of a combination of the latter
  two in m116.
