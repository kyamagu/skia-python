v13x offers a lot of new functionality. This document covers breaking changes
when upgrading from v8x.

- Linux users: `libEGL.so` (from mesa-libEGL, libglvnd, or your graphic
  card's vendor e.g. NVidia) is required to be present on Linux hosts.
  This is associated with the general change on Linux from X11 to Wayland,
  and from GTK3 to GTK4.

- Apple users and non-open-source GPU driver: Upstream added subtantial
  GPU/driver detection code. You may need to request compatible OpenGL
  profile to match your GPU/driver's capability. For Apple Mac users,
  with `glfw`, adds the following:

```
# see https://www.glfw.org/faq#macos
glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 2)
glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, True)
glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
```

  Other examples are in [README.m116](relnotes/README.m116.md).

# General

- Some routines need a new `skia.SamplingOptions()` argument, or
  switch from `skia.FilterQuality` to `skia.SamplingOptions()`.
  See [README.m124](relnotes/README.m124.md) for migration from
  `skia.FilterQuality`.

- `Typeface()` and `Typeface.MakeDefault()` now emit a Deprecation Warning.
  Use `Typeface("")` instead. Also avoid `skia.Font()` and
  `skia.Font(None, ...)` ; e.g. `skia.Font(skia.Typeface(''))` should be used.
  See [README.m121](relnotes/README.m121.md)

- A fair number of `skia.ImageFilter.*Filter` and `skia.*PathEffect` are
  gone. Sorry. Upstream likely recommends reimplementation with `skia.RuntimeEffect`.
  Please file issues, or better, pulls to emulate those old APIs. TODO

- Misc small changes everywhere.
