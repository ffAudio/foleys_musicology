
#pragma once

namespace foleys
{

class NoteArrangement
{
public:
    NoteArrangement (Score::Part& part);

    void addClef (const Score::Measure& measure);

    void addTimeSignature (const Score::Measure& measure);

    void addAccidentals (const Score::Measure& measure);

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteArrangement)
};

}
