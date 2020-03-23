from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools

__version__ = '0.0.1'


SRC = [
    'src/skia/main.cpp',
    'src/skia/AutoCanvasRestore.cpp',
    'src/skia/Canvas.cpp',
    'src/skia/Data.cpp',
    'src/skia/Image.cpp',
    'src/skia/ImageInfo.cpp',
    'src/skia/IPoint.cpp',
    'src/skia/IRect.cpp',
    'src/skia/ISize.cpp',
    'src/skia/Surface.cpp',
    'src/skia/Paint.cpp',
    'src/skia/Path.cpp',
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


ext_modules = [
    Extension(
        'skia',
        SRC,
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True)
        ],
        language='c++'
    ),
]


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': ['-Iskia', '-Iskia/out/macos/gen'],
    }
    l_opts = {
        'msvc': [''],
        'unix': ['skia/out/macos/libskia.a'],
    }

    if sys.platform == 'darwin':
        darwin_opts = [
            '-stdlib=libc++',
            '-mmacosx-version-min=10.14',
        ]
        c_opts['unix'] += darwin_opts
        l_opts['unix'] += darwin_opts
        l_opts['unix'] += [
            '-framework', 'CoreServices',
            '-framework', 'CoreGraphics',
            '-framework', 'CoreText',
            '-framework', 'CoreFoundation'
        ]

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        link_opts = self.l_opts.get(ct, [])
        if ct == 'unix':
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
            opts.append('-std=c++17')
            opts.append('-fvisibility=hidden')
        elif ct == 'msvc':
            opts.append('/DVERSION_INFO=\\"%s\\"' % self.distribution.get_version())
        for ext in self.extensions:
            ext.extra_compile_args = opts
            ext.extra_link_args = link_opts
        build_ext.build_extensions(self)


setup(
    name='skia-python',
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
