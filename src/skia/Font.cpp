#include "common.h"
#include <pybind11/stl.h>

void initFont(py::module &m) {
// FontStyle
py::class_<SkFontStyle> fontstyle(m, "FontStyle");

py::enum_<SkFontStyle::Weight>(fontstyle, "Weight")
    .value("kInvisible_Weight", SkFontStyle::Weight::kInvisible_Weight)
    .value("kThin_Weight", SkFontStyle::Weight::kThin_Weight)
    .value("kExtraLight_Weight", SkFontStyle::Weight::kExtraLight_Weight)
    .value("kLight_Weight", SkFontStyle::Weight::kLight_Weight)
    .value("kNormal_Weight", SkFontStyle::Weight::kNormal_Weight)
    .value("kMedium_Weight", SkFontStyle::Weight::kMedium_Weight)
    .value("kSemiBold_Weight", SkFontStyle::Weight::kSemiBold_Weight)
    .value("kBold_Weight", SkFontStyle::Weight::kBold_Weight)
    .value("kExtraBold_Weight", SkFontStyle::Weight::kExtraBold_Weight)
    .value("kBlack_Weight", SkFontStyle::Weight::kBlack_Weight)
    .value("kExtraBlack_Weight", SkFontStyle::Weight::kExtraBlack_Weight)
    .export_values();

py::enum_<SkFontStyle::Width>(fontstyle, "Width")
    .value("kUltraCondensed_Width", SkFontStyle::Width::kUltraCondensed_Width)
    .value("kExtraCondensed_Width", SkFontStyle::Width::kExtraCondensed_Width)
    .value("kCondensed_Width", SkFontStyle::Width::kCondensed_Width)
    .value("kSemiCondensed_Width", SkFontStyle::Width::kSemiCondensed_Width)
    .value("kNormal_Width", SkFontStyle::Width::kNormal_Width)
    .value("kSemiExpanded_Width", SkFontStyle::Width::kSemiExpanded_Width)
    .value("kExpanded_Width", SkFontStyle::Width::kExpanded_Width)
    .value("kExtraExpanded_Width", SkFontStyle::Width::kExtraExpanded_Width)
    .value("kUltraExpanded_Width", SkFontStyle::Width::kUltraExpanded_Width)
    .export_values();

py::enum_<SkFontStyle::Slant>(fontstyle, "Slant")
    .value("kUpright_Slant", SkFontStyle::Slant::kUpright_Slant)
    .value("kItalic_Slant", SkFontStyle::Slant::kItalic_Slant)
    .value("kOblique_Slant", SkFontStyle::Slant::kOblique_Slant)
    .export_values();

fontstyle
    .def(py::init<int, int, SkFontStyle::Slant>())
    .def(py::init<>())
    .def("weight", &SkFontStyle::weight)
    .def("width", &SkFontStyle::width)
    .def("slant", &SkFontStyle::slant)
    .def_static("Normal", &SkFontStyle::Normal)
    .def_static("Bold", &SkFontStyle::Bold)
    .def_static("Italic", &SkFontStyle::Italic)
    .def_static("BoldItalic", &SkFontStyle::BoldItalic)
    ;

// Typeface
py::class_<SkFontArguments> fontarguments(m, "FontArguments", R"docstring(
    Represents a set of actual arguments for a font.
    )docstring");

py::class_<SkFontArguments::Axis>(fontarguments, "Axis")
    .def_readwrite("fStyleValue", &SkFontArguments::Axis::fStyleValue)
    .def_readwrite("fTag", &SkFontArguments::Axis::fTag)
    ;

py::class_<SkFontArguments::VariationPosition>
    variationposition(fontarguments, "VariationPosition");

py::class_<SkFontArguments::VariationPosition::Coordinate>(
    variationposition, "Coordinate")
    .def_readwrite("axis",
        &SkFontArguments::VariationPosition::Coordinate::axis)
    .def_readwrite("value",
        &SkFontArguments::VariationPosition::Coordinate::value)
    ;

variationposition
    .def_readwrite("coordinates",
        &SkFontArguments::VariationPosition::coordinates,
        py::return_value_policy::reference)
    .def_readwrite("coordinateCount",
        &SkFontArguments::VariationPosition::coordinateCount)
    ;

fontarguments
    .def(py::init<>())
    .def("setCollectionIndex", &SkFontArguments::setCollectionIndex)
    .def("setAxes", &SkFontArguments::setAxes)
    .def("setVariationDesignPosition",
        &SkFontArguments::setVariationDesignPosition)
    .def("getCollectionIndex", &SkFontArguments::getCollectionIndex)
    .def("getAxes", &SkFontArguments::getAxes,
        py::return_value_policy::reference)
    .def("getVariationDesignPosition",
        &SkFontArguments::getVariationDesignPosition)
    ;

py::class_<SkTypeface, sk_sp<SkTypeface>> typeface(m, "Typeface", R"docstring(
    The SkTypeface class specifies the typeface and intrinsic style of a font.

    This is used in the paint, along with optionally algorithmic settings like
    textSize, textSkewX, textScaleX, kFakeBoldText_Mask, to specify how text
    appears when drawn (and measured).

    Typeface objects are immutable, and so they can be shared between threads.
    )docstring");

py::enum_<SkTypeface::SerializeBehavior>(typeface, "SerializeBehavior",
    R"docstring(
    A typeface can serialize just a descriptor (names, etc.), or it can also
    include the actual font data (which can be large).
    )docstring")
    .value("kDoIncludeData", SkTypeface::SerializeBehavior::kDoIncludeData)
    .value("kDontIncludeData", SkTypeface::SerializeBehavior::kDontIncludeData)
    .value("kIncludeDataIfLocal",
        SkTypeface::SerializeBehavior::kIncludeDataIfLocal)
    .export_values();

typeface
    .def("fontStyle", &SkTypeface::fontStyle,
        "Returns the typeface's intrinsic style attributes.")
    .def("isBold", &SkTypeface::isBold,
        "Returns true if style() has the kBold bit set.")
    .def("isItalic", &SkTypeface::isBold,
        "Returns true if style() has the kItalic bit set.")
    .def("isFixedPitch", &SkTypeface::isFixedPitch,
        "Returns true if the typeface claims to be fixed-pitch.")
    .def("getVariationDesignPosition", &SkTypeface::getVariationDesignPosition,
        "Copy into 'coordinates' (allocated by the caller) the design "
        "variation coordinates.")
    .def("getVariationDesignParameters",
        &SkTypeface::getVariationDesignParameters,
        "Copy into 'parameters' (allocated by the caller) the design "
        "variation parameters.")
    .def("uniqueID", &SkTypeface::uniqueID,
        "Return a 32bit value for this typeface, unique for the underlying "
        "font data.")
    .def("makeClone", &SkTypeface::makeClone,
        "Return a new typeface based on this typeface but parameterized as "
        "specified in the SkFontArguments.")
    // .def("serialize",
    //     (void (SkTypeface::*)(SkWStream*, SkTypeface::SerializeBehavior) const)
    //     &SkTypeface::serialize,
    //     "Write a unique signature to a stream, sufficient to reconstruct a "
    //     "typeface referencing the same font when Deserialize is called.",
    //     py::arg("stream"), py::arg("behavior") =
    //         SkTypeface::SerializeBehavior::kIncludeDataIfLocal)
    .def("serialize",
        (sk_sp<SkData> (SkTypeface::*)(SkTypeface::SerializeBehavior) const)
        &SkTypeface::serialize,
        "Same as serialize(SkWStream*, ...) but returns the serialized data in "
        "SkData, instead of writing it to a stream.",
        py::arg("behavior") =
            SkTypeface::SerializeBehavior::kIncludeDataIfLocal)
    .def("unicharsToGlyphs", &SkTypeface::unicharsToGlyphs,
        "Given an array of UTF32 character codes, return their corresponding "
        "glyph IDs.")
    .def("unicharToGlyph", &SkTypeface::unicharToGlyph,
        "Return the glyphID that corresponds to the specified unicode "
        "code-point (in UTF32 encoding).")
    .def("countGlyphs", &SkTypeface::countGlyphs,
        "Return the number of glyphs in the typeface.")
    .def("countTables", &SkTypeface::countTables,
        "Return the number of tables in the font.")
    .def("getTableTags", &SkTypeface::getTableTags,
        "Copy into tags[] (allocated by the caller) the list of table tags in "
        "the font, and return the number.")
    .def("getTableSize", &SkTypeface::getTableSize,
        "Given a table tag, return the size of its contents, or 0 if not "
        "present.")
    .def("getTableData", &SkTypeface::getTableData,
        "Given a table tag, return the size of its contents, or 0 if not "
        "present.")
    .def("copyTableData", &SkTypeface::copyTableData,
        "Return an immutable copy of the requested font table, or nullptr if "
        "that table was not found.")
    .def("getUnitsPerEm", &SkTypeface::getUnitsPerEm,
        "Return the units-per-em value for this typeface, or zero if there is "
        "an error.")
    .def("getKerningPairAdjustments", &SkTypeface::getKerningPairAdjustments,
        "Given a run of glyphs, return the associated horizontal adjustments.")
    // .def("createFamilyNameIterator", &SkTypeface::createFamilyNameIterator,
    //     "Returns an iterator which will attempt to enumerate all of the "
    //     "family names specified by the font.")
    // .def("getFamilyName", &SkTypeface::getFamilyName,
    //     "Return the family name for this typeface.")
    // .def("openStream", &SkTypeface::openStream,
    //     "Return a stream for the contents of the font data, or NULL on "
    //     "failure.")
    // .def("makeFontData", &SkTypeface::makeFontData,
    //     "Return the font data, or nullptr on failure.")
    // .def("createScalerContext", &SkTypeface::createScalerContext,
    //     "Return a scalercontext for the given descriptor.")
    .def("getBounds", &SkTypeface::getBounds,
        "Return a rectangle (scaled to 1-pt) that represents the union of the "
        "bounds of all of the glyphs, but each one positioned at (0,).")
    // .def("filterRec", &SkTypeface::filterRec)
    // .def("getFontDescriptor", &SkTypeface::getFontDescriptor)
    .def_static("UniqueID", &SkTypeface::UniqueID,
        "Return the uniqueID for the specified typeface.")
    .def_static("Equal", &SkTypeface::Equal,
        "Returns true if the two typefaces reference the same underlying font, "
        "handling either being null (treating null as the default font)")
    .def_static("MakeDefault", &SkTypeface::MakeDefault,
        "Returns the default normal typeface, which is never nullptr.")
    .def_static("MakeFromName", &SkTypeface::MakeFromName,
        "Creates a new reference to the typeface that most closely matches the "
        "requested familyName and fontStyle.")
    .def_static("MakeFromFile", &SkTypeface::MakeFromFile,
        "Return a new typeface given a file.")
    // .def_static("MakeFromStream", &SkTypeface::MakeFromStream,
    //     "Return a new typeface given a stream.")
    .def_static("MakeFromData", &SkTypeface::MakeFromData,
        "Return a new typeface given a SkData.")
    // .def_static("MakeFromFontData", &SkTypeface::MakeFromFontData,
    //     "Return a new typeface given font data and configuration.")
    // .def_static("MakeDeserialize", &SkTypeface::MakeDeserialize,
    //     "Given the data previously written by serialize(), return a new "
    //     "instance of a typeface referring to the same font.")
    ;

// FontMgr
py::class_<SkFontStyleSet, sk_sp<SkFontStyleSet>>(m, "FontStyleSet")
    .def("count", &SkFontStyleSet::count)
    .def("getStyle", &SkFontStyleSet::getStyle)
    .def("createTypeface", &SkFontStyleSet::createTypeface)
    .def("matchStyle", &SkFontStyleSet::matchStyle)
    .def("unique", &SkFontStyleSet::unique,
        R"docstring(
        May return true if the caller is the only owner.

        Ensures that all previous owner's actions are complete.
        )docstring")
    .def("ref", &SkFontStyleSet::ref,
        R"docstring(
        Increment the reference count.

        Must be balanced by a call to unref().
        )docstring")
    .def("unref", &SkFontStyleSet::unref,
        R"docstring(
        Decrement the reference count.

        If the reference count is 1 before the decrement, then delete the
        object. Note that if this is the case, then the object needs to have
        been allocated via new, and not on the stack.
        )docstring")
    .def_static("CreateEmpty", &SkFontStyleSet::CreateEmpty)
    ;

py::class_<SkFontMgr, sk_sp<SkFontMgr>>(m, "FontMgr")
    .def("countFamilies", &SkFontMgr::countFamilies)
    .def("getFamilyName",
        [] (const SkFontMgr& fontmgr, int index) {
            SkString familyName;
            fontmgr.getFamilyName(index, &familyName);
            return py::str(familyName.c_str(), familyName.size());
        },
        py::arg("index"))
    .def("createStyleSet",
        [] (const SkFontMgr& fontmgr, int index) {
            return sk_sp<SkFontStyleSet>(fontmgr.createStyleSet(index));
        },
        py::arg("index"))
    .def("matchFamily",
        [] (const SkFontMgr& fontmgr, const std::string* familyName) {
            return sk_sp<SkFontStyleSet>(
                fontmgr.matchFamily(
                    (familyName) ? familyName->c_str() : nullptr));
        },
        R"docstring(
        Never returns NULL; will return an empty set if the name is not found.

        Passing nullptr as the parameter will return the default system family.
        Note that most systems don't have a default system family, so passing
        nullptr will often result in the empty set.

        It is possible that this will return a style set not accessible from
        createStyleSet(int) due to hidden or auto-activated fonts.
        )docstring",
        py::arg("familyName"))
    .def("matchFamilyStyle",
        [] (const SkFontMgr& fontmgr, const std::string& familyName,
            const SkFontStyle& style) {
            return sk_sp<SkTypeface>(
                fontmgr.matchFamilyStyle(familyName.c_str(), style));
        },
        R"docstring(
        Find the closest matching typeface to the specified familyName and style
        and return a ref to it.

        Will return nullptr if no 'good' match is found.

        Passing nullptr as the parameter for familyName will return the
        default system font.

        It is possible that this will return a style set not accessible from
        createStyleSet(int) or matchFamily(const char[]) due to hidden or
        auto-activated fonts.
        )docstring",
        py::arg("familyName"), py::arg("style"))
    .def("matchFamilyStyleCharacter",
        [] (const SkFontMgr& fontmgr, const std::string& familyName,
            const SkFontStyle& style, const std::vector<std::string>& bcp47,
            SkUnichar character) {
            std::vector<const char*> bcp47_(bcp47.size());
            std::transform(bcp47.begin(), bcp47.end(), bcp47_.begin(),
                [] (const std::string& c) { return c.c_str(); });
            return sk_sp<SkTypeface>(
                fontmgr.matchFamilyStyleCharacter(
                    familyName.c_str(), style, &bcp47_[0], bcp47_.size(),
                    character));
        },
        R"docstring(
        Use the system fallback to find a typeface for the given character.

        Note that bcp47 is a combination of ISO 639, 15924, and 3166-1 codes, so
        it is fine to just pass a ISO 639 here.

        Will return NULL if no family can be found for the character in the
        system fallback.

        Passing nullptr as the parameter for familyName will return the
        default system font.

        bcp47[0] is the least significant fallback, bcp47[bcp47Count-1] is the
        most significant. If no specified bcp47 codes match, any font with the
        requested character will be matched.
        )docstring",
        py::arg("familyName"), py::arg("style"), py::arg("bcp47"),
        py::arg("character"))
    .def("matchFaceStyle",
        [] (const SkFontMgr& fontmgr, const SkTypeface* face,
            const SkFontStyle& style) {
            return sk_sp<SkTypeface>(fontmgr.matchFaceStyle(face, style));
        },
        py::arg("face"), py::arg("style"))
    .def("makeFromData", &SkFontMgr::makeFromData,
        R"docstring(
        Create a typeface for the specified data and TTC index (pass 0 for none)
        or NULL if the data is not recognized.
        )docstring",
        py::arg("data"), py::arg("ttcIndex") = 0)
    // .def("makeFromStream", &SkFontMgr::makeFromStream)
    // .def("makeFromStream", &SkFontMgr::makeFromStream)
    // .def("makeFromFontData", &SkFontMgr::makeFromFontData)
    .def("makeFromFile",
        [] (const SkFontMgr& fontmgr, const std::string& path, int ttcIndex) {
            return fontmgr.makeFromFile(path.c_str(), ttcIndex);
        },
        R"docstring(
        Create a typeface for the specified fileName and TTC index (pass 0 for
        none) or NULL if the file is not found, or its contents are not
        recognized.
        )docstring",
        py::arg("path"), py::arg("ttcIndex") = 0)
    .def("legacyMakeTypeface",
        [] (const SkFontMgr& fontmgr, const std::string& familyName,
            const SkFontStyle& style) {
            return fontmgr.legacyMakeTypeface(familyName.c_str(), style);
        },
        py::arg("familyName"), py::arg("style"))
    .def("unique", &SkFontMgr::unique,
        R"docstring(
        May return true if the caller is the only owner.

        Ensures that all previous owner's actions are complete.
        )docstring")
    .def("ref", &SkFontMgr::ref,
        R"docstring(
        Increment the reference count.

        Must be balanced by a call to unref().
        )docstring")
    .def("unref", &SkFontMgr::unref,
        R"docstring(
        Decrement the reference count.

        If the reference count is 1 before the decrement, then delete the
        object. Note that if this is the case, then the object needs to have
        been allocated via new, and not on the stack.
        )docstring")
    .def_static("RefDefault", &SkFontMgr::RefDefault,
        R"docstring(
        Return the default fontmgr.
        )docstring")
    ;

// Font
py::enum_<SkFontHinting>(m, "FontHinting")
    .value("kNone", SkFontHinting::kNone)
    .value("kSlight", SkFontHinting::kSlight)
    .value("kNormal", SkFontHinting::kNormal)
    .value("kFull", SkFontHinting::kFull)
    .export_values();

py::enum_<SkTextEncoding>(m, "TextEncoding")
    .value("kUTF8", SkTextEncoding::kUTF8)
    .value("kUTF16", SkTextEncoding::kUTF16)
    .value("kUTF32", SkTextEncoding::kUTF32)
    .value("kGlyphID", SkTextEncoding::kGlyphID)
    .export_values();

py::class_<SkFont> font(m, "Font", R"docstring(
    SkFont controls options applied when drawing and measuring text.
    )docstring");

py::enum_<SkFont::Edging>(font, "Edging", R"docstring(
    Whether edge pixels draw opaque or with partial transparency.
    )docstring")
    .value("kAlias", SkFont::Edging::kAlias)
    .value("kAntiAlias", SkFont::Edging::kAntiAlias)
    .value("kSubpixelAntiAlias", SkFont::Edging::kSubpixelAntiAlias)
    .export_values();

font
    .def(py::init<>(), "Constructs SkFont with default values.")
    .def(py::init<sk_sp<SkTypeface>, SkScalar>(),
        "Constructs SkFont with default values with SkTypeface and size in "
        "points.")
    .def(py::init<sk_sp<SkTypeface>>(),
        "Constructs SkFont with default values with SkTypeface.")
    .def(py::init<sk_sp<SkTypeface>, SkScalar, SkScalar, SkScalar>(),
        "Constructs SkFont with default values with SkTypeface and size in "
        "points, horizontal scale, and horizontal skew.")
    .def("__eq__", &SkFont::operator==,
        "Compares SkFont and font, and returns true if they are equivalent.",
        py::is_operator())
    .def("__ne__", &SkFont::operator!=,
        "Compares SkFont and font, and returns true if they are not "
        "equivalent.", py::is_operator())
    .def("isForceAutoHinting", &SkFont::isForceAutoHinting,
        "If true, instructs the font manager to always hint glyphs.")
    .def("isEmbeddedBitmaps", &SkFont::isEmbeddedBitmaps,
        "Returns true if font engine may return glyphs from font bitmaps "
        "instead of from outlines.")
    .def("isSubpixel", &SkFont::isSubpixel,
        "Returns true if glyphs may be drawn at sub-pixel offsets.")
    .def("isLinearMetrics", &SkFont::isLinearMetrics,
        "Returns true if font and glyph metrics are requested to be linearly "
        "scalable.")
    .def("isEmbolden", &SkFont::isEmbolden,
        "Returns true if glyphs may be drawn at sub-pixel offsets.")
    .def("isBaselineSnap", &SkFont::isBaselineSnap,
        "Returns true if baselines will be snapped to pixel positions when the "
        "current transformation matrix is axis aligned.")
    .def("setForceAutoHinting", &SkFont::setForceAutoHinting,
        "Sets whether to always hint glyphs.")
    .def("setEmbeddedBitmaps", &SkFont::setEmbeddedBitmaps,
        "Requests, but does not require, to use bitmaps in fonts instead of "
        "outlines.")
    .def("setSubpixel", &SkFont::setSubpixel,
        "Requests, but does not require, that glyphs respect sub-pixel "
        "positioning.")
    .def("setLinearMetrics", &SkFont::setLinearMetrics,
        "Requests, but does not require, linearly scalable font and glyph "
        "metrics.")
    .def("setEmbolden", &SkFont::setEmbolden,
        "Increases stroke width when creating glyph bitmaps to approximate a "
        "bold typeface.")
    .def("setBaselineSnap", &SkFont::setBaselineSnap,
        "Requests that baselines be snapped to pixels when the current "
        "transformation matrix is axis aligned.")
    .def("getEdging", &SkFont::getEdging,
        "Whether edge pixels draw opaque or with partial transparency.")
    .def("setEdging", &SkFont::setEdging,
        "Requests, but does not require, that edge pixels draw opaque or with "
        "partial transparency.")
    .def("setHinting", &SkFont::setHinting,
        "Sets level of glyph outline adjustment.")
    .def("getHinting", &SkFont::getHinting,
        "Returns level of glyph outline adjustment.")
    .def("makeWithSize", &SkFont::makeWithSize,
        "Returns a font with the same attributes of this font, but with the "
        "specified size.")
    .def("getTypeface", &SkFont::getTypeface,
        "Returns SkTypeface if set, or nullptr.",
        py::return_value_policy::reference)
    .def("getTypefaceOrDefault", &SkFont::getTypefaceOrDefault,
        "Returns SkTypeface if set, or the default typeface.",
        py::return_value_policy::reference)
    .def("getSize", &SkFont::getSize, "Returns text size in points.")
    .def("getScaleX", &SkFont::getScaleX, "Returns text scale on x-axis.")
    .def("getSkewX", &SkFont::getSkewX, "Returns text skew on x-axis.")
    .def("refTypeface", &SkFont::refTypeface,
        "Increases SkTypeface SkRefCnt by one.")
    .def("refTypefaceOrDefault", &SkFont::refTypefaceOrDefault,
        "Increases SkTypeface SkRefCnt by one.")
    .def("setTypeface", &SkFont::setTypeface,
        "Sets SkTypeface to typeface, decreasing SkRefCnt of the previous "
        "SkTypeface.")
    .def("setSize", &SkFont::setSize, "Sets text size in points.")
    .def("setScaleX", &SkFont::setScaleX, "Sets text scale on x-axis.")
    .def("setSkewX", &SkFont::setSkewX, " Sets text skew on x-axis.")
    .def("textToGlyphs", &SkFont::textToGlyphs,
        "Converts text into glyph indices.")
    .def("unicharToGlyph", &SkFont::unicharToGlyph,
        "Returns glyph index for Unicode character.")
    .def("unicharsToGlyphs", &SkFont::unicharsToGlyphs)
    .def("countText", &SkFont::countText,
        "Returns number of glyphs represented by text.")
    .def("measureText",
        (SkScalar (SkFont::*)(const void*, size_t, SkTextEncoding, SkRect*)
            const) &SkFont::measureText,
        "Returns the advance width of text.",
        py::arg("text"), py::arg("byteLength"), py::arg("encoding"),
        py::arg("bounds") = nullptr)
    .def("measureText",
        (SkScalar (SkFont::*)(const void*, size_t, SkTextEncoding, SkRect*,
            const SkPaint*) const) &SkFont::measureText,
        "Returns the advance width of text.")
    .def("getWidths",
        (void  (SkFont::*)(const SkGlyphID[], int, SkScalar[], SkRect[]) const)
        &SkFont::getWidths,
        "DEPRECATED Retrieves the advance and bounds for each glyph in glyphs.")
    .def("getWidths",
        (void  (SkFont::*)(const SkGlyphID[], int, SkScalar[], std::nullptr_t)
            const) &SkFont::getWidths)
    .def("getWidths",
        (void  (SkFont::*)(const SkGlyphID[], int, SkScalar[]) const)
        &SkFont::getWidths,
        "Retrieves the advance and bounds for each glyph in glyphs.")
    .def("getWidthsBounds", &SkFont::getWidthsBounds,
        "Retrieves the advance and bounds for each glyph in glyphs.")
    .def("getBounds", &SkFont::getBounds,
        "Retrieves the bounds for each glyph in glyphs.")
    .def("getPos", &SkFont::getPos,
        "Retrieves the positions for each glyph, beginning at the specified "
        "origin.")
    .def("getXPos", &SkFont::getXPos,
        "Retrieves the x-positions for each glyph, beginning at the specified "
        "origin.")
    .def("getPath", &SkFont::getPath,
        "Modifies path to be the outline of the glyph.")
    .def("getPaths", &SkFont::getPaths,
        "Returns path corresponding to glyph array.")
    .def("getMetrics", &SkFont::getMetrics,
        "Returns SkFontMetrics associated with SkTypeface.")
    .def("getSpacing", &SkFont::getSpacing,
        "Returns the recommended spacing between lines: the sum of metrics "
        "descent, ascent, and leading.")
    .def("dump", &SkFont::dump, "Dumps fields of the font to SkDebugf.")
    ;

py::class_<SkFontMetrics> fontmetrics(m, "FontMetrics", R"docstring(
    The metrics of an SkFont.

    The metric values are consistent with the Skia y-down coordinate system.
    )docstring");

py::enum_<SkFontMetrics::FontMetricsFlags>(fontmetrics, "FontMetricsFlags",
    R"docstring(
    FontMetricsFlags indicate when certain metrics are valid; the underline or
    strikeout metrics may be valid and zero.

    Fonts with embedded bitmaps may not have valid underline or strikeout
    metrics.
    )docstring")
    .value("kUnderlineThicknessIsValid_Flag",
        SkFontMetrics::FontMetricsFlags::kUnderlineThicknessIsValid_Flag,
        "set if fUnderlineThickness is valid")
    .value("kUnderlinePositionIsValid_Flag",
        SkFontMetrics::FontMetricsFlags::kUnderlinePositionIsValid_Flag,
        "set if fUnderlinePosition is valid")
    .value("kStrikeoutThicknessIsValid_Flag",
        SkFontMetrics::FontMetricsFlags::kStrikeoutThicknessIsValid_Flag,
        "set if fStrikeoutThickness is valid")
    .value("kStrikeoutPositionIsValid_Flag",
        SkFontMetrics::FontMetricsFlags::kStrikeoutPositionIsValid_Flag,
        "set if fStrikeoutPosition is valid")
    .export_values();

fontmetrics
    .def("hasUnderlineThickness", &SkFontMetrics::hasUnderlineThickness,
        "Returns true if SkFontMetrics has a valid underline thickness, and "
        "sets thickness to that value.")
    .def("hasUnderlinePosition", &SkFontMetrics::hasUnderlinePosition,
        "Returns true if SkFontMetrics has a valid underline position, and "
        "sets position to that value.")
    .def("hasStrikeoutThickness", &SkFontMetrics::hasStrikeoutThickness,
        "Returns true if SkFontMetrics has a valid strikeout thickness, and "
        "sets thickness to that value.")
    .def("hasStrikeoutPosition", &SkFontMetrics::hasStrikeoutPosition,
        "Returns true if SkFontMetrics has a valid strikeout position, and "
        "sets position to that value.")
    .def_readwrite("fFlags", &SkFontMetrics::fFlags,
        "FontMetricsFlags indicating which metrics are valid.")
    .def_readwrite("fTop", &SkFontMetrics::fTop,
        "greatest extent above origin of any glyph bounding box, typically "
        "negative; deprecated with variable fonts")
    .def_readwrite("fAscent", &SkFontMetrics::fAscent,
        "distance to reserve above baseline, typically negative")
    .def_readwrite("fDescent", &SkFontMetrics::fDescent,
        "distance to reserve below baseline, typically positive")
    .def_readwrite("fBottom", &SkFontMetrics::fBottom,
        "greatest extent below origin of any glyph bounding box, typically "
        "positive; deprecated with variable fonts")
    .def_readwrite("fLeading", &SkFontMetrics::fLeading,
        "distance to add between lines, typically positive or zero")
    .def_readwrite("fAvgCharWidth", &SkFontMetrics::fAvgCharWidth,
        "average character width, zero if unknown")
    .def_readwrite("fMaxCharWidth", &SkFontMetrics::fMaxCharWidth,
        "maximum character width, zero if unknown")
    .def_readwrite("fXMin", &SkFontMetrics::fXMin,
        "greatest extent to left of origin of any glyph bounding box, "
        "typically negative; deprecated with variable fonts")
    .def_readwrite("fXMax", &SkFontMetrics::fXMax,
        "greatest extent to right of origin of any glyph bounding box, "
        "typically positive; deprecated with variable fonts")
    .def_readwrite("fXHeight", &SkFontMetrics::fXHeight,
        "height of lower-case 'x', zero if unknown, typically negative")
    .def_readwrite("fCapHeight", &SkFontMetrics::fCapHeight,
        "height of an upper-case letter, zero if unknown, typically negative")
    .def_readwrite("fUnderlineThickness", &SkFontMetrics::fUnderlineThickness,
        "underline thickness")
    .def_readwrite("fUnderlinePosition", &SkFontMetrics::fUnderlinePosition,
        "distance from baseline to top of stroke, typically positive")
    .def_readwrite("fStrikeoutThickness", &SkFontMetrics::fStrikeoutThickness,
        "strikeout thickness")
    .def_readwrite("fStrikeoutPosition", &SkFontMetrics::fStrikeoutPosition,
        "distance from baseline to bottom of stroke, typically negative")
    ;
}
