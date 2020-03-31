#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>, true);

class PyPicture : public SkPicture {
public:
    void playback(
        SkCanvas *canvas, SkPicture::AbortCallback *callback=nullptr
        ) const override {
        PYBIND11_OVERLOAD_PURE(void, SkPicture, playback, canvas, callback);
    }
    SkRect cullRect() const override {
        PYBIND11_OVERLOAD_PURE(SkRect, SkPicture, cullRect);
    }
    int approximateOpCount() const override {
        PYBIND11_OVERLOAD_PURE(int, SkPicture, approximateOpCount);
    }
    size_t approximateBytesUsed() const override {
        PYBIND11_OVERLOAD_PURE(size_t, SkPicture, approximateBytesUsed);
    }
};

class PyBBoxHierarchy : public SkBBoxHierarchy {
public:
    void insert(const SkRect rects[], int N) override {
        PYBIND11_OVERLOAD_PURE(void, SkBBoxHierarchy, insert, rects, N);
    }
    void insert(
        const SkRect rects[], const SkBBoxHierarchy::Metadata m[],
        int N) override {
        PYBIND11_OVERLOAD(void, SkBBoxHierarchy, insert, rects, N);
    }
    void search(const SkRect& query, std::vector<int> *results) const override {
        PYBIND11_OVERLOAD_PURE(void, SkBBoxHierarchy, search, query, results);
    }
    size_t bytesUsed() const override {
        PYBIND11_OVERLOAD_PURE(size_t, SkBBoxHierarchy, bytesUsed);
    }
};


void initPicture(py::module &m) {
py::class_<SkPicture, PyPicture, sk_sp<SkPicture>>(m, "Picture")
    .def("playback", &SkPicture::playback,
        "Replays the drawing commands on the specified canvas.")
    .def("cullRect", &SkPicture::cullRect,
        "Returns cull SkRect for this picture, passed in when SkPicture was "
        "created.")
    .def("uniqueID", &SkPicture::uniqueID,
        "Returns a non-zero value unique among SkPicture in Skia process.")
    // .def("serialize",
    //     py::overload_cast<const SkSerialProcs*>(&SkPicture::serialize),
    //     "Returns storage containing SkData describing SkPicture, using "
    //     "optional custom encoders.")
    // .def("serialize",
    //     py::overload_cast<SkWStream*, const SkSerialProcs*>(
    //         &SkPicture::serialize),
    //     "Writes picture to stream, using optional custom encoders.")
    .def("approximateOpCount", &SkPicture::approximateOpCount,
        "Returns the approximate number of operations in SkPicture.")
    .def("approximateBytesUsed", &SkPicture::approximateBytesUsed,
        "Returns the approximate byte size of SkPicture.")
    .def("makeShader",
        py::overload_cast<SkTileMode, SkTileMode, const SkMatrix*,
            const SkRect*>(&SkPicture::makeShader, py::const_),
        "Return a new shader that will draw with this picture.")
    .def("makeShader",
        py::overload_cast<SkTileMode, SkTileMode, const SkMatrix*>(
            &SkPicture::makeShader, py::const_))
    .def("unique", &SkPicture::unique,
        "May return true if the caller is the only owner.")
    .def("ref", &SkPicture::ref,
        "Increment the reference count.")
    .def("unref", &SkPicture::unref,
        "Decrement the reference count.")
    // .def_static("MakeFromStream", &SkPicture::MakeFromStream,
    //     "Recreates SkPicture that was serialized into a stream.")
    // .def_static("MakeFromData",
    //     py::overload_cast<const SkData*, const SkDeserialProcs*>(
    //         &SkPicture::MakeFromData),
    //     "Recreates SkPicture that was serialized into data.")
    // .def_static("MakeFromData",
    //     py::overload_cast<const void*, size_t, const SkDeserialProcs*>(
    //         &SkPicture::MakeFromData))
    .def_static("MakePlaceholder", &SkPicture::MakePlaceholder,
        "Returns a placeholder SkPicture.")
    ;
py::class_<SkBBoxHierarchy, PyBBoxHierarchy, sk_sp<SkBBoxHierarchy>>
    bboxhierarchy(m, "BBoxHierarchy");
py::class_<SkBBoxHierarchy::Metadata>(bboxhierarchy, "Metadata")
    .def_readwrite("isDraw", &SkBBoxHierarchy::Metadata::isDraw);
bboxhierarchy
    .def(py::init())
    .def("insert",
        py::overload_cast<const SkRect[], int>(&SkBBoxHierarchy::insert),
        "Insert N bounding boxes into the hierarchy.")
    .def("insert",
        py::overload_cast<const SkRect[], const SkBBoxHierarchy::Metadata[],
            int>(&SkBBoxHierarchy::insert))
    .def("search", &SkBBoxHierarchy::search,
        "Populate results with the indices of bounding boxes intersecting "
        "that query.")
    .def("bytesUsed", &SkBBoxHierarchy::bytesUsed,
        "Return approximate size in memory of *this.")
    .def("unique", &SkBBoxHierarchy::unique,
        "May return true if the caller is the only owner.")
    .def("ref", &SkBBoxHierarchy::ref,
        "Increment the reference count.")
    .def("unref", &SkBBoxHierarchy::unref,
        "Decrement the reference count.")
    ;
py::class_<SkPictureRecorder> picturerecorder(m, "PictureRecorder");
py::enum_<SkPictureRecorder::RecordFlags>(picturerecorder, "RecordFlags")
    .value("kPlaybackDrawPicture_RecordFlag",
        SkPictureRecorder::RecordFlags::kPlaybackDrawPicture_RecordFlag)
    .export_values();
py::enum_<SkPictureRecorder::FinishFlags>(picturerecorder, "FinishFlags");
picturerecorder
    .def(py::init())
    .def("beginRecording",
        py::overload_cast<const SkRect&, sk_sp<SkBBoxHierarchy>, uint32_t>(
            &SkPictureRecorder::beginRecording),
        "Returns the canvas that records the drawing commands.")
    // .def("beginRecording",
    //     py::overload_cast<const SkRect&, SkBBHFactory*, uint32_t>(
    //         &SkPictureRecorder::beginRecording))
    // .def("beginRecording",
    //     py::overload_cast<SkScalar, SkScalar, SkBBHFactory*, uint32_t>(
    //         &SkPictureRecorder::beginRecording))
    .def("getRecordingCanvas", &SkPictureRecorder::getRecordingCanvas,
        "Returns the recording canvas if one is active, or NULL if recording "
        "is not active.")
    .def("finishRecordingAsPicture",
        &SkPictureRecorder::finishRecordingAsPicture,
        "Signal that the caller is done recording.")
    .def("finishRecordingAsPictureWithCull",
        &SkPictureRecorder::finishRecordingAsPictureWithCull,
        "Signal that the caller is done recording, and update the cull rect "
        "to use for bounding box hierarchy (BBH) generation.")
    .def("finishRecordingAsDrawable",
        &SkPictureRecorder::finishRecordingAsDrawable,
        "Signal that the caller is done recording.")
    ;
}
