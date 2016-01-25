//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file elf_dumper.cpp 
/// 
//=====================================================================
#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <assert.h>
#include "elf_dumper.h"
#include "elf_decoder.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include "AtiElfR800.h"
//#include "AtiElfSI.h"
using namespace std;

#ifdef _WINDOWS
    #define CONV      __stdcall             ///< Calling convention for internal Function
#else
    #define CONV
#endif

#define DUMPREG(member)   sprintf(buff, #member": 0x%08x\n", packedData->member); *out += buff;
#define DUMPARRAY(member)   sprintf(buff, #member": 0x%08x, 0x%08x, 0x%08x, 0x%08x\n", packedData->member[0],packedData->member[1],packedData->member[2],packedData->member[3]); *out += buff;
#define DUMPBOOL(member)   sprintf(buff, #member": %s\n", packedData->member ? "true" : "false"); *out += buff;

struct AtiElfCommon
{
    AtiElf_SC_HWSHADER_COMMON;
};


void CONV
sclDumpIL(unsigned int* ilStream, unsigned int ilLength, string* out)
{
    if (ilStream == 0)
    {
        return;
    }

    char* charStream = (char*)ilStream;

    for (unsigned int i = 0; i < ilLength; ++i)
    {
        out->push_back(charStream[i]);
    }
}


//
/// shader stage enums
//
typedef enum cmShaderStageRec
{
    CM_VERTEX_SHADER,                          ///< vertex shader
    CM_TESS_CONTROL_SHADER,                    ///< tessellation control shader (hull shader)
    CM_TESS_EVAL_SHADER,                       ///< tessellation evaluation shader (domain shader)
    CM_GEOMETRY_SHADER,                        ///< geometry shader
    CM_FRAGMENT_SHADER,                        ///< fragment shader (pixel shader)
    CM_COMPUTE_SHADER,                         ///< compute shader
    CM_FETCH_SHADER,                           ///< fetch shader
    cmShaderStage_FIRST = CM_VERTEX_SHADER,                                 ///< First enum
    cmShaderStage_LAST  = CM_FETCH_SHADER,                                  ///< Last enum
    cmShaderStage_COUNT = CM_COMPUTE_SHADER - CM_VERTEX_SHADER + 1,         ///< public shader stage count, fetch shader isn't included
    cmGfxShaderStage_COUNT = CM_FRAGMENT_SHADER - CM_VERTEX_SHADER + 1,     ///< GFX shader stage count
    CM_INVALID_SHADER = -1,                                                 ///< Invalid shader type
} cmShaderStage;

void SI_DumpCommonPackedData(AtiElf_SC_SI_HWSHADER_BASE* packedData, string* out)
{
    char buff[256];
    DUMPREG(X32XhShaderMemHandle);
    DUMPREG(CodeLenInByte);
    DUMPREG(u32UserElementCount);

    for (uint32 i = 0; i < packedData->u32UserElementCount; i++)
    {
        DUMPREG(pUserElement[i].dataClass);
        DUMPREG(pUserElement[i].u32ApiSlot);
        DUMPREG(pUserElement[i].u32StartUserReg);
        DUMPREG(pUserElement[i].u32UserRegCount);
    }

    DUMPREG(u32NumVgprs);
    DUMPREG(u32NumSgprs);
    DUMPREG(u32FloatMode);
    DUMPBOOL(bIeeeMode);
    DUMPBOOL(bUsesPrimId);
    DUMPBOOL(bUsesVertexId);
    DUMPREG(scratchSize);
}

void SI_DumpVSPackedData(AtiElf_SC_SIVSHWSHADER* packedData, string* out)
{
    char buff[256];
    SI_DumpCommonPackedData(packedData, out);
    DUMPREG(numVsInSemantics);

    for (uint32 i = 0; i < packedData->numVsInSemantics; i++)
    {
        DUMPREG(vsInSemantics[i].usage);
        DUMPREG(vsInSemantics[i].usageIdx);
        DUMPREG(vsInSemantics[i].dataVgpr);
        DUMPREG(vsInSemantics[i].dataSize);
    }

    DUMPREG(numVsOutSemantics);

    for (uint32 i = 0; i < packedData->numVsOutSemantics; i++)
    {
        DUMPREG(vsOutSemantics[i].usage);
        DUMPREG(vsOutSemantics[i].usageIdx);
        DUMPREG(vsOutSemantics[i].paramIdx);
    }

    DUMPREG(spiShaderPgmRsrc2Vs);
    DUMPREG(paClVsOutCntl);
    DUMPREG(spiVsOutConfig);
    DUMPREG(spiShaderPosFormat);
    DUMPREG(vgtStrmoutConfig);
    DUMPREG(vgprCompCnt);
    DUMPREG(exportVertexSize);
    DUMPBOOL(useEdgeFlags);
    DUMPBOOL(remapClipDistance);
    DUMPREG(hwShaderStage);
    sprintf(buff, "compileFlags: 0x%08x\n", *(uint32*)&packedData->compileFlags); *out += buff;
    DUMPREG(gsMode);
    DUMPBOOL(isOnChipGs);
    DUMPREG(targetLdsSize);
}

void SI_DumpHSPackedData(AtiElf_SC_SIHSHWSHADER* packedData, string* out)
{
    char buff[256];
    SI_DumpCommonPackedData(packedData, out);
    DUMPREG(spiShaderPgmRsrc2Hs);
    DUMPREG(numInputCP);
    DUMPREG(numOutputCP);
    DUMPREG(numPatchConst);
    DUMPREG(cpStride);
    DUMPREG(numThreadsPerPatch);
    DUMPREG(tessFactorStride);
    DUMPREG(tessDomain);
    DUMPREG(tessPartition);
    DUMPREG(tessOutputPrimitive);
    DUMPREG(maxTessFactor);
    DUMPREG(firstEdgeTessFactorIndex);
    DUMPBOOL(bOffchipLDS);
    DUMPBOOL(isOnChipGs);
}


void SI_DumpGSPackedData(AtiElf_SC_SIGSHWSHADER* packedData, string* out)
{
    char buff[256];
    SI_DumpCommonPackedData(packedData, out);
    DUMPREG(spiShaderPgmRsrc2Gs);
    DUMPREG(vgtGsOutPrimType);
    DUMPREG(vgtGsInstanceCnt);
    DUMPREG(maxOutputVertexCount);
    DUMPARRAY(streamVertexSize);
    DUMPREG(gsMode);
    DUMPBOOL(isOnChipGs);
    DUMPREG(esVertsPerSubgrp);
    DUMPREG(gsPrimsPerSubgrp);
    DUMPREG(ldsEsGsSize);
    SI_DumpVSPackedData(&packedData->copyShader, out);
}


void SI_DumpCSPackedData(AtiElf_SC_SICSHWSHADER* packedData, string* out)
{
    char buff[256];
    SI_DumpCommonPackedData(packedData, out);
    DUMPREG(computePgmRsrc2);
    DUMPREG(computeMaxWaveId);
    DUMPREG(numThreadX);
    DUMPREG(numThreadY);
    DUMPREG(numThreadZ);
    DUMPBOOL(bOrderedAppendEnable);
}

void SI_DumpPSPackedData(AtiElf_SC_SIPSHWSHADER* packedData, string* out)
{
    char buff[256];
    SI_DumpCommonPackedData(packedData, out);
    DUMPREG(numPsInSemantics);

    for (uint32 i = 0; i < packedData->numPsInSemantics; i++)
    {
        DUMPREG(psInSemantics[i].usage);
        DUMPREG(psInSemantics[i].usageIdx);
        DUMPREG(psInSemantics[i].inputIdx);
        DUMPREG(psInSemantics[i].defaultVal);
        DUMPREG(psInSemantics[i].flatShade);
        DUMPREG(psInTexCoordIndex[i]);
    }

    DUMPREG(spiShaderPgmRsrc2Ps);
    DUMPREG(spiShaderZFormat);
    DUMPREG(spiPsInControl);
    DUMPREG(spiPsInputAddr);
    DUMPREG(spiPsInputEna);
    DUMPREG(spiBarycCntl);
    DUMPREG(dbShaderControl);
    DUMPREG(cbShaderMask);
    DUMPREG(exportPatchCodeSize);
    DUMPREG(numPsExports);
    DUMPREG(dualBlending);
    DUMPREG(defaultExportFmt);
    //TODO: dump exportPatchInfo

}

void SI_DumpHWShaderData(void* data, unsigned int length, cmShaderStage stage, char*& outISA, int& iOutIsaSize, std::string* out)
{
    (void)length;
    //AtiElfCommon* elfPackedData = (AtiElfCommon*)data;

    const char* il = NULL;
    const char* il2 = NULL;
    uint32 codeinbytes = 0;
    uint32 copyshaderbytes = 0;

    switch (stage)
    {
        case CM_VERTEX_SHADER:
        {

            il = (const char*) & ((AtiElf_SC_SIVSHWSHADER*)data)[1];
            codeinbytes = ((AtiElf_SC_SIVSHWSHADER*)data)->CodeLenInByte;
            break;
        }

        case CM_TESS_CONTROL_SHADER:
        {

            il = (const char*) & ((AtiElf_SC_SIHSHWSHADER*)data)[1];
            codeinbytes = ((AtiElf_SC_SIHSHWSHADER*)data)->CodeLenInByte;
            break;
        }

        case CM_TESS_EVAL_SHADER:
        {
            il = (const char*) & ((AtiElf_SC_SIVSHWSHADER*)data)[1];
            codeinbytes = ((AtiElf_SC_SIVSHWSHADER*)data)->CodeLenInByte;
            break;
        }

        case CM_GEOMETRY_SHADER:
        {

            il = (const char*) & ((AtiElf_SC_SIGSHWSHADER*)data)[1];
            il2 = (const char*) & ((AtiElf_SC_SIGSHWSHADER*)data)[1] + ((AtiElf_SC_SIGSHWSHADER*)data)->CodeLenInByte;
            codeinbytes = ((AtiElf_SC_SIGSHWSHADER*)data)->CodeLenInByte;
            copyshaderbytes = ((AtiElf_SC_SIGSHWSHADER*)data)->copyShader.CodeLenInByte;
            break;
        }

        case CM_FRAGMENT_SHADER:
        {
            il = (const char*) & ((AtiElf_SC_SIPSHWSHADER*)data)[1];
            codeinbytes = ((AtiElf_SC_SIPSHWSHADER*)data)->CodeLenInByte;
            break;
        }

        case CM_COMPUTE_SHADER:
        {

            il = (const char*) & ((AtiElf_SC_SICSHWSHADER*)data)[1];
            codeinbytes = ((AtiElf_SC_SICSHWSHADER*)data)->CodeLenInByte;
            break;
        }

        default:
            //assert(0);
            return;
    }


    // isa
    iOutIsaSize = codeinbytes;
    outISA = new char[codeinbytes];
    memcpy(outISA, il, codeinbytes);

    //const char* hwstring = "\n--------------------------- isa binary ------------------------\n";
    //out->append(hwstring);
    //out->append(il, codeinbytes);
    //const char* hwstring = "\n\n---------------------------- statics --------------------------\n";
    //out->append(hwstring);

    switch (stage)
    {
        case CM_VERTEX_SHADER:
            SI_DumpVSPackedData((AtiElf_SC_SIVSHWSHADER*)data, out);
            break;

        case CM_TESS_CONTROL_SHADER:
            SI_DumpHSPackedData((AtiElf_SC_SIHSHWSHADER*)data, out);
            break;

        case CM_TESS_EVAL_SHADER:
            SI_DumpVSPackedData((AtiElf_SC_SIVSHWSHADER*)data, out);
            break;

        case CM_GEOMETRY_SHADER:
            SI_DumpGSPackedData((AtiElf_SC_SIGSHWSHADER*)data, out);
            break;

        case CM_FRAGMENT_SHADER:
            SI_DumpPSPackedData((AtiElf_SC_SIPSHWSHADER*)data, out);
            break;

        case CM_COMPUTE_SHADER:
            SI_DumpCSPackedData((AtiElf_SC_SICSHWSHADER*)data, out);
            break;

        default:
            //assert(0);
            return;
    }
}





void CONV sclDumpHWData(void* data, unsigned int length, int asicID, cmShaderStage stage, char*& outISA, int& iOutIsaSize, std::string* out)
{
    (void)asicID;

    // We filter pre-GCN devices earlier in the compilation process, so
    // we can simply treat all devices as valid GCN devices.
    SI_DumpHWShaderData(data, length, stage, outISA, iOutIsaSize, out);
}


void printSectionTitle(const string title, size_t n, ofstream& fout)
{
    for (uint32 i = 0; i < n; i++)
    {
        fout << "*";
    }

    fout << endl;
    fout << title.c_str() << endl;

    for (uint32 i = 0; i < n; i++)
    {
        fout << "*";
    }

    fout << endl;
}


bool ELFDumper::DecodeProgram(char* binaryCode, int length)
{
    _programDecoder = new ProgramDecoder(binaryCode, length);

    if (!_programDecoder->ExtractProgramBinary(_prog))
    {
        return false;
    }
    else
    {
        return true;
    }
}


bool ELFDumper::InitOutputStream(std::string output)
{
    _outputStream.open(output.c_str(), std::ios_base::out);

    if (_outputStream.good())
    {
        return true;
    }

    return false;
}


void ELFDumper::DumpData()
{
    //initialize default sections
    if (progSectionArray.size() == 0)
    {
        progSectionArray.push_back(SHT_LOUSER + ATIELF_INTERNAL_SECTION_OFFSET);
        progSectionArray.push_back(SHT_LOUSER + ATIELF_PARAMETER_SECTION_OFFSET);
        progSectionArray.push_back(SHT_LOUSER + ATIELF_SETTING_SECTION_OFFSET);
        progSectionArray.push_back(SHT_LOUSER + ATIELF_SOURCESTRING_SECTION_OFFSET);
        progSectionArray.push_back(SHT_SYMTAB);
    }

    if (shaderSectionArray.size() == 0)
    {
        shaderSectionArray.push_back(SHT_LOUSER + ATIELF_INPUT_SECTION_OFFSET);
        shaderSectionArray.push_back(SHT_LOUSER + ATIELF_OUTPUT_SECTION_OFFSET);
        shaderSectionArray.push_back(SHT_LOUSER + ATIELF_CONSTANT_SECTION_OFFSET);
        shaderSectionArray.push_back(SHT_LOUSER + ATIELF_LOOPCONSTANT_SECTION_OFFSET);
        shaderSectionArray.push_back(SHT_LOUSER + ATIELF_IL_SECTION_OFFSET);
        shaderSectionArray.push_back(SHT_PROGBITS);
        shaderSectionArray.push_back(SHT_LOUSER + ATIELF_PROGRAMINFO_SECTION_OFFSET);
        shaderSectionArray.push_back(SHT_LOUSER + ATIELF_PROGUSAGEINFO_SECTION_OFFSET);
        shaderSectionArray.push_back(SHT_LOUSER + ATIELF_SYMBOL_SECTION_OFFSET);
    }

    for (uint32 i = 0; i < progSectionArray.size(); i++)
    {
        switch (progSectionArray[i])
        {
            case SHT_LOUSER + ATIELF_INTERNAL_SECTION_OFFSET:
                OutputInternalSection();
                break;

            case SHT_LOUSER + ATIELF_PARAMETER_SECTION_OFFSET:
                OutputParameterSection();
                break;

            case SHT_LOUSER + ATIELF_SETTING_SECTION_OFFSET:
                OutputSettingSection();
                break;

            case SHT_LOUSER + ATIELF_SOURCESTRING_SECTION_OFFSET:
                OutputSourceStringSection();
                break;

            case SHT_SYMTAB:
                OutputSymbolTable();
                break;
        }
    }
}


void ELFDumper::GetSymbolTable(const SectionList& sections, uint32 sectionNames)
{
    uint32 symName = _programDecoder->GetSectionIndex(sectionNames, sections, ".symtab");
    uint32 strName = _programDecoder->GetSectionIndex(sectionNames, sections, ".strtab");

    char*  symbolName = NULL;
    uint32 shaderlength;
    void* shaderdata = NULL;

    std::string title = "*               Symbol Table               *";
    printSectionTitle(title, title.length(), _outputStream);

    _outputStream << setw(40) << setiosflags(ios::left) << "symbol name" << setw(10) << setiosflags(ios::left) << "data size" << endl;

    for (uint32 count = 1; count < sections[symName].header.sh_size / sizeof(Elf32_Sym); count++)
    {
        _programDecoder->ReadSymData(count, symName, strName, sections, symbolName, shaderdata, shaderlength);
        _outputStream << setw(40) << setiosflags(ios::left) << symbolName << setw(10) << setiosflags(ios::left) << shaderlength << endl;
    }

    _outputStream << endl;
}


void ELFDumper::OutputSourceStringSection()
{
    string title = "*               Source String Section               *";
    printSectionTitle(title, title.length(), _outputStream);
    const char shaderStage[] = {'V', 'H', 'D', 'G', 'F', 'C'};
    char str[30];

    for (uint32 shaderName = 0; shaderName < 6; shaderName++)
    {
        if (!_prog[shaderName].sourceString.empty())
        {
            sprintf(str, "-------Shader_%c_Source------", shaderStage[shaderName]);
            _outputStream << str << endl << endl;
            _outputStream << _prog[shaderName].sourceString[0].c_str() << endl << endl;
        }
    }
}

void ELFDumper::OutputInternalSection()
{
    string title = "*               Source Internal Section               *";
    printSectionTitle(title, title.length(), _outputStream);

    const char shaderStage[] = {'V', 'H', 'D', 'G', 'F', 'C'};
    char str[50];

    for (uint32 shaderName = 0; shaderName < 6; shaderName++)
    {
        if (!_prog[shaderName].sclProg.empty())
        {
            sprintf(str, "------Shader_%c_SCLProgram------", shaderStage[shaderName]);
            _outputStream << str << endl << endl;
            _outputStream << setw(20) << setiosflags(ios::left) << "Data Length" << _prog[shaderName].sclProg[0]->len << endl;
            _outputStream << setw(20) << setiosflags(ios::left) << "language" << _prog[shaderName].sclProg[0]->language << endl;
            _outputStream << setw(20) << setiosflags(ios::left) << "errorCode" << _prog[shaderName].sclProg[0]->errorCode << endl;
            _outputStream << setw(20) << setiosflags(ios::left) << "errorPos" << _prog[shaderName].sclProg[0]->errorPos << endl;
            _outputStream << setw(20) << setiosflags(ios::left) << "errorString" << _prog[shaderName].sclProg[0]->errorString.c_str() << endl;
            _outputStream << endl;
        }
    }
}



void ELFDumper::OutputSettingSection()
{
    string title = "*               Setting Section               *";
    printSectionTitle(title, title.length(), _outputStream);
    _outputStream << setw(20) << setiosflags(ios::left) << "asicID" << _prog[0].asicID << endl;
    _outputStream << setw(20) << setiosflags(ios::left) << "sourceVersion" << _prog[0].sourceVersion.c_str() << endl;
    _outputStream << endl;
}

void ELFDumper::OutputParameterSection()
{
    string title = "*               Parameter Section               *";
    printSectionTitle(title, title.length(), _outputStream);

    _outputStream << "bindAttribLocationMap      " << "Length = " << _prog[0].parameter->bindAttribLocationMap.size() << endl;
    _outputStream << setw(10) << setiosflags(ios::left) << "key" << setw(10) << setiosflags(ios::left) << "value" << endl;
    map<string, uint32>::iterator bal_it;

    for (bal_it = _prog[0].parameter->bindAttribLocationMap.begin(); bal_it != _prog[0].parameter->bindAttribLocationMap.end(); ++bal_it)
    {
        _outputStream << setw(10) << setiosflags(ios::left) << bal_it->first.c_str() << setw(10) << setiosflags(ios::left) << bal_it->second << endl;
    }

    _outputStream << endl;

    _outputStream << "bindFragDataLocationMap      " << "Length = " << _prog[0].parameter->bindFragDataLocationMap.size() << endl;
    _outputStream << setw(10) << setiosflags(ios::left) << "key" << setw(10) << setiosflags(ios::left) << "value" << endl;
    map<string, uint32>::iterator bfdl_it;

    for (bfdl_it = _prog[0].parameter->bindFragDataLocationMap.begin(); bfdl_it != _prog[0].parameter->bindFragDataLocationMap.end(); ++bfdl_it)
    {
        _outputStream << setw(10) << setiosflags(ios::left) << bfdl_it->first.c_str() << setw(10) << setiosflags(ios::left) << bfdl_it->second << endl;
    }

    _outputStream << endl;

    _outputStream << "tfVaryings      " << "Length = " << _prog[0].parameter->tfVaryingCount << endl;
    _outputStream << setw(10) << setiosflags(ios::left) << "name" << setw(10) << setiosflags(ios::left) << "u32ALL" << endl;

    for (uint32 i = 0; i < _prog[0].parameter->tfVaryingCount; i++)
    {
        _outputStream << setw(10) << setiosflags(ios::left) << _prog[0].parameter->tfVaryings[i].name << setw(10) << setiosflags(ios::left) << _prog[0].parameter->tfVaryings[i].u32All << endl;
    }

    _outputStream << endl;

    _outputStream << setw(20) << setiosflags(ios::left) << "tfBufferMode" << _prog[0].parameter->tfBufferMode << endl;
    _outputStream << endl;
}
ELFDumper* uniqueDumper = NULL;
ELFDumper& ELFDumper::singleton(void)
{
    if (uniqueDumper == NULL)
    {
        uniqueDumper = new ELFDumper();
        uniqueDumper->_ISABinary = NULL;
        uniqueDumper->_iIsaBinarySize = 0;
        uniqueDumper->_ILBinary = NULL;
        uniqueDumper->_iIlBinarySize = 0;
        uniqueDumper->_pStatistics = NULL;
    }

    return *uniqueDumper;
}

void ELFDumper::destroy(void)
{
    if (uniqueDumper)
    {
        if (NULL != _ISABinary)
        {
            delete[] _ISABinary;
            _ISABinary = NULL;
        }

        if (NULL != _ILBinary)
        {
            delete[] _ILBinary;
            _ILBinary = NULL;
        }

        _iIsaBinarySize = 0;
        _iIlBinarySize = 0;
        delete uniqueDumper;
    }

    uniqueDumper = NULL;
}

void ELFDumper::OutputSymbolTable()
{
    //_symbolTable.clear();
    _programDecoder->Seek(0);

    Elf32_Ehdr header;
    _programDecoder->ReadHeader(header);

    SectionList sections;
    _programDecoder->Seek(header.e_shoff);

    for (uint32 i = 0; i < header.e_shnum; i++)
    {
        _programDecoder->ReadSectionHeaderTableEntry(sections);
    }

    for (uint32 i = 0; i < sections.size(); i++)
    {
        if (sections[i].header.sh_type == SHT_LOUSER + ATIELF_INTERNAL_SECTION_OFFSET)
        {
            GetSymbolTable(sections, header.e_shstrndx);
        }
    }
}

string getShaderName(sclShaderLanguage lang)
{
    switch (lang)
    {
        case SCL_VERTEX_SHADER:
        case SCL_VERTEX_PROGRAM:
        case SCL_IL_VERTEX_SHADER:
            return "Vertex Shader Data";

        case SCL_HULL_SHADER:
            return "Hull Shader Data";

        case SCL_DOMAIN_SHADER:
            return "Domain Shader Data";

        case SCL_GEOMETRY_SHADER:
            return "Geometry Shader Data";

        case SCL_COMPUTE_SHADER:
            return "Compute Shader Data";

        case SCL_FETCH_SHADER_BIN:
            return "Fetch Shader Data";

        case SCL_FRAGMENT_SHADER:
        case SCL_FRAGMENT_PROGRAM:
        case SCL_IL_FRAGMENT_SHADER:
            return "Fragment Shader Data";

        default:
            return "";
    }
}

void printShaderTitle(uint32 stage, uint32 n, ofstream& fout)
{
    fout << endl;

    for (uint32 i = 0; i < n; i++)
    {
        fout << "-";
    }

    fout << getShaderName((sclShaderLanguage) stage).c_str();

    for (uint32 i = 0; i < n; i++)
    {
        fout << "-";
    }

    fout << endl;
    fout << endl;
}


void ELFDumper::DecodeShader()
{
    for (int i = 0; i < 6; i++)
    {
        if (!_prog[i].sclProg.empty())
        {
            ShaderDecoder decoder(_prog[i].sclProg[0]->language, _prog[i].sclProg[0]->data, _prog[i].sclProg[0]->len);

            if (!decoder.ExtractShaderBinary(&_programStatic[i], _prog[i].sclProg[0]))
            {
                printf("Unable to decode the %s \n", getShaderName(_prog[i].sclProg[0]->language).c_str());
            }
            else
            {
                printShaderTitle(i, 60, _outputStream);

                for (size_t j = 0; j < shaderSectionArray.size(); j++)
                {
                    switch (shaderSectionArray[j])
                    {
                        case SHT_LOUSER + ATIELF_INPUT_SECTION_OFFSET:
                            OutputShaderInputs(i);
                            break;

                        case SHT_LOUSER + ATIELF_OUTPUT_SECTION_OFFSET:
                            OutputShaderOutputs(i);
                            break;

                        case SHT_LOUSER + ATIELF_CONSTANT_SECTION_OFFSET:
                            OutputShaderConstants(i);
                            break;

                        case SHT_LOUSER + ATIELF_LOOPCONSTANT_SECTION_OFFSET:
                            OutputShaderLoopConstants(i);
                            break;

                        case SHT_LOUSER + ATIELF_IL_SECTION_OFFSET:
                            OutputShaderIL(i);
                            break;

                        case SHT_PROGBITS:
                            OutputShaderText(i);
                            break;

                        case SHT_LOUSER + ATIELF_PROGRAMINFO_SECTION_OFFSET:
                            OutputShaderProgramInfo(i);
                            break;

                        case SHT_LOUSER + ATIELF_PROGUSAGEINFO_SECTION_OFFSET:
                            OutputShaderProgramUsageInfo(i);
                            break;

                        case SHT_LOUSER + ATIELF_SYMBOL_SECTION_OFFSET:
                            OutputShaderSymbols(i);
                            break;
                    }
                }
            }
        }
    }
}


const char* ELFDumper::GetConstantTypeStr(AtiElfConstantType type)
{
    switch (type)
    {
        case ATIELF_CONST_LITERAL:
            return "literal";

        case ATIELF_CONST_LOCAL:
            return "local";

        case ATIELF_CONST_GLOBAL:
            return "global";

        default:
            return "unknown";
    }
}
void ELFDumper::OutputShaderConstants(uint32 n)
{
    string title = "*               Shader Constants               *";
    printSectionTitle(title, title.length(), _outputStream);
    //Literal constatns
    _outputStream << "Constants      " << "Length = " << _programStatic[n].numConstants << endl;
    _outputStream
            << setw(10) << setiosflags(ios::left) << "Type"
            << setw(20) << setiosflags(ios::left) << "Virtual offset"
            << setw(20) << setiosflags(ios::left) << "Physical offset"
            << setw(40) << setiosflags(ios::left) << "Literal value" << endl;

    for (uint32 i = 0; i < _programStatic[n].numConstants; i++)
    {
        _outputStream
                << setw(10) << setiosflags(ios::left) << GetConstantTypeStr(_programStatic[n].constants[i].type)
                << setw(20) << setiosflags(ios::left) << _programStatic[n].constants[i].vofs
                << setw(20) << setiosflags(ios::left) << _programStatic[n].constants[i].pofs
                << setiosflags(ios::left)
                << "float: "
                << _programStatic[n].constants[i].value[0] << " "
                << _programStatic[n].constants[i].value[1] << " "
                << _programStatic[n].constants[i].value[2] << " "
                << _programStatic[n].constants[i].value[3] << "; "
                << "int: "
                << *(int32*)(&_programStatic[n].constants[i].value[0]) << " "
                << *(int32*)(&_programStatic[n].constants[i].value[1]) << " "
                << *(int32*)(&_programStatic[n].constants[i].value[2]) << " "
                << *(int32*)(&_programStatic[n].constants[i].value[3]) << "; "
                << endl;
    }

    _outputStream << endl;

}

void ELFDumper::OutputShaderLoopConstants(uint32 n)
{
    string title = "*               Shader Loop Constants               *";
    printSectionTitle(title, title.length(), _outputStream);
    //loop constants
    _outputStream << "loop constants      " << "Length = " << _programStatic[n].numIntConstants << endl;
    _outputStream
            << setw(20) << setiosflags(ios::left) << "Type"
            << setw(20) << setiosflags(ios::left) << "Virtual offset"
            << setw(20) << setiosflags(ios::left) << "Physical offset"
            << setw(20) << setiosflags(ios::left) << "Constants value" << endl;

    for (uint32 i = 0; i < _programStatic[n].numIntConstants; i++)
    {
        _outputStream
                << setw(20) << setiosflags(ios::left) << _programStatic[n].intConstants[i].type
                << setw(20) << setiosflags(ios::left) << _programStatic[n].intConstants[i].vofs
                << setw(20) << setiosflags(ios::left) << _programStatic[n].intConstants[i].pofs
                << setw(20) << setiosflags(ios::left) << _programStatic[n].intConstants[i].value << endl;
    }

    _outputStream << endl;
}

void ELFDumper::OutputShaderIL(uint32 n)
{
    if (_outputStream.good())
    {
        string title = "*               Shader IL  binary            *";
        printSectionTitle(title, title.length(), _outputStream);
        //Convert IL stream to pointer array

        for (uint32 i = 0; i < _programStatic[n].ilStreamSizeInBytes; ++i)
        {
            _outputStream << _programStatic[n].ilByteStream[i];
        }

        _outputStream << std::endl;
    }
    else
    {
        _iIlBinarySize = _programStatic[n].ilStreamSizeInBytes;
        _ILBinary = new char[_programStatic[n].ilStreamSizeInBytes];
        memcpy(_ILBinary, _programStatic[n].ilByteStream, _iIlBinarySize);
    }
}

void ELFDumper::OutputShaderText(uint32 stage)
{
    string title = "*               elf hardware section              *";
    printSectionTitle(title, title.length(), _outputStream);

    sclDumpHWData(_programStatic[stage].text, _programStatic[stage].textSize, _prog[0].asicID, (cmShaderStage)stage, _ISABinary, _iIsaBinarySize, &_sStatitics);

    switch (stage)
    {
        case CM_VERTEX_SHADER:
            _pStatistics = (AtiElf_SC_SIVSHWSHADER*)_programStatic[stage].text;
            break;

        case CM_TESS_CONTROL_SHADER:
            _pStatistics = (AtiElf_SC_SIHSHWSHADER*)_programStatic[stage].text;
            break;

        case CM_TESS_EVAL_SHADER:
            _pStatistics = (AtiElf_SC_SIVSHWSHADER*)_programStatic[stage].text;
            break;

        case CM_GEOMETRY_SHADER:
            _pStatistics = (AtiElf_SC_SIGSHWSHADER*)_programStatic[stage].text;
            break;

        case CM_FRAGMENT_SHADER:
            _pStatistics = (AtiElf_SC_SIPSHWSHADER*)_programStatic[stage].text;
            break;

        case CM_COMPUTE_SHADER:
            _pStatistics = (AtiElf_SC_SICSHWSHADER*)_programStatic[stage].text;
            break;

        default:
            //assert(0);
            return;
    }
}

void ELFDumper::OutputShaderProgramInfo(uint32 n)
{
    string title = "*               Shader Program Info              *";
    printSectionTitle(title, title.length(), _outputStream);
    AtiElfProgramInfo* progInfo = _programStatic[n].progInfo;

    switch (n)
    {
        case 0:
            _outputStream << "tess prim type: " << progInfo->VertexShaderStruct._tessPrimType << endl;
            _outputStream << "uses vertex ID: " << progInfo->VertexShaderStruct._usesVertexID << endl;
            _outputStream << "uses base vertex: " << progInfo->VertexShaderStruct._usesBaseVertex << endl;
            _outputStream << "uses base instance: " << progInfo->VertexShaderStruct._usesBaseInstance << endl;
            break;

        case 5:
            _outputStream << "work size x: " << progInfo->ComputeShaderStruct._numThreadPerGroupX << endl;
            _outputStream << "work size y: " << progInfo->ComputeShaderStruct._numThreadPerGroupY << endl;
            _outputStream << "work size y: " << progInfo->ComputeShaderStruct._numThreadPerGroupZ << endl;

        case 4:
            _outputStream << "using dual blend mask: " << progInfo->FragmentShaderStruct._usingDualBlendMask << endl;
            _outputStream << "uses flat interp qualifier: " << progInfo->FragmentShaderStruct._useFlatInterpMode << endl;

        default:
            break;
    }

    _outputStream << "use constant buffers: " << progInfo->_constantBufferMask << endl;
    _outputStream << "max scratch buffer needed: " << progInfo->_maxScratchRegsNeeded << endl;
    _outputStream << "max uniform location: " << progInfo->maxUniformLocation << endl;
    _outputStream << "max subroutine Index: " << progInfo->maxSubroutineIndex << endl;
    _outputStream << "max subroutine uniform location: " << progInfo->maxSubroutineUniformLocation << endl;
    _outputStream << endl;
}

void ELFDumper::OutputShaderProgramUsageInfo(uint32 stage)
{
    string title = "*               Shader Program Usage Info              *";
    printSectionTitle(title, title.length(), _outputStream);
    AtiElfProgramUsageInfo* usageInfo = _programStatic[stage].progUsageInfo;

    if (!usageInfo)
    {
        return;
    }

    _outputStream << "instructions: " << usageInfo->_arbProgramParameter._instructions << endl;
    _outputStream << "native instructions: " << usageInfo->_arbProgramParameter._nativeInstructions << endl;
    _outputStream << "alu instuctions: " << usageInfo->_arbProgramParameter._aluInstructions << endl;
    _outputStream << "native alu instructions: " << usageInfo->_arbProgramParameter._nativeAluInstructions << endl;
    _outputStream << "tex instructions: " << usageInfo->_arbProgramParameter._texInstructions << endl;
    _outputStream << "native tex instructions: " << usageInfo->_arbProgramParameter._nativeTexInstructions << endl;
    _outputStream << "tex indirections: " << usageInfo->_arbProgramParameter._texIndirections << endl;
    _outputStream << "native tex indirections: " << usageInfo->_arbProgramParameter._nativeTexIndirections << endl;
    _outputStream << "temporaries: " << usageInfo->_arbProgramParameter._temporaries << endl;
    _outputStream << "native temporaries: " << usageInfo->_arbProgramParameter._nativeTemporaries << endl;
    _outputStream << "parameters: " << usageInfo->_arbProgramParameter._parameters << endl;
    _outputStream << "native parameters: " << usageInfo->_arbProgramParameter._nativeParameters << endl;
    _outputStream << "attribs: " << usageInfo->_arbProgramParameter._attribs << endl;
    _outputStream << "native attribs: " << usageInfo->_arbProgramParameter._nativeAttribs << endl;
    _outputStream << "address registers: " << usageInfo->_arbProgramParameter._addressRegisters << endl;
    _outputStream << "native address registers: " << usageInfo->_arbProgramParameter._nativeAddressRegisters << endl;
    _outputStream << "under native limits: " << usageInfo->_arbProgramParameter._underNativeLimits << endl;

    _outputStream << "uses primitive ID: " << usageInfo->_usesPrimitiveID << endl;
    _outputStream << "uses clip distatance: " << usageInfo->_usesClipDistance << endl;
    _outputStream << "uses UAV image mask: " << usageInfo->_uavImageMask << endl;
    _outputStream << "uses UAV image mask shader storage buffer: " << usageInfo->_uavImageMaskShaderStorageBuffer << endl;

    _outputStream << "uses tex image mask: " << usageInfo->_usesTexImageMask << endl;
    _outputStream << "tex image mask: " << usageInfo->_texImageMask << endl;
    _outputStream << "uses shadow mask: " << usageInfo->_usesShadowMask << endl;

    switch (stage)
    {
        case CM_VERTEX_SHADER:
            _outputStream << "position invariant: " << usageInfo->VertexShaderParamStruct._positionInvariant << endl;
            _outputStream << "is stride: " << usageInfo->VertexShaderParamStruct._lsStride << endl;
            _outputStream << "fs type: " << (int32)usageInfo->VertexShaderParamStruct._fetchType << endl;
            _outputStream << "fs rentern address reg: " << (int32)usageInfo->VertexShaderParamStruct._fetchReturnAddrReg << endl;
            _outputStream << "fs input stream table reg: " << (int32)usageInfo->VertexShaderParamStruct._fetchInputStreamTableReg << endl;
            _outputStream << "fs vertex id channel: " << (int32)usageInfo->VertexShaderParamStruct._fetchVertexIdChannel << endl;
            _outputStream << "enable another set attrib alias: " << usageInfo->VertexShaderParamStruct._enableAnotherSetAttribAlias << endl;
            break;

        case CM_FRAGMENT_SHADER:
            _outputStream << "uses primary color: " << usageInfo->FragmentShaderParamStruct._usePrimaryColor << endl;
            _outputStream << "uses secondary color: " << usageInfo->FragmentShaderParamStruct._useSecondaryColor << endl;
            _outputStream << "conservative z: " << usageInfo->FragmentShaderParamStruct._conservativeZ << endl;
            _outputStream << "uses fog coord: " << usageInfo->FragmentShaderParamStruct._usesFogCoord << endl;
            _outputStream << "uses tex coord mask: " << usageInfo->FragmentShaderParamStruct._usesTexCoordMask << endl;
            _outputStream << "aa stipple tex coord: " << (int32)(usageInfo->FragmentShaderParamStruct._aaStippleTexCoord) << endl;
            _outputStream << "aa stipple tex unit: " << (int32)(usageInfo->FragmentShaderParamStruct._aaStippleTexUnit) << endl;
            _outputStream << "bitmap tex coord: " << (int32)(usageInfo->FragmentShaderParamStruct._bitmapTexCoord) << endl;
            _outputStream << "bitmap tex unit: " << (int32)(usageInfo->FragmentShaderParamStruct._bitmapTexUnit) << endl;
            _outputStream << "aa stipple tex vir unit: " << (int32)(usageInfo->FragmentShaderParamStruct._aaStippleTexVirtUnit) << endl;
            _outputStream << "bitmap tex vir unit: " << (int32)(usageInfo->FragmentShaderParamStruct._bitmapTexVirtUnit) << endl;
            _outputStream << "need sample info CBs: " << usageInfo->FragmentShaderParamStruct._needSampleInfoCBs << endl;
            break;

        case CM_GEOMETRY_SHADER:
            _outputStream << "geometery max gs vertices: " << usageInfo->GeometryShaderParamStruct._maxGSVertices[0];
            _outputStream << "geometry input topology: " << usageInfo->GeometryShaderParamStruct._inputTopology;
            _outputStream << "geometry output topology: " << usageInfo->GeometryShaderParamStruct._outputTopology[0];
            _outputStream << "geometry invocations: " << usageInfo->GeometryShaderParamStruct._gsInvocations;
            break;

        case CM_TESS_CONTROL_SHADER:
            _outputStream << "tess output vertices: " << usageInfo->TessellationEvalShaderParamStruct._tessOutputVertices;
            break;

        case CM_TESS_EVAL_SHADER:
            _outputStream << "tess gen mode: " << usageInfo->TessellationControlShaderParamStruct._tessGenMode;
            _outputStream << "tess gen spacing: " << usageInfo->TessellationControlShaderParamStruct._tessGenSpacing;
            _outputStream << "tess genvertex order: " << usageInfo->TessellationControlShaderParamStruct._tessGenVertexOrder;
            _outputStream << "tess gen point mode: " << usageInfo->TessellationControlShaderParamStruct._tessGenPointMode;
            break;

        case CM_COMPUTE_SHADER:
            _outputStream << "workDimension: " << usageInfo->ComputeShaderParamStruct._workDimension;
            _outputStream << "workSizeX: " << usageInfo->ComputeShaderParamStruct._workSizeX;
            _outputStream << "workSizeY: " << usageInfo->ComputeShaderParamStruct._workSizeY;
            _outputStream << "workSizeZ: " << usageInfo->ComputeShaderParamStruct._workSizeZ;
            break;

        default:
            break;
    }

    _outputStream << "Texture    " << "Length = " << usageInfo->_textureTypeSize << endl;
    //TODO: _outputStream
    //<<setw(20)<<setiosflags(ios::left)<<"Type"<<endl;
    //for (uint32 i = 0; i < _programStatic[n].usageInfo._usesTextureSize; i++)
    //{
    //  _outputStream<<usageInfo._usesTexture[i]<<endl;
    //}
    _outputStream << "Image    " << "Length = " << usageInfo->_imageTypeAndFormatSize << endl;
    //TODO: _outputStream
    //<<setw(20)<<setiosflags(ios::left)<<"Type"
    //  <<setw(20)<<setiosflags(ios::left)<<"imageFormat"<<endl;
    //for (uint32 i = 0; i < usageInfo._usesImageSize; i++)
    //{
    //  _outputStream
    //  <<setw(20)<<setiosflags(ios::left)<<usageInfo._usesImage[i].type
    //      <<setw(20)<<setiosflags(ios::left)<<usageInfo._usesImage[i].imageFormat<<endl;
    //}
    _outputStream << endl;
}

void ELFDumper::PrintSymbol(const AtiElfSymbol& symbol, ofstream& fout)
{
    //Inherited property
    fout << "type: " << symbol.type << endl;
    fout << "data type: " << symbol.dataType << endl;
    fout << "size: " << symbol.size << endl;
    fout << "count: " << symbol.count << endl;
    fout << "isArray: " << symbol.isArray << endl;
    //Qualifier
    fout << "qualifier:" << "(precision: " << symbol.AtiElfStruct.dataPrecQual << " layoutStyle: " << symbol.AtiElfStruct.layoutStyleQualifier << " layoutMajor: " << symbol.AtiElfStruct.layoutMajorQualifier <<
         " invariant: " << symbol.AtiElfStruct.invariantQualifier << " precise: " << symbol.AtiElfStruct.preciseQualifier << " storage: " << symbol.AtiElfStruct.storageQualifier << " interp: " << symbol.AtiElfStruct.interpQualifier <<
         " patch: " << symbol.AtiElfStruct.patchQualifier << ")" << endl;

    //Name
    if (symbol.name)
    {
        fout << "name: " << symbol.name << endl;
    }

    if (symbol.baseName)
    {
        fout << "base name: " << symbol.baseName << endl;
    }

    if (symbol.uniformBlockName)
    {
        fout << "uniform block name: " << symbol.uniformBlockName << endl;
    }
}

void ELFDumper::OutputShaderSymbols(uint32 n)
{
    string title = "*               Shader Symbols               *";
    printSectionTitle(title, title.length(), _outputStream);

    _outputStream << "Symbols Table   " << "Length = " << _programStatic[n].numSymbols << endl;

    for (uint32 i = 0; i < _programStatic[n].numSymbols; i++)
    {
        _outputStream << "----------No." << i << "----------" << endl;
        PrintSymbol(_programStatic[n].symbols[i], _outputStream);
        _outputStream << "subsize: " << _programStatic[n].symbols[i].subSize << endl;
        _outputStream << "subroutine type id: " << _programStatic[n].symbols[i].subroutineTypeID << endl;
        _outputStream << "uniform offset: " << _programStatic[n].symbols[i].uniformOffset << endl;
        _outputStream << "uniform binding: " << _programStatic[n].symbols[i].uniformBinding << endl;
        _outputStream << "resource offset: " << _programStatic[n].symbols[i].resourceIndex << endl;
        _outputStream << "layout location: " << _programStatic[n].symbols[i].layoutLocation << endl;
        _outputStream << "layout index: " << _programStatic[n].symbols[i].layoutIndex << endl;
        _outputStream << "matrix stride: " << _programStatic[n].symbols[i].matrixStride << endl;

        _outputStream << "has top level array:" << _programStatic[n].symbols[i].hasTopLevelArray
                      << "      isUnsized: " << _programStatic[n].symbols[i].isUnsizedTopLevelArray << endl;

        if (_programStatic[n].symbols[i].mangledName)
        {
            _outputStream << "mangled name: " << _programStatic[n].symbols[i].mangledName << endl;
        }

        if (_programStatic[n].symbols[i].arrayInfoSize)
        {
            _outputStream << "arrayInfoSize: " << _programStatic[n].symbols[i].arrayInfoSize
                          << "     arrayDim: " << _programStatic[n].symbols[i].arrayDim << endl;

            for (int arrayIndex = 0; arrayIndex < _programStatic[n].symbols[i].arrayInfoSize; arrayIndex++)
            {
                /*
                _outputStream << "    arraySize: " << _programStatic[n].symbols[i].arrayInfo[arrayIndex].arraySize
                              << "    minIndex: " << _programStatic[n].symbols[i].arrayInfo[arrayIndex].arrayMinIndex
                              << "    stride: " << _programStatic[n].symbols[i].arrayInfo[arrayIndex].offsetStride
                              << "    member stride: " << _programStatic[n].symbols[i].arrayInfo[arrayIndex].memberStride << endl;
                              */
            }
        }

        _outputStream << endl;
    }
}


void ELFDumper::OutputShaderInputs(uint32 n)
{
    string title = "*               Shader Input               *";
    printSectionTitle(title, title.length(), _outputStream);
    _outputStream << "Inputs      " << "Length = " << _programStatic[n].numInputs << endl;
    _outputStream << setw(20) << setiosflags(ios::left) << "virtual address" << setw(20) << setiosflags(ios::left) << "physical address"
                  << setw(10) << setiosflags(ios::left) << "isFloat16" << endl;

    for (uint32 i = 0; i < _programStatic[n].numInputs; i++)
    {
        _outputStream << setw(20) << setiosflags(ios::left) << _programStatic[n].inputs[i].vofs << setw(20) << setiosflags(ios::left) << _programStatic[n].inputs[i].pofs
                      << setw(10) << setiosflags(ios::left) << _programStatic[n].inputs[i].isFloat16 << endl;
    }

    _outputStream << endl;
}

const char* getType(int index)
{
    if (index < 0 || index > (int)ATIELF_OUTPUT_STREAMID)
    {
        return "invalid";
    }

    const char* type[14] =
    {
        "gllELF_OUTPUT_POS",
        "gllELF_OUTPUT_POINTSIZE",
        "gllELF_OUTPUT_COLOR",
        "gllELF_OUTPUT_SECONDARYCOLOR",
        "gllELF_OUTPUT_GENERIC",
        "gllELF_OUTPUT_DEPTH",
        "gllELF_OUTPUT_CLIPDISTANCE",
        "gllELF_OUTPUT_PRIMITIVEID",
        "gllELF_OUTPUT_LAYER",
        "gllELF_OUTPUT_VIEWPORTINDEX",
        "gllELF_OUTPUT_STENCIL",
        "gllELF_OUTPUT_STENCIL_VALUE",
        "gllELF_OUTPUT_SAMPLEMASK",
        "gllELF_OUTPUT_STREAMID"
    };
    return type[index];
}

void ELFDumper::OutputShaderOutputs(uint32 n)
{
    string title = "*               Shader Output               *";
    printSectionTitle(title, title.length(), _outputStream);
    _outputStream << "Outputs      " << "Length = " << _programStatic[n].numVoutputs << endl;
    _outputStream
            << setw(20) << setiosflags(ios::left) << "name"
            << setw(20) << setiosflags(ios::left) << "data_type"
            << setw(20) << setiosflags(ios::left) << "size"
            << setw(30) << setiosflags(ios::left) << "type"
            << setw(20) << setiosflags(ios::left) << "pofs"
            << setw(20) << setiosflags(ios::left) << "array_size" << endl;

    for (uint32 i = 0; i < _programStatic[n].numVoutputs; i++)
    {
        if (_programStatic[n].voutputs[i].name)
        {
            _outputStream << setw(20) << setiosflags(ios::left) << _programStatic[n].voutputs[i].name;
        }
        else
        {
            _outputStream << setw(20) << setiosflags(ios::left) << "null";
        }

        _outputStream
                << setw(20) << setiosflags(ios::left) << _programStatic[n].voutputs[i].data_type
                << setw(20) << setiosflags(ios::left) << 13
                << setw(30) << setiosflags(ios::left) << getType(_programStatic[n].voutputs[i].type)
                << setw(20) << setiosflags(ios::left) << _programStatic[n].voutputs[i].pofs
                << setw(20) << setiosflags(ios::left) << _programStatic[n].voutputs[i].array_size << endl;
    }

    _outputStream << endl;
}

int ELFDumper::GetIsaBinarySize()
{
    return _iIsaBinarySize;
}

void ELFDumper::GetIsaBinary(char* ISABinary)
{
    memcpy(ISABinary, _ISABinary, _iIsaBinarySize);
}

int ELFDumper::GetIlBinarySize()
{
    return _iIlBinarySize;
}

void ELFDumper::GetILBinary(char* ILBinary)
{
    memcpy(ILBinary, _ILBinary, _iIlBinarySize);
}

void ELFDumper::GetStatistics(std::string& sStatitics)
{
    sStatitics = _sStatitics;
}

void ELFDumper::GetStatistics(AtiElf_SC_SI_HWSHADER_BASE*& pStatistics)
{
    pStatistics = _pStatistics;
}

