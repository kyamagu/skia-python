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

}
