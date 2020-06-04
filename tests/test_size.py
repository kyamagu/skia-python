import pytest
import skia


@pytest.fixture
def isize():
    return skia.ISize(100, 100)


@pytest.fixture
def size():
    return skia.Size(100., 100.)


@pytest.mark.parametrize('args', [
    tuple(),
    (100, 100,),
    ((100, 100),),
])
def test_ISize_init(args):
    assert isinstance(skia.ISize(*args), skia.ISize)


def test_ISize_Make():
    assert isinstance(skia.ISize.Make(100, 100), skia.ISize)


def test_ISize_MakeEmpty():
    assert isinstance(skia.ISize.MakeEmpty(), skia.ISize)


def test_ISize_set(isize):
    isize.set(100, 100)


def test_ISize_isEmpty(isize):
    assert not isize.isEmpty()


def test_ISize_isZero(isize):
    assert not isize.isZero()


def test_ISize_width(isize):
    assert isize.area() == (100 * 100)


def test_ISize_width(isize):
    assert isize.width() == 100


def test_ISize_height(isize):
    assert isize.height() == 100


def test_ISize_fWidth_fHeight(isize):
    assert isize.fWidth == 100
    assert isize.fHeight == 100


def test_ISize_equals(isize):
    assert isize.equals(100, 100)


def test_ISize_setEmpty(isize):
    isize.setEmpty()
    assert isize.isEmpty()


def test_ISize_iter(isize):
    width, height = tuple(isize)


def test_ISize_len(isize):
    assert len(isize) == 2


def test_ISize_repr(isize):
    assert repr(isize) == 'ISize(100, 100)'


@pytest.mark.parametrize('args', [
    tuple(),
    (1, 1,),
    (skia.ISize(1, 0),),
    ((1, 1,),),
])
def test_Size_init(args):
    assert isinstance(skia.Size(*args), skia.Size)


@pytest.mark.parametrize('args', [
    (skia.ISize(1, 1),),
    (1, 1),
])
def test_Size_Make(args):
    assert isinstance(skia.Size.Make(*args), skia.Size)


def test_Size_MakeEmpty():
    assert isinstance(skia.Size.MakeEmpty(), skia.Size)


def test_Size_set(size):
    size.set(0, 0)


def test_Size_isEmpty(size):
    assert not size.isEmpty()


def test_Size_isZero(size):
    assert not size.isZero()


def test_Size_width(size):
    assert size.width() == 100


def test_Size_height(size):
    assert size.height() == 100


def test_Size_equals(size):
    assert not size.equals(100, 101)


def test_Size_toRound(size):
    assert isinstance(size.toRound(), skia.ISize)


def test_Size_toCeil(size):
    assert isinstance(size.toCeil(), skia.ISize)


def test_Size_toFloor(size):
    assert isinstance(size.toFloor(), skia.ISize)


def test_Size_fWidth_fHeight(size):
    size.fWidth = 1
    size.fHeight = 1
    assert size.fWidth == 1
    assert size.fHeight == 1


def test_Size_setEmpty(size):
    size.setEmpty()
    assert size.isEmpty()


def test_Size_iter(size):
    width, height = tuple(size)


def test_Size_len(size):
    assert len(size) == 2


def test_Size_repr(size):
    assert repr(size) == 'Size(100, 100)'
