import skia
import pytest


@pytest.fixture
def textblob():
    return skia.TextBlob('foo', skia.Font())


def test_TextBlob_init():
    assert isinstance(skia.TextBlob('foo', skia.Font()), skia.TextBlob)


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
    builder.allocRun(skia.Font(), [0x20, 0x21], 0, 0)


def test_TextBlobBuilder_allocRunPosH(builder):
    builder.allocRunPosH(skia.Font(), [0x20, 0x21], [0, 1], 0)


def test_TextBlobBuilder_allocRunPos(builder):
    builder.allocRunPos(
        skia.Font(), [0x20, 0x21], [skia.Point(0, 0), skia.Point(1, 0)])


def test_TextBlobBuilder_allocRunRSXform(builder):
    xform = [
        skia.RSXform(1, 0, 0, 0),
        skia.RSXform(1, 0, 1, 0),
    ]
    builder.allocRunRSXform(skia.Font(), [0x20, 0x21], xform)


def test_TextBlobBuilder_make(builder):
    builder.allocRun(skia.Font(), [0x20, 0x21], 0, 0)
    assert isinstance(builder.make(), skia.TextBlob)
