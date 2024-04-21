#include "common.h"
#include <include/core/SkSamplingOptions.h>

void initSamplingOptions(py::module& m) {
    py::class_<SkSamplingOptions> samplingoptions(m, "SamplingOptions");

    py::enum_<SkFilterMode>(m, "FilterMode")
        .value("kNearest", SkFilterMode::kNearest,
            "single sample point (nearest neighbor)")
        .value("kLinear", SkFilterMode::kLinear,
            "interporate between 2x2 sample points (bilinear interpolation)")
        .export_values();

    py::enum_<SkMipmapMode>(m, "MipmapMode")
        .value("kNone", SkMipmapMode::kNone,
            "ignore mipmap levels, sample from the \"base\"")
        .value("kNearest", SkMipmapMode::kNearest,
            "sample from the nearest level")
        .value("kLinear", SkMipmapMode::kLinear,
            "interpolate between the two nearest levels")
        .export_values();

    py::class_<SkCubicResampler>(m, "CubicResampler")
        .def_readwrite("B", &SkCubicResampler::B)
        .def_readwrite("C", &SkCubicResampler::C)
        .def_static("Mitchell", &SkCubicResampler::Mitchell)
        .def_static("CatmullRom", &SkCubicResampler::CatmullRom);

    samplingoptions
        .def(py::init<>())
        .def(py::init<SkSamplingOptions>(),
            py::arg("SamplingOptions"))
        .def(py::init<SkCubicResampler>(),
            py::arg("CubicResampler"))
        .def(py::init<SkFilterMode, SkMipmapMode>(),
            py::arg("FilterMode"), py::arg("MipmapMode"))
        .def_readonly("maxAniso", &SkSamplingOptions::maxAniso)
        .def_readonly("useCubic", &SkSamplingOptions::useCubic)
        .def_readonly("cubic", &SkSamplingOptions::cubic)
        .def_readonly("filter", &SkSamplingOptions::filter)
        .def_readonly("mipmap", &SkSamplingOptions::mipmap)
        .def_static("Aniso", &SkSamplingOptions::Aniso, py::arg("maxAniso"))
        .def("__eq__",
            [](const SkSamplingOptions& self, const SkSamplingOptions& other) {
                return self == other;
            },
            py::is_operator())
        .def("__ne__",
            [](const SkSamplingOptions& self, const SkSamplingOptions& other) {
                return self != other;
            },
            py::is_operator());
}
