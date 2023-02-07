#include "LowFrequencyOscillator.h"

#include "Proj5Constants.h"
#include <iostream>
#include <cmath>

///////////////////////////////////////////////////////
// Implementation of UDT3: LowFrequencyOscillator

LowFrequencyOscillator::LowFrequencyOscillator (const double sampleRate)
    : sampleRateInHz(sampleRate), phaseIncrementPerStep(Proj5Constants::piTwice / sampleRate)
{
    std::cout << "constructor LowFrequencyOscillator" << std::endl;        
}

LowFrequencyOscillator::~LowFrequencyOscillator()
{
    std::cout << "destructor LowFrequencyOscillator" << std::endl;            
}

// 3 things it can do:
// 1) reset. This retriggers the LFO and adjust several parameters
void LowFrequencyOscillator::reset (const double speedInHz, const double initialPhase, 
                                    double riseTimeInSeconds)
{
    phaseIncrementPerStep = speedInHz * Proj5Constants::piTwice / sampleRateInHz;
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
    
    if (currentPhase >= Proj5Constants::piTwice)
    {
        currentPhase -= Proj5Constants::piTwice;
    }
    
    return static_cast<float>(currentAmplitude) * unscaledSample;
}

// 3) dump LFO output to console
void LowFrequencyOscillator::dumpToConsole (const int totalNumSamples, 
                                            const int displayEveryNthStep)
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

// print functions for members, Part 2
void LowFrequencyOscillator::printSampleRateInHz (const std::string& prefix) const
{
    std::cout << prefix << "sampleRateInHz: " << this->sampleRateInHz << std::endl;
}

void LowFrequencyOscillator::printPhaseIncrementPerStep (const std::string& prefix) const
{
    std::cout << prefix << "phaseIncrementPerStep: " << this->phaseIncrementPerStep << std::endl;
}

void LowFrequencyOscillator::printAmplitudeDeltaPerStep (const std::string& prefix) const
{
    std::cout << prefix << "amplitudeDeltaPerStep: " << this->amplitudeDeltaPerStep << std::endl;
}

void LowFrequencyOscillator::printCurrentPhase (const std::string& prefix) const
{
    std::cout << prefix << "currentPhase: " << this->currentPhase << std::endl;
}

void LowFrequencyOscillator::printCurrentAmplitude (const std::string& prefix) const
{
    std::cout << prefix << "currentAmplitude: " << this->currentAmplitude << std::endl;
}

void LowFrequencyOscillator::printLowFrequencyOscillator (const std::string& prefix) const
{
    this->printSampleRateInHz (prefix);
    this->printPhaseIncrementPerStep (prefix);
    this->printAmplitudeDeltaPerStep (prefix);
    this->printCurrentPhase (prefix);
    this->printCurrentAmplitude (prefix);
}

