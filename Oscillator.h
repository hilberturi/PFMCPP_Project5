#pragma once

#include "LeakedObjectDetector.h"
#include <string>

struct Oscillator
{
    struct SingleCycleWaveform 
    {    
        SingleCycleWaveform (const int numSupportPoints);
        ~SingleCycleWaveform();

        // 5 properties:
        //    1) name (std::string)
        std::string name {"sine"};
        //    2) interpolation type (int), 0 means linear interpolation 
        int interpolationType {0};
        //    3) number of equidistant support points with sampled data (int)
        int numSupportPoints;
        //    4) phase increment between adjacent support points 
        double phaseIncrement;
        //    5) flip polarity flag which controls polarity reversal (bool)
        bool flipPolarity {false};

        // 3 things it can do:
        //     1) compute interpolated sample given phase and angular velocity
        float computeInterpolatedSample (const float phase, const float oscAngularVelocity, const bool silent = true) const;

        //     2) transform shape in various ways
        void transformShape (bool const flipPolarity = false, const bool normalize = false, 
                             bool const removeDcOffset = false, const float warp = 0);

        //     3) dump waveform by generating samples at equidistant steps (as
        //        given by numSupportPoints) and showing them on the console.
        void dumpWaveform (const float scaling = 1) const;

        // I need an internal helper function, 0 <= sampleIndex < numSupportPoints
        float getSampleAtIndex (const int sampleIndex) const;

        // print functions for members, Part 2
        void printName (const std::string& prefix = "this->") const;
        void printInterpolationType (const std::string& prefix = "this->") const;
        void printNumSupportPoints (const std::string& prefix = "this->") const;
        void printPhaseIncrement (const std::string& prefix = "this->") const;
        void printFlipPolarity (const std::string& prefix = "this->") const;
        void printWaveform (const std::string& prefix = "this->") const;
    };

    Oscillator (const std::string& oscillatorId, float sampleRate = 44100);
    ~Oscillator();

    // 5 properties:
    //     1) oscillator identifier (std::string)
    std::string oscillatorId;
    //     2) waveform (SingleCycleWaveform)
    SingleCycleWaveform waveform {16};
    //     3) sample rate (float)
    float sampleRate;
    //     4) angular velocity
    float angularVelocity;
    //     5) current phase (float)
    float currentPhase {0};

    // 3 things it can do:
    //     1) reset to given frequency and initial phase
    void reset (float frequency, float initialPhase = 0);

    //     2) generate sample
    float generateSample(); // returns generated sample

    //     3) dump a given number of samples to console Samples (optionally without changing internal state)
    void dumpSamples (int numSamples, bool restoreCurrentPhase = false);

    // print functions for members, Part 2
    void printOscillatorId (const std::string& prefix = "this->") const;
    void printSampleRate (const std::string& prefix = "this->") const;
    void printAngularVelocity (const std::string& prefix = "this->") const;
    void printCurrentPhase (const std::string& prefix = "this->") const;
    void printWaveform (const std::string& prefix = "this->") const;
    void printOscillator (const std::string& prefix = "this->") const;

    JUCE_LEAK_DETECTOR(Oscillator)
};
