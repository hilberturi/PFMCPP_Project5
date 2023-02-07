#pragma once

#include "LeakedObjectDetector.h"
#include <string>

/*
 copied UDT 2: Envelope Gate  (actually a new UDT since I skipped Project 3)
 */
struct EnvelopeGate
{
    // we do not know about enums yet and are instructed to use primitive
    // types such as 'int', so I will define some constant int expressions 
    // instead of introducing an enum in order to comply with instructions:
    static constexpr int ENVELOPESTATE_OFF {0};
    static constexpr int ENVELOPESTATE_ATTACK {1};
    static constexpr int ENVELOPESTATE_DECAY {2};
    static constexpr int ENVELOPESTATE_SUSTAIN {3};
    static constexpr int ENVELOPESTATE_RELEASE {4};

    // UDT2: Nested UDT: Envelope Parameters
    struct EnvelopeParameters
    {
        EnvelopeParameters(const float sustainLevel = 1);
        ~EnvelopeParameters();

        // 5 properties:
        //     1) attack time in samples (int)
        int attackTimeInSamples {0};
        //     2) decay time in samples (int)
        int decayTimeInSamples {0};
        //     3) normalized sustain level (float)
        float normalizedSustainLevel;
        //     4) release time in samples (int)
        int releaseTimeInSamples {0};
        //     5) exponent for shape power function
        //        default corresponds to linear shape:
        float exponentOfShapePowerFunction {1}; 
        // 3 things it can do:
        //     1) transform value by applying shape power function (or inverse)
        float applyShapePowerFunction (const float value, const bool computeInverseFunction = false) const;

        //     2) adjust parameters. midValuePoint is the normalized value in (0,1) 
        //         where the shape transform should reach 0.5
        //        CAUTION: must NOT be called when the EnvelopeGate is active 
        //        (i.e. in a state different from ENVELOPESTATE_OFF)
        void adjustParameters (const double attackTimeInSeconds, const double decayTimeInSeconds, const float sustainLevel, 
                               double const releaseTimeInSeconds, double midValuePoint = 0.5, const double sampleRateInHz = 44100);

        //     3) something that requires a loop => dump shape
        void dumpShape (int numberOfSteps = 11) const;

        // print functions for members, Part 2
        void printAttackTimeInSamples (const std::string& prefix = "this->") const;
        void printDecayTimeInSamples (const std::string& prefix = "this->") const;
        void printNormalizedSustainLevel (const std::string& prefix = "this->") const;
        void printReleaseTimeInSamples (const std::string& prefix = "this->") const;
        void printExponentOfShapePowerFunction (const std::string& prefix = "this->") const;
        void printEnvelopeParameters (const std::string& prefix = "this->") const;
    };

    // enclosing UDT2: EnvelopeGate
    EnvelopeGate(const float sustainLevel = 1);
    ~EnvelopeGate();

    // 5 properties:
    //     1) Envelope Parameters (nested UDT)
    EnvelopeParameters envelopeParameters;
    //     2) envelope state (int) 0: off, 1:attack, 2:decay, 3:sustain, 4:release
    //        obviously one should use an enum here but we do not know about enums yet.
    int envelopeState {ENVELOPESTATE_OFF};
    //     3) normalized target value approached by current state 
    //        ('normalized' means target before applying shape transform)
    float normalizedTargetValueInCurrentState {0};
    //     4) change per step in order to reach normalized target in given number of steps
    float normalizedDeltaPerStep {0};
    //     5) previous computed normalized sample
    float lastComputedNormalizedSample {0};
    // 3 things it can do:
    //     1) trigger envelope (bool keyPressed, bool allowRetriggger)
    //        returns true if an active envelope has been retriggered by a new key press
    bool triggerEnvelope (const bool keyPressed, const bool allowRetrigger = true); 

    //     2) get next sample for envelope gate
    float computeNextEnvelopeGateSample();

    //     3) dump envelope gate response, assuming that the key is being pressed for numSamplesKeyPressed
    void dumpEnvelopeGateResponse (const int numSamplesKeyPressed, int displayEveryNthStep = 1, const int maxStepsAllowed = 1000000);

    // print functions for members, Part 2
    void printEnvelopeState (const std::string& prefix = "this->") const;
    void printNormalizedTargetValueInCurrentState (const std::string& prefix = "this->") const;
    void printNormalizedDeltaPerStep (const std::string& prefix = "this->") const;
    void printLastComputedNormalizedSample (const std::string& prefix = "this->") const;
    void printEnvelopeParameters (const std::string& prefix = "this->") const;
    void printEnvelopeGate (const std::string& prefix = "this->") const;

    JUCE_LEAK_DETECTOR(EnvelopeGate)
};
