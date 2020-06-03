import skia
import pytest


@pytest.fixture
def color4f():
    return skia.Color4f.kRed;


@pytest.mark.parametrize('args', [
    (0xFFFFFFFF,),
    (1., 1., 1.,),
    ((1., 1., 1.),),
    (1., 1., 1., 1.,),
    ((1., 1., 1., 1.),),
])
def test_Color4f_init(args):
    assert isinstance(skia.Color4f(*args), skia.Color4f)


def test_Color4f_eq(color4f):
    assert color4f == color4f


def test_Color4f_ne(color4f):
    assert color4f != skia.Color4f.kBlue


@pytest.mark.parametrize('operand', [1.0, skia.Color4f.kBlue])
def test_Color4f_mul(color4f, operand):
    assert isinstance(color4f * operand, skia.Color4f)


def test_Color4f_vec(color4f):
    assert isinstance(color4f.vec(), memoryview)


def test_Color4f_getitem_setitem():
    color4f = skia.Color4f(0xFFFFFFFF)
    assert isinstance(color4f[0], float)
    color4f[0] = 1.0


def test_Color4f_isOpaque(color4f):
    assert isinstance(color4f.isOpaque(), bool)


def test_Color4f_fitsInBytes(color4f):
    assert isinstance(color4f.fitsInBytes(), bool)


def test_Color4f_toColor(color4f):
    assert isinstance(color4f.toColor(), int)
    assert isinstance(int(color4f), int)


def test_Color4f_repr(color4f):
    assert isinstance(repr(color4f), str)


def test_Color4f_toBytes_RGBA(color4f):
    assert isinstance(color4f.toBytes_RGBA(), int)


def test_Color4f_makeOpaque(color4f):
    assert isinstance(color4f.makeOpaque(), skia.Color4f)


def test_Color4f_FromColor():
    assert isinstance(skia.Color4f.FromColor(0xFF000000), skia.Color4f)


def test_Color4f_FromBytes_RGBA():
    assert isinstance(skia.Color4f.FromBytes_RGBA(0x000000FF), skia.Color4f)


@pytest.mark.parametrize('args', [
    (255, 255, 255,),
    (255, 255, 255, 255,),
])
def test_Color(args):
    assert isinstance(skia.Color(*args), int)


def test_ColorSetRGB():
    assert isinstance(skia.ColorSetRGB(255, 255, 255), int)


def test_ColorGetA():
    assert skia.ColorGetA(0xFFFFFFFF) == 0xFF


def test_ColorGetR():
    assert skia.ColorGetR(0xFFFFFFFF) == 0xFF


def test_ColorGetG():
    assert skia.ColorGetG(0xFFFFFFFF) == 0xFF


def test_ColorGetB():
    assert skia.ColorGetB(0xFFFFFFFF) == 0xFF


def test_ColorSetARGB():
    assert isinstance(skia.ColorSetARGB(255, 255, 255, 255), int)


def test_ColorSetA():
    assert isinstance(skia.ColorSetA(0xFFFFFFFF, 0), int)


def test_RGBToHSV():
    assert isinstance(skia.RGBToHSV(255, 255, 0), list)


def test_ColorToHSV():
    assert isinstance(skia.ColorToHSV(0xFFFFFF00), list)


def test_HSVToColor():
    assert isinstance(skia.HSVToColor([0., 1.0, 0.5]), int)


def test_PreMultiplyARGB():
    assert isinstance(skia.PreMultiplyARGB(255, 255, 255, 255), int)


def test_PreMultiplyColor():
    assert isinstance(skia.PreMultiplyColor(0xFFFFFFFF), int)
