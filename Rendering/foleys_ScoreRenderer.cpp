

namespace foleys
{

ScoreRenderer::ScoreRenderer()
{
}

void ScoreRenderer::drawHarmonicStaff (juce::Graphics& g, juce::Rectangle<float> bounds, const Score::Part& part)
{
    auto centerLine = juce::Point<float> (bounds.getX(), bounds.getCentreY());
    auto xPosition = 0.0f;
    auto xEndPosition = 0.0f;

    for (auto measure = part.measures.cbegin(); measure != part.measures.cend(); ++measure)
    {
        NoteArrangement arrangement;

        if (measure == part.measures.cbegin() || measure->clef != (measure-1)->clef )
            arrangement.addClef (*this, *measure);

        if (measure == part.measures.cbegin() || measure->signature != (measure-1)->signature )
            arrangement.addTimeSignature (*this, *measure);

        if (measure == part.measures.cbegin() || measure->fifth != (measure-1)->fifth )
            arrangement.addAccidentals (*this, *measure);

        // calculate those over all parts later
        auto clefTab = arrangement.clefBox.isEmpty() ? 0.0f : beamSpacing * noteSize;
        auto signatureTab = arrangement.clefBox.isEmpty() ? beamSpacing * noteSize : clefTab + arrangement.clefBox.getRight() + beamSpacing * noteSize;
        auto accidentalsTab = arrangement.signatureBox.isEmpty() ? signatureTab : signatureTab + arrangement.signatureBox.getRight() + beamSpacing * noteSize;
        auto notesStart = arrangement.accidentalsBox.isEmpty() ? accidentalsTab : accidentalsTab + arrangement.accidentalsBox.getRight() + beamSpacing * noteSize;

        arrangement.drawMeasure (g, centerLine, clefTab, signatureTab, accidentalsTab);

        xPosition = notesStart;

        if (!measure->notes.empty() && std::get<1>(measure->noteNeedsAccidental (measure->notes.begin())))
            xPosition += (glyphBoxes [smufl::GaccidentalSharp].getWidth() + barlineSeparation) * noteSize;

        drawNotes (g, *measure, centerLine, { xPosition, xPosition + measureSize });

        xPosition += measureSize + noteSize;

        const auto staffTop = centerLine.getY() - 2.0f * noteSize;
        const auto staffHeight = 4.0f * noteSize;

        const auto nextMeasure = measure + 1;
        if (nextMeasure == part.measures.cend())
        {
            g.fillRect (centerLine.getX() + xPosition, staffTop,
                        thinBarlineThickness * noteSize, staffHeight);
            xPosition += (thinBarlineThickness + beamSpacing) * noteSize;

            g.fillRect (centerLine.getX() + xPosition, staffTop,
                        thickBarlineThickness * noteSize, staffHeight);
            xPosition += thickBarlineThickness * noteSize;
        }
        else if (measure->clef != nextMeasure->clef || measure->fifth != nextMeasure->fifth)
        {
            g.fillRect (centerLine.getX() + xPosition, staffTop,
                        thinBarlineThickness * noteSize, staffHeight);
            xPosition += (thinBarlineThickness + beamSpacing) * noteSize;
            g.fillRect (centerLine.getX() + xPosition, staffTop,
                        thinBarlineThickness * noteSize, staffHeight);
            xPosition += thinBarlineThickness * noteSize;
        }
        else
        {
            g.fillRect (centerLine.getX() + xPosition, staffTop,
                        thinBarlineThickness * noteSize, staffHeight);
            xPosition += thinBarlineThickness * noteSize;
        }

        centerLine.addXY (xPosition, 0.0f);
        xEndPosition = centerLine.getX();
    }

    for (int i=0; i < 5; ++i)
        g.fillRect (bounds.getX(), bounds.getCentreY() + (i - 2) * noteSize - staffLineThickness * factor * 0.5f, xEndPosition - bounds.getX(), staffLineThickness * factor);
}


void ScoreRenderer::drawNotes (juce::Graphics& g, const Score::Measure& measure, juce::Point<float> centerLine, juce::Range<float> xPositions)
{
    const auto scale = juce::AffineTransform::scale (factor * noteSize);

    auto noteXpos = xPositions.getStart();

    for (auto note = measure.notes.cbegin(); note != measure.notes.cend(); ++note)
    {
        if (note->note == Score::Note::Rest)
        {
            juce::Path rest;

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

            auto position = centerLine + juce::Point<float> (noteXpos, 0);
            g.fillPath (rest, scale.translated (position));

            noteXpos += xPositions.getLength() / note->duration;

            continue;
        }

        juce::Path noteHead;
        if (note->duration >= 3)
            getGlyph (smufl::GnoteheadBlack, noteHead);
        else if (note->duration == 2)
            getGlyph (smufl::GnoteheadHalf, noteHead);
        else if (note->duration == 1)
            getGlyph (smufl::GnoteheadWhole, noteHead);

        auto linesOffset = measure.getOffsetFromCentreLine (*note);
        auto position = centerLine + juce::Point<float> (noteXpos, 0.5f * linesOffset * noteSize);
        g.fillPath (noteHead, scale.translated (position));

        juce::Path ledgerLine;
        getGlyph (note->duration < 2 ? smufl::GlegerLineWide : smufl::GlegerLine, ledgerLine);
        for (int i=6; i <= linesOffset; i += 2)
        {
            auto ledgerPos = centerLine + juce::Point<float> (noteXpos, 0.5f * i * noteSize);
            g.fillPath (ledgerLine, scale.translated (ledgerPos));
        }

        for (int i=-6; i >= linesOffset; i -= 2)
        {
            auto ledgerPos = centerLine + juce::Point<float> (noteXpos, 0.5f * i * noteSize);
            g.fillPath (ledgerLine, scale.translated (ledgerPos));
        }

        auto [accidental, needed] = measure.noteNeedsAccidental (note);
        if (needed)
        {
            smufl::Glyph accidentalGlyph = smufl::LAST_GLYPH;
            switch (accidental)
            {
                case -2: accidentalGlyph = smufl::GaccidentalDoubleFlat; break;
                case -1: accidentalGlyph = smufl::GaccidentalFlat; break;
                case 0: accidentalGlyph = smufl::GaccidentalNatural; break;
                case 1: accidentalGlyph = smufl::GaccidentalSharp; break;
                case 2: accidentalGlyph = smufl::GaccidentalDoubleSharp; break;
            }

            if (accidentalGlyph != smufl::LAST_GLYPH)
                g.fillPath (getGlyph (accidentalGlyph),
                            scale.translated (position - juce::Point<float>((glyphBoxes [accidentalGlyph].getWidth() + barlineSeparation) * noteSize, 0)));
        }

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

                    if (note->duration == 8)
                        g.fillPath (getGlyph (smufl::Gflag8thUp), scale.translated (position.getX() + noteSize * (p.getX() - stemThickness), position.getY() - noteSize * (p.getY() + 3.5f)));
                    else if (note->duration == 16)
                        g.fillPath (getGlyph (smufl::Gflag16thUp), scale.translated (position.getX() + noteSize * (p.getX() - stemThickness), position.getY() - noteSize * (p.getY() + 3.5f)));
                    else if (note->duration == 32)
                        g.fillPath (getGlyph (smufl::Gflag32ndUp), scale.translated (position.getX() + noteSize * (p.getX() - stemThickness), position.getY() - noteSize * (p.getY() + 3.5f)));
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

                    if (note->duration == 8)
                        g.fillPath (getGlyph (smufl::Gflag8thDown), scale.translated (position.getX() + noteSize * p.getX(), position.getY() + noteSize * (3.5f - p.getY())));
                    else if (note->duration == 16)
                        g.fillPath (getGlyph (smufl::Gflag16thDown), scale.translated (position.getX() + noteSize * p.getX(), position.getY() + noteSize * (3.5f - p.getY())));
                    else if (note->duration == 32)
                        g.fillPath (getGlyph (smufl::Gflag32ndDown), scale.translated (position.getX() + noteSize * p.getX(), position.getY() + noteSize * (3.5f - p.getY())));
                }
            }
        }

        noteXpos += xPositions.getLength() / note->duration;
    }
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

float ScoreRenderer::getNoteSize() const
{
    return noteSize;
}

float ScoreRenderer::getFactor() const
{
    return factor;
}

void ScoreRenderer::setMeasureSize (float size)
{
    measureSize = size;
}

bool ScoreRenderer::getGlyph (smufl::Glyph glyph, juce::Path& path) const
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

juce::Path ScoreRenderer::getGlyph (smufl::Glyph glyph) const
{
    juce::Path p;
    getGlyph (glyph, p);
    return p;
}

juce::Rectangle<float> ScoreRenderer::getGlyphBox (smufl::Glyph glyph) const
{
    const auto& it = glyphBoxes.find (glyph);
    if (it != glyphBoxes.cend())
        return it->second;

    return {};
}

} // namespace foleys
