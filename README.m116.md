This is a partial port of [skia-python](https://github.com/kyamagu/skia-python/)
from m87 to m116, possibly disabling any API that's not working.

It concentrates on OT-SVG, and fixing these two issues:

[SkSVGDOM::renderNode() is not exposed in python](https://github.com/kyamagu/skia-python/issues/192)
[three-args contructor to SkMemoryStream not exposed.](https://github.com/kyamagu/skia-python/issues/194)

Some COLRv1-related internals are also exposed for access.
