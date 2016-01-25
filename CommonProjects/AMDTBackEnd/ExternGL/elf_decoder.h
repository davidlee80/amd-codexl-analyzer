//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file elf_decoder.h 
/// 
//=====================================================================
#ifndef _ELF_DECODER_H
#define _ELF_DECODER_H

#include <vector>
#include <map>
#include "elf.h"
#ifdef _WIN32
    #include "elf_amdtypes.h"
#endif // _WIN32
#include "scl_types.h"
#include <string>
#include <string.h>
#include "AtiElfSI.h"

//
/// Transform Feedback Buffer mode type
//

struct gllVertexOutput
{
    char*   name;                        ///< name of the vertex output
    //
    /// all fields of vertex output
    //
    union
    {
        //
        /// all fields of vertex output
        //
        struct
        {
            AtiElfSymbolDataType  data_type : 8;   ///< data type
            int32                 size : 6;        ///< amount of components
            AtiElfOutputType      type : 5;        ///< semantic type
            uint32                pofs : 5;        ///< the pofs info
            uint32                array_size : 8;  ///< array size
        } VertexOutputStruct;
        uint32 u32All;                             ///< all fields for vertex output
    };

};

typedef enum
{
    GLL_TF_BUFFER_INTERLEAVED_ATTRIBS,
    GLL_TF_BUFFER_SEPARATE_ATTRIBS,
} gllTransformFeedbackBufferMode;


struct ProgramParameters
{
    ProgramParameters()
    {
        memset(tfVaryings, 0, sizeof(tfVaryings));
        tfVaryingCount = 0;
        tfBufferMode = GLL_TF_BUFFER_INTERLEAVED_ATTRIBS;
    }

    // destructor
    ~ProgramParameters()
    {
        bindAttribLocationMap.clear();
        bindFragDataLocationMap.clear();

        for (uint32 i = 0; i < tfVaryingCount; i++)
        {
            if (tfVaryings[i].name)
            {
                delete [] tfVaryings[i].name;
                tfVaryings[i].name = NULL;
                tfVaryings[i].u32All = 0;
            }
        }
    }
private:
    ProgramParameters& operator=(const ProgramParameters& rhs)
    {
        (void)rhs;
        return *this;
    }
public:
    // for location binding
    std::map<std::string, uint32> bindAttribLocationMap;     ///< [in] attribute location mapping
    std::map<std::string, uint32> bindFragDataLocationMap;   ///< [in] frag data location mapping

    // for transform feedback
    enum { MAX_NUM_TRANSFORM_FEEDBACK_VARYINGS = 64 };
    gllVertexOutput     tfVaryings[MAX_NUM_TRANSFORM_FEEDBACK_VARYINGS];                ///< [in] Array of Varyings defined by the app.
    uint32              tfVaryingCount;                                                 ///< [in] number of varyings passed in by the user

    gllTransformFeedbackBufferMode  tfBufferMode;     ///< [in]The buffer mode that is currently set for Transform Feedback.
};

class MemoryReader
{
public:
    MemoryReader(const void* memory)
        : m_base((unsigned char*)memory)
        , m_readptr((unsigned char*)memory)
    {
    }

    void seek(uint32 pos)
    {
        m_readptr = &m_base[pos];
    }

    const void* getoffsetAddress(uint32 pos)
    {
        return &m_base[pos];
    }

    void read(uint32& value)
    {
        unsigned char buffer[4];
        buffer[0] = *m_readptr++;
        buffer[1] = *m_readptr++;
        buffer[2] = *m_readptr++;
        buffer[3] = *m_readptr++;

        value = (((uint32)buffer[0] << 0) |
                 ((uint32)buffer[1] << 8) |
                 ((uint32)buffer[2] << 16) |
                 ((uint32)buffer[3] << 24));
    }

    void read(uint16& value)
    {
        unsigned char buffer[2];
        buffer[0] = *m_readptr++;
        buffer[1] = *m_readptr++;

        value = (((uint32)buffer[0] << 0) |
                 ((uint32)buffer[1] << 8));
    }

    void read(uint8& value)
    {
        value = *m_readptr++;
    }

    void read(void* buffer, uint32 size)
    {
        unsigned char* tmp = (unsigned char*)buffer;

        for (uint32 i = 0; i < size; i++)
        {
            tmp[i] = *m_readptr++;
        }
    }

private:
    const unsigned char* m_base;
    const unsigned char* m_readptr;
};

///< elf section descriptor
struct Section
{
    Elf32_Shdr  header;
    const void* data;
};

typedef std::vector<Section> SectionList;

class ELFPackageDecoder
{
public:
    ELFPackageDecoder(const void* data, uint32 length);  ///< construct
    bool ReadHeader(Elf32_Ehdr& header);                 ///< read ELF header and check the consistancy.
    bool ReadProgramHeader(Elf32_Phdr& header);          ///< read ELF program header
    void ReadSectionHeaderTableEntry(SectionList&  sections); ///< build ELF section list
    void ReadSymData(uint32 symindex, uint32 symtabIndex, uint32 strtabIndex, const SectionList& sections, char*& symname, void*& symdata, uint32& symlength);
    uint32 GetSectionIndex(uint32 sectionNames, const SectionList& sections, const char* name);
protected:
    MemoryReader reader;
};

struct  ProgramBinary
{
    ProgramBinary() { asicID = 0xFFFFFFFF; sourceVersion = ""; parameter = NULL; }
    ~ProgramBinary()
    {
        delete parameter;

        for (size_t i = 0; i < key.size(); i++)
        {
            delete[] key[i];
            key[i] = NULL;
        }
    }
    uint32                 asicID;
    std::string               sourceVersion;
    ProgramParameters*     parameter;
    std::vector<std::string>     sourceString;
    std::vector<sclProgram*>  sclProg;
    std::vector<uint32*>      key;

};



class ProgramDecoder: public ELFPackageDecoder
{
public:
    ProgramDecoder(const void* data, uint32 length);
    void Seek(uint32 pos);
    bool ExtractProgramBinary(ProgramBinary* prog);
private:
    bool DecodeSections(const SectionList&  sections, uint32  sectionNames, ProgramBinary* prog);
    void DecodeSourceString(const SectionList& sections, uint32 sectionNames, ProgramBinary* prog);
    void DecodeSettings(const Section& section, uint32& asicID, std::string& driverVersion);
    void DecodeParameter(const Section& section, ProgramParameters*&  parameter);
    void packProgramParameter(const void* src, ProgramParameters& dst);
    bool DecodeInternal(const SectionList& sections, uint32 sectionNames, ProgramBinary* prog);
    void DecodeShader(const Section& section, const char* name, ProgramBinary* prog);
    sclProgram* CreateSCLProgramObject(const void* unpackedProgramInfoHandle, uint32 unpackedProgramInfoLength, const void* unpackedProgramHandle, uint32 unpackedProgramLength);
    uint32 _asicNum;
};

//
/// Static program information
//
/// Contains information that is based on compiled shader binary.
//
struct ELFDebugSection
{
    void*                   debugInfo;                          ///<Debug data pointer
    uint32                  debugInfoSize;                      ///<Debug data size
    std::string                  debugInfoTitle;                     ///<Debug section name
};



class ELFProgram
{
public:

    // inputs
    uint32                  numInputs;                          ///< number of input streams
    AtiElfInput*               inputs;                          ///< map of input streams to registers, include tess info (sent to EP)
    // vertex outputs
    uint32                  numVoutputs;                        ///< number of vertex outputs
    AtiElfOutput*           voutputs;                           ///< list of vertex outputs this program uses
    // symbols
    uint32                  numSymbols;                         ///< number of symbols
    AtiElfSymbol*            symbols;                           ///< symbols
    // int constants
    uint32                  numIntConstants;                    ///< number of int constants
    AtiElfIntegerConstant*   intConstants;                      ///< int constants
    // constants
    uint32                  numConstants;                       ///< number of literal constants
    AtiElfConstant*          constants;
    // IL
    uint32                  ilStreamSizeInBytes;                ///< IL stream size in bytes
    char*                   ilByteStream;                       ///< IL byte stream
    // Text
    AtiElf_SC_SI_HWSHADER_BASE*                   text;
    uint32                  textSize;

    std::vector<ELFDebugSection>  debuggerInfos;

    // binary
    uintp                   len;                                ///< length in bytes of the program
    const void*             data;                               ///< data of the program (ELF binary)
    // info
    AtiElfProgramInfo*       progInfo;                          ///< program info
    AtiElfProgramUsageInfo*  progUsageInfo;                     ///< program usage info
    std::vector<std::string>      section_names;
public:
    //
    /// Constructor
    //
    ELFProgram();

    //
    /// Destructor
    //
    ~ELFProgram();

public:

    //
    /// Delete vertex ouputs
    //
    void DeleteVertexOutput();

    //
    /// Delete all symbol info
    //
    void DeleteSymbols();

    //
    /// Delete all constants
    //
    void DeleteConstants();

    //
    /// Destroy
    //
    void Destroy();

};


///< decoder for SCL output
class ShaderDecoder : public ELFPackageDecoder
{
public:
    ShaderDecoder(sclShaderLanguage target, const void* data, uint32 length);
    /// Function to extract all info from the ELF binary.
    bool ExtractShaderBinary(ELFProgram* pProgStatic, const sclProgram* sclProg);
private:
    /// call sub function to decode the section list
    void DecodeSections(const SectionList& sections, uint32  sectionNames, ELFProgram* pProgStatic);

    /// Sub-function which decodes ELF inputs information.
    void DecodeInputs(const Section& section, const char* name, ELFProgram* pProgStatic);

    /// Sub-function which decodes ELF map information from svp inputs to hw inputs.
    void DecodeSVPInputs(const Section& section, const char* name, ELFProgram* pProgStatic);

    /// Sub-function which decodes ELF constant information.
    void DecodeConstants(const Section& section, const char* name, ELFProgram* pProgStatic);

    /// Sub-function which decodes ELF literal int constant information.
    void DecodeLoopConstants(const Section& section, const char* name, ELFProgram* pProgStatic);

    /// Sub-function which decodes ELF symbol information.
    void DecodeSymbols(const Section& section, const char* name, ELFProgram* pProgStatic);

    /// Sub-function which gets the IL out of the ELF
    void DecodeIL(const Section& section, const char* name, ELFProgram* pProgStatic);

    /// Sub-function which gets the hardware data out of the ELF
    void DecodeText(const Section& section, const char* name, ELFProgram* pProgStatic);

    /// sub-function which decode ELF program info
    void DecodeProgramInfo(const Section& section, const char* name, ELFProgram* pProgStatic);
    void DecodeProgramUsageInfo(const Section& section, const char* name, ELFProgram* pProgStatic);

    /// sub-function which decode ELF output
    void DecodeOutputs(const Section& section, const char* name, ELFProgram* pProgStatic);

    /// shader language
    sclShaderLanguage _language;
};

#endif