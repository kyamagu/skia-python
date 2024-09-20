import skia
import pytest

def test_Unicodes_init0():
    if sys.platform.startswith("win"):
        pytest.skip("Known not to work; To be investigated.")
    assert isinstance(skia.Unicode(), skia.Unicode)

# recommended alias (upstream):
def test_Unicodes_init1():
    if sys.platform.startswith("win"):
        pytest.skip("Known not to work; To be investigated.")
    assert isinstance(skia.Unicodes.ICU.Make(), skia.Unicode)

# Canonical
def test_Unicodes_init2():
    if sys.platform.startswith("win"):
        pytest.skip("Known not to work; To be investigated.")
    assert isinstance(skia.Unicode.ICU_Make(), skia.Unicode)
