//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file elf_dumper.h 
/// 
//=====================================================================
#ifndef _ELF_DUMPER_H
#define _ELF_DUMPER_H

#include <string>
#include "elf_decoder.h"
#include <fstream>
#include "AtiElfSI.h"

class ELFDumper
{
public:
    static ELFDumper& singleton(void);
    void destroy(void);
    bool DecodeProgram(char* binaryCode, int length);
    void DecodeShader();
    bool InitOutputStream(std::string output);
    void DumpData();
    void OutputInternalSection();
    void OutputParameterSection();
    void OutputSettingSection();
    void OutputSourceStringSection();
    void GetSymbolTable(const SectionList& sections, uint32 sectionNames);
    void OutputSymbolTable();
    //shader
    void OutputShaderInputs(uint32 n);
    void OutputShaderOutputs(uint32 n);
    void OutputShaderConstants(uint32 n);
    void OutputShaderLoopConstants(uint32 n);
    void OutputShaderIL(uint32 n);
    void OutputShaderText(uint32 n);
    void OutputShaderProgramInfo(uint32 n);
    void OutputShaderProgramUsageInfo(uint32 n);
    void PrintSymbol(const AtiElfSymbol& symbol, std::ofstream& fout);
    void OutputShaderSymbols(uint32 n);
    void CloseOutputStream();

    virtual void GetStatistics(std::string& sStatitics);
    int GetIsaBinarySize();
    void GetIsaBinary(char* ISABinary);
    int GetIlBinarySize();
    void GetILBinary(char* ILBinary);
    virtual void GetStatistics(AtiElf_SC_SI_HWSHADER_BASE*& pStatistics);

private:
    const char* GetConstantTypeStr(AtiElfConstantType type);
    ProgramDecoder* _programDecoder;
    ProgramBinary _prog[6];
    ELFProgram _programStatic[6];
    std::ofstream _outputStream;

    std::vector<unsigned int> progSectionArray;
    std::vector<unsigned int> shaderSectionArray;

    // CodeXL-specific.
    std::string _sStatitics;
    char* _ISABinary;
    int _iIsaBinarySize;
    char* _ILBinary;
    int _iIlBinarySize;
    AtiElf_SC_SI_HWSHADER_BASE* _pStatistics;
};


#endif