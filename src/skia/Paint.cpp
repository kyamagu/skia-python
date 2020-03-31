#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

// Static variables must be declared.
const int SkPaint::kStyleCount;
const int SkPaint::kCapCount;
const int SkPaint::kJoinCount;

void initPaint(py::module &m) {
// Paint
py::class_<SkPaint> paint(m, "Paint");

py::enum_<SkPaint::Style>(paint, "Style")
    .value("kFill", SkPaint::Style::kFill_Style, "set to fill geometry")
    .value("kStroke", SkPaint::Style::kStroke_Style, "set to stroke geometry")
    .value("kStrokeAndFill", SkPaint::Style::kStrokeAndFill_Style,
        "sets to stroke and fill geometry")
    .export_values();

py::enum_<SkPaint::Cap>(paint, "Cap")
    .value("kButt", SkPaint::Cap::kButt_Cap, "no stroke extension")
    .value("kRound", SkPaint::Cap::kRound_Cap, "adds circle")
    .value("kSquare", SkPaint::Cap::kSquare_Cap, "adds square")
    .value("kLast", SkPaint::Cap::kLast_Cap, "largest Cap value")
    .value("kDefault", SkPaint::Cap::kDefault_Cap,
        "equivalent to kButt_Cap")
    .export_values();

py::enum_<SkPaint::Join>(paint, "Join")
    .value("kMiter", SkPaint::Join::kMiter_Join, "extends to miter limit")
    .value("kRound", SkPaint::Join::kRound_Join, "adds circle")
    .value("kBevel", SkPaint::Join::kBevel_Join, "connects outside edges")
    .value("kLast", SkPaint::Join::kLast_Join,
        "equivalent to the largest value for Join")
    .value("kDefault", SkPaint::Join::kDefault_Join,
        "equivalent to kMiter_Join")
    .export_values();

paint
    .def(py::init<>(), "Constructs SkPaint with default values.")
    .def(py::init<const SkColor4f&, SkColorSpace*>(),
        "Constructs SkPaint with default values and the given color.")
    .def(py::init<const SkPaint&>(), "Makes a shallow copy of SkPaint.")
    // .def(py::init<SkPaint&&>())
    .def("getHash", &SkPaint::getHash,
        "Returns a hash generated from SkPaint values and pointers.")
    .def("reset", &SkPaint::reset,
        "Sets all SkPaint contents to their initial values.")
    .def("isAntiAlias", &SkPaint::isAntiAlias,
        "Returns true if pixels on the active edges of SkPath may be drawn "
        "with partial transparency.")
    .def("setAntiAlias", &SkPaint::setAntiAlias,
        "Requests, but does not require, that edge pixels draw opaque or "
        "with partial transparency.")
    .def("isDither", &SkPaint::isDither,
        "Returns true if color error may be distributed to smooth color "
        "transition.")
    .def("setDither", &SkPaint::setDither,
        "Requests, but does not require, to distribute color error.")
    .def("getFilterQuality", &SkPaint::getFilterQuality,
        "Returns SkFilterQuality, the image filtering level.")
    .def("setFilterQuality", &SkPaint::setFilterQuality,
        "Sets SkFilterQuality, the image filtering level.")
    .def("getStyle", &SkPaint::getStyle,
        "Returns whether the geometry is filled, stroked, or filled and "
        "stroked.")
    .def("setStyle", &SkPaint::setStyle,
        "Sets whether the geometry is filled, stroked, or filled and stroked.")
    .def("getColor", &SkPaint::getColor,
        "Retrieves alpha and RGB, unpremultiplied, packed into 32 bits.")
    .def("getColor4f", &SkPaint::getColor4f,
        "Retrieves alpha and RGB, unpremultiplied, as four floating point values.")
    .def("setColor", py::overload_cast<SkColor>(&SkPaint::setColor),
        "Sets alpha and RGB used when stroking and filling.")
    .def("setColor",
        py::overload_cast<const SkColor4f&, SkColorSpace*>(&SkPaint::setColor),
        "Sets alpha and RGB used when stroking and filling.")
    .def("setColor4f", &SkPaint::setColor4f)
    .def("getAlphaf", &SkPaint::getAlphaf,
        "Retrieves alpha from the color used when stroking and filling.")
    .def("setAlphaf", &SkPaint::setAlphaf,
        "Replaces alpha, leaving RGB unchanged.")
    .def("getAlpha", &SkPaint::getAlpha)
    .def("setAlpha", &SkPaint::setAlpha)
    .def("setARGB", &SkPaint::setARGB,
        "Sets color used when drawing solid fills.")
    .def("getStrokeWidth", &SkPaint::getStrokeWidth,
        "Returns the thickness of the pen used by SkPaint to outline the "
        "shape.")
    .def("setStrokeWidth", &SkPaint::setStrokeWidth,
        "Sets the thickness of the pen used by the paint to outline the shape.")
    .def("getStrokeMiter", &SkPaint::getStrokeMiter,
        "Returns the limit at which a sharp corner is drawn beveled.")
    .def("setStrokeMiter", &SkPaint::setStrokeMiter,
        "Sets the limit at which a sharp corner is drawn beveled.")
    .def("getStrokeCap", &SkPaint::getStrokeCap,
        "Returns the geometry drawn at the beginning and end of strokes.")
    .def("setStrokeCap", &SkPaint::setStrokeCap,
        "Sets the geometry drawn at the beginning and end of strokes.")
    .def("getStrokeJoin", &SkPaint::getStrokeJoin,
        "Returns the geometry drawn at the corners of strokes.")
    .def("setStrokeJoin", &SkPaint::setStrokeJoin,
        "Sets the geometry drawn at the corners of strokes.")
    .def("getFillPath",
        py::overload_cast<const SkPath&, SkPath*, const SkRect*, SkScalar>(
            &SkPaint::getFillPath, py::const_),
        "Returns the filled equivalent of the stroked path.")
    .def("getFillPath",
        py::overload_cast<const SkPath&, SkPath*>(
            &SkPaint::getFillPath, py::const_),
        "Returns the filled equivalent of the stroked path.")
    .def("getShader", &SkPaint::getShader,
        "Returns optional colors used when filling a path, such as a gradient.",
        py::return_value_policy::reference)
    .def("refShader", &SkPaint::refShader,
        "Returns optional colors used when filling a path, such as a gradient.")
    .def("setShader", &SkPaint::setShader,
        "Sets optional colors used when filling a path, such as a gradient.")
    .def("getColorFilter", &SkPaint::getColorFilter,
        "Returns SkColorFilter if set, or nullptr.",
        py::return_value_policy::reference)
    .def("refColorFilter", &SkPaint::refColorFilter,
        "Returns SkColorFilter if set, or nullptr.")
    .def("setColorFilter", &SkPaint::setColorFilter,
        "Sets SkColorFilter to filter, decreasing SkRefCnt of the previous "
        "SkColorFilter.")
    .def("getBlendMode", &SkPaint::getBlendMode, "Returns SkBlendMode.")
    .def("isSrcOver", &SkPaint::isSrcOver,
        "Returns true if SkBlendMode is SkBlendMode::kSrcOver, the default.")
    .def("setBlendMode", &SkPaint::setBlendMode,
        "Sets SkBlendMode to mode.")
    .def("getPathEffect", &SkPaint::getPathEffect,
        "Returns SkPathEffect if set, or nullptr.",
        py::return_value_policy::reference)
    .def("refPathEffect", &SkPaint::refPathEffect,
        "Returns SkPathEffect if set, or nullptr.")
    .def("setPathEffect", &SkPaint::setPathEffect,
        "Sets SkPathEffect to pathEffect, decreasing SkRefCnt of the previous "
        "SkPathEffect.")
    .def("getMaskFilter", &SkPaint::getMaskFilter,
        "Returns SkMaskFilter if set, or nullptr.",
        py::return_value_policy::reference)
    .def("refMaskFilter", &SkPaint::refMaskFilter,
        "Returns SkMaskFilter if set, or nullptr.")
    .def("setMaskFilter", &SkPaint::setMaskFilter,
        "Sets SkMaskFilter to maskFilter, decreasing SkRefCnt of the previous "
        "SkMaskFilter.")
    .def("getImageFilter", &SkPaint::getImageFilter,
        "Returns SkImageFilter if set, or nullptr.",
        py::return_value_policy::reference)
    .def("refImageFilter", &SkPaint::refImageFilter,
        "Returns SkImageFilter if set, or nullptr.")
    .def("setImageFilter", &SkPaint::setImageFilter,
        "Sets SkImageFilter to imageFilter, decreasing SkRefCnt of the "
        "previous SkImageFilter.")
    .def("nothingToDraw", &SkPaint::nothingToDraw,
        "Returns true if SkPaint prevents all drawing; otherwise, the SkPaint "
        "may or may not allow drawing.")
    .def("canComputeFastBounds", &SkPaint::canComputeFastBounds,
        "(to be made private) Returns true if SkPaint does not include "
        "elements requiring extensive computation to compute SkBaseDevice "
        "bounds of drawn geometry.")
    .def("computeFastBounds", &SkPaint::computeFastBounds,
        "(to be made private) Only call this if canComputeFastBounds() "
        "returned true.")
    .def("computeFastStrokeBounds", &SkPaint::computeFastStrokeBounds,
        "(to be made private)")
    .def("doComputeFastBounds", &SkPaint::doComputeFastBounds,
        "(to be made private) Computes the bounds, overriding the SkPaint "
        "SkPaint::Style.")
    .def_readonly_static("kStyleCount", &SkPaint::kStyleCount)
    .def_readonly_static("kCapCount", &SkPaint::kCapCount)
    .def_readonly_static("kJoinCount", &SkPaint::kJoinCount)
    .def(py::self == py::self,
        "Compares a and b, and returns true if a and b are equivalent.")
    .def(py::self != py::self,
        "Compares a and b, and returns true if a and b are not equivalent.")
    ;
// Shader
// TODO: Need a wrapper class for pure virtual functions.
py::class_<SkShader, sk_sp<SkShader>> shader(m, "Shader");
py::class_<SkShader::GradientInfo>(shader, "GradientInfo")
    .def_readwrite("fColorCount", &SkShader::GradientInfo::fColorCount)
    .def_readwrite("fColors", &SkShader::GradientInfo::fColors)
    .def_readwrite("fColorOffsets", &SkShader::GradientInfo::fColorOffsets)
    // .def_readwrite("fPoint", &SkShader::GradientInfo::fPoint)
    // .def_readwrite("fRadius", &SkShader::GradientInfo::fRadius)
    .def_readwrite("fTileMode", &SkShader::GradientInfo::fTileMode)
    .def_readwrite("fGradientFlags", &SkShader::GradientInfo::fGradientFlags)
    ;

py::enum_<SkShader::GradientType>(shader, "GradientType")
    .value("kNone", SkShader::GradientType::kNone_GradientType)
    .value("kColor", SkShader::GradientType::kColor_GradientType)
    .value("kLinear", SkShader::GradientType::kLinear_GradientType)
    .value("kRadial", SkShader::GradientType::kRadial_GradientType)
    .value("kSweep", SkShader::GradientType::kSweep_GradientType)
    .value("kConical", SkShader::GradientType::kConical_GradientType)
    .value("kLast", SkShader::GradientType::kLast_GradientType)
    .export_values();

py::enum_<SkShader::Type>(shader, "Type")
    .value("kSkColorFilter", SkShader::Type::kSkColorFilter_Type)
    .value("kSkDrawable", SkShader::Type::kSkDrawable_Type)
    .value("kSkDrawLooper", SkShader::Type::kSkDrawLooper_Type)
    .value("kSkImageFilter", SkShader::Type::kSkImageFilter_Type)
    .value("kSkMaskFilter", SkShader::Type::kSkMaskFilter_Type)
    .value("kSkPathEffect", SkShader::Type::kSkPathEffect_Type)
    .value("kSkPixelRef", SkShader::Type::kSkPixelRef_Type)
    .value("kSkUnused4", SkShader::Type::kSkUnused_Type4)
    .value("kSkShaderBase", SkShader::Type::kSkShaderBase_Type)
    .value("kSkUnused", SkShader::Type::kSkUnused_Type)
    .value("kSkUnused2", SkShader::Type::kSkUnused_Type2)
    // .value("kSkUnused3", SkShader::Type::kSkUnused_Type3)
    .export_values();

shader
    .def("isOpaque", &SkShader::isOpaque,
        "Returns true if the shader is guaranteed to produce only opaque "
        "colors, subject to the SkPaint using the shader to apply an opaque "
        "alpha value.")
    .def("isAImage",
        py::overload_cast<SkMatrix*, SkTileMode[2]>(
            &SkShader::isAImage, py::const_),
        "Iff this shader is backed by a single SkImage, return its ptr (the "
        "caller must ref this if they want to keep it longer than the lifetime "
        "of the shader).")
    .def("isAImage", py::overload_cast<>(&SkShader::isAImage, py::const_))
    .def("asAGradient", &SkShader::asAGradient)
    .def("makeWithLocalMatrix", &SkShader::makeWithLocalMatrix,
        "Return a shader that will apply the specified localMatrix to this "
        "shader.")
    .def("makeWithColorFilter", &SkShader::makeWithColorFilter,
        "Create a new shader that produces the same colors as invoking this "
        "shader and then applying the colorfilter.")
    // .def("getFactory", &SkShader::getFactory,
    //     "Implement this to return a factory function pointer that can be "
    //     "called to recreate your class given a buffer (previously written to "
    //     "by your override of flatten().")
    // .def("getTypeName", &SkShader::getTypeName,
    //     "Returns the name of the object's class.")
    // .def("flatten", &SkShader::flatten,
    //     "Override this if your subclass needs to record data that it will "
    //     "need to recreate itself from its CreateProc (returned by "
    //     "getFactory())")
    // .def("getFlattenableType", &SkShader::getFlattenableType)
    // .def("serialize",
    //     py::overload_cast<const SkSerialProcs*>(
    //         &SkShader::serialize, py::const_))
    // .def("serialize",
    //     py::overload_cast<void*, size_t, const SkSerialProcs*>(
    //         &SkShader::serialize, py::const_))
    .def("unique", &SkShader::unique,
        "May return true if the caller is the only owner.")
    .def("ref", &SkShader::ref, "Increment the reference count.")
    .def("unref", &SkShader::unref, "Decrement the reference count.")
    ;
// ColorFilter
py::class_<SkColorFilter, sk_sp<SkColorFilter>>(m, "ColorFilter");
// PathEffect
py::class_<SkPathEffect, sk_sp<SkPathEffect>>(m, "PathEffect");
// MaskFilter
py::class_<SkMaskFilter, sk_sp<SkMaskFilter>>(m, "MaskFilter");
// ImageFilter
py::class_<SkImageFilter, sk_sp<SkImageFilter>>(m, "ImageFilter");
}
