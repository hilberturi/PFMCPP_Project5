#include "Oscillator.h"
#include "Proj5Constants.h"
#include <iostream>
#include <cmath>

// Implementation of nested UDT Oscillator::SingleCycleWaveform:
Oscillator::SingleCycleWaveform::SingleCycleWaveform (int numberOfSupportPoints)
    : numSupportPoints(numberOfSupportPoints), phaseIncrement(Proj5Constants::piTwice / numSupportPoints)
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
float Oscillator::SingleCycleWaveform::computeInterpolatedSample (float phase, float oscAngularVelocity, bool silent) const
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
void Oscillator::SingleCycleWaveform::transformShape (bool doFlipPolarity, bool normalize, 
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
    ::dumpWaveform (float scaling) const
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

float Oscillator::SingleCycleWaveform::getSampleAtIndex (int sampleIndex) const
{
    // This would normally look up the i-th precomputed value
    // in an array, but we do not know yet about arrays and
    // similar structures in C++.
    // no range check
    float rawSampleAtIndex = static_cast<float>(sin (sampleIndex * phaseIncrement));
    return (flipPolarity ? -rawSampleAtIndex : rawSampleAtIndex);
}

void Oscillator::SingleCycleWaveform::printName (const std::string& prefix) const
{
    std::cout << prefix << "name: " << this->name << std::endl;
}

void Oscillator::SingleCycleWaveform::printInterpolationType (const std::string& prefix) const
{
    std::cout << prefix << "interpolationType: " << this->interpolationType << std::endl;
}

void Oscillator::SingleCycleWaveform::printNumSupportPoints (const std::string& prefix) const
{
    std::cout << prefix << "numSupportPoints: " << this->numSupportPoints << std::endl;
}

void Oscillator::SingleCycleWaveform::printPhaseIncrement (const std::string& prefix) const
{
    std::cout << prefix << "phaseIncrement: " << this->phaseIncrement << std::endl;
}

void Oscillator::SingleCycleWaveform::printFlipPolarity (const std::string& prefix) const
{
    std::cout << prefix << "flipPolarity: " << this->flipPolarity << std::endl;
}

void Oscillator::SingleCycleWaveform::printWaveform (const std::string& prefix) const
{
    this->printName (prefix);
    this->printInterpolationType (prefix);
    this->printNumSupportPoints (prefix);
    this->printPhaseIncrement (prefix);
    this->printFlipPolarity (prefix);
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
    angularVelocity = static_cast<float>(Proj5Constants::piTwice) * frequency / sampleRate;
    currentPhase = initialPhase;
}

//     2) generate sample
float Oscillator::generateSample()
{
    float sample = waveform.computeInterpolatedSample(currentPhase, angularVelocity);

    // prepare for next sample
    currentPhase += angularVelocity;
    
    if (currentPhase >= Proj5Constants::piTwiceFloat)
    {
        currentPhase -= Proj5Constants::piTwiceFloat;
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

void Oscillator::printOscillatorId (const std::string& prefix) const
{
    std::cout << prefix << "oscillatorId: " << this->oscillatorId << std::endl;
}

void Oscillator::printSampleRate (const std::string& prefix) const
{
    std::cout << prefix << "sampleRate: " << this->sampleRate << std::endl;
}

void Oscillator::printAngularVelocity (const std::string& prefix) const
{
    std::cout << prefix << "angularVelocity: " << this->angularVelocity << std::endl;
}

void Oscillator::printCurrentPhase (const std::string& prefix) const
{
    std::cout << prefix << "currentPhase: " << this->currentPhase << std::endl;
}

void Oscillator::printWaveform (const std::string& prefix) const
{
    this->waveform.printWaveform (prefix + "waveform.");
}

void Oscillator::printOscillator (const std::string& prefix) const
{
    this->printOscillatorId (prefix);
    this->printSampleRate (prefix);
    this->printAngularVelocity (prefix);
    this->printCurrentPhase (prefix);
    this->printWaveform (prefix);
}
