#include <genesis.h>
#include <synth.h>
#include <operator.h>

static void initOperators(void);
static void updateAlgorithmAndFeedback(void);
static void updateGlobalLFO(void);
static void updateStereoAndLFO(void);
static void updateFreqAndOctave(void);
static void updateNote(void);
static void setFrequency(u16 freq, u8 octave);
static void setAlgorithm(u8 algorithm, u8 feedback);
static void setGlobalLFO(u8 enable, u8 freq);
static void setStereoAndLFO(u8 stereo, u8 ams, u8 fms);

static Operator operators[OPERATOR_COUNT];

static OperatorDefinition defaultOperatorDefinitions[OPERATOR_COUNT] =
    {{1, 1, 35, 15, 2, 1, 5, 2, 1, 1},
     {13, 0, 45, 2, 25, 0, 36, 2, 1, 1},
     {3, 3, 38, 1, 31, 0, 5, 2, 1, 1},
     {1, 0, 0, 2, 25, 0, 7, 2, 10, 6}};

static FmParameter fmParameters[] = {
    {"Glob LFO ", 1, 1, 1, 1, updateGlobalLFO},
    {"LFO Freq ", 1, 3, 7, 1, updateGlobalLFO},
    {"Note     ", 2, 1, 11, 1, updateNote},
    {"Freq Num ", 4, 653, 2047, 4, updateFreqAndOctave},
    {"Octave   ", 1, 4, 7, 1, updateFreqAndOctave},
    {"Algorithm", 1, 0, 7, 1, updateAlgorithmAndFeedback},
    {"Feedback ", 1, 0, 7, 1, updateAlgorithmAndFeedback},
    {"LFO AMS  ", 1, 0, 7, 1, updateStereoAndLFO},
    {"LFO FMS  ", 1, 0, 7, 1, updateStereoAndLFO},
    {"Stereo   ", 1, 3, 3, 1, updateStereoAndLFO}};

FmParameter *synth_fmParameter(FmParameters parameter)
{
    return &fmParameters[parameter];
}

Operator *synth_operator(u8 opNumber)
{
    return &operators[opNumber];
}

static void initOperators(void)
{
    for (u8 i = 0; i < OPERATOR_COUNT; i++)
    {
        operator_init(&operators[i], i, &defaultOperatorDefinitions[i]);
    }
}

void synth_init(void)
{
    initOperators();
    YM2612_writeReg(0, 0x27, 0); // Ch 3 Normal
    YM2612_writeReg(0, 0x28, 0); // All channels off
    YM2612_writeReg(0, 0x28, 1);
    YM2612_writeReg(0, 0x28, 2);
    YM2612_writeReg(0, 0x28, 4);
    YM2612_writeReg(0, 0x28, 5);
    YM2612_writeReg(0, 0x28, 6);
    YM2612_writeReg(0, 0x90, 0); // Proprietary
    YM2612_writeReg(0, 0x94, 0);
    YM2612_writeReg(0, 0x98, 0);
    YM2612_writeReg(0, 0x9C, 0);
}

void synth_playNote(void)
{
    updateGlobalLFO();
    for (int i = 0; i < OPERATOR_COUNT; i++)
    {
        operator_update(&operators[i]);
    }
    updateAlgorithmAndFeedback();
    updateStereoAndLFO();
    YM2612_writeReg(0, 0x28, 0x00); // Key off
    updateFreqAndOctave();
    YM2612_writeReg(0, 0x28, 0xF0); // Key On
}

void synth_stopNote(void)
{
    YM2612_writeReg(0, 0x28, 0x00); // Key Off
}

static void setStereoAndLFO(u8 stereo, u8 ams, u8 fms)
{
    YM2612_writeReg(0, 0xB4, (stereo << 6) + (ams << 3) + fms);
}

static void setGlobalLFO(u8 enable, u8 freq)
{
    YM2612_writeReg(0, 0x22, (enable << 3) + freq);
}

static void setFrequency(u16 freq, u8 octave)
{
    YM2612_writeReg(0, 0xA4, (freq >> 8) + (octave << 3));
    YM2612_writeReg(0, 0xA0, freq);
}

static void setAlgorithm(u8 algorithm, u8 feedback)
{
    YM2612_writeReg(0, 0xB0, algorithm + (feedback << 3));
}

static void updateNote(void)
{
    const u16 notes_freq[] = {617, 653, 692, 733, 777, 823, 872, 924, 979, 1037, 1099, 1164};
    u16 note_index = fmParameters[PARAMETER_NOTE].value;
    u16 note_freq = notes_freq[note_index];
    fmParameters[PARAMETER_FREQ].value = note_freq;
    fmParameters[PARAMETER_FREQ].onUpdate();
}

static void updateStereoAndLFO(void)
{
    setStereoAndLFO(
        fmParameters[PARAMETER_STEREO].value,
        fmParameters[PARAMETER_LFO_AMS].value,
        fmParameters[PARAMETER_LFO_FMS].value);
}

static void updateFreqAndOctave(void)
{
    setFrequency(
        fmParameters[PARAMETER_FREQ].value,
        fmParameters[PARAMETER_OCTAVE].value);
}

static void updateGlobalLFO(void)
{
    setGlobalLFO(
        fmParameters[PARAMETER_G_LFO_ON].value,
        fmParameters[PARAMETER_G_LFO_FREQ].value);
}

static void updateAlgorithmAndFeedback(void)
{
    setAlgorithm(
        fmParameters[PARAMETER_ALGORITHM].value,
        fmParameters[PARAMETER_FEEDBACK].value);
}
