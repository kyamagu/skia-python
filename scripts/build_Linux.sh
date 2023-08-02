#!/usr/bin/env bash

export PATH=${PWD}/depot_tools:$PATH

EXTRA_CFLAGS=""

if [[ $(uname -m) == "aarch64" ]]; then
    # Install ninja for aarch64
    yum -y install epel-release && \
        yum repolist && \
        yum install -y ninja-build && \
        ln -s ninja-build /usr/bin/ninja &&
        mv depot_tools/ninja depot_tools/ninja.bak
fi

# Install system dependencies
if [[ $EUID -eq 0 ]]; then
    yum install -y \
        python3 \
        fontconfig-devel \
        mesa-libGL-devel \
        xorg-x11-server-Xvfb \
        mesa-dri-drivers && \
        yum clean all && \
        rm -rf /var/cache/yum
fi

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
    python build/gen.py && \
    ninja -C out gn && \
    cd ..

# Build skia
cd skia && \
    patch -p1 < ../patch/skia-m116-minimize-download.patch && \
    python3 tools/git-sync-deps && \
    cp -f ../gn/out/gn bin/gn && \
    bin/gn gen out/Release --args="
is_official_build=true
skia_enable_svg=true
skia_use_system_libjpeg_turbo=false
skia_use_system_libwebp=false
skia_use_system_libpng=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
skia_use_system_freetype2=false
extra_cflags_cc=[\"-frtti\"]
extra_ldflags=[\"-lrt\"]
" && \
    ninja -C out/Release && \
    cd ..
