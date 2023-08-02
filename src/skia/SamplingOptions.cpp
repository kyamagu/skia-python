#include "common.h"
#include <include/core/SkSamplingOptions.h>

void initSamplingOptions(py::module& m) {
    py::class_<SkSamplingOptions> samplingoptions(m, "SamplingOptions");

    samplingoptions
        .def(py::init<>());
}
