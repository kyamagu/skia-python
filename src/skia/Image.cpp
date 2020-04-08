#include "common.h"

void initImage(py::module &m) {
py::enum_<SkFilterQuality>(m, "FilterQuality", R"docstring(
    Controls how much filtering to be done when scaling/transforming complex
    colors e.g. image.
    )docstring")
    .value("kNone", SkFilterQuality::kNone_SkFilterQuality,
        "fastest but lowest quality, typically nearest-neighbor")
    .value("kLow", SkFilterQuality::kLow_SkFilterQuality,
        "typically bilerp")
    .value("kMedium", SkFilterQuality::kMedium_SkFilterQuality,
        "typically bilerp + mipmaps for down-scaling")
    .value("kHigh", SkFilterQuality::kHigh_SkFilterQuality,
        "slowest but highest quality, typically bicubic or better")
    .value("kLast", SkFilterQuality::kLast_SkFilterQuality)
    .export_values();

py::enum_<SkTileMode>(m, "TileMode")
    .value("kClamp", SkTileMode::kClamp,
        "Replicate the edge color if the shader draws outside of its original "
        "bounds.")
    .value("kRepeat", SkTileMode::kRepeat,
        "Repeat the shader's image horizontally and vertically.")
    .value("kMirror", SkTileMode::kMirror,
        "Repeat the shader's image horizontally and vertically, alternating "
        "mirror images so that adjacent images always seam.")
    .value("kDecal", SkTileMode::kDecal,
        "Only draw within the original domain, return transparent-black "
        "everywhere else.")
    .value("kLastTileMode", SkTileMode::kLastTileMode,
        "")
    .export_values();

py::enum_<SkEncodedImageFormat>(m, "EncodedImageFormat", R"docstring(
    Enum describing format of encoded data.
    )docstring")
    .value("kBMP", SkEncodedImageFormat::kBMP)
    .value("kGIF", SkEncodedImageFormat::kGIF)
    .value("kICO", SkEncodedImageFormat::kICO)
    .value("kJPEG", SkEncodedImageFormat::kJPEG)
    .value("kPNG", SkEncodedImageFormat::kPNG)
    .value("kWBMP", SkEncodedImageFormat::kWBMP)
    .value("kWEBP", SkEncodedImageFormat::kWEBP)
    .value("kPKM", SkEncodedImageFormat::kPKM)
    .value("kKTX", SkEncodedImageFormat::kKTX)
    .value("kASTC", SkEncodedImageFormat::kASTC)
    .value("kDNG", SkEncodedImageFormat::kDNG)
    .value("kHEIF", SkEncodedImageFormat::kHEIF)
    .export_values();

py::class_<SkImage, sk_sp<SkImage>> image(m, "Image", R"docstring(
    SkImage describes a two dimensional array of pixels to draw.

    The pixels may be decoded in a raster bitmap, encoded in a SkPicture or
    compressed data stream, or located in GPU memory as a GPU texture.

    SkImage cannot be modified after it is created. SkImage may allocate
    additional storage as needed; for instance, an encoded SkImage may decode
    when drawn.

    SkImage width and height are greater than zero. Creating an SkImage with
    zero width or height returns SkImage equal to nullptr.

    SkImage may be created from SkBitmap, SkPixmap, SkSurface, SkPicture,
    encoded streams, GPU texture, YUV_ColorSpace data, or hardware buffer.
    Encoded streams supported include BMP, GIF, HEIF, ICO, JPEG, PNG, WBMP,
    WebP. Supported encoding details vary with platform.
    )docstring");

py::enum_<SkImage::CompressionType>(image, "CompressionType")
    .value("kNone", SkImage::CompressionType::kNone)
    .value("kETC2_RGB8_UNORM", SkImage::CompressionType::kETC2_RGB8_UNORM)
    .value("kBC1_RGB8_UNORM", SkImage::CompressionType::kBC1_RGB8_UNORM)
    .value("kBC1_RGBA8_UNORM", SkImage::CompressionType::kBC1_RGBA8_UNORM)
    .value("kLast", SkImage::CompressionType::kLast)
    .export_values();

py::enum_<SkImage::BitDepth>(image, "BitDepth")
    .value("kU8", SkImage::BitDepth::kU8)
    .value("kF16", SkImage::BitDepth::kF16)
    .export_values();

py::enum_<SkImage::CachingHint>(image, "CachingHint")
    .value("kAllow_CachingHint", SkImage::CachingHint::kAllow_CachingHint)
    .value("kDisallow_CachingHint", SkImage::CachingHint::kDisallow_CachingHint)
    .export_values();

image
    .def("imageInfo", &SkImage::imageInfo,
        "Returns a SkImageInfo describing the width, height, color type, alpha "
        "type, and color space of the SkImage.")
    .def("width", &SkImage::width, "Returns pixel count in each row.")
    .def("height", &SkImage::height, "Returns pixel row count.")
    .def("dimensions", &SkImage::dimensions,
        "Returns SkISize { width(), height() }.")
    .def("bounds", &SkImage::bounds,
        "Returns SkIRect { 0, 0, width(), height() }.")
    .def("uniqueID", &SkImage::uniqueID, "Returns value unique to image.")
    .def("alphaType", &SkImage::alphaType, "Returns SkAlphaType.")
    .def("colorType", &SkImage::colorType,
        "Returns SkColorType if known; otherwise, returns "
        "kUnknown_SkColorType.")
    .def("colorSpace", &SkImage::colorSpace,
        "Returns SkColorSpace, the range of colors, associated with SkImage.",
        py::return_value_policy::reference)
    .def("refColorSpace", &SkImage::refColorSpace,
        "Returns a smart pointer to SkColorSpace, the range of colors, "
        "associated with SkImage.")
    .def("isAlphaOnly", &SkImage::isAlphaOnly,
        "Returns true if SkImage pixels represent transparency only.")
    .def("isOpaque", &SkImage::isOpaque,
        "Returns true if pixels ignore their alpha value and are treated as "
        "fully opaque.")
    .def("makeShader",
        py::overload_cast<SkTileMode, SkTileMode, const SkMatrix*>(
            &SkImage::makeShader, py::const_),
        "Creates SkShader from SkImage.")
    .def("makeShader",
        py::overload_cast<SkTileMode, SkTileMode, const SkMatrix&>(
            &SkImage::makeShader, py::const_))
    .def("makeShader",
        py::overload_cast<const SkMatrix*>(&SkImage::makeShader, py::const_))
    .def("makeShader",
        py::overload_cast<const SkMatrix&>(&SkImage::makeShader, py::const_))
    .def("peekPixels", &SkImage::peekPixels,
        "Copies SkImage pixel address, row bytes, and SkImageInfo to pixmap, "
        "if address is available, and returns true.")
    .def("isTextureBacked", &SkImage::isTextureBacked,
        "Returns true the contents of SkImage was created on or uploaded to "
        "GPU memory, and is available as a GPU texture.")
    .def("isValid", &SkImage::isValid,
        "Returns true if SkImage can be drawn on either raster surface or GPU "
        "surface.")
    // .def("flush",
    //     py::overload_cast<GrContext*, const GrFlushInfo&>(&SkImage::flush),
    //     "Flushes any pending uses of texture-backed images in the GPU "
    //     "backend.")
    .def("flush",
        py::overload_cast<GrContext*>(&SkImage::flush),
        "Version of flush() that uses a default GrFlushInfo.")
    // .def("getBackendTexture", &SkImage::getBackendTexture,
    //     "Retrieves the back-end texture.")
    .def("readPixels",
        (bool (SkImage::*)(const SkImageInfo&, void*, size_t, int, int,
            SkImage::CachingHint) const) &SkImage::readPixels,
        "Copies SkRect of pixels from SkImage to dstPixels.",
        py::arg("dstInfo"), py::arg("dstPixels"), py::arg("dstRowBytes"),
        py::arg("srcX"), py::arg("srcY"), py::arg("cachingHint") =
            SkImage::CachingHint::kAllow_CachingHint)
    .def("readPixels",
        (bool (SkImage::*)(const SkPixmap&, int, int, SkImage::CachingHint)
            const) &SkImage::readPixels,
        "Copies a SkRect of pixels from SkImage to dst.",
        py::arg("dst"), py::arg("srcX"), py::arg("srcY"),
        py::arg("cachingHint") = SkImage::CachingHint::kAllow_CachingHint)
    .def("scalePixels", &SkImage::scalePixels,
        "Copies SkImage to dst, scaling pixels to fit dst.width() and "
        "dst.height(), and converting pixels to match dst.colorType() and "
        "dst.alphaType().")
    .def("encodeToData",
        (sk_sp<SkData> (SkImage::*)(SkEncodedImageFormat, int) const)
        &SkImage::encodeToData,
        "Encodes SkImage pixels, returning result as SkData.")
    .def("encodeToData",
        (sk_sp<SkData> (SkImage::*)() const) &SkImage::encodeToData,
        "Encodes SkImage pixels, returning result as SkData.")
    .def("refEncodedData", &SkImage::refEncodedData,
        "Returns encoded SkImage pixels as SkData, if SkImage was created from "
        "supported encoded stream format.")
    .def("makeSubset", &SkImage::makeSubset, "Returns subset of SkImage.")
    // .def("makeTextureImage", &SkImage::makeTextureImage,
    //     "Returns SkImage backed by GPU texture associated with context.")
    .def("makeNonTextureImage", &SkImage::makeNonTextureImage,
        "Returns raster image or lazy image.")
    .def("makeRasterImage", &SkImage::makeRasterImage, "Returns raster image.")
    // .def("makeWithFilter",
    //     (sk_sp<SkImage> (SkImage::*)(GrContext*, const SkImageFilter*,
    //         const SkIRect&, const SkIRect&, SkIRect*, SkIPoint*))
    //     &SkImage::makeWithFilter,
    //     "Creates filtered SkImage.")
    // .def("makeWithFilter",
    //     (sk_sp<SkImage> (SkImage::*)(const SkImageFilter*,
    //         const SkIRect&, const SkIRect&, SkIRect*, SkIPoint*))
    //     &SkImage::makeWithFilter,
    //     "To be deprecated.")
    .def("asLegacyBitmap", &SkImage::asLegacyBitmap,
        "Deprecated.")
    .def("isLazyGenerated", &SkImage::isLazyGenerated,
        "Returns true if SkImage is backed by an image-generator or other "
        "service that creates and caches its pixels or texture on-demand.")
    .def("makeColorSpace", &SkImage::makeColorSpace,
        "Creates SkImage in target SkColorSpace.")
    .def("makeColorTypeAndColorSpace", &SkImage::makeColorTypeAndColorSpace,
        "Experimental.")
    .def("reinterpretColorSpace", &SkImage::reinterpretColorSpace,
        "Creates a new SkImage identical to this one, but with a different "
        "SkColorSpace.")
    .def("unique", &SkImage::unique,
        "May return true if the caller is the only owner.")
    .def("ref", &SkImage::ref, "Increment the reference count.")
    .def("unref", &SkImage::unref, "Decrement the reference count.")
    .def_static("MakeRasterCopy", &SkImage::MakeRasterCopy,
        "Creates SkImage from SkPixmap and copy of pixels.")
    .def_static("MakeRasterData", &SkImage::MakeRasterData,
        "Creates SkImage from SkImageInfo, sharing pixels.")
    .def_static("MakeFromRaster", &SkImage::MakeFromRaster,
        "Creates SkImage from pixmap, sharing SkPixmap pixels.")
    .def_static("MakeFromBitmap", &SkImage::MakeFromBitmap,
        "Creates SkImage from bitmap, sharing or copying bitmap pixels.")
    // .def_static("MakeFromGenerator", &SkImage::MakeFromGenerator,
    //     "Creates SkImage from data returned by imageGenerator.")
    .def_static("MakeFromEncoded", &SkImage::MakeFromEncoded,
        "Return an image backed by the encoded data, but attempt to defer "
        "decoding until the image is actually used/drawn.")
    .def_static("DecodeToRaster",
        (sk_sp<SkImage> (*)(const void*, size_t, const SkIRect*))
        &SkImage::DecodeToRaster,
        "Decode the data in encoded/length into a raster image.",
        py::arg("encoded"), py::arg("length"), py::arg("subset") = nullptr)
    .def_static("DecodeToRaster",
        (sk_sp<SkImage> (*)(const sk_sp<SkData>&, const SkIRect*))
        &SkImage::DecodeToRaster,
        py::arg("data"), py::arg("subset") = nullptr)
    .def_static("DecodeToTexture",
        (sk_sp<SkImage> (*)(GrContext*, const void*, size_t, const SkIRect*))
        &SkImage::DecodeToTexture,
        "Decode the data in encoded/length into a texture-backed image.",
        py::arg("ctx"), py::arg("encoded"), py::arg("length"),
        py::arg("subset") = nullptr)
    .def_static("DecodeToTexture",
        (sk_sp<SkImage> (*)(GrContext*, const sk_sp<SkData>&, const SkIRect*))
        &SkImage::DecodeToTexture,
        py::arg("ctx"), py::arg("data"), py::arg("subset") = nullptr)
    // .def_static("MakeTextureFromCompressed",
    //     &SkImage::MakeTextureFromCompressed,
    //     "Creates a GPU-backed SkImage from compressed data.")
    // .def_static("MakeFromCompressed", &SkImage::MakeFromCompressed,
    //     "To be deprecated.")
    .def_static("MakeRasterFromCompressed", &SkImage::MakeRasterFromCompressed,
        "Creates a CPU-backed SkImage from compressed data.")
    .def_static("MakeFromTexture",
        (sk_sp<SkImage> (*)(GrContext*, const GrBackendTexture&,
            GrSurfaceOrigin, SkColorType, SkAlphaType, sk_sp<SkColorSpace>))
        &SkImage::MakeFromTexture,
        "Creates SkImage from GPU texture associated with context.")
    // .def_static("MakeFromTexture",
    //     (sk_sp<SkImage> (*)(GrContext*, const GrBackendTexture&,
    //         GrSurfaceOrigin, SkColorType, SkAlphaType, sk_sp<SkColorSpace>,
    //         TextureReleaseProc, ReleaseContext))
    //     &SkImage::MakeFromTexture,
    //     "Creates SkImage from GPU texture associated with context.")
    // .def_static("MakeFromCompressedTexture",
    //     &SkImage::MakeFromCompressedTexture,
    //     "Creates an SkImage from a GPU backend texture.")
    .def_static("MakeCrossContextFromPixmap",
        &SkImage::MakeCrossContextFromPixmap,
        "Creates SkImage from pixmap.")
    /*
    .def_static("MakeFromAdoptedTexture",
        &SkImage::MakeFromAdoptedTexture,
        "Creates SkImage from backendTexture associated with context.")
    .def_static("MakeFromYUVATexturesCopy", &SkImage::MakeFromYUVATexturesCopy,
        "Creates an SkImage by flattening the specified YUVA planes into a "
        "single, interleaved RGBA image.")
    .def_static("MakeFromYUVATexturesCopyWithExternalBackend",
        &SkImage::MakeFromYUVATexturesCopyWithExternalBackend,
        "Creates an SkImage by flattening the specified YUVA planes into a "
        "single, interleaved RGBA image.")
    .def_static("MakeFromYUVATextures", &SkImage::MakeFromYUVATextures,
        "Creates an SkImage by storing the specified YUVA planes into an image,"
        " to be rendered via multitexturing.")
    .def_static("MakeFromYUVAPixmaps", &SkImage::MakeFromYUVAPixmaps,
        "Creates SkImage from pixmap array representing YUVA data.")
    .def_static("MakeFromYUVTexturesCopy", &SkImage::MakeFromYUVTexturesCopy,
        "To be deprecated.")
    .def_static("MakeFromYUVTexturesCopyWithExternalBackend",
        &SkImage::MakeFromYUVTexturesCopyWithExternalBackend,
        "To be deprecated.")
    .def_static("MakeFromNV12TexturesCopy", &SkImage::MakeFromNV12TexturesCopy,
        "Creates SkImage from copy of nv12Textures, an array of textures on "
        "GPU.")
    .def_static("MakeFromNV12TexturesCopyWithExternalBackend",
        &SkImage::MakeFromNV12TexturesCopyWithExternalBackend,
        "Creates SkImage from copy of nv12Textures, an array of textures on "
        "GPU.")
    */
    .def_static("MakeFromPicture", &SkImage::MakeFromPicture,
        "Creates SkImage from picture.")
    // .def_static("MakeBackendTextureFromSkImage",
    //     &SkImage::MakeBackendTextureFromSkImage,
    //     "Creates a GrBackendTexture from the provided SkImage.")
    ;
}
