#!/usr/bin/env bash

export PATH=${PWD}/depot_tools:$PATH

EXTRA_CFLAGS=""

export CC=gcc
export CXX=g++
export AR=ar
export CFLAGS="-Wno-deprecated-copy"
export LDFLAGS="-lrt"

# Install system dependencies
if [[ $EUID -eq 0 ]]; then
    yum -y install epel-release && \
    yum install -y \
        python3 \
        ninja-build gn \
        fontconfig-devel \
        mesa-libGL-devel \
        xorg-x11-server-Xvfb \
        mesa-dri-drivers && \
        yum clean all && \
        mv depot_tools/ninja depot_tools/ninja.bak && \
        mv depot_tools/gn depot_tools/gn.bak && \
        rm -rf /var/cache/yum
fi

# Build skia
cd skia && \
    patch -p1 < ../patch/skia-m137-minimize-download.patch && \
    patch -p1 < ../patch/skia-m132-colrv1-freetype.diff && \
    python3 tools/git-sync-deps && \
    gn gen out/Release --args="
is_official_build=true
skia_enable_svg=true
skia_use_vulkan=true
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
