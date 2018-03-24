#include <operator.h>
#include <genesis.h>

static void updateMulDt1(Operator *op);
static void updateTotalLevel(Operator *op);
static void updateRsAr(Operator *op);
static void updateAmD1r(Operator *op);
static void updateD2r(Operator *op);
static void updateD1lRr(Operator *op);
static void setMulDt1(u8 opNum, u8 mul, u8 dt1);
static void setTotalLevel(u8 opNum, u8 totalLevel);
static void setRsAr(u8 opNum, u8 rs, u8 ar);
static void setAmD1r(u8 opNum, u8 am, u8 d1r);
static void setD2r(u8 opNum, u8 d2r);
static void setD1lRr(u8 opNum, u8 d1l, u8 rr);

struct OperatorParameter
{
    const char name[12];
    const u16 minSize;
    const u16 maxValue;
    const u8 step;
    void (*onUpdate)(Operator *op);
};

static OperatorParameter parameters[OPERATOR_PARAMETER_COUNT] = {
    {"Multiple", 2, 15, 1, updateMulDt1},
    {"Detune", 1, 7, 1, updateMulDt1},
    {"Total Lvl", 3, 127, 1, updateTotalLevel},
    {"Rate Scale", 1, 2, 1, updateRsAr},
    {"Atck Rate", 2, 31, 1, updateRsAr},
    {"Amp Mode?", 1, 1, 1, updateAmD1r},
    {"1st Decay", 2, 31, 1, updateAmD1r},
    {"2nd Decay", 2, 31, 1, updateD2r},
    {"Sub Level", 2, 15, 1, updateD1lRr},
    {"Rel Rate", 2, 15, 1, updateD1lRr}};

void operator_init(Operator *op, u8 opNumber, OperatorDefinition *definition)
{
    op->opNumber = opNumber;
    op->parameters = &parameters[0];
    op->parameterValue[OP_PARAMETER_DT1] = definition->detune;
    op->parameterValue[OP_PARAMETER_MUL] = definition->multiple;
    op->parameterValue[OP_PARAMETER_TL] = definition->totalLevel;
    op->parameterValue[OP_PARAMETER_RS] = definition->rateScale;
    op->parameterValue[OP_PARAMETER_AR] = definition->attackRate;
    op->parameterValue[OP_PARAMETER_AM] = definition->ampMode;
    op->parameterValue[OP_PARAMETER_D1R] = definition->firstDecay;
    op->parameterValue[OP_PARAMETER_D2R] = definition->secondDecay;
    op->parameterValue[OP_PARAMETER_D1L] = definition->subLevel;
    op->parameterValue[OP_PARAMETER_RR] = definition->releaseRate;
}

u16 operator_parameterValue(Operator *op, OpParameters parameter)
{
    return op->parameterValue[parameter];
}

const char *operator_parameterName(Operator *op, OpParameters parameter)
{
    return &op->parameters[parameter].name[0];
}

u16 operator_parameterMinSize(Operator *op, OpParameters parameter)
{
    return op->parameters[parameter].minSize;
}

u8 operator_parameterStep(Operator *op, OpParameters parameter)
{
    return op->parameters[parameter].step;
}

void operator_setParameterValue(Operator *op, OpParameters parameter, u16 value)
{
    OperatorParameter p = parameters[parameter];
    if (value > p.maxValue)
    {
        value = 0;
    }
    if (value == (u16)-1)
    {
        value = p.maxValue;
    }
    op->parameterValue[parameter] = value;
    op->parameters[parameter].onUpdate(op);
}

void operator_update(Operator *op)
{
    updateMulDt1(op);
    updateTotalLevel(op);
    updateRsAr(op);
    updateAmD1r(op);
    updateD2r(op);
    updateD1lRr(op);
}

static void updateMulDt1(Operator *op)
{
    setMulDt1(
        op->opNumber,
        op->parameterValue[OP_PARAMETER_MUL],
        op->parameterValue[OP_PARAMETER_DT1]);
}

static void updateTotalLevel(Operator *op)
{
    setTotalLevel(
        op->opNumber,
        op->parameterValue[OP_PARAMETER_TL]);
}

static void updateRsAr(Operator *op)
{
    setRsAr(
        op->opNumber,
        op->parameterValue[OP_PARAMETER_RS],
        op->parameterValue[OP_PARAMETER_AR]);
}

static void updateAmD1r(Operator *op)
{
    setAmD1r(
        op->opNumber,
        op->parameterValue[OP_PARAMETER_AM],
        op->parameterValue[OP_PARAMETER_D1R]);
}

static void updateD2r(Operator *op)
{
    setD2r(
        op->opNumber,
        op->parameterValue[OP_PARAMETER_D2R]);
}

static void updateD1lRr(Operator *op)
{
    setD1lRr(
        op->opNumber,
        op->parameterValue[OP_PARAMETER_D1L],
        op->parameterValue[OP_PARAMETER_RR]);
}

static void setMulDt1(u8 opNum, u8 mul, u8 dt1)
{
    YM2612_writeReg(0, 0x30 + (opNum * 4), mul + (dt1 << 4));
}

static void setTotalLevel(u8 opNum, u8 totalLevel)
{
    YM2612_writeReg(0, 0x40 + (opNum * 4), totalLevel);
}

static void setRsAr(u8 opNum, u8 rs, u8 ar)
{
    YM2612_writeReg(0, 0x50 + (opNum * 4), ar + (rs << 6));
}

static void setAmD1r(u8 opNum, u8 am, u8 d1r)
{
    YM2612_writeReg(0, 0x60 + (opNum * 4), (am << 7) + d1r);
}

static void setD2r(u8 opNum, u8 d2r)
{
    YM2612_writeReg(0, 0x70 + (opNum * 4), d2r);
}

static void setD1lRr(u8 opNum, u8 d1l, u8 rr)
{
    YM2612_writeReg(0, 0x80 + (opNum * 4), rr + (d1l << 4));
}
