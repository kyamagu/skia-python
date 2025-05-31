import skia
import pytest

@pytest.fixture(scope='session')
def runtime_effect():
    return skia.RuntimeEffect.MakeForShader("vec4 main(vec2 inCoords){return vec4(0,0,0,0);}")

@pytest.fixture(scope='session')
def runtime_effect_color():
    return skia.RuntimeEffect.MakeForColorFilter("vec4 main(vec4 inColor){return vec4(0,0,0,0);}")

@pytest.fixture(scope='session')
def runtime_effect_blender():
    return skia.RuntimeEffect.MakeForBlender("vec4 main(vec4 srcColor, vec4 dstColor){return vec4(0,0,0,0);}")

def test_RuntimeEffect_init0(runtime_effect):
    assert isinstance(runtime_effect, skia.RuntimeEffect)

def test_RuntimeEffect_init1(runtime_effect_color):
    assert isinstance(runtime_effect_color, skia.RuntimeEffect)

def test_RuntimeEffect_init2(runtime_effect_blender):
    assert isinstance(runtime_effect_blender, skia.RuntimeEffect)

def test_V2_init0():
    assert isinstance(skia.V2(0,0), skia.V2)

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
    (0),
    (0.0),
    (skia.V2(0,0)),
    (skia.V3(0,0,0)),
    (skia.V4(0,0,0,0)),
    ([0,0]),
    ([0,0,0]),
    ([0,0,0,0]),
    ((0,0)),
    ((0,0,0)),
    ((0,0,0,0)),
])
def test_RuntimeEffectBuilder_setUniform(runtime_effect_builder, arg):
    runtime_effect_builder.setUniform("bogus", arg)


def test_RuntimeEffect_make_type1(runtime_effect):
    assert isinstance(runtime_effect.makeShader(None), skia.Shader)

def test_RuntimeEffect_make_type2(runtime_effect_color):
    assert isinstance(runtime_effect_color.makeColorFilter(None), skia.ColorFilter)

@pytest.mark.xfail(reason="We don't bind SkBlender yet. TODO")
def test_RuntimeEffect_make_type3(runtime_effect_blender):
    assert isinstance(runtime_effect_blender.makeBlender(None), skia.Blender)


# These test implicit conversion to RuntimeEffectChildPtr
def test_RuntimeEffectBuilder_setChild1(runtime_effect_builder, runtime_effect):
    runtime_effect_builder.setChild("bogus", runtime_effect.makeShader(None))

def test_RuntimeEffectBuilder_setChild2(runtime_effect_builder, runtime_effect_color):
    runtime_effect_builder.setChild("bogus", runtime_effect_color.makeColorFilter(None))

@pytest.mark.xfail(reason="We don't bind SkBlender yet. TODO")
def test_RuntimeEffectBuilder_setChild3(runtime_effect_builder, runtime_effect_blender):
    runtime_effect_builder.setChild("bogus", runtime_effect_blender.makeBlender(None))


@pytest.mark.xfail(reason="We throw on invalid struct and don't use the whole struct anymore / at the moment.")
def test_RuntimeEffectResult_init0():
    assert isinstance(skia.RuntimeEffectResult(), skia.RuntimeEffectResult)

def test_RuntimeEffectChildPtr_init0():
    assert isinstance(skia.RuntimeEffectChildPtr(), skia.RuntimeEffectChildPtr)

def test_VectorRuntimeEffectChildPtr_init0():
    assert isinstance(skia.VectorRuntimeEffectChildPtr(), skia.VectorRuntimeEffectChildPtr)

def test_SpanRuntimeEffectChildPtr_init0():
    assert isinstance(skia.SpanRuntimeEffectChildPtr(), skia.SpanRuntimeEffectChildPtr)

def test_RuntimeEffectBuilderUniform_init0():
    assert isinstance(skia.RuntimeEffectBuilderUniform(), skia.RuntimeEffectBuilderUniform)

def test_RuntimeEffectBuilderChild_init0():
    assert isinstance(skia.RuntimeEffectBuilderChild(), skia.RuntimeEffectBuilderChild)


@pytest.fixture(scope='session')
def builder_with_uniforms():
    effect = skia.RuntimeEffect.MakeForShader("""
uniform int    iInteger;
uniform float  iFloat;
uniform float2 iFloat2;
uniform float3 iFloat3;
uniform float4 iFloat4;
uniform shader iImage;
vec4 main(vec2 inCoords){return vec4(0,0,0,0);}""")
    return skia.RuntimeShaderBuilder(effect)

def test_builder_uniform_count(builder_with_uniforms):
    assert len(builder_with_uniforms.effect().uniforms()) == 5

def test_builder_children_count(builder_with_uniforms):
    assert len(builder_with_uniforms.children()) == 1

def test_builder_effect_children_count(builder_with_uniforms):
    assert len(builder_with_uniforms.effect().children()) == 1

def test_builder_effect_children_name(builder_with_uniforms):
    assert builder_with_uniforms.effect().children()[0].name == "iImage"

def test_builder_nonexistent_uniform(builder_with_uniforms):
    assert builder_with_uniforms.uniform("iDoesNotExist").type == None

#Actually blindly setting non-existent uniform does not fail.
#But we also test coercion here.
def test_builder_uniform_int_set(builder_with_uniforms):
    assert builder_with_uniforms.uniform("iInteger").type == skia.RuntimeEffect.UniformType.kInt
    builder_with_uniforms.setUniform("iInteger", 0)

def test_builder_uniform_float_set(builder_with_uniforms):
    assert builder_with_uniforms.uniform("iFloat").type == skia.RuntimeEffect.UniformType.kFloat
    builder_with_uniforms.setUniform("iFloat", 0.0)

def test_builder_uniform_float2_set(builder_with_uniforms):
    assert builder_with_uniforms.uniform("iFloat2").type == skia.RuntimeEffect.UniformType.kFloat2
    builder_with_uniforms.setUniform("iFloat", (0.0, 0.0))

def test_builder_uniform_float3_set(builder_with_uniforms):
    assert builder_with_uniforms.uniform("iFloat3").type == skia.RuntimeEffect.UniformType.kFloat3
    builder_with_uniforms.setUniform("iFloat", (0.0, 0.0, 0.0))

def test_builder_uniform_float4_set(builder_with_uniforms):
    assert builder_with_uniforms.uniform("iFloat4").type == skia.RuntimeEffect.UniformType.kFloat4
    builder_with_uniforms.setUniform("iFloat", (0.0, 0.0, 0.0, 0.0))

def test_builder_uniform_shader_set(builder_with_uniforms):
    assert builder_with_uniforms.child("iImage").type == skia.RuntimeEffect.ChildType.kShader
    builder_with_uniforms.setChild("iImage", skia.Bitmap().makeShader())
