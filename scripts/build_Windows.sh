#!/usr/bin/env bash

export PATH="${PWD}/depot_tools:$PATH"

if [[ ${RUNNER_ARCH} == "ARM64" ]]; then
    EXTRA_ARGS='
target_cpu="arm64"
skia_use_freetype=false
skia_enable_fontmgr_custom_empty=false
'
    echo "setting ARM64 Windows Skia build"
fi


# Build skia
cd skia && \
    patch -p1 < ../patch/skia-m136-minimize-download.patch && \
    patch -p1 < ../patch/skia-m132-colrv1-freetype.diff && \
    python tools/git-sync-deps && \
    bin/gn gen out/Release --args="
is_official_build=true
skia_enable_svg=true
skia_use_vulkan=true
skia_use_freetype=true
skia_use_system_freetype2=false
skia_enable_fontmgr_custom_empty=true
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
    ninja -C out/Release && \
    cd ..
