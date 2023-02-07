#pragma once

// Wrapper classes added for Part 3

// forward declarations
struct Oscillator;
struct EnvelopeGate;
struct LowFrequencyOscillator;
struct CompoundOscillator;
struct SimpleMonoSynth;


struct OscillatorWrapper
{
    OscillatorWrapper (Oscillator* pointerToGetOwned);
    ~OscillatorWrapper();

    Oscillator* oscPtr;
};

struct EnvelopeGateWrapper
{
    EnvelopeGateWrapper (EnvelopeGate* pointerToGetOwned);
    ~EnvelopeGateWrapper();

    EnvelopeGate* egPtr;
};

struct LowFrequencyOscillatorWrapper
{
    LowFrequencyOscillatorWrapper (LowFrequencyOscillator* pointerToGetOwned);
    ~LowFrequencyOscillatorWrapper();

    LowFrequencyOscillator* lfoPtr;
};

struct CompoundOscillatorWrapper
{
    CompoundOscillatorWrapper (CompoundOscillator* pointerToGetOwned);
    ~CompoundOscillatorWrapper();
    CompoundOscillator* coscPtr;
};

struct SimpleMonoSynthWrapper
{
    SimpleMonoSynthWrapper (SimpleMonoSynth* pointerToGetOwned);
    ~SimpleMonoSynthWrapper();

    SimpleMonoSynth* synthPtr;
};
