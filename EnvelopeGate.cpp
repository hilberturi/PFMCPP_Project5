#include "EnvelopeGate.h"
#include <iostream>
#include <cmath>

///////////////////////////////////////////////////////
// Implementation of nested UDT: EnvelopeParameters:

EnvelopeGate::EnvelopeParameters::EnvelopeParameters(const float sustainLevel)
    : normalizedSustainLevel (applyShapePowerFunction (sustainLevel, true))
{
    std::cout << "constructor EnvelopeParameters" << std::endl;
}

EnvelopeGate::EnvelopeParameters::~EnvelopeParameters()
{
    std::cout << "destructor EnvelopeParameters" << std::endl;    
}

// 1) transform value by applying shape power function (or inverse function depending on flag)
float EnvelopeGate::EnvelopeParameters::applyShapePowerFunction (const float value, const bool computeInverseFunction) const
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

void EnvelopeGate::EnvelopeParameters::adjustParameters (const double attackTimeInSeconds, const double decayTimeInSeconds, 
                                                         const float sustainLevel, const double releaseTimeInSeconds, 
                                                         double midValuePoint, const double sampleRateInHz)
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
void EnvelopeGate::EnvelopeParameters::dumpShape (int numberOfSteps) const
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

// print functions for members, Part 2
void EnvelopeGate::EnvelopeParameters::printAttackTimeInSamples (const std::string& prefix) const
{
    std::cout << prefix << "attackTimeInSamples: " << this->attackTimeInSamples << std::endl;
}

void EnvelopeGate::EnvelopeParameters::printDecayTimeInSamples (const std::string& prefix) const
{
    std::cout << prefix << "decayTimeInSamples: " << this->decayTimeInSamples << std::endl;
}

void EnvelopeGate::EnvelopeParameters::printNormalizedSustainLevel (const std::string& prefix) const
{
    std::cout << prefix << "normalizedSustainLevel: " << this->normalizedSustainLevel << std::endl;
}

void EnvelopeGate::EnvelopeParameters::printReleaseTimeInSamples (const std::string& prefix) const
{
    std::cout << prefix << "releaseTimeInSamples: " << this->releaseTimeInSamples << std::endl;
}

void EnvelopeGate::EnvelopeParameters::printExponentOfShapePowerFunction (const std::string& prefix) const
{
    std::cout << prefix << "exponentOfShapePowerFunction: " << this->exponentOfShapePowerFunction << std::endl;
}

void EnvelopeGate::EnvelopeParameters::printEnvelopeParameters (const std::string& prefix) const
{
    std::string paramsPrefix {prefix + "envelopeParameters."};    
    this->printAttackTimeInSamples (prefix);
    this->printDecayTimeInSamples(prefix);
    this->printNormalizedSustainLevel(prefix);
    this->printReleaseTimeInSamples(prefix);
    this->printExponentOfShapePowerFunction(prefix);
}

///////////////////////////////////////////////////////
// Implementation of main UDT2: EnvelopeGate:

EnvelopeGate::EnvelopeGate (const float sustainLevel)
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
bool EnvelopeGate::triggerEnvelope (const bool keyPressed, const bool allowRetrigger)
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
                double normalizedDelta = static_cast<double>(-lastComputedNormalizedSample);
                normalizedDeltaPerStep = static_cast<float>(normalizedDelta / releaseTimeInSamples);
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
        double normalizedDelta = static_cast<double>(1 - lastComputedNormalizedSample);
        normalizedDeltaPerStep = static_cast<float> (normalizedDelta / attackTimeInSamples);
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
                double normalizedDelta = static_cast<double>(envelopeParameters.normalizedSustainLevel
                                        - lastComputedNormalizedSample);
                normalizedDeltaPerStep = static_cast<float> (normalizedDelta / decayTimeInSamples);
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
void EnvelopeGate::dumpEnvelopeGateResponse (const int numSamplesKeyPressed,
                                             int displayEveryNthStep, 
                                             const int maxStepsAllowed)
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


// print functions for members, Part 2
void EnvelopeGate::printEnvelopeState (const std::string& prefix) const
{
    std::cout << prefix << "envelopeState: " << this->envelopeState << std::endl;
}

void EnvelopeGate::printNormalizedTargetValueInCurrentState (const std::string& prefix) const
{
    std::cout << prefix << "normalizedTargetValueInCurrentState: " 
              << this->normalizedTargetValueInCurrentState << std::endl;
}

void EnvelopeGate::printNormalizedDeltaPerStep (const std::string& prefix) const
{
    std::cout << prefix << "normalizedDeltaPerStep: " << this->normalizedDeltaPerStep << std::endl;
}

void EnvelopeGate::printLastComputedNormalizedSample (const std::string& prefix) const
{
    std::cout << prefix << "lastComputedNormalizedSample: " << this->lastComputedNormalizedSample << std::endl;
}

void EnvelopeGate::printEnvelopeParameters (const std::string& prefix) const
{
    this->envelopeParameters.printEnvelopeParameters (prefix + "envelopeParameters.");
}

void EnvelopeGate::printEnvelopeGate (const std::string& prefix) const
{
    this->printEnvelopeState (prefix);
    this->printNormalizedTargetValueInCurrentState (prefix);
    this->printNormalizedDeltaPerStep (prefix);
    this->printLastComputedNormalizedSample (prefix);
    this->printEnvelopeParameters (prefix);
}
