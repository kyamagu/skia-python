#include "common.h"

void initPathMeasure(py::module &m) {
py::class_<SkPathMeasure> path_measure(m, "PathMeasure", R"docstring(
    :py:class:`PathMeasure`
    )docstring");

py::enum_<SkPathMeasure::MatrixFlags>(path_measure, "GetPosAndTan")
    .value("kGetPosition_MatrixFlag",
        SkPathMeasure::MatrixFlags::kGetPosition_MatrixFlag,
        "get position")
    .value("kGetTangent_MatrixFlag",
        SkPathMeasure::MatrixFlags::kGetTangent_MatrixFlag,
        "get tangent")
    .value("kGetPosAndTan_MatrixFlag",
        SkPathMeasure::MatrixFlags::kGetPosAndTan_MatrixFlag,
        "get position and tangent")
    .export_values();

path_measure
    .def(py::init<>(),
        R"docstring(
        Initializes a PathMeasure without an associated Path
        )docstring")
    .def(py::init<const SkPath&, bool, SkScalar>(),
        R"docstring(
        Initialize the PathMeasure with the specified path.

        The parts of the path that are needed are copied, so the
        client is free to modify/delete the path after this call.

        resScale controls the precision of the measure. values > 1
        increase the precision (and possibly slow down the
        computation). )docstring",
        py::arg("path"), py::arg("forceClosed"),
        py::arg("resScale") = SK_Scalar1)
    .def("getLength", &SkPathMeasure::getLength,
        R"docstring(
        Return the total length of the current contour, or 0 if no
        path is associated.
        )docstring")
    .def("getMatrix",
        []  (SkPathMeasure& measure, SkScalar distance,
             SkPathMeasure::MatrixFlags flags)
         -> py::object {
            SkMatrix matrix;
            if (measure.getMatrix(distance, &matrix, flags)) {
                return py::cast(matrix);
            } else {
                return py::none();
            }
        },
        R"docstring(
        Pins distance to 0 <= distance <= getLength(), and then
        computes the corresponding matrix (by calling getPosTan).

        Returns false if there is no path, or a zero-length path was
        specified, in which case matrix is unchanged.

        :return: None if there is no path, or a zero-length path was
        specified. Otherwise returns a :py:class:`Matrix` containing
        the result.
        )docstring",
        py::arg("distance"),
        py::arg("flags") = SkPathMeasure::MatrixFlags::kGetPosAndTan_MatrixFlag)
    .def("getPosTan",
        []  (SkPathMeasure& measure, SkScalar distance) -> py::object {
            SkPoint position;
            SkVector tangent;
            if (measure.getPosTan(distance, &position, &tangent)) {
                return py::make_tuple(position, tangent);
            } else {
                return py::none();
            }
        },
        R"docstring(
        Pins distance to 0 <= distance <= getLength(), and then
        computes the corresponding position and tangent.

        :return: None if there is no path, or a zero-length path was
        specified. Otherwise returns a tuple containing the position
        :py:class:`Point` and tangent :py:class:`Vector`.
        )docstring",
        py::arg("distance"))
    .def("getSegment", &SkPathMeasure::getSegment,
        R"docstring(
        Given a start and stop distance, return in dst the intervening
        segment(s).

        :return: If the segment is zero-length, return false, else
        return true. startD and stopD are pinned to legal values
        (0..getLength()). If startD > stopD then return false (and
        leave dst untouched). Begin the segment with a moveTo if
        startWithMoveTo is true
        )docstring",
        py::arg("startD"), py::arg("stopD"), py::arg("dst"),
        py::arg("startWithMoveTo"))
    .def("isClosed", &SkPathMeasure::isClosed,
        R"docstring(
        :return: true if the current contour is closed()
        )docstring")
    .def("nextContour", &SkPathMeasure::nextContour,
        R"docstring(
        Move to the next contour in the path.

        :return: true if one exists, or false if we're done with the
        path.
        )docstring")
    .def("setPath", &SkPathMeasure::setPath,
        R"docstring(
        Reset the pathmeasure with the specified path.

        The parts of the path that are needed are copied, so the
        client is free to modify/delete the path after this call..
        )docstring")
    ;

}
