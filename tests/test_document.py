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


def test_Document_enter_exit(stream):
    with skia.PDF.MakeDocument(stream) as document:
        assert isinstance(document, skia.Document)


def test_Document_page(document):
    with document.page(100, 100) as canvas:
        assert isinstance(canvas, skia.Canvas)


def test_Document_beginPage_endPage(document_canvas):
    assert isinstance(document_canvas, skia.Canvas)


def test_Document_abort(document):
    document.abort()


def test_PDF_SetNodeId(document_canvas):
    skia.PDF.SetNodeId(document_canvas, 1)


def test_PDF_MakeDocument_1(stream):
    assert isinstance(
        skia.PDF.MakeDocument(stream, skia.PDF.Metadata()), skia.Document)


def test_PDF_MakeDocument_2(stream):
    assert isinstance(skia.PDF.MakeDocument(stream), skia.Document)


def test_PDF_MakeDocument_3(stream):
    assert isinstance(skia.PDF.MakeDocument(stream, Title='foo'), skia.Document)


@pytest.fixture
def attribute_list():
    return skia.PDF.AttributeList()


def test_PDF_AttributeList_init(attribute_list):
    assert isinstance(attribute_list, skia.PDF.AttributeList)


def test_PDF_AttributeList_appendInt(attribute_list):
    attribute_list.appendInt('Layout', 'RowSpan', 1)


def test_PDF_AttributeList_appendFloat(attribute_list):
    attribute_list.appendFloat('Layout', 'RowSpan', 1.)


def test_PDF_AttributeList_appendString(attribute_list):
    attribute_list.appendString('Table', 'Title', 'foo')


def test_PDF_AttributeList_appendFloatArray(attribute_list):
    attribute_list.appendFloatArray('Table', 'BBox', [1., 2.])


def test_PDF_AttributeList_appendStringArray(attribute_list):
    attribute_list.appendStringArray('List', 'Label', ['foo', 'bar'])


@pytest.mark.parametrize('args', [tuple(), ({},)])
def test_PDF_Metadata_init(args):
    assert isinstance(skia.PDF.Metadata(*args), skia.PDF.Metadata)


def test_PDF_Metadata_init_kwargs():
    metadata = skia.PDF.Metadata(dict(
        Title='',
        Author='',
        Subject='',
        Keywords='',
        Creator='',
        Producer='',
        RasterDPI=96,
        PDFA=True,
        EncodingQuality=100,
        StructureElementTreeRoot=None,
        ))
    assert isinstance(metadata, skia.PDF.Metadata)


def test_PDF_StructureElementNode_init():
    assert isinstance(
        skia.PDF.StructureElementNode(), skia.PDF.StructureElementNode)
