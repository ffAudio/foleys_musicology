
#pragma once

namespace foleys
{

class ScoreRenderer
{
public:

    ScoreRenderer();

    void drawHarmonicStaff (juce::Graphics&, juce::Rectangle<float> bounds);

    /**
     Set the SMuFL font for rendering

     @param typeface    the typeface (from an otf or ttf file)
     @param metadata    the font metadata from the json file provided with the font
     */
    void setFont (juce::Typeface::Ptr typeface, const juce::var& metadata);


private:

    /**
     Look up the glyph from the Typeface from the SMuFL glyph

     @param glyph   the SMuFL enum for the requested glyph
     @param path    the outline of the glyph will be returned here
     */
    bool getGlyph (smufl::Glyph glyph, juce::Path& path);

    /**
     Draws the clef for a system

     @param g           the Graphics context
     @param type        the type of the clef as glyph
     @param centerLine  is middle left point of the center
     @param xPosition   is a cursor that will be advanced by the used space of the glyphs
     */
    void drawClef (juce::Graphics& g, smufl::Glyph type, juce::Point<float> centerLine, float& xPosition);

    /**
     Draws the accidentals for a bar/staff.

     @param g           the Graphics context
     @param fifth       the number of accidentals in the circle of fifths. Use negative numbers for flat
     @param transposed  allows to adapt for different clefs, use 0 for G-clefs
     @param centerLine  is the middle left point of the center
     @param xPosition   is a cursor that will be advanced by the used space of the glyphs
     */
    void drawAccidentals (juce::Graphics& g, int fifth, int transposed, juce::Point<float> centerLine, float& xPosition);

    void drawNotes (juce::Graphics& g, juce::Point<float> centerLine, juce::Range<float> xPositions);

    float noteSize = 12.0f;
    float factor   = 16.0f;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScoreRenderer)
};

} // namespace foleys
