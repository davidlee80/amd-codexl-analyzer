//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file AtiElfSI.h 
/// 
//=====================================================================
#ifndef __AtiElfSI_h__
#define __AtiElfSI_h__




#define AtiElf_SC_SI_VS_MAX_INPUTS       32
#define AtiElf_SC_SI_VS_MAX_OUTPUTS      32
#define AtiElf_SC_SI_PS_MAX_INPUTS       32
#define AtiElf_SC_SI_PS_MAX_OUTPUTS      8
#define AtiElf_SC_SI_NUM_INTBUF          2
#define AtiElf_SC_SI_NUM_USER_ELEMENT    16

// Max PS export patch code size in DWORDs
#define AtiElf_SC_SI_MAX_EXPORT_CODE_SIZE 6
#define AtiElf_SC_SI_NUM_EXPORT_FMT  0xa

// Number of SO streams
#define AtiElf_SC_SI_SO_MAX_STREAMS  4
// Number of SO buffers
#define AtiElf_SC_SI_SO_MAX_BUFFERS  4

// Invalid user data register
#define AtiElf_SC_SI_INVALID_USER_DATA_REG  0xFFFFFFFF

/*
 *  Target HW shader stage for compilation.
 */
/*typedef enum E_SC_HW_SHADER_STAGE
{
    STAGE_HW_LS,
    STAGE_HW_HS,
    STAGE_HW_ES,
    STAGE_HW_GS,
    STAGE_HW_VS,
    STAGE_HW_PS,
    STAGE_HW_CS,
} E_SC_HW_SHADER_STAGE;*/

/*
 *  Flags to guide shader compilation.
 */
struct AtiElf_SC_COMPILE_FLAGS
{
    // To make this structure consistent with SC_COMPILE_FLAGS, we add some reserved bits.
    uint32 psPrimId           : 1; // PS needs primID input
    uint32 useHsOffChip       : 1; // HS can use off-ship LDS
    uint32 clampScratchAccess : 1; // add code to clamp scratch assesses
    uint32 streamOutEnable    : 1; // enable writes to stream-out buffers
    uint32 reserved1          : 4;
    uint32 useGsOnChip        : 1; // ES and GS can use on-chip LDS. (CI+)
    uint32 reserved2          : 23;
} ;

/*
 *  User data layout element descriptor. Defines type (class) of data and
 *  where in user registers it has to be stored.
 *
 *  NOTES:
 *  1) API slot is only valid for immediate resources, samplers and etc.
 *  2) ALU const info is only valid for immediate float consts and describes
 *     the constant number as well as the first channel that contains the data.
 *     The constant channel encoding is X=0, Y=1, Z=2 and W=3. It's illegal
 *     to cross the float4 constant boundary. For example, if the first channel
 *     is Z, then the max value for u32UserRegCount should be 2.
 */
struct AtiElf_SC_SI_USER_DATA_ELEMENT
{
    uint32 dataClass;
    union
    {
        // API slot of the resource descriptor (textures, samplers, CB, VB, etc.)
        uint32     u32ApiSlot;
        // Immediate ALU float const selector
        struct
        {
            uint32 u32AluConstChannel : 2;
            uint32 u32AluConst        : 30;
        } ALUSelectorStruct;
    };
    // First user register where data should be loaded by the driver
    uint32         u32StartUserReg;
    // Number of consecutive user registers that should be loaded
    uint32         u32UserRegCount;
};
/*
 *  Pixel shader input declaration to be used for semantic mapping.
 */
struct AtiElf_SC_SI_PSSHADERDCL_IN
{
    uint32 usage                  : 8;  // semantic usage. IL_IMPORTUSAGE.
    uint32 usageIdx               : 8;  // semantic index. Opaque to SC.
    uint32 inputIdx               : 8;  // PS input index.
    uint32 defaultVal             : 2;  // default val if VS doesn't export.
    uint32 flatShade              : 1;  // set if interp mode is constant.
    uint32 fp16Mask               : 2;  // bit mask for fp16 attribute, bit 0: attr0, bit 1: attr1
    uint32 reserved               : 3;
};

/*
 *  Pixel shader export patch .
 */
struct AtiElf_SC_SI_EXPORT_PATCH_INFO
{
    // Patch offset (in DWORDs) to start of an export sequence
    uint32 patchOffset;
    // Conversion code snippet for each of the export formats
    uint32 patchCode[AtiElf_SC_SI_NUM_EXPORT_FMT][AtiElf_SC_SI_MAX_EXPORT_CODE_SIZE];
};


/*
 *  Vertex shader input declaration to be used for semantic mapping with FS.
 */
struct AtiElf_SC_SI_VSSHADERDCL_IN
{
    uint32 usage     : 8;  // semantic usage. IL_IMPORTUSAGE.
    uint32 usageIdx  : 8;  // semantic index. Opaque to SC.
    uint32 dataVgpr  : 8;  // first VGPR to contain fetch result
    uint32 dataSize  : 2;  // (fetch_size - 1), size in elements
    uint32 reserved  : 6;
};

/*
 *  Vertex shader output declaration to be used for semantic mapping.
 *  The paramIdx is the index of the export parameter SC uses in the shader.
 */
struct AtiElf_SC_SI_VSSHADERDCL_OUT
{
    uint32 usage     : 8;      // semantic usage. IL_IMPORTUSAGE.
    uint32 usageIdx  : 8;      // semantic index. Opaque to SC.
    uint32 paramIdx  : 8;      // attribute export parameter index (0-31)
    uint32 reserved  : 8;
};

#define AtiElf_SC_SI_HWSHADER_COMMON       \
    uint32                 X32XhShaderMemHandle;   \
    uint32                 X32XhConstBufferMemHandle[AtiElf_SC_SI_NUM_INTBUF];\
    uint32                 CodeLenInByte;\
    uint32   u32UserElementCount; /*Number of user data descriptors  */\
    AtiElf_SC_SI_USER_DATA_ELEMENT pUserElement[AtiElf_SC_SI_NUM_USER_ELEMENT]; /*User data descriptors */\
    /* Common HW shader info about registers and execution modes*/ \
    uint32   u32NumVgprs;  \
    uint32   u32NumSgprs;  \
    uint32   u32FloatMode; \
    bool     bIeeeMode;  \
    bool     bUsesPrimId; \
    bool     bUsesVertexId; \
    uint32   scratchSize;/* Scratch size in DWORDs for a single thread*/


struct AtiElf_SC_SI_HWSHADER_BASE
{
    AtiElf_SC_HWSHADER_COMMON;
    AtiElf_SC_SI_HWSHADER_COMMON;
#ifdef TRAP_SUPPORT
    bool     bDebugMode;
    uint32   trapCodeLenInByte;
#endif
};
/*
 *  Pixel shader
 */

struct AtiElf_SC_SIPSHWSHADER : public AtiElf_SC_SI_HWSHADER_BASE
{
    // Input semantics
    uint32 numPsInSemantics;
    AtiElf_SC_SI_PSSHADERDCL_IN psInSemantics[AtiElf_SC_SI_PS_MAX_INPUTS];

    // Mapping of a PS interpolant to a texture coordinate index (0xffffffff if that interpolant is not a texture coordinate)
    uint32 psInTexCoordIndex[AtiElf_SC_SI_PS_MAX_INPUTS];

    // PS specific shader resources
    uint32 spiShaderPgmRsrc2Ps;

    uint32 spiShaderZFormat;
    uint32 spiPsInControl;
    uint32 spiPsInputAddr;
    uint32 spiPsInputEna;

    uint32 spiBarycCntl;  // XXX mbalci: this would come from SCL(so SC) too; waiting for new interface change to be ported.

    uint32 dbShaderControl;
    uint32 cbShaderMask;

    // Size of each export patch fragment
    uint32 exportPatchCodeSize;
    // Number of RT exports
    uint32 numPsExports;
    // true if dual blending is enable
    bool dualBlending;

    // true if Fragment Ordering is forced
    bool forceFragOrdering;

    // Export patch info per RT
    AtiElf_SC_SI_EXPORT_PATCH_INFO exportPatchInfo[AtiElf_SC_SI_PS_MAX_OUTPUTS];
    uint32 defaultExportFmt;
};
/*
 *  Vertex shader
 */
struct AtiElf_SC_SIVSHWSHADER : public AtiElf_SC_SI_HWSHADER_BASE
{
    // Input semantics
    uint32 numVsInSemantics;
    AtiElf_SC_SI_VSSHADERDCL_IN vsInSemantics[AtiElf_SC_SI_VS_MAX_INPUTS];

    // Output semantics
    uint32 numVsOutSemantics;
    AtiElf_SC_SI_VSSHADERDCL_OUT vsOutSemantics[AtiElf_SC_SI_VS_MAX_OUTPUTS];

    // LS/ES/VS specific shader resources
    union
    {
        uint32 spiShaderPgmRsrc2Ls;
        uint32 spiShaderPgmRsrc2Es;
        uint32 spiShaderPgmRsrc2Vs;
    };

    // SGPR corresponding to gl_DrawIDARB
    uint32 vsDrawIdUserReg;

    // SC-provided values for certain VS-specific registers
    uint32 paClVsOutCntl;
    uint32 spiVsOutConfig;
    uint32 spiShaderPosFormat;
    uint32 vgtStrmoutConfig;

    // Number of SPI-generated VGPRs referenced by the vertex shader
    uint32 vgprCompCnt;

    // Exported vertex size in DWORDs, can be used to program ESGS ring size
    uint32 exportVertexSize;

    bool   useEdgeFlags;
    bool   remapClipDistance; //true if clip distance[4-7] is remapped to [0-3]
    uint32 hwShaderStage;    // hardware stage which this shader actually in when execution
    AtiElf_SC_COMPILE_FLAGS compileFlags;     // compile flag
    uint32 gsMode;                            // gs mode

    bool   isOnChipGs;
    uint32 targetLdsSize;
};

/*
 *  Geometry shader
 */
struct AtiElf_SC_SIGSHWSHADER: public AtiElf_SC_SI_HWSHADER_BASE
{
    // GS specific shader resources
    uint32  spiShaderPgmRsrc2Gs;

    uint32  vgtGsOutPrimType;
    uint32  vgtGsInstanceCnt;

    // Copy shader for this GS
    AtiElf_SC_SIVSHWSHADER copyShader;

    // Max number of verticies generated by GS, write to VGT_GS_MAX_VERT_OUT register
    uint32 maxOutputVertexCount;
    // Per stream export vertex size in DWORDs
    uint32 streamVertexSize[AtiElf_SC_SI_SO_MAX_STREAMS];

    uint32 gsMode;

    bool isOnChipGs;
    uint32 esVertsPerSubgrp;
    uint32 gsPrimsPerSubgrp;
    uint32 ldsEsGsSize;
};
/*
 *  Tessellation control shader
 */
struct AtiElf_SC_SIHSHWSHADER: public AtiElf_SC_SI_HWSHADER_BASE
{
    // HS specific shader resources
    uint32  spiShaderPgmRsrc2Hs;

    // The number of input-control-points per patch declared in HS
    uint32 numInputCP;
    // The number of output-control-points per patch declared in HS
    uint32 numOutputCP;
    // The number of patch-constant outputs per patch declared in HS
    uint32 numPatchConst;
    // The size of one output control-point by a HS, in BYTE!
    uint32 cpStride;
    // The number of HS threads that VGT invokes per patch
    // When cpStride is not zero, numThreadsPerPatch should always be equal to numOutputCP.
    // When cpStride is zero, i.e. there is no hs-cp-phase, we set numThreadsPerPatch
    // to a proper number for performance.
    uint32 numThreadsPerPatch;
    // The amount of output data from HS to tessellation-factor buffer
    // per patch, again in bytes
    uint32 tessFactorStride;

    // states declared in HS, and affect the tessellator
    uint32 tessDomain;
    uint32 tessPartition;
    uint32 tessOutputPrimitive;
    uint32 maxTessFactor;
    uint32 firstEdgeTessFactorIndex;
    bool bOffchipLDS;

    bool isOnChipGs;
};

/*
 *   Fetch shader
 */
struct AtiElf_SIFSHWSHADER
{
    // Total shader structure size
    uint32 shaderByteSize;

    // Byte offset to the start of the code
    uint32 codeByteOffset;

    // Byte size of the code segment
    uint32 codeByteSize;

    // Number of AtiElf_SIFSHWSTREAMINFO elements in the VB input table
    uint32 vbTableElementCount;

    // Number of VGPRs referenced by the fetch shader
    uint32 vgprCount;

    // Number of SGPRs referenced by the fetch shader
    uint32 sgprCount;

    // Number of system-generated VGPRs that the fetch shader references
    // (needed to program vertex shader SPI_SHADER_PGM_RSRC1.VGPR_CMP_CNT if
    // fetch shader uses more sys vgprs than the vertex shader)
    uint32 vgprCompCnt;
};

/*
 *  Compute shader
 */
struct AtiElf_SC_SICSHWSHADER : public AtiElf_SC_SI_HWSHADER_BASE
{
    uint32 computePgmRsrc2;
    uint32 computeMaxWaveId;
    uint32 numThreadX;
    uint32 numThreadY;
    uint32 numThreadZ;
    bool   bOrderedAppendEnable;
};

#endif


