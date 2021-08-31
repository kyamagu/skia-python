#!/usr/bin/env bash

export PATH="${PWD}/depot_tools:$PATH"

# Build skia
cd skia && \
    python tools/git-sync-deps && \
    patch -p1 < ../patch/make_data_assembly.patch && \
    bin/gn gen out/Release --args='
is_official_build=true
skia_enable_tools=true
skia_use_system_libjpeg_turbo=false
skia_use_system_libwebp=false
skia_use_system_libpng=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
skia_use_system_expat=false
skia_use_system_zlib=false
extra_cflags_cc=["/GR", "/EHsc", "/MD"]
' && \
    ninja -C out/Release skia skia.h experimental_svg_model && \
    cd ..
