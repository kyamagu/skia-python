#!/usr/bin/env bash

export PATH=${PWD}/depot_tools:$PATH

EXTRA_CFLAGS=""

if [[ $(uname -m) == "aarch64" ]]; then
    # Install ninja for aarch64
    yum -y install epel-release && \
        yum repolist && \
        yum install -y ninja-build && \
        mv depot_tools/ninja depot_tools/ninja.bak
fi

# Install system dependencies
yum install -y \
    fontconfig-devel \
    mesa-libGL-devel \
    xorg-x11-server-Xvfb \
    mesa-dri-drivers && \
    yum clean all && \
    rm -rf /var/cache/yum

# EL8 anomaly: EL7 is python 2 and EL9 is python 3
[[ -f /usr/bin/python ]] || ln -s /usr/bin/python3 /usr/bin/python

if [[ $(uname -m) == "aarch64" ]] && [[ $CI_SKIP_BUILD == "true" ]]; then
    # gn and skia already built in a previous job
    exit 0
fi

# Build gn
export CC=gcc
export CXX=g++
export AR=ar
export CFLAGS="-Wno-deprecated-copy"
export LDFLAGS="-lrt"
git clone https://gn.googlesource.com/gn && \
    cd gn && \
    git checkout 981f46c64d1456d2083b1a2fa1367e753e0cdc1b && \
    python3 build/gen.py && \
    ninja -C out gn && \
    cd ..

# Build skia
cd skia && \
    patch -p1 < ../patch/git-sync-deps.patch && \
    python tools/git-sync-deps && \
    patch -p1 < ../patch/make_data_assembly.patch && \
    patch -p1 < ../patch/libjpeg-arm.patch && \
    patch -p1 < ../patch/skia-m87-c++-code.diff && \
    cp -f ../gn/out/gn bin/gn && \
    bin/gn gen out/Release --args="
is_official_build=true
skia_enable_tools=true
skia_use_system_libjpeg_turbo=false
skia_use_system_libwebp=false
skia_use_system_libpng=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
extra_cflags_cc=[\"-frtti\"]
extra_ldflags=[\"-lrt\"]
" && \
    ninja -C out/Release skia skia.h experimental_svg_model && \
    cd ..
