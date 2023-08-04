#include "common.h"
#include <modules/svg/include/SkSVGDOM.h>
#include <modules/svg/include/SkSVGNode.h>
#include <modules/svg/include/SkSVGSVG.h>
#include <modules/svg/include/SkSVGRenderContext.h>

void initSVGDOM(py::module &m) {
py::class_<SkSVGDOM, sk_sp<SkSVGDOM>, SkRefCnt> SVGDOM(m, "SVGDOM");

SVGDOM
    // .def_static("MakeFromDOM", &SkSVGDOM::MakeFromDOM, py::arg("dom"))
    .def_static("MakeFromStream", &SkSVGDOM::MakeFromStream, py::arg("stream"))
    .def("containerSize", &SkSVGDOM::containerSize)
    .def("setContainerSize", &SkSVGDOM::setContainerSize)
    // .def("setRoot", &SkSVGDOM::setRoot)
    // .def("findNodeById", &SkSVGDOM::findNodeById)
    .def("render", &SkSVGDOM::render)
    .def("renderNode",
        [] (const SkSVGDOM& self, SkCanvas* canvas, const char* id) {
            /*
             * Just Make up a new default
             * SkSVGPresentationContext for now.
             */
            SkSVGPresentationContext pctx;
            return self.renderNode(canvas, pctx, id);
        })
    ;
}
