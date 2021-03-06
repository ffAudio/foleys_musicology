
#pragma once

namespace foleys
{

class ScoreRenderer
{
public:

    ScoreRenderer();

    void drawHarmonicStaff (juce::Graphics&, juce::Rectangle<float> bounds, const Score::Part& part);

    /**
     Set the SMuFL font for rendering

     @param typeface    the typeface (from an otf or ttf file)
     @param metadata    the font metadata from the json file provided with the font
     @param factor      the factor, that the glyphs need to be scaled additionally to noteSize
     */
    void setFont (juce::Typeface::Ptr typeface, const juce::var& metadata, float factor);

    /**
     Set the note size
     */
    void setNoteSize (float size);
    float getNoteSize() const;

    /**
     returns the factor to apply
     */
    float getFactor() const;

    /**
     Set the size of a measure (exclusive accidentals, clef, etc.)
     */
    void setMeasureSize (float size);

    /**
     Look up the glyph from the Typeface from the SMuFL glyph

     @param glyph   the SMuFL enum for the requested glyph
     @param path    the outline of the glyph will be returned here
     */
    bool getGlyph (smufl::Glyph glyph, juce::Path& path) const;

    /**
     Returns the glyph for a SMuFL code

     @param glyph the SMuFL enum for the requested glyph
     */
    juce::Path getGlyph (smufl::Glyph glyph) const;

    /**
     returns the bounding box as defined in the fonts metadata for the glyph

     @param glyph the SMuFL enum for the requested bounding box
     */
    juce::Rectangle<float> getGlyphBox (smufl::Glyph glyph) const;

private:


    void drawNotes (juce::Graphics& g, const Score::Measure& measure, juce::Point<float> centerLine, juce::Range<float> xPositions);

    float noteSize    = 12.0f;
    float factor      = 16.0f;
    float measureSize = 250.0f;

    juce::Typeface::Ptr typeface;

    // engraving defaults
    float arrowShaftThickness        = 0.16f;
    float barlineSeparation          = 0.4f;
    float beamSpacing                = 0.25f;
    float beamThickness              = 0.5f;
    float bracketThickness           = 0.5f;
    float dashedBarlineDashLength    = 0.5f;
    float dashedBarlineGapLength     = 0.25f;
    float dashedBarlineThickness     = 0.16f;
    float hairpinThickness           = 0.16f;
    float legerLineExtension         = 0.4f;
    float legerLineThickness         = 0.16f;
    float lyricLineThickness         = 0.16f;
    float octaveLineThickness        = 0.16f;
    float pedalLineThickness         = 0.16f;
    float repeatBarlineDotSeparation = 0.16f;
    float repeatEndingLineThickness  = 0.16f;
    float slurEndpointThickness      = 0.1f;
    float slurMidpointThickness      = 0.22f;
    float staffLineThickness         = 0.13f;
    float stemThickness              = 0.12f;
    float subBracketThickness        = 0.16f;
    float textEnclosureThickness     = 0.16f;
    float thickBarlineThickness      = 0.5f;
    float thinBarlineThickness       = 0.16f;
    float tieEndpointThickness       = 0.1f;
    float tieMidpointThickness       = 0.22f;
    float tupletBracketThickness     = 0.16f;

    std::unordered_map<smufl::Glyph, juce::Rectangle<float>> glyphBoxes;

    std::unordered_map<smufl::Glyph, juce::Point<float>> stemUpSE;
    std::unordered_map<smufl::Glyph, juce::Point<float>> stemDownNW;
    std::unordered_map<smufl::Glyph, juce::Point<float>> stemUpNE;
    std::unordered_map<smufl::Glyph, juce::Point<float>> stemDownSW;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScoreRenderer)
};

} // namespace foleys
