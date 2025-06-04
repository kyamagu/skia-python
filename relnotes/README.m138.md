m138 includes a lot of changes in the CI build machinery, OpenGL,
shader effects, and API coverage closer to other Skia bindings
(Skia4Delphi and SkiaSharp). **IMPORTANT:** The presence of `libEGL.so` (part of
mesa-libEGL / libglvnd-egl) is now a requirement on Linux.

New to m138:

- We now offers arm64 window wheels for windows 11. This requires
  numpy 2.3.0rc1. Numpy 2.3.0rc1 supports only python 3.11/3.12/3.13.
  The other wheels continue to use numpy 2.2.x.

  We carry two patches, one enables OpenGL. It was disabled upstream earlier,
  but apparently OpenGL works well enough on arm64 windows 11 with a
  compatibility pack from Microsoft.
  (https://apps.microsoft.com/detail/9nqpsl29bfff?hl=en-US&gl=US)
  The other is backported from m139
  (https://issues.skia.org/420271794) for compatibility with FreeType.

  Both patches are expected to be dropped in a future skia-python release.

- On Linux, we now offer OpenGL initializaion with EGL as an option.
  This is required to work with GTK4, and other (headless) situation
  of hardware acceleration without X11, with
  `skia.GrDirectContext.MakeGL(skia.GrGLInterface.MakeEGL())` .
  `skia.GrDirectContext.MakeGL()` (the default) is identical
  `skia.GrDirectContext.MakeGL(skia.GrGLInterface.MakeGLX())` ,
  continues to be the older (GLX) way of using OpenGL under X11.

  As a result, you'll need to have libEGL (part of mesa-libEGL / libglvnd-egl)
  on your system, too. Many Linux users likely have this already,
  but please check.

  There are examples of using OpenGL with skia-python in
  GLFW, SDL2, SDL3, WX, GTK3, GTK4, QT5, QT6 at
  https://github.com/HinTak/skia-python-examples/ . Besides the GTK4
  EGL requirement, we also filed a QT6 regression:
  https://bugreports.qt.io/browse/QTBUG-137158
  [Regression] Qt6's QOpenGLWidget crashes when interacting with Skia's CPU raster

- A lot of additions under `skia.RuntimeEffect*`. 

  This allows skia-python to run all the `shaders/*.sksl`
  examples from
  https://github.com/jimmckeeth/SkiaSimpleShaderViewer/,
  a Skia4Delphi-based SkSL viewer,
  with a small python script `SkSL_SimpleShaderViewer.py`,
  in https://github.com/HinTak/skia-python-examples/ .
  and `SkSL_ShaderMulti+SDL2.py` which contains examples
  from https://shaders.skia.org/ .

- `Canvas.drawTextOnPath(text, path, ...)` re-added, for
  drawing morphed text around a curved path. It was removed
  in upstream Skia after m70, but the SkiaSharp people deemed it
  useful enough to re-implement it. We adapted the old
  Google-written code to current skia.
  There is a small demo, `draw_text_on_path.py`, in
  https://github.com/HinTak/skia-python-examples/ .

- Added `Surface` constructor taking width, height, colortype
  or `ImageInfo` as arguments. To match interaction with other
  software expecting specific colortype order.

- (Not user-visible: a few methods are overloaded and require
   `py::overload_cast()` added around them to continue to work)

CI build changes:

- We now build aarch64 linux wheels on native hosts, much faster
  than under qemu emulation and also allows a lot of simplification.
  Added arm64 windows wheels for windows 11. The latter is against
  numpy 2.3.0rc1.

- There is a new `build_and_test_on_Fedora` section in CI which builds
  skia-python in a more manual way, and test its headless EGL
  functionality, without cibuildwheel.
