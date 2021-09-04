#!/usr/bin/env bash

export PATH=${PWD}/depot_tools:$PATH

# Build skia
export CC=clang
export CXX=clang++

TARGET_ARCH=${TARGET_ARCH:-$(uname -m)}

if [[ ${TARGET_ARCH} == "arm64" ]]; then
    EXTRA_ARGS='
target_cpu="arm64"
extra_asmflags=["--target=arm64-apple-macos11",]
extra_cflags=["--target=arm64-apple-macos11",]
extra_ldflags=["--target=arm64-apple-macos11",]
'
fi

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
extra_cflags_cc=[\"-frtti\"]
${EXTRA_ARGS}
" && \
    ninja -C out/Release skia skia.h experimental_svg_model && \
    cd ..
