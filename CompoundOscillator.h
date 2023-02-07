#pragma once

#include "LeakedObjectDetector.h"
#include "Oscillator.h"
#include <string>

/*
 new UDT 4: CompoundOscillator
 with 2 member functions
 */
struct CompoundOscillator
{
    CompoundOscillator(float sampleRate = 44100);
    ~CompoundOscillator();

    // Properties:
    // 1) Oscillator oscA
    Oscillator oscA;
    // 2) Oscillator oscB
    Oscillator oscB;
    // 3) mix balance of oscillators. 0 = oscA only, 1 = oscB only.
    float balance = 0.5f;
    // 4) detune in cent. oscA will be low and oscB will be sharp by given amount
    float detuneInCent = 2.5f;
    // 5) phase offset oscillator B. Initial phase compared to oscA will be shifted by given offset.
    float phaseOffsetOscillatorB = 0;

    // Things it can do:
    // 1) resets both oscillators at once and adjusts parameters
    void reset (const float frequency, const float initialPhaseOscA = 0);
    // 2) generate sample by mixing samples of both embedded oscillators
    float generateSample();
    // 3) dump given number of generated samples on console
    void dumpSamples (const int numSamples, const bool restoreCurrentPhase = false);

    // print functions for members, Part 2
    void printBalance (const std::string& prefix = "this->") const;
    void printDetuneInCent (const std::string& prefix = "this->") const;
    void printPhaseOffsetOscillatorB (const std::string& prefix = "this->") const;
    void printOscA (const std::string& prefix = "this->") const;
    void printOscB (const std::string& prefix = "this->") const;

    JUCE_LEAK_DETECTOR(CompoundOscillator)
};
