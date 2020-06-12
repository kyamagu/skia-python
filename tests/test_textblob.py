import skia
import pytest


@pytest.fixture
def textblob():
    return skia.TextBlob('foo', skia.Font())


@pytest.mark.parametrize('args', [
    ('foo', skia.Font()),
    ('foo', skia.Font(), [(0, 10), (5, 10), (10, 10)]),
])
def test_TextBlob_init(args):
    assert isinstance(skia.TextBlob(*args), skia.TextBlob)


def test_TextBlob_iter(textblob):
    for run in textblob:
        assert isinstance(run, skia.TextBlob.Iter.Run)


def test_TextBlob_bounds(textblob):
    assert isinstance(textblob.bounds(), skia.Rect)


def test_TextBlob_serialize(textblob):
    assert isinstance(textblob.serialize(), skia.Data)


def test_TextBlob_uniqueID(textblob):
    assert isinstance(textblob.uniqueID(), int)


def test_TextBlob_getIntercepts(textblob):
    assert isinstance(textblob.getIntercepts([0, 1]), list)


def test_TextBlob_MakeFromText():
    assert isinstance(
        skia.TextBlob.MakeFromText('foo', skia.Font()), skia.TextBlob)


def test_TextBlob_MakeFromString():
    assert isinstance(
        skia.TextBlob.MakeFromString('foo', skia.Font()), skia.TextBlob)


def test_TextBlob_MakeFromPosTextH():
    assert isinstance(
        skia.TextBlob.MakeFromPosTextH(
            'foo', [0, 1, 2], 0, skia.Font()), skia.TextBlob)


def test_TextBlob_MakeFromPosText():
    assert isinstance(
        skia.TextBlob.MakeFromPosText(
            'foo', [skia.Point(0, 0), skia.Point(1, 0), skia.Point(2, 0)],
            skia.Font()),
        skia.TextBlob)


def test_TextBlob_MakeFromRSXform():
    xform = [
        skia.RSXform(1, 0, 0, 0),
        skia.RSXform(1, 0, 1, 0),
        skia.RSXform(1, 0, 2, 0),
    ]
    assert isinstance(
        skia.TextBlob.MakeFromRSXform('foo', xform, skia.Font()), skia.TextBlob)


def test_TexbBlob_Deserialize(textblob):
    data = textblob.serialize()
    assert isinstance(skia.TextBlob.Deserialize(data), skia.TextBlob)


def test_TextBlob_Iter_init(textblob):
    assert isinstance(skia.TextBlob.Iter(textblob), skia.TextBlob.Iter)


def test_TextBlob_Iter_next(textblob):
    run = skia.TextBlob.Iter.Run()
    it = skia.TextBlob.Iter(textblob)
    assert isinstance(it.next(run), bool)


@pytest.fixture
def run(textblob):
    run = skia.TextBlob.Iter.Run()
    it = skia.TextBlob.Iter(textblob)
    while it.next(run):
        break
    yield run


def test_TextBlob_Iter_Run_repr(run):
    assert isinstance(repr(run), str)


def test_TextBlob_Iter_Run_fTypeface(run):
    assert isinstance(run.fTypeface, (skia.Typeface, type(None)))


def test_TextBlob_Iter_Run_fGlyphCount(run):
    assert isinstance(run.fGlyphCount, int)


def test_TextBlob_Iter_Run_fGlyphIndices(run):
    assert isinstance(run.fGlyphIndices, list)


@pytest.fixture
def builder():
    return skia.TextBlobBuilder()


def test_TextBlobBuilder_init():
    assert isinstance(skia.TextBlobBuilder(), skia.TextBlobBuilder)


def test_TextBlobBuilder_allocRun(builder):
    builder.allocRun('foo', skia.Font(), 0, 0)


def test_TextBlobBuilder_allocRunPosH(builder):
    font = skia.Font()
    builder.allocRunPosH(font, font.textToGlyphs('foo'), [0, 1, 2], 0)


def test_TextBlobBuilder_allocRunPos(builder):
    font = skia.Font()
    builder.allocRunPos(
        skia.Font(), font.textToGlyphs('foo'), [(0, 0), (1, 0), (2, 0)])


def test_TextBlobBuilder_allocRunRSXform(builder):
    font = skia.Font()
    xform = [
        skia.RSXform(1, 0, 0, 0),
        skia.RSXform(1, 0, 1, 0),
        skia.RSXform(1, 0, 2, 0),
    ]
    builder.allocRunRSXform(font, font.textToGlyphs('foo'), xform)


def test_TextBlobBuilder_make(builder):
    builder.allocRun('foo', skia.Font(), 0, 0)
    assert isinstance(builder.make(), skia.TextBlob)
