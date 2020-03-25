#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

void initPaint(py::module &m) {
py::enum_<SkFilterQuality>(m, "FilterQuality");
// Paint
py::class_<SkPaint> paint(m, "Paint");
py::enum_<SkPaint::Style>(paint, "Style");
py::enum_<SkPaint::Cap>(paint, "Cap");
py::enum_<SkPaint::Join>(paint, "Join");
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
    ;
// Shader
py::class_<SkShader, sk_sp<SkShader>>(m, "Shader")
    .def("isOpaque", &SkShader::isOpaque,
        "Returns true if the shader is guaranteed to produce only opaque "
        "colors, subject to the SkPaint using the shader to apply an opaque "
        "alpha value.")
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
