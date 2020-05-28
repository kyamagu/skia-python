import skia
import pytest


@pytest.fixture
def shader():
    return skia.GradientShader.MakeLinear(
        [skia.Point(0, 0), skia.Point(1, 1)], [0xFFFF00FF, 0xFFFFFF00],
        [])


def test_Shader_isOpaque(shader):
    assert isinstance(shader.isOpaque(), bool)


@pytest.mark.parametrize('args', [
    (skia.Matrix(), [skia.TileMode.kClamp, skia.TileMode.kClamp]),
    tuple(),
])
def test_Shader_isAImage(shader, args):
    assert isinstance(shader.isAImage(*args), (bool, type(None), skia.Image))


def test_Shader_asAGradient(shader):
    info = skia.Shader.GradientInfo()
    assert isinstance(shader.asAGradient(info), skia.Shader.GradientType)


def test_Shader_makeWithLocalMatrix(shader):
    assert isinstance(shader.makeWithLocalMatrix(skia.Matrix()), skia.Shader)


def test_Shader_makeWithColorFilter(shader):
    assert isinstance(
        shader.makeWithColorFilter(skia.LumaColorFilter.Make()), skia.Shader)


def test_Shaders_Empty():
    assert isinstance(skia.Shaders.Empty(), skia.Shader)


@pytest.mark.parametrize('args', [
    (skia.ColorRED,),
    (skia.ColorRED, skia.ColorSpace.MakeSRGBLinear()),
])
def test_Shaders_Color(args):
    assert isinstance(skia.Shaders.Color(*args), skia.Shader)


def test_Shaders_Blend(shader):
    assert isinstance(
        skia.Shaders.Blend(skia.BlendMode.kSrc, shader, shader),
        skia.Shader)


def test_Shaders_Lerp(shader):
    assert isinstance(
        skia.Shaders.Lerp(0.5, shader, shader), skia.Shader)


@pytest.mark.parametrize('args', [
    ([skia.Point(0, 0), skia.Point(1, 1)], [0xFFFF00FF, 0xFFFFFF00]),
    ([skia.Point(0, 0), skia.Point(1, 1)], [0xFFFF00FF, 0xFFFFFF00], [0, 1],
        skia.TileMode.kClamp, 0, skia.Matrix()),
])
def test_GradientShader_MakeLinear(args):
    assert isinstance(skia.GradientShader.MakeLinear(*args), skia.Shader)


def test_GradientShader_MakeRadial():
    assert isinstance(skia.GradientShader.MakeRadial(
        skia.Point(10, 10), 5, [0xFFFF00FF, 0xFFFFFF00]), skia.Shader)


def test_GradientShader_MakeTwoPointConical():
    assert isinstance(skia.GradientShader.MakeTwoPointConical(
        skia.Point(0, 0), 0, skia.Point(1, 1), 90, [0xFFFF00FF, 0xFFFFFF00]),
    skia.Shader)


def test_GradientShader_MakeSweep():
    assert isinstance(skia.GradientShader.MakeSweep(
        10, 10, [0xFFFF00FF, 0xFFFFFF00]), skia.Shader)


def test_PerlinNoiseShader_MakeFractalNoise():
    assert isinstance(skia.PerlinNoiseShader.MakeFractalNoise(
        4, 4, 2, 0), skia.Shader)


def test_PerlinNoiseShader_MakeTurbulence():
    assert isinstance(skia.PerlinNoiseShader.MakeTurbulence(
        4, 4, 2, 0), skia.Shader)


def test_PerlinNoiseShader_MakeImprovedNoise():
    assert isinstance(skia.PerlinNoiseShader.MakeImprovedNoise(
        4, 4, 2, 1), skia.Shader)
