#!/usr/bin/env bash

# Install system dependencies
if [[ $EUID -eq 0 ]]; then
    if [[ ! -e "/etc/fedora-release" ]] ; then yum -y install epel-release ; fi
    yum install -y \
        python3 \
        ninja-build gn \
        fontconfig-devel \
        mesa-libGL-devel \
        mesa-libEGL-devel libglvnd-devel mesa-libGLES-devel libglvnd mesa-libGLES mesa-libEGL libglvnd-egl \
        xorg-x11-server-Xvfb \
        mesa-dri-drivers && \
        yum clean all && \
        mv depot_tools/ninja depot_tools/ninja.bak && \
        mv depot_tools/gn depot_tools/gn.bak && \
        rm -rf /var/cache/yum
fi

# Build skia
cd skia && \
    patch -p1 < ../patch/skia-m138-minimize-download.patch && \
    patch -p1 < ../patch/skia-m132-colrv1-freetype.diff && \
    patch -p1 < ../patch/skia-m132-egl-runtime.diff && \
    python3 tools/git-sync-deps && \
    gn gen out/Release --args="
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
    ninja -C out/Release && \
    cd ..
