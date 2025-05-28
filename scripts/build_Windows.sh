#!/usr/bin/env bash

export PATH="${PWD}/depot_tools:$PATH"

if [[ ${RUNNER_ARCH} == "ARM64" ]]; then
    EXTRA_ARGS='
target_cpu="arm64"
'
    echo "setting ARM64 Windows Skia build"
fi


# Build skia
cd skia && \
    patch -p1 -R < ../patch/0001-Disable-OpenGL-for-Windows-on-ARM64.patch && \
    python tools/git-sync-deps && \
    patch -p1 < ../patch/make_data_assembly.patch && \
    patch -p1 < ../patch/skia-m87-c++-code.diff && \
    bin/gn gen out/Release --args="
is_official_build=true
skia_enable_tools=true
skia_use_system_libjpeg_turbo=false
skia_use_system_libwebp=false
skia_use_system_libpng=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
skia_use_system_expat=false
skia_use_system_zlib=false
extra_cflags_cc=[\"/GR\", \"/EHsc\", \"/MD\"]
${EXTRA_ARGS}
" && \
    ninja -C out/Release skia skia.h experimental_svg_model && \
    cd ..
