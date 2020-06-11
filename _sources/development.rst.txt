Development
===========

Python binding design
---------------------

`skia-python` is built upon `pybind11`_. Python bindings are mostly plain
conversion from `Skia C++ API`_. Function names remain the same.

.. _pybind11: https://github.com/pybind/pybind11
.. _Skia C++ API: https://api.skia.org/

A few differences are:

- All bindings reside in ``skia`` module.
- Removes class name prefix ``Sk``; e.g., ``SkCanvas`` is ``skia.Canvas``.
- Some method signatures adapt to Python style; e.g.,
  - :py:meth:`skia.Surface.__init__` accepts additional args
  - :py:class:`skia.Paint` is implicitly convertible from ``dict``.

Contributing
------------

Development happens at `Github`_. Feel free to post to `issues`_ or make `PR`_.

.. _Github: https://github.com/kyamagu/skia-python
.. _issues: https://github.com/kyamagu/skia-python/issues
.. _PR: https://github.com/kyamagu/skia-python/pulls
