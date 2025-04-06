Since m134:

- `skia.PDF.MakeDocument()` is rewritten to give old behavior.
  Upstream's `SkPDFMetdata` has added 2 required fields `jpegDecoder` and `jpegEncoder`.
  `SkPDF::MakeDocument(SkWStream*)` now requires them supplied to make a reasonable PDF.
  `skia.PDF.Metadata()` (without arguments) is now initialized with Skia's built-in jpeg
  encoder and decoder. The bare `SkPDF::Metadata` constructor without jpeg
  encoder/decoder set is not exposed to Python code at the moment. REVISIT when
  there is valid usage of alternative jpeg encoder/decoder to the built-in.

- Removing a few @pytest.mark.skip's before @pytest.fixture.
  They have no effect and now generates warning. Mistakenly added a while ago.
