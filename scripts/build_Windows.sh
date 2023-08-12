#!/usr/bin/env bash

export PATH="${PWD}/depot_tools:$PATH"

# Build skia
cd skia && \
    patch -p1 < ../patch/skia-m117-minimize-download.patch && \
    patch -p1 < ../patch/skia-m116-colrv1-freetype.diff && \
    python tools/git-sync-deps && \
    bin/gn gen out/Release --args='
is_official_build=true
skia_enable_svg=true
skia_use_system_libjpeg_turbo=false
skia_use_system_libwebp=false
skia_use_system_libpng=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
skia_use_system_expat=false
skia_use_system_zlib=false
extra_cflags_cc=["/GR", "/EHsc", "/MD"]
' && \
    ninja -C out/Release && \
    cd ..
