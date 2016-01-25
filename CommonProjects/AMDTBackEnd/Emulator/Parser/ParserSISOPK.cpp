//=============================================================
/// Copyright 2013-2016 (c), Advanced Micro Devices, Inc.
///
/// \file   ParserSISOPK.cpp
/// \author GPU Developer Tools
/// \version $Revision: #7 $
/// \brief Description: Parser for SOPK (Scalar instruction with one inline constant input and one output) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSISOPK.cpp#7 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author:  AMD Developer Tools Team
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSISOPK.h"

SOPKInstruction::SIMM16 ParserSISOPK::GetSIMM16(Instruction::instruction32bit hexInstruction, unsigned int& ridx)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOPK, simm16, SIMM16, 0);
#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOPKInstruction::SIMM16##FIELD_MIN) && (IN <= SOPKInstruction::SIMM16##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOPKInstruction::SIMM16##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOPKInstruction::SIMM16##FIELD) \
    { \
        return SOPKInstruction::SIMM16##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(simm16, ridx);
#undef X
#undef X_RANGE

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN)\
    if ((IN >= SOPKInstruction::SIMM16##FIELD_MIN) && (IN <= SOPKInstruction::SIMM16##FIELD_MAX)) \
    { \
        return SOPKInstruction::SIMM16##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOPKInstruction::SIMM16##FIELD) \
    { \
        return SOPKInstruction::SIMM16##FIELD; \
    }
    SCALAR_INSTRUCTION_FIELDS(simm16);
    GENERIC_INSTRUCTION_FIELDS_2(simm16);
#undef X
#undef X_RANGE
    return SOPKInstruction::SIMM16Illegal;
}

SISOPKInstruction::OP ParserSISOPK::GetSISOPKOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOPK, op, OP, 23);

    if ((op > SISOPKInstruction::S_MOVK_I32 && op < SISOPKInstruction::S_CMOVK_I32)
        || (op >= SISOPKInstruction::S_RESERVED))
    {
        return SISOPKInstruction::S_RESERVED;
    }
    else
    {
        return op;
    }
}

VISOPKInstruction::OP ParserSISOPK::GetVISOPKOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, SOPK, op, OP, 23);

    if (op > VISOPKInstruction::S_ILLEGAL)
    {
        return VISOPKInstruction::S_ILLEGAL;
    }
    else
    {
        return op;
    }
}

SOPKInstruction::SDST ParserSISOPK::GetSDST(Instruction::instruction32bit hexInstruction, unsigned int& ridx)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOPK, sdst, SDST, 16);
#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOPKInstruction::SDST##FIELD_MIN) && (IN <= SOPKInstruction::SDST##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOPKInstruction::SDST##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOPKInstruction::SDST##FIELD) \
    { \
        return SOPKInstruction::SDST##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(sdst, ridx);
#undef X
#undef X_RANGE
    return SOPKInstruction::SDSTIllegal;
}

ParserSI::kaStatus ParserSISOPK::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, Instruction*& instruction, bool , uint32_t, int iLabel /*=NO_LABEL*/ , int iGotoLabel /*=NO_LABEL*/)
{
    unsigned int simm16Ridx = 0, sdstRidx = 0;
    SOPKInstruction::SIMM16 simm16 = GetSIMM16(hexInstruction, simm16Ridx);
    SOPKInstruction::SDST sdst = GetSDST(hexInstruction, sdstRidx);

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SISOPKInstruction::OP op = GetSISOPKOp(hexInstruction);
        instruction = new SISOPKInstruction(simm16, op, sdst, simm16Ridx, sdstRidx, iLabel, iGotoLabel);
    }
    else
    {
        VISOPKInstruction::OP op = GetVISOPKOp(hexInstruction);
        instruction = new VISOPKInstruction(simm16, op, sdst, simm16Ridx, sdstRidx, iLabel, iGotoLabel);
    }

    return ParserSI::Status_SUCCESS;
}

ParserSI::kaStatus ParserSISOPK::Parse(GDT_HW_GENERATION, Instruction::instruction64bit, Instruction*&, int /*iLabel =NO_LABEL*/ , int /*iGotoLabel =NO_LABEL*/)
{
    return ParserSI::Status_64BitInstructionNotSupported;
}

