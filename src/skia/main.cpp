#include "common.h"

#define STRING(s) #s

// Declarations.
void initBitmap(py::module &);
void initBlendMode(py::module &);
void initCanvas(py::module &);
void initColor(py::module &);
void initColorSpace(py::module &);
void initData(py::module &);
void initGrContext(py::module &);
void initFont(py::module &);
void initImage(py::module &);
void initImageInfo(py::module &);
void initMatrix(py::module &);
void initPaint(py::module &);
void initPath(py::module &);
void initPicture(py::module &);
void initPixmap(py::module &);
void initPoint(py::module &);
void initRect(py::module &);
void initRegion(py::module &);
void initSize(py::module &);
void initSurface(py::module &);
void initTextBlob(py::module &);
void initVertices(py::module &);

// Main entry point.
PYBIND11_MODULE(skia, m) {
    m.doc() = R"docstring(
        skia
        ----

        Python Skia binding.

        .. currentmodule:: skia

        .. autosummary::
            :toctree: _generate
            :nosignatures:
            :template: class.rst

            AlphaType
            ApplyPerspectiveClip
            AutoCanvasRestore
            BBoxHierarchy
            BackingFit
            Bitmap
            BlendMode
            BlendModeCoeff
            Budgeted
            Canvas
            ClipOp
            Color4f
            ColorFilter
            ColorInfo
            ColorSpace
            ColorType
            Data
            DeserialProcs
            EncodedImageFormat
            FilterQuality
            Font
            FontArguments
            FontHinting
            FontMetrics
            FontStyle
            GrBackendApi
            GrBackendSemaphore
            GrBackendTexture
            GrContext
            GrFlushFlags
            GrGLBackendState
            GrGLInterface
            GrMipMapped
            GrProtected
            GrRenderable
            GrSemaphoresSubmitted
            GrSurfaceOrigin
            IPoint
            IRect
            ISize
            Image
            ImageFilter
            ImageInfo
            M44
            MaskFilter
            Matrix
            Paint
            Path
            PathConvexityType
            PathDirection
            PathEffect
            PathFillType
            PathSegmentMask
            PathVerb
            Picture
            PictureRecorder
            PixelGeometry
            Pixmap
            Point
            Point3
            RRect
            RSXform
            Rect
            Region
            Shader
            Size
            Surface
            SurfaceCharacterization
            SurfaceProps
            TextBlob
            TextBlobBuilder
            TextEncoding
            TileMode
            Typeface
            Vertices
            YUVColorSpace
    )docstring";

    initBlendMode(m);
    initColor(m);
    initColorSpace(m);
    initSize(m);
    initPoint(m);
    initRect(m);
    initRegion(m);
    initMatrix(m);
    initData(m);

    initBitmap(m);
    initCanvas(m);
    initFont(m);
    initGrContext(m);
    initImage(m);
    initImageInfo(m);
    initPaint(m);
    initPath(m);
    initPicture(m);
    initPixmap(m);
    initSurface(m);
    initTextBlob(m);
    initVertices(m);

#ifdef VERSION_INFO
    m.attr("__version__") = STRING(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
