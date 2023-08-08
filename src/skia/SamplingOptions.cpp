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

    samplingoptions
        .def(py::init<>());
}
