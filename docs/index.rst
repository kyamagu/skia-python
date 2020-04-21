skia-python
===========

`skia-python` is a Python binding to `Skia Graphics Library`_.

Skia is an open source 2D graphics library which provides common APIs that work
across a variety of hardware and software platforms.

`skia-python` is developed at `Github`_.

.. _Skia Graphics Library: https://skia.org/

.. _Github: https://github.com/kyamagu/skia-python

Install
-------

Binary package is available on PyPI:

.. code-block:: bash

    pip install skia-python

Supported platforms:

- Linux x86_64
- macOS
- Windows x86_64

For Linux platforms, there must be OpenGL and fontconfig installed:

.. code-block:: bash

    apt-get install libfontconfig1 libgl1-mesa-glx libgl1-mesa-dri

Or:

.. code-block:: bash

    yum install fontconfig mesa-libGL mesa-dri-drivers


Documentation
-------------

.. toctree::
   :maxdepth: 2

   build
   usage
   reference

Indices and tables
------------------

* :ref:`genindex`
* :ref:`search`
