#include "CompoundOscillator.h"

#include "Proj5Constants.h"

#include <iostream>
#include <cmath>

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
void CompoundOscillator::reset (const float frequency, const float initialPhaseOscA)
{
    float detuneFactor = (detuneInCent > 0 ? powf (2, detuneInCent / 1200) : 1);    
    
    oscA.reset (frequency / detuneFactor, initialPhaseOscA);
    
    float initialPhaseOscB = initialPhaseOscA + phaseOffsetOscillatorB;
    
    if (initialPhaseOscB > Proj5Constants::piTwiceFloat)
    {
        initialPhaseOscB -= Proj5Constants::piTwiceFloat;    
    }
    oscB.reset (frequency * detuneFactor, initialPhaseOscB);
}


// 2) generate sample by mixing samples of both embedded oscillators
float CompoundOscillator::generateSample()
{
    return (1.0f - balance) * oscA.generateSample() + balance * oscB.generateSample();
}


// 3) dump given number of generated samples on console
void CompoundOscillator::dumpSamples(const int numSamples, const bool restoreCurrentPhase)
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

// print functions for members, Part 2
void CompoundOscillator::printBalance (const std::string& prefix) const
{
    std::cout << prefix << "balance: " << this->balance << std::endl;
}

void CompoundOscillator::printDetuneInCent (const std::string& prefix) const
{
    std::cout << prefix << "detuneInCent: " << this->detuneInCent << std::endl;
}

void CompoundOscillator::printPhaseOffsetOscillatorB (const std::string& prefix) const
{
    std::cout << prefix << "phaseOffsetOscillatorB: " << this->phaseOffsetOscillatorB << std::endl;
}

void CompoundOscillator::printOscA (const std::string& prefix) const
{
    this->oscA.printOscillator (prefix + "oscA.");
}

void CompoundOscillator::printOscB (const std::string& prefix) const
{
    this->oscB.printOscillator (prefix + "oscB.");
}
