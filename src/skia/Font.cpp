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
    .def_property_readonly("coordinates",
        [] (const SkFontArguments::VariationPosition& vp) {
            auto coords = py::list(vp.coordinateCount);
            for (int i = 0; i < vp.coordinateCount; ++i)
                coords[i] = py::cast(vp.coordinates[i]);
            return coords;
        })
    .def_readonly("coordinateCount",
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

py::class_<SkTypeface, sk_sp<SkTypeface>, SkRefCnt> typeface(
    m, "Typeface", R"docstring(
    The :py:class:`Typeface` class specifies the typeface and intrinsic style of
    a font.

    This is used in the paint, along with optionally algorithmic settings like
    textSize, textSkewX, textScaleX, kFakeBoldText_Mask, to specify how text
    appears when drawn (and measured).

    Typeface objects are immutable, and so they can be shared between threads.
    )docstring");

py::enum_<SkTypeface::SerializeBehavior>(typeface, "SerializeBehavior",
    R"docstring(
    A typeface can serialize just a descriptor (names, etc.), or it can also
    include the actual font data (which can be large).

    This enum controls how serialize() decides what to serialize.
    )docstring")
    .value("kDoIncludeData", SkTypeface::SerializeBehavior::kDoIncludeData)
    .value("kDontIncludeData", SkTypeface::SerializeBehavior::kDontIncludeData)
    .value("kIncludeDataIfLocal",
        SkTypeface::SerializeBehavior::kIncludeDataIfLocal)
    .export_values();

typeface
    .def("fontStyle", &SkTypeface::fontStyle,
        R"docstring(
        Returns the typeface's intrinsic style attributes.
        )docstring")
    .def("isBold", &SkTypeface::isBold,
        R"docstring(
        Returns true if style() has the kBold bit set.
        )docstring")
    .def("isItalic", &SkTypeface::isBold,
        R"docstring(
        Returns true if style() has the kItalic bit set.
        )docstring")
    .def("isFixedPitch", &SkTypeface::isFixedPitch,
        R"docstring(
        Returns true if the typeface claims to be fixed-pitch.
        )docstring")
    .def("getVariationDesignPosition",
        [] (const SkTypeface& typeface, int coordinateCount) {
            std::vector<SkFontArguments::VariationPosition::Coordinate>
                coords(coordinateCount);
            auto count = typeface.getVariationDesignPosition(
                &coords[0], coords.size());
            if (count == -1)
                throw std::runtime_error("Failed to get");
            else if (count > int(coords.size()))
                coords.erase(coords.begin() + count, coords.end());
            return coords;
        },
        R"docstring(
        Returns the design variation coordinates.

        :param int coordinateCount: the maximum number of entries to get.
        :return: list of coordinates
        :rtype: List[skia.FontArguments.VariationPosition.Coordinate]
        )docstring",
        py::arg("coordinateCount") = 10)
    .def("getVariationDesignParameters",
        [] (const SkTypeface& typeface, int parameterCount) {
            std::vector<SkFontParameters::Variation::Axis>
                params(parameterCount);
            auto count = typeface.getVariationDesignParameters(
                &params[0], params.size());
            if (count == -1)
                throw std::runtime_error("Failed to get");
            else if (count > int(params.size()))
                params.erase(params.begin() + count, params.end());
            return params;
        },
        R"docstring(
        Returns the design variation parameters.

        :param int parameterCount: the maximum number of entries to get.
        :return: list of parameters
        :rtype: List[skia.FontArguments.VariationPosition.Axis]
        )docstring",
        py::arg("parameterCount") = 10)
    .def("uniqueID", &SkTypeface::uniqueID,
        R"docstring(
        Return a 32bit value for this typeface, unique for the underlying font
        data.

        Will never return 0.
        )docstring")
    .def("makeClone", &SkTypeface::makeClone,
        R"docstring(
        Return a new typeface based on this typeface but parameterized as
        specified in the :py:class:`FontArguments`.

        If the :py:class:`FontArguments` does not supply an argument for a
        parameter in the font then the value from this typeface will be used as
        the value for that argument. If the cloned typeface would be exaclty the
        same as this typeface then this typeface may be ref'ed and returned. May
        return nullptr on failure.
        )docstring",
        py::arg("fontArguments"))
    // .def("serialize",
    //     (void (SkTypeface::*)(SkWStream*, SkTypeface::SerializeBehavior) const)
    //     &SkTypeface::serialize,
    //     "Write a unique signature to a stream, sufficient to reconstruct a "
    //     "typeface referencing the same font when Deserialize is called.",
    //     py::arg("stream"), py::arg("behavior") =
    //         SkTypeface::SerializeBehavior::kIncludeDataIfLocal)
    .def("serialize",
        py::overload_cast<SkTypeface::SerializeBehavior>(
            &SkTypeface::serialize, py::const_),
        R"docstring(
        Returns a unique signature to a stream, sufficient to reconstruct a
        typeface referencing the same font when Deserialize is called.
        )docstring",
        py::arg("behavior") =
            SkTypeface::SerializeBehavior::kIncludeDataIfLocal)
    .def("unicharsToGlyphs",
        [] (const SkTypeface& typeface, const std::vector<SkUnichar>& chars) {
            std::vector<SkGlyphID> glyphs(chars.size());
            typeface.unicharsToGlyphs(&chars[0], chars.size(), &glyphs[0]);
            return glyphs;
        },
        R"docstring(
        Given an array of UTF32 character codes, return their corresponding
        glyph IDs.

        :param chars: the array of UTF32 chars.
        :return: the corresponding glyph IDs for each character.
        )docstring",
        py::arg("chars"))
    .def("unicharToGlyph", &SkTypeface::unicharToGlyph,
        R"docstring(
        Return the glyphID that corresponds to the specified unicode code-point
        (in UTF32 encoding).

        If the unichar is not supported, returns 0.

        This is a short-cut for calling :py:meth:`unicharsToGlyphs`.
        )docstring",
        py::arg("unichar"))
    .def("countGlyphs", &SkTypeface::countGlyphs,
        R"docstring(
        Return the number of glyphs in the typeface.
        )docstring")
    .def("countTables", &SkTypeface::countTables,
        R"docstring(
        Return the number of tables in the font.
        )docstring")
    .def("getTableTags",
        [] (const SkTypeface& typeface) {
            std::vector<SkFontTableTag> tags(typeface.countTables());
            size_t size = typeface.getTableTags(&tags[0]);
            if (size < tags.size())
                throw std::runtime_error("Failed to get table tags.");
            return tags;
        },
        R"docstring(
        Returns the list of table tags in the font.
        )docstring")
    .def("getTableSize", &SkTypeface::getTableSize,
        R"docstring(
        Given a table tag, return the size of its contents, or 0 if not present.
        )docstring",
        py::arg("tag"))
    .def("getTableData",
        [] (const SkTypeface& typeface, SkFontTableTag tag) {
            size_t size = typeface.getTableSize(tag);
            std::vector<char> data(size);
            auto written = typeface.getTableData(tag, 0, data.size(), &data[0]);
            if (written == 0 && size > 0)
                throw std::runtime_error("Failed to get table data.");
            return py::bytes(&data[0], data.size());
        },
        R"docstring(
        Returns the contents of a table.

        Note that the contents of the table will be in their native endian order
        (which for most truetype tables is big endian). If the table tag is not
        found, or there is an error copying the data, then 0 is returned. If
        this happens, it is possible that some or all of the memory pointed to
        by data may have been written to, even though an error has occured.

        :param tag: The table tag whose contents are to be copied.
        :return: table contents
        )docstring",
        py::arg("tag"))
    .def("copyTableData", &SkTypeface::copyTableData,
        R"docstring(
        Return an immutable copy of the requested font table, or nullptr if that
        table was not found.

        This can sometimes be faster than calling :py:meth:`getTableData` twice:
        once to find the length, and then again to copy the data.

        :param tag: The table tag whose contents are to be copied
        :return: an immutable copy of the table's data, or nullptr.
        )docstring",
        py::arg("tag"))
    .def("getUnitsPerEm", &SkTypeface::getUnitsPerEm,
        R"docstring(
        Return the units-per-em value for this typeface, or zero if there is an
        error.
        )docstring")
    .def("getKerningPairAdjustments",
        [] (const SkTypeface& typeface,
            const std::vector<SkGlyphID>& glyphs) -> py::object {
            std::vector<int32_t> adjustments(glyphs.size() - 1);
            auto result = typeface.getKerningPairAdjustments(
                &glyphs[0], glyphs.size(), &adjustments[0]);
            if (!result) {
                // Kerning is not supported for this typeface.
                return py::none();
            }
            return py::cast(adjustments);
        },
        R"docstring(
        Given a run of glyphs, return the associated horizontal adjustments.

        Adjustments are in "design units", which are integers relative to the
        typeface's units per em (see getUnitsPerEm).

        Some typefaces are known to never support kerning. If the typeface does
        not support kerning, then this method will return None (no kerning) for
        all possible glyph runs.
        )docstring",
        py::arg("glyphs"))
    .def("getFamilyNames",
        [] (const SkTypeface& typeface) {
            SkTypeface::LocalizedString name;
            py::list results;
            auto it = typeface.createFamilyNameIterator();
            if (!it)
                throw std::runtime_error("Null pointer exception");
            while (it->next(&name)) {
                auto fString = py::str(
                    name.fString.c_str(), name.fString.size());
                auto fLanguage = py::str(
                    name.fLanguage.c_str(), name.fLanguage.size());
                results.append(py::make_tuple(fString, fLanguage));
            }
            it->unref();
            return results;
        },
        R"docstring(
        Returns a list of (family name, language) pairs specified by the font.
        )docstring")
    .def("getFamilyName",
        [] (const SkTypeface& typeface) {
            SkString name;
            typeface.getFamilyName(&name);
            return py::str(name.c_str(), name.size());
        },
        R"docstring(
        Return the family name for this typeface.

        It will always be returned encoded as UTF8, but the language of the name
        is whatever the host platform chooses.
        )docstring")
    // .def("openStream", &SkTypeface::openStream,
    //     "Return a stream for the contents of the font data, or NULL on "
    //     "failure.")
    // .def("makeFontData", &SkTypeface::makeFontData,
    //     R"docstring(
    //     Return the font data, or nullptr on failure.
    //     )docstring")
    // .def("createScalerContext", &SkTypeface::createScalerContext,
    //     "Return a scalercontext for the given descriptor.")
    .def("getBounds", &SkTypeface::getBounds,
        R"docstring(
        Return a rectangle (scaled to 1-pt) that represents the union of the
        bounds of all of the glyphs, but each one positioned at (0,).

        This may be conservatively large, and will not take into account any
        hinting or other size-specific adjustments.
        )docstring")
    // .def("filterRec", &SkTypeface::filterRec)
    // .def("getFontDescriptor", &SkTypeface::getFontDescriptor)
    .def_static("UniqueID", &SkTypeface::UniqueID,
        R"docstring(
        Return the uniqueID for the specified typeface.

        If the face is null, resolve it to the default font and return its
        uniqueID. Will never return 0.
        )docstring",
        py::arg("typeface"))
    .def_static("Equal", &SkTypeface::Equal,
        R"docstring(
        Returns true if the two typefaces reference the same underlying font,
        handling either being null (treating null as the default font)
        )docstring")
    .def_static("MakeDefault", &SkTypeface::MakeDefault,
        R"docstring(
        Returns the default normal typeface, which is never nullptr.
        )docstring")
    .def_static("MakeFromName",
        [] (const std::string* familyName, const SkFontStyle& fontStyle) {
            return SkTypeface::MakeFromName(
                (familyName) ? familyName->c_str() : nullptr, fontStyle);
        },
        R"docstring(
        Creates a new reference to the typeface that most closely matches the
        requested familyName and fontStyle.

        This method allows extended font face specifiers as in the
        :py:class:`FontStyle` type. Will never return null.

        :param familyName:  May be NULL. The name of the font family.
        :param fontStyle:   The style of the typeface.
        :return: reference to the closest-matching typeface.
        )docstring",
        py::arg("familyName"), py::arg("fontStyle"))
    .def_static("MakeFromFile",
        [] (const std::string& path, int index) {
            return SkTypeface::MakeFromFile(&path[0], index);
        },
        R"docstring(
        Return a new typeface given a file.

        If the file does not exist, or is not a valid font file, returns
        nullptr.
        )docstring",
        py::arg("path"), py::arg("index") = 0)
    // .def_static("MakeFromStream", &SkTypeface::MakeFromStream,
    //     "Return a new typeface given a stream.")
    .def_static("MakeFromData", &SkTypeface::MakeFromData,
        R"docstring(
        Return a new typeface given a :py:class:`Data`.

        If the data is null, or is not a valid font file, returns nullptr.

        Use :py:meth:`MakeDeserialize` for the result of :py:meth:`serialize`.
        )docstring",
        py::arg("data"), py::arg("index") = 0)
    // .def_static("MakeFromFontData", &SkTypeface::MakeFromFontData,
    //     "Return a new typeface given font data and configuration.")
    .def_static("MakeDeserialize",
        [] (const sk_sp<SkData>& data) {
            SkMemoryStream stream(data);
            return SkTypeface::MakeDeserialize(&stream);
        },
        R"docstring(
        Given the data previously written by :py:meth:`serialize`, return a new
        instance of a typeface referring to the same font.

        If that font is not available, return nullptr.
        )docstring",
        py::arg("dats"))
    ;

// FontMgr
py::class_<SkFontStyleSet, sk_sp<SkFontStyleSet>, SkRefCnt>(m, "FontStyleSet")
    .def("count", &SkFontStyleSet::count)
    .def("getStyle", &SkFontStyleSet::getStyle)
    .def("createTypeface", &SkFontStyleSet::createTypeface)
    .def("matchStyle", &SkFontStyleSet::matchStyle)
    .def_static("CreateEmpty", &SkFontStyleSet::CreateEmpty)
    ;

py::class_<SkFontMgr, sk_sp<SkFontMgr>, SkRefCnt>(m, "FontMgr")
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
    :py:class:`Font` controls options applied when drawing and measuring text.
    )docstring");

py::enum_<SkFont::Edging>(font, "Edging", R"docstring(
    Whether edge pixels draw opaque or with partial transparency.
    )docstring")
    .value("kAlias", SkFont::Edging::kAlias)
    .value("kAntiAlias", SkFont::Edging::kAntiAlias)
    .value("kSubpixelAntiAlias", SkFont::Edging::kSubpixelAntiAlias)
    .export_values();

font
    .def(py::init<>(), "Constructs :py:class:`Font` with default values.")
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
