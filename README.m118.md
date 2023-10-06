Since m117:

* Remove GrBackendSurfaceMutableState
* Remove bridge code for legacy GL GrBackendSurface code
  * GrBackendFormat::MakeGL, GrBackendFormat::asGLFormat,
    GrBackendTexture::getGLTextureInfo,
    GrBackendTexture::glTextureParametersModified,
    GrBackendRenderTarget::getGLFramebufferInfo,
    now emulated
  * GL-related GrBackendTexture, GrBackendRenderTarget constructors removed
