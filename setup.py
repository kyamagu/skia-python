from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import sys

__version__ = '0.0.1'


SOURCES = [
    'src/skia/main.cpp',
    'src/skia/AutoCanvasRestore.cpp',
    'src/skia/Bitmap.cpp',
    'src/skia/BlendMode.cpp',
    'src/skia/Canvas.cpp',
    'src/skia/Color.cpp',
    'src/skia/ColorSpace.cpp',
    'src/skia/Data.cpp',
    'src/skia/Font.cpp',
    'src/skia/GrContext.cpp',
    'src/skia/Image.cpp',
    'src/skia/ImageInfo.cpp',
    'src/skia/Matrix.cpp',
    'src/skia/Paint.cpp',
    'src/skia/Path.cpp',
    'src/skia/Picture.cpp',
    'src/skia/Pixmap.cpp',
    'src/skia/Point.cpp',
    'src/skia/Rect.cpp',
    'src/skia/Region.cpp',
    'src/skia/RRect.cpp',
    'src/skia/Size.cpp',
    'src/skia/Surface.cpp',
    'src/skia/TextBlob.cpp',
    'src/skia/TextBlobBuilder.cpp',
    'src/skia/Vertices.cpp',
]

SKIA_PATH = os.getenv('SKIA_PATH', 'skia')

if sys.platform == 'win32':
    DEFINE_MACROS = []  # doesn't work for cl.exe
    LIBRARIES = [
        'FontSub',
        'Ole32',
        'OleAut32',
        'User32',
        'Usp10',
        'OpenGL32',
        'Gdi32',
        # 'd3d12',
        # 'dxgi',
        # 'd3dcompiler',
    ]
    EXTRA_OBJECTS = [os.path.join(SKIA_PATH, 'out', 'Release', 'skia.lib')]
    EXTRA_COMPILE_ARGS = [
        '/std:c++latest',
        '/DVERSION_INFO=%s' % __version__,
        '/Zc:inline',
        '/GR',
        # Disable a bunch of warnings.
        '/wd5030',  # Warnings about unknown attributes.
        '/wd4244',  # Conversion from 'float' to 'int', possible loss of data.
        '/wd4267',  # Conversion from 'size_t' to 'int', possible loss of data.
        '/wd4800',  # Forcing value to bool 'true' or 'false'.
        '/wd4180',  # Qualifier applied to function type has no meaning.
        '/UNDEBUG',
    ]
    EXTRA_LINK_ARGS = [
        '/OPT:ICF',
        '/OPT:REF',
        '/DEBUG',
    ]
elif sys.platform == 'darwin':
    DEFINE_MACROS = [('VERSION_INFO', __version__)]
    LIBRARIES = [
        'dl',
    ]
    EXTRA_OBJECTS = [os.path.join(SKIA_PATH, 'out', 'Release', 'libskia.a')]
    EXTRA_COMPILE_ARGS = [
        '-std=c++14',
        '-stdlib=libc++',
        '-mmacosx-version-min=10.9',
        '-fvisibility=hidden',
    ]
    EXTRA_LINK_ARGS = [
        '-stdlib=libc++',
        '-mmacosx-version-min=10.9',
        '-dead_strip',
        '-framework', 'AppKit',
        '-framework', 'ApplicationServices',
        '-framework', 'OpenGL',
        #'-framework', 'Metal',
        #'-framework', 'MetalKit',
        #'-framework', 'Foundation',
    ]
else:
    DEFINE_MACROS = [('VERSION_INFO', __version__)]
    LIBRARIES = [
        'dl',
        'fontconfig',
        'freetype',
        'GL',
        # 'GLESv2',
    ]
    EXTRA_OBJECTS = [os.path.join(SKIA_PATH, 'out', 'Release', 'libskia.a')]
    EXTRA_COMPILE_ARGS = [
        '-std=c++14',
        '-fvisibility=hidden',
        '-Wno-attributes',
        '-fdata-sections',
        '-ffunction-sections',
    ]
    EXTRA_LINK_ARGS = [
        '-Wl,--gc-sections',
    ]


class get_pybind_include(object):
    """Helper class to determine the pybind11 include path

    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked. """

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    def build_extensions(self):
        if sys.platform == 'linux':
            try:
                self.compiler.compiler_so.remove('-Wstrict-prototypes')
            except (AttributeError, ValueError):
                pass
        build_ext.build_extensions(self)


extension = Extension(
    'skia',
    SOURCES,
    include_dirs=[
        # Path to pybind11 headers
        get_pybind_include(),
        get_pybind_include(user=True),
        SKIA_PATH,
        os.path.join(SKIA_PATH, 'out', 'Release', 'gen'),
    ],
    define_macros=DEFINE_MACROS,
    libraries=LIBRARIES,
    extra_objects=EXTRA_OBJECTS,
    extra_compile_args=EXTRA_COMPILE_ARGS,
    extra_link_args=EXTRA_LINK_ARGS,
    language='c++',
)


setup(
    name='skia',
    version=__version__,
    author='Kota Yamaguchi',
    author_email='KotaYamaguchi1984@gmail.com',
    url='https://github.com/kyamagu/skia-python',
    description='Skia python binding',
    long_description=open('README.md', 'r').read(),
    ext_modules=[extension],
    install_requires=['pybind11>=2.4'],
    setup_requires=['pybind11>=2.4'],
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
)
