#include <stdexcept>
#include "common.h"
#include <include/effects/SkRuntimeEffect.h>
//#include <include/core/SkM44.h> // defines SkV3, SkV4 ; M44 used in Matrix/Canvas ; Revisit.
#include <pybind11/stl_bind.h>

PYBIND11_MAKE_OPAQUE(std::vector<SkRuntimeEffect::ChildPtr>)

void initRuntimeEffect(py::module &m) {
py::class_<SkRuntimeEffect, sk_sp<SkRuntimeEffect>, SkRefCnt> runtime_effect(m, "RuntimeEffect");

py::class_<SkRuntimeEffect::ChildPtr> runtime_effect_childptr(m, "RuntimeEffectChildPtr");

py::bind_vector<std::vector<SkRuntimeEffect::ChildPtr>>(m, "VectorSkRuntimeEffectChildPtr");
py::class_<SkSpan<const SkRuntimeEffect::ChildPtr>> span_runtime_effect_childptr(m, "SpanRuntimeEffectChildPtr");

py::class_<SkRuntimeEffectBuilder> runtime_effect_builder(m, "RuntimeEffectBuilder");

py::class_<SkV3>(m, "V3")
    .def(py::init(
        [] (float x, float y, float z) {
            return SkV3{x, y, z};
        }))
    ;

py::class_<SkV4>(m, "V4")
    .def(py::init(
        [] (float x, float y, float z, float w) {
            return SkV4{x, y, z, w};
        }))
    ;

py::class_<SkRuntimeEffect::Result>(m, "RuntimeEffectResult")
    .def_readwrite("effect", &SkRuntimeEffect::Result::effect)
    .def_readwrite("errorText", &SkRuntimeEffect::Result::errorText)
    ;

runtime_effect_childptr
    .def(py::init<>())
    .def(py::init<sk_sp<SkShader>>())
    .def(py::init<sk_sp<SkColorFilter>>())
    .def(py::init<sk_sp<SkBlender>>())
    ;

span_runtime_effect_childptr
    .def(py::init<>())
    .def(py::init<const SkRuntimeEffect::ChildPtr*, size_t>())
    .def(py::init<const SkSpan<const SkRuntimeEffect::ChildPtr>&>())
    .def(py::init(
        [] (std::vector<SkRuntimeEffect::ChildPtr>& v) {
            return SkSpan<SkRuntimeEffect::ChildPtr>(&v[0], v.size());
        }))
    .def(py::init<const SkSpan<SkRuntimeEffect::ChildPtr>&>())
    ;

py::implicitly_convertible<sk_sp<SkShader>, SkRuntimeEffect::ChildPtr>();
py::implicitly_convertible<sk_sp<SkColorFilter>, SkRuntimeEffect::ChildPtr>();
py::implicitly_convertible<sk_sp<SkBlender>, SkRuntimeEffect::ChildPtr>();
py::implicitly_convertible<std::vector<SkRuntimeEffect::ChildPtr>, SkSpan<const SkRuntimeEffect::ChildPtr>>();

/* Should all of these static methods just check Result.effect being non-null, throw with errorText if null? */
runtime_effect
    .def_static("MakeForColorFilter",
        [] (SkString sksl, const SkRuntimeEffect::Options& options) {
            auto [effect, err] = SkRuntimeEffect::MakeForColorFilter(sksl, options);
            if (!effect)
                throw std::runtime_error(err.data());
            return effect;
        },
        py::arg("sksl"), py::arg("options"))
    .def_static("MakeForColorFilter",
        [] (SkString sksl) {
            auto [effect, err] = SkRuntimeEffect::MakeForColorFilter(sksl);
            if (!effect)
                throw std::runtime_error(err.data());
            return effect;
        },
        py::arg("sksl"))
    .def_static("MakeForShader",
        [] (SkString sksl, const SkRuntimeEffect::Options& options) {
            auto [effect, err] = SkRuntimeEffect::MakeForShader(sksl, options);
            if (!effect)
                throw std::runtime_error(err.data());
            return effect;
        },
        py::arg("sksl"), py::arg("options"))
    .def_static("MakeForShader",
        [] (SkString sksl) {
            auto [effect, err] = SkRuntimeEffect::MakeForShader(sksl);
            if (!effect)
                throw std::runtime_error(err.data());
            return effect;
        },
        py::arg("sksl"))
    .def_static("MakeForBlender",
        [] (SkString sksl, const SkRuntimeEffect::Options& options) {
            auto [effect, err] = SkRuntimeEffect::MakeForBlender(sksl, options);
            if (!effect)
                throw std::runtime_error(err.data());
            return effect;
        },
        py::arg("sksl"), py::arg("options"))
    .def_static("MakeForBlender",
        [] (SkString sksl) {
            auto [effect, err] = SkRuntimeEffect::MakeForBlender(sksl);
            if (!effect)
                throw std::runtime_error(err.data());
            return effect;
        },
        py::arg("sksl"))
    .def("makeShader",
        [] (SkRuntimeEffect& runtime_effect, sk_sp<const SkData> uniforms) {
            return runtime_effect.makeShader(uniforms, {});
        },
         py::arg("uniforms"))
    .def("makeShader",
        py::overload_cast<sk_sp<const SkData>, sk_sp<SkShader>[], size_t, const SkMatrix*>(&SkRuntimeEffect::makeShader, py::const_),
        py::arg("uniforms"), py::arg("children"),
        py::arg("childCount"), py::arg("localMatrix") = nullptr)
    .def("makeShader",
        py::overload_cast<sk_sp<const SkData>, SkSpan<const SkRuntimeEffect::ChildPtr>, const SkMatrix*>(&SkRuntimeEffect::makeShader, py::const_),
        py::arg("uniforms"), py::arg("children"),
        py::arg("localMatrix") = nullptr)
    .def("makeColorFilter",
        py::overload_cast<sk_sp<const SkData>>(&SkRuntimeEffect::makeColorFilter, py::const_),
        py::arg("uniforms"))
    .def("makeColorFilter",
        py::overload_cast<sk_sp<const SkData>, sk_sp<SkColorFilter>[], size_t>(&SkRuntimeEffect::makeColorFilter, py::const_),
        py::arg("uniforms"), py::arg("children"),
        py::arg("childCount"))
    .def("makeColorFilter",
        py::overload_cast<sk_sp<const SkData>, SkSpan<const SkRuntimeEffect::ChildPtr>>(&SkRuntimeEffect::makeColorFilter, py::const_),
        py::arg("uniforms"), py::arg("children"))
    .def("makeBlender",
        [] (SkRuntimeEffect& runtime_effect, sk_sp<const SkData> uniforms) {
            return runtime_effect.makeBlender(uniforms, {});
        },
        py::arg("uniforms"))
    .def("makeBlender",
        py::overload_cast<sk_sp<const SkData>, SkSpan<const SkRuntimeEffect::ChildPtr>>(&SkRuntimeEffect::makeBlender, py::const_),
        py::arg("uniforms"), py::arg("children") = SkSpan<const SkRuntimeEffect::ChildPtr>{})
    ;

py::class_<SkRuntimeEffectBuilder::BuilderUniform>(m, "RuntimeEffectBuilderUniform")
    .def(py::init<>())
    ;

py::class_<SkRuntimeEffectBuilder::BuilderChild>(m, "RuntimeEffectBuilderChild")
    .def(py::init<>())
    ;

runtime_effect_builder
    .def(py::init<sk_sp<SkRuntimeEffect>>())
    .def(py::init<sk_sp<SkRuntimeEffect>, sk_sp<SkData>>())
    .def("uniform", &SkRuntimeEffectBuilder::uniform,
        py::arg("name"))
    .def("child", &SkRuntimeEffectBuilder::child,
        py::arg("name"))
    .def("setUniform",
        [] (SkRuntimeEffectBuilder& builder, std::string_view name, int uniform) {
            auto v = builder.uniform(name);
            v = uniform;
        },
        py::arg("name"), py::arg("uniform"))
    .def("setUniform",
        [] (SkRuntimeEffectBuilder& builder, std::string_view name, const SkV3& uniform) {
            auto v = builder.uniform(name);
            v = uniform;
        },
        py::arg("name"), py::arg("uniform"))
    .def("setUniform",
        [] (SkRuntimeEffectBuilder& builder, std::string_view name, const SkV4& uniform) {
            auto v = builder.uniform(name);
            v = uniform;
        },
        py::arg("name"), py::arg("uniform"))
    .def("setChild",
        [] (SkRuntimeEffectBuilder& builder, std::string_view name, sk_sp<SkShader> child) {
            auto v = builder.child(name);
            v = child;
        },
        py::arg("name"), py::arg("child"))
    .def("uniforms", &SkRuntimeEffectBuilder::uniforms)
    .def("children", &SkRuntimeEffectBuilder::children)
    .def("makeShader", &SkRuntimeEffectBuilder::makeShader,
        py::arg("localMatrix") = nullptr)
    .def("makeColorFilter", &SkRuntimeEffectBuilder::makeColorFilter)
    .def("makeBlender", &SkRuntimeEffectBuilder::makeBlender)
    ;

m.attr("RuntimeShaderBuilder") = m.attr("RuntimeEffectBuilder");
m.attr("RuntimeColorFilterBuilder") = m.attr("RuntimeEffectBuilder");
m.attr("RuntimeBlendBuilder") = m.attr("RuntimeEffectBuilder");
}
