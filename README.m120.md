New in m120:

* Ruimentary support (TextBlob::MakeFromShapedText) of text-shaping via
  upstream's libSkShaper module. Intially this was added to support
  emoji's with skin-tone modifiers (#195), but has the fortunate side-effect
  that now LTR languages (Arabic, Hebrew, Tibetan ...) work as desired in
  skia-python's drawing. Note libSkShaper is buggy on windows. 
  ( https://issues.skia.org/issues/310510988 )

* Option to use freetype as fontmgr on non-linux (#213) - using
  skia.FontMgr.New_Custom_Empty() (upstream's SkFontMgr_New_Custom_Empty).
  This allows Windows/Mac users to use some font formats not supported by
  DirectWrite/CoreText (see #195); and also work around bug in CoreText (#138,
  https://issues.skia.org/310510989 ).

* OT-SVG font support is on by default now (#212, also see #195). 

* Vulkan is enabled for Linux/Windows. Most of the APIs were in m87 (stubs?)
  but were made optional in m98+, and dependent on GPU backend compiled in.
  For Mac OS X users, upstream removed MoltenVK support in m83, and recommend
  using Metal backend (TODO).

Since m119:

* Removed upstream:

  SkTypeface::UniqueID()
  SkFont::refTypefaceOrDefault and SkFont::getTypefaceOrDefault() 
  GrBackendSemaphore::initGL and GrBackendSemaphore::glSync

* A few Image.* methods are re-enabled - they were emulated in m116+
  but left disabled mistakenly.

  MakeTextureFromCompressed, MakeFromCompressedTexture,
  MakeFromYUVAPixmaps, makeNonTextureImage,
  MakeRasterFromCompressed, MakeCrossContextFromPixmap,
  MakeFromAdoptedTexture .
  
Since m87:

* sampleCnt in "GrBackendRenderTarget(int width, int height, int sampleCnt, const GrVkImageInfo& vkInfo)"
  is ignored. In m87 header, "/** Deprecated. Samplecount is now part of GrVkImageInfo. */". 
