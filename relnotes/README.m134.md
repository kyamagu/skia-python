Wheel building:

- Upgrading to manylinux_2_28 (from manylinux2014)


Bug fixes:

- Adding a __iter__ method to skia.Iter (compatibility with python 3.13.1)

- Avoid intermittent segfault in Typeface.getKerningPairAdjustments()


Additions / Enhancements:

- Binding SkFontMgr_New_Custom_Directory as FontMgr.New_Custom_Directory()

- Binding skia::textlayout::TypefaceFontProvider as skia.textlayout.TypefaceFontProvider()

- Binding skia::textlayout::StrutStyle as skia.textlayout.StrutStyle() class,
  adding textlayout.ParagraphStyle.setStrutStyle() method,
  and textlayout.TextStyle.setLetterSpacing / setWordSpacing methods.

