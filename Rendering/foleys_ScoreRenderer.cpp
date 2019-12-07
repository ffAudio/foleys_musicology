

namespace foleys
{

ScoreRenderer::ScoreRenderer()
{
}

void ScoreRenderer::drawHarmonicStaff (juce::Graphics& g, juce::Rectangle<float> bounds, const Score::Part& part)
{
    for (int i=0; i < 5; ++i)
        g.fillRect (bounds.getX(), bounds.getCentreY() + (i - 2) * noteSize - staffLineThickness * factor * 0.5f, bounds.getWidth(), staffLineThickness * factor);

    const auto centerLine = juce::Point<float> (bounds.getX(), bounds.getCentreY());
    auto xPosition = 0.0f;

    for (auto measure = part.measures.cbegin(); measure != part.measures.cend(); ++measure)
    {
        if (measure == part.measures.cbegin() || measure->clef != (measure-1)->clef )
        {
            drawClef (g, *measure, centerLine, xPosition);
            xPosition += beamSpacing * noteSize;
        }

        if (measure == part.measures.cbegin() || measure->fifth != (measure-1)->fifth )
        {
            drawAccidentals (g, *measure, centerLine, xPosition);
            xPosition += beamSpacing * noteSize;
        }

        drawNotes (g, *measure, centerLine, { xPosition, xPosition + 250.0f });

        xPosition += 260.0f;
    }
}

void ScoreRenderer::drawClef (juce::Graphics& g, const Score::Measure& measure, juce::Point<float> centerLine, float& xPosition)
{
    float        offset = 0.0f;
    juce::Path   path;
    smufl::Glyph glyph;

    switch (measure.clef)
    {
        case Score::GClef:
            offset = noteSize;
            if (measure.octave == -2)
                glyph = smufl::GgClef15mb;
            else if (measure.octave == -1)
                glyph = smufl::GgClef8vb;
            else if (measure.octave == 1)
                glyph = smufl::GgClef8va;
            else if (measure.octave == 2)
                glyph = smufl::GgClef15ma;
            else
                glyph = smufl::GgClef;
            break;
        case Score::FClef:
            offset = -noteSize;
            if (measure.octave == -2)
                glyph = smufl::GfClef15mb;
            else if (measure.octave == -1)
                glyph = smufl::GfClef8vb;
            else if (measure.octave == 1)
                glyph = smufl::GfClef8va;
            else if (measure.octave == 2)
                glyph = smufl::GfClef15ma;
            else
                glyph = smufl::GfClef;
            break;
        case Score::CClef:
            glyph = smufl::GcClef;
            offset = -0.5f * measure.transpose * noteSize;
            break;
        default:
            jassertfalse;
            return;
    }

    getGlyph (glyph, path);
    g.fillPath (path, juce::AffineTransform::scale (factor * noteSize).translated (centerLine.getX() + xPosition, centerLine.getY() + offset));

    const auto bbox = glyphBoxes [glyph];
    xPosition += bbox.getWidth() * noteSize;
}

void ScoreRenderer::drawAccidentals (juce::Graphics& g, const Score::Measure& measure, juce::Point<float> centerLine, float& xPosition)
{
    if (measure.fifth == 0)
        return;

    juce::Path accidental;
    if (measure.fifth > 0)
    {
        getGlyph (smufl::GaccidentalSharp, accidental);
        const auto bbox = glyphBoxes [smufl::GaccidentalSharp];
        auto offset = measure.clef == Score::FClef ? -4.5f
                                                   : measure.clef == Score::CClef ? -1.5f - measure.transpose * 0.5f
                                                                                  : -2.0f;
        if (offset < -2.5f)
            offset += 3.5f;

        for (int i = 0; i < measure.fifth; ++i)
        {
            g.fillPath (accidental, juce::AffineTransform::scale (factor * noteSize)
                                                          .translated (centerLine + juce::Point<float> (xPosition, offset * noteSize )));
            xPosition += bbox.getWidth() * noteSize;
            offset -= 2.0f;
            if (offset < -2.5f)
                offset += 3.5f;
        }
    }
    else if (measure.fifth < 0)
    {
        getGlyph (smufl::GaccidentalFlat, accidental);
        const auto bbox = glyphBoxes [smufl::GaccidentalFlat];
        auto offset = measure.clef == Score::FClef ? -2.5f
                                                   : measure.clef == Score::CClef ? 0.5f + measure.transpose * 0.5f
                                                                                  : 0.0f;
        if (offset > 1.5f)
            offset -= 3.5f;

        for (int i = 0; i < -measure.fifth; ++i)
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

void ScoreRenderer::drawNotes (juce::Graphics& g, const Score::Measure& measure, juce::Point<float> centerLine, juce::Range<float> xPositions)
{
    for (auto note = measure.notes.cbegin(); note != measure.notes.cend(); ++note)
    {
        if (note->note == Score::Note::Rest)
        {
            juce::Path rest;
            auto xPosition = measure.getPotitionOfNote (note);
            if (note->duration == 1)
                getGlyph (smufl::GrestWhole, rest);
            else if (note->duration == 2)
                getGlyph (smufl::GrestHalf, rest);
            else if (note->duration == 4)
                getGlyph (smufl::GrestQuarter, rest);
            else if (note->duration == 8)
                getGlyph (smufl::Grest8th, rest);
            else if (note->duration == 16)
                getGlyph (smufl::Grest16th, rest);
            else if (note->duration == 32)
                getGlyph (smufl::Grest32nd, rest);
            else if (note->duration == 64)
                getGlyph (smufl::Grest64th, rest);

            auto position = centerLine + juce::Point<float> (xPositions.getStart() + xPosition * xPositions.getLength(), 0);
            g.fillPath (rest, juce::AffineTransform::scale (factor * noteSize).translated (position));
            continue;
        }

        juce::Path noteHead;
        if (note->duration >= 3)
            getGlyph (smufl::GnoteheadBlack, noteHead);
        else if (note->duration == 2)
            getGlyph (smufl::GnoteheadHalf, noteHead);
        else if (note->duration == 1)
            getGlyph (smufl::GnoteheadWhole, noteHead);

        auto xPosition = measure.getPotitionOfNote (note);
        auto linesOffset = measure.getOffsetFromCentreLine (*note);
        auto position = centerLine + juce::Point<float> (xPositions.getStart() + xPosition * xPositions.getLength(),
                                                         0.5f * linesOffset * noteSize);
        g.fillPath (noteHead, juce::AffineTransform::scale (factor * noteSize).translated (position));

        if (note->duration > 1)
        {
            if (linesOffset >= 0)
            {
                // staff up
                const auto& anchor = stemUpSE.find (smufl::GnoteheadBlack);
                if (anchor != stemUpSE.cend())
                {
                    const auto p = anchor->second;
                    g.fillRect (position.getX() + noteSize * (p.getX() - stemThickness),
                                position.getY() - noteSize * (p.getY() + 3.5f),
                                stemThickness * noteSize, 3.5f * noteSize);
                }
            }
            else
            {
                // staff down
                const auto& anchor = stemDownNW.find (smufl::GnoteheadBlack);
                if (anchor != stemDownNW.cend())
                {
                    const auto p = anchor->second;
                    g.fillRect (position.getX() + noteSize * (p.getX()),
                                position.getY() - noteSize * (p.getY()),
                                stemThickness * noteSize, 3.5f * noteSize);
                }
            }
        }
    }

    g.fillRect (centerLine.getX() + xPositions.getEnd(),
                centerLine.getY() - 2.0f * noteSize,
                thinBarlineThickness * noteSize,
                4.0f * noteSize);
}

void ScoreRenderer::setFont (juce::Typeface::Ptr typefaceToUse, const juce::var& metadata, float factorToUse)
{
    typeface = typefaceToUse;
    factor = factorToUse;

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
            // Use this to verify your metadata. You can ignore glyphs, that the renderer is not using.
            // DBG ("No glyph bounding box: " << smufl::glyphIDs [glyph]);
        }
    }

    // glyph anchors
    stemUpSE.clear();
    stemDownNW.clear();
    stemUpNE.clear();
    stemDownSW.clear();

    std::unordered_map<juce::String, smufl::Glyph> glyphMap;
    for (int i=0; i < int (smufl::LAST_GLYPH); ++i)
        glyphMap [smufl::glyphIDs [i]] = smufl::Glyph (i);

    const auto& anchors = metadata ["glyphsWithAnchors"];
    if (auto* object = anchors.getDynamicObject())
    {
        for (const auto& anchor : object->getProperties())
        {
            const auto& it = glyphMap.find (anchor.name.toString());
            if (it != glyphMap.end())
            {
                const auto& upse = anchor.value ["stemUpSE"];
                if (upse.isArray()) stemUpSE [it->second] = juce::Point<float> (float (upse [0]), float (upse [1]));

                const auto& downnw = anchor.value ["stemDownNW"];
                if (downnw.isArray()) stemDownNW [it->second] = juce::Point<float> (float (downnw [0]), float (downnw [1]));

                const auto& upne = anchor.value ["stemUpNE"];
                if (upne.isArray()) stemUpNE [it->second] = juce::Point<float> (float (upne [0]), float (upne [1]));

                const auto& downsw = anchor.value ["stemDownSW"];
                if (downsw.isArray()) stemDownSW [it->second] = juce::Point<float> (float (downsw [0]), float (downsw [1]));
            }
        }
    }
}

void ScoreRenderer::setNoteSize (float size)
{
    noteSize = size;
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
