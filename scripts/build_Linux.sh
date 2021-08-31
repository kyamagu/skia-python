#!/usr/bin/env bash

export PATH=${PWD}/depot_tools:$PATH

# Install system dependencies
yum install -y \
    fontconfig-devel \
    mesa-libGL-devel \
    xorg-x11-server-Xvfb \
    mesa-dri-drivers \
    ninja-build && \
    yum clean all && \
    rm -rf /var/cache/yum

# Build gn
export CC=gcc
export CXX=g++
export AR=ar
export CFLAGS="-Wno-deprecated-copy"
export LDFLAGS="-lrt"
git clone https://gn.googlesource.com/gn && \
    cd gn && \
    git checkout 981f46c64d1456d2083b1a2fa1367e753e0cdc1b && \
    python build/gen.py && \
    ninja -C out gn && \
    cd ..

# Build skia
cd skia && \
    patch -p1 < ../patch/git-sync-deps.patch && \
    python tools/git-sync-deps && \
    cp -f ../gn/out/gn bin/gn && \
    bin/gn gen out/Release --args='
is_official_build=true
skia_enable_tools=true
skia_use_system_libjpeg_turbo=false
skia_use_system_libwebp=false
skia_use_system_libpng=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
extra_cflags_cc=["-frtti"]
extra_ldflags=["-lrt"]
' && \
    ninja -C out/Release skia skia.h experimental_svg_model && \
    cd ..
