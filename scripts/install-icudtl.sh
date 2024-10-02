#!/usr/bin/env bash
python -c 'import urllib.request; urllib.request.urlretrieve("https://github.com/unicode-org/icu/releases/download/release-75-1/icu4c-75_1-data-bin-l.zip", "icudatal.zip")'
unzip icudatal.zip
python -c 'import os, shutil, site; shutil.copy2("icudt75l.dat", os.path.join(site.getsitepackages()[0], "icudtl.dat"))'
