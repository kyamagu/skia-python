from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import sys
import glob

try:
    from numpy.distutils.ccompiler import CCompiler_compile
    import distutils.ccompiler
    distutils.ccompiler.CCompiler.compile = CCompiler_compile
except ImportError:
    pass

NAME = 'skia-python'
__version__ = '139.0'

SKIA_PATH = os.getenv('SKIA_PATH', 'skia')
SKIA_OUT_PATH = os.getenv(
    'SKIA_OUT_PATH', os.path.join(SKIA_PATH, 'out', 'Release')
)

data_files = []
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
        'Advapi32',
    ]
    EXTRA_OBJECTS = list(
    ) + [os.path.join(SKIA_OUT_PATH, 'svg.lib'), os.path.join(SKIA_OUT_PATH, 'skresources.lib'), os.path.join(SKIA_OUT_PATH, 'skia.lib'),
         os.path.join(SKIA_OUT_PATH, 'skparagraph.lib'), os.path.join(SKIA_OUT_PATH, 'skshaper.lib'),
         os.path.join(SKIA_OUT_PATH, 'skunicode_icu.lib'), os.path.join(SKIA_OUT_PATH, 'skunicode_core.lib'),
         os.path.join(SKIA_OUT_PATH, 'pathops.lib')]
    EXTRA_COMPILE_ARGS = [
        '/std:c++17',  # c++20 fails.
        '/DVERSION_INFO=%s' % __version__,
        '/DSK_GL',
        '/DSK_VULKAN',
        '/DSK_GANESH=1',
        '/Zc:inline',
        # Disable a bunch of warnings.
        '/wd5030',  # Warnings about unknown attributes.
        '/wd4244',  # Conversion from 'float' to 'int', possible loss of data.
        '/wd4267',  # Conversion from 'size_t' to 'int', possible loss of data.
        '/wd4800',  # Forcing value to bool 'true' or 'false'.
        '/wd4180',  # Qualifier applied to function type has no meaning.
        '/MD',  # Bugfix: https://bugs.python.org/issue38597
    ]
    EXTRA_LINK_ARGS = [
        '/OPT:ICF',
        '/OPT:REF',
    ]
    data_files = [('Lib/site-packages', [os.path.join(SKIA_OUT_PATH, 'icudtl.dat')])]
elif sys.platform == 'darwin':
    DEFINE_MACROS = [
        ('VERSION_INFO', __version__),
        ('SK_GL', ''),
        ('SK_GANESH', '1'),
        ('SK_METAL', ''),
    ]
    LIBRARIES = [
        'dl',
    ]
    EXTRA_OBJECTS = list(
    ) + [os.path.join(SKIA_OUT_PATH, 'libsvg.a'), os.path.join(SKIA_OUT_PATH, 'libskia.a'),
         os.path.join(SKIA_OUT_PATH, 'libskparagraph.a'), os.path.join(SKIA_OUT_PATH, 'libskshaper.a'),
         os.path.join(SKIA_OUT_PATH, 'libskunicode_icu.a'), os.path.join(SKIA_OUT_PATH, 'libskunicode_core.a'),
         os.path.join(SKIA_OUT_PATH, 'libpathops.a')]
    EXTRA_COMPILE_ARGS = [
        '-std=c++17',
        '-stdlib=libc++',
        '-mmacosx-version-min=11.0',
        '-fvisibility=hidden',
    ]
    EXTRA_LINK_ARGS = [
        '-stdlib=libc++',
        '-mmacosx-version-min=11.0',
        '-dead_strip',
        '-framework',
        'AppKit',
        '-framework',
        'ApplicationServices',
        '-framework',
        'OpenGL',
    ]
else:
    DEFINE_MACROS = [
        ('VERSION_INFO', __version__),
        ('SK_GL', ''),
        ('SK_VULKAN', ''),
        ('SK_GANESH', '1'),
    ]
    LIBRARIES = [
        'dl',
        'fontconfig',
        'EGL',
        'GL',
        'expat',
    ]
    EXTRA_OBJECTS = list(
    ) + [os.path.join(SKIA_OUT_PATH, 'libsvg.a'), os.path.join(SKIA_OUT_PATH, 'libskresources.a'), os.path.join(SKIA_OUT_PATH, 'libskia.a'),
         os.path.join(SKIA_OUT_PATH, 'libskparagraph.a'), os.path.join(SKIA_OUT_PATH, 'libskshaper.a'),
         os.path.join(SKIA_OUT_PATH, 'libskunicode_icu.a'), os.path.join(SKIA_OUT_PATH, 'libskunicode_core.a'),
         os.path.join(SKIA_OUT_PATH, 'libpathops.a')]
    EXTRA_COMPILE_ARGS = [
        '-std=c++17',
        '-fvisibility=hidden',
        '-Wno-attributes',
        '-fdata-sections',
        '-ffunction-sections',
    ]
    EXTRA_LINK_ARGS = [
        '-Wl,--gc-sections',
        '-s',
        '-O3',
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
    sources=list(glob.glob(os.path.join('src', 'skia', '*.cpp'))),
    include_dirs=[
        # Path to pybind11 headers
        get_pybind_include(),
        get_pybind_include(user=True),
        SKIA_PATH,
        os.path.join(SKIA_PATH, "third_party/externals/freetype/include"),
        os.path.join(SKIA_PATH, "third_party/externals/vulkan-headers/include"),
        os.path.join(SKIA_PATH, "include/third_party/vulkan"),
        os.path.join(SKIA_OUT_PATH, 'gen'),
    ],
    define_macros=DEFINE_MACROS,
    libraries=LIBRARIES,
    extra_objects=EXTRA_OBJECTS,
    extra_compile_args=EXTRA_COMPILE_ARGS,
    extra_link_args=EXTRA_LINK_ARGS,
    depends=[os.path.join('src', 'skia', 'common.h')],
    language='c++',
)

setup(
    name=NAME,
    version=__version__,
    author='Kota Yamaguchi',
    author_email='KotaYamaguchi1984@gmail.com',
    url='https://github.com/kyamagu/skia-python',
    description='Skia python binding',
    long_description=open('README.md', 'r').read(),
    long_description_content_type='text/markdown',
    ext_modules=[extension],
    data_files=data_files,
    install_requires=[
        'numpy',
        'pybind11>=2.6'
    ],
    setup_requires=['pybind11>=2.6'],
    cmdclass={'build_ext': BuildExt},
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Intended Audience :: End Users/Desktop',
        'License :: OSI Approved :: BSD License',
        'Operating System :: MacOS :: MacOS X',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: POSIX',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3.11',
        'Programming Language :: Python :: 3.12',
        'Programming Language :: Python :: 3.13',
        'Topic :: Multimedia :: Graphics',
        'Topic :: Scientific/Engineering :: Visualization',
    ],
    zip_safe=False,
)
