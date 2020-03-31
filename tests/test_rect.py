import pytest
import skia


@pytest.fixture()
def irect():
    return skia.IRect(0, 0, 240, 120)


def test_IRect_init():
    assert isinstance(skia.IRect(), skia.IRect)
    assert isinstance(skia.IRect(240, 120), skia.IRect)
    assert isinstance(skia.IRect(0, 0, 240, 120), skia.IRect)
    assert isinstance(skia.IRect.MakeEmpty(), skia.IRect)
    assert isinstance(skia.IRect.MakeWH(240, 120), skia.IRect)
    assert isinstance(skia.IRect.MakeSize(skia.ISize.MakeEmpty()), skia.IRect)
    assert isinstance(skia.IRect.MakeLTRB(0, 0, 240, 120), skia.IRect)
    assert isinstance(skia.IRect.MakeXYWH(0, 0, 240, 120), skia.IRect)


def test_IRect_methods(irect):
    assert irect.__repr__() == 'IRect(0, 0, 240, 120)'
    assert irect.left() == 0
    assert irect.top() == 0
    assert irect.right() == 240
    assert irect.bottom() == 120
    assert irect.x() == 0
    assert irect.y() == 0
    assert irect.topLeft() == skia.IPoint.Make(0, 0)
    assert irect.width() == 240
    assert irect.height() == 120
    assert isinstance(irect.size(), skia.ISize)
    assert irect.width64() == 240
    assert irect.height64() == 120
    assert not irect.isEmpty64()
    assert not irect.isEmpty()
    irect.setEmpty()
    irect.setLTRB(0, 0, 240, 120)
    irect.setXYWH(0, 0, 240, 120)
    irect.setWH(240, 120)
    assert isinstance(irect.makeOffset(10, 10), skia.IRect)
    assert isinstance(irect.makeOffset(skia.IPoint.Make(10, 10)), skia.IRect)
    assert isinstance(irect.makeInset(10, 10), skia.IRect)
    assert isinstance(irect.makeOutset(10, 10), skia.IRect)
    irect.offset(0, 0)
    irect.offset(skia.IPoint.Make(0, 0))
    irect.inset(0, 0)
    irect.outset(0, 0)
    irect.adjust(0, 0, 0, 0)
    assert irect.contains(20, 10)
    assert irect.contains(skia.IRect.MakeLTRB(10, 10, 20, 20))
    assert irect.contains(skia.Rect.MakeLTRB(10, 10, 20, 20))
    assert irect.containsNoEmptyCheck(skia.IRect.MakeLTRB(10, 10, 20, 20))
    assert irect.intersect(skia.IRect.MakeLTRB(0, 0, 250, 130))
    assert irect.intersect(skia.IRect.MakeLTRB(0, 0, 240, 120),
                           skia.IRect.MakeLTRB(0, 0, 250, 130))
    irect.join(skia.IRect.MakeLTRB(10, 10, 20, 20))
    irect.sort()
    assert isinstance(irect.makeSorted(), skia.IRect)
    assert skia.IRect.Intersects(irect, skia.IRect(10, 10, 20, 20))


def test_IRect_attributes(irect):
    assert irect.fLeft == 0
    assert irect.fTop == 0
    assert irect.fRight == 240
    assert irect.fBottom == 120


def test_IRect_operators(irect):
    assert irect == skia.IRect.MakeLTRB(0, 0, 240, 120)
    assert irect != skia.IRect.MakeEmpty()


@pytest.fixture()
def rect():
    return skia.Rect(0, 0, 240, 120)


def test_Rect_init():
    assert isinstance(skia.Rect(), skia.Rect)
    assert isinstance(skia.Rect(240, 120), skia.Rect)
    assert isinstance(skia.Rect(0, 0, 240, 120), skia.Rect)
    assert isinstance(skia.Rect.MakeEmpty(), skia.Rect)
    assert isinstance(skia.Rect.MakeWH(240, 120), skia.Rect)
    assert isinstance(skia.Rect.MakeSize(skia.Size.MakeEmpty()), skia.Rect)
    assert isinstance(skia.Rect.MakeLTRB(0, 0, 240, 120), skia.Rect)
    assert isinstance(skia.Rect.MakeXYWH(0, 0, 240, 120), skia.Rect)
    assert isinstance(skia.Rect.Make(skia.ISize.MakeEmpty()), skia.Rect)
    assert isinstance(skia.Rect.Make(skia.IRect.MakeEmpty()), skia.Rect)


def test_Rect_methods(rect):
    assert rect.__repr__() == 'Rect(0, 0, 240, 120)'
    assert not rect.isEmpty()
    assert rect.isSorted()
    assert rect.isFinite()
    assert rect.x() == 0.
    assert rect.y() == 0.
    assert rect.left() == 0.
    assert rect.top() == 0.
    assert rect.right() == 240.
    assert rect.bottom() == 120.
    assert rect.width() == 240.
    assert rect.height() == 120.
    assert rect.centerX() == 120.
    assert rect.centerY() == 60.
    # rect.toQuad()
    rect.setEmpty()
    rect.set(skia.IRect(240, 120))
    rect.setLTRB(0, 0, 240, 120)
    # rect.setBounds()
    # rect.setBoundsCheck()
    # rect.setBoundsNoCheck()
    rect.set(skia.Point.Make(0, 0), skia.Point.Make(240, 120))
    rect.setXYWH(0, 0, 240, 120)
    rect.setWH(240, 120)
    rect.setIWH(240, 120)
    assert isinstance(rect.makeOffset(10, 10), skia.Rect)
    assert isinstance(rect.makeOffset(skia.Point.Make(10, 10)), skia.Rect)
    assert isinstance(rect.makeInset(10, 10), skia.Rect)
    assert isinstance(rect.makeOutset(10, 10), skia.Rect)
    rect.offset(0, 0)
    rect.offset(skia.Point.Make(0, 0))
    rect.offsetTo(0, 0)
    rect.inset(0, 0)
    rect.outset(0, 0)
    assert rect.intersect(skia.Rect.MakeLTRB(0, 0, 250, 130))
    assert rect.intersect(skia.Rect.MakeLTRB(0, 0, 240, 120),
                           skia.Rect.MakeLTRB(0, 0, 250, 130))
    rect.intersects(skia.Rect.MakeWH(20, 20))
    rect.join(skia.Rect.MakeLTRB(10, 10, 20, 20))
    rect.joinNonEmptyArg(skia.Rect.MakeLTRB(10, 10, 20, 20))
    rect.joinPossiblyEmptyRect(skia.Rect.MakeEmpty())
    assert rect.contains(20, 10)
    assert rect.contains(skia.Rect.MakeLTRB(10, 10, 20, 20))
    assert rect.contains(skia.IRect.MakeLTRB(10, 10, 20, 20))
    irect = skia.IRect.MakeEmpty()
    rect2 = skia.Rect.MakeEmpty()
    rect.round(irect)
    rect.roundOut(irect)
    rect.roundOut(rect2)
    rect.roundIn(irect)
    assert isinstance(rect.round(), skia.IRect)
    assert isinstance(rect.roundOut(), skia.IRect)
    rect.sort()
    assert isinstance(rect.makeSorted(), skia.Rect)
    rect.asScalars()
    rect.dump(True)
    rect.dump()
    rect.dumpHex()
    assert skia.Rect.Intersects(rect, skia.Rect.MakeLTRB(10, 10, 20, 20))


def test_Rect_attributes(rect):
    assert rect.fLeft == 0
    assert rect.fTop == 0
    assert rect.fRight == 240
    assert rect.fBottom == 120


def test_Rect_operators(rect):
    assert rect == skia.Rect.MakeLTRB(0, 0, 240, 120)
    assert rect != skia.Rect.MakeEmpty()


@pytest.fixture()
def rrect():
    return skia.RRect.MakeOval(skia.Rect.MakeWH(128, 128))


def test_RRect_init():
    skia.RRect.MakeEmpty()
    skia.RRect.MakeRect(skia.Rect.MakeEmpty())
    skia.RRect.MakeOval(skia.Rect.MakeEmpty())
    skia.RRect.MakeRectXY(skia.Rect.MakeWH(10, 10), 1, 1)


def test_RRect_methods(rrect):
    assert isinstance(rrect.getType(), skia.RRect.Type)
    assert isinstance(rrect.type(), skia.RRect.Type)
    assert not rrect.isEmpty()
    assert not rrect.isRect()
    assert rrect.isOval()
    assert not rrect.isSimple()
    assert not rrect.isNinePatch()
    assert not rrect.isComplex()
    assert rrect.width() == 128
    assert rrect.height() == 128
    assert isinstance(rrect.getSimpleRadii(), skia.Point)
    rrect.setEmpty()
    assert rrect.isEmpty()
    rrect.setRect(skia.Rect.MakeWH(128, 128))
    assert rrect.isRect()
    rrect.setOval(skia.Rect.MakeWH(128, 128))
    assert rrect.isOval()
    rrect.setRectXY(skia.Rect.MakeWH(128, 128), 1, 1)
    rrect.setNinePatch(skia.Rect.MakeWH(128, 128), 1, 2, 3, 1)
    assert rrect.isNinePatch()
    # rrect.setRectRadii()
    assert isinstance(rrect.rect(), skia.Rect)
    # assert isinstance(rrect.radii(skia.RRect.Corner), skia.Point)
    assert isinstance(rrect.getBounds(), skia.Rect)
    rrect2 = skia.RRect()
    rrect.inset(0, 0, rrect2)
    rrect.inset(0, 0)
    rrect.outset(0, 0, rrect2)
    rrect.outset(0, 0)
    rrect.offset(0, 0)
    assert isinstance(rrect.makeOffset(0, 0), skia.RRect)
    assert rrect.contains(skia.Rect(10, 10, 20, 20))
    assert rrect.isValid()
    # rrect.writeToMemory()
    # rrect.readFromMemory()
    # rrect.transform()
    rrect.dump(True)
    rrect.dump()
    rrect.dumpHex()


def test_RRect_operators(rrect):
    assert rrect == skia.RRect.MakeOval(skia.Rect.MakeWH(128, 128))
    assert rrect != skia.RRect.MakeEmpty()
