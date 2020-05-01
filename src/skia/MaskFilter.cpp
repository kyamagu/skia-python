#include "common.h"
#include <pybind11/stl.h>

void initMaskFilter(py::module &m) {
py::enum_<SkBlurStyle>(m, "BlurStyle", py::arithmetic())
    .value("kNormal_BlurStyle", SkBlurStyle::kNormal_SkBlurStyle,
        "fuzzy inside and outside")
    .value("kSolid_BlurStyle", SkBlurStyle::kSolid_SkBlurStyle,
        "solid inside, fuzzy outside")
    .value("kOuter_BlurStyle", SkBlurStyle::kOuter_SkBlurStyle,
        "nothing inside, fuzzy outside")
    .value("kInner_BlurStyle", SkBlurStyle::kInner_SkBlurStyle,
        "fuzzy inside, nothing outside")
    .value("kLastEnum_BlurStyle", SkBlurStyle::kLastEnum_SkBlurStyle,
        "")
    .export_values();

py::enum_<SkCoverageMode>(m, "ConvergeMode",
    R"docstring(
    Describes geometric operations (ala :py:class:`Region.Op`) that can be
    applied to coverage bytes.

    These can be thought of as variants of porter-duff (:py:class:`BlendMode`)
    modes, but only applied to the alpha channel.

    See :py:class:`MaskFilter` for ways to use these when combining two
    different masks.
    )docstring")
    .value("kUnion", SkCoverageMode::kUnion)
    .value("kIntersect", SkCoverageMode::kIntersect)
    .value("kDifference", SkCoverageMode::kDifference)
    .value("kReverseDifference", SkCoverageMode::kReverseDifference)
    .value("kXor", SkCoverageMode::kXor)
    .value("kLast", SkCoverageMode::kLast)
    .export_values();

py::class_<SkMaskFilter, sk_sp<SkMaskFilter>, SkFlattenable>(
    m, "MaskFilter",
    R"docstring(
    :py:class:`MaskFilter` is the base class for object that perform
    transformations on the mask before drawing it.

    An example subclass is Blur.

    .. rubric:: Subclasses

    .. autosummary::
        :nosignatures:

        ~skia.ShaderMaskFilter
        ~skia.TableMaskFilter
    )docstring")
    .def_static("MakeBlur", &SkMaskFilter::MakeBlur,
        R"docstring(
        Create a blur maskfilter.

        :param skia.BlurStyle style: The :py:class:`BlurStyle` to use
        :param float sigma: Standard deviation of the Gaussian blur to apply.
            Must be > 0.
        :param bool respectCTM: if true the blur's sigma is modified by the CTM.
        :return: The new blur maskfilter
        )docstring",
        py::arg("style"), py::arg("sigma"), py::arg("respectCTM") = true)
    .def_static("MakeCombine",
        [] (const SkMaskFilter& filterA, const SkMaskFilter& filterB,
            SkCoverageMode mode) {
            auto filterA_ = filterA.serialize();
            auto filterB_ = filterB.serialize();
            return SkMaskFilter::MakeCombine(
                SkMaskFilter::Deserialize(filterA_->data(), filterA_->size()),
                SkMaskFilter::Deserialize(filterB_->data(), filterB_->size()),
                mode);
        },
        R"docstring(
        Compose two maskfilters together using a coverage mode.

        Returns nullptr on failure.
        )docstring",
        py::arg("filterA"), py::arg("filterB"), py::arg("mode"))
    .def_static("MakeCompose",
        [] (const SkMaskFilter& outer, const SkMaskFilter& inner) {
            auto outer_ = outer.serialize();
            auto inner_ = inner.serialize();
            return SkMaskFilter::MakeCompose(
                SkMaskFilter::Deserialize(outer_->data(), outer_->size()),
                SkMaskFilter::Deserialize(inner_->data(), inner_->size()));
        },
        R"docstring(
        Construct a maskfilter whose effect is to first apply the inner filter
        and then apply the outer filter to the result of the inner's.

        Returns nullptr on failure.
        )docstring",
        py::arg("inner"), py::arg("outer"))
    .def_static("Deserialize",
        [] (py::buffer b) {
            auto info = b.request();
            return SkMaskFilter::Deserialize(
                info.ptr, info.shape[0] * info.strides[0]);
        },
        py::arg("data"))
    ;


py::class_<SkBlurMaskFilter>(m, "BlurMaskFilter")
    // .def_static("MakeEmboss",
    //     [] (SkScalar blurSigma, const std::vector<SkScalar>& direction,
    //         SkScalar ambient, SkScalar specular) {
    //         if (direction.size() != 3)
    //             throw std::runtime_error("direction must have 3 elements.");
    //         return SkBlurMaskFilter::MakeEmboss(
    //             blurSigma, &direction[0], ambient, specular);
    //     },
    //     py::arg("blurSigma"), py::arg("direction"), py::arg("ambient"),
    //     py::arg("specular"))
    ;


py::class_<SkShaderMaskFilter>(m, "ShaderMaskFilter")
    .def_static("Make",
        [] (const SkShader& shader) {
            auto data = shader.serialize();
            auto clone = SkShader::Deserialize(
                shader.getFlattenableType(), data->data(), data->size());
            return SkShaderMaskFilter::Make(sk_sp<SkShader>(
                reinterpret_cast<SkShader*>(clone.release())));
        });

py::class_<SkTableMaskFilter>(m, "TableMaskFilter",
    R"docstring(
    Applies a table lookup on each of the alpha values in the mask.

    Helper methods create some common tables (e.g. gamma, clipping)
    )docstring")
    .def_static("MakeGammaTable",
        [] (SkScalar gamma) {
            std::vector<uint8_t> table(256);
            SkTableMaskFilter::MakeGammaTable(&table[0], gamma);
            return py::cast(table);
        },
        R"docstring(
        Utility that sets the gamma table.

        :return: gamma table
        :rtype: List[int]
        )docstring",
        py::arg("gamma"))
    .def_static("MakeClipTable",
        [] (uint8_t min, uint8_t max) {
            std::vector<uint8_t> table(256);
            SkTableMaskFilter::MakeClipTable(&table[0], min, max);
            return py::cast(table);
        },
        R"docstring(
        Utility that creates a clipping table: clamps values below min to 0 and
        above max to 255, and rescales the remaining into 0..255.

        :return: clipping table
        :rtype: List[int]
        )docstring",
        py::arg("min"), py::arg("max"))
    .def_static("Create",
        [] (std::vector<uint8_t>& table) {
            return sk_sp<SkMaskFilter>(SkTableMaskFilter::Create(&table[0]));
        })
    .def_static("CreateGamma",
        [] (SkScalar gamma) {
            return sk_sp<SkMaskFilter>(SkTableMaskFilter::CreateGamma(gamma));
        })
    .def_static("CreateClip",
        [] (uint8_t min, uint8_t max) {
            return sk_sp<SkMaskFilter>(SkTableMaskFilter::CreateClip(min, max));
        });
}
