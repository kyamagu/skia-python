No user-visible addition since m121

Since m121:

* skia.Font(skia.Typeface('')) (empty name) seems to have been broken on Mac OS X.
  Already mentioned in README.m121 that, user should specify explicit font family
  style or font file choices. This is a general direction of moving away from choice of
  "any font", "any font manager" upstream. 


