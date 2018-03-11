#include <genesis.h>

typedef enum {
    PARAMETER_G_LFO_ON,
    PARAMETER_G_LFO_FREQ,
    PARAMETER_NOTE,
    PARAMETER_FREQ,
    PARAMETER_OCTAVE,
    PARAMETER_ALGORITHM,
    PARAMETER_FEEDBACK,
    PARAMETER_LFO_AMS,
    PARAMETER_LFO_FMS,
    PARAMETER_STEREO,
    PARAMETER_OP1_MUL,
    PARAMETER_OP1_DT1,
    PARAMETER_OP1_TL,
    PARAMETER_OP1_RS,
    PARAMETER_OP1_AR,
    PARAMETER_OP1_AM,
    PARAMETER_OP1_D1R,
    PARAMETER_OP1_D2R,
    PARAMETER_OP1_D1L,
    PARAMETER_OP1_RR
} FmParameters;

typedef struct {
    const char name[10];
    const u16 minSize;
    u16 value;
    const u16 maxValue;
    const u8 step;
    const void (*onUpdate)(void);
} FmParameter;

void s_init(void);
void s_playNote(void);
void s_stopNote(void);
FmParameter* s_fmParameter(FmParameters parameter);
u16 s_maxFmParameters(void);
