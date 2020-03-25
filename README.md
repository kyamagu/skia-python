# Skia python binding
Python binding to [Skia Graphics Library](https://skia.org/).

- Binding based on [pybind11](https://github.com/pybind/pybind11).

## Build

First, clone the repo.

```bash
git clone --recursive https://github.com/kyamagu/skia-python.git
cd skia-python
```

### manylinux2010

This is a CentOS 6 environment. Other linux distributions do not need some of
the following steps.

Set up `PATH` to the `depot_tools`. Note the build tools require relatively new
glibc and python 2.7.

```bash
export PATH="$PWD/depot_tools:$PATH"
```

Install compatible `gn` binary.

```bash
export CC=gcc
export CXX=g++
export AR=gcc
export LDFLAGS=-lrt

git clone https://gn.googlesource.com/gn
cd gn
python build/gen.py
ninja -C out
cp -f out/gn ../skia/bin/gn
cd ..
```

Install `libfreetype2` and `fontcofig` that works with old glibc.

```bash
yum install freetype-devel expat-devel
curl -L -O https://www.freedesktop.org/software/fontconfig/release/fontconfig-2.10.93.tar.bz2
tar xjf fontconfig-2.10.93.tar.bz2
cd fontconfig-2.10.93
./configure --prefix=/usr \
            --sysconfdir=/etc \
            --localstatedir=/var \
            --docdir=/usr/share/doc/fontconfig-2.10.93 \
            --disable-docs \
            --disable-static &&
make install
cd ..
```

Then, build skia library.

```bash
cd skia
bin/gn gen out/Release --args='is_official_build=false is_debug=false extra_cflags_cc=["-frtti"] extra_ldflags=["-lrt"]'
ninja -C out/Release skia skia.h
cd ..
```

Finally, build skia python binding.

```bash
python setup.py build
```

### macOS

Set up `PATH` to the `depot_tools`.

```bash
export PATH="$PWD/depot_tools:$PATH"
```

Then, build skia library.

```bash
cd skia
bin/gn gen out/Release --args='is_official_build=false is_debug=false extra_cflags_cc=["-frtti"]'
ninja -C out/Release skia skia.h
cd ..
```

Finally, build skia python binding.

```bash
python setup.py build
```
