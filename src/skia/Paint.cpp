#include "common.h"
#include <include/core/SkPathUtils.h>
#include <pybind11/operators.h>


void initColorFilter(py::module &);
void initPathEffect(py::module &);
void initShader(py::module &);
void initMaskFilter(py::module &);
void initImageFilter(py::module &);

// Static variables must be declared.
const int SkPaint::kStyleCount;
const int SkPaint::kCapCount;
const int SkPaint::kJoinCount;

class PyFlattanable : public SkFlattenable {
public:
    using SkFlattenable::SkFlattenable;
    Factory getFactory() const override {
        PYBIND11_OVERLOAD_PURE(Factory, SkFlattenable, getFactory);
    }
    const char* getTypeName() const override {
        PYBIND11_OVERLOAD_PURE(const char*, SkFlattenable, getTypeName);
    }
    Type getFlattenableType() const override {
        PYBIND11_OVERLOAD_PURE(Type, SkFlattenable, getFlattenableType);
    }
};

namespace {

SkPaint MakeFromDict(py::dict dict) {
    SkPaint paint;
    for (auto item : dict) {
        std::string key(py::str(item.first));
        auto value = item.second;
        if (key == "Alpha")
            paint.setAlpha(value.cast<uint8_t>());
        else if (key == "Alphaf")
            paint.setAlphaf(value.cast<float>());
        else if (key == "AntiAlias")
            paint.setAntiAlias(value.cast<bool>());
        else if (key == "ARGB") {
            auto t = value.cast<py::tuple>();
            if (t.size() != 3)
                throw py::value_error("ARGB must be 4-element tuple.");
            paint.setARGB(
                t[0].cast<uint8_t>(),
                t[1].cast<uint8_t>(),
                t[2].cast<uint8_t>(),
                t[3].cast<uint8_t>()
                );
        }
        else if (key == "BlendMode")
            paint.setBlendMode(value.cast<SkBlendMode>());
        else if (key == "Color")
            paint.setColor(value.cast<SkColor>());
        else if (key == "Color4f")
            paint.setColor4f(value.cast<SkColor4f>());  // TODO: ColorSpace
        else if (key == "ColorFilter")
            paint.setColorFilter(value.cast<sk_sp<SkColorFilter>>());
        else if (key == "Dither")
            paint.setDither(value.cast<bool>());
/*
        else if (key == "FilterQuality")
            paint.setFilterQuality(value.cast<SkFilterQuality>());
*/
        else if (key == "ImageFilter")
            paint.setImageFilter(value.cast<sk_sp<SkImageFilter>>());
        else if (key == "MaskFilter")
            paint.setMaskFilter(value.cast<sk_sp<SkMaskFilter>>());
        else if (key == "PathEffect")
            paint.setPathEffect(value.cast<sk_sp<SkPathEffect>>());
        else if (key == "Shader")
            paint.setShader(value.cast<sk_sp<SkShader>>());
        // else if (key == "Stroke")
        //     paint.setStroke(value.cast<bool>());
        else if (key == "StrokeCap")
            paint.setStrokeCap(value.cast<SkPaint::Cap>());
        else if (key == "StrokeJoin")
            paint.setStrokeJoin(value.cast<SkPaint::Join>());
        else if (key == "StrokeMiter")
            paint.setStrokeMiter(value.cast<SkScalar>());
        else if (key == "StrokeWidth")
            paint.setStrokeWidth(value.cast<SkScalar>());
        else if (key == "Style")
            paint.setStyle(value.cast<SkPaint::Style>());
        else
            throw py::key_error(key);
    }
    return paint;
}

}  // namespace


void initPaint(py::module &m) {
// Paint
py::class_<SkPaint> paint(m, "Paint", R"docstring(
    :py:class:`Paint` controls options applied when drawing.

    :py:class:`Paint` collects all options outside of the :py:class:`Canvas`
    clip and :py:class:`Canvas` matrix.

    Various options apply to strokes and fills, and images.

    :py:class:`Paint` collects effects and filters that describe single-pass and
    multiple-pass algorithms that alter the drawing geometry, color, and
    transparency. For instance, :py:class:`Paint` does not directly implement
    dashing or blur, but contains the objects that do so.

    Example::

        paint = skia.Paint(
            Alphaf=1.0,
            AntiAlias=True,
            Color=skia.ColorBLUE,
            StrokeWidth=2,
            Style=skia.Paint.kStroke_Style,
            )

    :py:class:`Paint` is implicitly convertible from python dict. It is
    possible to replace :py:class:`Paint` with ``dict`` where required::

        canvas.drawPaint({'Color': 0xFFFFFFFF})
        canvas.drawLine((0, 0), (1, 1), {'Color': 0xFF0000FF})
    )docstring");

py::enum_<SkPaint::Style>(paint, "Style")
    .value("kFill_Style", SkPaint::Style::kFill_Style,
        "set to fill geometry")
    .value("kStroke_Style", SkPaint::Style::kStroke_Style,
        "set to stroke geometry")
    .value("kStrokeAndFill_Style", SkPaint::Style::kStrokeAndFill_Style,
        "sets to stroke and fill geometry")
    .export_values();

py::enum_<SkPaint::Cap>(paint, "Cap")
    .value("kButt_Cap", SkPaint::Cap::kButt_Cap, "no stroke extension")
    .value("kRound_Cap", SkPaint::Cap::kRound_Cap, "adds circle")
    .value("kSquare_Cap", SkPaint::Cap::kSquare_Cap, "adds square")
    .value("kLast_Cap", SkPaint::Cap::kLast_Cap, "largest Cap value")
    .value("kDefault_Cap", SkPaint::Cap::kDefault_Cap,
        "equivalent to kButt_Cap")
    .export_values();

py::enum_<SkPaint::Join>(paint, "Join")
    .value("kMiter_Join", SkPaint::Join::kMiter_Join, "extends to miter limit")
    .value("kRound_Join", SkPaint::Join::kRound_Join, "adds circle")
    .value("kBevel_Join", SkPaint::Join::kBevel_Join, "connects outside edges")
    .value("kLast_Join", SkPaint::Join::kLast_Join,
        "equivalent to the largest value for Join")
    .value("kDefault_Join", SkPaint::Join::kDefault_Join,
        "equivalent to kMiter_Join")
    .export_values();

paint
    .def(py::init<>(),
        R"docstring(
        Constructs :py:class:`Paint` with default values.
        )docstring")
    .def(py::init<const SkColor4f&, SkColorSpace*>(),
        R"docstring(
        Constructs :py:class:`Paint` with default values and the given color.

        Sets alpha and RGB used when stroking and filling. The color is four
        floating point values, unpremultiplied. The color values are interpreted
        as being in the colorSpace. If colorSpace is nullptr, then color is
        assumed to be in the sRGB color space.

        :color: unpremultiplied RGBA
        :colorSpace: :py:class:`ColorSpace` describing the
            encoding of color
        )docstring",
        py::arg("color"), py::arg("colorSpace") = nullptr)
    .def(py::init<const SkPaint&>(),
        R"docstring(
        Makes a shallow copy of :py:class:`Paint`.

        :py:class:`PathEffect`, :py:class:`Shader`, :py:class:`MaskFilter`,
        :py:class:`ColorFilter`, and :py:class:`ImageFilter` are shared between
        the original paint and the copy. Objects containing :py:class:`RefCnt`
        increment their references by one.

        The referenced objects :py:class:`PathEffect`, :py:class:`Shader`,
        :py:class:`MaskFilter`, :py:class:`ColorFilter`, and
        :py:class:`ImageFilter` cannot be modified after they are created. This
        prevents objects with :py:class:`RefCnt` from being modified once
        :py:class:`Paint` refers to them.

        :paint: original to copy
        )docstring",
        py::arg("paint"))
    .def(py::init([] (py::kwargs kwargs) { return MakeFromDict(kwargs); }),
        R"docstring(
        Constructs :py:class:`Paint` with keyword arguments. See ``setXXX``
        methods for required signatures.

        Example::

            paint = skia.Paint(
                Alpha=255,
                Alphaf=1.0,
                AntiAlias=True,
                Color=0xFFFFFFFF,
                Color4f=skia.Color4f.FromColor(0xFF00FF00),
                ColorFilter=skia.LumaColorFilter.Make(),
                Dither=False,
                FilterQuality=skia.kMedium_FilterQuality,
                ImageFilter=skia.ImageFilters.Blur(1.0, 1.0),
                MaskFilter=skia.MaskFilter.MakeBlur(skia.kNormal_BlurStyle, 1.),
                PathEffect=skia.DashPathEffect.Make([2., 1.], 0),
                Shader=skia.Shaders.Empty(),
                StrokeCap=skia.Paint.kButt_Cap,
                StrokeJoin=skia.Paint.kMiter_Join,
                StrokeMiter=0,
                StrokeWidth=2,
                Style=skia.Paint.kStroke_Style,
                )
        )docstring")
    .def(py::init(&MakeFromDict),
        R"docstring(
        Constructs :py:class:`Paint` from python dict::

            d = {
                'Alpha': 255,
                'Alphaf': 1.0,
                'AntiAlias': True,
                'Color': 0xFFFFFFFF,
                'Color4f': skia.Color4f.FromColor(0xFF00FF00),
                'ColorFilter': skia.LumaColorFilter.Make(),
                'Dither': False,
                'FilterQuality': skia.kMedium_FilterQuality,
                'ImageFilter': skia.ImageFilters.Blur(1.0, 1.0),
                'MaskFilter': skia.MaskFilter.MakeBlur(skia.kNormal_BlurStyle, 1.),
                'PathEffect': skia.DashPathEffect.Make([2., 1.], 0),
                'Shader': skia.Shaders.Empty(),
                'StrokeCap': skia.Paint.kButt_Cap,
                'StrokeJoin': skia.Paint.kMiter_Join,
                'StrokeMiter': 0,
                'StrokeWidth': 2,
                'Style': skia.Paint.kStroke_Style,
            }
            paint = skia.Paint(d)

        )docstring",
        py::arg("d").none(false))
/*
    .def("getHash", &SkPaint::getHash,
        R"docstring(
        Returns a hash generated from :py:class:`Paint` values and pointers.

        Identical hashes guarantee that the paints are equivalent, but differing
        hashes do not guarantee that the paints have differing contents.

        If operator :py:meth:`__eq__` returns true for two paints, their hashes
        are also equal.

        The hash returned is platform and implementation specific.

        :return: a shallow hash
        )docstring")
*/
    .def("reset", &SkPaint::reset,
        R"docstring(
        Sets all :py:class:`Paint` contents to their initial values.

        This is equivalent to replacing :py:class:`Paint` with the result of
        :py:meth:`__init__` without arguments.
        )docstring")
    .def("isAntiAlias", &SkPaint::isAntiAlias,
        R"docstring(
        Returns true if pixels on the active edges of :py:class:`Path` may be
        drawn with partial transparency.

        :return: antialiasing state
        )docstring")
    .def("setAntiAlias", &SkPaint::setAntiAlias,
        R"docstring(
        Requests, but does not require, that edge pixels draw opaque or with
        partial transparency.

        :param bool aa: setting for antialiasing
        )docstring",
        py::arg("aa"))
    .def("isDither", &SkPaint::isDither,
        R"docstring(
        Returns true if color error may be distributed to smooth color
        transition.

        :return: dithering state
        )docstring")
    .def("setDither", &SkPaint::setDither,
        R"docstring(
        Requests, but does not require, to distribute color error.

        :param bool dither: dither  setting for ditering
        )docstring",
        py::arg("dither"))
/*
    .def("getFilterQuality", &SkPaint::getFilterQuality,
        R"docstring(
        Returns :py:class:`FilterQuality`, the image filtering level.

        A lower setting draws faster; a higher setting looks better when the
        image is scaled.
        )docstring")
    .def("setFilterQuality", &SkPaint::setFilterQuality,
        R"docstring(
        Sets :py:class:`FilterQuality`, the image filtering level.

        A lower setting draws faster; a higher setting looks better when the
        image is scaled. Does not check to see if quality is valid.

        :param skia.FilterQuality quality: filter quality
        )docstring",
        py::arg("quality"))
*/
    .def("getStyle", &SkPaint::getStyle,
        R"docstring(
        Returns whether the geometry is filled, stroked, or filled and stroked.
        )docstring")
    .def("setStyle", &SkPaint::setStyle,
        R"docstring(
        Sets whether the geometry is filled, stroked, or filled and stroked.

        Has no effect if style is not a legal :py:class:`Paint.Style` value.
        )docstring",
        py::arg("style"))
    .def("getColor", &SkPaint::getColor,
        R"docstring(
        Retrieves alpha and RGB, unpremultiplied, packed into 32 bits.

        Use helpers :py:func:`ColorGetA`, :py:func:`ColorGetR`,
        :py:func:`ColorGetG`, and :py:func:`ColorGetB` to extract a color
        component.

        :return: unpremultiplied ARGB
        )docstring")
    .def("getColor4f", &SkPaint::getColor4f,
        R"docstring(
        Retrieves alpha and RGB, unpremultiplied, as four floating point values.

        RGB are are extended sRGB values (sRGB gamut, and encoded with the sRGB
        transfer function).

        :return: unpremultiplied RGBA
        )docstring")
    .def("setColor", py::overload_cast<SkColor>(&SkPaint::setColor),
        R"docstring(
        Sets alpha and RGB used when stroking and filling.

        The color is a 32-bit value, unpremultiplied, packing 8-bit components
        for alpha, red, blue, and green.

        :param int color: unpremultiplied ARGB
        )docstring",
        py::arg("color"))
    .def("setColor4f", &SkPaint::setColor4f,
        R"docstring(
        Sets alpha and RGB used when stroking and filling.

        The color is four floating point values, unpremultiplied. The color
        values are interpreted as being in the colorSpace. If colorSpace is
        nullptr, then color is assumed to be in the sRGB color space.

        :param skia.Color4f color: unpremultiplied RGBA
        :param skia.ColorSpace colorSpace: :py:class:`ColorSpace` describing the
            encoding of color
        )docstring",
        py::arg("color"), py::arg("colorSpace") = nullptr)
    .def("getAlphaf", &SkPaint::getAlphaf,
        R"docstring(
        Retrieves alpha from the color used when stroking and filling.

        :return: alpha ranging from zero, fully transparent, to 255, fully
            opaque
        )docstring")
    .def("setAlphaf", &SkPaint::setAlphaf,
        R"docstring(
        Replaces alpha, leaving RGB unchanged.

        An out of range value triggers an assert in the debug build. a is a
        value from 0.0 to 1.0. a set to zero makes color fully transparent; a
        set to 1.0 makes color fully opaque.

        :param float a: alpha component of color
        )docstring",
        py::arg("a"))
    .def("getAlpha", &SkPaint::getAlpha)
    .def("setAlpha", &SkPaint::setAlpha, py::arg("a"))
    .def("setARGB", &SkPaint::setARGB,
        R"docstring(
        Sets color used when drawing solid fills.

        The color components range from 0 to 255. The color is unpremultiplied;
        alpha sets the transparency independent of RGB.

        :param int a: amount of alpha, from fully transparent (0) to fully
            opaque (255)
        :param int r: amount of red, from no red (0) to full red (255)
        :param int g: amount of green, from no green (0) to full green (255)
        :param int b: amount of blue, from no blue (0) to full blue (255)
        )docstring",
        py::arg("a"), py::arg("r"), py::arg("g"), py::arg("b"))
    .def("getStrokeWidth", &SkPaint::getStrokeWidth,
        R"docstring(
        Returns the thickness of the pen used by SkPaint to outline the shape.

        :return: zero for hairline, greater than zero for pen thickness
        )docstring")
    .def("setStrokeWidth", &SkPaint::setStrokeWidth,
        R"docstring(
        Sets the thickness of the pen used by the paint to outline the shape.

        Has no effect if width is less than zero.

        :param float width: zero thickness for hairline; greater than zero for
            pen thickness
        )docstring",
        py::arg("width"))
    .def("getStrokeMiter", &SkPaint::getStrokeMiter,
        R"docstring(
        Returns the limit at which a sharp corner is drawn beveled.

        :return: zero and greater miter limit
        )docstring")
    .def("setStrokeMiter", &SkPaint::setStrokeMiter,
        R"docstring(
        Sets the limit at which a sharp corner is drawn beveled.

        Valid values are zero and greater. Has no effect if miter is less than zero.

        :param float miter: zero and greater miter limit
        )docstring",
        py::arg("miter"))
    .def("getStrokeCap", &SkPaint::getStrokeCap,
        R"docstring(
        Returns the geometry drawn at the beginning and end of strokes.
        )docstring")
    .def("setStrokeCap", &SkPaint::setStrokeCap,
        R"docstring(
        Sets the geometry drawn at the beginning and end of strokes.
        )docstring",
        py::arg("cap"))
    .def("getStrokeJoin", &SkPaint::getStrokeJoin,
        R"docstring(
        Returns the geometry drawn at the corners of strokes.
        )docstring")
    .def("setStrokeJoin", &SkPaint::setStrokeJoin,
        R"docstring(
        Sets the geometry drawn at the corners of strokes.
        )docstring",
        py::arg("join"))
    .def("getFillPath",
        [] (const SkPaint& paint, const SkPath &src, SkPath *dst, const SkRect *cullRect, SkScalar resScale) {
            return skpathutils::FillPathWithPaint(src, paint, dst, cullRect, resScale);
        },
        R"docstring(
        Returns the filled equivalent of the stroked path.

        :param src: :py:class:`Path` read to create a filled version
        :param dst: resulting :py:class:`Path`; may be the same as src, but may
            not be nullptr
        :param cullRect: optional limit passed to :py:class:`PathEffect`
        :param resScale: if > 1, increase precision, else if (0 < resScale < 1)
            reduce precision to favor speed and size
        :return: true if the path represents style fill, or false if it
            represents hairline
        )docstring",
        py::arg("src"), py::arg("dst"), py::arg("cullRect") = nullptr,
        py::arg("resScale") = 1)
    // .def("getFillPath",
    //     py::overload_cast<const SkPath&, SkPath*>(
    //         &SkPaint::getFillPath, py::const_))
    .def("getShader", &SkPaint::getShader,
        R"docstring(
        Returns optional colors used when filling a path, such as a gradient.

        Does not alter :py:class:`Shader` :py:class:`RefCnt`.

        :return: :py:class:`Shader` if previously set, nullptr otherwise
        )docstring",
        py::return_value_policy::reference_internal)
    .def("refShader", &SkPaint::refShader,
        R"docstring(
        Returns optional colors used when filling a path, such as a gradient.

        Increases :py:class:`Shader` :py:class:`RefCnt` by one.

        :return: :py:class:`Shader` if previously set, nullptr otherwise
        )docstring")
    .def("setShader",
        [] (SkPaint& paint, const SkShader& shader) {
            auto data = shader.serialize();
            auto clone = SkShader::Deserialize(
                shader.getFlattenableType(), data->data(), data->size());
            paint.setShader(sk_sp<SkShader>(
                reinterpret_cast<SkShader*>(clone.release())));
        },
        R"docstring(
        Sets optional colors used when filling a path, such as a gradient.

        Sets :py:class:`Shader` to shader, decreasing :py:class:`RefCnt` of the
        previous :py:class:`Shader`. Increments shader :py:class:`RefCnt` by
        one.

        :param skia.Shader shader: how geometry is filled with color; if
            nullptr, color is used instead
        )docstring",
        py::arg("shader"))
    .def("getColorFilter", &SkPaint::getColorFilter,
        R"docstring(
        Returns :py:class:`ColorFilter` if set, or nullptr.

        Does not alter :py:class:`ColorFilter` :py:class:`RefCnt`.

        :return: :py:class:`ColorFilter` if previously set, nullptr otherwise
        )docstring",
        py::return_value_policy::reference_internal)
    .def("refColorFilter", &SkPaint::refColorFilter,
        R"docstring(
        Returns :py:class:`ColorFilter` if set, or nullptr.

        Increases :py:class:`ColorFilter` :py:class:`RefCnt` by one.

        :return: :py:class:`ColorFilter` if set, or nullptr
        )docstring")
    .def("setColorFilter",
        [] (SkPaint& paint, const SkColorFilter& colorFilter) {
            paint.setColorFilter(CloneFlattenable(colorFilter));
        },
        R"docstring(
        Sets :py:class:`ColorFilter` to filter, decreasing :py:class:`RefCnt` of
        the previous :py:class:`ColorFilter`.

        Pass nullptr to clear :py:class:`ColorFilter`.

        Increments filter :py:class:`RefCnt` by one.

        :param skia.ColorFilter colorFilter: :py:class:`ColorFilter` to apply to
            subsequent draw
        )docstring",
        py::arg("colorFilter"))
/*
    .def("getBlendMode", &SkPaint::getBlendMode,
        R"docstring(
        Returns :py:class:`BlendMode`.

        By default, returns :py:attr:`BlendMode.kSrcOver`.

        :return: mode used to combine source color with destination color
        )docstring")
*/
    .def("isSrcOver", &SkPaint::isSrcOver,
        R"docstring(
        Returns true if :py:class:`BlendMode` is :py:attr:`BlendMode.kSrcOver`,
        the default.

        :return: true if :py:class:`BlendMode` is :py:attr:`BlendMode.kSrcOver`
        )docstring")
    .def("setBlendMode", &SkPaint::setBlendMode,
        R"docstring(
        Sets :py:class:`BlendMode` to mode.

        Does not check for valid input.

        :param skia.BlendMode mode: :py:class:`BlendMode` used to combine source
            color and destination
        )docstring",
        py::arg("mode"))
    .def("getPathEffect", &SkPaint::getPathEffect,
        R"docstring(
        Returns :py:class:`PathEffect` if set, or nullptr.

        Does not alter :py:class:`PathEffect` :py:class:`RefCnt`.

        :return: :py:class:`PathEffect` if previously set, nullptr otherwise
        )docstring",
        py::return_value_policy::reference_internal)
    .def("refPathEffect", &SkPaint::refPathEffect,
        R"docstring(
        Returns :py:class:`PathEffect` if set, or nullptr.

        Increases :py:class:`PathEffect` :py:class:`RefCnt` by one.

        :return: :py:class:`PathEffect` if previously set, nullptr otherwise
        )docstring")
    .def("setPathEffect",
        [] (SkPaint& paint, const SkPathEffect& pathEffect) {
            auto data = pathEffect.serialize();
            paint.setPathEffect(
                SkPathEffect::Deserialize(data->data(), data->size()));
        },
        R"docstring(
        Sets :py:class:`PathEffect` to pathEffect, decreasing :py:class:`RefCnt`
        of the previous :py:class:`PathEffect`.

        Pass nullptr to leave the path geometry unaltered.

        Increments pathEffect :py:class:`RefCnt` by one.

        :param skia.PathEffect pathEffect: replace :py:class:`Path` with a
            modification when drawn
        )docstring",
        py::arg("pathEffect"))
    .def("getMaskFilter", &SkPaint::getMaskFilter,
        R"docstring(
        Returns :py:class:`MaskFilter` if set, or nullptr.

        Does not alter :py:class:`MaskFilter` :py:class:`RefCnt`.

        :return: :py:class:`MaskFilter` if previously set, nullptr otherwise
        )docstring",
        py::return_value_policy::reference_internal)
    .def("refMaskFilter", &SkPaint::refMaskFilter,
        R"docstring(
        Returns :py:class:`MaskFilter` if set, or nullptr.

        Increases :py:class:`MaskFilter` :py:class:`RefCnt` by one.

        :return: :py:class:`MaskFilter` if previously set, nullptr otherwise
        )docstring")
    .def("setMaskFilter",
        [] (SkPaint& paint, const SkMaskFilter& maskFilter) {
            auto data = maskFilter.serialize();
            paint.setMaskFilter(
                SkMaskFilter::Deserialize(data->data(), data->size()));
        },
        R"docstring(
        Sets :py:class:`MaskFilter` to maskFilter, decreasing :py:class:`RefCnt`
        of the previous :py:class:`MaskFilter`.

        Pass nullptr to clear :py:class:`MaskFilter` and leave
        :py:class:`MaskFilter` effect on mask alpha unaltered.

        Increments maskFilter :py:class:`RefCnt` by one.

        :param skia.MaskFilter maskFilter: modifies clipping mask generated from
            drawn geometry
        )docstring",
        py::arg("maskFilter"))
    .def("getImageFilter", &SkPaint::getImageFilter,
        R"docstring(
        Returns :py:class:`ImageFilter` if set, or nullptr.

        Does not alter :py:class:`ImageFilter` :py:class:`RefCnt`.

        :return: :py:class:`ImageFilter` if previously set, nullptr otherwise
        )docstring",
        py::return_value_policy::reference_internal)
    .def("refImageFilter", &SkPaint::refImageFilter,
        R"docstring(
        Returns :py:class:`ImageFilter` if set, or nullptr.

        Increases :py:class:`ImageFilter` :py:class:`RefCnt` by one.

        :return: :py:class:`ImageFilter` if previously set, nullptr otherwise
        )docstring")
    .def("setImageFilter", &SkPaint::setImageFilter,
        R"docstring(
        Sets :py:class:`ImageFilter` to imageFilter, decreasing
        :py:class:`RefCnt` of the previous :py:class:`ImageFilter`.

        Pass nullptr to clear :py:class:`ImageFilter`, and remove
        :py:class:`ImageFilter` effect on drawing.

        Increments imageFilter :py:class:`RefCnt` by one.

        :param skia.ImageFilter imageFilter: how :py:class:`Image` is sampled
            when transformed
        )docstring",
        py::arg("imageFilter"))
    .def("nothingToDraw", &SkPaint::nothingToDraw,
        R"docstring(
        Returns true if :py:class:`Paint` prevents all drawing; otherwise, the
        :py:class:`Paint` may or may not allow drawing.

        Returns true if, for example, :py:class:`BlendMode` combined with alpha
        computes a new alpha of zero.

        :return: true if :py:class:`Paint` prevents all drawing
        )docstring")
    .def("canComputeFastBounds", &SkPaint::canComputeFastBounds,
        R"docstring(
        (to be made private) Returns true if :py:class:`Paint` does not include
        elements requiring extensive computation to compute
        :py:class:`BaseDevice` bounds of drawn geometry.

        For instance, :py:class:`Paint` with :py:class:`PathEffect` always
        returns false.

        :return: true if :py:class:`Paint` allows for fast computation of bounds
        )docstring")
    .def("computeFastBounds",
        [] (const SkPaint& paint, const SkRect& orig) {
            SkRect storage;
            return paint.computeFastBounds(orig, &storage);
        },
        R"docstring(
        (to be made private) Only call this if :py:meth:`canComputeFastBounds`
        returned true.

        This takes a raw rectangle (the raw bounds of a shape), and adjusts it
        for stylistic effects in the paint (e.g. stroking). It returns the
        adjusted bounds that can then be used for :py:meth:`Canvas.quickReject`
        tests.

        :param orig:     geometry modified by :py:class:`Paint` when drawn
        :return:         fast computed bounds
        )docstring",
        py::arg("orig"))
    .def("computeFastStrokeBounds",
        [] (const SkPaint& paint, const SkRect& orig) {
            SkRect storage;
            return paint.computeFastStrokeBounds(orig, &storage);
        },
        R"docstring(
        (to be made private)

        :param orig:    geometry modified by SkPaint when drawn
        :return:        fast computed bounds
        )docstring",
        py::arg("orig"))
    .def("doComputeFastBounds",
        [] (const SkPaint& paint, const SkRect& orig, SkPaint::Style style) {
            SkRect storage;
            return paint.doComputeFastBounds(orig, &storage, style);
        },
        R"docstring(
        (to be made private) Computes the bounds, overriding the
        :py:class:`Paint` :py:class:`Paint.Style`.

        This can be used to account for additional width required by stroking
        orig, without altering :py:class:`Paint.Style` set to fill.

        :param orig:    geometry modified by :py:class:`Paint` when drawn
        :param style:   overrides :py:class:`Paint.Style`
        :return:        fast computed bounds
        )docstring",
        py::arg("orig"), py::arg("style"))
    .def_readonly_static("kStyleCount", &SkPaint::kStyleCount)
    .def_readonly_static("kCapCount", &SkPaint::kCapCount)
    .def_readonly_static("kJoinCount", &SkPaint::kJoinCount)
    .def(py::self == py::self,
        R"docstring(
        Compares a and b, and returns true if a and b are equivalent.

        May return false if :py:class:`PathEffect`, :py:class:`Shader`,
        :py:class:`MaskFilter`, :py:class:`ColorFilter`, or
        :py:class:`ImageFilter` have identical contents but different pointers.

        :param other: :py:class:`Paint` to compare
        :return: true if :py:class:`Paint` pair are equivalent
        )docstring",
        py::arg("other"))
    .def(py::self != py::self,
        R"docstring(
        Compares a and b, and returns true if a and b are not equivalent.

        May return true if :py:class:`PathEffect`, :py:class:`Shader`,
        :py:class:`MaskFilter`, :py:class:`ColorFilter`, or
        :py:class:`ImageFilter` have identical contents but different pointers.

        :param other: :py:class:`Paint` to compare
        :return: true if :py:class:`Paint` pair are not equivalent
        )docstring",
        py::arg("other"))
    ;

py::implicitly_convertible<py::dict, SkPaint>();

py::class_<SkFlattenable, PyFlattanable, sk_sp<SkFlattenable>, SkRefCnt>
    flattenable(m, "Flattanable", R"docstring(
    :py:class:`Flattenable` is the base class for objects that need to be
    flattened into a data stream for either transport or as part of the key to
    the font cache.
    )docstring");

py::enum_<SkFlattenable::Type>(flattenable, "Type")
    .value("kColorFilter_Type", SkFlattenable::Type::kSkColorFilter_Type)
    .value("kDrawable_Type", SkFlattenable::Type::kSkDrawable_Type)
    .value("kDrawLooper_Type", SkFlattenable::Type::kSkDrawLooper_Type)
    .value("kImageFilter_Type", SkFlattenable::Type::kSkImageFilter_Type)
    .value("kMaskFilter_Type", SkFlattenable::Type::kSkMaskFilter_Type)
    .value("kPathEffect_Type", SkFlattenable::Type::kSkPathEffect_Type)
/*
    .value("kPixelRef_Type", SkFlattenable::Type::kSkPixelRef_Type)
    .value("kUnused_Type4", SkFlattenable::Type::kSkUnused_Type4)
    .value("kShaderBase_Type", SkFlattenable::Type::kSkShaderBase_Type)
    .value("kUnused_Type", SkFlattenable::Type::kSkUnused_Type)
    .value("kUnused_Type2", SkFlattenable::Type::kSkUnused_Type2)
    .value("kUnused_Type3", SkFlattenable::Type::kSkUnused_Type3)
*/
    .export_values();

flattenable
    // .def("getFactory", &SkFlattenable::getFactory)
    .def("getTypeName", &SkFlattenable::getTypeName,
        R"docstring(
        Returns the name of the object's class.

        Implemented in :py:class:`~skia.Drawable`.
        )docstring")
    // .def("flatten", &SkFlattenable::flatten)
    .def("getFlattenableType", &SkFlattenable::getFlattenableType)
    .def("serialize",
        [] (const SkFlattenable& flattenable) {
            return flattenable.serialize();
        })
    // .def_static("NameToFactory", &SkFlattenable::NameToFactory)
    // .def_static("FactoryToName", &SkFlattenable::FactoryToName)
    // .def_static("Register", &SkFlattenable::Register)
    .def_static("Deserialize",
        [] (SkFlattenable::Type type, py::buffer b) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.shape[0] * info.strides[0] : 0;
            return SkFlattenable::Deserialize(type, info.ptr, size);
        },
        py::arg("type"), py::arg("b"))
    ;

initColorFilter(m);
initPathEffect(m);
initShader(m);
initMaskFilter(m);
initImageFilter(m);
}
