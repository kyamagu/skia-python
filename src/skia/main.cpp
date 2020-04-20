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
void initPathEffect(py::module &);
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
            Canvas.SaveLayerFlags
            Canvas.SrcRectConstraint
            Canvas.PointMode
            Canvas.QuadAAFlags
            Canvas.SaveLayerRec
            Canvas.Lattice
            Canvas.Lattice.RectType
            ClipOp
            Color4f
            ColorFilter
            ColorInfo
            ColorSpace
            ColorType
            CornerPathEffect
            DashPathEffect
            Data
            DeserialProcs
            DiscretePathEffect
            EncodedImageFormat
            FilterQuality
            Flattanable
            Font
            FontArguments
            FontHinting
            FontMetrics
            FontMgr
            FontStyle
            FontStyleSet
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
            MatrixPathEffect
            MergePathEffect
            Paint
            Path
            Path.Iter
            Path.RawIter
            Path.ArcSize
            Path.AddPathMode
            Path.SegmentMask
            Path.Verb
            PathConvexityType
            PathDirection
            PathEffect
            PathEffect.DashInfo
            PathEffect.DashType
            PathEffect.PointData
            PathEffect.PointData.PointFlags
            Path1DPathEffect
            Path1DPathEffect.Style
            Path2DPathEffect
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
            StrokePathEffect
            Surface
            Surface.AsyncReadResult
            Surface.ContentChangeMode
            Surface.BackendHandleAccess
            Surface.RescaleGamma
            Surface.BackendSurfaceAccess
            Surface.FlushFlags
            SurfaceCharacterization
            SurfaceProps
            SurfaceProps.Flags
            SurfaceProps.InitType
            TextBlob
            TextBlobBuilder
            TextEncoding
            TileMode
            TrimPathEffect
            TrimPathEffect.Mode
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
    initFont(m);
    initGrContext(m);
    initImage(m);
    initImageInfo(m);
    initPaint(m);
    initPath(m);
    initPathEffect(m);
    initPicture(m);
    initPixmap(m);
    initTextBlob(m);
    initVertices(m);

    initCanvas(m);
    initSurface(m);

#ifdef VERSION_INFO
    m.attr("__version__") = STRING(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
