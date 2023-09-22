#include "common.h"
#include <include/core/SkSerialProcs.h>
#include <include/core/SkRSXform.h>
#include <pybind11/stl.h>

template<>
struct py::detail::has_operator_delete<SkTextBlob, void> : std::false_type {};

void initTextBlob(py::module &m) {
py::class_<SkTextBlob, sk_sp<SkTextBlob>> textblob(m, "TextBlob", R"docstring(
    :py:class:`TextBlob` combines multiple text runs into an immutable
    container.

    Each text run consists of glyphs, :py:class:`Paint`, and position. Only
    parts of :py:class:`Paint` related to fonts and text rendering are used by
    run.

    Example::

        textblob = skia.TextBlob('text', skia.Font())
        for run in textblob:
            print(run.fGlyphIndices)
    )docstring");

py::class_<SkTextBlob::Iter> iter(textblob, "Iter",
    R"docstring(
    Iterator for :py:class:`~skia.TextBlob.Iter.Run`.

    Example::

        run = skia.TextBlob.Iter.Run()
        it = skia.TextBlob.Iter(textblob)
        while it.next(run):
            print(run)

        for run in textblob():
            print(run)
    )docstring");

py::class_<SkTextBlob::Iter::Run>(iter, "Run")
    .def(py::init<>())
    .def("__repr__",
        [] (const SkTextBlob::Iter::Run& run) {
            std::stringstream stream;
            stream << "Run([";
            for (int i = 0; i < run.fGlyphCount; ++i) {
                stream << run.fGlyphIndices[i];
                if (i < run.fGlyphCount - 1)
                    stream << ", ";
            }
            stream << "])";
            return stream.str();
        })
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
    .def("__next__",
        [] (SkTextBlob::Iter& it) {
            SkTextBlob::Iter::Run run;
            if (it.next(&run))
                return run;
            throw py::stop_iteration();
        })
    .def("next", &SkTextBlob::Iter::next, py::arg("run"))
    ;

textblob
    .def(py::init(
        [] (const std::string& text, const SkFont& font, py::object pos,
            SkTextEncoding encoding) {
            if (pos.is_none())
                return SkTextBlob::MakeFromText(
                    text.c_str(), text.size(), font, encoding);
            std::vector<SkPoint> pos_(pos.cast<std::vector<SkPoint>>());
            int count = font.countText(text.c_str(), text.size(), encoding);
            if (count != pos_.size())
                throw py::value_error(
                    py::str(
                        "len(text) = {} does not match len(pos) = {}").format(
                        count, pos_.size()));
            return SkTextBlob::MakeFromPosText(
                text.c_str(), text.size(), &pos_[0], font, encoding);
        }),
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
        :param List[skia.Point] positions: array of positions, must contain
            values for all of the character points.
        :param skia.TextEncoding encoding: text encoding used in the text array
        )docstring",
        py::arg("text"), py::arg("font"), py::arg("positions") = nullptr,
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def("__iter__",
        [] (const SkTextBlob& textblob) { return SkTextBlob::Iter(textblob); },
        py::keep_alive<0, 1>())
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
        [] (const SkTextBlob& textblob, py::iterable bounds,
            const SkPaint* paint) {
            auto bounds_ = bounds.cast<std::vector<SkScalar>>();
            if (bounds_.size() != 2) {
                std::stringstream stream;
                stream << "Bounds must have two elements (given "
                    << bounds_.size() << " elements).";
                throw py::value_error(stream.str());
            }
            int glyphs = 0;
            SkTextBlob::Iter::Run run;
            SkTextBlob::Iter iter(textblob);
            while (iter.next(&run))
                glyphs += run.fGlyphCount;
            std::vector<SkScalar> intervals(2 * glyphs);
            size_t result = textblob.getIntercepts(
                &bounds_[0], &intervals[0], paint);
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
    .def("refCntGreaterThan", &SkTextBlob::refCntGreaterThan, py::arg("count"))
    .def_static("MakeFromText",
        [] (const std::string& text, const SkFont& font,
            SkTextEncoding encoding) {
            return SkTextBlob::MakeFromText(
                text.c_str(), text.size(), font, encoding);
        },
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
        [] (const std::string& text, py::iterable xpos,
            SkScalar constY, const SkFont& font, SkTextEncoding encoding) {
            auto xpos_ = xpos.cast<std::vector<SkScalar>>();
            int count = font.countText(text.c_str(), text.size(), encoding);
            if (count != xpos_.size()) {
                std::stringstream stream;
                stream << "text and xpos must have the same number of elements "
                    << "(len(text) = " << count << ", "
                    << "len(xpos) = " << xpos_.size() << ").";
                throw py::value_error(stream.str());
            }
            return SkTextBlob::MakeFromPosTextH(
                text.c_str(), text.size(), &xpos_[0], constY, font, encoding);
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
            int count = font.countText(text.c_str(), text.size(), encoding);
            if (count != pos.size())
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
            int count = font.countText(text.c_str(), text.size(), encoding);
            if (count != xform.size())
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

textblobbuilder
    .def(py::init(), "Constructs empty :py:class:`TextBlobBuilder`.")
    .def("make", &SkTextBlobBuilder::make,
        R"docstring(
        Returns :py:class:`TextBlob` built from runs of glyphs added by builder.

        Returned :py:class:`TextBlob` is immutable; it may be copied, but its
        contents may not be altered. Returns nullptr if no runs of glyphs were
        added by builder.

        Resets :py:class:`TextBlobBuilder` to its initial empty state, allowing
        it to be reused to build a new set of runs.

        :return: :py:class:`TextBlob` or nullptr
        )docstring")
    .def("allocRun",
        [] (SkTextBlobBuilder& builder, const std::string& text,
            const SkFont& font, SkScalar x, SkScalar y,
            const SkRect* bounds, SkTextEncoding encoding) {
            int glyphCount = font.countText(&text[0], text.size(), encoding);
            auto run = builder.allocRun(font, glyphCount, x, y, bounds);
            font.textToGlyphs(
                &text[0], text.size(), encoding, run.glyphs, glyphCount);
        },
        R"docstring(
        Sets a new run with glyphs.

        Glyphs share metrics in font.

        Glyphs are positioned on a baseline at (x, y), using font metrics to
        determine their relative placement.

        bounds defines an optional bounding box, used to suppress drawing when
        :py:class:`TextBlob` bounds does not intersect :py:class:`Surface`
        bounds. If bounds is nullptr, :py:class:`TextBlob` bounds is computed
        from (x, y) and RunBuffer::glyphs metrics.

        :param skia.Font font: :py:class:`Font` used for this run
        :param str text: text for the run
        :param float x: horizontal offset within the blob
        :param float y: vertical offset within the blob
        :param skia.Rect bounds: optional run bounding box
        :param skia.TextEncoding encoding: specifies the encoding of the text
            array
        )docstring",
        py::arg("text"), py::arg("font"), py::arg("x"), py::arg("y"),
        py::arg("bounds") = nullptr,
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def("allocRunPosH",
        [] (SkTextBlobBuilder& builder, const SkFont& font,
            const std::vector<SkGlyphID>& glyphs, py::iterable xpos,
            SkScalar y, const SkRect* bounds) {
            auto xpos_ = xpos.cast<std::vector<SkScalar>>();
            if (glyphs.size() != xpos_.size()) {
                std::stringstream stream;
                stream << "glyphs and xpos must have the same number of "
                    << "elements (len(glyphs) = " << glyphs.size() << ", "
                    << "len(xpos) = " << xpos_.size() << ").";
                throw py::value_error(stream.str());
            }
            auto run = builder.allocRunPosH(font, glyphs.size(), y, bounds);
            std::copy(glyphs.begin(), glyphs.end(), run.glyphs);
            std::copy(xpos_.begin(), xpos_.end(), run.pos);
        },
        R"docstring(
        Sets a new run with glyphs and positions along baseline.

        Glyphs share metrics in font.

        Glyphs are positioned on a baseline at y, using x-axis positions.

        bounds defines an optional bounding box, used to suppress drawing when
        :py:class:`TextBlob` bounds does not intersect :py:class:`Surface`
        bounds. If bounds is nullptr, :py:class:`TextBlob` bounds is computed
        from y, RunBuffer::pos, and RunBuffer::glyphs metrics.

        :param skia.Font font: :py:class:`Font` used for this run
        :param List[int] glyphs: array of glyph IDs
        :param List[float] xpos: horizontal offsets within the blob
        :param float y: vertical offset within the blob
        :param skia.Rect bounds: optional run bounding box
        )docstring",
        py::arg("font"), py::arg("glyphs"), py::arg("xpos"), py::arg("y"),
        py::arg("bounds") = nullptr)
    .def("allocRunPos",
        [] (SkTextBlobBuilder& builder, const SkFont& font,
            const std::vector<SkGlyphID>& glyphs, py::iterable positions,
            const SkRect* bounds) {
            auto positions_ = positions.cast<std::vector<SkPoint>>();
            if (glyphs.size() != positions_.size())
                throw std::runtime_error(
                    "glyphs and positions must have the same size.");
            auto run = builder.allocRunPos(font, glyphs.size(), bounds);
            std::copy(glyphs.begin(), glyphs.end(), run.glyphs);
            std::copy(positions_.begin(), positions_.end(), run.points());
        },
        R"docstring(
        Sets a new run with glyphs and :py:class:`Point` positions.

        Glyphs share metrics in font.

        Glyphs are positioned using :py:class:`Point`, using two scalar values
        for each :py:class:`Point`.

        bounds defines an optional bounding box, used to suppress drawing when
        :py:class:`TextBlob` bounds does not intersect :py:class:`Surface`
        bounds. If bounds is nullptr, :py:class:`TextBlob` bounds is computed
        from RunBuffer::pos and RunBuffer::glyphs metrics.

        :param skia.Font font: :py:class:`Font` used for this run
        :param List[int] glyphs: array of glyph IDs
        :param List[skia.Point] positions: offsets within the blob
        :param skia.Rect bounds: optional run bounding box
        )docstring",
        py::arg("font"), py::arg("glyphs"), py::arg("positions"),
        py::arg("bounds") = nullptr)
    .def("allocRunRSXform",
        [] (SkTextBlobBuilder& builder, const SkFont& font,
            const std::vector<SkGlyphID>& glyphs,
            const std::vector<SkRSXform>& xforms) {
            if (glyphs.size() != xforms.size())
                throw std::runtime_error(
                    "glyphs and xforms must have the same size.");
            auto run = builder.allocRunRSXform(font, glyphs.size());
            std::copy(glyphs.begin(), glyphs.end(), run.glyphs);
            std::copy(xforms.begin(), xforms.end(), run.xforms());
        },
        py::arg("font"), py::arg("glyphs"), py::arg("xforms"))
    ;
}
