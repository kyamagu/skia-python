#include "common.h"

void initGrContext_gl(py::module &m) {

py::enum_<GrGLFormat>(m, "GrGLFormat")
    .value("kUnknown", GrGLFormat::kUnknown)
    .value("kRGBA8", GrGLFormat::kRGBA8)
    .value("kR8", GrGLFormat::kR8)
    .value("kALPHA8", GrGLFormat::kALPHA8)
    .value("kLUMINANCE8", GrGLFormat::kLUMINANCE8)
    .value("kBGRA8", GrGLFormat::kBGRA8)
    .value("kRGB565", GrGLFormat::kRGB565)
    .value("kRGBA16F", GrGLFormat::kRGBA16F)
    .value("kR16F", GrGLFormat::kR16F)
    .value("kRGB8", GrGLFormat::kRGB8)
    .value("kRG8", GrGLFormat::kRG8)
    .value("kRGB10_A2", GrGLFormat::kRGB10_A2)
    .value("kRGBA4", GrGLFormat::kRGBA4)
    .value("kSRGB8_ALPHA8", GrGLFormat::kSRGB8_ALPHA8)
    .value("kCOMPRESSED_ETC1_RGB8", GrGLFormat::kCOMPRESSED_ETC1_RGB8)
    .value("kCOMPRESSED_RGB8_ETC2", GrGLFormat::kCOMPRESSED_RGB8_ETC2)
    .value("kCOMPRESSED_RGB8_BC1", GrGLFormat::kCOMPRESSED_RGB8_BC1)
    .value("kCOMPRESSED_RGBA8_BC1", GrGLFormat::kCOMPRESSED_RGBA8_BC1)
    .value("kR16", GrGLFormat::kR16)
    .value("kRG16", GrGLFormat::kRG16)
    .value("kRGBA16", GrGLFormat::kRGBA16)
    .value("kRG16F", GrGLFormat::kRG16F)
    .value("kLUMINANCE16F", GrGLFormat::kLUMINANCE16F)
    .value("kLast", GrGLFormat::kLast)
    .export_values();

py::class_<GrGLTextureInfo>(m, "GrGLTextureInfo")
    .def(py::init<>())
    .def(py::init<GrGLenum, GrGLuint, GrGLenum>(),
        py::arg("target"), py::arg("id"), py::arg("format") = 0)
    .def_readwrite("fTarget", &GrGLTextureInfo::fTarget)
    .def_readwrite("fID", &GrGLTextureInfo::fID)
    .def_readwrite("fFormat", &GrGLTextureInfo::fFormat)
    .def("__eq__", &GrGLTextureInfo::operator==, py::is_operator())
    ;

py::class_<GrGLFramebufferInfo>(m, "GrGLFramebufferInfo")
    .def(py::init<>())
    .def(py::init<GrGLuint, GrGLenum>(),
        py::arg("FBOID"), py::arg("format") = 0)
    .def_readwrite("fFBOID", &GrGLFramebufferInfo::fFBOID)
    .def_readwrite("fFormat", &GrGLFramebufferInfo::fFormat)
    .def("__eq__", &GrGLFramebufferInfo::operator==, py::is_operator())
    ;

py::class_<GrGLInterface, sk_sp<GrGLInterface>, SkRefCnt>(
    m, "GrGLInterface")
    .def(py::init([] {
        sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
        if (!interface.get())
            throw std::runtime_error("null pointer exception.");
        const GrGLInterface* ptr = interface.release();
        return const_cast<GrGLInterface*>(ptr);
    }))
    .def("validate", &GrGLInterface::validate)
#ifdef __linux__
    .def_static("MakeEGL", &GrGLInterfaces::MakeEGL)
    .def_static("MakeGLX", &GrGLInterfaces::MakeGLX)
#endif
    ;

}
