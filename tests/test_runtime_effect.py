import skia
import pytest

@pytest.fixture(scope='session')
def runtime_effect():
    return skia.RuntimeEffect.MakeForShader("half4 main(float2 coord){return half4(0,0,0,0);}")

def test_RuntimeEffect_init0(runtime_effect):
    assert isinstance(runtime_effect, skia.RuntimeEffect)


def test_V3_init0():
    assert isinstance(skia.V3(0,0,0), skia.V3)

def test_V4_init0():
    assert isinstance(skia.V4(0,0,0,0), skia.V4)


@pytest.fixture(scope='session')
def runtime_effect_builder(runtime_effect):
    return skia.RuntimeEffectBuilder(runtime_effect)

def test_RuntimeEffectBuilder_init0(runtime_effect_builder):
    assert isinstance(runtime_effect_builder, skia.RuntimeEffectBuilder)

# setUniform is polymorphic, so we are just testing that the
# accepted object types do not throw errors.
@pytest.mark.parametrize('arg', [
    (skia.V3(0,0,0)),
    (skia.V4(0,0,0,0)),
])
def test_RuntimeEffectBuilder_setUniform(runtime_effect_builder, arg):
    runtime_effect_builder.setUniform("bogus", arg)


def test_RuntimeEffect_make_type1(runtime_effect):
    assert isinstance(runtime_effect.makeShader(None), skia.Shader)

@pytest.mark.xfail(reason="Current SkSL snipplet returns None as ColorFilter; TODO: Find useful SkSL snipplet for construction")
def test_RuntimeEffect_make_type2(runtime_effect):
    assert isinstance(runtime_effect.makeColorFilter(None), skia.ColorFilter)

@pytest.mark.xfail(reason="We don't bind SkBlender yet. TODO")
def test_RuntimeEffect_make_type3(runtime_effect):
    assert isinstance(runtime_effect.makeBlender(None), skia.Blender)


# These test implicit conversion to RuntimeEffectChildPtr
def test_RuntimeEffectBuilder_setChild1(runtime_effect_builder, runtime_effect):
    runtime_effect_builder.setChild("bogus", runtime_effect.makeShader(None))

def test_RuntimeEffectBuilder_setChild2(runtime_effect_builder, runtime_effect):
    runtime_effect_builder.setChild("bogus", runtime_effect.makeColorFilter(None))

@pytest.mark.xfail(reason="We don't bind SkBlender yet. TODO")
def test_RuntimeEffectBuilder_setChild3(runtime_effect_builder, runtime_effect):
    runtime_effect_builder.setChild("bogus", runtime_effect.makeBlender(None))


@pytest.mark.xfail(reason="We throw on invalid struct and don't use the whole struct anymore / at the moment.")
def test_RuntimeEffectResult_init0():
    assert isinstance(skia.RuntimeEffectResult(), skia.RuntimeEffectResult)

def test_RuntimeEffectChildPtr_init0():
    assert isinstance(skia.RuntimeEffectChildPtr(), skia.RuntimeEffectChildPtr)

def test_VectorSkRuntimeEffectChildPtr_init0():
    assert isinstance(skia.VectorSkRuntimeEffectChildPtr(), skia.VectorSkRuntimeEffectChildPtr)

def test_SpanRuntimeEffectChildPtr_init0():
    assert isinstance(skia.SpanRuntimeEffectChildPtr(), skia.SpanRuntimeEffectChildPtr)

def test_RuntimeEffectBuilderUniform_init0():
    assert isinstance(skia.RuntimeEffectBuilderUniform(), skia.RuntimeEffectBuilderUniform)

def test_RuntimeEffectBuilderChild_init0():
    assert isinstance(skia.RuntimeEffectBuilderChild(), skia.RuntimeEffectBuilderChild)
