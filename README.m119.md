Since m118:

* SkImage::makeWithFilter emulated
* GrDirectContext::purgeUnlockedResources, GrDirectContext::flushAndSubmit,
  GrDirectContext::submit takes enums instead of true/false
* some Sk*PathEffect have been removed from the public API
  (TODO: emulate?)
