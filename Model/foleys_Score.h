
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

    Score() = default;
    static std::unique_ptr<Score> fromMusicXml (juce::XmlElement& xml);

    //==============================================================================

    struct Note
    {
        enum NoteName
        {
            Rest = 0,
            C, D, E, F, G, A, B
        };

        /** note counting from C, 0=rest */
        NoteName note   = Rest;

        /** octave on piano (4: keyhole) */
        uint8_t octave = 4;

        /** positive: number of sharps, negative: number of flats */
        int8_t accidentals = 0;

        /** duration: 4=quarter, 0=brevis */
        uint8_t duration = 4;

        /** tuple: number of notes to make the next duration */
        uint8_t tuple = 2;

        /** dotted */
        uint8_t dotted = 0;

        int getNoteNumber() const;

        static Note fromNoteNumber (int noteNumber, bool preferFlat=false);
    };

    //==============================================================================

    class Measure
    {
    public:
        Measure();

        Clef clef      = GClef;

        /** Relative octave of the clef */
        int8_t  octave    = 0;

        /** C-clef can be used on any transposition. 0=centre line */
        int8_t  transpose = 0;

        /** scale in the circle of fifths, positive for sharps, negative for flats */
        int8_t  fifth     = 0;

        /** Time signature, first: enumerator, second: denominator */
        std::pair<uint8_t, uint8_t> signature = { 4, 4 };

        std::vector<Note> notes;

        /** returns the x position of a note inside a measure */
        float getPositionOfNote (std::vector<Note>::const_iterator note) const;

        /** Returns the number of half staff lines from the middle saff */
        int getOffsetFromCentreLine (const Note& note) const;

        std::tuple<int, bool> noteNeedsAccidental (std::vector<Note>::const_iterator note) const;

        double calculateNotesDuration();
        double calculateMeasureDuration();

    private:
        JUCE_LEAK_DETECTOR (Measure)
    };

    //==============================================================================

    class Part
    {
    public:
        Part();

        std::vector<Measure> measures;

    private:
        JUCE_LEAK_DETECTOR (Part)
    };

    //==============================================================================


private:
    std::vector<Part> parts;
    juce::ValueTree   model;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Score)
};

} // namespace foleys
