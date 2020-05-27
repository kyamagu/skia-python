import skia
import pytest


@pytest.mark.parametrize('args', [
    tuple(),
    (skia.Region(),),
    (skia.IRect(100, 100),),
])
def test_Region_init(args):
    assert isinstance(skia.Region(*args), skia.Region)


@pytest.fixture
def region():
    return skia.Region(skia.IRect(100, 100))


@pytest.fixture
def other():
    return skia.Region(skia.IRect(50, 50))


def test_Region_eq(region):
    assert region == region


def test_Region_ne(region, other):
    assert region != other


def test_Region_set(region, other):
    assert isinstance(region.set(other), bool)


def test_Region_swap(region, other):
    region.swap(other)


def test_Region_isEmpty(region):
    assert isinstance(region.isEmpty(), bool)


def test_Region_isRect(region):
    assert isinstance(region.isRect(), bool)


def test_Region_isComplex(region):
    assert isinstance(region.isComplex(), bool)


def test_Region_getBounds(region):
    assert isinstance(region.getBounds(), skia.IRect)


def test_Region_computeRegionComplexity(region):
    assert isinstance(region.computeRegionComplexity(), int)


def test_Region_getBoundaryPath(region):
    path = skia.Path()
    assert isinstance(region.getBoundaryPath(path), bool)


def test_Region_setEmpty(region):
    assert isinstance(region.setEmpty(), bool)


def test_Region_setRect(region):
    assert isinstance(region.setRect(skia.IRect(10, 10)), bool)


def test_Region_setRects(region):
    assert isinstance(region.setRects([skia.IRect(10, 10)]), bool)


def test_Region_setRegion(region, other):
    assert isinstance(region.setRegion(other), bool)


def test_Region_setPath(region, other):
    path = skia.Path()
    path.addRect(skia.Rect(50, 50))
    assert isinstance(region.setPath(path, other), bool)


@pytest.mark.parametrize('args', [
    (skia.IRect(10, 10),),
    (skia.Region(),),
])
def test_Region_intersects(region, args):
    assert isinstance(region.intersects(*args), bool)


@pytest.mark.parametrize('args', [
    (5, 5,),
    (skia.IRect(10, 10),),
    (skia.Region(skia.IRect(10, 10)),),
])
def test_Region_contains(region, args):
    assert isinstance(region.contains(*args), bool)


def test_Region_quickContains(region):
    assert isinstance(region.quickContains(skia.IRect(5, 5)), bool)


@pytest.mark.parametrize('args', [
    (skia.IRect(10, 10),),
    (skia.Region(skia.IRect(10, 10)),),
])
def test_Region_quickReject(region, args):
    assert isinstance(region.quickReject(*args), bool)


def test_Region_translate(region):
    assert isinstance(region.translate(10, 10), skia.Region)


@pytest.mark.parametrize('args', [
    (skia.IRect(10, 10), skia.Region.kDifference_Op),
    (skia.Region(skia.IRect(10, 10)), skia.Region.kDifference_Op),
    (skia.IRect(10, 10), skia.Region(skia.IRect(10, 10)),
        skia.Region.kDifference_Op),
    (skia.Region(skia.IRect(10, 10)), skia.IRect(10, 10),
        skia.Region.kDifference_Op),
    (skia.Region(skia.IRect(10, 10)), skia.Region(skia.IRect(10, 10)),
        skia.Region.kDifference_Op),
])
def test_Region_op(region, args):
    assert isinstance(region.op(*args), bool)


@pytest.fixture
def serialized_region(region):
    return region.writeToMemory()


def test_Region_writeToMemory(serialized_region):
    assert isinstance(serialized_region, skia.Data)


def test_Region_readFromMemory(region, serialized_region):
    assert isinstance(region.readFromMemory(serialized_region), int)


def test_Region_iter(region):
    for rect in region:
        assert isinstance(rect, skia.IRect)


def test_Region_cliperator(region):
    for rect in region.cliperator(skia.IRect(100, 100)):
        assert isinstance(rect, skia.IRect)


def test_Region_spanerator(region):
    for left, right in region.spanerator(10, 0, 100):
        pass


def test_Region_sub(region):
    assert isinstance(region - region, skia.Region)


def test_Region_and(region):
    assert isinstance(region & region, skia.Region)


def test_Region_or(region):
    assert isinstance(region | region, skia.Region)


def test_Region_xor(region):
    assert isinstance(region ^ region, skia.Region)


def test_Region_isub(region):
    region -= region


def test_Region_iand(region):
    region &= region


def test_Region_ior(region):
    region |= region


def test_Region_ixor(region):
    region ^= region


@pytest.fixture
def cliperator(region):
    yield skia.Region.Cliperator(region, skia.IRect(100, 100))


def test_Region_Cliperator_init(cliperator):
    assert isinstance(cliperator, skia.Region.Cliperator)


def test_Region_Cliperator_done_next_rect(cliperator):
    while not cliperator.done():
        assert isinstance(cliperator.rect(), skia.IRect)
        cliperator.next()


def test_Region_Cliperator_iter(cliperator):
    for rect in cliperator:
        assert isinstance(rect, skia.IRect)


@pytest.fixture
def iterator(region):
    yield skia.Region.Iterator(region)


def test_Region_Iterator_init(iterator):
    assert isinstance(iterator, skia.Region.Iterator)


def test_Region_Iterator_rewind(iterator):
    assert isinstance(iterator.rewind(), bool)


def test_Region_Iterator_done_next_rect(iterator):
    while not iterator.done():
        assert isinstance(iterator.rect(), skia.IRect)
        iterator.next()


def test_Region_Iterator_rgn(iterator):
    assert isinstance(iterator.rgn(), skia.Region)


def test_Region_Iterator_reset(region):
    iterator = skia.Region.Iterator()
    iterator.reset(region)


def test_Region_Iterator_iter(iterator):
    for rect in iterator:
        assert isinstance(rect, skia.IRect)


@pytest.fixture
def spanerator(region):
    yield skia.Region.Spanerator(region, 10, 0, 100)


def test_Region_Spanerator_next(spanerator):
    left, right = 0, 0
    while spanerator.next(left, right):
        assert isinstance(left, int)
        assert isinstance(right, int)


def test_Region_Spanerator_iter(spanerator):
    for left, right in spanerator:
        assert isinstance(left, int)
        assert isinstance(right, int)
