#pragma once

#include <string>

// Wrapper classes added for Part 3

struct Oscillator;

struct OscillatorWrapper
{
    OscillatorWrapper (Oscillator* pointerToGetOwned);
    ~OscillatorWrapper();

    Oscillator* oscPtr;
};

struct EnvelopeGate;

struct EnvelopeGateWrapper
{
    EnvelopeGateWrapper (EnvelopeGate* pointerToGetOwned);
    ~EnvelopeGateWrapper();

    EnvelopeGate* egPtr;
};

struct LowFrequencyOscillator;

struct LowFrequencyOscillatorWrapper
{
    LowFrequencyOscillatorWrapper (LowFrequencyOscillator* pointerToGetOwned);
    ~LowFrequencyOscillatorWrapper();

    LowFrequencyOscillator* lfoPtr;
};

struct CompoundOscillator;

struct CompoundOscillatorWrapper
{
    CompoundOscillatorWrapper (CompoundOscillator* pointerToGetOwned);
    ~CompoundOscillatorWrapper();
    CompoundOscillator* coscPtr;
};

struct SimpleMonoSynth;

struct SimpleMonoSynthWrapper
{
    SimpleMonoSynthWrapper (SimpleMonoSynth* pointerToGetOwned);
    ~SimpleMonoSynthWrapper();

    SimpleMonoSynth* synthPtr; // == nullptr; in video, but there's no implicit ctor and explicit one inits the pointer
};
