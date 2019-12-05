

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

Score::Note::Note (juce::ValueTree& tree)
{

}

//==============================================================================

Score::Measure::Measure (juce::ValueTree& tree)
{

}

//==============================================================================

Score::Part::Part (juce::ValueTree& tree)
{

}

//==============================================================================


} // namespace foleys
