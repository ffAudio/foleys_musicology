

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

Score::Note Score::Note::fromNoteNumber (int noteNumber, bool preferFlat)
{
    Score::Note note;
    note.octave = (noteNumber / 12) - 1;
    switch (noteNumber % 12)
    {
        case 0: note.note = Score::Note::C; break;
        case 1:
            note.note = preferFlat ? Score::Note::D : Score::Note::C;
            note.accidentals = preferFlat ? -1 : 1;
            break;
        case 2: note.note = Score::Note::D; break;
        case 3:
            note.note = preferFlat ? Score::Note::E : Score::Note::D;
            note.accidentals = preferFlat ? -1 : 1;
            break;
        case 4: note.note = Score::Note::E; break;
        case 5: note.note = Score::Note::F; break;
        case 6:
            note.note = preferFlat ? Score::Note::G : Score::Note::F;
            note.accidentals = preferFlat ? -1 : 1;
            break;
        case 7: note.note = Score::Note::G; break;
        case 8:
            note.note = preferFlat ? Score::Note::A : Score::Note::G;
            note.accidentals = preferFlat ? -1 : 1;
            break;
        case 9: note.note = Score::Note::A; break;
        case 10:
            note.note = preferFlat ? Score::Note::B : Score::Note::A;
            note.accidentals = preferFlat ? -1 : 1;
            break;
        case 11: note.note = Score::Note::B; break;
        default: note.note = Score::Note::Rest; break;
    }

    return note;
}

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

double Score::Measure::calculateNotesDuration()
{
    double duration = 0;
    for (auto note : notes)
    {
        duration += 1.0 / note.duration;
    }
    return duration;
}

double Score::Measure::calculateMeasureDuration()
{
    return signature.first / double (signature.second);
}

//==============================================================================

Score::Part::Part()
{

}

//==============================================================================


} // namespace foleys
