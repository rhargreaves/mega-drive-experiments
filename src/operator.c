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

void operator_init(Operator *op, u8 opNumber)
{
    op->opNumber = opNumber;
    OperatorParameter paras[OPERATOR_PARAMETER_COUNT] = {
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
    memcpy(&op->parameters, &paras, sizeof paras);
    switch (opNumber)
    {
    case 0:
        op->parameterValue[OP_PARAMETER_DT1] = 1;
        op->parameterValue[OP_PARAMETER_MUL] = 1;
        op->parameterValue[OP_PARAMETER_TL] = 35;
        op->parameterValue[OP_PARAMETER_RS] = 15;
        op->parameterValue[OP_PARAMETER_AR] = 2;
        op->parameterValue[OP_PARAMETER_AM] = 1;
        op->parameterValue[OP_PARAMETER_D1R] = 5;
        op->parameterValue[OP_PARAMETER_D2R] = 2;
        op->parameterValue[OP_PARAMETER_D1L] = 1;
        op->parameterValue[OP_PARAMETER_RR] = 1;
        break;
    case 1:
        op->parameterValue[OP_PARAMETER_MUL] = 13;
        op->parameterValue[OP_PARAMETER_DT1] = 0;
        op->parameterValue[OP_PARAMETER_TL] = 45;
        op->parameterValue[OP_PARAMETER_RS] = 2;
        op->parameterValue[OP_PARAMETER_AR] = 25;
        op->parameterValue[OP_PARAMETER_AM] = 0;
        op->parameterValue[OP_PARAMETER_D1R] = 36;
        op->parameterValue[OP_PARAMETER_D2R] = 2;
        op->parameterValue[OP_PARAMETER_D1L] = 1;
        op->parameterValue[OP_PARAMETER_RR] = 1;
        break;
    case 2:
        op->parameterValue[OP_PARAMETER_MUL] = 3;
        op->parameterValue[OP_PARAMETER_DT1] = 3;
        op->parameterValue[OP_PARAMETER_TL] = 38;
        op->parameterValue[OP_PARAMETER_RS] = 1;
        op->parameterValue[OP_PARAMETER_AR] = 31;
        op->parameterValue[OP_PARAMETER_AM] = 0;
        op->parameterValue[OP_PARAMETER_D1R] = 5;
        op->parameterValue[OP_PARAMETER_D2R] = 2;
        op->parameterValue[OP_PARAMETER_D1L] = 1;
        op->parameterValue[OP_PARAMETER_RR] = 1;
        break;
    case 3:
        op->parameterValue[OP_PARAMETER_MUL] = 1;
        op->parameterValue[OP_PARAMETER_DT1] = 0;
        op->parameterValue[OP_PARAMETER_TL] = 0;
        op->parameterValue[OP_PARAMETER_RS] = 2;
        op->parameterValue[OP_PARAMETER_AR] = 25;
        op->parameterValue[OP_PARAMETER_AM] = 0;
        op->parameterValue[OP_PARAMETER_D1R] = 7;
        op->parameterValue[OP_PARAMETER_D2R] = 2;
        op->parameterValue[OP_PARAMETER_D1L] = 10;
        op->parameterValue[OP_PARAMETER_RR] = 6;
        break;
    }
}

OperatorParameter *operator_parameter(Operator *op, OpParameters parameter)
{
    return &op->parameters[parameter];
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
