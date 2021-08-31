#!/usr/bin/env bash

export PATH=${PWD}/depot_tools:$PATH

# Build skia
export CC=clang
export CXX=clang++
export MACOSX_DEPLOYMENT_TARGET="10.9"

function apply_patch {
    patch -p1 < ../patch/find_xcode_sysroot.patch;
    patch -p1 < ../patch/make_data_assembly.patch;
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
