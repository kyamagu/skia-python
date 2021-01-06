#include "common.h"
#include "experimental/svg/model/SkSVGDOM.h"
#include "experimental/svg/model/SkSVGNode.h"

void initSVGDOM(py::module &m) {
py::class_<SkSVGDOM, sk_sp<SkSVGDOM>, SkRefCnt> SVGDOM(m, "SVGDOM");

SVGDOM.def(py::init<>())
    // .def_static("MakeFromDOM", &SkSVGDOM::MakeFromDOM, py::arg("dom"))
    .def_static("MakeFromStream", &SkSVGDOM::MakeFromStream, py::arg("stream"))
    .def("containerSize", &SkSVGDOM::containerSize)
    .def("setContainerSize", &SkSVGDOM::setContainerSize)
    // .def("setRoot", &SkSVGDOM::setRoot)
    // .def("findNodeById", &SkSVGDOM::findNodeById)
    .def("render", &SkSVGDOM::render)
    ;
}
