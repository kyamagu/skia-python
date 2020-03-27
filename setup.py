from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import sys
import setuptools

__version__ = '0.0.1'


SRC = [
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


skia_path = os.getenv('SKIA_PATH', 'skia')
if sys.platform.startswith('win'):
    EXTRA_OBJECTS = [
        os.path.join(skia_path, 'out', 'Release', 'skia.lib'),
        'user32.lib',
    ]
else:
    EXTRA_OBJECTS = [os.path.join(skia_path, 'out', 'Release', 'libskia.a')]


ext_modules = [
    Extension(
        'skia',
        SRC,
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            skia_path,
            os.path.join(skia_path, 'out', 'Release', 'gen'),
        ],
        extra_objects=EXTRA_OBJECTS,
        language='c++'
    ),
]


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/std:c++latest'],
        'unix': ['-std=c++14', '-fvisibility=hidden'],
    }
    l_opts = {
        'msvc': [],
        'unix': [],
    }

    if sys.platform == 'darwin':
        darwin_opts = [
            '-stdlib=libc++',
            '-mmacosx-version-min=10.9',
        ]
        c_opts['unix'] += darwin_opts
        l_opts['unix'] += darwin_opts
        l_opts['unix'] += [
            '-framework', 'CoreServices',
            '-framework', 'CoreGraphics',
            '-framework', 'CoreText',
            '-framework', 'CoreFoundation'
        ]
    elif sys.platform == 'linux':
        c_opts['unix'] += ['-Wno-attributes',]
        l_opts['unix'] += ['-lfontconfig', '-lfreetype']

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        link_opts = self.l_opts.get(ct, [])
        if ct == 'unix':
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
        elif ct == 'msvc':
            opts.append('/DVERSION_INFO=\\"%s\\"' % self.distribution.get_version())

        if sys.platform == 'linux':
            try:
                self.compiler.compiler_so.remove('-Wstrict-prototypes')
            except (AttributeError, ValueError):
                pass

        for ext in self.extensions:
            ext.extra_compile_args = opts
            ext.extra_link_args = link_opts
        build_ext.build_extensions(self)


setup(
    name='skia',
    version=__version__,
    author='Kota Yamaguchi',
    author_email='KotaYamaguchi1984@gmail.com',
    url='https://github.com/kyamagu/skia-python',
    description='Skia python binding',
    long_description=open('README.md', 'r').read(),
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.4'],
    setup_requires=['pybind11>=2.4'],
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
)
