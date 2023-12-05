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
}

cd skia && \
    patch -p1 < ../patch/skia-m120-minimize-download.patch && \
    patch -p1 < ../patch/skia-m116-colrv1-freetype.diff && \
    python3 tools/git-sync-deps && \
    bin/gn gen out/Release --args="
is_official_build=true
skia_enable_svg=true
skia_use_freetype=true
skia_use_system_freetype2=false
skia_enable_fontmgr_custom_empty=true
skia_use_system_libjpeg_turbo=false
skia_use_system_libwebp=false
skia_use_system_libpng=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
skia_use_system_expat=false
extra_cflags_cc=[\"-frtti\"]
${EXTRA_ARGS}
" && \
    ninja -C out/Release && \
    cd ..
