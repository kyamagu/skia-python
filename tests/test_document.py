import skia
import pytest


@pytest.fixture
def stream():
    return skia.DynamicMemoryWStream()


@pytest.fixture
def document(stream):
    document = skia.PDF.MakeDocument(stream)
    assert document is not None
    yield document
    document.close()


@pytest.fixture
def document_canvas(document):
    canvas = document.beginPage(100, 100)
    yield canvas
    document.endPage()


def test_Document_beginPage_endPage(document_canvas):
    assert isinstance(document_canvas, skia.Canvas)


def test_Document_abort(document):
    document.abort()


def test_PDF_SetNodeId(document_canvas):
    skia.PDF.SetNodeId(document_canvas, 1)


def test_PDF_MakeDocument(stream):
    assert isinstance(skia.PDF.MakeDocument(stream), skia.Document)
