//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file elf_decoder.cpp 
/// 
//=====================================================================

#include "elf_decoder.h"
#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <assert.h>
#include <AMDTBaseTools/Include/gtDefinitions.h>
#pragma warning( disable : 4996 )



ELFProgram::ELFProgram()
{
    // inputs
    numInputs = 0;
    inputs = NULL;

    // outputs
    numVoutputs = 0;
    voutputs = NULL;

    // symbols
    numSymbols = 0;
    symbols = NULL;

    // int constants
    numIntConstants = 0;
    intConstants = NULL;

    // constants
    numConstants = 0;
    constants = NULL;

    // IL
    ilStreamSizeInBytes = 0;
    ilByteStream = NULL;

    // binary
    len = 0;
    data = NULL;
    //info
    progInfo = NULL;
    progUsageInfo = NULL;
}

ELFProgram::~ELFProgram()
{
    Destroy();
    // make sure memory is correctly released
    symbols = NULL;
    voutputs = NULL;
    inputs = NULL;
    ilByteStream = NULL;
    constants = NULL;
    intConstants = NULL;
    data = NULL;
    progInfo = NULL;
    progUsageInfo = NULL;
}


void ELFProgram::Destroy()
{
    // inputs
    if (inputs)
    {
        delete[] inputs;
        inputs = NULL;
    }

    numInputs = 0;

    // outputs
    DeleteVertexOutput();

    // symbols
    DeleteSymbols();

    // constants
    DeleteConstants();

    // IL
    if (ilByteStream)
    {
        delete[] ilByteStream;
        ilByteStream = NULL;
    }

    ilStreamSizeInBytes = 0;

    // binary
    if (data)
    {
        delete[](char*)data;
        data = NULL;
    }

    len = 0;

    // info
    delete progInfo;
    delete progUsageInfo;
}

//
// Delete vertex ouputs
//
void
ELFProgram::DeleteVertexOutput()
{
    if (voutputs)
    {
        for (uint32 i = 0; i < numVoutputs; i++)
        {
            if (voutputs[i].name)
            {
                delete[](voutputs[i].name);
                voutputs[i].name = NULL;
            }
        }

        delete[](voutputs);
        voutputs = NULL;
        numVoutputs = 0;
    }
}

//
// Delete all symbol info
//
void
ELFProgram::DeleteSymbols()
{
    if (symbols != NULL)
    {
        for (uint32 i = 0; i < numSymbols; i++)
        {
            delete[] symbols[i].name;
            delete[] symbols[i].baseName;
            delete[] symbols[i].uniformBlockName;
            delete[] symbols[i].mangledName;
            symbols[i].name = NULL;
            symbols[i].baseName = NULL;
            symbols[i].uniformBlockName = NULL;
            symbols[i].mangledName = NULL;
        }

        delete[] symbols;
        symbols = NULL;
    }

    numSymbols = 0;
}

//
// Delete all constants
//
void
ELFProgram::DeleteConstants()
{
    if (constants != NULL)
    {
        delete[] constants;
        constants = NULL;
    }

    numConstants = 0;

    if (intConstants != NULL)
    {
        delete[] intConstants;
        intConstants = NULL;
    }

    numIntConstants = 0;
}

ELFPackageDecoder::ELFPackageDecoder(const void* data, uint32 length) : reader(data)
{
    GT_UNREFERENCED_PARAMETER(data);
    GT_UNREFERENCED_PARAMETER(length);
}

bool
ELFPackageDecoder::ReadHeader(Elf32_Ehdr&   header)
{
    reader.read(&header.e_ident[0], EI_NIDENT);
    reader.read(header.e_type);
    reader.read(header.e_machine);
    reader.read(header.e_version);
    reader.read(header.e_entry);
    reader.read(header.e_phoff);
    reader.read(header.e_shoff);
    reader.read(header.e_flags);
    reader.read(header.e_ehsize);
    reader.read(header.e_phentsize);
    reader.read(header.e_phnum);
    reader.read(header.e_shentsize);
    reader.read(header.e_shnum);
    reader.read(header.e_shstrndx);

    //
    //  Verify the ELF header
    //
    if ((header.e_ident[EI_MAG0] != ELFMAG0) ||
        (header.e_ident[EI_MAG1] != ELFMAG1) ||
        (header.e_ident[EI_MAG2] != ELFMAG2) ||
        (header.e_ident[EI_MAG3] != ELFMAG3))
    {
        //
        //  Header doesn't contain an ELF identifier
        //
        return false;
    }

    //
    //  Verify the Elf Class
    //
    if (header.e_ident[EI_CLASS] != ELFCLASS32)
    {
        return false;
    }

    //
    //  Verify the byte ordering
    //
    if (header.e_ident[EI_DATA] != ELFDATA2LSB)
    {
        return false;
    }

    //
    //  Verify the header version
    //
    if (header.e_ident[EI_VERSION] != EV_CURRENT)
    {
        return false;
    }

    //
    //  Verify the ABI info
    //
    if (header.e_ident[EI_OSABI] != 99)
    {
        return false;
    }

    //
    //  Verify the type
    //
    if (header.e_type != ET_EXEC)
    {
        return false;
    }



    //
    //  Verify the processor flags
    //
    if ((header.e_flags < 0) ||
        (header.e_flags > 5))
    {
        return false;
    }

    return true;
}

bool
ELFPackageDecoder::ReadProgramHeader(Elf32_Phdr&   header)
{
    reader.read(header.p_type);
    reader.read(header.p_offset);
    reader.read(header.p_vaddr);
    reader.read(header.p_paddr);
    reader.read(header.p_filesz);
    reader.read(header.p_memsz);
    reader.read(header.p_flags);
    reader.read(header.p_align);

    return true;
}

uint32 ELFPackageDecoder::GetSectionIndex(uint32 sectionNames, const SectionList& sections, const char* name)
{
    uint32 index = 0;
    const char* stringTable = (const char*)sections[sectionNames].data;

    for (uint32 i = 0; i < sections.size(); i++)
    {
        const char* sname = &stringTable[sections[i].header.sh_name];

        if (strcmp(sname, name) == 0)
        {
            index = i;
            break;
        }
    }

    return index;
}


void ELFPackageDecoder::ReadSymData(uint32 symindex, uint32 symtabIndex, uint32 strtabIndex, const SectionList& sections, char*& symname, void*& symdata, uint32& symlength)
{
    if (symindex >= sections[symtabIndex].header.sh_size / sizeof(Elf32_Sym))
    {
        return;
    }

    const Elf32_Sym* p = (const Elf32_Sym*)(sections[symtabIndex].data);
    symlength = p[symindex].st_size;
    symname = (char*)sections[strtabIndex].data + p[symindex].st_name;
    symdata = (void*)((char*)sections[p[symindex].st_shndx].data + p[symindex].st_value);
}

ProgramDecoder::ProgramDecoder(const void* data, uint32 length) : ELFPackageDecoder(data, length)
{
    //_shHandle=hSHState;
    _asicNum = 0;
}

void ELFPackageDecoder::ReadSectionHeaderTableEntry(SectionList&  sections)
{
    Section section;

    reader.read(section.header.sh_name);
    reader.read(section.header.sh_type);
    reader.read(section.header.sh_flags);
    reader.read(section.header.sh_addr);
    reader.read(section.header.sh_offset);
    reader.read(section.header.sh_size);
    reader.read(section.header.sh_link);
    reader.read(section.header.sh_info);
    reader.read(section.header.sh_addralign);
    reader.read(section.header.sh_entsize);

    section.data = reader.getoffsetAddress(section.header.sh_offset);

    sections.push_back(section);
}

void ProgramDecoder::DecodeSettings(const Section& section,  uint32& asicID,  std::string& driverVersion)
{
    // if the two variables are already got, please ignore the process
    if (driverVersion.size() > 0)
    {
        return;
    }

    uint32 asicSize;
    uint32 len = section.header.sh_size;
    const char* p = (const char*)section.data;
    memcpy((char*)&asicSize, p, sizeof(uint32));
    p += sizeof(uint32);

    uint32* asicList = new uint32[asicSize];
    memcpy((char*)asicList, p, sizeof(uint32) * asicSize);
    p += sizeof(uint32) * asicSize;
    /*find the most suitable asicID*/
    uint32 asic_final = asicList[0];

    for (uint32 i = 1; i < asicSize; i++)
    {
        if (abs((int32)asicList[i] - (int32)asicID) < abs((int32)asic_final - (int32)asicID))
        {
            asic_final = asicList[i];
            _asicNum = i;
        }
    }

    asicID = asic_final;

    delete[] asicList;
    uint32 strLen = len - sizeof(uint32) * (1 + asicSize) + 1;

    char* str = new char[strLen];
    memcpy(str, p, (strLen - 1));
    str[strLen - 1] = '\0';
    driverVersion.append(str);
    delete[] str;
}


bool ProgramDecoder::DecodeSections(const SectionList&  sections,  uint32  sectionNames,  ProgramBinary* prog)
{
    //const char* stringTable = (const char*)sections[sectionNames].data;
    bool success = true;

    for (uint32 i = 0; i < sections.size(); i++)
    {
        //
        //  Skip the .shstrndx (section header string table)
        //
        if (i == sectionNames)
        {
            continue;
        }

        //const char* name = &stringTable[sections[i].header.sh_name];

        switch (sections[i].header.sh_type)
        {
            case SHT_LOUSER + ATIELF_SETTING_SECTION_OFFSET:
                DecodeSettings(sections[i], prog[0].asicID, prog[0].sourceVersion);
                break;

            case SHT_LOUSER + ATIELF_PARAMETER_SECTION_OFFSET:
                DecodeParameter(sections[i], prog[0].parameter);
                break;

            case SHT_LOUSER + ATIELF_CHECKSUM_SECTION_OFFSET:
            case SHT_PROGBITS:
                break;

            case SHT_STRTAB:
            case SHT_SYMTAB:
            case SHT_NULL:
                /*.symtab .strtab .comment info*/
                break;

            case SHT_LOUSER + ATIELF_SOURCESTRING_SECTION_OFFSET:
                DecodeSourceString(sections, sectionNames, prog);
                break;

            case SHT_LOUSER + ATIELF_INTERNAL_SECTION_OFFSET:
                /*.internal sections, contain serval stage elf packages*/
                success = DecodeInternal(sections, sectionNames, prog);
                break;

            default:
                //assert(!"Unknown/unexpected section type!");
                break;
        }
    }

    return success;
}



bool
ProgramDecoder::ExtractProgramBinary(ProgramBinary* prog)
{
    Elf32_Ehdr header;
    Elf32_Phdr prgheader;

    if (!ReadHeader(header))
    {
        return false;
    }

    reader.seek(header.e_phoff);

    if (!ReadProgramHeader(prgheader))
    {
        return false;
    }

    //
    //  Read the section headers
    //
    SectionList sections;
    reader.seek(header.e_shoff);

    for (uint32 i = 0; i < header.e_shnum; i++)
    {
        ReadSectionHeaderTableEntry(sections);
    }

    return DecodeSections(sections, header.e_shstrndx, prog);
}



void ProgramDecoder::DecodeParameter(const Section& section, ProgramParameters*&  parameter)
{
    //uint32 len = section.header.sh_size;
    parameter = new ProgramParameters;
    packProgramParameter(section.data, *parameter);
}

void ProgramDecoder::packProgramParameter(const void* src, ProgramParameters& dst)
{
    uint32 value, attMapLen, fragMapLen;
    const char* p = (const char*)src;
    //read counter for complexs parameters
    memcpy((char*)&attMapLen, p, sizeof(uint32));
    p += sizeof(uint32);

    memcpy((char*)&fragMapLen, p, sizeof(uint32));
    p += sizeof(uint32);

    memcpy((char*)&value, p, sizeof(dst.tfVaryingCount));
    p += sizeof(dst.tfVaryingCount);
    dst.tfVaryingCount = 0;

    //dst.tfVaryingCount = value;
    //read each members
    for (uint32 i = 0; i < attMapLen; i++)
    {
        std::string str;
        str.append(p);
        p += (str.size() + 1);

        memcpy((char*)&value, p, sizeof(uint32));
        p += sizeof(uint32);
        dst.bindAttribLocationMap[str] = value;
    }

    for (uint32 i = 0; i < fragMapLen; i++)
    {
        std::string str;
        str.append(p);
        p += (str.size() + 1);

        memcpy((char*)&value, p, sizeof(uint32));
        p += sizeof(uint32);
        dst.bindFragDataLocationMap[str] = value;
    }

    for (uint32 i = 0; i < dst.tfVaryingCount; i++)
    {
        std::string str;
        str.append(p);
        p += (str.size() + 1);

        dst.tfVaryings[i].name = new char[(str.size() + 1)];
        memcpy(dst.tfVaryings[i].name, str.c_str(), str.size());
        dst.tfVaryings[i].name[str.size()] = '\0';

        memcpy(&dst.tfVaryings[i].u32All, p, sizeof(dst.tfVaryings[i].u32All));
        p += sizeof(dst.tfVaryings[i].u32All);
    }

    //read all simple parameters
    uint32 tfBufferModeOffset = (uint32)((const unsigned char*)&dst.tfBufferMode - (const unsigned char*)&dst);
    memcpy((char*)&dst.tfBufferMode, p, sizeof(ProgramParameters) - tfBufferModeOffset);
    p += (sizeof(ProgramParameters) - tfBufferModeOffset);
}


void ProgramDecoder::DecodeSourceString(const SectionList& sections, uint32 sectionNames, ProgramBinary* prog)
{
    uint32 strName = GetSectionIndex(sectionNames, sections, ".strtab");
    uint32 symName = GetSectionIndex(sectionNames, sections, ".symtab");
    char*  sourceName = NULL;
    const char shaderStage[] = {'V', 'H', 'D', 'G', 'F', 'C'};
    uint32 count = 1;
    uint32 shaderlength;
    char str[30];
    void* shaderdata = NULL;

    /*__Shader_%c_Source_%d_*/
    for (count = 1; count < sections[symName].header.sh_size / sizeof(Elf32_Sym); count++)
    {
        ReadSymData(count, symName, strName, sections, sourceName, shaderdata, shaderlength);

        for (uint32 shaderName = 0; shaderName < 6; shaderName++)
        {
            sprintf(str, "__Shader_%c_Source_", shaderStage[shaderName]);

            if (strncmp(sourceName, str, strlen(str)) == 0)
            {

                char* strInner = new char[shaderlength + 1];
                memcpy(strInner, shaderdata, shaderlength);
                strInner[shaderlength] = '\0';
                std::string ss(strInner);
                prog[shaderName].sourceString.push_back(ss);
                delete[] strInner;
                break;
            }
        }
    }

}



/*Decode key and binary pair*/
bool ProgramDecoder::DecodeInternal(const SectionList& sections, uint32 sectionNames, ProgramBinary* prog)
{
    uint32 strName = GetSectionIndex(sectionNames, sections, ".strtab");
    uint32 symName = GetSectionIndex(sectionNames, sections, ".symtab");
    //uint32 crcName = GetSectionIndex(sectionNames, sections, ".crc");
    //uint32* crc = (uint32*)sections[crcName].data;
    char*  symbolName = NULL;
    const char shaderStage[] = {'V', 'H', 'D', 'G', 'F', 'C'};
    uint32 shaderCount[7];
    //GLLMemSet(shaderCount, 0, cmShaderStage_COUNT*sizeof(uint32));
    memset(shaderCount, 0, 7 * sizeof(uint32));
    uint32 count = 1;
    uint32 shaderlength;
    void* shaderdata = NULL;
    void* shaderBinaryInfo = NULL;
    uint32 shaderBinaryInfoLength = 0;
    void* shaderElfBinary = NULL;
    uint32 shaderElfBinaryLength = 0;

    char str[100];
    uint32 shaderName;
    bool success = true;

    /*__Shader_%c_key_%d_*/
    for (count = 1; count < sections[symName].header.sh_size / sizeof(Elf32_Sym); count++)
    {
        ReadSymData(count, symName, strName, sections, symbolName, shaderdata, shaderlength);

        for (shaderName = 0; shaderName < 6; shaderName++)
        {
            sprintf(str, "__Shader_%c_AsicID_%u_Key_", shaderStage[shaderName], prog[0].asicID);

            if (strncmp(symbolName, str, strlen(str)) == 0)
            {

                uint32* key = new uint32[shaderlength / sizeof(uint32)];
                memcpy((char*)key, shaderdata, shaderlength);
                prog[shaderName].key.push_back(key);
                break;
            }

            sprintf(str, "__Shader_%c_AsicID_%u_BinaryInfo_", shaderStage[shaderName], prog[0].asicID);

            if (strncmp(symbolName, str, strlen(str)) == 0)
            {
                /*BinaryInfo and ElfBinary would come in pairs, otherwise something would be wrong!*/
                if (shaderBinaryInfoLength)
                {
                    success = false;
                    return success;
                }

                shaderBinaryInfo = shaderdata;
                shaderBinaryInfoLength = shaderlength;
            }

            sprintf(str, "__Shader_%c_AsicID_%u_ElfBinary_", shaderStage[shaderName], prog[0].asicID);

            if (strncmp(symbolName, str, strlen(str)) == 0)
            {
                /*BinaryInfo and ElfBinary would come in pairs, otherwise something would be wrong!*/
                if (shaderElfBinaryLength)
                {
                    success = false;
                    return success;
                }

                shaderElfBinary = shaderdata;
                shaderElfBinaryLength = shaderlength;
            }

            sprintf(str, "__Shader_%c_Stage_AsicID_%u_", shaderStage[shaderName], prog[0].asicID);

            if (strncmp(symbolName, str, strlen(str)) == 0)
            {
                /* uint32 crcValue = crc32((const uint8 *)shaderdata, shaderlength);
                 if((crcValue != crc[shaderName + _asicNum * cmShaderStage_COUNT]) && (crc[shaderName + _asicNum * cmShaderStage_COUNT] != NOCHKSUM))
                 {
                     success = false;
                     return success;
                 }
                 break;*/
            }

            /*if both BinaryInfo and Binary have been ready, begin to decode to scl program*/
            if (shaderBinaryInfoLength && shaderElfBinaryLength)
            {
                sclProgram* sclProg = new sclProgram;
                sclProg = CreateSCLProgramObject(shaderBinaryInfo, shaderBinaryInfoLength, shaderElfBinary, shaderElfBinaryLength);
                prog[shaderName].sclProg.push_back(sclProg);
                shaderBinaryInfo = NULL;
                shaderBinaryInfoLength = 0;
                shaderElfBinary = NULL;
                shaderElfBinaryLength = 0;
                break;
            }

        }
    }

    return success;
}


sclProgram* ProgramDecoder::CreateSCLProgramObject(const void* unpackedProgramInfoHandle,
                                                   uint32 unpackedProgramInfoLength,
                                                   const void* unpackedProgramHandle,
                                                   uint32 unpackedProgramLength)
{
    GT_UNREFERENCED_PARAMETER(unpackedProgramInfoLength);
    sclProgram* program = new sclProgram;

    const uint32* p = (const uint32*)unpackedProgramInfoHandle;

    program->errorCode = (sclErrorCode) * p++;
    program->errorPos = *p++;
    program->language = (sclShaderLanguage) * p++;

    uint32 errorLen;
    errorLen = *p++;

    char* tmp = new char[errorLen + 1];
    const char* pc = (const char*)p;

    memcpy(tmp, pc, errorLen);
    tmp[errorLen] = '\0';
    program->errorString.append(tmp);


    program->len = unpackedProgramLength;

    program->data = (const void*)new char[program->len];
    pc = (const char*)unpackedProgramHandle;

    memcpy((char*)program->data, pc, program->len);

    delete[] tmp;

    return program;
}

void
ProgramDecoder::Seek(uint32 pos)
{
    reader.seek(pos);
}



ShaderDecoder::ShaderDecoder(sclShaderLanguage lang, const void* data, uint32 length) : ELFPackageDecoder(data, length)
{
    _language = lang;
}

bool
ShaderDecoder::ExtractShaderBinary(ELFProgram* pProgStatic, const sclProgram* sclProg)
{
    Elf32_Ehdr header;
    Elf32_Phdr prgheader;

    if (!ReadHeader(header))
    {
        return false;
    }

    reader.seek(header.e_phoff);

    if (!ReadProgramHeader(prgheader))
    {
        return false;
    }

    _language = sclProg->language;
    //
    //  Read the section headers
    //
    SectionList sections;
    reader.seek(header.e_shoff);

    for (uint32 i = 0; i < header.e_shnum; i++)
    {
        ReadSectionHeaderTableEntry(sections);
        pProgStatic->section_names.push_back(std::string());
    }

    DecodeSections(sections, header.e_shstrndx, pProgStatic);

    return true;
}


void ShaderDecoder::DecodeSections(const SectionList& sections, uint32 sectionNames, ELFProgram* pProgStatic)
{
    const char* stringTable = (const char*)sections[sectionNames].data;

    //bool remapInputVof = false;
    for (uint32 i = 0; i < sections.size(); i++)
    {
        //
        //  Skip the .shstrndx (section header string table)
        //
        if (i == sectionNames)
        {
            continue;
        }

        const char* name = &stringTable[sections[i].header.sh_name];
        pProgStatic->section_names[i] =  name;

        switch (sections[i].header.sh_type)
        {
            case SHT_LOUSER + ATIELF_INPUT_SECTION_OFFSET:
                DecodeInputs(sections[i], name, pProgStatic);
                break;

            case SHT_LOUSER + ATIELF_OUTPUT_SECTION_OFFSET:
                DecodeOutputs(sections[i], name, pProgStatic);
                break;

            case SHT_LOUSER + ATIELF_CONSTANT_SECTION_OFFSET:
                DecodeConstants(sections[i], name, pProgStatic);
                break;

            case SHT_LOUSER + ATIELF_PROGRAMINFO_SECTION_OFFSET:
                DecodeProgramInfo(sections[i], name, pProgStatic);
                break;

            case SHT_LOUSER + ATIELF_PROGUSAGEINFO_SECTION_OFFSET:
                DecodeProgramUsageInfo(sections[i], name, pProgStatic);
                break;

            case SHT_LOUSER + ATIELF_IL_SECTION_OFFSET:
                DecodeIL(sections[i], name, pProgStatic);
                break;

            case SHT_LOUSER + ATIELF_SYMBOL_SECTION_OFFSET:
                DecodeSymbols(sections[i], name, pProgStatic);
                break;

            case SHT_LOUSER + ATIELF_SCALAR_CONSTANT_SECTION_OFFSET:
                break;

            case SHT_LOUSER + ATIELF_LOOPCONSTANT_SECTION_OFFSET:
                DecodeLoopConstants(sections[i], name, pProgStatic);
                break;

            //for softvap
            case SHT_LOUSER + ATIELF_S2H_INPUT_MAP_SECTION_OFFSET:
            case SHT_LOUSER + ATIELF_SOFTVAPIL_SECTION_OFFSET:
            case SHT_LOUSER + ATIELF_PATCHED_IL_SECTION_OFFSET:
            case SHT_LOUSER + ATIELF_PASSTHRU_VS_SECTION_OFFSET:
                break;

            case SHT_PROGBITS:
                DecodeText(sections[i], name, pProgStatic);

            case SHT_STRTAB:
            case SHT_SYMTAB:
            case SHT_NULL:
                /*.symtab .strtab .comment and other debug info*/
                break;

            default:
                //GLLAssert(!"Unknown/unexpected section type!");
                break;
        }
    }

}

void ShaderDecoder::DecodeText(const Section& section, const char* name, ELFProgram* pProgStatic)
{
    if (strcmp(name, ".text") == 0)
    {
        pProgStatic->textSize = section.header.sh_size;
        pProgStatic->text = (AtiElf_SC_SI_HWSHADER_BASE*)section.data;
    }
    else
    {
        ELFDebugSection dsection;
        dsection.debugInfoSize = section.header.sh_size;
        dsection.debugInfo = (void*)section.data;
        dsection.debugInfoTitle = name;
        pProgStatic->debuggerInfos.push_back(dsection);
    }


}

void
ShaderDecoder::DecodeInputs(const Section& section, const char* name, ELFProgram* pProgStatic)
{
    GT_UNREFERENCED_PARAMETER(name);
    const uint32 sectionSize = section.header.sh_size;

    //
    //  Ensure the name of the section is correct
    //
    assert(strcmp(name, ".inputs") == 0);

    delete [] pProgStatic->inputs;

    // gllInput only cares about attributes, so we have to remove other inputs
    const uint32 numElfInputs = sectionSize / sizeof(AtiElfInput);

    const AtiElfInput* inp = (const AtiElfInput*)section.data;

    pProgStatic->numInputs = numElfInputs;

    pProgStatic->inputs = new AtiElfInput[numElfInputs];
    //uint32 currentInput = 0;

    for (uint32 i = 0; i < numElfInputs; i++)
    {
        pProgStatic->inputs[i] = inp[i];
    }

}

void
ShaderDecoder::DecodeOutputs(const Section& section, const char* name, ELFProgram* pProgStatic)
{
    GT_UNREFERENCED_PARAMETER(name);
    const uint32 outputCount  = section.header.sh_entsize;
    const char* binPtr = (const char*) section.data;
    const uint32 name_offset = sizeof(AtiElfOutput) - sizeof(char*);

    //
    //  Ensure the name of the section is correct
    //
    assert(strcmp(name, ".outputs") == 0);

    pProgStatic->voutputs = new AtiElfOutput[outputCount];
    pProgStatic->numVoutputs = outputCount;

    for (uint32 i = 0; i < outputCount; i++)
    {
        const AtiElfOutput* outp = (const AtiElfOutput*)binPtr;
        pProgStatic->voutputs[i] = *outp;
        const char* outname = &binPtr[name_offset];
        size_t len = strlen(outname) + 1;
        char* nameInner = new char[len];
        memcpy(nameInner, outname, len);
        pProgStatic->voutputs[i].name = nameInner;
        binPtr += outp->structSize;
    }
}

void
ShaderDecoder::DecodeConstants(const Section& section, const char* name, ELFProgram* pProgStatic)
{
    GT_UNREFERENCED_PARAMETER(name);
    const uint32 sectionSize = section.header.sh_size;
    const uint32 constCount  = sectionSize / sizeof(AtiElfConstant);

    //
    //  Ensure the name of the section is correct
    //
    assert(strcmp(name, ".constants") == 0);

    const AtiElfConstant* cp = (const AtiElfConstant*)section.data;

    pProgStatic->constants = new AtiElfConstant[constCount];

    for (uint32 i = 0/*, cll = 0, clc = 0, cgl = 0*/; i < constCount; i++)
    {
        pProgStatic->constants[i] = cp[i];
    }
}

void
ShaderDecoder::DecodeLoopConstants(const Section& section, const char* name, ELFProgram* pProgStatic)
{
    (void)name;
    const uint32 sectionSize = section.header.sh_size;
    const uint32 loopConstCount  = sectionSize / sizeof(AtiElfIntegerConstant);

    pProgStatic->intConstants = new AtiElfIntegerConstant[loopConstCount];

    const AtiElfIntegerConstant* cp = (const AtiElfIntegerConstant*)section.data;

    pProgStatic->numIntConstants = loopConstCount;

    for (uint32 i = 0; i < loopConstCount; i++)
    {
        pProgStatic->intConstants[i] = *cp;
    }

    //pProgStatic->usageInfo._maxIntPhysicalConstant = maxPhysicalConstant;
}

void
ShaderDecoder::DecodeIL(const Section& section, const char* name, ELFProgram* pProgStatic)
{
    (void)name;
    unsigned char* binPtr = (unsigned char*) section.data;
    int streamSize;

    streamSize = (binPtr[0] << 0) |
                 (binPtr[1] << 8) |
                 (binPtr[2] << 16) |
                 (binPtr[3] << 24);

    pProgStatic->ilByteStream = new char[streamSize];
    pProgStatic->ilStreamSizeInBytes = streamSize;

    memcpy(pProgStatic->ilByteStream, binPtr, streamSize);
}



void
ShaderDecoder::DecodeProgramInfo(const Section& section, const char* name, ELFProgram* pProgStatic)
{
    GT_UNREFERENCED_PARAMETER(name);
    AtiElfProgramInfo* p = (AtiElfProgramInfo*)(section.data);
    pProgStatic->progInfo = new AtiElfProgramInfo();
    *pProgStatic->progInfo = *p;
}

void
ShaderDecoder::DecodeProgramUsageInfo(const Section& section, const char* name, ELFProgram* pProgStatic)
{
    GT_UNREFERENCED_PARAMETER(name);
    GT_UNREFERENCED_PARAMETER(section);
    GT_UNREFERENCED_PARAMETER(pProgStatic);
}

void
ShaderDecoder::DecodeSymbols(const Section& section, const char* name, ELFProgram* pProgStatic)
{
    GT_UNREFERENCED_PARAMETER(name);
    uint32 sectionSize = section.header.sh_size;
    uint32 symCount = sectionSize / sizeof(AtiElfSymbol);
    unsigned char* binPtr = (unsigned char*) section.data;

    symCount = (binPtr[0] << 0) |
               (binPtr[1] << 8) |
               (binPtr[2] << 16) |
               (binPtr[3] << 24);

    binPtr += 4;

    pProgStatic->numSymbols = symCount;
    pProgStatic->symbols = new AtiElfSymbol[symCount];
    AtiElfSymbol* symPtr = pProgStatic->symbols;

    for (uint32 i = 0; i < symCount; i++)
    {
        AtiElfSymbol*  elfSymbolPtr = (AtiElfSymbol*) binPtr;
        unsigned char* symName;
        size_t         nameLen;
        unsigned char* symBaseName;
        size_t         baseNameLen;
        unsigned char* uniformBlockName;
        size_t         ubNameLen;
        unsigned char* mangledName;
        size_t         mangledNameLen;
        int32          binPtrIndex = 0;

        binPtrIndex = (int32)((const char*)(&elfSymbolPtr->name) - (const char*)elfSymbolPtr);
        symName = &binPtr[binPtrIndex];
        nameLen = strlen((char*)symName) + 1;
        binPtrIndex += (int32)nameLen;
        symBaseName = &binPtr[binPtrIndex];
        baseNameLen = strlen((char*)symBaseName) + 1;
        binPtrIndex += (int32)baseNameLen;

        // uniform block name
        uniformBlockName = &binPtr[binPtrIndex];
        ubNameLen = strlen((char*)uniformBlockName) + 1;
        binPtrIndex += (int32)ubNameLen;
        mangledName = &binPtr[binPtrIndex];
        mangledNameLen = strlen((char*)mangledName) + 1;
        binPtrIndex += (int32)mangledNameLen;
        binPtr += elfSymbolPtr->structSize;

        //copy other members directly
        symPtr[i] = *elfSymbolPtr;
        //copy other members directly
        symPtr[i].name     = new char[nameLen];
        strcpy((char*)symPtr[i].name, (char*)symName);
        symPtr[i].baseName = new char[baseNameLen];
        strcpy((char*)symPtr[i].baseName, (char*)symBaseName);
        symPtr[i].uniformBlockName = new char[ubNameLen];
        strcpy((char*)symPtr[i].uniformBlockName, (char*)uniformBlockName);
        symPtr[i].mangledName = new char[mangledNameLen];
        strcpy((char*)symPtr[i].mangledName, (char*)mangledName);
    }

    symPtr = NULL;
}
