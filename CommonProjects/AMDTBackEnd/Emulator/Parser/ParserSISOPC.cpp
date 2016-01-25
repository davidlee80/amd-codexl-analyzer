//=============================================================
/// Copyright 2013-2016 (c), Advanced Micro Devices, Inc.
///
/// \file   ParserSISOPC.cpp
/// \author GPU Developer Tools
/// \version $Revision: #7 $
/// \brief Description: Parser for SOPC (Scalar instruction taking two inputs and producing a comparison result. Can be followed by a 32-bit literal constant) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSISOPC.cpp#7 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author:  AMD Developer Tools Team
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSISOPC.h"

SOPCInstruction::SSRC ParserSISOPC::GetSSRC(Instruction::instruction32bit hexInstruction, unsigned int&, unsigned int idxSSRC)
{
    SOPCInstruction::SSRC ssrc = (SOPCInstruction::SSRC)0;

    switch (idxSSRC)
    {
        case 0 :
            ssrc = static_cast<SOPCInstruction::SSRC>(hexInstruction & SOPCMask_SSRC0);
            break;

        case 1:
            ssrc = static_cast<SOPCInstruction::SSRC>((hexInstruction & SOPCMask_SSRC1) >> 8);
            break;
    }

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOPCInstruction::SSRC##FIELD_MIN) && (IN <= SOPCInstruction::SSRC##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOPCInstruction::SSRC##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOPCInstruction::SSRC##FIELD) \
    { \
        return SOPCInstruction::SSRC##FIELD; \
    }
    //GENERIC_INSTRUCTION_FIELDS_1(ssrc, ridx);
#undef X
#undef X_RANGE

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN)\
    if ((IN >= SOPCInstruction::SSRC##FIELD_MIN) && (IN <= SOPCInstruction::SSRC##FIELD_MAX)) \
    { \
        return SOPCInstruction::SSRC##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOPCInstruction::SSRC##FIELD) \
    { \
        return SOPCInstruction::SSRC##FIELD; \
    }
    SCALAR_INSTRUCTION_FIELDS(ssrc);
    GENERIC_INSTRUCTION_FIELDS_2(ssrc);
#undef X
#undef X_RANGE
    return SOPCInstruction::SSRCIllegal;
}

SISOPCInstruction::OP ParserSISOPC::GetSISOPCOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOPC, op, OP, 16);

    if ((op >= SISOPCInstruction::S_ILLEGALD))
    {
        return SISOPCInstruction::S_ILLEGALD;
    }
    else
    {
        return op;
    }
}

VISOPCInstruction::OP ParserSISOPC::GetVISOPCOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, SOPC, op, OP, 16);

    if ((op >= VISOPCInstruction::S_ILLEGAL))
    {
        return VISOPCInstruction::S_ILLEGAL;
    }
    else
    {
        return op;
    }
}

ParserSI::kaStatus ParserSISOPC::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, Instruction*& instruction, bool , uint32_t , int iLabel /*=NO_LABEL*/ , int iGotoLabel /*=NO_LABEL*/)
{
    unsigned int ridx0 = 0, ridx1 = 0;
    SOPCInstruction::SSRC ssrc0 = GetSSRC(hexInstruction, ridx0, 0);
    SOPCInstruction::SSRC ssrc1 = GetSSRC(hexInstruction, ridx1, 1);

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SISOPCInstruction::OP op = GetSISOPCOp(hexInstruction);
        instruction = new SISOPCInstruction(ssrc0, ssrc1, op, ridx0, ridx1, iLabel, iGotoLabel);
    }
    else
    {
        VISOPCInstruction::OP op = GetVISOPCOp(hexInstruction);
        instruction = new VISOPCInstruction(ssrc0, ssrc1, op, ridx0, ridx1, iLabel, iGotoLabel);
    }

    return ParserSI::Status_SUCCESS;
}

ParserSI::kaStatus ParserSISOPC::Parse(GDT_HW_GENERATION, Instruction::instruction64bit, Instruction*&, int /*iLabel =NO_LABEL*/ , int/* iGotoLabel =NO_LABEL*/)
{
    return ParserSI::Status_64BitInstructionNotSupported;
}
