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
void initRefCnt(py::module &);
void initRegion(py::module &);
void initShader(py::module &);
void initSize(py::module &);
void initSurface(py::module &);
void initTextBlob(py::module &);
void initVertices(py::module &);

// Main entry point.
PYBIND11_MODULE(skia, m) {
    m.doc() = R"docstring(
    Python Skia binding module.
    )docstring";

    initRefCnt(m);

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
    initShader(m);
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
