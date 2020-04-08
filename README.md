# Skia python binding
Python binding to [Skia Graphics Library](https://skia.org/).

- Binding based on [pybind11](https://github.com/pybind/pybind11).

## Installation

First, clone the repo.

```bash
git clone --recursive https://github.com/kyamagu/skia-python.git
cd skia-python
```

### Linux

Prerequisites:

- Python 2.7 (build time only)
- GLIBC >= 2.17
- fontconfig >= 2.10.93
- OpenGL


Install dependencies:

```bash
apt-get install -y libfontconfig1-dev libgl-dev libgl1-mesa-dri
```

or,

```bash
yum install -y fontconfig-devel mesa-libGL-devel mesa-dri-drivers
```

Set up `PATH` to the `depot_tools`. Note the build tools require relatively new
glibc and python 2.7.

```bash
export PATH="$PWD/depot_tools:$PATH"
```

Then, build skia library. At this point, `python` executable should be python 2.

```bash
cd skia
python2 tools/git-sync-deps
bin/gn gen out/Release --args='is_official_build=false is_debug=false extra_cflags_cc=["-frtti"] extra_ldflags=["-lrt"]'
ninja -C out/Release skia skia.h
cd ..
```

For detailed Skia build instructions, check [the official page](https://skia.org/).

Finally, build and install skia python binding. At this point, `python` should
be set to the desired version.

```bash
export SKIA_PATH=$PWD/skia
python -m pip install pybind11 numpy
python setup.py install
```

### macOS

Set up `PATH` to the `depot_tools`.

```bash
export PATH="$PWD/depot_tools:$PATH"
```

Then, build skia library. At this point, `python` executable should be python 2.

```bash
cd skia
python2 tools/git-sync-deps
bin/gn gen out/Release --args='is_official_build=false is_debug=false extra_cflags_cc=["-frtti"]'
ninja -C out/Release skia skia.h
cd ..
```

For detailed Skia build instructions, check [the official page](https://skia.org/).

Finally, build and install skia python binding. At this point, `python` should
be set to the desired version.

```bash
export SKIA_PATH=$PWD/skia
python -m pip install pybind11 numpy
python setup.py install
```

### Windows

Windows binary can be built using the generic steps above. However, Windows
support is experimental.


## Testing

```bash
python -m pip install pytest numpy glfw
python -m pytest tests
```

Alternatively, use `tox` to run tests under various python versions.

```bash
export SKIA_PATH=$PWD/skia
tox
```

## Documentation

Once skia-python is installed, sphinx documentation can be built:

```
python -m pip install sphinx
make -C docs html
```


## Contributing

Feel free to post an issue or PR.
