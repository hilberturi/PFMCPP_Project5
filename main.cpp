#include <iostream>
#include <string>
#include <cmath>
/*
Project 5: Part 1 / 4
 video Chapter 2 - Part 12

 Create a branch named Part1

Purpose:  This project continues developing Project3.
       you will learn how to take code from existing projects and migrate only what you need to new projects
       you will learn how to write code that doesn't leak as well as how to refactor. 

NOTE: there are 2 sets of instructions:
       One for if you completed project 3
       One for if you skipped project 3.

 Destructors

===============================================================     
 If you completed Project 3:
 
 1) Copy 3 of your user-defined types (the ones with constructors and for()/while() loops from Project 3) here
    Choose the classes that contained nested classes.  Include the nested classes when you copy them over.

 2) move all of your implementations of all functions to OUTSIDE of the class.

 3) add destructors
        make the destructors do something like print out the name of the class.
===============================================================
If you skipped Project 3:
write 3 UDTs below that EACH have: 
        5 member variables
            the member variable names and types should be relevant to the work the UDT will perform.
            pick properties that can be represented with 'int float double bool char std::string'
        3 member functions with an arbitrary number of parameters
            give some of those parameters default values.
        constructors that initialize some of these member variables
            the remaining member variables should be initialized in-class
        for() or while() loops that modify member variables
 1) 2 of your 3 UDTs need to have a nested UDT.
    this nested UDT should fulfill the same requirements as above:
        5 member variables  
        3 member functions
        constructors and loops.
        
 2) Define your implementations of all functions OUTSIDE of the class. 
 NO IN-CLASS IMPLEMENTATION ALLOWED
 3) add destructors to all of your UDTs
        make the destructors do something like print out the name of the class.
===============================================================

 4) add 2 new UDTs that use only the types you copied above as member variables.

 5) add 2 member functions that use your member variables to each of these new UDTs

 6) Add constructors and destructors to these 2 new types that do stuff.  
        maybe print out the name of the class being destructed, or call a member function of one of the members.  be creative
 
 7) copy over your main() from the end of Project3 
        get it to compile with the types you copied over.
        remove any code in main() that uses types you didn't copy over.
 
 8) Instantiate your 2 UDT's from step 4) in the main() function at the bottom.
       call their member functions.
 
 9) After you finish, click the [run] button.  Clear up any errors or warnings as best you can.

 you can resolve any [-Wdeprecated] warnings by adding -Wno-deprecated to list of compiler arguments in the .replit file. all of the "-Wno" in that file are compiler arguments.
 You can resolve any [-Wpadded] warnings by adding -Wno-padded to the list of compiler arguments in the .replit file. all of the "-Wno" in that file are compiler arguments.

 Ignore the Atomic.h and LeakedObjectDetector.h files for now.
 You will use those in Part 3 of this project.

 */

/*
 copied UDT 1: Oscillator (actually a new UDT since I skipped Project 3)
 */

// later I would switch to Pi etc. as provided by JUCE
// and I would avoid this static variable and use of cmath. 

static double piTwice = 2 * 3.1415926535897932384626433832795;
static float piTwiceFloat = static_cast<float>(2 * 3.1415926535897932384626433832795);

struct Oscillator
{
    struct SingleCycleWaveform 
    {    
        SingleCycleWaveform (int numSupportPoints);
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
        float computeInterpolatedSample (float phase, float oscAngularVelocity, bool silent = true);

        //     2) transform shape in various ways
        void transformShape (bool flipPolarity = false, bool normalize = false, 
                             bool removeDcOffset = false, float warp = 0);

        //     3) dump waveform by generating samples at equidistant steps (as
        //        given by numSupportPoints) and showing them on the console.
        void dumpWaveform (float scaling = 1);

        // I need an internal helper function, 0 <= sampleIndex < numSupportPoints
        float getSampleAtIndex (int sampleIndex);
    };

    Oscillator (std::string oscillatorId, float sampleRate = 44100);
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
};

// Implementation of nested UDT Oscillator::SingleCycleWaveform:
Oscillator::SingleCycleWaveform::SingleCycleWaveform (int numberOfSupportPoints)
    : numSupportPoints(numberOfSupportPoints), phaseIncrement(piTwice / numSupportPoints)
{
    std::cout << "constructor SingleCycleWaveform" << std::endl; 
    
    // the constructor should init the array of equidistant sampled values,
    // but at these point we do not know about collections yet.
}

Oscillator::SingleCycleWaveform::~SingleCycleWaveform()
{
    std::cout << "destructor SingleCycleWaveform" << std::endl; 
}

//     1) compute interpolated sample, phase in [0..2 pi)
float Oscillator::SingleCycleWaveform
    ::computeInterpolatedSample (float phase, float oscAngularVelocity, bool silent)
{
    if (! silent)
    {
        std::cout << "oscAngularVelocity [" << oscAngularVelocity << "]  "
                     "=> should look up downsampled waveform in a mipmap";
    }    
    // we will ignore angularVelocity since for the sine wave, there are 
    // no partials to take care of

    // is it OK to use modern initializer notation for local variables or does it look awkward:
    
    float exactLookupPosition {phase / static_cast<float>(phaseIncrement)};
    
    int firstLookupIndex = static_cast<int>(exactLookupPosition);

    float secondLookupIndexWeight = exactLookupPosition - static_cast<float>(firstLookupIndex);
    
    if (firstLookupIndex == numSupportPoints)
    {
        // this should not be the case since nextPhase < piTwice, but perhaps
        // it can happen in rare cases due to limited floating point precision 
        firstLookupIndex = 0;
    }
    
    int secondLookupIndex = firstLookupIndex + 1;
    
    if (secondLookupIndex == numSupportPoints)
    {
        secondLookupIndex = 0;    
    }

    if (interpolationType == 0)
    {
        // linear interpolation
        return (1.0f - secondLookupIndexWeight) * getSampleAtIndex (firstLookupIndex) 
               + secondLookupIndexWeight * getSampleAtIndex (secondLookupIndex);
    }

    // else: no interpolation at all
    return getSampleAtIndex (firstLookupIndex);
}

// helper function getSampleAtIndex()
// (should rather be private)


//     2) transform shape in various ways
void Oscillator::SingleCycleWaveform
    ::transformShape (bool doFlipPolarity, bool normalize, 
                      bool removeDcOffset, float warp)
{
     // for the moment, we only set the flipPolarity and reversePhase flags
    flipPolarity = doFlipPolarity;
    std::cout << "transform shape: normalize [" << normalize << "], "
                 "removeDcOffset [" << removeDcOffset << "], "
                 "warp [" << warp << "] have no effect yet";
}

//     3) dump waveform by generating samples at equidistant steps 
// (as given by numSupportPoints) and showing them on the console.
void Oscillator::SingleCycleWaveform
    ::dumpWaveform (float scaling)
{
    
    for (int i = 0; i < numSupportPoints; ++i)
    {
        float sample = scaling * getSampleAtIndex (i);
        float phase = static_cast<float>(i * phaseIncrement);
        
        std::cout << "supportPoint [" << i << "], "
                     "phase [" << phase << "] "
                     "=> sample [" << sample << "]"
                  << std::endl;
    }
}

float Oscillator::SingleCycleWaveform::getSampleAtIndex (int sampleIndex)
{
    // This would normally look up the i-th precomputed value
    // in an array, but we do not know yet about arrays and
    // similar structures in C++.
    // no range check
    float rawSampleAtIndex = static_cast<float>(sin (sampleIndex * phaseIncrement));
    return (flipPolarity ? -rawSampleAtIndex : rawSampleAtIndex);
}

// Implementation of struct Oscillator

Oscillator::Oscillator (std::string id, float rate)
    : oscillatorId(id), sampleRate(rate)
{
    std::cout << "constructor Oscillator" << std::endl;
}

Oscillator::~Oscillator()
{
    std::cout << "destructor Oscillator" << std::endl; 
}

//     1) reset to given frequency and initial phase
void Oscillator::reset (float frequency, float initialPhase)
{
    angularVelocity = static_cast<float>(piTwice) * frequency / sampleRate;
    currentPhase = initialPhase;
}

//     2) generate sample
float Oscillator::generateSample()
{
    float sample = waveform.computeInterpolatedSample(currentPhase, angularVelocity);

    // prepare for next sample
    currentPhase += angularVelocity;
    
    if (currentPhase >= piTwiceFloat)
    {
        currentPhase -= piTwiceFloat;
    }
    
    return sample;
}

 //     3) dump a given number of samples to console Samples (optionally without changing internal state)
void Oscillator::dumpSamples (int numSamples, bool restoreCurrentPhase)
{
    float currentPhaseBeforeDumpSamples = currentPhase;
    
    for (int i = 0; i < numSamples; ++i)
    {
        float savedPhase = currentPhase;
        float sample = generateSample();
        
        std::cout << "index [" << i << "], "
                     "phase [" << savedPhase << "] "
                     "=> sample [" << sample << "]"
                  << std::endl;
    }

    if (restoreCurrentPhase)
    {
        currentPhase = currentPhaseBeforeDumpSamples;
    }
}


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
        EnvelopeParameters(float sustainLevel = 1);
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
        float applyShapePowerFunction (float value, bool computeInverseFunction = false);

        //     2) adjust parameters. midValuePoint is the normalized value in (0,1) 
        //         where the shape transform should reach 0.5
        //        CAUTION: must NOT be called when the EnvelopeGate is active 
        //        (i.e. in a state different from ENVELOPESTATE_OFF)
        void adjustParameters (double attackTimeInSeconds, double decayTimeInSeconds, float sustainLevel, 
                               double releaseTimeInSeconds, double midValuePoint = 0.5, double sampleRateInHz = 44100);

        //     3) something that requires a loop => dump shape
        void dumpShape (int numberOfSteps = 11);
    };

    // enclosing UDT2: EnvelopeGate
    EnvelopeGate(float sustainLevel = 1);
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
    bool triggerEnvelope (bool keyPressed, bool allowRetrigger = true); 

    //     2) get next sample for envelope gate
    float computeNextEnvelopeGateSample();

    //     3) dump envelope gate response, assuming that the key is being pressed for numSamplesKeyPressed
    void dumpEnvelopeGateResponse (int numSamplesKeyPressed, int displayEveryNthStep = 1, int maxStepsAllowed = 1000000);
};


///////////////////////////////////////////////////////
// Implementation of nested UDT: EnvelopeParameters:

EnvelopeGate::EnvelopeParameters::EnvelopeParameters(float sustainLevel)
    : normalizedSustainLevel (applyShapePowerFunction (sustainLevel, true))
{
    std::cout << "constructor EnvelopeParameters" << std::endl;
}

EnvelopeGate::EnvelopeParameters::~EnvelopeParameters()
{
    std::cout << "destructor EnvelopeParameters" << std::endl;    
}

// 1) transform value by applying shape power function (or inverse function depending on flag)
float EnvelopeGate::EnvelopeParameters
    ::applyShapePowerFunction (float value, bool computeInverseFunction)
{
    if (computeInverseFunction)
    {
        return powf (value, 1.0f / exponentOfShapePowerFunction);
    }

    // else: return result of applying power function with given shape exponent to value
    return value <= 0 ? 0 : powf (value, exponentOfShapePowerFunction);
}

// 2) adjust parameters. 
// midValuePoint is the normalized value in (0,1) where the shape transform should reach 0.5 
// CAUTION: must NOT be called when the EnvelopeGate is active 
// (i.e. in a state different from ENVELOPESTATE_OFF)

void EnvelopeGate::EnvelopeParameters
    ::adjustParameters (double attackTimeInSeconds, double decayTimeInSeconds, 
                        float sustainLevel, double releaseTimeInSeconds, 
                        double midValuePoint, double sampleRateInHz)
{
    attackTimeInSamples = static_cast<int>(attackTimeInSeconds * sampleRateInHz + 0.5);
    decayTimeInSamples = static_cast<int>(decayTimeInSeconds * sampleRateInHz + 0.5);
    releaseTimeInSamples = static_cast<int>(releaseTimeInSeconds * sampleRateInHz + 0.5);

    if (midValuePoint <= 0 || midValuePoint >= 1)
    {
        std::cout << "Invalid parameter [" << midValuePoint << "], "
                     "must be in between 0 and 1 (exclusive) "
                     "=> using default [0.5]" << std::endl;
        
        midValuePoint = 0.5;               
    }
    exponentOfShapePowerFunction = static_cast<float>(log (0.5) / log (midValuePoint));

    normalizedSustainLevel = applyShapePowerFunction (sustainLevel, true);
}

// 3) something that requires a loop => dump shape
void EnvelopeGate::EnvelopeParameters
    ::dumpShape (int numberOfSteps)
{
    if (numberOfSteps <= 2)
    {
        std::cout << "Invalid parameter [" << numberOfSteps << "], "
                     "using default [11] instead" << std::endl;
        numberOfSteps = 11;        
    }
    
    double stepWidth = 1.0 / (numberOfSteps - 1);

    for (int i = 0; i < numberOfSteps; ++i)
    {
        float normalizedValue = static_cast<float>(i * stepWidth);
        float transformedValue = applyShapePowerFunction(normalizedValue);
        
        std::cout << "step [" << i << "], "
                     "normalized value [" << normalizedValue << "] "
                     "=> transformed value [" << transformedValue << "]" 
                  << std::endl;
    }
}


///////////////////////////////////////////////////////
// Implementation of main UDT2: EnvelopeGate:

EnvelopeGate::EnvelopeGate (float sustainLevel)
    : envelopeParameters (sustainLevel)
{
    std::cout << "constructor EnvelopeGate" << std::endl;    
}

EnvelopeGate::~EnvelopeGate()
{
    std::cout << "destructor EnvelopeGate" << std::endl;        
}

// 1) trigger envelope (bool keyPressed, bool allowRetriggger)
// returns true if an active envelope has been retriggered by a new key press
bool EnvelopeGate::triggerEnvelope (bool keyPressed, bool allowRetrigger)
{
    if (! keyPressed)
    {
        if (envelopeState != ENVELOPESTATE_OFF 
            && envelopeState != ENVELOPESTATE_RELEASE)
        {
            // switch to release state 
            envelopeState = ENVELOPESTATE_RELEASE;
            normalizedTargetValueInCurrentState = 0;
            int releaseTimeInSamples = envelopeParameters.releaseTimeInSamples;
            
            if (releaseTimeInSamples > 0)
            {
                double normalizedDelta = 
                    static_cast<double>(-lastComputedNormalizedSample);
                normalizedDeltaPerStep = 
                    static_cast<float> (normalizedDelta / releaseTimeInSamples);
            }
            else
            {
                // slope is irrelevant in this case since we'll skip forward to OFF mode
                lastComputedNormalizedSample = normalizedTargetValueInCurrentState;
                normalizedDeltaPerStep = 0;
            }
        }
        return false;
    }
    // { keyPressed }
    
    if (envelopeState != ENVELOPESTATE_OFF && !allowRetrigger) 
    {
        std::cout << "Ignoring attempt to retrigger since allowRetrigger = false" 
                  << std::endl;
        return false;
    }

    // { envelopeState == ENVELOPESTATE_OFF || allowRetrigger }

    bool retriggerHappened = envelopeState != ENVELOPESTATE_OFF;
    
    if (! retriggerHappened)
    {
        lastComputedNormalizedSample = 0;
        // otherwise well keep the last value as a starting point for attack
    }

    // switch to ATTACK state
    envelopeState = ENVELOPESTATE_ATTACK;
    normalizedTargetValueInCurrentState = 1;
    int attackTimeInSamples = envelopeParameters.attackTimeInSamples;

    if (attackTimeInSamples > 0)
    {
        double normalizedDelta = 
            static_cast<double>(1 - lastComputedNormalizedSample);
        normalizedDeltaPerStep = 
            static_cast<float> (normalizedDelta / attackTimeInSamples);
    }
    else
    {
        // slope is irrelevant in this case since we'll skip forward to DECAY mode
        lastComputedNormalizedSample = normalizedTargetValueInCurrentState;
        normalizedDeltaPerStep = 0;
    }
    return retriggerHappened;
}


// 2) get next sample for envelope gate
// PLEASE NOTE: This member functions is pretty long and violates D.R.Y. 
// Obviously I'd normally split it up into smaller pieces! But here, 
// the UDT is limited to 3 member functions, so I'll keep it this way.

float EnvelopeGate::computeNextEnvelopeGateSample()
{
    if (envelopeState == ENVELOPESTATE_OFF)
    {
        return 0;        
    }

    if (envelopeState == ENVELOPESTATE_ATTACK)
    {
        if (lastComputedNormalizedSample >= normalizedTargetValueInCurrentState)
        {
            // target value reached  => switch to decay state
            envelopeState = ENVELOPESTATE_DECAY;
            normalizedTargetValueInCurrentState = envelopeParameters.normalizedSustainLevel;
            int decayTimeInSamples = envelopeParameters.decayTimeInSamples;
            
            if (decayTimeInSamples > 0)
            {
                double normalizedDelta = 
                    static_cast<double>(envelopeParameters.normalizedSustainLevel
                                        - lastComputedNormalizedSample);
                normalizedDeltaPerStep = 
                    static_cast<float> (normalizedDelta / decayTimeInSamples);
            }
            else
            {
                // slope is irrelevant in this case since we'll skip forward to SUSTAIN mode
                lastComputedNormalizedSample = normalizedTargetValueInCurrentState;
                normalizedDeltaPerStep = 0;
            }
        }
        else
        {
            // compute next value in decay state and return
            lastComputedNormalizedSample += normalizedDeltaPerStep;
            return envelopeParameters.applyShapePowerFunction (lastComputedNormalizedSample);
        }        
    }

    if (envelopeState == ENVELOPESTATE_DECAY)
    {
        if (lastComputedNormalizedSample <= normalizedTargetValueInCurrentState)
        {
            // target value reached => switch to sustain state.
            // TODO if sustain level is 0, we skip to off state:

            envelopeState = ENVELOPESTATE_SUSTAIN;
            normalizedTargetValueInCurrentState = envelopeParameters.normalizedSustainLevel;
            lastComputedNormalizedSample = normalizedTargetValueInCurrentState;
            normalizedDeltaPerStep = 0;
        }
        else
        {
            lastComputedNormalizedSample += normalizedDeltaPerStep;
            return envelopeParameters.applyShapePowerFunction (lastComputedNormalizedSample);
        }
    }

    if (envelopeState == ENVELOPESTATE_SUSTAIN)
    {
        // repeat sustain value. In this case, it would make sense to store the
        // result of applying the shape function as well
        return envelopeParameters.applyShapePowerFunction (lastComputedNormalizedSample);
    }

    // { envelopeState == ENVELOPESTATE_RELEASE }
    // { normalizedTargetValueInCurrentState == 0 }
    
    if (lastComputedNormalizedSample <= normalizedTargetValueInCurrentState)
    {
        // zero reached, move to off state
        envelopeState = ENVELOPESTATE_OFF;
        normalizedTargetValueInCurrentState = 0;
        lastComputedNormalizedSample = 0;
        normalizedDeltaPerStep = 0;
        return 0;
    }
    lastComputedNormalizedSample += normalizedDeltaPerStep;
    return envelopeParameters.applyShapePowerFunction (lastComputedNormalizedSample);
}


// 3) dump envelope gate response, assuming that the key is being pressed for numSamplesKeyPressed
void EnvelopeGate::dumpEnvelopeGateResponse (int numSamplesKeyPressed,
                                             int displayEveryNthStep, 
                                             int maxStepsAllowed)
{
    if (displayEveryNthStep <= 0)
    {
        std::cout << "Invalid parameter [" << displayEveryNthStep << "], "
                     "using default [1] instead" << std::endl;
        displayEveryNthStep = 1;
    }

    std::cout << "Triggering fresh envelope with key press" << std::endl;
    envelopeState = ENVELOPESTATE_OFF;
    triggerEnvelope (true);
    
    int i = 0;
    float currentEnvelopeSample {0};
    
    while (i < numSamplesKeyPressed)
    {
        currentEnvelopeSample = computeNextEnvelopeGateSample();
        
        if (i % displayEveryNthStep == 0)
        {
            std::cout << "step [" << i << "], EG sample [" << currentEnvelopeSample << "]" << std::endl;
        }
        ++i;
    }

    std::cout << "Switching to release phase" << std::endl;
    triggerEnvelope (false);
    
    // dump output as long as generated samples are not zero.
    // for safety reasons, the number of steps will be limited
    // by maxStepsAllowed
    
    while (i < maxStepsAllowed)
    {
        // by the D.R.Y. principle, this repeated block should be extracted
        // into its own member function, but I have already reached
        // the maximum number of functions allowed for the UDt so
        // I won't do it this time
        
        currentEnvelopeSample = computeNextEnvelopeGateSample();
        
        if (i % displayEveryNthStep == 0)
        {
            std::cout << "step [" << i << "], EG sample [" << currentEnvelopeSample << "]" << std::endl;

            if (currentEnvelopeSample <= 0)
            {
                break;
            }            
        }
        ++i;
    }

    if (currentEnvelopeSample <= 0)
    {
        std::cout << "Finished envelope dump (zero level reached)" << std::endl;
    }
    else
    {
        std::cout << "Finished envelope dump (max steps exceeded)" << std::endl;
    }
}




/*
 copied UDT 3: LowFrequencyOscillator / LFO (actually a new UDT since I skipped Project 3)
 UDT3 does not use a nested UDT.
 */
struct LowFrequencyOscillator
{
    LowFrequencyOscillator (double sampleRateInHz = 44100);
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
    void reset (double speedInHz = 1, double initialPhase = 0, double riseTimeInSeconds = 0);

    //    2) generate single sample. This is a bipolar LFO with samples in [-1, 1].
    float generateSample();

    //    3) dump LFO output to console
    void dumpToConsole (int totalNumSamples, int displayEveryNthStep = 1);
};

///////////////////////////////////////////////////////
// Implementation of UDT3: LowFrequencyOscillator

LowFrequencyOscillator::LowFrequencyOscillator (double sampleRate)
    : sampleRateInHz(sampleRate), phaseIncrementPerStep(piTwice / sampleRate)
{
    std::cout << "constructor LowFrequencyOscillator" << std::endl;        
}

LowFrequencyOscillator::~LowFrequencyOscillator()
{
    std::cout << "destructor LowFrequencyOscillator" << std::endl;            
}

// 3 things it can do:
// 1) reset. This retriggers the LFO and adjust several parameters
void LowFrequencyOscillator::reset (double speedInHz, double initialPhase, 
                                    double riseTimeInSeconds)
{
    phaseIncrementPerStep = speedInHz * piTwice / sampleRateInHz;
    currentPhase = initialPhase;
    
    if (riseTimeInSeconds < 0)
    {
        std::cout << "illegal negative rise time [" << riseTimeInSeconds << "] "
                     "=> using default [0] instead"
                  << std::endl;
        
        riseTimeInSeconds = 0;
    }
    
    amplitudeDeltaPerStep = riseTimeInSeconds / sampleRateInHz;
    currentAmplitude = (riseTimeInSeconds > 0 ? 0 : 1);
}

// 2) generate single sample. This is a bipolar LFO with samples in [-1, 1].
float LowFrequencyOscillator::generateSample()
{
    if (currentAmplitude < 1) 
    {
        currentAmplitude += amplitudeDeltaPerStep;
    }

    float unscaledSample = static_cast<float> (sin (currentPhase));    
    currentPhase += phaseIncrementPerStep;
    
    if (currentPhase >= piTwice)
    {
        currentPhase -= piTwice;
    }
    
    return static_cast<float>(currentAmplitude) * unscaledSample;
}

// 3) dump LFO output to console
void LowFrequencyOscillator::dumpToConsole (int totalNumSamples, 
                                            int displayEveryNthStep)
{
    for (int i = 0; i < totalNumSamples; ++i)
    {
        double savedPhase = currentPhase;
        float sample = generateSample();

        if (i % displayEveryNthStep == 0)
        {
            std::cout << "index [" << i << "], "
                         "phase [" << savedPhase << "] "
                         "=> sample [" << sample << "]"
                      << std::endl;
        }
    }
}


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
    void reset (float frequency, float initialPhaseOscA = 0);
    // 2) generate sample by mixing samples of both embedded oscillators
    float generateSample();
    // 3) dump given number of generated samples on console
    void dumpSamples(int numSamples, bool restoreCurrentPhase = false);
};

///////////////////////////////////////////////////////
// Implementation of UDT4: CompoundOscillator

CompoundOscillator::CompoundOscillator(float sampleRate)
    : oscA("oscA", sampleRate), oscB("oscB", sampleRate)
{
   std::cout << "constructor CompoundOscillator" << std::endl;            
}

CompoundOscillator::~CompoundOscillator()
{
   std::cout << "destructor CompoundOscillator" << std::endl;                
}


// 1) resets both oscillators at once and adjusts parameters
void CompoundOscillator::reset (float frequency, float initialPhaseOscA)
{
    float detuneFactor = (detuneInCent > 0 ? powf (2, detuneInCent / 1200) : 1);    
    
    oscA.reset (frequency / detuneFactor, initialPhaseOscA);
    
    float initialPhaseOscB = initialPhaseOscA + phaseOffsetOscillatorB;
    
    if (initialPhaseOscB > piTwiceFloat)
    {
        initialPhaseOscB -= piTwiceFloat;    
    }
    oscB.reset (frequency * detuneFactor, initialPhaseOscB);
}


// 2) generate sample by mixing samples of both embedded oscillators
float CompoundOscillator::generateSample()
{
    return (1.0f - balance) * oscA.generateSample() + balance * oscB.generateSample();
}


// 3) dump given number of generated samples on console
void CompoundOscillator::dumpSamples(int numSamples, bool restoreCurrentPhase)
{
    float currentPhaseOscABeforeDumpSamples = oscA.currentPhase;
    float currentPhaseOscBBeforeDumpSamples = oscB.currentPhase;

    for (int i = 0; i < numSamples; ++i)
    {
        float savedPhaseA = oscA.currentPhase;
        float savedPhaseB = oscB.currentPhase;
        float sample = generateSample();
        
        std::cout << "index [" << i << "], "
                     "phaseA [" << savedPhaseA << "] "
                     "phaseB [" << savedPhaseB << "] "
                     "=> sample [" << sample << "]"
                  << std::endl;
    }

    if (restoreCurrentPhase)
    {
        oscA.currentPhase = currentPhaseOscABeforeDumpSamples;
        oscB.currentPhase = currentPhaseOscBBeforeDumpSamples;
    }    
}


/*
 new UDT 5: Simple Mono-Synth (without Filter)
 with 2 member functions
 */
struct SimpleMonoSynth
{
    SimpleMonoSynth(double sampleRateInHz = 44100);
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
    void triggerNote (bool keyPressed = true, int midiNoteNumber = 34, float velocity = 1, double tuningInHz = 440);
    //   2) generate sample
    float generateSample();
    //   3) dump given number of samples on console. Shown step numbers can be shifted by offset.
    void dumpSamples (int numSteps, int firstStepOffset = 0);
};

///////////////////////////////////////////////////////
// Implementation of UDT5: SimpleMonoSynth

SimpleMonoSynth::SimpleMonoSynth (double sampleRateInHz)
    : oscillator ("osc", static_cast<float>(sampleRateInHz)), lfo (sampleRateInHz)
{
   std::cout << "constructor SimpleMonoSynth" << std::endl;                
}

SimpleMonoSynth::~SimpleMonoSynth()
{
   std::cout << "destructor SimpleMonoSynth" << std::endl;                    
}

// 1) trigger a note given midi pitch, velocity in 0..1. Trigger again with keyPressed = false for release of key.
void SimpleMonoSynth::triggerNote (bool keyPressed, int midiNoteNumber, float velocity,  
                                   double tuningInHz)
{
    if (! keyPressed)
    {
        envelopeGate.triggerEnvelope (false); 
        // all other settings will be ignored since we do not want to affect the release phase
        return;
    }
    // { keyPressed == true }
    
    // assuming that MIDI note number 34 corresponds to A4 concert pitch (usually 440 Hz):
    float frequency = static_cast<float> (tuningInHz * pow (2, (midiNoteNumber - 34) / 12.0));

    // avoid clicks for frequency change by keeping phase for currently playing note if we retrigger 
    // with a new note:
    float initialPhase = 
        (envelopeGate.envelopeState == EnvelopeGate::ENVELOPESTATE_OFF ? 0 :  oscillator.currentPhase);
    
    oscillator.reset (frequency, initialPhase);

    envelopeGate.triggerEnvelope (true, true);

    amplitudeOfPlayingNote = 0.6f + 0.4f * velocity;
    
    // for now, the LFO will be free running, so we won't restart it here.
}


// 2) generate sample
float SimpleMonoSynth::generateSample()
{
    float unscaledSample = oscillator.generateSample();
    float envelopeGateScaling = envelopeGate.computeNextEnvelopeGateSample();
    float lfoSample = lfo.generateSample();
    
    float lfoAmplitudeScaling = 1.0f - amountOfLfoLevelModulation
                                + (amountOfLfoLevelModulation * (0.5f + 0.5f * lfoSample));

    return amplitudeOfPlayingNote 
           * envelopeGateScaling
           * lfoAmplitudeScaling
           * unscaledSample;
}


// 3) dump given number of samples on console. Shown step numbers can be shifted by offset.
void SimpleMonoSynth::dumpSamples (int numSteps, int firstStepOffset)
{
    int stepLimit = numSteps + firstStepOffset;
    
    for (int i = firstStepOffset; i < stepLimit; ++i)
    {
        std::cout << "step [" << i << "] "
                  << "=> sample [" << generateSample() << "]"
                  << std::endl;
    }
}
        
/*
 MAKE SURE YOU ARE NOT ON THE MASTER BRANCH

 Commit your changes by clicking on the Source Control panel on the left, entering a message, and click [Commit and push].
 
 If you didn't already: 
    Make a pull request after you make your first commit
    pin the pull request link and this repl.it link to our DM thread in a single message.

 send me a DM to review your pull request when the project is ready for review.

 Wait for my code review.
 */

int main()
{
    {
        // block for testing UDT1: Oscillator and nested UDT SingleCycleWaveform
        
        // PLEASE NOTE: I will not test generateSample() here because it is already
        // called by the dump function (same for other UDTs).
        
        Oscillator oscillator {"osc1"};
        
        std::cout << "dumping 11 samples @ 4410 Hz:" << std::endl;
        oscillator.reset (4410);
        oscillator.dumpSamples (11, true);
        std::cout << std::endl;
    
        std::cout << "dumping nested SingleCycleWaveform" << std::endl;
        oscillator.waveform.dumpWaveform();
        
        std::cout << std::endl;
    }
        
    {
        // block for testing UDT2: EnvelopeGate and nested EnvelopeParameters

        std::cout << std::endl;
        
        EnvelopeGate envelopeGate {};

        std::cout << "dumping default linear transformation shape" << std::endl;
        envelopeGate.envelopeParameters.dumpShape (11);        

        std::cout << std::endl
                  << "changing transformation shape to midPoint at 0.7"
                  << std::endl;

        double attackTimeInSeconds  = 0.005;
        double decayTimeInSeconds   = 0.010;
        double releaseTimeInSeconds = 0.015;
        float sustainLevel = 0.5;
        double midValuePoint = 0.7;

        envelopeGate.envelopeParameters
                    .adjustParameters (attackTimeInSeconds, 
                                       decayTimeInSeconds, 
                                       sustainLevel, 
                                       releaseTimeInSeconds, 
                                       midValuePoint);

        std::cout << "dumping modified transformation shape" << std::endl;
        envelopeGate.envelopeParameters.dumpShape (11);        
        
        std::cout << std::endl;

        std::cout << "dumping output of envelope gate" << std::endl;
        envelopeGate.dumpEnvelopeGateResponse (882, 88, 1600);
        
        std::cout << std::endl;        
    }

    {
        // block for testing UDT3: LowFrequencyOscillator (no nested type)

        std::cout << std::endl;        

        LowFrequencyOscillator lfo {44100};

        double speedInHz = 1; 
        double initialPhase = 0; 
        double riseTimeInSeconds = 1;
        
        lfo.reset (speedInHz, initialPhase, riseTimeInSeconds);

        std::cout << "dumping LFO, speed 1 Hz, sample rate 44100 Hz, rise time 1s"
                  << std::endl;        
        
        lfo.dumpToConsole (99225, 11025);
        
        std::cout << std::endl;        
    }

    {
        // block for testing UDT4: CompoundOscillator

        std::cout << std::endl;        
        
        CompoundOscillator oscillator {44100};
        std::cout << "dumping 11 samples @ 4410 Hz:" << std::endl;
        oscillator.detuneInCent = 0;
        oscillator.phaseOffsetOscillatorB = piTwiceFloat / 4.0f;
        oscillator.reset (4410);
        // oscA has initialPhase 0 and oscB has initialPhase 90 deg.        
        oscillator.dumpSamples (11, true);
    
        std::cout << std::endl;    
    }

    {
        // block for testing UDT5: SimpleMonoSynth

        std::cout << std::endl;        

        double sampleRateInHz {44100};
        
        SimpleMonoSynth synth {sampleRateInHz};

        // in order to make visible if the LFO and the gate work,
        // I trigger a very high note at 11025 Hz that has 4 samples
        // 0, 1, 0, -1 per cycle.
        // so we can see in the output what happens to the 
        // amplitudes in each cycle of 4 samples.
        
        int midiNoteNumber = 34;
        double tuningInHz = sampleRateInHz / 4;
        float velocity = 1;

        // we start with showing the effect of the gate
        // by demonstrating the Attack, Decay, Sustain and Release.
        // The modulation amount of the LFO will be set to 0
        // and we will use full velocity (amplitude 1)
        // so that we only see the effect of the envelope gate:

        synth.amountOfLfoLevelModulation = 0;

        double attackTimeInSeconds = 4 / sampleRateInHz;
        double decayTimeInSeconds = 4 / sampleRateInHz; 
        float sustainLevel = 0.5f;
        double releaseTimeInSeconds = 4 / sampleRateInHz;
        
        synth.envelopeGate
             .envelopeParameters
             .adjustParameters (attackTimeInSeconds, decayTimeInSeconds, 
                                sustainLevel, releaseTimeInSeconds);

        std::cout << std::endl 
                  << "dump SimpleMonoSynth output in ADSR phases, no LFO" 
                  << std::endl;
        
        synth.triggerNote (true, midiNoteNumber, velocity, tuningInHz);
        
        // we should see 4 samples in attack phase, in decay phase, and in sustain phase each:
        synth.dumpSamples(12);
        
        // start the release phase and show 4 samples:
        
        synth.triggerNote (false);

        std::cout << "*note off has occurred*" << std::endl;

        synth.dumpSamples(4, 12);

        // finally let's demonstrate the effect of the LFO.

        std::cout << std::endl;

        synth = SimpleMonoSynth(sampleRateInHz);

        std::cout << std::endl 
                  << std::endl
                  << "dump SimpleMonoSynth output with gate at 1, full LFO amount" 
                  << std::endl;
        
        synth.envelopeGate
             .envelopeParameters
             .adjustParameters (0, 0, 1, 0);

        double lfoSpeedInHz = 16 / sampleRateInHz;        
        synth.lfo.reset (lfoSpeedInHz);        
        synth.amountOfLfoLevelModulation = 1;

        synth.triggerNote (true, midiNoteNumber, velocity, tuningInHz);
        
        // we should see a full modulation cycle of the LFO:
        synth.dumpSamples(17);
        
        std::cout << std::endl;        

    }
    std::cout << "good to go!" << std::endl;
}
