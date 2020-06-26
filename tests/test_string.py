import skia
import pytest


@pytest.fixture
def string():
    return skia.String('foo')


def test_String_str(string):
    assert isinstance(str(string), str)


def test_String_repr(string):
    assert isinstance(repr(string), str)


@pytest.mark.parametrize('args', [
    tuple(),
    (4,),
    ('foo',),
    ('foo', 3),
    (skia.String('foo'),),
])
def test_String_init(args):
    assert isinstance(skia.String(*args), skia.String)


def test_String_isEmpty(string):
    assert not string.isEmpty()


def test_String_size(string):
    assert string.size() == 3


def test_String_c_str(string):
    assert string.c_str() == 'foo'


def test_String_getitem(string):
    assert string[0] == 'f'


def test_String_setitem():
    string = skia.String(1)
    string[0] = 'f'
    assert string.c_str() == 'f'


def test_String_len(string):
    assert len(string) == 3


def test_String_iter(string):
    assert isinstance(list(string), list)


@pytest.mark.parametrize('args', [
    (skia.String('foo'),),
    ('foo',),
    ('foo', 3),
])
def test_String_equals(string, args):
    assert isinstance(string.equals(*args), bool)


def test_String___eq__(string):
    assert string == string


@pytest.mark.parametrize('args', [
    ('foo',),
    ('f',),
])
def test_String_startsWith(string, args):
    assert isinstance(string.startsWith(*args), bool)


@pytest.mark.parametrize('args', [
    ('foo',),
    ('f',),
])
def test_String_endsWith(string, args):
    assert isinstance(string.endsWith(*args), bool)


@pytest.mark.parametrize('args', [
    ('foo',),
    ('f',),
])
def test_String_contains(string, args):
    assert isinstance(string.contains(*args), bool)


def test_String___contains__(string):
    assert 'foo' in string


def test_String_find(string):
    assert isinstance(string.find('oo'), int)


def test_String_findLastOf(string):
    assert isinstance(string.findLastOf('o'), int)


def test_String_reset(string):
    string.reset()


def test_String_resize(string):
    string.resize(3)


@pytest.mark.parametrize('args', [
    (skia.String('foo'),),
    ('foo',),
    ('foo', 3),
])
def test_String_set(string, args):
    string.set(*args)


@pytest.mark.parametrize('args', [
    (0, skia.String('foo'),),
    (0, 'foo',),
    (0, 'foo', 3),
])
def test_String_insert(string, args):
    string.insert(*args)


def test_String_insertUnichar(string):
    string.insertUnichar(0, ord('x'))


def test_String_insertS32(string):
    string.insertS32(0, 0)


def test_String_insertS64(string):
    string.insertS64(0, 0)


def test_String_insertU32(string):
    string.insertU32(0, 0)


def test_String_insertU64(string):
    string.insertU64(0, 0)


def test_String_insertHex(string):
    string.insertHex(0, 0)


def test_String_insertScalar(string):
    string.insertScalar(0, 0.0)


@pytest.mark.parametrize('args', [
    (skia.String('foo'),),
    ('foo',),
    ('foo', 3),
])
def test_String_append(string, args):
    string.append(*args)


def test_String_appendUnichar(string):
    string.appendUnichar(ord('x'))


def test_String_appendS32(string):
    string.appendS32(0)


def test_String_appendS64(string):
    string.appendS64(0)


def test_String_appendU32(string):
    string.appendU32(0)


def test_String_appendU64(string):
    string.appendU64(0)


def test_String_appendHex(string):
    string.appendHex(0)


def test_String_appendScalar(string):
    string.appendScalar(0.0)


@pytest.mark.parametrize('args', [
    (skia.String('foo'),),
    ('foo',),
    ('foo', 3),
])
def test_String_prepend(string, args):
    string.prepend(*args)


def test_String_prependUnichar(string):
    string.prependUnichar(ord('x'))


def test_String_prependS32(string):
    string.prependS32(0)


def test_String_prependS64(string):
    string.prependS64(0)


def test_String_prependHex(string):
    string.prependHex(0)


def test_String_prependScalar(string):
    string.prependScalar(0.0)


def test_String_remove(string):
    string.remove(0, 1)


@pytest.mark.parametrize('operand', [
    skia.String('foo'),
    'foo',
    'f',
])
def test_String___iadd__(string, operand):
    string += operand


def test_String_remove(string):
    string.swap('bar')
