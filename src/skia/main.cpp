#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

// Declarations.
void initAutoCanvasRestore(py::module &);
void initBitmap(py::module &);
void initBlendMode(py::module &);
void initCanvas(py::module &);
void initColor(py::module &);
void initData(py::module &);
void initGrContext(py::module &);
void initFont(py::module &);
void initImage(py::module &);
void initImageInfo(py::module &);
void initIPoint(py::module &);
void initIRect(py::module &);
void initISize(py::module &);
void initMatrix(py::module &);
void initPaint(py::module &);
void initPath(py::module &);
void initPicture(py::module &);
void initPixmap(py::module &);
void initRect(py::module &);
void initRegion(py::module &);
void initRRect(py::module &);
void initSurface(py::module &);
void initTextBlob(py::module &);
void initTextBlobBuilder(py::module &);
void initVertices(py::module &);

// Main entry point.
PYBIND11_MODULE(skia, m) {
    m.doc() = R"docstring(
        Pybind11 skia plugin
        --------------------
        .. currentmodule:: skia
        .. autosummary::
            :toctree: _generate
            AutoCanvasRestore
            Bitmap
            BlendMode
            Canvas
            Color
            ColorSpace
            Data
            Font
            Image
            ImageInfo
            IPoint
            IRect
            ISize
            Matrix
            Paint
            Path
            Picture
            Pixmap
            Rect
            Region
            RRect
            Surface
            TextBlob
            TextBlobBuilder
    )docstring";

    initAutoCanvasRestore(m);
    initBitmap(m);
    initBlendMode(m);
    initCanvas(m);
    initColor(m);
    initData(m);
    initFont(m);
    initGrContext(m);
    initImage(m);
    initImageInfo(m);
    initIPoint(m);
    initIRect(m);
    initISize(m);
    initMatrix(m);
    initPaint(m);
    initPath(m);
    initPicture(m);
    initPixmap(m);
    initRect(m);
    initRegion(m);
    initRRect(m);
    initSurface(m);
    initTextBlob(m);
    initTextBlobBuilder(m);
    initVertices(m);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
