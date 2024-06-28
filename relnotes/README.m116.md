This is a partial port of [skia-python](https://github.com/kyamagu/skia-python/)
from `m87` to `m116`, possibly disabling any `m87` APIs that have no close `m116` equivalents.

It concentrates on OT-SVG, and fixing these two issues:

* [SkSVGDOM::renderNode() is not exposed in python](https://github.com/kyamagu/skia-python/issues/192)
* [three-args contructor to SkMemoryStream not exposed.](https://github.com/kyamagu/skia-python/issues/194)

The SVG module left experimental in `m88` upstream. It has received many improvements since.

Some COLRv1-related Skia internals from upstream's on-going effort in this area
are also exposed for access. This experimental functionality is available to
Linux/FreeType users only.

Special mention of [0lru](https://github.com/0lru) who provided a
[draft m98 pull](https://github.com/kyamagu/skia-python/pull/181) for which some ideas
of this update had taken from.

# General overview of changes between `m87` and `m116`

* TL;DR - `m87` users would likely find most existing python scripts work. Some
  routines need a new `skia.SamplingOptions()` argument, or
  switch from `skia.FilterQuality` to `skia.SamplingOptions()`.
  OpenGL users, especially on Apple Mac OS, may now need to specify
  a compatible OpenGL profile for their GPU hardware/software combination
  to avoid shader-compilation related errors (See more about this below).
  Please report `AttributeError: 'skia.AAA' object has no attribute 'BBB'` errors,
  to prioritize fixing remaining differences between `m87` and `m116`.

* The number of public symbols/routines in upstream skia is around 2400,
  consistently between `m87` and `m116` (`m88`, `m98`, `m103` were examined).
  skia-python `m87` accesses just over ~1000 at link time, and possibly
  another 100 or two via `dynamic_cast`'ing at runtime. Less than ~800
  of them has exact equivalents in `m116`. A good proportion of
  the 200+ differ by additional arguments, often with defaults
  (e.g. `skia.SamplingOptions()`). A few with not-useful arguments have them removed.
  Some of the rest, like the Image I/O routines and Surface routines,
  are considered too often used and too important, and are emulated in m116. The rest ... read on:

* Be **WARN**'ed: some `m87` APIs (about 5% in total, many in the `ImageFilter` namespace)
  are removed/disabled when there are no obvious new-equivalents, or not-too-troblesome
  emulations with `m116`. The "AttributeError" error mentioned above.

* Be **WARN**'ed on OpenGL usage: Google folks added subtantial GPU/driver
  detection code in upsteam Skia between m87 and m116, to optimize for speed and
  work-around driver bugs. If you use a non-open-source GPU driver, i.e.
  everybody except Mesa on Linux, and especially Apple Mac users,
  you may need to request compatible OpenGL profile to match the your
  GPU/driver's capability. For Apple Mac users, with `glfw`, adds the following:

```
# see https://www.glfw.org/faq#macos
glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 2)
glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, True)
glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
```

  OSX OpenGL users may need to add `GLUT_3_2_CORE_PROFILE` to their `glutInitDisplayMode()`
  invocation e.g. `glutInitDisplayMode(... | GLUT_3_2_CORE_PROFILE)`. `pysdl2` users
  need:

```
sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_CONTEXT_MAJOR_VERSION, 3)
sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_CONTEXT_MINOR_VERSION, 2)
sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG, True)
sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_CONTEXT_PROFILE_MASK,
                         sdl2.SDL_GL_CONTEXT_PROFILE_CORE)
```

* Where it is possible, when `m87` APIs disappear, emulations with `m116`
  is done. So these are "new emulations of old APIs". While they work,
  they might be withdrawn/changed later:

  Image I/O and decoding routines -
  `encodeToData`, `MakeRasterCopy`,
  `MakeFromRaster`, `MakeFromBitmap`, `MakeFromEncoded`,
  `MakeTextureFromCompressed`, `MakeRasterFromCompressed`,
  `MakeFromTexture`, `MakeFromCompressedTexture`,
  `MakeCrossContextFromPixmap`, `MakeFromAdoptedTexture`,
  `MakeFromYUVATexturesCopy`, `MakeFromYUVATexturesCopyWithExternalBackend`,
  `MakeFromYUVATextures`, `MakeFromYUVAPixmaps`, `MakeFromYUVAPixmaps`,
  `MakeFromPicture`, `MakeBackendTextureFromSkImage`,
  `MakeBackendTextureFromSkImage`
  - these are emulated in `m116`. In particular, upstream recommends
    using the graphic format encoding/decoding routines directly. Not yet directly exposed in `skia-python`.

  Surface methods -
  `Surface.MakeRasterN32Premul`, `Surface.MakeRasterDirect`,
  `Surface.getBackendTexture`, `Surface.getBackendRenderTarget`,
  `Surface.MakeRaster`, `Surface.MakeFromBackendTexture`,
  `Surface.MakeFromBackendRenderTarget`, `Surface::MakeRenderTarget`,
  `Surface.MakeNull`.
  - these are simple renamings; we might add the new names and remove
    the old names, and document them as renamed.

* Most `GrContext` class methods were merged into `GrDirectContext` class
  methods. For now, we have an alias between those two classes, and
  internally, use `GrContext` as it is shorter. In the long term,
  people should use `GrDirectContext`, to align with upstream documentation.

* New `SamplingOptions` class; there is only a default constructor in m116 for now.
  Many APIs changes from `m87` to `m116` concern adding new options with it, or
  changing from `FilterQuality` enum to `SamplingOptions`.

  m124 Addendum: m124 completed `SamplingOptions` class. In m124 or later,
  to migrate from m87 FilterQuality-based code:

  FilterQuality.kHigh_FilterQuality   -> SamplingOptions(CubicResampler.Mitchell())
  FilterQuality.kMedium_FilterQuality -> SamplingOptions(FilterMode.kLinear, MipmapMode.kNearest)    // cpu
                                         or SamplingOptions(FilterMode.kLinear, MipmapMode.kLinear)  // gpu
  FilterQuality.kLow_FilterQuality    -> SamplingOptions(FilterMode.kLinear, MipmapMode.kNone)
  FilterQuality.kNone_FilterQuality   -> SamplingOptions(FilterMode.kNearest, MipmapMode.kNone)

* `yuvainfo.PlanarConfig` enums were splitted into a combination of
  `yuvainfo.PlaneConfig` and `yuvainfo.Subsampling` enums. Where the former
  enum was used in `m87`, it is the equivalent of a combination of the latter
  two in `m116`.
