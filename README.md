# Skia python binding

![Build and test](https://github.com/kyamagu/skia-python/workflows/Build%20and%20test/badge.svg)
[![PyPI version](https://badge.fury.io/py/skia-python.svg)](https://badge.fury.io/py/skia-python)

Python binding to [Skia Graphics Library](https://skia.org/).

- Binding based on [pybind11](https://github.com/pybind/pybind11).

Currently, the binding is under active development.

## Install

Binary package is available on PyPI:

```bash
pip install skia-python
```

Supported platforms:

- Linux x86_64
- macOS
- Windows x86_64

For Linux platforms, there must be OpenGL and fontconfig installed:

```bash
apt-get install libfontconfig1 libgl1-mesa-glx libgl1-mesa-dri
```

Or:

```bash
yum install fontconfig mesa-libGL mesa-dri-drivers
```

For unsupported environment, check the [build instruction](https://kyamagu.github.io/skia-python/install.html).

## Examples

- [Showcase](https://github.com/kyamagu/skia-python/blob/master/notebooks/Showcase.ipynb)
- [Canvas Overview](https://github.com/kyamagu/skia-python/blob/master/notebooks/Canvas-Overview.ipynb)
- [Canvas Creation](https://github.com/kyamagu/skia-python/blob/master/notebooks/Canvas-Creation.ipynb)
- [Path Overview](https://github.com/kyamagu/skia-python/blob/master/notebooks/Path-Overview.ipynb)
- [Paint Overview](https://github.com/kyamagu/skia-python/blob/master/notebooks/Paint-Overview.ipynb)
- [Python Image I/O](https://github.com/kyamagu/skia-python/blob/master/notebooks/Python-Image-IO.ipynb)

## Documentation

https://kyamagu.github.io/skia-python

- [Tutorial](https://kyamagu.github.io/skia-python/tutorial/)
- [Reference](https://kyamagu.github.io/skia-python/reference/)

## Contributing

Feel free to [post an issue](https://github.com/kyamagu/skia-python/issues) or [PR](https://github.com/kyamagu/skia-python/pulls).
