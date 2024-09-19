Since m129:

* Upstream removed `SkColorFilter::filterColor`, so `ColorFilter.filterColor` is now emulated.


Since m128 (last beta release):

* We now build for Mac OS 11.0 (instead of 10.13; github CI warning)

* We now bind uptream's experimental `skparagraph` module and the `SkParagraph` class,
  to provide multi-line text paragraph layout.
  The functionality is under the `skia.textlayout` namespace, as `Paragraph`, etc.

  * There is a `FontMgr.OneFontMgr()` method which takes a font file or data, which returns
    a font manager having knowledge of exactly one font. `FontMgr.New_Custom_Empty()` has been
    overloaded as an alias to this, too.

  * The `SkUnicode` class is now available under python as `skia.Unicode`.

  * There are two examples `shape_text.py` (a python port of upstream's example), and
    `skparagraph-example.py` hosted [elsewhere](https://github.com/HinTak/skia-python-examples/).

  * Note that the entire `skparagraph` module is still experimental and subjected to change.
    Font choices on Linux are sensitive to LANG and FC_LANG, and you may need to set/unset
    them for desired outcome.
    See [filed issue upstream](https://issues.skia.org/361963992) for details and updates.

* Improved preview of default arguments in function signatures

* Some parametric tests involving `skia.SurfaceProps` removed during the m87->m116 changes
  are re-added.
