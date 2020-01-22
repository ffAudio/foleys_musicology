


namespace foleys
{


void NoteArrangement::addClef (const ScoreRenderer& renderer, const Score::Measure& measure)
{
    const auto   noteSize = renderer.getNoteSize();
    const auto   factor = renderer.getFactor();
    float        offset = 0.0f;
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

    renderer.getGlyph (glyph, clef);
    clef.applyTransform (juce::AffineTransform::scale (factor * noteSize).translated (0.0f, offset));
//    g.fillPath (getGlyph (glyph), juce::AffineTransform::scale (factor * noteSize).translated (centerLine.getX() + xPosition, centerLine.getY() + offset));

    clefBox = renderer.getGlyphBox (glyph).transformedBy (juce::AffineTransform::scale (noteSize).translated (0.0f, offset));
}

void NoteArrangement::addTimeSignature (const ScoreRenderer& renderer, const Score::Measure& measure)
{

}

void NoteArrangement::addAccidentals (const ScoreRenderer& renderer, const Score::Measure& measure)
{
    if (measure.fifth == 0)
        return;

    const auto noteSize  = renderer.getNoteSize();
    const auto factor    = renderer.getFactor();
    const auto scale     = juce::AffineTransform::scale (factor * noteSize);
    const auto boxScale  = juce::AffineTransform::scale (noteSize);
    float      xPosition = 0.0f;

    if (measure.fifth > 0)
    {
        const auto accidental = renderer.getGlyph (smufl::GaccidentalSharp);
        const auto bbox = renderer.getGlyphBox (smufl::GaccidentalSharp);
        auto offset = measure.clef == Score::FClef ? -4.5f
                                                   : measure.clef == Score::CClef ? -1.5f - measure.transpose * 0.5f
                                                                                  : -2.0f;
        if (offset < -2.5f)
            offset += 3.5f;

        for (int i = 0; i < measure.fifth; ++i)
        {
            accidentals.addPath (accidental, scale.translated (juce::Point<float> (xPosition, offset * noteSize)));
            accidentalsBox = accidentalsBox.getUnion (bbox.transformedBy (boxScale.translated (juce::Point<float> (xPosition, offset * noteSize))));
            xPosition += bbox.getWidth() * noteSize;

            offset -= 2.0f;
            if (offset < -2.5f)
                offset += 3.5f;
        }
    }
    else if (measure.fifth < 0)
    {
        const auto accidental = renderer.getGlyph (smufl::GaccidentalFlat);
        const auto bbox = renderer.getGlyphBox (smufl::GaccidentalFlat);
        auto offset = measure.clef == Score::FClef ? -2.5f
                                                   : measure.clef == Score::CClef ? 0.5f + measure.transpose * 0.5f
                                                                                  : 0.0f;
        if (offset > 1.5f)
            offset -= 3.5f;

        for (int i = 0; i < -measure.fifth; ++i)
        {
            accidentals.addPath (accidental, scale.translated (juce::Point<float> (xPosition, offset * noteSize)));
            accidentalsBox = accidentalsBox.getUnion (bbox.transformedBy (boxScale.translated (juce::Point<float> (xPosition, offset * noteSize))));
            xPosition += bbox.getWidth() * noteSize;

            offset += 2.0f;
            if (offset > 1.5f)
                offset -= 3.5f;
        }
    }
}

void NoteArrangement::drawMeasure (juce::Graphics& g, juce::Point<float> origin, float clefTab, float signatureTab, float accidentalsTab)
{
    g.fillPath (clef, juce::AffineTransform::translation (origin.getX() + clefTab, origin.getY()));
    g.fillPath (timeSignature, juce::AffineTransform::translation (origin.getX() + signatureTab, origin.getY()));
    g.fillPath (accidentals, juce::AffineTransform::translation (origin.getX() + accidentalsTab, origin.getY()));
}

} // namespace foleys
