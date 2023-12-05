Since m116:

* Surface::flushAndSubmit() and Surface::flush() now emulated
  ("Remove legacy SkImage and SkSurface methods")
* Canvas::flush() emulated
* GrDirectContext/GrContext reorganized (#198)
* Image::getBackendTexture() / Image::flush()/Image::flushAndSubmit() emulated
* Image::encodeToData() from GPU backend now works (was broken between m87 and m116)
* new RTreeFactory class added
* PictureRecorder.FinishFlags removed
* more PictureRecorder.beginRecording() overloads
* new ScalarInfinity/ScalarNegativeInfinity defines
* bug fix in TextBlob textcount (#200)

* CI aarch64 build further split into 3rd-party and main to lower job time
* documentation: locking to sphinx==6.2.1 as an interrim measure for splinx 7 breakage.
                 uses "--pre" to build v11x.0by version of doc

Since m87 (disabled in m116):
* GrDirectContext::ComputeImageSize() now emulated
* Image::MakeFromTexture() re-enabled (#198)
* Image::makeSubset()/Image::flush() re-enabled
* Image::makeTextureImage() / Image::MakeBackendTextureFromImage() emulated
* Shader::Lerp() emulated (#198)
* Surface::MakeFromBackendTexture()/Surface::MakeFromBackendRenderTarget() re-enabled (#198)
