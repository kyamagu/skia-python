import skia
import pytest


@pytest.fixture(scope='session')
def path():
    return skia.Path()


@pytest.fixture(scope='session')
def itr():
    return skia.Path.Iter()


@pytest.fixture(scope='session')
def rawiter():
    return skia.Path.RawIter()


@pytest.mark.parametrize('args', [
    tuple(),
])
def test_Path_init(args):
    assert isinstance(skia.Path(*args), skia.Path)


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.Path(), True),
])
def test_Path_Iter_init(args):
    assert isinstance(skia.Path.Iter(*args), skia.Path.Iter)


def test_Path_Iter_setPath(itr):
    itr.setPath(skia.Path(), True)


def test_Path_Iter_next(itr):
    result = itr.next()
    assert isinstance(result, tuple)
    assert isinstance(result[0], skia.Path.Verb)
    assert isinstance(result[1], list)


# def test_Path_Iter_conicWeight(itr):
#     assert isinstance(itr.conicWeight())


def test_Path_Iter_isCloseLine(itr):
    assert isinstance(itr.isCloseLine(), bool)


def test_Path_Iter_isClosedContour(itr):
    assert isinstance(itr.isClosedContour(), bool)


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.Path(),),
])
def test_Path_RawIter_init(args):
    assert isinstance(skia.Path.RawIter(*args), skia.Path.RawIter)


def test_Path_RawIter_setPath(rawiter):
    rawiter.setPath(skia.Path())


def test_Path_RawIter_next(rawiter):
    result = rawiter.next()
    assert isinstance(result, tuple)
    assert isinstance(result[0], skia.Path.Verb)
    assert isinstance(result[1], list)


def test_Path_RawIter_peek(rawiter):
    assert isinstance(rawiter.peek(), skia.Path.Verb)


# def test_Path_RawIter_conicWeight(rawiter):
#     assert isinstance(rawiter.conicWeight())
