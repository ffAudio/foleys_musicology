

namespace foleys
{

Score::Score (juce::ValueTree& tree)
{
}

std::unique_ptr<Score> Score::fromMusicXml (juce::XmlElement& xml)
{
    juce::ValueTree tree;
    auto score = std::make_unique<Score>(tree);
    return score;
}

//==============================================================================


//==============================================================================

Score::Measure::Measure()
{
}

float Score::Measure::getPotitionOfNote (std::vector<Note>::const_iterator note) const
{
    return std::distance (notes.cbegin(), note) / float (notes.size());
}

int Score::Measure::getOffsetFromCentreLine (const Note& note) const
{
    if (note.note == Note::Rest)
        return 0;

    int offset = clef == CClef ? transpose + 1
                               : clef == FClef ? -5 : 0;

    return offset - (note.octave - 5) * 7 - int (note.note);
}

//==============================================================================

Score::Part::Part()
{

}

//==============================================================================


} // namespace foleys
