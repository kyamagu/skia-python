#include "common.h"
#include <include/effects/SkRuntimeEffect.h>

void initRuntimeEffect(py::module &m) {
py::class_<SkRuntimeEffect, sk_sp<SkRuntimeEffect>, SkRefCnt> runtime_effect(m, "RuntimeEffect");

py::class_<SkRuntimeEffect::Result> runtime_effect_result(m, "RuntimeEffectResult");
py::class_<SkRuntimeEffect::ChildPtr> runtime_effect_childptr(m, "RuntimeEffectChildPtr");

py::class_<SkRuntimeEffectBuilder> runtime_effect_builder(m, "RuntimeEffectBuilder");

/* Should all of these static methods just check Result.effect being non-null, throw with errorText if null? */
runtime_effect
    .def_static("MakeForColorFilter", py::overload_cast<SkString, const SkRuntimeEffect::Options&>(&SkRuntimeEffect::MakeForColorFilter),
                py::arg("sksl"), py::arg("options"))
    .def_static("MakeForColorFilter", py::overload_cast<SkString>(&SkRuntimeEffect::MakeForColorFilter),
                py::arg("sksl"))
    .def_static("MakeForShader", py::overload_cast<SkString, const SkRuntimeEffect::Options&>(&SkRuntimeEffect::MakeForShader),
                py::arg("sksl"), py::arg("options"))
    .def_static("MakeForShader", py::overload_cast<SkString>(&SkRuntimeEffect::MakeForShader),
                py::arg("sksl"))
    .def_static("MakeForBlender", py::overload_cast<SkString, const SkRuntimeEffect::Options&>(&SkRuntimeEffect::MakeForBlender),
                py::arg("sksl"), py::arg("options"))
    .def_static("MakeForBlender", py::overload_cast<SkString>(&SkRuntimeEffect::MakeForBlender),
                py::arg("sksl"))
    // missing overloaded methods involving "SkSpan<const ChildPtr>..."
    .def("makeShader",
         py::overload_cast<sk_sp<const SkData>, sk_sp<SkShader>[], size_t, const SkMatrix*>(&SkRuntimeEffect::makeShader, py::const_),
         py::arg("uniforms"), py::arg("children"),
         py::arg("childCount"), py::arg("localMatrix") = nullptr)
    .def("makeColorFilter",
         py::overload_cast<sk_sp<const SkData>>(&SkRuntimeEffect::makeColorFilter, py::const_),
         py::arg("uniforms"))
    .def("makeColorFilter",
         py::overload_cast<sk_sp<const SkData>, sk_sp<SkColorFilter>[], size_t>(&SkRuntimeEffect::makeColorFilter, py::const_),
         py::arg("uniforms"), py::arg("children"),
         py::arg("childCount"))
    .def("makeBlender",
         py::overload_cast<sk_sp<const SkData>, SkSpan<const SkRuntimeEffect::ChildPtr>>(&SkRuntimeEffect::makeColorFilter, py::const_),
         py::arg("uniforms"), py::arg("children") = SkSpan<const SkRuntimeEffect::ChildPtr>{})
    ;

runtime_effect_builder
    .def(py::init<sk_sp<SkRuntimeEffect>>())
    .def(py::init<sk_sp<SkRuntimeEffect>, sk_sp<SkData>>())
    .def("uniform", &SkRuntimeEffectBuilder::uniform,
        py::arg("name"))
    .def("child", &SkRuntimeEffectBuilder::child,
        py::arg("name"))
    .def("uniforms", &SkRuntimeEffectBuilder::uniforms)
    .def("children", &SkRuntimeEffectBuilder::children)
    .def("makeShader", &SkRuntimeEffectBuilder::makeShader,
        py::arg("localMatrix") = nullptr)
    .def("makeColorFilter", &SkRuntimeEffectBuilder::makeColorFilter)
    .def("makeBlender", &SkRuntimeEffectBuilder::makeBlender)
    ;
}
