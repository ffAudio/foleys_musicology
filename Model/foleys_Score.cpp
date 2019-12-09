

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

float Score::Measure::getPositionOfNote (std::vector<Note>::const_iterator note) const
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

std::tuple<int, bool> Score::Measure::noteNeedsAccidental (std::vector<Note>::const_iterator note) const
{
    // FIXME: check, if not already set, and check, if needs a natural
    switch (note->note) {
        case Note::C:
            if (note->accidentals == 0)
                return {note->accidentals, fifth >= 2 || fifth <= -6};
            else if (note->accidentals == 1)
                return {note->accidentals, fifth < 2};
            else if (note->accidentals == 2)
                return {note->accidentals, true};
            else if (note->accidentals == -1)
                return {note->accidentals, fifth > -6};
            else if (note->accidentals == -2)
                return {note->accidentals, true};

        case Note::D:
            if (note->accidentals == 0)
                return {note->accidentals, fifth >= 4 || fifth <= -4};
            else if (note->accidentals == 1)
                return {note->accidentals, fifth < 4};
            else if (note->accidentals == 2)
                return {note->accidentals, true};
            else if (note->accidentals == -1)
                return {note->accidentals, fifth > -4};
            else if (note->accidentals == -2)
                return {note->accidentals, true};

        case Note::E:
            if (note->accidentals == 0)
                return {note->accidentals, fifth >= 6 || fifth <= -2};
            else if (note->accidentals == 1)
                return {note->accidentals, fifth < 6};
            else if (note->accidentals == 2)
                return {note->accidentals, true};
            else if (note->accidentals == -1)
                return {note->accidentals, fifth > -2};
            else if (note->accidentals == -2)
                return {note->accidentals, true};

        case Note::F:
            if (note->accidentals == 0)
                return {note->accidentals, fifth >= 1 || fifth <= -7};
            else if (note->accidentals == 1)
                return {note->accidentals, fifth < 1};
            else if (note->accidentals == 2)
                return {note->accidentals, true};
            else if (note->accidentals == -1)
                return {note->accidentals, fifth > -7};
            else if (note->accidentals == -2)
                return {note->accidentals, true};

        case Note::G:
            if (note->accidentals == 0)
                return {note->accidentals, fifth >= 3 || fifth <= -5};
            else if (note->accidentals == 1)
                return {note->accidentals, fifth < 3};
            else if (note->accidentals == 2)
                return {note->accidentals, true};
            else if (note->accidentals == -1)
                return {note->accidentals, fifth > -5};
            else if (note->accidentals == -2)
                return {note->accidentals, true};

        case Note::A:
            if (note->accidentals == 0)
                return {note->accidentals, fifth >= 5 || fifth <= -3};
            else if (note->accidentals == 1)
                return {note->accidentals, fifth < 5};
            else if (note->accidentals == 2)
                return {note->accidentals, true};
            else if (note->accidentals == -1)
                return {note->accidentals, fifth > -3};
            else if (note->accidentals == -2)
                return {note->accidentals, true};

        case Note::B:
            if (note->accidentals == 0)
                return {note->accidentals, fifth >= 7 || fifth <= -1};
            else if (note->accidentals == 1)
                return {note->accidentals, fifth < 7};
            else if (note->accidentals == 2)
                return {note->accidentals, true};
            else if (note->accidentals == -1)
                return {note->accidentals, fifth > -1};
            else if (note->accidentals == -2)
                return {note->accidentals, true};

        default:
            break;
    }

    return {note->accidentals, false};
}

//==============================================================================

Score::Part::Part()
{

}

//==============================================================================


} // namespace foleys
