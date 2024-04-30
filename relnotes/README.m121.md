New in m121:

* Typeface.MakeEmpty() returns a non-null typeface which contains no glyphs. See also Typeface()
  item below.

Since m120:

* ColorFilters.Compose() re-enabled (likely accidentally disabled in m87->m116)

* TableColorFilter.MakeARGB() now emulated (was removed upstream in m116)

* Typeface() and Typeface.MakeDefault() now emit a Deprecation Warning as upstream deprecates
  "default typeface". Explicit font family, style or font file choices are recommended. If users
  really want "any font with visible glyphs" (instead of Typeface.MakeEmpty()), they probably
  should switch to use 'Typeface("")' to suppress the warning. Both Typeface() and
  Typeface.MakeDefault(), the no-argument forms, are likely to be removed in the future.

  Also avoid skia.Font() and skia.Font(None, ...) ; e.g. skia.Font(skia.Typeface('')) should be used.
