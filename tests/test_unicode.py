import skia
import pytest

def test_Unicodes_init0():
    assert isinstance(skia.Unicode(), skia.Unicode)

# recommended alias (upstream):
def test_Unicodes_init1():
    assert isinstance(skia.Unicodes.ICU.Make(), skia.Unicode)

# Canonical
def test_Unicodes_init2():
    assert isinstance(skia.Unicode.ICU_Make(), skia.Unicode)
