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
    // types such as 'int', so I will define some constant expressions 
    // instead of introducing an enum in order to comply with instructions:
    static constexpr int ENVELOPESTATE_OFF {0};
    static constexpr int ENVELOPESTATE_ATTACK {1};
    static constexpr int ENVELOPESTATE_DECAY {2};
    static constexpr int ENVELOPESTATE_SUSTAIN {3};
    static constexpr int ENVELOPESTATE_RELEASE {4};

    // UDT2: Nested UDT: Envelope Gate Parameters
    struct EnvelopeGateParameters
    {
        EnvelopeGateParameters();
        ~EnvelopeGateParameters();

        // 5 properties:
        //     1) attack time in samples (int)
        int attackTimeInSamples;
        //     2) decay time in samples (int)
        int decayTimeInSamples;
        //     3) normalized sustain level (float)
        float normalizedSustainLevel;
        //     4) release time in samples (int)
        int releaseTimeInSamples;
        //     5) skew factor / shape (float) 
        //        point in (0..1) at which a level of 0.5 shall be reached
        float exponentOfShapePowerFunction = 1; // default corresponds to linear shape

        // 3 things it can do:
        //     1) transform value by applying shape power function (or inverse)
        float transformValueByApplyingShapePowerFunction (float value, bool computeInverseFunction = false);

        //     2) adjust parameters (with values given in Hz and sample rate)
        // Todo: really use normalized sustain level here?
        // Todo: really supply sample rate here?
        void adjustParameters (float attackTimeInSeconds, float decayTimeInSeconds, float normalizedSustainLevel, 
                               float releaseTimeInSeconds, float skewFactor = 0.5f, float sampleRateInHz = 44100);

        //     3) something that requires a loop => dump shape
        void dumpShape (int numberOfSteps);
    };

    // enclosing UDT2: EnvelopeGate
    EnvelopeGate();
    ~EnvelopeGate();

    // 5 properties:
    //     1) Envelope Gate Parameters (nested UDT)
    EnvelopeGateParameters envelopeGateParameters;
    //     2) envelope state (int) 0: off, 1:attack, 2:decay, 3:sustain, 4:release
    //        obviously one should use an enum here but we do not know about enums yet.
    int envelopeState {0};
    //     3) current time in state in samples
    int currentTimeInStateInSamples {0};
    //     4) TODO
    //     5) TODO
    // 3 things it can do:
    //     1) trigger envelope (bool keyPressed, bool allowRetriggger)
    bool triggerEnvelope (bool keyPressed, bool allowRetrigger = false); // returns true if an active envelope has been retriggered by a new key press

    //     2) get next sample for envelope gate
    float computeNextEnvelopeGateSample();

    //     3) dump envelope gate response, assuming that the key is being pressed for numSamplesKeyPressed
    void dumpEnvelopeGateResponse (int numSamplesKeyPressed);
};

// Implementation of nested UDT: EnvelopeGateParameters:

EnvelopeGate::EnvelopeGateParameters::EnvelopeGateParameters()
{
    std::cout << "constructor EnvelopeGateParameters" << std::endl;
}

EnvelopeGate::EnvelopeGateParameters::~EnvelopeGateParameters()
{
    std::cout << "destructor EnvelopeGateParameters" << std::endl;    
}

// 1) transform value by applying shape power function (or inverse function depending on flag)
float EnvelopeGate::EnvelopeGateParameters
    ::transformValueByApplyingShapePowerFunction (float value, bool computeInverseFunction)
{
    if (computeInverseFunction)
    {
        return powf (value, 1.0f / exponentOfShapePowerFunction);
    }

    // else: return result of applying power function with given shape exponent to value
    return powf (value, exponentOfShapePowerFunction);
}

// 2) adjust parameters (with values given in Hz and sample rate)
// Todo: really use normalized sustain level here?
// Todo: really supply sample rate here?
void EnvelopeGate::EnvelopeGateParameters
    ::adjustParameters (float attackTimeInSeconds, float decayTimeInSeconds, 
                        float newNormalizedSustainLevel, float releaseTimeInSeconds, 
                        float skewFactor, float sampleRateInHz)
{
    
}

// 3) something that requires a loop => dump shape
void EnvelopeGate::EnvelopeGateParameters::dumpShape (int numberOfSteps)
{
    
}

// Implementation of main UDT2: EnvelopeGate:

EnvelopeGate::EnvelopeGate()
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
            envelopeState = ENVELOPESTATE_RELEASE;
            currentTimeInStateInSamples = 0;
        }
        return false;
    }

    bool retriggerHappened = envelopeState != ENVELOPESTATE_OFF 
                             && allowRetrigger;
    
    envelopeState = ENVELOPESTATE_ATTACK;

    return retriggerHappened;
}

// 2) get next sample for envelope gate
float EnvelopeGate::computeNextEnvelopeGateSample()
{
    if (envelopeState == ENVELOPESTATE_OFF)
    {
        return 0;        
    }
    if (envelopeState == ENVELOPESTATE_ATTACK)
    {
        
    }
    if (envelopeState == ENVELOPESTATE_DECAY)
    {
        
    }
    if (envelopeState == ENVELOPESTATE_SUSTAIN)
    {
        
    }
    if (envelopeState == ENVELOPESTATE_RELEASE)
    {
        
    }
    // all cases have been covered
    return 0;
}

// 3) dump envelope gate response, assuming that the key is being pressed for numSamplesKeyPressed
void EnvelopeGate::dumpEnvelopeGateResponse (int numSamplesKeyPressed)
{
    // TODO: note-on trigger 
    for (int i = 0; i < numSamplesKeyPressed; ++i)
    {
        
    }

    // TODO: key-press false trigger 
    // TODO: dump output as long as generated samples are not zero.
    
}




/*
 copied UDT 3:
 */

/*
 new UDT 4:
 with 2 member functions
 */

/*
 new UDT 5:
 with 2 member functions
 */

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

        Oscillator oscillator {"osc1"};
        
        std::cout << "dumping 10 samples @ 4410 Hz:" << std::endl;
        oscillator.reset (4410);
        oscillator.dumpSamples (10, true);
        std::cout << std::endl;
    
        std::cout << "dumping nested SingleCycleWaveform" << std::endl;
        oscillator.waveform.dumpWaveform();
    }
        
    
    std::cout << "good to go!" << std::endl;
}
