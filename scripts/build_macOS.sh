#!/usr/bin/env bash

# Build skia
export CC=clang
export CXX=clang++
export MACOSX_DEPLOYMENT_TARGET="10.9"


function apply_patch {
    patch -p1 < ../patch/find_xcode_sysroot.patch;
    2to3 -w third_party/externals/icu/scripts/make_data_assembly.py;
}


cd skia && \
    python tools/git-sync-deps && \
    apply_patch && \
    bin/gn gen out/Release --args="
is_official_build=true
skia_enable_tools=true
skia_use_system_libjpeg_turbo=false
skia_use_system_libwebp=false
skia_use_system_libpng=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
extra_cflags_cc=[\"-frtti\", \"-mmacosx-version-min=${MACOSX_DEPLOYMENT_TARGET}\"]
" && \
    ninja -C out/Release skia skia.h experimental_svg_model && \
    cd ..
