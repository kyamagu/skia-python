import pytest
import skia


@pytest.fixture()
def irect():
    return skia.IRect(0, 0, 240, 120)


@pytest.mark.parametrize('args', [
    tuple(),
    (240, 120),
    (0, 0, 240, 120),
    (tuple(),),
    ((240, 120),),
    ((0, 0, 240, 120),),
])
def test_IRect_init(args):
    assert isinstance(skia.IRect(*args), skia.IRect)


def test_IRect_iter(irect):
    assert tuple(irect) == (0, 0, 240, 120)


def test_IRect_len(irect):
    assert len(irect) == 4


def test_IRect_repr(irect):
    assert repr(irect) == 'IRect(0, 0, 240, 120)'


def test_IRect_left(irect):
    assert irect.left() == 0


def test_IRect_top(irect):
    assert irect.top() == 0


def test_IRect_right(irect):
    assert irect.right() == 240


def test_IRect_bottom(irect):
    assert irect.bottom() == 120


def test_IRect_x(irect):
    assert irect.x() == 0


def test_IRect_y(irect):
    assert irect.y() == 0


def test_IRect_topLeft(irect):
    assert irect.topLeft() == (0, 0)


def test_IRect_width(irect):
    assert irect.width() == 240


def test_IRect_height(irect):
    assert irect.height() == 120


def test_IRect_size(irect):
    assert irect.size() == (240, 120)


def test_IRect_width64(irect):
    assert irect.width64() == 240


def test_IRect_height64(irect):
    assert irect.height64() == 120


def test_IRect_isEmpty64(irect):
    assert not irect.isEmpty64()


def test_IRect_isEmpty(irect):
    assert not irect.isEmpty()


def test_IRect_setEmpty(irect):
    irect.setEmpty()


def test_IRect_setLTRB(irect):
    irect.setLTRB(0, 0, 240, 120)


def test_IRect_setXYWH(irect):
    irect.setXYWH(0, 0, 240, 120)


def test_IRect_setWH(irect):
    irect.setWH(240, 120)


@pytest.mark.parametrize('args', [(10, 10), (skia.IPoint(10, 10),),])
def test_IRect_makeOffset(irect, args):
    assert isinstance(irect.makeOffset(*args), skia.IRect)


def test_IRect_makeInset(irect):
    assert isinstance(irect.makeInset(10, 10), skia.IRect)


def test_IRect_makeOutset(irect):
    assert isinstance(irect.makeOutset(10, 10), skia.IRect)


@pytest.mark.parametrize('args', [(10, 10), (skia.IPoint(10, 10),),])
def test_IRect_offset(irect, args):
    irect.offset(*args)


def test_IRect_inset(irect):
    irect.inset(0, 0)


def test_IRect_outset(irect):
    irect.outset(0, 0)


def test_IRect_adjust(irect):
    irect.adjust(0, 0, 0, 0)


@pytest.mark.parametrize('args', [
    (20, 10),
    (skia.IRect(10, 10, 20, 20),),
    (skia.Rect(10, 10, 20, 20),),
])
def test_IRect_contains(irect, args):
    assert irect.contains(*args)


@pytest.mark.parametrize('operand', [
    skia.IRect(10, 10, 20, 20),
    skia.Rect(10, 10, 20, 20),
])
def test_IRect__contains(irect, operand):
    assert operand in irect


def test_IRect__not_contains(irect):
    assert (60, 60) in skia.IRect(50, 50, 100, 100)
    assert (120, 120) not in skia.IRect(50, 50, 100, 100)


def test_IRect_containsNoEmptyCheck(irect):
    assert irect.containsNoEmptyCheck(skia.IRect(10, 10, 20, 20))


def test_IRect_intersect(irect):
    assert irect.intersect(skia.IRect.MakeLTRB(0, 0, 250, 130))
    assert irect.intersect(skia.IRect.MakeLTRB(0, 0, 240, 120),
                           skia.IRect.MakeLTRB(0, 0, 250, 130))


def test_IRect_join(irect):
    irect.join(skia.IRect(10, 10, 20, 20))


def test_IRect_sort(irect):
    irect.sort()


def test_IRect_makeSorted(irect):
    assert isinstance(irect.makeSorted(), skia.IRect)


def test_IRect_Intersects(irect):
    assert skia.IRect.Intersects(irect, skia.IRect(10, 10, 20, 20))


def test_IRect_attributes(irect):
    assert irect.fLeft == 0
    assert irect.fTop == 0
    assert irect.fRight == 240
    assert irect.fBottom == 120


def test_IRect_MakeEmpty():
    assert isinstance(skia.IRect.MakeEmpty(), skia.IRect)


def test_IRect_MakeWH():
    assert isinstance(skia.IRect.MakeWH(240, 120), skia.IRect)


def test_IRect_MakeSize():
    assert isinstance(skia.IRect.MakeSize(skia.ISize()), skia.IRect)


def test_IRect_MakeLTRB():
    assert isinstance(skia.IRect.MakeLTRB(0, 0, 240, 120), skia.IRect)


def test_IRect_MakeXYWH():
    assert isinstance(skia.IRect.MakeXYWH(0, 0, 240, 120), skia.IRect)


def test_IRect_eq(irect):
    assert irect == skia.IRect(0, 0, 240, 120)


def test_IRect_ne(irect):
    assert irect != skia.IRect()


@pytest.fixture()
def rect():
    return skia.Rect(0, 0, 240, 120)


@pytest.mark.parametrize('args', [
    tuple(),
    (240, 120),
    (0, 0, 240, 120),
    (skia.IRect(),),
    (skia.ISize(),),
    (tuple(),),
    ((240, 120),),
    ((0, 0, 240, 120),),
])
def test_Rect_init(args):
    assert isinstance(skia.Rect(*args), skia.Rect)


def test_Rect_iter(rect):
    assert tuple(rect) == (0, 0, 240, 120)


def test_Rect_len(rect):
    assert len(rect) == 4


def test_Rect_repr(rect):
    assert repr(rect) == 'Rect(0, 0, 240, 120)'


def test_Rect_isEmpty(rect):
    assert not rect.isEmpty()


def test_Rect_isSorted(rect):
    assert rect.isSorted()


def test_Rect_isFinite(rect):
    assert rect.isFinite()


def test_Rect_x(rect):
    assert rect.x() == 0.


def test_Rect_y(rect):
    assert rect.y() == 0.


def test_Rect_left(rect):
    assert rect.left() == 0.


def test_Rect_top(rect):
    assert rect.top() == 0.


def test_Rect_right(rect):
    assert rect.right() == 240.


def test_Rect_bottom(rect):
    assert rect.bottom() == 120.


def test_Rect_width(rect):
    assert rect.width() == 240.


def test_Rect_height(rect):
    assert rect.height() == 120.


def test_Rect_centerX(rect):
    assert rect.centerX() == 120.


def test_Rect_centerY(rect):
    assert rect.centerY() == 60.


def test_Rect_toQuad(rect):
    quad = rect.toQuad()
    assert isinstance(quad, list)
    assert len(quad) == 4


def test_Rect_setEmpty(rect):
    rect.setEmpty()


def test_Rect_setLTRB(rect):
    rect.setLTRB(0, 0, 240, 120)


def test_Rect_setBounds(rect):
    rect.setBounds([(0, 0), (100, 100)])


def test_Rect_setBoundsCheck(rect):
    assert rect.setBoundsCheck([(0, 0), (100, 100)])


def test_Rect_setBoundsNoCheck(rect):
    rect.setBoundsNoCheck([(0, 0), (100, 100)])


@pytest.mark.parametrize('args', [
    (skia.IRect(240, 120),),
    (skia.Point(0, 0), skia.Point(240, 120)),
])
def test_Rect_set(rect, args):
    rect.set(*args)


def test_Rect_setXYWH(rect):
    rect.setXYWH(0, 0, 240, 120)


def test_Rect_setWH(rect):
    rect.setWH(240, 120)


def test_Rect_setIWH(rect):
    rect.setIWH(240, 120)


@pytest.mark.parametrize('args', [
    (10, 10,),
    (skia.Point(10, 10),),
])
def test_Point_makeOffset(rect, args):
    assert isinstance(rect.makeOffset(*args), skia.Rect)


def test_Point_makeInset(rect):
    assert isinstance(rect.makeInset(10, 10), skia.Rect)


def test_Point_makeOutset(rect):
    assert isinstance(rect.makeOutset(10, 10), skia.Rect)


@pytest.mark.parametrize('args', [
    (0, 0,),
    (skia.Point(0, 0),),
])
def test_Rect_offset(rect, args):
    rect.offset(*args)


def test_Rect_offsetTo(rect):
    rect.offsetTo(0, 0)


def test_Rect_inset(rect):
    rect.inset(0, 0)


def test_Rect_outset(rect):
    rect.outset(0, 0)


@pytest.mark.parametrize('args', [
    (skia.Rect(0, 0, 250, 130),),
    (skia.Rect(0, 0, 240, 120), skia.Rect(0, 0, 250, 130)),
])
def test_Rect_intersect(rect, args):
    assert rect.intersect(*args)


def test_Rect_intersects(rect):
    rect.intersects(skia.Rect(20, 20))


def test_Rect_join(rect):
    rect.join(skia.Rect(10, 10, 20, 20))


def test_Rect_joinNonEmptyArg(rect):
    rect.joinNonEmptyArg(skia.Rect(10, 10, 20, 20))


def test_Rect_joinPossiblyEmptyRect(rect):
    rect.joinPossiblyEmptyRect(skia.Rect())


@pytest.mark.parametrize('operand', [
    (20, 10),
    skia.Rect(10, 10, 20, 20),
    skia.IRect(10, 10, 20, 20),
])
def test_Rect__contains(rect, operand):
    assert operand in rect


@pytest.mark.parametrize('args', [
    (20, 10),
    (skia.Rect(10, 10, 20, 20),),
    (skia.IRect(10, 10, 20, 20),),
])
def test_Rect_contains(rect, args):
    assert rect.contains(*args)


def test_Rect_round(rect):
    assert isinstance(rect.round(), skia.IRect)


def test_Rect_roundOut(rect):
    assert isinstance(rect.roundOut(), skia.IRect)


def test_Rect_roundIn(rect):
    assert isinstance(rect.roundIn(), skia.IRect)


def test_Rect_sort(rect):
    rect.sort()


def test_Rect_makeSorted(rect):
    assert isinstance(rect.makeSorted(), skia.Rect)


def test_Rect_asScalars(rect):
    view = rect.asScalars()
    assert isinstance(view, memoryview)


@pytest.mark.parametrize('args', [tuple(), (True,)])
def test_Rect_dumpHex(rect, args):
    rect.dump(*args)


def test_Rect_dumpHex(rect):
    rect.dumpHex()


def test_Rect_Intersects(rect):
    assert skia.Rect.Intersects(rect, skia.Rect(10, 10, 20, 20))


def test_Rect_attributes(rect):
    assert rect.fLeft == 0
    assert rect.fTop == 0
    assert rect.fRight == 240
    assert rect.fBottom == 120


def test_Rect_eq(rect):
    assert rect == skia.Rect(0, 0, 240, 120)


def test_Rect_ne(rect):
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
