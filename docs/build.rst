Build instruction
=================

First, clone the repo.

.. code-block:: bash

    git clone --recursive https://github.com/kyamagu/skia-python.git
    cd skia-python

The repository bundles `skia` and its build tools (`depot_tools`) as submodules.
`skia` must be built first to build Python binding.

For building skia, there are a few compile-time requirements:

- Run-time type information (RTTI) must be enabled.

  - ``-frtti`` flag for ``clang`` or ``gcc``
  - ``/GR`` flag for ``cl.exe`` (Visual C++)

- ``/MD`` flag must be set for ``cl.exe`` (Visual C++)
- ``gn`` args should include ``skia_enable_tools = true`` to build ``skia.h`` header.
- Static linking is assumed; therefore, ``is_component_build = false``.

For detailed Skia build instructions, check `the official page`_.

.. _the official page: https://skia.org/

Linux
-----

Prerequisites:

- Python 2.7 (build time only)
- GLIBC >= 2.17
- fontconfig >= 2.10.93
- OpenGL


Install dependencies:

.. code-block:: bash

    apt-get install -y libfontconfig1-dev libgl-dev libgl1-mesa-dri

or:

.. code-block:: bash

    yum install -y fontconfig-devel mesa-libGL-devel mesa-dri-drivers


Set up ``PATH`` to the ``depot_tools``. build skia library. At this point,
``python`` executable should be python 2. Note the build tools require
relatively new glibc and python 2.7.

.. code-block:: bash

    export PATH="$PWD/depot_tools:$PATH"
    cd skia
    python2 tools/git-sync-deps
    bin/gn gen out/Release --args='is_official_build=true skia_enable_tools=true skia_use_system_libjpeg_turbo=false skia_use_system_libwebp=false skia_use_system_libpng=false skia_use_system_icu=false skia_use_system_harfbuzz=false extra_cflags_cc=["-frtti"] extra_ldflags=["-lrt"]'
    ninja -C out/Release skia skia.h
    cd ..

Then, build the skia python binding. At this point, ``python`` should be set to
the desired version.

.. code-block:: bash

    export SKIA_PATH=$PWD/skia
    export SKIA_OUT_PATH=$SKIA_PATH/out/Release
    python -m pip install pybind11 numpy wheel
    python setup.py bdist_wheel

macOS
-----

Prerequisites:

- Python 2.7 (build time only)
- Xcode Command Line Tools

Set up ``PATH`` to the ``depot_tools``, and build skia library. At this point,
``python`` executable should be python 2.

.. code-block:: bash

    export PATH="$PWD/depot_tools:$PATH"
    cd skia
    python2 tools/git-sync-deps
    bin/gn gen out/Release --args='is_official_build=true skia_enable_tools=true skia_use_system_libjpeg_turbo=false skia_use_system_libwebp=false skia_use_system_libpng=false skia_use_system_icu=false skia_use_system_harfbuzz=false extra_cflags_cc=["-frtti"]'
    ninja -C out/Release skia skia.h
    cd ..

Then, build the skia python binding. At this point, ``python`` should be set to
the desired version.

.. code-block:: bash

    export SKIA_PATH=$PWD/skia
    export SKIA_OUT_PATH=$SKIA_PATH/out/Release
    python -m pip install pybind11 numpy wheel
    python setup.py bdist_wheel

Windows
-------

Prerequisites:

- Python 2.7 (build time only)
- Visual C++ version that supports C++14

Windows binary can be built using the generic steps above.

.. code-block:: powershell

    $env:Path += ";$pwd\depot_tools"

    cd skia
    python2 tools\git-sync-deps
    bin\gn gen out\Release --args='is_official_build=true skia_enable_tools=true skia_use_system_libjpeg_turbo=false skia_use_system_libwebp=false skia_use_system_libpng=false skia_use_system_icu=false skia_use_system_harfbuzz=false skia_use_system_expat=false skia_use_system_zlib=false extra_cflags_cc=[\"/GR\", \"/EHsc\", \"/MD\"] target_cpu=\"x86_64\"'
    ninja -C out\Release skia skia.h
    cd ..

    python -m pip install pybind11 numpy wheel
    python setup.py bdist_wheel

Testing
-------

.. code-block:: bash

    python -m pip install pytest numpy glfw
    python -m pytest tests

Alternatively, use ``tox`` to run tests under various python versions.

.. code-block:: bash

    export SKIA_PATH=$PWD/skia
    export SKIA_OUT_PATH=$SKIA_PATH/out/Release
    tox


Building documentation
----------------------

Once skia-python is installed, sphinx documentation can be built:

.. code-block:: bash

    python -m pip install sphinx
    python setup.py build_sphinx
