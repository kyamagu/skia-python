#include "common.h"

// Static variables must be declared.
constexpr int SkMatrix::kMScaleX;
constexpr int SkMatrix::kMSkewX;
constexpr int SkMatrix::kMTransX;
constexpr int SkMatrix::kMSkewY;
constexpr int SkMatrix::kMScaleY;
constexpr int SkMatrix::kMTransY;
constexpr int SkMatrix::kMPersp0;
constexpr int SkMatrix::kMPersp1;
constexpr int SkMatrix::kMPersp2;
constexpr int SkMatrix::kAScaleX;
constexpr int SkMatrix::kASkewY;
constexpr int SkMatrix::kASkewX;
constexpr int SkMatrix::kAScaleY;
constexpr int SkMatrix::kATransX;
constexpr int SkMatrix::kATransY;

void initMatrix(py::module &m) {
py::enum_<SkApplyPerspectiveClip>(m, "ApplyPerspectiveClip")
    .value("kNo", SkApplyPerspectiveClip::kNo,
        "Don't pre-clip the geometry before applying the (perspective) matrix.")
    .value("kYes", SkApplyPerspectiveClip::kYes,
        "Do pre-clip the geometry before applying the (perspective) matrix.")
    .export_values();

// Matrix
py::class_<SkMatrix> matrix(m, "Matrix", R"docstring(
    SkMatrix holds a 3x3 matrix for transforming coordinates.

    This allows mapping SkPoint and vectors with translation, scaling, skewing,
    rotation, and perspective.

    SkMatrix elements are in row major order. SkMatrix does not have a
    constructor, so it must be explicitly initialized. setIdentity() initializes
    SkMatrix so it has no effect. setTranslate(), setScale(), setSkew(),
    setRotate(), set9 and setAll() initializes all SkMatrix elements with the
    corresponding mapping.

    SkMatrix includes a hidden variable that classifies the type of matrix to
    improve performance. SkMatrix is not thread safe unless getType() is called
    first.
    )docstring");

py::enum_<SkMatrix::ScaleToFit>(matrix, "ScaleToFit")
    .value("kFill", SkMatrix::ScaleToFit::kFill_ScaleToFit,
        "scales in x and y to fill destination SkRect")
    .value("kStart", SkMatrix::ScaleToFit::kStart_ScaleToFit,
        "scales and aligns to left and top")
    .value("kCenter", SkMatrix::ScaleToFit::kCenter_ScaleToFit,
        "scales and aligns to center")
    .value("kEnd", SkMatrix::ScaleToFit::kEnd_ScaleToFit,
        "scales and aligns to right and bottom")
    .export_values();

py::enum_<SkMatrix::TypeMask>(matrix, "TypeMask")
    .value("kIdentity_Mask", SkMatrix::TypeMask::kIdentity_Mask,
        "identity SkMatrix; all bits clear")
    .value("kTranslate_Mask", SkMatrix::TypeMask::kTranslate_Mask,
        "translation SkMatrix")
    .value("kScale_Mask", SkMatrix::TypeMask::kScale_Mask,
        "scale SkMatrix")
    .value("kAffine_Mask", SkMatrix::TypeMask::kAffine_Mask,
        "skew or rotate SkMatrix")
    .value("kPerspective_Mask", SkMatrix::TypeMask::kPerspective_Mask,
        "perspective SkMatrix")
    .export_values();

matrix
    .def(py::init<>(), "Creates an identity SkMatrix.")
    .def("getType", &SkMatrix::getType,
        "Returns a bit field describing the transformations the matrix may "
        "perform.")
    .def("isIdentity", &SkMatrix::isIdentity,
        "Returns true if SkMatrix is identity.")
    .def("isScaleTranslate", &SkMatrix::isScaleTranslate,
        "Returns true if SkMatrix at most scales and translates.")
    .def("isTranslate", &SkMatrix::isTranslate,
        "Returns true if SkMatrix is identity, or translates.")
    .def("rectStaysRect", &SkMatrix::rectStaysRect,
        "Returns true SkMatrix maps SkRect to another SkRect.")
    .def("preservesAxisAlignment", &SkMatrix::preservesAxisAlignment,
        "Returns true SkMatrix maps SkRect to another SkRect.")
    .def("hasPerspective", &SkMatrix::hasPerspective,
        "Returns true if the matrix contains perspective elements.")
    .def("isSimilarity", &SkMatrix::isSimilarity,
        "Returns true if SkMatrix contains only translation, rotation, "
        "reflection, and uniform scale.")
    .def("preservesRightAngles", &SkMatrix::preservesRightAngles,
        "Returns true if SkMatrix contains only translation, rotation, "
        "reflection, and scale.")
    .def("__getitem__", [](const SkMatrix& m, int index) {
            return m[index];
        }, "Returns one matrix value.", py::is_operator())
    .def("get", &SkMatrix::get, "Returns one matrix value.")
    .def("getScaleX", &SkMatrix::getScaleX,
        "Returns scale factor multiplied by x-axis input, contributing to "
        "x-axis output.")
    .def("getScaleY", &SkMatrix::getScaleY,
        "Returns scale factor multiplied by y-axis input, contributing to "
        "y-axis output.")
    .def("getSkewY", &SkMatrix::getSkewY,
        "Returns scale factor multiplied by x-axis input, contributing to "
        "y-axis output.")
    .def("getSkewX", &SkMatrix::getSkewX,
        "Returns scale factor multiplied by y-axis input, contributing to "
        "x-axis output.")
    .def("getTranslateX", &SkMatrix::getTranslateX,
        "Returns translation contributing to x-axis output.")
    .def("getTranslateY", &SkMatrix::getTranslateY,
        "Returns translation contributing to y-axis output.")
    .def("getPerspX", &SkMatrix::getPerspX,
        "Returns factor scaling input x-axis relative to input y-axis.")
    .def("getPerspY", &SkMatrix::getPerspY,
        "Returns factor scaling input y-axis relative to input x-axis.")
    .def("__setitem__", [](SkMatrix& m, int index, SkScalar value) {
            m[index] = value;
        }, "Returns writable SkMatrix value.", py::is_operator())
    .def("set", &SkMatrix::set, "Sets SkMatrix value.")
    .def("setScaleX", &SkMatrix::setScaleX, "Sets horizontal scale factor.")
    .def("setScaleY", &SkMatrix::setScaleY, "Sets vertical scale factor.")
    .def("setSkewY", &SkMatrix::setSkewY, "Sets vertical skew factor.")
    .def("setSkewX", &SkMatrix::setSkewX, "Sets horizontal skew factor.")
    .def("setTranslateX", &SkMatrix::setTranslateX,
        "Sets horizontal translation.")
    .def("setTranslateY", &SkMatrix::setTranslateY,
        "Sets vertical translation.")
    .def("setPerspX", &SkMatrix::setPerspX,
        "Sets input x-axis perspective factor, which causes mapXY() to vary "
        "input x-axis values inversely proportional to input y-axis values.")
    .def("setPerspY", &SkMatrix::setPerspY,
        "Sets input y-axis perspective factor, which causes mapXY() to vary "
        "input y-axis values inversely proportional to input x-axis values.")
    .def("setAll", &SkMatrix::setAll, "Sets all values from parameters.")
    .def("get9", &SkMatrix::get9,
        "Copies nine scalar values contained by SkMatrix into buffer, in "
        "member value ascending order: kMScaleX, kMSkewX, kMTransX, kMSkewY, "
        "kMScaleY, kMTransY, kMPersp0, kMPersp1, kMPersp2.")
    .def("set9", &SkMatrix::set9,
        "Sets SkMatrix to nine scalar values in buffer, in member value "
        "ascending order: kMScaleX, kMSkewX, kMTransX, kMSkewY, kMScaleY, "
        "kMTransY, kMPersp0, kMPersp1, kMPersp2.")
    .def("reset", &SkMatrix::reset,
        "Sets SkMatrix to identity; which has no effect on mapped SkPoint.")
    .def("setIdentity", &SkMatrix::setIdentity,
        "Sets SkMatrix to identity; which has no effect on mapped SkPoint.")
    .def("setTranslate",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::setTranslate),
        "Sets SkMatrix to translate by (dx, dy).")
    .def("setTranslate",
        py::overload_cast<const SkVector&>(&SkMatrix::setTranslate),
        "Sets SkMatrix to translate by (dx, dy).")
    .def("setScale",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkMatrix::setScale),
        "Sets SkMatrix to scale by sx and sy, about a pivot point at (px, py).")
    .def("setScale",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::setScale),
        "Sets SkMatrix to scale by sx and sy about at pivot point at (0, 0).")
    .def("setRotate",
        py::overload_cast<SkScalar, SkScalar, SkScalar>(&SkMatrix::setRotate),
        "Sets SkMatrix to rotate by degrees about a pivot point at (px, py).")
    .def("setRotate", py::overload_cast<SkScalar>(&SkMatrix::setRotate),
        "Sets SkMatrix to rotate by degrees about a pivot point at (0, 0).")
    .def("setSinCos",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkMatrix::setSinCos),
        "Sets SkMatrix to rotate by sinValue and cosValue, about a pivot point"
        " at (px, py).")
    .def("setSinCos",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::setSinCos),
        "Sets SkMatrix to rotate by sinValue and cosValue, about a pivot point"
        " at (0, 0).")
    .def("setRSXform", &SkMatrix::setRSXform,
        "Sets SkMatrix to rotate, scale, and translate using a compressed "
        "matrix form.")
    .def("setSkew",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkMatrix::setSkew),
        "Sets SkMatrix to skew by kx and ky, about a pivot point at (px, py).")
    .def("setSkew", py::overload_cast<SkScalar, SkScalar>(&SkMatrix::setSkew),
        "Sets SkMatrix to skew by kx and ky, about a pivot point at (0, 0).")
    .def("setConcat", &SkMatrix::setConcat,
        "Sets SkMatrix to SkMatrix a multiplied by SkMatrix b.")
    .def("preTranslate", &SkMatrix::preTranslate,
        "Sets SkMatrix to SkMatrix multiplied by SkMatrix constructed from "
        "translation (dx, dy).")
    .def("preScale",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkMatrix::preScale),
        "Sets SkMatrix to SkMatrix multiplied by SkMatrix constructed from "
        "scaling by (sx, sy) about pivot point (px, py).")
    .def("preScale",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::preScale),
        "Sets SkMatrix to SkMatrix multiplied by SkMatrix constructed from "
        "scaling by (sx, sy) about pivot point (0, 0).")
    .def("preRotate",
        py::overload_cast<SkScalar, SkScalar, SkScalar>(&SkMatrix::preRotate),
        "Sets SkMatrix to SkMatrix multiplied by SkMatrix constructed from "
        "rotating by degrees about pivot point (px, py).")
    .def("preRotate",
        py::overload_cast<SkScalar>(&SkMatrix::preRotate),
        "Sets SkMatrix to SkMatrix multiplied by SkMatrix constructed from "
        "rotating by degrees about pivot point (0, 0).")
    .def("preSkew",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkMatrix::preSkew),
        "Sets SkMatrix to SkMatrix multiplied by SkMatrix constructed from "
        "skewing by (kx, ky) about pivot point (px, py).")
    .def("preSkew",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::preSkew),
        "Sets SkMatrix to SkMatrix multiplied by SkMatrix constructed from "
        "skewing by (kx, ky) about pivot point (0, 0).")
    .def("preConcat", &SkMatrix::preConcat,
        "Sets SkMatrix to SkMatrix multiplied by SkMatrix other.")
    .def("postTranslate", &SkMatrix::postTranslate,
        "Sets SkMatrix to SkMatrix constructed from translation (dx, dy) "
        "multiplied by SkMatrix.")
    .def("postScale",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkMatrix::postScale),
        "Sets SkMatrix to SkMatrix constructed from scaling by (sx, sy) about "
        "pivot point (px, py), multiplied by SkMatrix.")
    .def("postScale",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::postScale),
        "Sets SkMatrix to SkMatrix constructed from scaling by (sx, sy) about "
        "pivot point (0, 0), multiplied by SkMatrix.")
    .def("postRotate",
        py::overload_cast<SkScalar, SkScalar, SkScalar>(&SkMatrix::postRotate),
        "Sets SkMatrix to SkMatrix constructed from rotating by degrees about "
        "pivot point (px, py), multiplied by SkMatrix.")
    .def("postRotate",
        py::overload_cast<SkScalar>(&SkMatrix::postRotate),
        "Sets SkMatrix to SkMatrix constructed from rotating by degrees about "
        "pivot point (0, 0), multiplied by SkMatrix.")
    .def("postSkew",
        py::overload_cast<SkScalar, SkScalar, SkScalar, SkScalar>(
            &SkMatrix::postSkew),
        "Sets SkMatrix to SkMatrix constructed from skewing by (kx, ky) about "
        "pivot point (px, py), multiplied by SkMatrix.")
    .def("postSkew",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::postSkew),
        "Sets SkMatrix to SkMatrix constructed from skewing by (kx, ky) about "
        "pivot point (0, 0), multiplied by SkMatrix.")
    .def("postConcat", &SkMatrix::postConcat,
        "Sets SkMatrix to SkMatrix other multiplied by SkMatrix.")
    .def("setRectToRect", &SkMatrix::setRectToRect,
        "Sets SkMatrix to scale and translate src SkRect to dst SkRect.")
    .def("setPolyToPoly", &SkMatrix::setPolyToPoly,
        "Sets SkMatrix to map src to dst.")
    .def("invert", &SkMatrix::invert,
        "Sets inverse to reciprocal matrix, returning true if SkMatrix can be "
        "inverted.")
    .def("asAffine", &SkMatrix::asAffine, "Fills affine in column major order.")
    .def("setAffine", &SkMatrix::setAffine,
        "Sets SkMatrix to affine values, passed in column major order.")
    .def("normalizePerspective", &SkMatrix::normalizePerspective,
        "A matrix is categorized as 'perspective' if the bottom row is not "
        "[0, 0, 1].")
    .def("mapPoints",
        py::overload_cast<SkPoint[], const SkPoint[], int>(
            &SkMatrix::mapPoints, py::const_),
        "Maps src SkPoint array of length count to dst SkPoint array of equal "
        "or greater length.")
    .def("mapPoints",
        py::overload_cast<SkPoint[], int>(&SkMatrix::mapPoints, py::const_),
        "Maps pts SkPoint array of length count in place.")
    .def("mapHomogeneousPoints",
        py::overload_cast<SkPoint3[], const SkPoint3[], int>(
            &SkMatrix::mapHomogeneousPoints, py::const_),
        "Maps src SkPoint3 array of length count to dst SkPoint3 array, which "
        "must of length count or greater.")
    .def("mapHomogeneousPoints",
        py::overload_cast<SkPoint3[], const SkPoint[], int>(
            &SkMatrix::mapHomogeneousPoints, py::const_),
        "Returns homogeneous points, starting with 2D src points (with implied "
        "w = 1).")
    .def("mapXY",
        py::overload_cast<SkScalar, SkScalar, SkPoint*>(
            &SkMatrix::mapXY, py::const_),
        "Maps SkPoint (x, y) to result.")
    .def("mapXY",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::mapXY, py::const_),
        "Returns SkPoint (x, y) multiplied by SkMatrix.")
    .def("mapVectors",
        py::overload_cast<SkVector[], const SkVector[], int>(
            &SkMatrix::mapVectors, py::const_),
        "Maps src vector array of length count to vector SkPoint array of "
        "equal or greater length.")
    .def("mapVectors",
        py::overload_cast<SkVector[], int>(&SkMatrix::mapVectors, py::const_),
        "Maps vecs vector array of length count in place, multiplying each "
        "vector by SkMatrix, treating SkMatrix translation as zero.")
    .def("mapVector",
        py::overload_cast<SkScalar, SkScalar, SkVector*>(
            &SkMatrix::mapVector, py::const_),
        "Maps vector (dx, dy) to result.")
    .def("mapVector",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::mapVector, py::const_),
        "Returns vector (dx, dy) multiplied by SkMatrix, treating SkMatrix "
        "translation as zero.")
    .def("mapRect",
        py::overload_cast<SkRect*, const SkRect&, SkApplyPerspectiveClip>(
            &SkMatrix::mapRect, py::const_),
        "Sets rect to bounds of rect corners mapped by SkMatrix.")
    .def("mapRect",
        py::overload_cast<SkRect*, SkApplyPerspectiveClip>(
            &SkMatrix::mapRect, py::const_),
        "Sets rect to bounds of rect corners mapped by SkMatrix.")
    .def("mapRect",
        py::overload_cast<const SkRect&, SkApplyPerspectiveClip>(
            &SkMatrix::mapRect, py::const_),
        "Returns bounds of src corners mapped by SkMatrix.")
    .def("mapRectToQuad", &SkMatrix::mapRectToQuad,
        "Maps four corners of rect to dst.")
    .def("mapRectScaleTranslate", &SkMatrix::mapRectScaleTranslate,
        "Sets dst to bounds of src corners mapped by SkMatrix.")
    .def("mapRadius", &SkMatrix::mapRadius,
        "Returns geometric mean radius of ellipse formed by constructing "
        "circle of size radius, and mapping constructed circle with SkMatrix.")
    .def("dump", &SkMatrix::dump,
        "Writes text representation of SkMatrix to standard output.")
    .def("getMinScale", &SkMatrix::getMinScale,
        "Returns the minimum scaling factor of SkMatrix by decomposing the "
        "scaling and skewing elements.")
    .def("getMaxScale", &SkMatrix::getMaxScale,
        "Returns the maximum scaling factor of SkMatrix by decomposing the "
        "scaling and skewing elements.")
    .def("getMinMaxScales", &SkMatrix::getMinMaxScales,
        "Sets scaleFactors[0] to the minimum scaling factor, and "
        "scaleFactors[1] to the maximum scaling factor.")
    .def("decomposeScale", &SkMatrix::decomposeScale,
        "Decomposes SkMatrix into scale components and whatever remains.")
    .def("dirtyMatrixTypeCache", &SkMatrix::dirtyMatrixTypeCache,
        "Sets internal cache to unknown state.")
    .def("setScaleTranslate", &SkMatrix::setScaleTranslate,
        "Initializes SkMatrix with scale and translate elements.")
    .def("isFinite", &SkMatrix::isFinite,
        "Returns true if all elements of the matrix are finite.")
    .def_static("MakeScale",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::MakeScale),
        "Sets SkMatrix to scale by (sx, sy).")
    .def_static("MakeScale",
        py::overload_cast<SkScalar>(&SkMatrix::MakeScale),
        "Sets SkMatrix to scale by (scale, scale).")
    .def_static("MakeTrans",
        py::overload_cast<SkScalar, SkScalar>(&SkMatrix::MakeTrans),
        "Sets SkMatrix to translate by (dx, dy).")
    .def_static("MakeTrans",
        py::overload_cast<SkVector>(&SkMatrix::MakeTrans),
        "Sets SkMatrix to translate by (t.x(), t.y()).")
    .def_static("MakeTrans",
        py::overload_cast<SkIVector>(&SkMatrix::MakeTrans))
    .def_static("MakeAll", &SkMatrix::MakeAll, "Sets SkMatrix.")
    .def_static("MakeRectToRect", &SkMatrix::MakeRectToRect,
        "Returns SkMatrix set to scale and translate src SkRect to dst SkRect.")
    .def_static("SetAffineIdentity", &SkMatrix::SetAffineIdentity,
        "Fills affine with identity values in column major order.")
    .def_static("I", &SkMatrix::I,
        "Returns reference to const identity SkMatrix.")
    .def_static("InvalidMatrix", &SkMatrix::InvalidMatrix,
        "Returns reference to a const SkMatrix with invalid values.")
    .def_static("Concat", &SkMatrix::Concat,
        "Returns SkMatrix a multiplied by SkMatrix b.")
    .def_readonly_static("kMScaleX", &SkMatrix::kMScaleX)
    .def_readonly_static("kMSkewX", &SkMatrix::kMSkewX)
    .def_readonly_static("kMTransX", &SkMatrix::kMTransX)
    .def_readonly_static("kMSkewY", &SkMatrix::kMSkewY)
    .def_readonly_static("kMScaleY", &SkMatrix::kMScaleY)
    .def_readonly_static("kMTransY", &SkMatrix::kMTransY)
    .def_readonly_static("kMPersp0", &SkMatrix::kMPersp0)
    .def_readonly_static("kMPersp1", &SkMatrix::kMPersp1)
    .def_readonly_static("kMPersp2", &SkMatrix::kMPersp2)
    .def_readonly_static("kAScaleX", &SkMatrix::kAScaleX)
    .def_readonly_static("kASkewY", &SkMatrix::kASkewY)
    .def_readonly_static("kASkewX", &SkMatrix::kASkewX)
    .def_readonly_static("kAScaleY", &SkMatrix::kAScaleY)
    .def_readonly_static("kATransX", &SkMatrix::kATransX)
    .def_readonly_static("kATransY", &SkMatrix::kATransY)
    ;

// M44
py::class_<SkM44>(m, "M44", R"docstring(
    4x4 matrix used by SkCanvas and other parts of Skia.

    Skia assumes a right-handed coordinate system: +X goes to the right +Y goes
    down +Z goes into the screen (away from the viewer)
    )docstring")
    .def(py::init<>())
    ;

// RSXform
py::class_<SkRSXform>(m, "RSXform", R"docstring(
    A compressed form of a rotation+scale matrix.

    [ fSCos -fSSin fTx ] [ fSSin fSCos fTy ] [ 0 0 1 ]
    )docstring")
    .def(py::init(&SkRSXform::Make),
        py::arg("scos"), py::arg("ssin"), py::arg("tx"), py::arg("ty"))
    .def_static("Make",
        &SkRSXform::Make,
        py::arg("scos"), py::arg("ssin"), py::arg("tx"), py::arg("ty"))
    .def_static("MakeFromRadians", &SkRSXform::MakeFromRadians,
        py::arg("scale"), py::arg("radians"), py::arg("tx"), py::arg("ty"),
        py::arg("ax"), py::arg("ay"))
    .def("rectStaysRect", &SkRSXform::rectStaysRect)
    .def("setIdentity", &SkRSXform::setIdentity)
    .def("set", &SkRSXform::set,
        py::arg("scos"), py::arg("ssin"), py::arg("tx"), py::arg("ty"))
    .def("toQuad", py::overload_cast<SkScalar, SkScalar, SkPoint[4]>(
        &SkRSXform::toQuad, py::const_),
        py::arg("width"), py::arg("height"), py::arg("quqd"))
    .def("toQuad", py::overload_cast<const SkSize&, SkPoint[4]>(
        &SkRSXform::toQuad, py::const_),
        py::arg("size"), py::arg("quqd"))
    .def("toTriStrip", &SkRSXform::toTriStrip,
        py::arg("width"), py::arg("height"), py::arg("strip"))
    .def_readwrite("fSCos", &SkRSXform::fSCos)
    .def_readwrite("fSSin", &SkRSXform::fSSin)
    .def_readwrite("fTx", &SkRSXform::fTx)
    .def_readwrite("fTy", &SkRSXform::fTy)
    ;
}
