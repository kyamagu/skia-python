#include "common.h"

#define XSTRING(s) STRING(s)
#define STRING(s) #s

// Declarations.
void initBitmap(py::module &);
void initBlendMode(py::module &);
void initCanvas(py::module&);
void initColor(py::module &);
void initCodec(py::module &);
void initColorSpace(py::module &);
void initData(py::module &);
void initDocument(py::module &);
void initGrDirectContext(py::module &);
void initFont(py::module &);
void initImage(py::module &);
void initImageInfo(py::module &);
void initMatrix(py::module &);
void initPaint(py::module&);
void initPath(py::module &);
void initPathMeasure(py::module &);
void initPicture(py::module &);
void initPixmap(py::module &);
void initPoint(py::module&);
void initRect(py::module&);
void initRefCnt(py::module&);
void initRegion(py::module&);
void initSamplingOptions(py::module&);
void initSize(py::module&);
void initStream(py::module &);
void initString(py::module&);
void initSurface(py::module &);
void initTextBlob(py::module &);
void initVertices(py::module &);
void initSVGDOM(py::module &);

// Main entry point.
PYBIND11_MODULE(skia, m)
{
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
    initSamplingOptions(m);
    initStream(m);
    initString(m);
    initCodec(m);
    initBitmap(m);
    initDocument(m);
    initFont(m);
    initGrDirectContext(m);
    initImageInfo(m);
    initImage(m);
    initPaint(m);
    initPath(m);
    initPathMeasure(m);
    initPicture(m);
    initPixmap(m);
    initTextBlob(m);
    initVertices(m);
    initCanvas(m);
    initSurface(m);
    // todo   initSVGDOM(m);

#ifdef VERSION_INFO
    m.attr("__version__") = XSTRING(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif

}
