#include "common.h"
#include <pybind11/stl.h>

template<>
struct py::detail::has_operator_delete<SkTextBlob, void> : std::false_type {};

sk_sp<SkTextBlob> MakeFromText(
    const std::string& text, const SkFont& font, SkTextEncoding encoding) {
    return SkTextBlob::MakeFromText(text.c_str(), text.size(), font, encoding);
}

void initTextBlob(py::module &m) {
py::class_<SkTextBlob, sk_sp<SkTextBlob>> textblob(m, "TextBlob", R"docstring(
    :py:class:`TextBlob` combines multiple text runs into an immutable
    container.

    Each text run consists of glyphs, :py:class:`Paint`, and position. Only
    parts of :py:class:`Paint` related to fonts and text rendering are used by
    run.
    )docstring");

py::class_<SkTextBlob::Iter> iter(textblob, "Iter");

py::class_<SkTextBlob::Iter::Run>(iter, "Run")
    .def(py::init<>())
    .def_readonly("fTypeface", &SkTextBlob::Iter::Run::fTypeface,
        py::return_value_policy::reference)
    .def_readonly("fGlyphCount", &SkTextBlob::Iter::Run::fGlyphCount)
    .def_property_readonly("fGlyphIndices",
        [] (const SkTextBlob::Iter::Run& run) {
            return std::vector<uint16_t>(
                run.fGlyphIndices, run.fGlyphIndices + run.fGlyphCount);
        })
    ;

iter
    .def(py::init<const SkTextBlob&>())
    .def("next", &SkTextBlob::Iter::next)
    ;

textblob
    .def(py::init(&MakeFromText),
        R"docstring(
        Creates :py:class:`TextBlob` with a single run.

        `font` contains attributes used to define the run text.

        This function uses the default character-to-glyph mapping from the
        :py:class:`Typeface` in font. It does not perform typeface fallback for
        characters not found in the :py:class:`Typeface`. It does not perform
        kerning or other complex shaping; glyphs are positioned based on their
        default advances.

        :param str text: character code points or glyphs drawn
        :param skia.Font font: text size, typeface, text scale, and so on, used
            to draw
        :param skia.TextEncoding encoding: text encoding used in the text array
        )docstring",
        py::arg("text"), py::arg("font"),
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def("bounds", &SkTextBlob::bounds,
        R"docstring(
        Returns conservative bounding box.

        Uses :py:class:`Paint` associated with each glyph to determine glyph
        bounds, and unions all bounds. Returned bounds may be larger than the
        bounds of all glyphs in runs.

        :return: conservative bounding box
        )docstring")
    .def("uniqueID", &SkTextBlob::uniqueID,
        R"docstring(
        Returns a non-zero value unique among all text blobs.

        :return: identifier for :py:class:`TextBlob`
        )docstring")
    .def("getIntercepts",
        [] (const SkTextBlob& textblob, const std::vector<SkScalar>& bounds,
            const SkPaint* paint) {
            if (bounds.size() != 2)
                throw std::runtime_error("Bounds must have two elements.");
            int glyphs = 0;
            SkTextBlob::Iter::Run run;
            SkTextBlob::Iter iter(textblob);
            while (iter.next(&run))
                glyphs += run.fGlyphCount;
            std::vector<SkScalar> intervals(2 * glyphs);
            size_t result = textblob.getIntercepts(
                &bounds[0], &intervals[0], paint);
            if (result != intervals.size())
                intervals.erase(intervals.begin() + result, intervals.end());
            return intervals;
        },
        R"docstring(
        Returns the number of intervals that intersect bounds.

        bounds describes a pair of lines parallel to the text advance. The
        return count is zero or a multiple of two, and is at most twice the
        number of glyphs in the the blob.

        Pass nullptr for intervals to determine the size of the interval array.

        Runs within the blob that contain :py:class:`RSXform` are ignored when
        computing intercepts.

        :param List[skia.Scalar] bounds: lower and upper line parallel to the
            advance
        :param skia.Paint paint: specifies stroking, :py:class:`PathEffect` that
            affects the result; may be nullptr
        :return: intersections; may be empty
        )docstring",
        py::arg("bounds"), py::arg("paint") = nullptr)
    .def("serialize",
        [] (const SkTextBlob& textblob) {
            return textblob.serialize(SkSerialProcs());
        },
        R"docstring(
        Returns storage containing :py:class:`Data` describing
        :py:class:`TextBlob`, using optional custom encoders.

        procs.fTypefaceProc permits supplying a custom function to encode
        :py:class:`Typeface`. If procs.fTypefaceProc is nullptr, default
        encoding is used. procs.fTypefaceCtx may be used to provide user context
        to procs.fTypefaceProc; procs.fTypefaceProc is called with a pointer to
        :py:class:`Typeface` and user context.

        :return: storage containing serialized :py:class:`TextBlob`
        )docstring")
    .def("unique", &SkTextBlob::unique)
    .def("ref", &SkTextBlob::ref)
    .def("unref", &SkTextBlob::unref)
    .def("deref", &SkTextBlob::deref)
    .def("refCntGreaterThan", &SkTextBlob::refCntGreaterThan)
    .def_static("MakeFromText", &MakeFromText,
        R"docstring(
        Creates :py:class:`TextBlob` with a single run.

        font contains attributes used to define the run text.

        When encoding is :py:attr:`TextEncoding.kUTF8`,
        :py:attr:`TextEncoding.kUTF16`, or :py:attr:`TextEncoding.kUTF32`, this
        function uses the default character-to-glyph mapping from the
        :py:class:`Typeface` in font. It does not perform typeface fallback for
        characters not found in the :py:class:`Typeface`. It does not perform
        kerning or other complex shaping; glyphs are positioned based on their
        default advances.

        :param str text: character code points or glyphs drawn
        :param skia.Font font: text size, typeface, text scale, and so on, used
            to draw
        :param skia.TextEncoding encoding: text encoding used in the text array
        :return: :py:class:`TextBlob` constructed from one run
        )docstring",
        py::arg("text"), py::arg("font"),
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def_static("MakeFromString",
        [] (const std::string& string, const SkFont& font,
            SkTextEncoding encoding) {
            return SkTextBlob::MakeFromString(string.c_str(), font, encoding);
        },
        R"docstring(
        Creates :py:class:`TextBlob` with a single run.

        string meaning depends on :py:class:`TextEncoding`; by default, string
        is encoded as UTF-8.

        font contains attributes used to define the run text.

        When encoding is :py:attr:`TextEncoding.kUTF8`,
        :py:attr:`TextEncoding.kUTF16`, or :py:attr:`TextEncoding.kUTF32`, this
        function uses the default character-to-glyph mapping from the
        :py:class:`Typeface` in font. It does not perform typeface fallback for
        characters not found in the :py:class:`Typeface`. It does not perform
        kerning or other complex shaping; glyphs are positioned based on their
        default advances.

        :param str string: character code points or glyphs drawn
        :param skia.Font font: text size, typeface, text scale, and so on, used
            to draw
        :param skia.TextEncoding encoding: text encoding used in the text array
        :return: :py:class:`TextBlob` constructed from one run
        )docstring",
        py::arg("string"), py::arg("font"),
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def_static("MakeFromPosTextH",
        [] (const std::string& text, const std::vector<SkScalar>& xpos,
            SkScalar constY, const SkFont& font, SkTextEncoding encoding) {
            if (text.size() != xpos.size())
                throw std::runtime_error(
                    "text and xpos must have the same number of elements.");
            return SkTextBlob::MakeFromPosTextH(
                text.c_str(), text.size(), &xpos[0], constY, font, encoding);
        },
        R"docstring(
        Returns a textblob built from a single run of text with x-positions and
        a single y value.

        This is equivalent to using :py:class:`TextBlobBuilder` and calling
        :py:meth:`TextBlobBuilder.allocRunPosH`.

        :param str text: character code points or glyphs drawn (based on
            encoding)
        :param List[float] xpos: array of x-positions, must contain values for
            all of the character points.
        :param float constY: shared y-position for each character point, to be
            paired with each xpos.
        :param skia.Font font: :py:class:`Font` used for this run
        :param skia.TextEncoding encoding: specifies the encoding of the text
            array.
        :return: new textblob or nullptr
        )docstring",
        py::arg("text"), py::arg("xpos"), py::arg("constY"), py::arg("font"),
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def_static("MakeFromPosText",
        [] (const std::string& text, const std::vector<SkPoint>& pos,
            const SkFont& font, SkTextEncoding encoding) {
            if (text.size() != pos.size())
                throw std::runtime_error(
                    "text and pos must have the same number of elements.");
            return SkTextBlob::MakeFromPosText(
                text.c_str(), text.size(), &pos[0], font, encoding);
        },
        R"docstring(
        Returns a textblob built from a single run of text with x-positions and
        a single y value.

        This is equivalent to using :py:class:`TextBlobBuilder` and calling
        :py:meth:`TextBlobBuilder.allocRunPosH`.

        :param str text: character code points or glyphs drawn (based on
            encoding)
        :param List[skia.Point] pos: array of positions, must contain values for
            all of the character points.
        :param skia.Font font: :py:class:`Font` used for this run
        :param skia.TextEncoding encoding: specifies the encoding of the text
            array.
        :return: new textblob or nullptr
        )docstring",
        py::arg("text"), py::arg("pos"), py::arg("font"),
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def_static("MakeFromRSXform",
        [] (const std::string& text, const std::vector<SkRSXform>& xform,
            const SkFont& font, SkTextEncoding encoding) {
            if (text.size() != xform.size())
                throw std::runtime_error(
                    "text and xform must have the same number of elements.");
            return SkTextBlob::MakeFromRSXform(
                text.c_str(), text.size(), &xform[0], font, encoding);
        },
        py::arg("text"), py::arg("xform"), py::arg("font"),
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def_static("Deserialize",
        [] (py::buffer b) {
            auto info = b.request();
            size_t size = (info.ndim) ? info.shape[0] * info.strides[0] : 0;
            return SkTextBlob::Deserialize(info.ptr, size, SkDeserialProcs());
        },
        R"docstring(
        Recreates :py:class:`TextBlob` that was serialized into data.

        Returns constructed :py:class:`TextBlob` if successful; otherwise,
        returns nullptr. Fails if size is smaller than required data length, or
        if data does not permit constructing valid :py:class:`TextBlob`.

        procs.fTypefaceProc permits supplying a custom function to decode
        :py:class:`Typeface`. If procs.fTypefaceProc is nullptr, default
        decoding is used. procs.fTypefaceCtx may be used to provide user context
        to procs.fTypefaceProc; procs.fTypefaceProc is called with a pointer to
        :py:class:`Typeface` data, data byte length, and user context.

        :param Union[bytes,bytearray,memoryview] data: serial data
        :return: :py:class:`TextBlob` constructed from data in memory
        )docstring",
        py::arg("data"))
    ;

py::class_<SkTextBlobBuilder> textblobbuilder(m, "TextBlobBuilder", R"docstring(
    Helper class for constructing :py:class:`TextBlob`.
    )docstring");

py::class_<SkTextBlobBuilder::RunBuffer>(textblobbuilder, "RunBuffer",
    R"docstring(
    RunBuffer supplies storage for glyphs and positions within a run.
    )docstring")
    .def("points", &SkTextBlobBuilder::RunBuffer::points,
        py::return_value_policy::reference)
    .def("xforms", &SkTextBlobBuilder::RunBuffer::xforms,
        py::return_value_policy::reference)
    .def_readwrite("glyphs", &SkTextBlobBuilder::RunBuffer::glyphs,
        "storage for glyphs in run ",
        py::return_value_policy::reference)
    .def_readwrite("pos", &SkTextBlobBuilder::RunBuffer::pos,
        "storage for positions in run.",
        py::return_value_policy::reference)
    .def_readwrite("utf8text", &SkTextBlobBuilder::RunBuffer::utf8text,
        "reserved for future use.",
        py::return_value_policy::reference)
    .def_readwrite("clusters", &SkTextBlobBuilder::RunBuffer::clusters,
        "reserved for future use.",
        py::return_value_policy::reference)
    ;

textblobbuilder
    .def(py::init(), "Constructs empty :py:class:`TextBlobBuilder`.")
    .def("make", &SkTextBlobBuilder::make,
        "Returns SkTextBlob built from runs of glyphs added by builder.")
    .def("allocRun", &SkTextBlobBuilder::allocRun,
        "Returns run with storage for glyphs.")
    .def("allocRunPosH", &SkTextBlobBuilder::allocRunPosH,
        "Returns run with storage for glyphs and positions along baseline.")
    .def("allocRunPos", &SkTextBlobBuilder::allocRunPos,
        "Returns run with storage for glyphs and SkPoint positions.")
    // .def("allocRunRSXform", &SkTextBlobBuilder::allocRunRSXform)
    ;
}
