//=============================================================
/// Copyright 2013-2016 (c), Advanced Micro Devices, Inc.
///
/// \file   ParserSISOP1.cpp
/// \author GPU Developer Tools
/// \version $Revision: #7 $
/// \brief Description: Parser for SOP1 (Scalar instruction with one input and one output. Can be followed by a 32-bit literal constant) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSISOP1.cpp#7 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author:  AMD Developer Tools Team
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSISOP1.h"

SOP1Instruction::SSRC ParserSISOP1::GetSSRC0(Instruction::instruction32bit hexInstruction, unsigned int& ridx)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOP1, ssrc, SSRC, 0);
#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOP1Instruction::SSRC##FIELD_MIN) && (IN <= SOP1Instruction::SSRC##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOP1Instruction::SSRC##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOP1Instruction::SSRC##FIELD) \
    { \
        return SOP1Instruction::SSRC##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(ssrc, ridx);
#undef X
#undef X_RANGE

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN)\
    if ((IN >= SOP1Instruction::SSRC##FIELD_MIN) && (IN <= SOP1Instruction::SSRC##FIELD_MAX)) \
    { \
        return SOP1Instruction::SSRC##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOP1Instruction::SSRC##FIELD) \
    { \
        return SOP1Instruction::SSRC##FIELD; \
    }
    SCALAR_INSTRUCTION_FIELDS(ssrc);
    GENERIC_INSTRUCTION_FIELDS_2(ssrc);
#undef X
#undef X_RANGE
    return SOP1Instruction::SSRCIllegal;
    //return ssrc;
}

SISOP1Instruction::OP ParserSISOP1::GetSISOP1Op(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOP1, op, OP, 8);

    if ((op < SISOP1Instruction::S_MOV_B32)
        || (op > SISOP1Instruction::S_RFE_B64 && op < SISOP1Instruction::S_AND_SAVEEXEC_B64)
        || (op > SISOP1Instruction::S_CBRANCH_JOIN && op < SISOP1Instruction::S_ABS_I32)
        || (op >= SISOP1Instruction::S_RESERVED))
    {
        return SISOP1Instruction::S_RESERVED;
    }
    else
    {
        return op;
    }
}

VISOP1Instruction::OP ParserSISOP1::GetVISOP1Op(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, SOP1, op, OP, 8);

    if ((op < VISOP1Instruction::s_Illeagal))
    {
        return VISOP1Instruction::s_Illeagal;
    }
    else
    {
        return op;
    }
}

SOP1Instruction::SDST ParserSISOP1::GetSDST(Instruction::instruction32bit hexInstruction, unsigned int& ridx)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOP1, sdst, SDST, 16);
#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOP1Instruction::SDST##FIELD_MIN) && (IN <= SOP1Instruction::SDST##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOP1Instruction::SDST##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOP1Instruction::SDST##FIELD) \
    { \
        return SOP1Instruction::SDST##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(sdst, ridx);
#undef X
#undef X_RANGE
    return SOP1Instruction::SDSTIllegal;
}

ParserSI::kaStatus ParserSISOP1::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, Instruction*& instruction, bool, uint32_t, int iLabel /*=NO_LABEL*/ , int iGotoLabel /*=NO_LABEL*/)
{
    unsigned int ridx0 = 0, sdstRidx1 = 0;
    SOP1Instruction::SSRC ssrc0 = GetSSRC0(hexInstruction, ridx0);
    SOP1Instruction::SDST sdst = GetSDST(hexInstruction, sdstRidx1);

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SISOP1Instruction::OP op = GetSISOP1Op(hexInstruction);
        instruction = new SISOP1Instruction(ssrc0, op, sdst, ridx0, sdstRidx1, iLabel, iGotoLabel);
    }
    else
    {
        VISOP1Instruction::OP op = GetVISOP1Op(hexInstruction);
        instruction = new VISOP1Instruction(ssrc0, op, sdst, ridx0, sdstRidx1, iLabel, iGotoLabel);
    }


    return ParserSI::Status_SUCCESS;
}

ParserSI::kaStatus ParserSISOP1::Parse(GDT_HW_GENERATION, Instruction::instruction64bit, Instruction*&, int/* iLabel =NO_LABEL*/ , int /*iGotoLabel =NO_LABEL*/)
{
    return ParserSI::Status_64BitInstructionNotSupported;
}
