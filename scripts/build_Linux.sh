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
    yum install -y \
        python3 \
        fontconfig-devel \
        mesa-libGL-devel \
        xorg-x11-server-Xvfb \
        mesa-dri-drivers && \
        yum clean all && \
        rm -rf /var/cache/yum
fi

# Wheel-building needs fontconfig-devel from above.
# Simply quit, if it looks like a previous run was successful:
if [[ -f "skia/out/Release/libskia.a" ]] ; then
    exit 0
fi

if [[ $(uname -m) == "aarch64" ]]; then
    # Install ninja for aarch64
    yum -y install epel-release && \
        yum repolist && \
        yum install -y ninja-build && \
        ln -s ninja-build /usr/bin/ninja &&
        mv depot_tools/ninja depot_tools/ninja.bak
fi

# libicu.a is the largest 3rd-party; if it already exists, we run ninja
# a 2nd time and exit.
# Running ninja a 2nd-time is safe - it is no-ops if skia is already built too.
# The 3rd-party libraries below are built in size-order; we built libicu last to signal
# having built most of them.
if [[ -f "skia/out/Release/libicu.a" ]] ; then
    cd skia && \
        ninja -C out/Release && \
        cd ..
    exit $?
fi
### 2nd round<->1st round ###

# Build gn
git clone https://gn.googlesource.com/gn && \
    cd gn && \
    git checkout fe330c0ae1ec29db30b6f830e50771a335e071fb && \
    python build/gen.py && \
    ninja -C out gn && \
    cd ..

# Build skia
cd skia && \
    patch -p1 < ../patch/skia-m120-minimize-download.patch && \
    patch -p1 < ../patch/skia-m116-colrv1-freetype.diff && \
    python3 tools/git-sync-deps && \
    cp -f ../gn/out/gn bin/gn && \
    bin/gn gen out/Release --args="
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
    ninja -C out/Release \
          third_party/freetype2 \
          third_party/libwebp \
          third_party/dng_sdk \
          third_party/harfbuzz \
          third_party/icu && \
    ( ( [[ $(uname -m) == "aarch64" ]] && echo "On aarch64 - Please run me again!" ) || \
          ninja -C out/Release ) && \
    cd ..
