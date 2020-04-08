#include "common.h"

template<>
struct py::detail::has_operator_delete<SkVertices, void> : std::false_type {};

void initVertices(py::module &m) {
py::class_<SkVertices, sk_sp<SkVertices>> vertices(m, "Vertices", R"docstring(
    An immutable set of vertex data that can be used with SkCanvas::drawVertices.
    )docstring");

vertices
    .def("uniqueID", &SkVertices::uniqueID)
    .def("bounds", &SkVertices::bounds)
    .def("approximateSize", &SkVertices::approximateSize)
    .def("encode", &SkVertices::encode,
        "Pack the vertices object into a byte buffer.")
    // .def("getInfo", &SkVertices::getInfo)
    .def("unique", &SkVertices::unique)
    .def("ref", &SkVertices::ref)
    .def("unref", &SkVertices::unref)
    .def("deref", &SkVertices::deref)
    .def("refCntGreaterThan", &SkVertices::refCntGreaterThan)
    .def_static("MakeCopy",
        (sk_sp<SkVertices> (*)(SkVertices::VertexMode, int, const SkPoint[],
            const SkPoint[], const SkColor[], int, const uint16_t[]))
        &SkVertices::MakeCopy,
        "Create a vertices by copying the specified arrays.")
    .def_static("MakeCopy",
        (sk_sp<SkVertices> (*)(SkVertices::VertexMode, int, const SkPoint[],
            const SkPoint[], const SkColor[]))
        &SkVertices::MakeCopy)
    .def_static("Decode", &SkVertices::Decode,
        "Recreate a vertices from a buffer previously created by calling "
        "encode().")
    ;

py::enum_<SkVertices::VertexMode>(vertices, "VertexMode")
    .value("kTriangles_VertexMode",
        SkVertices::VertexMode::kTriangles_VertexMode)
    .value("kTriangleStrip_VertexMode",
        SkVertices::VertexMode::kTriangleStrip_VertexMode)
    .value("kTriangleFan_VertexMode",
        SkVertices::VertexMode::kTriangleFan_VertexMode)
    .value("kLast_VertexMode", SkVertices::VertexMode::kLast_VertexMode)
    .export_values()
    ;
}
