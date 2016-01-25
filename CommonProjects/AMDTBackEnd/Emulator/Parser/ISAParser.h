//=====================================================================
// Copyright 2013-2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// 
//=====================================================================

#ifndef __ISAPARSER_H
#define __ISAPARSER_H
#include <AMDTBackEnd/Include/beAMDTBackEndDllBuild.h>

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <set>
#include "ParserSI.h"
#include "ISAProgramGraph.h"

/// Parser for the ISA instructions
class KA_BACKEND_DECLDIR ParserISA
{
public:
    /// ctor
    explicit ParserISA(ParserSI::LoggingCallBackFuncP logFunc = NULL);

    /// dtor
    ~ParserISA();

    /// Parse the 32 instruction.
    /// \returns true if the instruction`s parsing succeeded.
    bool Parse(const std::string& isaLine, GDT_HW_GENERATION asicGen, Instruction::instruction32bit hexInstruction, bool isLiteral32b = false, uint32_t literal32b = 0, int iLabel = NO_LABEL, int iGotoLabel = NO_LABEL, int iLineCount = 0);

    /// Parse the 64 instruction.
    /// \returns true if the instruction`s parsing succeeded.
    bool Parse(const std::string& isaLine, GDT_HW_GENERATION asicGen, Instruction::instruction64bit hexInstruction, int iLabel = NO_LABEL, int iGotoLabel = NO_LABEL, int iLineCount = 0);

    /// Parse the ISA.
    /// \returns true if ISA`s parsing succeeded.
    bool Parse(const std::string& isa);

    /// Parse the ISA for size of.
    /// \returns true if ISA`s parsing succeeded.
    bool ParseForSize(const std::string& isa);

    /// Splits the given isa source code line to a set of strings:
    /// instrOpCode - string representation of the instruction's opcode.
    /// params - string representation of the instruction's parameters.
    /// binaryRepresentation - string representation of the instruction's binary representation.
    /// offset - string representation of the instruction's offset within the program.
    bool SplitIsaLine(const std::string& isaSourceCodeLine, std::string& instrOpCode,
                      std::string& params, std::string& binaryRepresentation, std::string& offset) const;

    /// -----------------------------------------------------------------------------------------------
    /// \brief Name:        GetInstructions
    /// \brief Description: Get all ISA instructions for the program.
    /// \return std::vector<Instruction*>
    /// -----------------------------------------------------------------------------------------------
    const std::vector<Instruction*>& GetInstructions() const { return m_instructions;}

    void GetNumOfInstructionsInCategory(ISAProgramGraph::NumOfInstructionsInCategory NumOfInstructionsInCategory[ISAProgramGraph::CALC_NUM_OF_PATHES], std::string sDumpGraph);

    /// Return the head of the ISA graph
    const ISACodeBlock* GetGraphHead();

    unsigned int GetVgprs() const   { return m_vgprs;    }
    unsigned int GetSgprs() const   { return m_sgprs;    }
    unsigned int GetCodeLen() const { return m_CodeLen;  }

    /// this is for the analysis
    void SetNumOfLoopIteration(int iNumOfLoopIteration);
    int GetNumOfLoopIteration();

private:

    unsigned int m_sgprs;
    unsigned int m_vgprs;
    unsigned int m_CodeLen;

    /// all instructions generated for the ISA
    std::vector<Instruction*> m_instructions;

    /// The map between Parser`s instruction kind identifier and the parser
    std::map<Instruction::InstructionSet, ParserSI*> m_parsersSI;

    /// -----------------------------------------------------------------------------------------------
    /// \brief Name:        ResetInstsCounters
    /// \brief Description: Reset all instruction counters.
    /// \return std::vector<Instruction*>
    /// -----------------------------------------------------------------------------------------------
    void ResetInstsCounters();

    /// -----------------------------------------------------------------------------------------------
    /// \brief Name:        GetLabel
    /// \brief Description: Get an ISA line and return the label if any.
    /// \return label no. or NO_LABEL if none
    /// -----------------------------------------------------------------------------------------------
    int GetLabel(const std::string& sISALine);

    /// -----------------------------------------------------------------------------------------------
    /// \brief Name:        GetGotoLabel
    /// \brief Description: Get an ISA line and return the goto label if any (where this is an branch instruction.
    /// \return label no. or NO_LABEL if none
    /// -----------------------------------------------------------------------------------------------
    int GetGotoLabel(const std::string& sISALine);

    /// -----------------------------------------------------------------------------------------------
    /// \brief Name:        ParseToVector
    /// \brief Description: Parse the ISA program line by line and push the instructions in a vector
    /// \return true if succeeded
    /// -----------------------------------------------------------------------------------------------
    bool ParseToVector(const std::string& isa);

    /// the ISA program graph
    ISAProgramGraph m_pIsaTree;

};

#endif //__ISAPARSER_H

