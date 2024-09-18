Since m129:

* Upstream removed `SkColorFilter::filterColor`, so `ColorFilter.filterColor` is now emulated.


Since m128 (last beta release):

* We now build for Mac OS 11.0 (instead of 10.13; github CI warning)

* We now binds uptream's skparagraph, to provide multi-line text paragraph layout.
  The functionality is under the `skia.textlayout` namespace, as `Paragraph`, etc.

  * There is a `FontMgr.OneFontMgr()` method which takes a font file or data, which returns
    a font manager having knowledge of exactly one font. `FontMgr.New_Custom_Empty()` has been
    overloaded as an alias to this too.

  * The `SkUnicode` class is now available under python as `skia.Unicode`.

* Improved preview of default arguments in function signatures

* Some parametric tests involving `skia.SurfaceProps` removed during the m87->m116 changes
  were re-added.
