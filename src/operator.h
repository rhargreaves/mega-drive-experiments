#pragma once
#include <genesis.h>

#define OPERATOR_PARAMETER_COUNT 12

typedef enum {
    OP_PARAMETER_DT1,
    OP_PARAMETER_MUL,
    OP_PARAMETER_TL,
    OP_PARAMETER_RS,
    OP_PARAMETER_AR,
    OP_PARAMETER_AM,
    OP_PARAMETER_D1R,
    OP_PARAMETER_D2R,
    OP_PARAMETER_D1L,
    OP_PARAMETER_RR,
    OP_PARAMETER_CH3_OCTAVE,
    OP_PARAMETER_CH3_FREQ,
} OpParameters;

typedef struct Operator Operator;
typedef struct OperatorParameter OperatorParameter;

struct Operator
{
    u8 opNumber;
    u8 chanNumber;
    OperatorParameter const *parameters;
    u16 parameterValues[OPERATOR_PARAMETER_COUNT];
};

void operator_init(Operator *op, u8 opNumber, u8 chanNumber, const u16 parameterValues[OPERATOR_PARAMETER_COUNT]);
u16 operator_parameterValue(Operator *op, OpParameters parameter);
void operator_parameterUpdate(Operator *op, OpParameters parameter);
void operator_setParameterValue(Operator *op, OpParameters parameter, u16 value);
void operator_update(Operator *op);
