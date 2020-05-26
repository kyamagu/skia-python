#include "common.h"
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

template<typename T>
using NumPy = py::array_t<T, py::array::c_style | py::array::forcecast>;



void initCodec(py::module &m) {

	

	py::class_<SkCodec> codec(m, "Codec",
	R"docstring(
	:py:class:`Codec` Abstraction layer directly on top of an image codec.
	Allows decoding of multiframe images

	The original Skia SkCodec has a MakeFromData method but here its just a 
	constructor doing the same thing because Codec cant be explicitly constucted.

	.. autosummary::
		:nosignatures:

		~Codec.Result
		~Codec.SelectionPolicy
		~Codec.FrameInfo
		~Codec.Options
		~Codec.SelectionPolicy
		~Codec.EncodedOrigin
		~Codec.DisposalMethod
		~Codec.ScanlineOrder

	)docstring");

	py::class_<SkCodec::FrameInfo> FrameInfo(codec, "FrameInfo",
		R"docstring(
		Information about individual frames in a multi-framed image.
		)docstring");
	
	FrameInfo
		.def(py::init<>())
		.def_readwrite("fRequiredFrame",&SkCodec::FrameInfo::fRequiredFrame,
			R"docstring(
			The frame that this frame needs to be blended with
			)docstring")
		.def_readwrite("fDuration",&SkCodec::FrameInfo::fDuration,
			R"docstring(
			Number of milliseconds to show this frame.
			)docstring")
		.def_readwrite("fFullyReceived",&SkCodec::FrameInfo::fFullyReceived,
			R"docstring(
			Whether the end marker for this frame is contained in the stream.
			)docstring")
		.def_readwrite("fAlphaType",&SkCodec::FrameInfo::fAlphaType)
		.def_readwrite("fDisposalMethod",&SkCodec::FrameInfo::fDisposalMethod,
			R"docstring(
			How this frame should be modified before decoding the next one.
			)docstring");

	py::class_<SkCodec::Options> Options(codec, "Options",
		R"docstring(
		Additional options to pass to getPixels.
		)docstring");

	Options
		.def(py::init<>())
		.def_readwrite("fZeroInitialized",&SkCodec::Options::fZeroInitialized)
		.def_readwrite("fSubset",&SkCodec::Options::fSubset)
		.def_readwrite("fFrameIndex",&SkCodec::Options::fFrameIndex,
			R"docstring(
			The frame to decode.
			)docstring")
		.def_readwrite("fPriorFrame",&SkCodec::Options::fPriorFrame,
			R"docstring(
			If not kNoFrame, the dst already contains the prior frame at this index.
			)docstring");

	py::enum_<SkCodec::SelectionPolicy> SelectionPolicy(codec, "SelectionPolicy",
		R"docstring(
		For container formats that contain both still images and image sequences,
		instruct the decoder how the output should be selected.
		)docstring");
	SelectionPolicy
		.value("kPreferStillImage", SkCodec::SelectionPolicy::kPreferStillImage)
		.value("kPreferAnimation", SkCodec::SelectionPolicy::kPreferAnimation)
		.export_values();

	py::enum_<SkCodec::ZeroInitialized> ZeroInitialized(codec, "ZeroInitialized",
		R"docstring(
		Whether or not the memory passed to getPixels is zero initialized.
		)docstring");
	ZeroInitialized
		.value("kYes_ZeroInitialized", SkCodec::ZeroInitialized::kYes_ZeroInitialized)
		.value("kNo_ZeroInitialized", SkCodec::ZeroInitialized::kNo_ZeroInitialized)
		.export_values();

	py::enum_<SkCodec::Result> Result(codec, "Result",
		R"docstring(
		Error codes for various SkCodec methods.
		)docstring");
	Result
		.value("kSuccess", SkCodec::Result::kSuccess)
		.value("kIncompleteInput", SkCodec::Result::kIncompleteInput)
		.value("kErrorInInput", SkCodec::Result::kErrorInInput)
		.value("kInvalidConversion", SkCodec::Result::kInvalidConversion)
		.value("kInvalidScale", SkCodec::Result::kInvalidScale)
		.value("kInvalidParameters", SkCodec::Result::kInvalidParameters)
		.value("kInvalidInput", SkCodec::Result::kInvalidInput)
		.value("kCouldNotRewind", SkCodec::Result::kCouldNotRewind)
		.value("kInternalError", SkCodec::Result::kInternalError)
		.value("kUnimplemented", SkCodec::Result::kUnimplemented)
		.export_values();

	py::enum_<SkEncodedOrigin> EncodedOrigin(codec, "EncodedOrigin",
		R"docstring(
		Image orientation values from their encoded EXIF data
		These values match the orientation www.exif.org/Exif2-2.PDF.
		)docstring");
	EncodedOrigin
		.value("kTopLeft_SkEncodedOrigin", SkEncodedOrigin::kTopLeft_SkEncodedOrigin)
		.value("kTopRight_SkEncodedOrigin", SkEncodedOrigin::kTopRight_SkEncodedOrigin)
		.value("kBottomRight_SkEncodedOrigin", SkEncodedOrigin::kBottomRight_SkEncodedOrigin)
		.value("kBottomLeft_SkEncodedOrigin", SkEncodedOrigin::kBottomLeft_SkEncodedOrigin)
		.value("kLeftTop_SkEncodedOrigin", SkEncodedOrigin::kLeftTop_SkEncodedOrigin)
		.value("kRightTop_SkEncodedOrigin", SkEncodedOrigin::kRightTop_SkEncodedOrigin)
		.value("kRightBottom_SkEncodedOrigin", SkEncodedOrigin::kRightBottom_SkEncodedOrigin)
		.value("kLeftBottom_SkEncodedOrigin", SkEncodedOrigin::kLeftBottom_SkEncodedOrigin)
		.value("kDefault_SkEncodedOrigin", SkEncodedOrigin::kDefault_SkEncodedOrigin)
		.value("kLast_SkEncodedOrigin", SkEncodedOrigin::kLast_SkEncodedOrigin)
		.export_values();

	py::enum_<SkCodecAnimation::DisposalMethod> DisposalMethod(codec, "DisposalMethod",
		R"docstring(
		This specifies how the next frame is based on this frame.
		
		)docstring");
	DisposalMethod
		.value("kKeep", SkCodecAnimation::DisposalMethod::kKeep,
			R"docstring(
			The next frame should be drawn on top of this one.
			)docstring")
		.value("kRestoreBGColor", SkCodecAnimation::DisposalMethod::kRestoreBGColor,
			R"docstring(
			Similar to Keep, except the area inside this frame's rectangle
			should be cleared to the BackGround color (transparent) before
			drawing the next frame.
			)docstring")
		.value("kRestorePrevious", SkCodecAnimation::DisposalMethod::kRestorePrevious,
			R"docstring(
			The next frame should be drawn on top of the previous frame - i.e.
			disregarding this one.
			)docstring")
		.export_values();

	py::enum_<SkCodec::SkScanlineOrder> ScanlineOrder(codec, "ScanlineOrder",
		R"docstring(
		The order in which rows are output from the scanline decoder is not the
		same for all variations of all image types.  This explains the possible
		output row orderings.
		)docstring");
	ScanlineOrder
		.value("kKeep", SkCodec::SkScanlineOrder::kTopDown_SkScanlineOrder)
		.value("kRestoreBGColor", SkCodec::SkScanlineOrder::kBottomUp_SkScanlineOrder)
		.export_values();

	codec
		.def(py::init([] (py::buffer array) {
			auto info = array.request();
			size_t size = (info.ndim) ? info.strides[0] * info.shape[0] : 0;
			auto data = SkData::MakeWithCopy(info.ptr, size);

			return SkCodec::MakeFromData(std::move(data));
		}),R"docstring(
		If this stream represents an encoded image that we know how to decode,
	 	return an SkCodec that can decode it. Otherwise return None.
		)docstring")
		.def_static("ResultToString",&SkCodec::ResultToString,
			R"docstring(
			Returns a textual representation of :py:class:`Codec.Result`
			)docstring",
			py::arg("result"))
		.def("getFrameCount",&SkCodec::getFrameCount,
			R"docstring(
			Return the number of frames in the image.
			)docstring")
		.def("getInfo",&SkCodec::getInfo,
			R"docstring(
			Return a reasonable SkImageInfo to decode into.
			)docstring")
		.def("dimensions",&SkCodec::dimensions,
			R"docstring(
			)docstring")
		.def("bounds",&SkCodec::bounds,
			R"docstring(
			)docstring")
		.def("getOrigin",&SkCodec::getOrigin,
			R"docstring(
			Returns the image orientation stored in the EXIF data.
	 		If there is no EXIF data, or if we cannot read the EXIF data, returns kTopLeft.

			)docstring")
		.def("getValidSubset",&SkCodec::getValidSubset,
			R"docstring(
			Return (via desiredSubset) a subset which can decoded from this codec,
	 		or false if this codec cannot decode subsets or anything similar to
	 		desiredSubset.

			)docstring",py::arg("desiredSubset"))
		.def("getScaledDimensions",&SkCodec::getScaledDimensions,R"docstring(
			Return a size that approximately supports the desired scale factor.

			
		)docstring",py::arg("desiredScale"))
		.def("getEncodedFormat",&SkCodec::getEncodedFormat,
			R"docstring(
			Format of the encoded data.
			
			)docstring")
		.def("getFrameInfo",py::overload_cast<int,SkCodec::FrameInfo*>(&SkCodec::getFrameInfo,py::const_),
			R"docstring(
			Returns frame information about a frame at a specific index

			)docstring",py::arg("index"),py::arg("info"))
		.def("getFrameInfo",py::overload_cast<>(&SkCodec::getFrameInfo),
			R"docstring(
			Returns info about all the frames in the image as a list
			On still images you get an empty list
			)docstring")
		.def("getRepetitionCount",&SkCodec::getRepetitionCount,
			R"docstring(
			Return the number of times to repeat, if this image is animated.
			It can return kRepetitionCountInfinite, a negative number, meaning that the animation
	 		should loop forever.
	 		For still (non-animated) image codecs, this will return 0.
	 		Example: repetition count of 4 means each frame is played 5 times and then stops
			)docstring")
		.def("getPixels",py::overload_cast<const SkImageInfo&,void*,size_t,const SkCodec::Options*>(&SkCodec::getPixels),
			R"docstring(
			Decode into the given pixels, a block of memory of size at
			least (info.fHeight - 1) * rowBytes + (info.fWidth *
			bytesPerPixel)

			Repeated calls to this function should give the same results.
			
			If this is the same as the initial image info it uses that for the result
			otherwise it preforms the appropriate convertion to Scale,ColorSpace
			If it cannot scale returns skia.Result.kInvalidScale
			If the ColorSpace maps to the ICC profile returned by getICCProfile(),
			the transformation is a no-op.
			If the ColorSpace is None, no transformation is done

			When called end scanline decoding requiring starScanlineDecode() ??

		)docstring",py::arg("info"),py::arg("pixels"),py::arg("rowBytes"),py::arg("options"))
		.def("getPixels",py::overload_cast<const SkImageInfo&,void*,size_t>(&SkCodec::getPixels),
			R"docstring(

			
			)docstring",py::arg("info"),py::arg("pixels"),py::arg("rowBytes"))
		.def("getPixels",py::overload_cast<const SkPixmap&,const SkCodec::Options*>(&SkCodec::getPixels),
			R"docstring(
			Simpler getPixels using a pixmap instead of 
			
			)docstring",py::arg("pixmap"),py::arg("options") = nullptr)
		.def("startIncrementalDecode",py::overload_cast<const SkImageInfo&,void*,size_t,const SkCodec::Options*>(&SkCodec::startIncrementalDecode),
			R"docstring(
			
			)docstring",
			py::arg("dstInfo"),py::arg("dst"),py::arg("rowBytes"),py::arg("options"))
		.def("startIncrementalDecode",py::overload_cast<const SkImageInfo&,void*,size_t>(&SkCodec::startIncrementalDecode),
			R"docstring(
			
			)docstring",
			py::arg("dstInfo"),py::arg("dst"),py::arg("rowBytes"))
		.def("startScanlineDecode",py::overload_cast<const SkImageInfo&>(&SkCodec::startScanlineDecode),
			R"docstring(

			)docstring")
		.def("startScanlineDecode",py::overload_cast<const SkImageInfo&,const SkCodec::Options*>(&SkCodec::startScanlineDecode),
			R"docstring(

			)docstring")
		.def("getScanlines",&SkCodec::getScanlines,
			R"docstring(

			)docstring",py::arg("dst"),py::arg("countLines"),py::arg("rowBytes"))
		.def("skipScanlines",&SkCodec::skipScanlines,
			R"docstring(
			)docstring",py::arg("countLines"))
		.def("getScanlineOrder",&SkCodec::getScanlineOrder,
			R"docstring(
			An enum representing the order in which scanlines will be returned by
			the scanline decoder.
			This is undefined before startScanlineDecode() is called.
			)docstring")
		.def("nextScanline",&SkCodec::nextScanline,
			R"docstring(
			Returns the y-coordinate of the next row to be returned by the scanline
			decoder.
			Results are undefined when not in scanline decoding mode.
			)docstring")
		.def("outputScanline",&SkCodec::outputScanline,
			R"docstring(
			Returns the output y-coordinate of the row that corresponds to an input
			y-coordinate.  The input y-coordinate represents where the scanline
			is located in the encoded data.
			This will equal inputScanline, except in the case of strangely
			encoded image types (bottom-up bmps, interlaced gifs).
			)docstring",py::arg("inputScanline"))
		.def_readonly_static("kRepetitionCountInfinite",&SkCodec::kRepetitionCountInfinite)
		.def_readonly_static("kNoFrame",&SkCodec::kNoFrame,
			R"docstring(
			Sentinel value to used to describe "no frame"
			used in :py::class:`skia.Codec.FrameInfo` and :py::class:`skia.Codec.Options`
			)docstring");

}
