#!/usr/bin/env bash

# Build skia
export CC=clang
export CXX=clang++
export MACOSX_DEPLOYMENT_TARGET="10.9"
cd skia && \
    patch -p1 < ../patch/find_xcode_sysroot.patch && \
    python tools/git-sync-deps && \
    bin/gn gen out/Release --args='
is_official_build=true
skia_enable_tools=true
skia_use_system_libjpeg_turbo=false
skia_use_system_libwebp=false
skia_use_system_libpng=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
extra_cflags_cc=["-frtti", "-mmacosx-version-min=10.9"]
' && \
    ninja -C out/Release skia skia.h experimental_svg_model && \
    cd ..
