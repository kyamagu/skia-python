#include "common.h"
#include <pybind11/stl.h>

namespace {

typedef struct _PyPDF {} PyPDF;

// Helper to support with statement.
class PyAutoDocumentPage {
public:
    PyAutoDocumentPage(SkDocument* document, SkScalar width, SkScalar height)
        : width_(width), height_(height), document_(document) {}
    SkCanvas* beginPage() { return document_->beginPage(width_, height_); }
    void endPage() { document_->endPage(); }
private:
    SkScalar width_;
    SkScalar height_;
    SkDocument* document_;
};

SkPDF::Metadata DictToMetadata(py::dict dict) {
    SkPDF::Metadata metadata;
    for (auto item : dict) {
        std::string key(item.first.cast<std::string>());
        if (key == "Title")
            metadata.fTitle = item.second.cast<SkString>();
        else if (key == "Author")
            metadata.fAuthor = item.second.cast<SkString>();
        else if (key == "Subject")
            metadata.fSubject = item.second.cast<SkString>();
        else if (key == "Keywords")
            metadata.fKeywords = item.second.cast<SkString>();
        else if (key == "Creator")
            metadata.fCreator = item.second.cast<SkString>();
        else if (key == "Producer")
            metadata.fProducer = item.second.cast<SkString>();
        else if (key == "RasterDPI")
            metadata.fRasterDPI = item.second.cast<SkScalar>();
        else if (key == "PDFA")
            metadata.fPDFA = item.second.cast<bool>();
        else if (key == "EncodingQuality")
            metadata.fEncodingQuality = item.second.cast<int>();
        else if (key == "StructureElementTreeRoot")
            metadata.fStructureElementTreeRoot =
                item.second.cast<SkPDF::StructureElementNode*>();
        else
            throw py::key_error(py::str("Invalid key: {}").format(key));
    }
    return metadata;
}

}

void initDocument(py::module &m) {

py::class_<SkDocument, sk_sp<SkDocument>, SkRefCnt>(m, "Document",
    R"docstring(
    High-level API for creating a document-based canvas.

    To use:

    1. Create a document, specifying a stream to store the output.
    2. For each "page" of content:

        a. canvas = doc.beginPage(...)
        b. draw_my_content(canvas)
        c. doc.endPage()

    3. Close the document with doc.close().

    ``skia-python`` supports ``with`` syntax to build a document::

        stream = skia.FILEWStream('output.pdf')
        with skia.PDF.MakeDocument(stream) as document:
            with document.page(480, 640) as canvas:
                draw(canvas)

    )docstring")
    .def("__enter__",
        [] (const SkDocument* document) { return document; })
    .def("__exit__",
        [] (SkDocument* document, py::object exc_type, py::object exc_value,
            py::object traceback) { document->close(); })
    .def("page",
        [] (SkDocument* document, SkScalar width, SkScalar height) {
            return PyAutoDocumentPage(document, width, height);
        },
        py::arg("width"), py::arg("height"), py::keep_alive<0, 1>())
    .def("beginPage", &SkDocument::beginPage,
        R"docstring(
        Begin a new page for the document, returning the canvas that will draw
        into the page.

        The document owns this canvas, and it will go out of scope when
        :py:meth:`endPage` or :py:meth:`close` is called, or the document is
        deleted.
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("content") = nullptr,
        py::return_value_policy::reference_internal)
    .def("endPage", &SkDocument::endPage,
        R"docstring(
        Call :py:meth:`endPage` when the content for the current page has been
        drawn (into the canvas returned by :py:meth:`beginPage`).

        After this call the canvas returned by :py:meth:`beginPage` will be
        out-of-scope.
        )docstring")
    .def("close", &SkDocument::close,
        R"docstring(
        Call :py:meth:`close` when all pages have been drawn.

        This will close the file or stream holding the document's contents.
        After :py:meth:`close` the document can no longer add new pages.
        Deleting the document will automatically call :py:meth:`close` if need
        be.
        )docstring")
    .def("abort", &SkDocument::abort,
        R"docstring(
        Call :py:meth:`abort` to stop producing the document immediately.

        The stream output must be ignored, and should not be trusted.
        )docstring")
    ;

py::class_<PyAutoDocumentPage>(m, "_AutoDocumentPage")
    .def("__enter__",
        [] (PyAutoDocumentPage& page) { return page.beginPage(); },
        py::return_value_policy::reference_internal)
    .def("__exit__",
        [] (PyAutoDocumentPage& page, py::object exc_type, py::object exc_value,
            py::object traceback) { page.endPage(); })
    ;

py::class_<PyPDF> pdf(m, "PDF");

/* todo: not in use? vanished?
py::enum_<SkPDF::DocumentStructureType>(pdf, "DocumentStructureType")
    .value("kDocument", SkPDF::DocumentStructureType::kDocument)
    .value("kPart", SkPDF::DocumentStructureType::kPart)
    .value("kArt", SkPDF::DocumentStructureType::kArt)
    .value("kSect", SkPDF::DocumentStructureType::kSect)
    .value("kDiv", SkPDF::DocumentStructureType::kDiv)
    .value("kBlockQuote", SkPDF::DocumentStructureType::kBlockQuote)
    .value("kCaption", SkPDF::DocumentStructureType::kCaption)
    .value("kTOC", SkPDF::DocumentStructureType::kTOC)
    .value("kTOCI", SkPDF::DocumentStructureType::kTOCI)
    .value("kIndex", SkPDF::DocumentStructureType::kIndex)
    .value("kNonStruct", SkPDF::DocumentStructureType::kNonStruct)
    .value("kPrivate", SkPDF::DocumentStructureType::kPrivate)
    .value("kH", SkPDF::DocumentStructureType::kH)
    .value("kH1", SkPDF::DocumentStructureType::kH1)
    .value("kH2", SkPDF::DocumentStructureType::kH2)
    .value("kH3", SkPDF::DocumentStructureType::kH3)
    .value("kH4", SkPDF::DocumentStructureType::kH4)
    .value("kH5", SkPDF::DocumentStructureType::kH5)
    .value("kH6", SkPDF::DocumentStructureType::kH6)
    .value("kP", SkPDF::DocumentStructureType::kP)
    .value("kL", SkPDF::DocumentStructureType::kL)
    .value("kLI", SkPDF::DocumentStructureType::kLI)
    .value("kLbl", SkPDF::DocumentStructureType::kLbl)
    .value("kLBody", SkPDF::DocumentStructureType::kLBody)
    .value("kTable", SkPDF::DocumentStructureType::kTable)
    .value("kTR", SkPDF::DocumentStructureType::kTR)
    .value("kTH", SkPDF::DocumentStructureType::kTH)
    .value("kTD", SkPDF::DocumentStructureType::kTD)
    .value("kTHead", SkPDF::DocumentStructureType::kTHead)
    .value("kTBody", SkPDF::DocumentStructureType::kTBody)
    .value("kTFoot", SkPDF::DocumentStructureType::kTFoot)
    .value("kSpan", SkPDF::DocumentStructureType::kSpan)
    .value("kQuote", SkPDF::DocumentStructureType::kQuote)
    .value("kNote", SkPDF::DocumentStructureType::kNote)
    .value("kReference", SkPDF::DocumentStructureType::kReference)
    .value("kBibEntry", SkPDF::DocumentStructureType::kBibEntry)
    .value("kCode", SkPDF::DocumentStructureType::kCode)
    .value("kLink", SkPDF::DocumentStructureType::kLink)
    .value("kAnnot", SkPDF::DocumentStructureType::kAnnot)
    .value("kRuby", SkPDF::DocumentStructureType::kRuby)
    .value("kRB", SkPDF::DocumentStructureType::kRB)
    .value("kRT", SkPDF::DocumentStructureType::kRT)
    .value("kRP", SkPDF::DocumentStructureType::kRP)
    .value("kWarichu", SkPDF::DocumentStructureType::kWarichu)
    .value("kWT", SkPDF::DocumentStructureType::kWT)
    .value("kWP", SkPDF::DocumentStructureType::kWP)
    .value("kFigure", SkPDF::DocumentStructureType::kFigure)
    .value("kFormula", SkPDF::DocumentStructureType::kFormula)
    .value("kForm", SkPDF::DocumentStructureType::kForm)
    .export_values();
    */

py::class_<SkPDF::AttributeList>(pdf, "AttributeList")
    .def(py::init<>())
    .def("appendInt", &SkPDF::AttributeList::appendInt,
        py::arg("owner"), py::arg("name"), py::arg("value"))
    .def("appendFloat", &SkPDF::AttributeList::appendFloat,
        py::arg("owner"), py::arg("name"), py::arg("value"))
    .def("appendString", &SkPDF::AttributeList::appendName,
        py::arg("owner"), py::arg("name"), py::arg("value"))
    .def("appendFloatArray", &SkPDF::AttributeList::appendFloatArray,
        py::arg("owner"), py::arg("name"), py::arg("value"))
    .def("appendStringArray", &SkPDF::AttributeList::appendNodeIdArray,
        py::arg("owner"), py::arg("name"), py::arg("value"))
    ;

py::class_<SkPDF::StructureElementNode>(pdf, "StructureElementNode",
    R"docstring(
    A node in a PDF structure tree, giving a semantic representation
    of the content.  Each node ID is associated with content
    by passing the :py:class:`Canvas` and node ID to :py:class:`PDF.SetNodeId`
    when drawing. NodeIDs should be unique within each tree.
    )docstring")
    .def(py::init<>())
    .def_readwrite("fTypeString",
        &SkPDF::StructureElementNode::fTypeString)
    // .def_readwrite("fChildVector",
    //     &SkPDF::StructureElementNode::fChildVector)
    .def_readwrite("fNodeId",
        &SkPDF::StructureElementNode::fNodeId)
    .def_readonly("fAdditionalNodeIds",
        &SkPDF::StructureElementNode::fAdditionalNodeIds)
    .def_readonly("fAttributes",
        &SkPDF::StructureElementNode::fAttributes)
    .def_readwrite("fAlt",
        &SkPDF::StructureElementNode::fAlt)
    .def_readwrite("fLang",
        &SkPDF::StructureElementNode::fLang)
    ;

py::class_<SkPDF::Metadata>(pdf, "Metadata",
    R"docstring(
    Optional metadata to be passed into the PDF factory function.
    )docstring")
    .def(py::init<>())
    .def(py::init(&DictToMetadata))
    .def_readwrite("fTitle", &SkPDF::Metadata::fTitle,
        R"docstring(
        The document's title.
        )docstring")
    .def_readwrite("fAuthor", &SkPDF::Metadata::fAuthor,
        R"docstring(
        The name of the person who created the document.
        )docstring")
    .def_readwrite("fSubject", &SkPDF::Metadata::fSubject,
        R"docstring(
        The subject of the document.
        )docstring")
    .def_readwrite("fKeywords", &SkPDF::Metadata::fKeywords,
        R"docstring(
        Keywords associated with the document.  Commas may be used to delineate
        keywords within the string.
        )docstring")
    .def_readwrite("fCreator", &SkPDF::Metadata::fCreator,
        R"docstring(
        If the document was converted to PDF from another format,
        the name of the conforming product that created the
        original document from which it was converted.
        )docstring")
    .def_readwrite("fProducer", &SkPDF::Metadata::fProducer,
        R"docstring(
        The product that is converting this document to PDF.
        )docstring")
    // TODO: Support datetime object conversion.
    /*
    .def_readwrite("fCreation", &SkPDF::Metadata::fCreation,
        R"docstring(
        The date and time the document was created.
        The zero default value represents an unknown/unset time.
        )docstring")
    .def_readwrite("fModified", &SkPDF::Metadata::fModified,
        R"docstring(
        The date and time the document was most recently modified.
        The zero default value represents an unknown/unset time.
        )docstring")
    */
    .def_readwrite("fRasterDPI", &SkPDF::Metadata::fRasterDPI,
        R"docstring(
        The DPI (pixels-per-inch) at which features without native PDF support
        will be rasterized (e.g. draw image with perspective, draw text with
        perspective, ...)  A larger DPI would create a PDF that reflects the
        original intent with better fidelity, but it can make for larger PDF
        files too, which would use more memory while rendering, and it would be
        slower to be processed or sent online or to printer.
        )docstring")
    .def_readwrite("fPDFA", &SkPDF::Metadata::fPDFA,
        R"docstring(
        If true, include XMP metadata, a document UUID, and sRGB output intent
        information.  This adds length to the document and makes it
        non-reproducable, but are necessary features for PDF/A-2b conformance
        )docstring")
    .def_readwrite("fEncodingQuality", &SkPDF::Metadata::fEncodingQuality,
        R"docstring(
        Encoding quality controls the trade-off between size and quality. By
        default this is set to 101 percent, which corresponds to lossless
        encoding. If this value is set to a value <= 100, and the image is
        opaque, it will be encoded (using JPEG) with that quality setting.
        )docstring")
    .def_readwrite("fStructureElementTreeRoot",
        &SkPDF::Metadata::fStructureElementTreeRoot,
        R"docstring(
        An optional tree of structured document tags that provide
        a semantic representation of the content. The caller
        should retain ownership.
        )docstring")
    /*
    .def_readwrite("fExecutor", &SkPDF::Metadata::fExecutor,
        R"docstring(
        Executor to handle threaded work within PDF Backend. If this is nullptr,
        then all work will be done serially on the main thread. To have worker
        threads assist with various tasks, set this to a valid SkExecutor
        instance. Currently used for executing Deflate algorithm in parallel.

        If set, the PDF output will be non-reproducible in the order and
        internal numbering of objects, but should render the same.

        Experimental.
        )docstring")
    .def_readwrite("fSubsetter", &SkPDF::Metadata::fSubsetter,
        R"docstring(
        Preferred Subsetter. Only respected if both are compiled in.

        The Sfntly subsetter is deprecated.

        Experimental.
        )docstring")
    */
    ;

pdf
    .def_static("SetNodeId", &SkPDF::SetNodeId,
        R"docstring(
        Associate a node ID with subsequent drawing commands in an
        :py:class:`Canvas`.

        The same node ID can appear in a :py:class:`~PDF.StructureElementNode`
        in order to associate a document's structure element tree with its
        content.

        A node ID of zero indicates no node ID.

        :param canvas: The canvas used to draw to the PDF.
        :param nodeId: The node ID for subsequent drawing commands.
        )docstring",
        py::arg("canvas"), py::arg("nodeId"))
    .def_static("MakeDocument",
        py::overload_cast<SkWStream*, const SkPDF::Metadata&>(
            &SkPDF::MakeDocument),
        R"docstring(
        Create a PDF-backed document, writing the results into a
        :py:class:`WStream`.

        PDF pages are sized in point units. 1 pt == 1/72 inch == 127/360 mm.

        :stream:  A PDF document will be written to this stream. The
            document may write to the stream at anytime during its lifetime,
            until either :py:meth:`~Document.close` is called or the document is
            deleted.
        :metadata:  a PDFmetadata object. Any fields may be left empty.
        :return: NULL if there is an error, otherwise a newly created PDF-backed
            :py:class:`Document`.
        )docstring",
        py::arg("stream"),  py::arg("metadata"))
    .def_static("MakeDocument",
        py::overload_cast<SkWStream*>(&SkPDF::MakeDocument),
        py::arg("stream"))
    .def_static("MakeDocument",
        [] (SkWStream* stream, py::kwargs kwargs) {
            return SkPDF::MakeDocument(stream, DictToMetadata(kwargs));
        },
        py::arg("stream"));

}  // initDocument
