import pytest
import skia


@pytest.mark.parametrize('width, height, is_empty, is_zero, area', [
    (0, 0, True, True, 0),
    (1, 2, False, False, 2),
    (0, 2, True, False, 0),
])
def test_ISize(width, height, is_empty, is_zero, area):
    assert isinstance(skia.ISize(), skia.ISize)
    assert isinstance(skia.ISize(width, height), skia.ISize)
    assert isinstance(skia.ISize.Make(width, height), skia.ISize)
    size = skia.ISize.MakeEmpty()
    assert isinstance(size, skia.ISize)
    size.set(width, height)
    assert size.isEmpty() == is_empty
    assert size.isZero() == is_zero
    assert size.area() == area
    assert size.width() == width
    assert size.height() == height
    assert size.fWidth == width
    assert size.fHeight == height
    assert size.equals(width, height)
    assert not size.equals(width, height + 1)
    size.setEmpty()
    assert size.isEmpty()


@pytest.mark.parametrize('width, height, is_empty, is_zero', [
    (0., 0., True, True),
    (1., 2., False, False),
    (0., 2., True, False),
])
def test_Size(width, height, is_empty, is_zero):
    assert isinstance(skia.Size(), skia.Size)
    assert isinstance(skia.Size(width, height), skia.Size)
    assert isinstance(skia.Size(skia.ISize()), skia.Size)
    assert isinstance(skia.Size.Make(width, height), skia.Size)
    assert isinstance(
        skia.Size.Make(skia.ISize.Make(int(width), int(height))), skia.Size)
    size = skia.Size.MakeEmpty()
    assert isinstance(size, skia.Size)
    size.set(width, height)
    assert size.isEmpty() == is_empty
    assert size.isZero() == is_zero
    assert size.width() == width
    assert size.height() == height
    assert size.equals(width, height)
    assert not size.equals(width, height + 1.)
    assert isinstance(size.toRound(), skia.ISize)
    assert isinstance(size.toCeil(), skia.ISize)
    assert isinstance(size.toFloor(), skia.ISize)
    assert size.fWidth == width
    assert size.fHeight == height
    size.setEmpty()
    assert size.isEmpty()
