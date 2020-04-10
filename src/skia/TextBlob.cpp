#include "common.h"

template<>
struct py::detail::has_operator_delete<SkTextBlob, void> : std::false_type {};

void initTextBlob(py::module &m) {

py::class_<SkDeserialProcs>(m, "DeserialProcs");

py::class_<SkTextBlob, sk_sp<SkTextBlob>>(m, "TextBlob", R"docstring(
    SkTextBlob combines multiple text runs into an immutable container.

    Each text run consists of glyphs, SkPaint, and position. Only parts of
    SkPaint related to fonts and text rendering are used by run.
    )docstring")
    .def(py::init([](const std::string& text, const SkFont& font) {
        return SkTextBlob::MakeFromText(text.c_str(), text.size(), font);
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
    :param skia.Font font: text size, typeface, text scale, and so on, used to
        draw
    )docstring",
    py::arg("text"), py::arg("font"))
    .def("bounds", &SkTextBlob::bounds,
        "Returns conservative bounding box.")
    .def("uniqueID", &SkTextBlob::uniqueID,
        "Returns a non-zero value unique among all text blobs.")
    .def("getIntercepts", &SkTextBlob::getIntercepts,
        "Returns the number of intervals that intersect bounds.")
    // .def("serialize",
    //     py::overload_cast<const SkSerialProcs&, void*, size_t>(
    //         &SkTextBlob::serialize),
    //     "Writes data to allow later reconstruction of SkTextBlob.")
    // .def("serialize",
    //     py::overload_cast<const SkSerialProcs&>(&SkTextBlob::serialize),
    //     "Returns storage containing SkData describing SkTextBlob, using "
    //     "optional custom encoders.")
    .def("unique", &SkTextBlob::unique)
    .def("ref", &SkTextBlob::ref)
    .def("unref", &SkTextBlob::unref)
    .def("deref", &SkTextBlob::deref)
    .def("refCntGreaterThan", &SkTextBlob::refCntGreaterThan)
    .def_static("MakeFromText", &SkTextBlob::MakeFromText,
        "Creates SkTextBlob with a single run.",
        py::arg("text"), py::arg("byteLength"), py::arg("font"),
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def_static("MakeFromString", &SkTextBlob::MakeFromString,
        "Creates SkTextBlob with a single run.",
        py::arg("string"), py::arg("font"),
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def_static("MakeFromPosTextH", &SkTextBlob::MakeFromPosTextH,
        "Returns a textblob built from a single run of text with x-positions "
        "and a single y value.",
        py::arg("text"), py::arg("byteLength"), py::arg("xpos"),
        py::arg("constY"), py::arg("font"),
        py::arg("encoding") = SkTextEncoding::kUTF8)
    .def_static("MakeFromPosText", &SkTextBlob::MakeFromPosText,
        "Returns a textblob built from a single run of text with positions.",
        py::arg("text"), py::arg("byteLength"), py::arg("pos"),
        py::arg("font"), py::arg("encoding") = SkTextEncoding::kUTF8)
    .def_static("MakeFromRSXform", &SkTextBlob::MakeFromRSXform)
    .def_static("Deserialize", &SkTextBlob::Deserialize,
        "Recreates SkTextBlob that was serialized into data.")
    ;

py::class_<SkTextBlobBuilder> textblobbuilder(m, "TextBlobBuilder", R"docstring(
    Helper class for constructing SkTextBlob.
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
    .def(py::init(), "Constructs empty SkTextBlobBuilder.")
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
