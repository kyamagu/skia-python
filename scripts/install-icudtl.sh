#!/usr/bin/env bash

# The version of icu used by skia can be read in:
# https://chromium.googlesource.com/chromium/deps/icu/+/<hash>
# where <hash> is the line in DEPS, or in our patch/skia-m<version>-minimize-download.patch.

# e.g.
# From m116 to m125, it is a0718d4f121727e30b8d52c7a189ebf5ab52421f
# https://chromium.googlesource.com/chromium/deps/icu/+/a0718d4f121727e30b8d52c7a189ebf5ab52421f/version.gni
# Says it is "69".
#
# From m126 to m130, it is 364118a1d9da24bb5b770ac3d762ac144d6da5a4
# https://chromium.googlesource.com/chromium/deps/icu/+/364118a1d9da24bb5b770ac3d762ac144d6da5a4/version.gni
# refers to "version.json", which says 74.

python -c 'import urllib.request; urllib.request.urlretrieve("https://github.com/unicode-org/icu/releases/download/release-74-2/icu4c-74_2-data-bin-l.zip", "icudatal.zip")'
unzip icudatal.zip
python -c 'import sys; print(sys.path)'
python -c 'import site; print(site.getsitepackages())'
# site.getsitepackages()[0] does not seem to work for venv
python -c 'import os, shutil, site; shutil.copy2("icudt74l.dat", os.path.join(site.getsitepackages()[1], "icudtl.dat"))'
