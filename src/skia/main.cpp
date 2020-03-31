#include <pybind11/pybind11.h>
#include <skia.h>

#define STRING(s) #s

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

// Declarations.
void initAutoCanvasRestore(py::module &);
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
void initSize(py::module &);
void initMatrix(py::module &);
void initPaint(py::module &);
void initPath(py::module &);
void initPicture(py::module &);
void initPixmap(py::module &);
void initPoint(py::module &);
void initRect(py::module &);
void initRegion(py::module &);
void initSurface(py::module &);
void initTextBlob(py::module &);
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
            Matrix
            Paint
            Path
            Picture
            Pixmap
            Point
            Rect
            Region
            RRect
            Size
            Surface
            TextBlob
            TextBlobBuilder
    )docstring";

    initAutoCanvasRestore(m);
    initBitmap(m);
    initBlendMode(m);
    initCanvas(m);
    initColor(m);
    initColorSpace(m);
    initData(m);
    initFont(m);
    initGrContext(m);
    initImage(m);
    initImageInfo(m);
    initMatrix(m);
    initPaint(m);
    initPath(m);
    initPicture(m);
    initPixmap(m);
    initPoint(m);
    initRect(m);
    initRegion(m);
    initSize(m);
    initSurface(m);
    initTextBlob(m);
    initVertices(m);

#ifdef VERSION_INFO
    m.attr("__version__") = STRING(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
