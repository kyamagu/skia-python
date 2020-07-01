#include "common.h"
#include <pybind11/stl.h>

namespace {

template <typename T>
const T* GetVectorPtr(py::object obj, size_t size) {
    if (obj.is_none())
        return nullptr;
    auto values = obj.cast<std::vector<T>>();
    if (values.size() != size)
        throw py::value_error(
            "Texs/Colors and positions must have the same size");
    return &values[0];
}

sk_sp<SkVertices> MakeCopy(
    SkVertices::VertexMode mode,
    const std::vector<SkPoint>& positions,
    py::object texs,
    py::object colors,
    py::object indices) {
    int vertexCount = positions.size();
    if (vertexCount == 0)
        throw py::value_error("Vertex must have at least one element");
    auto texs_ = GetVectorPtr<SkPoint>(texs, vertexCount);
    auto colors_ = GetVectorPtr<SkColor>(colors, vertexCount);
    if (indices.is_none())
        return SkVertices::MakeCopy(
            mode, vertexCount, &positions[0], texs_, colors_);
    auto indices_ = indices.cast<std::vector<uint16_t>>();
    return SkVertices::MakeCopy(
        mode, vertexCount, &positions[0], texs_, colors_,
        indices_.size(), &indices_[0]);
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
    .def("refCntGreaterThan", &SkVertices::refCntGreaterThan)
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
