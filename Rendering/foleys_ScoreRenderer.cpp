

namespace foleys
{

ScoreRenderer::ScoreRenderer()
{
}

void ScoreRenderer::drawHarmonicStaff (juce::Graphics& g, juce::Rectangle<float> bounds)
{
    for (int i=0; i < 5; ++i)
        g.fillRect (bounds.getX(), bounds.getCentreY() + (i - 2) * noteSize - staffLineThickness * factor * 0.5f, bounds.getWidth(), staffLineThickness * factor);

    const auto centerLine = juce::Point<float> (bounds.getX(), bounds.getCentreY());
    auto xPosition = 0.0f;

    drawClef (g, smufl::GcClef, centerLine, xPosition);
    xPosition += beamSpacing * noteSize;

    // g-clef: transpose 0 / f-clef: transpose 5 / c-clef: transpose -1
    drawAccidentals (g, -6, -1, centerLine, xPosition);
    xPosition += beamSpacing * noteSize;

    drawNotes (g, centerLine, { xPosition, xPosition + 250.0f });
}

void ScoreRenderer::drawClef (juce::Graphics& g, smufl::Glyph type, juce::Point<float> centerLine, float& xPosition)
{
    juce::Path clef;
    getGlyph (type, clef);
    float offset = 0.0f;
    switch (type)
    {
        case smufl::GgClef:
        case smufl::GgClef8va:
        case smufl::GgClef8vb:
        case smufl::GgClef8vbOld:
        case smufl::GgClef8vbCClef:
        case smufl::GgClef8vbParens:
        case smufl::GgClef15ma:
        case smufl::GgClef15mb:
        case smufl::GgClefLigatedNumberBelow:
        case smufl::GgClefLigatedNumberAbove:
        case smufl::GgClefArrowUp:
        case smufl::GgClefArrowDown:
            offset = noteSize;
            break;
        case smufl::GfClef:
        case smufl::GfClef8va:
        case smufl::GfClef8vb:
        case smufl::GfClef15ma:
        case smufl::GfClef15mb:
        case smufl::GfClefArrowUp:
        case smufl::GfClefArrowDown:
            offset = -noteSize;
            break;
        case smufl::GcClef:
        case smufl::GcClef8vb:
        case smufl::GcClefArrowUp:
        case smufl::GcClefArrowDown:
            break;
        default:
            jassertfalse;
            break;
    }

    g.fillPath (clef, juce::AffineTransform::scale (factor * noteSize).translated (centerLine.getX() + xPosition, centerLine.getY() + offset));

    const auto bbox = glyphBoxes [type];
    xPosition += bbox.getWidth() * noteSize;
}

void ScoreRenderer::drawAccidentals (juce::Graphics& g, int fifth, int transposed, juce::Point<float> centerLine, float& xPosition)
{
    juce::Path accidental;
    if (fifth > 0)
    {
        getGlyph (smufl::GaccidentalSharp, accidental);
        const auto bbox = glyphBoxes [smufl::GaccidentalSharp];
        auto offset = -2.0f - 0.5f * transposed;
        if (offset < -2.5f)
            offset += 3.5f;

        for (int i = 0; i < fifth; ++i)
        {
            g.fillPath (accidental, juce::AffineTransform::scale (factor * noteSize)
                                                          .translated (centerLine + juce::Point<float> (xPosition, offset * noteSize )));
            xPosition += bbox.getWidth() * noteSize;
            offset -= 2.0f;
            if (offset < -2.5f)
                offset += 3.5f;
        }
    }
    else if (fifth < 0)
    {
        getGlyph (smufl::GaccidentalFlat, accidental);
        const auto bbox = glyphBoxes [smufl::GaccidentalFlat];
        auto offset = -0.5f * transposed;
        if (offset > 1.5f)
            offset -= 3.5f;

        for (int i = 0; i < -fifth; ++i)
        {
            g.fillPath (accidental, juce::AffineTransform::scale (factor * noteSize)
                                                          .translated (centerLine + juce::Point<float> (xPosition, offset * noteSize )));
            xPosition += bbox.getWidth() * noteSize;
            offset += 2.0f;
            if (offset > 1.5f)
                offset -= 3.5f;
        }
    }
}

void ScoreRenderer::drawNotes (juce::Graphics& g, juce::Point<float> centerLine, juce::Range<float> xPositions)
{
    juce::Path note;
    getGlyph (smufl::GnoteheadBlack, note);

    auto distancePerQuarter = xPositions.getLength() / 4.0f;

    for (int i = 0; i < 4; ++i)
        g.fillPath (note, juce::AffineTransform::scale (factor * noteSize)
                                                .translated (centerLine + juce::Point<float> (xPositions.getStart() + i * distancePerQuarter, 0.0f)));

    g.fillRect (centerLine.getX() + xPositions.getEnd(),
                centerLine.getY() - 2.0f * noteSize,
                thinBarlineThickness * noteSize,
                4.0f * noteSize);
}

void ScoreRenderer::setFont (juce::Typeface::Ptr typefaceToUse, const juce::var& metadata)
{
    typeface = typefaceToUse;

    // engraving defaults
    const auto& defaults = metadata ["engravingDefaults"];
    arrowShaftThickness        = defaults.getProperty ("arrowShaftThickness",     0.16f);
    barlineSeparation          = defaults.getProperty ("barlineSeparation",       0.4f);
    beamSpacing                = defaults.getProperty ("beamSpacing",             0.25f);
    beamThickness              = defaults.getProperty ("beamThickness",           0.5f);
    bracketThickness           = defaults.getProperty ("bracketThickness",        0.5f);
    dashedBarlineDashLength    = defaults.getProperty ("dashedBarlineDashLength", 0.5f);
    dashedBarlineGapLength     = defaults.getProperty ("dashedBarlineGapLength",  0.25f);
    dashedBarlineThickness     = defaults.getProperty ("dashedBarlineThickness",  0.16f);
    hairpinThickness           = defaults.getProperty ("hairpinThickness",        0.16f);
    legerLineExtension         = defaults.getProperty ("legerLineExtension",      0.4f);
    legerLineThickness         = defaults.getProperty ("legerLineThickness",      0.16f);
    lyricLineThickness         = defaults.getProperty ("lyricLineThickness",      0.16f);
    octaveLineThickness        = defaults.getProperty ("octaveLineThickness",     0.16f);
    pedalLineThickness         = defaults.getProperty ("pedalLineThickness",      0.16f);
    repeatBarlineDotSeparation = defaults.getProperty ("repeatBarlineDotSeparation", 0.16f);
    repeatEndingLineThickness  = defaults.getProperty ("repeatEndingLineThickness", 0.16f);
    slurEndpointThickness      = defaults.getProperty ("slurEndpointThickness",   0.1f);
    slurMidpointThickness      = defaults.getProperty ("slurMidpointThickness",   0.22f);
    staffLineThickness         = defaults.getProperty ("staffLineThickness",      0.13f);
    stemThickness              = defaults.getProperty ("stemThickness",           0.12f);
    subBracketThickness        = defaults.getProperty ("subBracketThickness",     0.16f);
    textEnclosureThickness     = defaults.getProperty ("textEnclosureThickness",  0.16f);
    thickBarlineThickness      = defaults.getProperty ("thickBarlineThickness",   0.5f);
    thinBarlineThickness       = defaults.getProperty ("thinBarlineThickness",    0.16f);
    tieEndpointThickness       = defaults.getProperty ("tieEndpointThickness",    0.1f);
    tieMidpointThickness       = defaults.getProperty ("tieMidpointThickness",    0.22f);
    tupletBracketThickness     = defaults.getProperty ("tupletBracketThickness",  0.16f);

    // glyph boxes
    glyphBoxes.clear();
    const auto& boxes = metadata ["glyphBBoxes"];
    for (int i=0; i < smufl::LAST_GLYPH; ++i)
    {
        const auto  glyph    = smufl::Glyph (i);
        const auto& glyphBox = boxes [juce::Identifier (smufl::glyphIDs [glyph])];
        if (glyphBox.isObject())
        {
            const auto& northEast = glyphBox ["bBoxNE"];
            const auto& southWest = glyphBox ["bBoxSW"];
            jassert (northEast.isArray() && southWest.isArray());

            glyphBoxes [glyph] = juce::Rectangle<float> (float (southWest [0]),
                                                         float (northEast [1]) * -1.0f,
                                                         float (northEast [0]) - float (southWest [0]),
                                                         float (northEast [1]) - float ( southWest [1]));
        }
        else
        {
            DBG ("No glyph bounding box: " << smufl::glyphIDs [glyph]);
        }
    }
}


bool ScoreRenderer::getGlyph (smufl::Glyph glyph, juce::Path& path)
{
    juce::Array<int> glyphs;
    juce::Array<float> xOffsets;
    typeface->getGlyphPositions (juce::String::charToString (smufl::codepoint [glyph]), glyphs, xOffsets);
    if (glyphs [0] > 0)
    {
        typeface->getOutlineForGlyph (glyphs [0], path);
        return true;
    }

    return false;
}

} // namespace foleys
