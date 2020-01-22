
#pragma once

namespace foleys
{

class ScoreRenderer;

class NoteArrangement
{
public:
    NoteArrangement() = default;

    void addClef (const ScoreRenderer& renderer, const Score::Measure& measure);

    void addTimeSignature (const ScoreRenderer& renderer, const Score::Measure& measure);

    void addAccidentals (const ScoreRenderer& renderer, const Score::Measure& measure);

    /**
     draws the measure

     @param g the graphics context to draw onto
     @param origin the mid point of the staves start
     @param clefTab the tab stop to place the clef
     @param signatureTab the tab stop to place the time signature
     @param accidentalsTab the tab stop to place the accidentals
     */
    void drawMeasure (juce::Graphics& g, juce::Point<float> origin, float clefTab, float signatureTab, float accidentalsTab);

    juce::Rectangle<float> clefBox, signatureBox, accidentalsBox;

private:

    juce::Path clef;
    juce::Path timeSignature;
    juce::Path accidentals;

    JUCE_LEAK_DETECTOR (NoteArrangement)
};

}
