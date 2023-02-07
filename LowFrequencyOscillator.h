#pragma once

#include "LeakedObjectDetector.h"
#include <string>

/*
 copied UDT 3: LowFrequencyOscillator / LFO (actually a new UDT since I skipped Project 3)
 UDT3 does not use a nested UDT.
 */
struct LowFrequencyOscillator
{
    LowFrequencyOscillator (const double sampleRateInHz = 44100);
    ~LowFrequencyOscillator();
    
    // 5 properties:
    //    1) sample rate
    double sampleRateInHz;
    //    2) angular velocity per step, computed from speed and sample rate
    double phaseIncrementPerStep;
    //    3) rise time in samples => amplitude delta per step
    double amplitudeDeltaPerStep {0};
    //    4) current phase
    double currentPhase {0};
    //    5) current amplitude
    double currentAmplitude {1};

    // 3 things it can do:
    //    1) reset. This retriggers the LFO and adjust several parameters
    void reset (const double speedInHz = 1, const double initialPhase = 0, double riseTimeInSeconds = 0);

    //    2) generate single sample. This is a bipolar LFO with samples in [-1, 1].
    float generateSample();

    //    3) dump LFO output to console
    void dumpToConsole (const int totalNumSamples, const int displayEveryNthStep = 1);

    // print functions for members, Part 2
    void printSampleRateInHz (const std::string& prefix = "this->") const;
    void printPhaseIncrementPerStep (const std::string& prefix = "this->") const;
    void printAmplitudeDeltaPerStep (const std::string& prefix = "this->") const;
    void printCurrentPhase (const std::string& prefix = "this->") const;
    void printCurrentAmplitude (const std::string& prefix = "this->") const;
    void printLowFrequencyOscillator (const std::string& prefix = "this->") const;

    JUCE_LEAK_DETECTOR(LowFrequencyOscillator)
};
