#include "common.h"
#include <pybind11/stl.h>

const int SkCodec::kNoFrame;
const int SkCodec::kRepetitionCountInfinite;

namespace {

std::unique_ptr<SkCodec> MakeFromData(py::buffer b) {
    auto info = b.request();
    size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
    auto codec = SkCodec::MakeFromData(SkData::MakeWithoutCopy(info.ptr, size));
    if (!codec.get())
        throw std::runtime_error("Failed to make codec");
    return codec;
}

void* GetBufferPtr(const SkImageInfo& info, py::buffer& data, size_t rowBytes) {
    auto buffer = data.request(true);
    size_t given = (buffer.ndim) ? buffer.shape[0] * buffer.strides[0] : 0;
    if (given < info.computeByteSize(rowBytes))
        throw std::runtime_error("Buffer is smaller than required.");
    return buffer.ptr;
}

}

void initCodec(py::module &m) {

py::enum_<SkEncodedOrigin>(m, "EncodedOrigin",
    R"docstring(
    Image orientation values from their encoded EXIF data
    These values match the orientation www.exif.org/Exif2-2.PDF.
    )docstring")
    .value("kTopLeft_EncodedOrigin",
        SkEncodedOrigin::kTopLeft_SkEncodedOrigin)
    .value("kTopRight_EncodedOrigin",
        SkEncodedOrigin::kTopRight_SkEncodedOrigin)
    .value("kBottomRight_EncodedOrigin",
        SkEncodedOrigin::kBottomRight_SkEncodedOrigin)
    .value("kBottomLeft_EncodedOrigin",
        SkEncodedOrigin::kBottomLeft_SkEncodedOrigin)
    .value("kLeftTop_EncodedOrigin",
        SkEncodedOrigin::kLeftTop_SkEncodedOrigin)
    .value("kRightTop_EncodedOrigin",
        SkEncodedOrigin::kRightTop_SkEncodedOrigin)
    .value("kRightBottom_EncodedOrigin",
        SkEncodedOrigin::kRightBottom_SkEncodedOrigin)
    .value("kLeftBottom_EncodedOrigin",
        SkEncodedOrigin::kLeftBottom_SkEncodedOrigin)
    .value("kDefault_EncodedOrigin",
        SkEncodedOrigin::kDefault_SkEncodedOrigin)
    .value("kLast_EncodedOrigin",
        SkEncodedOrigin::kLast_SkEncodedOrigin)
    .export_values();

m.def("EncodedOriginToMatrix", &SkEncodedOriginToMatrix,
    R"docstring(
    Given an encoded origin and the width and height of the source data, returns
    a matrix that transforms the source rectangle [0, 0, w, h] to a correctly
    oriented destination rectangle, with the upper left corner still at [0, 0].
    )docstring",
    py::arg("origin"), py::arg("w"), py::arg("h"));

py::class_<SkCodec> codec(m, "Codec",
    R"docstring(
    :py:class:`Codec` Abstraction layer directly on top of an image codec.
    )docstring");

py::enum_<SkCodec::Result>(codec, "Result",
    R"docstring(
    Error codes for various :py:class:`Codec` methods.
    )docstring")
    .value("kSuccess", SkCodec::Result::kSuccess,
        R"docstring(
        General return value for success.
        )docstring")
    .value("kIncompleteInput", SkCodec::Result::kIncompleteInput,
        R"docstring(
        The input is incomplete. A partial image was generated.
        )docstring")
    .value("kErrorInInput", SkCodec::Result::kErrorInInput,
        R"docstring(
        Like kIncompleteInput, except the input had an error.

        If returned from an incremental decode, decoding cannot continue, even
        with more data.
        )docstring")
    .value("kInvalidConversion", SkCodec::Result::kInvalidConversion,
        R"docstring(
        The generator cannot convert to match the request, ignoring dimensions.
        )docstring")
    .value("kInvalidScale", SkCodec::Result::kInvalidScale,
        R"docstring(
        The generator cannot scale to requested size.
        )docstring")
    .value("kInvalidParameters", SkCodec::Result::kInvalidParameters,
        R"docstring(
        Parameters (besides info) are invalid. e.g. NULL pixels, rowBytes too
        small, etc.
        )docstring")
    .value("kInvalidInput", SkCodec::Result::kInvalidInput,
        R"docstring(
        The input did not contain a valid image.
        )docstring")
    .value("kCouldNotRewind", SkCodec::Result::kCouldNotRewind,
        R"docstring(
        Fulfilling this request requires rewinding the input, which is not
        supported for this input.
        )docstring")
    .value("kInternalError", SkCodec::Result::kInternalError,
        R"docstring(
        An internal error, such as OOM.
        )docstring")
    .value("kUnimplemented", SkCodec::Result::kUnimplemented,
        R"docstring(
        This method is not implemented by this codec.
        )docstring")
    .export_values();

py::enum_<SkCodec::SelectionPolicy>(codec, "SelectionPolicy",
    R"docstring(
    For container formats that contain both still images and image sequences,
    instruct the decoder how the output should be selected.
    )docstring")
    .value("kPreferStillImage", SkCodec::SelectionPolicy::kPreferStillImage,
        R"docstring(
        If the container format contains both still images and image sequences,
        :py:class:`Codec` should choose one of the still images. This is the
        default.
        )docstring")
    .value("kPreferAnimation", SkCodec::SelectionPolicy::kPreferAnimation,
        R"docstring(
        If the container format contains both still images and image sequences,
        :py:class:`Codec` should choose one of the image sequences for
        animation.
        )docstring")
    .export_values();

py::enum_<SkCodec::ZeroInitialized>(codec, "ZeroInitialized",
    R"docstring(
    Whether or not the memory passed to getPixels is zero initialized.
    )docstring")
    .value("kYes_ZeroInitialized",
        SkCodec::ZeroInitialized::kYes_ZeroInitialized,
        R"docstring(
        The memory passed to getPixels is zero initialized. The
        :py:class:`Codec` may take advantage of this by skipping writing zeroes.
        )docstring")
    .value("kNo_ZeroInitialized",
        SkCodec::ZeroInitialized::kNo_ZeroInitialized,
        R"docstring(
        The memory passed to `getPixels` has not been initialized to zero,
        so the :py:class:`Codec` must write all zeroes to memory.

        This is the default. It will be used if no Options struct is used.
        )docstring")
    .export_values();

py::class_<SkCodec::FrameInfo>(codec, "FrameInfo",
    R"docstring(
    Information about individual frames in a multi-framed image.
    )docstring")
    .def(py::init<>())
    .def_readwrite("fRequiredFrame", &SkCodec::FrameInfo::fRequiredFrame,
        R"docstring(
        The frame that this frame needs to be blended with, or
        kNoFrame if this frame is independent (so it can be
        drawn over an uninitialized buffer).

        Note that this is the *earliest* frame that can be used
        for blending. Any frame from [fRequiredFrame, i) can be
        used, unless its fDisposalMethod is kRestorePrevious.
        )docstring")
    .def_readwrite("fDuration", &SkCodec::FrameInfo::fDuration,
        R"docstring(
        Number of milliseconds to show this frame.
        )docstring")
    .def_readwrite("fFullyReceived", &SkCodec::FrameInfo::fFullyReceived,
        R"docstring(
        Whether the end marker for this frame is contained in the stream.

        Note: this does not guarantee that an attempt to decode will be
        complete. There could be an error in the stream.
        )docstring")
    .def_readwrite("fAlphaType", &SkCodec::FrameInfo::fAlphaType,
        R"docstring(
        This is conservative; it will still return non-opaque if e.g. a
        color index-based frame has a color with alpha but does not use it.
        )docstring")
    .def_readwrite("fDisposalMethod", &SkCodec::FrameInfo::fDisposalMethod,
        R"docstring(
        How this frame should be modified before decoding the next one.
        )docstring");

py::class_<SkCodec::Options>(codec, "Options",
    R"docstring(
    Additional options to pass to getPixels.
    )docstring")
    .def(py::init<>())
    .def_readwrite("fZeroInitialized", &SkCodec::Options::fZeroInitialized)
    .def_readwrite("fSubset", &SkCodec::Options::fSubset,
        R"docstring(
        If not NULL, represents a subset of the original image to decode.
        Must be within the bounds returned by :py:meth:`~SkCodec.getInfo`.
        If the EncodedFormat is :py:attr:`EncodedImageFormat.kWEBP` (the only
        one which currently supports subsets), the top and left values must be
        even.

        In getPixels and incremental decode, we will attempt to decode the
        exact rectangular subset specified by fSubset.

        In a scanline decode, it does not make sense to specify a subset
        top or subset height, since the client already controls which rows
        to get and which rows to skip.  During scanline decodes, we will
        require that the subset top be zero and the subset height be equal
        to the full height.  We will, however, use the values of
        subset left and subset width to decode partial scanlines on calls
        to :py:meth:`~SkCodec.getScanlines`.
        )docstring")
    .def_readwrite("fFrameIndex", &SkCodec::Options::fFrameIndex,
        R"docstring(
        The frame to decode.

        Only meaningful for multi-frame images.
        )docstring")
    .def_readwrite("fPriorFrame", &SkCodec::Options::fPriorFrame,
        R"docstring(
        If not kNoFrame, the dst already contains the prior frame at this index.

        Only meaningful for multi-frame images.

        If fFrameIndex needs to be blended with a prior frame (as reported by
        getFrameInfo[fFrameIndex].fRequiredFrame), the client can set this to
        any non-kRestorePrevious frame in [fRequiredFrame, fFrameIndex) to
        indicate that that frame is already in the dst. Options.fZeroInitialized
        is ignored in this case.

        If set to kNoFrame, the codec will decode any necessary required
        frame(s) first.
        )docstring");

py::enum_<SkCodec::SkScanlineOrder>(codec, "ScanlineOrder",
    R"docstring(
    The order in which rows are output from the scanline decoder is not the
    same for all variations of all image types.  This explains the possible
    output row orderings.
    )docstring")
    .value("kKeep", SkCodec::SkScanlineOrder::kTopDown_SkScanlineOrder,
        R"docstring(
        By far the most common, this indicates that the image can be decoded
        reliably using the scanline decoder, and that rows will be output in
        the logical order.
        )docstring")
    .value("kRestoreBGColor",
        SkCodec::SkScanlineOrder::kBottomUp_SkScanlineOrder,
        R"docstring(
        This indicates that the scanline decoder reliably outputs rows, but
        they will be returned in reverse order.  If the scanline format is
        kBottomUp, the nextScanline() API can be used to determine the actual
        y-coordinate of the next output row, but the client is not forced
        to take advantage of this, given that it's not too tough to keep
        track independently.

        For full image decodes, it is safe to get all of the scanlines at
        once, since the decoder will handle inverting the rows as it
        decodes.

        For subset decodes and sampling, it is simplest to get and skip
        scanlines one at a time, using the nextScanline() API.  It is
        possible to ask for larger chunks at a time, but this should be used
        with caution.  As with full image decodes, the decoder will handle
        inverting the requested rows, but rows will still be delivered
        starting from the bottom of the image.

        Upside down bmps are an example.
        )docstring")
    .export_values();

py::enum_<SkCodecAnimation::DisposalMethod>(codec, "DisposalMethod",
    R"docstring(
    This specifies how the next frame is based on this frame.

    Names are based on the GIF 89a spec.

    The numbers correspond to values in a GIF.
    )docstring")
    .value("kKeep", SkCodecAnimation::DisposalMethod::kKeep,
        R"docstring(
        The next frame should be drawn on top of this one.

        In a GIF, a value of 0 (not specified) is also treated as Keep.
        )docstring")
    .value("kRestoreBGColor",
        SkCodecAnimation::DisposalMethod::kRestoreBGColor,
        R"docstring(
        Similar to Keep, except the area inside this frame's rectangle
        should be cleared to the BackGround color (transparent) before
        drawing the next frame.
        )docstring")
    .value("kRestorePrevious",
        SkCodecAnimation::DisposalMethod::kRestorePrevious,
        R"docstring(
        The next frame should be drawn on top of the previous frame - i.e.
        disregarding this one.

        In a GIF, a value of 4 is also treated as RestorePrevious.
        )docstring")
    .export_values();

codec
    .def(py::init(&MakeFromData),
        R"docstring(
        If this stream represents an encoded image that we know how to decode,
        return an :py:class:`Codec` that can decode it. Otherwise return None.
        )docstring",
        py::arg("data"))
    .def_static("ResultToString", &SkCodec::ResultToString,
        R"docstring(
        Returns a textual representation of :py:class:`Codec.Result`
        )docstring",
        py::arg("result"))
    /* .def_static("MakeFromStream",
        [] (std::unique_ptr<SkStream>,
            SkCodec::SelectionPolicy selectionPolicy) {

            SkCodec::Result result;
            auto codec = SkCodec::MakeFromStream(
                stream, result, nullptr, selectionPolicy);
            if (result != SkCodec::kSuccess)
                throw std::runtime_error(SkCodec::ResultToString(result));
            return codec;
        },
        R"docstring(
        If this stream represents an encoded image that we know how to decode,
        return an :py:class:`Codec` that can decode it. Otherwise return NULL.

        As stated above, this call must be able to peek or read
        MinBufferedBytesNeeded to determine the correct format, and then start
        reading from the beginning. First it will attempt to peek, and it
        assumes that if less than MinBufferedBytesNeeded bytes (but more than
        zero) are returned, this is because the stream is shorter than this,
        so falling back to reading would not provide more data. If peek()
        returns zero bytes, this call will instead attempt to read(). This
        will require that the stream can be rewind()ed.

        If Result is not NULL, it will be set to either kSuccess if an
        :py:class:`Codec` is returned or a reason for the failure if NULL is
        returned.

        If :py:class:`PngChunkReader` is not NULL, take a ref and pass it to
        libpng if the image is a png.

        If the :py:class:`PngChunkReader` is not NULL then:

        - If the image is not a PNG, the :py:class:`PngChunkReader` will be
            ignored.
        - If the image is a PNG, the :py:class:`PngChunkReader` will be reffed.
        - If the PNG has unknown chunks, the :py:class:`PngChunkReader` will be
            used to handle these chunks.  :py:class:`PngChunkReader` will be
            called to read any unknown chunk at any point during the creation of
            the codec or the decode.  Note that if :py:class:`PngChunkReader`
            fails to read a chunk, this could result in a failure to create the
            codec or a failure to decode the image.
        - If the PNG does not contain unknown chunks, the
            :py:class:`PngChunkReader` will not be used or modified.

        If NULL is returned, the stream is deleted immediately. Otherwise, the
        :py:class:`Codec` takes ownership of it, and will delete it when done
        with it.
        )docstring",
        py::arg("stream"),
        py::arg("selectionPolicy") = SelectionPolicy::kPreferStillImage) */
    .def_static("MakeFromData", &MakeFromData,
        R"docstring(
        If this data represents an encoded image that we know how to decode,
        return an :py:class:`Codec` that can decode it. Otherwise return NULL.
        )docstring",
        py::arg("data"))
    .def("getInfo", &SkCodec::getInfo,
        R"docstring(
        Return a reasonable :py:class:`ImageInfo` to decode into.

        If the image has an ICC profile that does not map to an
        :py:class:`ColorSpace`, the returned :py:class:`ImageInfo` will use
        SRGB.
        )docstring")
    .def("dimensions", &SkCodec::dimensions)
    .def("bounds", &SkCodec::bounds)
    // .def("getICCProfile", &SkCodec::getICCProfile,
    //  R"docstring(
    //  Return the ICC profile of the encoded data.
    //  )docstring")
    .def("getOrigin", &SkCodec::getOrigin,
        R"docstring(
        Returns the image orientation stored in the EXIF data.
        If there is no EXIF data, or if we cannot read the EXIF data, returns
        :py:attr:`~EncodedOrigin.kTopLeft`.
        )docstring")
    .def("getScaledDimensions", &SkCodec::getScaledDimensions,
        R"docstring(
        Return a size that approximately supports the desired scale factor.
        The codec may not be able to scale efficiently to the exact scale
        factor requested, so return a size that approximates that scale.
        The returned value is the codec's suggestion for the closest valid
        scale that it can natively support
        )docstring",
        py::arg("desiredScale"))
    .def("getValidSubset", &SkCodec::getValidSubset,
        R"docstring(
        Return (via `desiredSubset`) a subset which can decoded from this codec,
        or false if this codec cannot decode subsets or anything similar to
        `desiredSubset`.

        :param desiredSubset: In/out parameter. As input, a desired subset of
            the original bounds (as specified by getInfo). If true is returned,
            desiredSubset may have been modified to a subset which is
            supported. Although a particular change may have been made to
            desiredSubset to create something supported, it is possible other
            changes could result in a valid subset.
            If false is returned, desiredSubset's value is undefined.
        :return: true if this codec supports decoding `desiredSubset` (as
            returned, potentially modified)
        )docstring",
        py::arg("desiredSubset"))
    .def("getEncodedFormat", &SkCodec::getEncodedFormat,
        R"docstring(
        Format of the encoded data.
        )docstring")
    .def("getPixels",
        [] (SkCodec& codec, const SkImageInfo& info, py::buffer data,
            size_t rowBytes, const SkCodec::Options* options) {
            auto ptr = GetBufferPtr(info, data, rowBytes);
            return codec.getPixels(info, ptr, rowBytes, options);
        },
        R"docstring(
        Decode into the given pixels, a block of memory of size at
        least (info.fHeight - 1) * rowBytes + (info.fWidth *
        bytesPerPixel)

        Repeated calls to this function should give the same results,
        allowing the PixelRef to be immutable.

        If a scanline decode is in progress, scanline mode will end, requiring
        the client to call :py:meth:`startScanlineDecode` in order to return to
        decoding scanlines.

        :info: A description of the format (config, size)
            expected by the caller.  This can simply be identical
            to the info returned by getInfo().

            This contract also allows the caller to specify
            different output-configs, which the implementation can
            decide to support or not.

            A size that does not match getInfo() implies a request
            to scale. If the generator cannot perform this scale,
            it will return kInvalidScale.

            If the info contains a non-null SkColorSpace, the codec
            will perform the appropriate color space transformation.

            If the caller passes in the SkColorSpace that maps to the
            ICC profile reported by getICCProfile(), the color space
            transformation is a no-op.

            If the caller passes a null SkColorSpace, no color space
            transformation will be done.

        :return: Result kSuccess, or another value explaining the type of
            failure.
        )docstring",
        py::arg("info"), py::arg("data"), py::arg("rowBytes"),
        py::arg("options") = nullptr)
    .def("getPixels",
        py::overload_cast<const SkPixmap&, const SkCodec::Options*>(
            &SkCodec::getPixels),
        py::arg("pixmap"), py::arg("options") = nullptr)
    .def("queryYUVAInfo",
        [] (const SkCodec& codec,
            const SkYUVAPixmapInfo::SupportedDataTypes& supportedDataTypes)
        -> py::object {
            SkYUVAPixmapInfo yuvaPixmapInfo;
            if (codec.queryYUVAInfo(supportedDataTypes, &yuvaPixmapInfo))
                return py::cast(yuvaPixmapInfo);
            return py::none();
        },
        R"docstring(
        If decoding to YUV is supported, this returns true. Otherwise, this
        returns false;

        :param supportedDataTypes: Indicates the data type/planar config
            combinations that are supported by the caller. If the generator
            supports decoding to YUV(A), but not as a type in
            supportedDataTypes, this method returns false.
        :return: Output that specifies the planar configuration, subsampling,
            orientation, chroma siting, plane color types, and row bytes.
        )docstring",
        py::arg("supportedDataTypes"))
    .def("getYUVAPlanes", &SkCodec::getYUVAPlanes,
        R"docstring(
        Returns kSuccess, or another value explaining the type of failure.
        This always attempts to perform a full decode. To get the planar
        configuration without decoding use queryYUVAInfo().

        :param yuvaPixmaps: Contains preallocated pixmaps configured according
            to a successful call to :py:meth:`~queryYUVAInfo`.
        )docstring",
        py::arg("yuvaPixmaps"))
    .def("startIncrementalDecode",
        [] (SkCodec& codec, const SkImageInfo& info, py::buffer data,
            size_t rowBytes, const SkCodec::Options* options) {
            auto ptr = GetBufferPtr(info, data, rowBytes);
            return codec.startIncrementalDecode(info, ptr, rowBytes, options);
        },
        R"docstring(
        Prepare for an incremental decode with the specified options.

        This may require a rewind.

        If kIncompleteInput is returned, may be called again after more data has
        been provided to the source :py:class:`Stream`.

        :param dstInfo: Info of the destination. If the dimensions do not match
            those of getInfo, this implies a scale.
        :param dst: Memory to write to. Needs to be large enough to hold the
            subset, if present, or the full image as described in dstInfo.
        :param options: Contains decoding options, including if memory is zero
            initialized and whether to decode a subset.
        :return: Enum representing success or reason for failure.
        )docstring",
        py::arg("dstInfo"), py::arg("dst"), py::arg("rowBytes"),
        py::arg("options") = nullptr)
    .def("incrementalDecode", &SkCodec::incrementalDecode,
        R"docstring(
        Start/continue the incremental decode.

        Not valid to call before a call to :py:meth:`startIncrementalDecode`
        returns kSuccess.

        If kIncompleteInput is returned, may be called again after more data has
        been provided to the source SkStream.

        Unlike getPixels and getScanlines, this does not do any filling. This is
        left up to the caller, since they may be skipping lines or continuing
        the decode later. In the latter case, they may choose to initialize all
        lines first, or only initialize the remaining lines after the first
        call.

        :param rowsDecoded: Optional output variable returning the total number
            of lines initialized. Only meaningful if this method returns
            kIncompleteInput. Otherwise the implementation may not set it.
            Note that some implementations may have initialized this many rows,
            but not necessarily finished those rows (e.g. interlaced PNG). This
            may be useful for determining what rows the client needs to
            initialize.
        :return: kSuccess if all lines requested in startIncrementalDecode have
            been completely decoded. kIncompleteInput otherwise.
        )docstring",
        py::arg("rowsDecoded") = nullptr)
    .def("startScanlineDecode",
        py::overload_cast<const SkImageInfo&, const SkCodec::Options*>(
            &SkCodec::startScanlineDecode),
        R"docstring(
        Prepare for a scanline decode with the specified options.

        After this call, this class will be ready to decode the first scanline.

        This must be called in order to call :py:meth:`getScanlines` or
        :py:meth:`skipScanlines`.

        This may require rewinding the stream.

        Not all Codecs support this.

        :param dstInfo: Info of the destination. If the dimensions do not match
            those of getInfo, this implies a scale.
        :param options: Contains decoding options, including if memory is zero
            initialized.
        :return: Enum representing success or reason for failure.
        )docstring",
        py::arg("dstInfo"), py::arg("options") = nullptr)
    .def("getScanlines",
        [] (SkCodec& codec, py::buffer data, int countLines, size_t rowBytes) {
            auto info = data.request();
            size_t given = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
            if (given < countLines * rowBytes)
                throw std::runtime_error("Buffer is smaller than required.");
            return codec.getScanlines(info.ptr, countLines, rowBytes);
        },
        R"docstring(
        Write the next countLines scanlines into dst.

        Not valid to call before calling startScanlineDecode().

        :param dst: Must be non-null, and large enough to hold countLines
            scanlines of size rowBytes.
        :param countLines: Number of lines to write.
        :param rowBytes: Number of bytes per row. Must be large enough to hold
            a scanline based on the :py:class:`ImageInfo` used to create this
            object.
        :return: the number of lines successfully decoded.  If this value is
            less than countLines, this will fill the remaining lines with a
            default value.
        )docstring",
        py::arg("dst").none(false), py::arg("countLines"), py::arg("rowBytes"))
    .def("skipScanlines", &SkCodec::skipScanlines,
        R"docstring(
        Skip count scanlines.

        Not valid to call before calling startScanlineDecode().

        The default version just calls onGetScanlines and discards the dst.
        NOTE: If skipped lines are the only lines with alpha, this default
        will make reallyHasAlpha return true, when it could have returned
        false.

        :return: true if the scanlines were successfully skipped
            false on failure, possible reasons for failure include:

            - An incomplete input image stream.
            - Calling this function before calling startScanlineDecode().
            - If countLines is less than zero or so large that it moves
                the current scanline past the end of the image.
        )docstring",
        py::arg("countLines"))
    .def("getScanlineOrder", &SkCodec::getScanlineOrder,
        R"docstring(
        An enum representing the order in which scanlines will be returned by
        the scanline decoder.

        This is undefined before :py:meth:`startScanlineDecode` is called.
        )docstring")
    .def("nextScanline", &SkCodec::nextScanline,
        R"docstring(
        Returns the y-coordinate of the next row to be returned by the scanline
        decoder.

        This will equal fCurrScanline, except in the case of strangely
        encoded image types (bottom-up bmps).

        Results are undefined when not in scanline decoding mode.
        )docstring")
    .def("outputScanline", &SkCodec::outputScanline,
        R"docstring(
        Returns the output y-coordinate of the row that corresponds to an input
        y-coordinate.  The input y-coordinate represents where the scanline
        is located in the encoded data.

        This will equal inputScanline, except in the case of strangely
        encoded image types (bottom-up bmps, interlaced gifs).
        )docstring", py::arg("inputScanline"))
    .def("getFrameCount", &SkCodec::getFrameCount,
        R"docstring(
        Return the number of frames in the image.

        May require reading through the stream.
        )docstring")
    .def("getFrameInfo",
        py::overload_cast<int, SkCodec::FrameInfo*>(
            &SkCodec::getFrameInfo, py::const_),
        R"docstring(
        Return info about a single frame.

        Only supported by multi-frame images. Does not read through the stream,
        so it should be called after :py:meth:`getFrameCount` to parse any
        frames that have not already been parsed.
        )docstring",
        py::arg("index"), py::arg("info"))
    .def("getFrameInfo",
        py::overload_cast<>(&SkCodec::getFrameInfo),
        R"docstring(
        Return info about all the frames in the image.

        May require reading through the stream to determine info about the
        frames (including the count).

        As such, future decoding calls may require a rewind.

        For still (non-animated) image codecs, this will return an empty vector.
        )docstring")
    .def("getRepetitionCount", &SkCodec::getRepetitionCount,
        R"docstring(
        Return the number of times to repeat, if this image is animated. This
        number does not　include the first play through of each frame. For
        example, a repetition count of 4 means　that each frame is played 5
        times and then the animation stops.
     　
        It can return kRepetitionCountInfinite, a negative number, meaning that
        the animation　should loop forever.
     　
        May require reading the stream to find the repetition count.
     　
        As such, future decoding calls may require a rewind.
     　
        For still (non-animated) image codecs, this will return 0.
        )docstring")
    .def_static("MinBufferedBytesNeeded", &SkCodec::MinBufferedBytesNeeded)
    .def_readonly_static("kNoFrame", &SkCodec::kNoFrame)
    .def_readonly_static("kRepetitionCountInfinite",
        &SkCodec::kRepetitionCountInfinite)
    ;
}
