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
        py::arg("width"), py::arg("height"))
    .def("beginPage", &SkDocument::beginPage,
        R"docstring(
        Begin a new page for the document, returning the canvas that will draw
        into the page.

        The document owns this canvas, and it will go out of scope when
        :py:meth:`endPage` or :py:meth:`close` is called, or the document is
        deleted.
        )docstring",
        py::arg("width"), py::arg("height"), py::arg("content") = nullptr,
        py::return_value_policy::reference)
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
        py::return_value_policy::reference)
    .def("__exit__",
        [] (PyAutoDocumentPage& page, py::object exc_type, py::object exc_value,
            py::object traceback) { page.endPage(); })
    ;

py::class_<PyPDF> pdf(m, "PDF");

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

py::class_<SkPDF::AttributeList>(pdf, "AttributeList")
    .def(py::init<>())
    .def("appendInt", &SkPDF::AttributeList::appendInt,
        py::arg("owner"), py::arg("name"), py::arg("value"))
    .def("appendFloat", &SkPDF::AttributeList::appendFloat,
        py::arg("owner"), py::arg("name"), py::arg("value"))
    .def("appendString", &SkPDF::AttributeList::appendString,
        py::arg("owner"), py::arg("name"), py::arg("value"))
    .def("appendFloatArray", &SkPDF::AttributeList::appendFloatArray,
        py::arg("owner"), py::arg("name"), py::arg("value"))
    .def("appendStringArray", &SkPDF::AttributeList::appendStringArray,
        py::arg("owner"), py::arg("name"), py::arg("value"))
    ;

py::class_<SkPDF::StructureElementNode>(pdf, "StructureElementNode")
    .def(py::init<>())
    ;

py::class_<SkPDF::Metadata>(pdf, "Metadata")
    .def(py::init<>())
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
        [] (SkWStream* stream, const SkPDF::Metadata* metadata) {
            if (metadata)
                return SkPDF::MakeDocument(stream, *metadata);
            return SkPDF::MakeDocument(stream);
        },
        R"docstring(
        Create a PDF-backed document, writing the results into a
        :py:class:`WStream`.

        PDF pages are sized in point units. 1 pt == 1/72 inch == 127/360 mm.

        :param stream:  A PDF document will be written to this stream. The
            document may write to the stream at anytime during its lifetime,
            until either :py:meth:`~Document.close` is called or the document is
            deleted.
        :param metadata:  a PDFmetadata object. Any fields may be left empty.
        :return: NULL if there is an error, otherwise a newly created PDF-backed
            :py:class:`Document`.
        )docstring",
        py::arg("stream"),  py::arg("metadata") = nullptr);

}  // initDocument
