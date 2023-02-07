#include "Wrappers.h"

// I follow the remark in the JUCE ScopedPointer here that delete should know the complete types:

#include "Oscillator.h"
#include "EnvelopeGate.h"
#include "LowFrequencyOscillator.h"
#include "CompoundOscillator.h"
#include "SimpleMonoSynth.h"

#include <iostream>

// Wrapper classes added for Part 3

OscillatorWrapper::OscillatorWrapper (Oscillator* pointerToGetOwned) : oscPtr(pointerToGetOwned)
{
    std::cout << "constructor OscillatorWrapper" << std::endl; 
}

OscillatorWrapper::~OscillatorWrapper()
{
    std::cout << "destructor OscillatorWrapper" << std::endl; 
    delete oscPtr;   
}

EnvelopeGateWrapper::EnvelopeGateWrapper (EnvelopeGate* pointerToGetOwned) : egPtr(pointerToGetOwned)
{
    std::cout << "constructor EnvelopeGateWrapper" << std::endl; 
}

EnvelopeGateWrapper::~EnvelopeGateWrapper()
{
    std::cout << "destructor EnvelopeGateWrapper" << std::endl; 
    delete egPtr;
}

LowFrequencyOscillatorWrapper::LowFrequencyOscillatorWrapper (LowFrequencyOscillator* pointerToGetOwned) : lfoPtr(pointerToGetOwned)
{
    std::cout << "constructor LowFrequencyOscillatorWrapper" << std::endl; 
}

LowFrequencyOscillatorWrapper::~LowFrequencyOscillatorWrapper()
{
    std::cout << "destructor LowFrequencyOscillatorWrapper" << std::endl; 
    delete lfoPtr;   
}

CompoundOscillatorWrapper::CompoundOscillatorWrapper (CompoundOscillator* pointerToGetOwned) : coscPtr(pointerToGetOwned)
{
    std::cout << "constructor CompoundOscillatorWrapper" << std::endl; 
}

CompoundOscillatorWrapper::~CompoundOscillatorWrapper()
{
    std::cout << "destructor CompoundOscillatorWrapper" << std::endl; 
    delete coscPtr;   
}

SimpleMonoSynthWrapper::SimpleMonoSynthWrapper (SimpleMonoSynth* pointerToGetOwned) : synthPtr(pointerToGetOwned)
{
    std::cout << "constructor SimpleMonoSynthWrapper" << std::endl; 
}

SimpleMonoSynthWrapper::~SimpleMonoSynthWrapper()
{
    std::cout << "destructor SimpleMonoSynthWrapper" << std::endl; 
    delete synthPtr;   
}
