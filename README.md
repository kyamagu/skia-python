# Skia python binding

![Build and test](https://github.com/kyamagu/skia-python/workflows/Build%20and%20test/badge.svg)
[![PyPI version](https://badge.fury.io/py/skia-python.svg)](https://badge.fury.io/py/skia-python)

Python binding to [Skia Graphics Library](https://skia.org/).

- Binding based on [pybind11](https://github.com/pybind/pybind11).

## Install

Binary package is available on PyPI:

```bash
pip install skia-python
```

Supported platforms: Python 3.8-3.13 (CPython) on

- Linux x86_64, aarch64
- macOS x86_64, arm64
- Windows x86_64, arm64 ; arm64 for the windows 11 and Python 3.11-3.13 combination only.

**IMPORTANT:** Starting v138rc1, `libEGL.so` (from mesa-libEGL, libglvnd, or your graphic
card's vendor e.g. NVidia) is required to be present on Linux hosts. This is associated
with the general change on Linux from X11 to Wayland, and from GTK3 to GTK4. Firefox started
to use it in 2021 (instead of the older `libGLX.so`), so it is likely newer Linux systems
already have it, but please check. skia-python v138.rc1+ supports hardware acceleration
via both GLX (still the default under X11) and EGL (newly added).

For Linux platforms, there must be OpenGL, libEGL and fontconfig installed:

```bash
apt-get install libfontconfig1 libgl1-mesa-glx libgl1-mesa-egl libegl1 libglvnd0 libgl1-mesa-dri
```

Or:

```bash
yum install fontconfig mesa-libGL mesa-libEGL libglvnd-egl mesa-dri-drivers
```

For unsupported environment, check the [build instruction](https://kyamagu.github.io/skia-python/install.html).

## Examples

- [Showcase](https://github.com/kyamagu/skia-python/blob/main/notebooks/Showcase.ipynb)
- [Canvas Overview](https://github.com/kyamagu/skia-python/blob/main/notebooks/Canvas-Overview.ipynb)
- [Canvas Creation](https://github.com/kyamagu/skia-python/blob/main/notebooks/Canvas-Creation.ipynb)
- [Path Overview](https://github.com/kyamagu/skia-python/blob/main/notebooks/Path-Overview.ipynb)
- [Paint Overview](https://github.com/kyamagu/skia-python/blob/main/notebooks/Paint-Overview.ipynb)
- [Python Image I/O](https://github.com/kyamagu/skia-python/blob/main/notebooks/Python-Image-IO.ipynb)
- [Drawing Texts](https://github.com/kyamagu/skia-python/blob/main/notebooks/Drawing-Texts.ipynb)

## Documentation

https://kyamagu.github.io/skia-python

- [Tutorial](https://kyamagu.github.io/skia-python/tutorial/)
- [Reference](https://kyamagu.github.io/skia-python/reference.html)

## Contributing

Feel free to [post an issue](https://github.com/kyamagu/skia-python/issues) or [PR](https://github.com/kyamagu/skia-python/pulls).
