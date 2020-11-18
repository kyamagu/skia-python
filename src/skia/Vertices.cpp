#include "common.h"
#include <pybind11/stl.h>

namespace {

sk_sp<SkVertices> MakeCopy(
    SkVertices::VertexMode mode,
    const std::vector<SkPoint>& positions,
    py::object texs,
    py::object colors,
    py::object indices) {
    int vertexCount = positions.size();
    if (vertexCount == 0)
        throw py::value_error("Vertex must have at least one element");
    std::vector<SkPoint> texs_;
    std::vector<SkColor> colors_;
    if (!texs.is_none()) {
        texs_ = texs.cast<std::vector<SkPoint>>();
        if (texs_.size() != size_t(vertexCount))
            throw py::value_error("Texs and positions must have the same size");
    }
    if (!colors.is_none()) {
        colors_ = colors.cast<std::vector<SkColor>>();
        if (colors_.size() != size_t(vertexCount))
            throw py::value_error(
                "Colors and positions must have the same size");
    }

    if (indices.is_none())
        return SkVertices::MakeCopy(
            mode, vertexCount, positions.data(),
            (texs.is_none()) ? nullptr : texs_.data(),
            (colors.is_none()) ? nullptr : colors_.data());
    auto indices_ = indices.cast<std::vector<uint16_t>>();
    return SkVertices::MakeCopy(
        mode, vertexCount, positions.data(),
        (texs.is_none()) ? nullptr : texs_.data(),
        (colors.is_none()) ? nullptr : colors_.data(),
        indices_.size(),
        indices_.data());
}

}  // namespace

template<>
struct py::detail::has_operator_delete<SkVertices, void> : std::false_type {};

void initVertices(py::module &m) {
py::class_<SkVertices, sk_sp<SkVertices>> vertices(m, "Vertices",
    R"docstring(
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
    .def(py::init(&MakeCopy),
        R"docstring(
        Create a vertices by copying the specified arrays.

        :param skia.Vertices.VertexMode mode: vertex mode
        :param List[skia.Point] positions: List of points
        :param List[skia.Point] texs: List of texs; may be None
        :param List[int] colors: List of colors; may be None
        :param List[int] indices: Optional list of indices; may be None
        )docstring",
        py::arg("mode"), py::arg("positions"), py::arg("texs") = nullptr,
        py::arg("colors") = nullptr, py::arg("indices") = nullptr)
    .def("uniqueID", &SkVertices::uniqueID)
    .def("bounds", &SkVertices::bounds)
    .def("approximateSize", &SkVertices::approximateSize)
    .def("unique", &SkVertices::unique)
    .def("ref", &SkVertices::ref)
    .def("unref", &SkVertices::unref)
    .def("deref", &SkVertices::deref)
    .def("refCntGreaterThan", &SkVertices::refCntGreaterThan, py::arg("count"))
    .def_static("MakeCopy", &MakeCopy,
        R"docstring(
        Create a vertices by copying the specified arrays.

        :param skia.Vertices.VertexMode mode: vertex mode
        :param List[skia.Point] positions: List of points
        :param List[skia.Point] texs: List of texs; may be None
        :param List[int] colors: List of colors; may be None
        :param List[int] indices: Optional list of indices; may be None
        )docstring",
        py::arg("mode"), py::arg("positions"), py::arg("texs") = nullptr,
        py::arg("colors") = nullptr, py::arg("indices") = nullptr)
    ;
}
