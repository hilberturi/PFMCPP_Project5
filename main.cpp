/*
 Project 5: Part 4 / 4
 video: Chapter 3 Part 7

 Create a branch named Part4
 
 Don't #include what you don't use
 
 1) Your task is to refactor your Chapter 3 Part 4 task into separate source and header files.
         Add files via the pane on the left.
 
 2) Put all of your declarations for each class in .h files
         One header file per class ( i.e. Raider.h for a class named "Raider" )
         Don't forget the '#pragma once'
 
 3) Put all of your implementations in .cpp files.
         one cpp file per class ( i.e. Raider.cpp for a class named "Raider" )
 
 4) Put all of your Wrapper classes in a single Wrappers.h file
         if you implemented your wrapper class functions in-class, you'll need to move them to Wrappers.cpp, which goes along with instruction 5): 
 
 5) NO IN-CLASS IMPLEMENTATION ALLOWED.
         the only exception is the existing Atomic.h and LeakedObjectDetector.h
        Chances are extremely high that you implemented your 
        wrapper class functions in-class, because that is what 
        everyone does after watching the video where I implemented 
        them in-class.
 
 6) for every .cpp file you have to make, insert it into the .replit file after 'main.cpp'.  Don't forget the spaces between file names.
 If you need help with this step, send me a DM.

 7) When you add the #include statements for your refactored source files into main.cpp: 
        '#include "Wrappers.h"' should be the first file that you include after LeakedObjectDetector.h
 
 8) Go through every cpp file and header file. check all function implementations for the following:
        Can this function argument be declared as 'const'? 
        Can this function be declared as 'const'?
        You learned about 'const' function arguments and 'const' functions in Project 5 Part 3
        As a refresher:
            If you aren't modifying the passed-in object inside the function, pass by 'const reference'.
            Marking a function parameter as 'const' means that you are promising that the parameter will not be modified.
            Additionally, you can mark class member functions as 'const'
            If you do this, you are promising that the member function will not modify any member variables.

        Mark every member function that is not modifying any member variables as 'const'
        Mark every function parameter that is not modified inside the function as 'const'
*/
#if false
//a function where the argument is passed by const-ref.
void Foo::someMemberFunction(const Axe& axe);

//a member function that is marked const, meaning it will not modify any member variables of the 'Axe' class.
void Axe::aConstMemberFunction() const { }
#endif
/*
 9) click the [run] button.  Clear up any errors or warnings as best you can.
 
 Remember, your Chapter 3 Part 4 task worked when it was all in one file. so you shouldn't need to edit any of it.  
         just split it up into source files and provide the appropriate #include directives.
         tip: you cannot forward-declare nested types!  
         The file that a nested type is defined in MUST be included wherever that nested type is written.
 */

#include "LeakedObjectDetector.h"

#include "Wrappers.h"

#include "Proj5Constants.h"
#include "Oscillator.h"
#include "EnvelopeGate.h"
#include "LowFrequencyOscillator.h"
#include "CompoundOscillator.h"
#include "SimpleMonoSynth.h"

#include <iostream>
#include <string>
#include <cmath>


/*
 MAKE SURE YOU ARE NOT ON THE MASTER BRANCH

 Commit your changes by clicking on the Source Control panel on the left, entering a message, and click [Commit and push].
 
 If you didn't already: 
    Make a pull request after you make your first commit
    pin the pull request link and this repl.it link to our DM thread in a single message.

 send me a DM to review your pull request when the project is ready for review.

 Wait for my code review.
 */

/*
 show lists of generated samples for UDTs 1-2 given a configuration
 that makes it easy to judge if the implementation works.
 Extracted from main Part1 since I want main to only print
 UDT members.
*/
void dumpUDTBehaviour() 
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
        oscillator.phaseOffsetOscillatorB = Proj5Constants::piTwiceFloat / 4.0f;
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
        
        int midiNoteNumber = 69;
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
                  << "dump SimpleMonoSynth output, gate at 1, full LFO amount, 1/4 LFO cycle" 
                  << std::endl;
        
        synth.envelopeGate
             .envelopeParameters
             .adjustParameters (0, 0, 1, 0);

        double lfoSpeedInHz = sampleRateInHz / 64;        
        synth.lfo.reset (lfoSpeedInHz);        
        synth.amountOfLfoLevelModulation = 1;

        synth.triggerNote (true, midiNoteNumber, velocity, tuningInHz);
        
        // we should see 1/4th of the modulation cycle of the LFO:
        synth.dumpSamples (17);
        
        std::cout << std::endl;        

    }    
}


int main()
{
    bool showUDTBehavior = false;
    
    if (showUDTBehavior)
    {
        // outputs as they looked like in Part1
        dumpUDTBehaviour();
    }

    // print members from here and from member functions
    // solution for Part2, with added wrappers for Part3
    
    {
        // block for testing UDT1: Oscillator and nested UDT SingleCycleWaveform
        
        std::string oscPrefix {"oscillator."};
        std::string wavePrefix {oscPrefix + "waveform."};
        
        OscillatorWrapper osc {new Oscillator {"osc1"}};
        osc.oscPtr->reset (4410);

        std::cout << std::endl;
        
        std::cout << oscPrefix << "oscillatorId: " << osc.oscPtr->oscillatorId << std::endl; 
        osc.oscPtr->printOscillatorId (oscPrefix);
        std::cout << oscPrefix << "sampleRate: " << osc.oscPtr->sampleRate << std::endl;
        osc.oscPtr->printSampleRate (oscPrefix);
        std::cout << oscPrefix << "angularVelocity: " << osc.oscPtr->angularVelocity << std::endl;
        osc.oscPtr->printAngularVelocity (oscPrefix);
        std::cout << oscPrefix << "currentPhase: " << osc.oscPtr->currentPhase << std::endl;
        osc.oscPtr->printCurrentPhase (oscPrefix);
        std::cout << wavePrefix << "name: " << osc.oscPtr->waveform.name << std::endl;
        osc.oscPtr->waveform.printName (wavePrefix);
        std::cout << wavePrefix << "interpolationType: " << osc.oscPtr->waveform.interpolationType << std::endl;
        osc.oscPtr->waveform.printInterpolationType (wavePrefix);
        std::cout << wavePrefix << "numSupportPoints: " << osc.oscPtr->waveform.numSupportPoints << std::endl;
        osc.oscPtr->waveform.printNumSupportPoints (wavePrefix);
        std::cout << wavePrefix << "phaseIncrement: " << osc.oscPtr->waveform.phaseIncrement << std::endl;
        osc.oscPtr->waveform.printPhaseIncrement (wavePrefix);
        std::cout << wavePrefix << "flipPolarity: " << osc.oscPtr->waveform.flipPolarity << std::endl;
        osc.oscPtr->waveform.printFlipPolarity (wavePrefix);
        
        std::cout << std::endl;
    }
        
    {
        // block for testing UDT2: EnvelopeGate and nested EnvelopeParameters

        std::cout << std::endl;
        
        EnvelopeGateWrapper envelopeGate {new EnvelopeGate};

        double attackTimeInSeconds  = 0.005;
        double decayTimeInSeconds   = 0.010;
        double releaseTimeInSeconds = 0.015;
        float sustainLevel = 0.5;
        double midValuePoint = 0.7;

        envelopeGate.egPtr->envelopeParameters
                    .adjustParameters (attackTimeInSeconds, 
                                       decayTimeInSeconds, 
                                       sustainLevel, 
                                       releaseTimeInSeconds, 
                                       midValuePoint);
        
        envelopeGate.egPtr->triggerEnvelope (true);

        std::cout << std::endl;

        std::string egPrefix {"envelopeGate."};
        std::string paramsPrefix {egPrefix + "envelopeParameters."};
        
        std::cout << egPrefix << "envelopeState: " 
                  << envelopeGate.egPtr->envelopeState << std::endl;
        envelopeGate.egPtr->printEnvelopeState (egPrefix);
        std::cout << egPrefix << "normalizedTargetValueInCurrentState: " 
                  << envelopeGate.egPtr->normalizedTargetValueInCurrentState << std::endl;        
        envelopeGate.egPtr->printNormalizedTargetValueInCurrentState (egPrefix);    
        std::cout << egPrefix << "normalizedDeltaPerStep: " 
                  << envelopeGate.egPtr->normalizedDeltaPerStep << std::endl;        
        envelopeGate.egPtr->printNormalizedDeltaPerStep(egPrefix);    
        std::cout << egPrefix << "lastComputedNormalizedSample: " 
                  << envelopeGate.egPtr->lastComputedNormalizedSample << std::endl;    
        envelopeGate.egPtr->printLastComputedNormalizedSample (egPrefix);    
        std::cout << paramsPrefix << "attackTimeInSamples: " 
                  << envelopeGate.egPtr->envelopeParameters.attackTimeInSamples << std::endl;        
        envelopeGate.egPtr->envelopeParameters.printAttackTimeInSamples (paramsPrefix);    
        std::cout << paramsPrefix << "decayTimeInSamples: " 
                  << envelopeGate.egPtr->envelopeParameters.decayTimeInSamples << std::endl;        
        envelopeGate.egPtr->envelopeParameters.printDecayTimeInSamples (paramsPrefix);    
        std::cout << paramsPrefix << "normalizedSustainLevel: " 
                  << envelopeGate.egPtr->envelopeParameters.normalizedSustainLevel << std::endl;        
        envelopeGate.egPtr->envelopeParameters.printNormalizedSustainLevel (paramsPrefix);    
        std::cout << paramsPrefix << "releaseTimeInSamples: " 
                  << envelopeGate.egPtr->envelopeParameters.releaseTimeInSamples << std::endl;        
        envelopeGate.egPtr->envelopeParameters.printReleaseTimeInSamples (paramsPrefix);    
        std::cout << paramsPrefix << "exponentOfShapePowerFunction: " 
                  << envelopeGate.egPtr->envelopeParameters.exponentOfShapePowerFunction << std::endl;        
        envelopeGate.egPtr->envelopeParameters.printExponentOfShapePowerFunction(paramsPrefix);    
        
        std::cout << std::endl;        
    }

    {
        // block for testing UDT3: LowFrequencyOscillator (no nested type)

        std::cout << std::endl;        

        LowFrequencyOscillatorWrapper lfo {new LowFrequencyOscillator {44100}};

        double speedInHz = 1; 
        double initialPhase = 0; 
        double riseTimeInSeconds = 1;
        
        lfo.lfoPtr->reset (speedInHz, initialPhase, riseTimeInSeconds);
                
        std::cout << std::endl;        

        std::string lfoPrefix {"lfo."};

        std::cout << lfoPrefix << "sampleRateInHz: " << lfo.lfoPtr->sampleRateInHz << std::endl; 
        lfo.lfoPtr->printSampleRateInHz (lfoPrefix);
        std::cout << lfoPrefix << "phaseIncrementPerStep: " << lfo.lfoPtr->phaseIncrementPerStep << std::endl;        
        lfo.lfoPtr->printPhaseIncrementPerStep (lfoPrefix);
        std::cout << lfoPrefix << "amplitudeDeltaPerStep: " << lfo.lfoPtr->amplitudeDeltaPerStep << std::endl;        
        lfo.lfoPtr->printAmplitudeDeltaPerStep (lfoPrefix);
        std::cout << lfoPrefix << "currentPhase: " << lfo.lfoPtr->currentPhase << std::endl;        
        lfo.lfoPtr->printCurrentPhase (lfoPrefix);
        std::cout << lfoPrefix << "currentAmplitude: " << lfo.lfoPtr->currentAmplitude << std::endl;        
        lfo.lfoPtr->printCurrentAmplitude (lfoPrefix);

        std::cout << std::endl;        
    }

    {
        // block for testing UDT4: CompoundOscillator

        std::cout << std::endl;        
        
        CompoundOscillator osc {44100};
        osc.detuneInCent = 5;
        osc.phaseOffsetOscillatorB = Proj5Constants::piTwiceFloat / 4.0f;
        osc.reset (4410);

        std::cout << std::endl;        

        std::string oscPrefix {"compoundOsc."};
        std::string oscAPrefix {oscPrefix + "oscA."};
        std::string oscBPrefix {oscPrefix + "oscB."};
        std::string waveAPrefix {oscAPrefix + "waveform."};
        std::string waveBPrefix {oscBPrefix + "waveform."};

        std::cout << oscPrefix << "balance: " << osc.balance << std::endl;
        osc.printBalance (oscPrefix);
        std::cout << oscPrefix << "detuneInCent: " << osc.detuneInCent << std::endl;        
        osc.printDetuneInCent (oscPrefix);        
        std::cout << oscPrefix << "phaseOffsetOscillatorB: " << osc.phaseOffsetOscillatorB << std::endl; 
        osc.printPhaseOffsetOscillatorB (oscPrefix);
        
        std::cout << oscAPrefix << "oscillatorId: " << osc.oscA.oscillatorId << std::endl;        
        osc.oscA.printOscillatorId (oscAPrefix);
        std::cout << oscAPrefix << "sampleRate: " << osc.oscA.sampleRate << std::endl;
        osc.oscA.printSampleRate (oscAPrefix);
        std::cout << oscAPrefix << "angularVelocity: " << osc.oscA.angularVelocity << std::endl;
        osc.oscA.printAngularVelocity (oscAPrefix);
        std::cout << oscAPrefix << "currentPhase: " << osc.oscA.currentPhase << std::endl;
        osc.oscA.printCurrentPhase (oscAPrefix);
        std::cout << waveAPrefix << "name: " << osc.oscA.waveform.name << std::endl;
        osc.oscA.waveform.printName (waveAPrefix);
        std::cout << waveAPrefix << "interpolationType: " << osc.oscA.waveform.interpolationType << std::endl;
        osc.oscA.waveform.printInterpolationType (waveAPrefix);
        std::cout << waveAPrefix << "numSupportPoints: " << osc.oscA.waveform.numSupportPoints << std::endl;
        osc.oscA.waveform.printNumSupportPoints (waveAPrefix);
        std::cout << waveAPrefix << "phaseIncrement: " << osc.oscA.waveform.phaseIncrement << std::endl;
        osc.oscA.waveform.printPhaseIncrement (waveAPrefix);
        std::cout << waveAPrefix << "flipPolarity: " << osc.oscA.waveform.flipPolarity << std::endl;
        osc.oscA.waveform.printFlipPolarity (waveAPrefix);

        std::cout << oscBPrefix << "oscillatorId: " << osc.oscB.oscillatorId << std::endl;        
        osc.oscB.printOscillatorId (oscBPrefix);
        std::cout << oscBPrefix << "sampleRate: " << osc.oscB.sampleRate << std::endl;
        osc.oscB.printSampleRate (oscBPrefix);
        std::cout << oscBPrefix << "angularVelocity: " << osc.oscB.angularVelocity << std::endl;
        osc.oscB.printAngularVelocity (oscBPrefix);
        std::cout << oscBPrefix << "currentPhase: " << osc.oscB.currentPhase << std::endl;
        osc.oscB.printCurrentPhase (oscBPrefix);
        std::cout << waveBPrefix << "name: " << osc.oscB.waveform.name << std::endl;
        osc.oscB.waveform.printName (waveBPrefix);
        std::cout << waveBPrefix << "interpolationType: " << osc.oscB.waveform.interpolationType << std::endl;
        osc.oscB.waveform.printInterpolationType (waveBPrefix);
        std::cout << waveBPrefix << "numSupportPoints: " << osc.oscB.waveform.numSupportPoints << std::endl;
        osc.oscB.waveform.printNumSupportPoints (waveBPrefix);
        std::cout << waveBPrefix << "phaseIncrement: " << osc.oscB.waveform.phaseIncrement << std::endl;
        osc.oscB.waveform.printPhaseIncrement (waveBPrefix);
        std::cout << waveBPrefix << "flipPolarity: " << osc.oscB.waveform.flipPolarity << std::endl;
        osc.oscB.waveform.printFlipPolarity (waveBPrefix);
        
        std::cout << std::endl;    
    }

    {
        // block for testing UDT5: SimpleMonoSynth

        std::cout << std::endl;        
        
        SimpleMonoSynthWrapper synth {new SimpleMonoSynth {44100}};
        
        int midiNoteNumber = 69;
        double tuningInHz = 440;
        float velocity = 0.8f;

        synth.synthPtr->amountOfLfoLevelModulation = 0.5f;

        double attackTimeInSeconds = 0.005;
        double decayTimeInSeconds = 0.1; 
        float sustainLevel = 0.4f;
        double releaseTimeInSeconds = 0.8;
        
        synth.synthPtr->envelopeGate
             .envelopeParameters
             .adjustParameters (attackTimeInSeconds, decayTimeInSeconds, 
                                sustainLevel, releaseTimeInSeconds);
        
        synth.synthPtr->triggerNote (true, midiNoteNumber, velocity, tuningInHz);
                
        std::cout << std::endl;        

        std::string synthPrefix {"synth."};
        std::string oscPrefix {synthPrefix + "oscillator."};
        std::string wavePrefix {oscPrefix + "waveform."};        
        std::string egPrefix {synthPrefix + "envelopeGate."};
        std::string paramsPrefix {egPrefix + "envelopeParameters."};
        std::string lfoPrefix {synthPrefix + "lfo."};

        std::cout << synthPrefix << "amountOfLfoLevelModulation: " 
                  << synth.synthPtr->amountOfLfoLevelModulation << std::endl;
        synth.synthPtr->printAmountOfLfoLevelModulation(synthPrefix);
        std::cout << synthPrefix << "amplitudeOfPlayingNote: " 
                  << synth.synthPtr->amplitudeOfPlayingNote << std::endl; 
        synth.synthPtr->printAmplitudeOfPlayingNote(synthPrefix);             

        std::cout << oscPrefix << "oscillatorId: " 
                  << synth.synthPtr->oscillator.oscillatorId << std::endl;
        synth.synthPtr->oscillator.printOscillatorId (oscPrefix);
        std::cout << oscPrefix << "sampleRate: " 
                  << synth.synthPtr->oscillator.sampleRate << std::endl;
        synth.synthPtr->oscillator.printSampleRate (oscPrefix);
        std::cout << oscPrefix << "angularVelocity: " 
                  << synth.synthPtr->oscillator.angularVelocity << std::endl;
        synth.synthPtr->oscillator.printAngularVelocity (oscPrefix);
        std::cout << oscPrefix << "currentPhase: " 
                  << synth.synthPtr->oscillator.currentPhase << std::endl;
        synth.synthPtr->oscillator.printCurrentPhase (oscPrefix);
        std::cout << wavePrefix << "name: " 
                  << synth.synthPtr->oscillator.waveform.name << std::endl;
        synth.synthPtr->oscillator.waveform.printName (wavePrefix);
        std::cout << wavePrefix << "interpolationType: " 
                  << synth.synthPtr->oscillator.waveform.interpolationType << std::endl;
        synth.synthPtr->oscillator.waveform.printInterpolationType (wavePrefix);
        std::cout << wavePrefix << "numSupportPoints: " 
                  << synth.synthPtr->oscillator.waveform.numSupportPoints << std::endl;
        synth.synthPtr->oscillator.waveform.printNumSupportPoints (wavePrefix);
        std::cout << wavePrefix << "phaseIncrement: " 
                  << synth.synthPtr->oscillator.waveform.phaseIncrement << std::endl;
        synth.synthPtr->oscillator.waveform.printPhaseIncrement (wavePrefix);
        std::cout << wavePrefix << "flipPolarity: " 
                  << synth.synthPtr->oscillator.waveform.flipPolarity << std::endl;
        synth.synthPtr->oscillator.waveform.printFlipPolarity (wavePrefix);

        std::cout << egPrefix << "envelopeState: " 
                  << synth.synthPtr->envelopeGate.envelopeState << std::endl;
        synth.synthPtr->envelopeGate.printEnvelopeState(egPrefix);
        std::cout << egPrefix << "normalizedTargetValueInCurrentState: " 
                  << synth.synthPtr->envelopeGate.normalizedTargetValueInCurrentState << std::endl;        
        synth.synthPtr->envelopeGate.printNormalizedTargetValueInCurrentState (egPrefix);
        std::cout << egPrefix << "normalizedDeltaPerStep: " 
                  << synth.synthPtr->envelopeGate.normalizedDeltaPerStep << std::endl;        
        synth.synthPtr->envelopeGate.printNormalizedDeltaPerStep (egPrefix);
        std::cout << egPrefix << "lastComputedNormalizedSample: " 
                  << synth.synthPtr->envelopeGate.lastComputedNormalizedSample << std::endl;    
        synth.synthPtr->envelopeGate.printLastComputedNormalizedSample (egPrefix);
        std::cout << paramsPrefix << "attackTimeInSamples: " 
                  << synth.synthPtr->envelopeGate.envelopeParameters.attackTimeInSamples << std::endl;     
        synth.synthPtr->envelopeGate.envelopeParameters.printAttackTimeInSamples (paramsPrefix);
        std::cout << paramsPrefix << "decayTimeInSamples: " 
                  << synth.synthPtr->envelopeGate.envelopeParameters.decayTimeInSamples << std::endl;        
        synth.synthPtr->envelopeGate.envelopeParameters.printDecayTimeInSamples (paramsPrefix);
        std::cout << paramsPrefix << "normalizedSustainLevel: " 
                  << synth.synthPtr->envelopeGate.envelopeParameters.normalizedSustainLevel << std::endl;        
        synth.synthPtr->envelopeGate.envelopeParameters.printNormalizedSustainLevel (paramsPrefix);
        std::cout << paramsPrefix << "releaseTimeInSamples: " 
                  << synth.synthPtr->envelopeGate.envelopeParameters.releaseTimeInSamples << std::endl;        
        synth.synthPtr->envelopeGate.envelopeParameters.printReleaseTimeInSamples (paramsPrefix);
        std::cout << paramsPrefix << "exponentOfShapePowerFunction: " 
                  << synth.synthPtr->envelopeGate.envelopeParameters.exponentOfShapePowerFunction << std::endl;        
        synth.synthPtr->envelopeGate.envelopeParameters.printExponentOfShapePowerFunction (paramsPrefix);

        std::cout << lfoPrefix << "sampleRateInHz: " 
                  << synth.synthPtr->lfo.sampleRateInHz << std::endl;        
        synth.synthPtr->lfo.printSampleRateInHz (lfoPrefix);
        std::cout << lfoPrefix << "phaseIncrementPerStep: " 
                  << synth.synthPtr->lfo.phaseIncrementPerStep << std::endl;        
        synth.synthPtr->lfo.printPhaseIncrementPerStep (lfoPrefix);
        std::cout << lfoPrefix << "amplitudeDeltaPerStep: " 
                  << synth.synthPtr->lfo.amplitudeDeltaPerStep << std::endl;        
        synth.synthPtr->lfo.printAmplitudeDeltaPerStep (lfoPrefix);
        std::cout << lfoPrefix << "currentPhase: " 
                  << synth.synthPtr->lfo.currentPhase << std::endl;        
        synth.synthPtr->lfo.printCurrentPhase (lfoPrefix);
        std::cout << lfoPrefix << "currentAmplitude: " 
                  << synth.synthPtr->lfo.currentAmplitude << std::endl;        
        synth.synthPtr->lfo.printCurrentAmplitude (lfoPrefix);

        std::cout << std::endl;    
    }
    std::cout << "good to go!" << std::endl;
}
