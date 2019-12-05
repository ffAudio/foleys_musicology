
#pragma once

namespace foleys
{



class Score
{
public:

    enum Clef
    {
        GClef = 0,
        CClef,
        FClef
    };

    Score (juce::ValueTree& tree);
    static std::unique_ptr<Score> fromMusicXml (juce::XmlElement& xml);

    //==============================================================================

    class Note
    {
    public:
        Note (juce::ValueTree& tree);
    private:
        JUCE_LEAK_DETECTOR (Note)
    };

    //==============================================================================

    class Measure
    {
    public:
        Measure (juce::ValueTree& tree);

        Clef clef      = GClef;
        int  octave    = 0;
        int  transpose = 0;  // used for C-clefs

        /** scale in the circle of fifths, positive for sharps, negative for flats */
        int  fifth     = 0;

    private:
        std::vector<Note> notes;
        JUCE_LEAK_DETECTOR (Measure)
    };

    //==============================================================================

    class Part
    {
    public:
        Part (juce::ValueTree& tree);

    private:
        std::vector<Measure> measures;
        JUCE_LEAK_DETECTOR (Part)
    };

    //==============================================================================


private:
    std::vector<Part> parts;
    juce::ValueTree   model;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Score)
};

} // namespace foleys
