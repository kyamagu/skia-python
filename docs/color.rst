Color
=====

.. currentmodule:: skia

In Skia, colors are represented as 32-bit ARGB word, where each component has
8-bit. Colors can be specified by Python literal or constructed from the
functions below.

Floating point representation is available via :py:class:`Color4f`.

Example::

    c = 0xFFFF0000  # ARGB
    c = skia.Color(255, 0, 0)  # RGB
    c = skia.Color(255, 0, 0, 255)  # RGBA
    c = skia.ColorSetRGB(255, 255, 0, 0)  # RGB
    c = skia.ColorSetARGB(255, 255, 0, 0)  # ARGB

.. rubric:: Functions

.. autosummary::
    :nosignatures:

    Color
    ColorGetA
    ColorGetR
    ColorGetG
    ColorGetB
    ColorSetA
    ColorSetARGB
    ColorSetRGB
    ColorToHSV
    RGBToHSV
    HSVToColor
    PreMultiplyARGB
    PreMultiplyColor

Functions
---------

.. autofunction:: Color

.. autofunction:: ColorGetA

.. autofunction:: ColorGetR

.. autofunction:: ColorGetG

.. autofunction:: ColorGetB

.. autofunction:: ColorSetA

.. autofunction:: ColorSetARGB

.. autofunction:: ColorSetRGB

.. autofunction:: ColorToHSV

.. autofunction:: RGBToHSV

.. autofunction:: HSVToColor

.. autofunction:: PreMultiplyARGB

.. autofunction:: PreMultiplyColor
