#include "SimpleMonoSynth.h"

#include <iostream>
#include <cmath>

///////////////////////////////////////////////////////
// Implementation of UDT5: SimpleMonoSynth

SimpleMonoSynth::SimpleMonoSynth (const double sampleRateInHz)
    : oscillator ("osc", static_cast<float>(sampleRateInHz)), lfo (sampleRateInHz)
{
   std::cout << "constructor SimpleMonoSynth" << std::endl;                
}

SimpleMonoSynth::~SimpleMonoSynth()
{
   std::cout << "destructor SimpleMonoSynth" << std::endl;                    
}

// 1) trigger a note given midi pitch, velocity in 0..1. Trigger again with keyPressed = false for release of key.
void SimpleMonoSynth::triggerNote (const bool keyPressed, const int midiNoteNumber, const float velocity,  
                                   const double tuningInHz)
{
    if (! keyPressed)
    {
        envelopeGate.triggerEnvelope (false); 
        // all other settings will be ignored since we do not want to affect the release phase
        return;
    }
    // { keyPressed == true }
    
    // assuming that MIDI note number 69 corresponds to A4 concert pitch (usually 440 Hz):
    float frequency = static_cast<float> (tuningInHz * pow (2, (midiNoteNumber - 69) / 12.0));

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
    
    float lfoAmplitudeScaling = 1.0f 
                                - amountOfLfoLevelModulation
                                + amountOfLfoLevelModulation * (0.5f + 0.5f * lfoSample);

    return amplitudeOfPlayingNote 
           * envelopeGateScaling
           * lfoAmplitudeScaling
           * unscaledSample;
}

// 3) dump given number of samples on console. Shown step numbers can be shifted by offset.
void SimpleMonoSynth::dumpSamples (const int numSteps, const int firstStepOffset)
{
    int stepLimit = numSteps + firstStepOffset;
    
    for (int i = firstStepOffset; i < stepLimit; ++i)
    {
        std::cout << "step [" << i << "] "
                  << "=> sample [" << generateSample() << "]"
                  << std::endl;
    }
}

// print functions for members, Part 2
void SimpleMonoSynth::printAmountOfLfoLevelModulation (const std::string& prefix) const
{
    std::cout << prefix << "amountOfLfoLevelModulation: " << this->amountOfLfoLevelModulation << std::endl;
}

void SimpleMonoSynth::printAmplitudeOfPlayingNote (const std::string& prefix) const
{
    std::cout << prefix << "amplitudeOfPlayingNote: " << this->amplitudeOfPlayingNote << std::endl;
}

void SimpleMonoSynth::printOscillator (const std::string& prefix) const
{
    this->oscillator.printOscillator (prefix + "oscillator.");
}

void SimpleMonoSynth::printEnvelopeGate (const std::string& prefix) const
{
    this->envelopeGate.printEnvelopeGate (prefix + "envelopeGate.");
}

void SimpleMonoSynth::printLfo (const std::string& prefix) const
{
    this->lfo.printLowFrequencyOscillator (prefix + "lfo.");
}
