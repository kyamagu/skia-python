Since m118:

* SkImage::makeWithFilter emulated
* GrDirectContext::purgeUnlockedResources, GrDirectContext::flushAndSubmit,
  GrDirectContext::submit takes enums instead of true/false
* some Sk*PathEffect have been removed from the public API
  (TODO: emulate?)
* Emulate GL-related GrBackendTexture, GrBackendRenderTarget constructors
  (was removed in m118)

Since m87:

* Emulate SkImageFilters::Xfermode with SkImageFilters::Blend
  - Milestone 88
    ------------
      * Add new SkImageFilters::Blend factory function, in place of the now deprecated
      SkImageFilters::Xfermode factory function. Behavior is identical, but name better matches
      conventions in SkShader and SkColorFilter.
      https://review.skia.org/324623
* Emulate pre-m116 SkTableColorFilter class and SkTableColorFilter::Make
