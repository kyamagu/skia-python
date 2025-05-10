#include <stdexcept>
#include "common.h"
#include <include/effects/SkRuntimeEffect.h>
//#include <include/core/SkM44.h> // defines SkV2, SkV3, SkV4 ; M44 used in Matrix/Canvas ; Revisit.
#include <pybind11/stl_bind.h>

PYBIND11_MAKE_OPAQUE(std::vector<SkRuntimeEffect::ChildPtr>)

void initRuntimeEffect(py::module &m) {
py::class_<SkRuntimeEffect, sk_sp<SkRuntimeEffect>, SkRefCnt> runtime_effect(m, "RuntimeEffect");

py::class_<SkRuntimeEffect::ChildPtr> runtime_effect_childptr(m, "RuntimeEffectChildPtr");

py::bind_vector<std::vector<SkRuntimeEffect::ChildPtr>>(m, "VectorRuntimeEffectChildPtr");
py::class_<SkSpan<const SkRuntimeEffect::ChildPtr>> span_runtime_effect_childptr(m, "SpanRuntimeEffectChildPtr");
py::class_<SkSpan<SkRuntimeEffect::Uniform const>> span_runtime_effect_uniform(m, "SpanRuntimeEffectUniform");

py::class_<SkRuntimeEffectBuilder> runtime_effect_builder(m, "RuntimeEffectBuilder");

py::enum_<SkRuntimeEffect::Uniform::Type>(runtime_effect, "UniformType")
    .value("kFloat",    SkRuntimeEffect::Uniform::Type::kFloat)
    .value("kFloat2",   SkRuntimeEffect::Uniform::Type::kFloat2)
    .value("kFloat3",   SkRuntimeEffect::Uniform::Type::kFloat3)
    .value("kFloat4",   SkRuntimeEffect::Uniform::Type::kFloat4)
    .value("kFloat2x2", SkRuntimeEffect::Uniform::Type::kFloat2x2)
    .value("kFloat3x3", SkRuntimeEffect::Uniform::Type::kFloat3x3)
    .value("kFloat4x4", SkRuntimeEffect::Uniform::Type::kFloat4x4)
    .value("kInt",      SkRuntimeEffect::Uniform::Type::kInt)
    .value("kInt2",     SkRuntimeEffect::Uniform::Type::kInt2)
    .value("kInt3",     SkRuntimeEffect::Uniform::Type::kInt3)
    .value("kInt4",     SkRuntimeEffect::Uniform::Type::kInt4)
    .export_values();

py::class_<SkV2>(m, "V2")
    .def(py::init(
        [] (float x, float y) {
            return SkV2{x, y};
        }))
    .def(py::init(
        [] (py::tuple v2) {
            if (v2.size() != 2)
                throw py::value_error("V2 must have exactly two elements.");
            return SkV2{v2[0].cast<float>(), v2[1].cast<float>()};
        }),
        py::arg("v2"))
    ;

py::implicitly_convertible<py::tuple, SkV2>();

py::class_<SkV3>(m, "V3")
    .def(py::init(
        [] (float x, float y, float z) {
            return SkV3{x, y, z};
        }))
    .def(py::init(
        [] (py::tuple v3) {
            if (v3.size() != 3)
                throw py::value_error("V3 must have exactly three elements.");
            return SkV3{v3[0].cast<float>(), v3[1].cast<float>(), v3[2].cast<float>()};
        }),
        py::arg("v3"))
    ;

py::implicitly_convertible<py::tuple, SkV3>();

py::class_<SkV4>(m, "V4")
    .def(py::init(
        [] (float x, float y, float z, float w) {
            return SkV4{x, y, z, w};
        }))
    .def(py::init(
        [] (py::tuple v4) {
            if (v4.size() != 4)
                throw py::value_error("V4 must have exactly four elements.");
            return SkV4{v4[0].cast<float>(), v4[1].cast<float>(), v4[2].cast<float>(), v4[3].cast<float>()};
        }),
        py::arg("v4"))
    ;

py::implicitly_convertible<py::tuple, SkV4>();

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

/*
  All of these static methods check Result.effect being non-null, throw with errorText if null;
  they differ from upsteam c++ APIs, which asks clients to check.
*/
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
    .def("uniforms", &SkRuntimeEffect::uniforms)
    ;

py::class_<SkRuntimeEffectBuilder::BuilderUniform>(m, "RuntimeEffectBuilderUniform")
    .def(py::init<>())
    .def_property_readonly("name",
        [] (const SkRuntimeEffectBuilder::BuilderUniform& uniform) {
            return uniform.fVar->name;
        })
    .def_property_readonly("type",
        [] (const SkRuntimeEffectBuilder::BuilderUniform& uniform) {
            return uniform.fVar->type;
        })
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
        [] (SkRuntimeEffectBuilder& builder, std::string_view name, float uniform) {
            auto v = builder.uniform(name);
            v = uniform;
        },
        py::arg("name"), py::arg("uniform"))
    .def("setUniform",
        [] (SkRuntimeEffectBuilder& builder, std::string_view name, const SkV2& uniform) {
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
    .def("setUniform",
        [] (SkRuntimeEffectBuilder& builder, std::string_view name, py::list vN) {
          if (vN.size() != 2 && vN.size() != 3 && vN.size() != 4)
                throw py::value_error("Input must have exactly two, three or four elements.");
            auto v = builder.uniform(name);
            if (vN.size() == 2)
                v = SkV2{vN[0].cast<float>(), vN[1].cast<float>()};
            if (vN.size() == 3)
                v = SkV3{vN[0].cast<float>(), vN[1].cast<float>(), vN[2].cast<float>()};
            if (vN.size() == 4)
                v = SkV4{vN[0].cast<float>(), vN[1].cast<float>(), vN[2].cast<float>(), vN[3].cast<float>()};
        },
        py::arg("name"), py::arg("uniform"))
    .def("setChild",
        [] (SkRuntimeEffectBuilder& builder, std::string_view name, sk_sp<SkShader> child) {
            auto v = builder.child(name);
            v = child;
        },
        py::arg("name"), py::arg("child"))
    .def("setChild",
        [] (SkRuntimeEffectBuilder& builder, std::string_view name, sk_sp<SkColorFilter> child) {
            auto v = builder.child(name);
            v = child;
        },
        py::arg("name"), py::arg("child"))
    .def("setChild",
        [] (SkRuntimeEffectBuilder& builder, std::string_view name, sk_sp<SkBlender> child) {
            auto v = builder.child(name);
            v = child;
        },
        py::arg("name"), py::arg("child"))
    .def("effect", &SkRuntimeEffectBuilder::effect)
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
