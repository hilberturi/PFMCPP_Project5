#pragma once

#include "LeakedObjectDetector.h"

#include "Oscillator.h"
#include "EnvelopeGate.h"
#include "LowFrequencyOscillator.h"

#include <string>

/*
 new UDT 5: Simple Mono-Synth (without Filter)
 with 2 member functions
 */
struct SimpleMonoSynth
{
    SimpleMonoSynth(const double sampleRateInHz = 44100);
    ~SimpleMonoSynth();

    // 5 properties:
    //   1) oscillator (I could also have chosen the compound oscillator)
    Oscillator oscillator;
    //   2) envelope gate
    EnvelopeGate envelopeGate {};
    //   3) Low-frequency oscillator
    LowFrequencyOscillator lfo;
    //   4) amount of LFO level modulation in [0, 1]
    float amountOfLfoLevelModulation {0};
    //   5) amplitude as given by velocity of last playing note
    float amplitudeOfPlayingNote {1};
    // Things it can do:
    //   1) trigger a note given midi pitch, velocity in 0..1. Trigger again with keyPressed = false for release of key.
    void triggerNote (const bool keyPressed = true, const int midiNoteNumber = 69, const float velocity = 1, const double tuningInHz = 440);
    //   2) generate sample
    float generateSample();
    //   3) dump given number of samples on console. Shown step numbers can be shifted by offset.
    void dumpSamples (const int numSteps, const int firstStepOffset = 0);

    // print functions for members, Part 2
    void printAmountOfLfoLevelModulation (const std::string& prefix = "this->") const;
    void printAmplitudeOfPlayingNote (const std::string& prefix = "this->") const;
    void printOscillator (const std::string& prefix = "this->") const;
    void printEnvelopeGate (const std::string& prefix = "this->") const;
    void printLfo (const std::string& prefix = "this->") const;

    JUCE_LEAK_DETECTOR(SimpleMonoSynth)
};
