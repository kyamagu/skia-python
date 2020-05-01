#include "common.h"

template<>
struct py::detail::has_operator_delete<SkVertices, void> : std::false_type {};

void initVertices(py::module &m) {
py::class_<SkVertices, sk_sp<SkVertices>> vertices(m, "Vertices", R"docstring(
    An immutable set of vertex data that can be used with
    :py:meth:`Canvas.drawVertices`.
    )docstring");

py::enum_<SkVertices::VertexMode>(vertices, "VertexMode")
    .value("kTriangles_VertexMode",
        SkVertices::VertexMode::kTriangles_VertexMode)
    .value("kTriangleStrip_VertexMode",
        SkVertices::VertexMode::kTriangleStrip_VertexMode)
    .value("kTriangleFan_VertexMode",
        SkVertices::VertexMode::kTriangleFan_VertexMode)
    .value("kLast_VertexMode",
        SkVertices::VertexMode::kLast_VertexMode)
    .export_values()
    ;

vertices
    .def(py::init(
        [] (SkVertices::VertexMode mode, py::list data) {
            auto size = data.size();
            std::vector<SkPoint> positions;
            std::vector<SkPoint> texs;
            std::vector<SkColor> colors;
            positions.reserve(size);
            texs.reserve(size);
            colors.reserve(size);
            for (auto item : data) {
                auto tuple = item.cast<py::tuple>();
                positions.push_back(*(tuple[0].cast<const SkPoint*>()));
                texs.push_back(*(tuple[1].cast<const SkPoint*>()));
                colors.push_back(tuple[2].cast<const SkColor>());
            }
            return SkVertices::MakeCopy(
                mode, size, &positions[0], &texs[0], &colors[0]);
        }),
        R"docstring(
        Create a vertices by copying the specified arrays.

        :param skia.Vertices.VertexMode mode: vertex mode
        :param list[tuple[skia.Point, skia.Point, int]] vertices: list of (
            position, tex, color)
        )docstring")
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
}
