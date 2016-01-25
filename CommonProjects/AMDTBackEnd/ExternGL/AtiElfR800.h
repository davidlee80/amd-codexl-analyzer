//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file AtiElfR800.h 
/// 
//=====================================================================
#ifndef __AtiElfR800_h__
#define __AtiElfR800_h__



#define AtiElf_SC_R800_GS_MAX_STREAMOUT_ELEMENTS    64
#define AtiElf_SC_R800_STREAMOUT_MAX_BUFFER_COUNT   4
#define AtiElf_SC_R800_STREAMOUT_MAX_STREAM_COUNT   4
#define AtiElf_SC_R800_VS_MAX_SEMANTIC_INPUTS       32
#define AtiElf_SC_R800_VS_MAX_SEMANTIC_OUTPUTS      48
#define AtiElf_SC_R800_GS_MAX_SEMANTIC_INPUTS       48
#define AtiElf_SC_R800_GS_MAX_SEMANTIC_OUTPUTS      48
#define AtiElf_SC_R800_PS_MAX_SEMANTIC_INPUTS       48
#define AtiElf_SC_R800_CS_MAX_SEMANTIC_INPUTS       32
#define AtiElf_SC_R800_MAX_CONSTANT_BUFFERS         16

#define AtiElf_SC_R600_NUM_OUT_SEMANTIC_REGS        10

#define AtiElf_SC_R800_NUM_INTBUF                   2
#define AtiElf_SC_R800_MAX_UAV                      12

struct AtiElf_SC_R800PSSHADERDCL_IN
{
    uint32 usage                    : 8;  // semantic usage. IL_IMPORTUSAGE.
    uint32 usageIdx                 : 8;  // semantic index. Opaque to SC.
    uint32 reg                      : 8;  // HW register  0-19
    uint32 defaultVal               : 2;  // default val if VS doesnt export.
    uint32 flatShade                : 1;  // set if interpolant is 'color'.
    // OR if interp mode is constant.
    uint32 centroid                 : 1;  // set if centroid-sampled interp.
    uint32 linear                   : 1;  // set if interp mode is non-perspect
    uint32 sample                   : 1;  // set if sample frequency
    uint32 unused                   : 2;
    uint32 swizzles[4];                   // component swizzle.
};

struct AtiElf_SC_R800GSSTREAMIODCL_OUT
{
    uint32 index;         // semantic index. Opaque to SC
    uint32 memOffset;     // memory offset in DWORDS for writing into I/O stream
    uint32 outputSlot;    // which stream (0-4) is being written to
    uint32 writeMask;     // lower 4 bits specify number of channels written
    uint32 stream;        // which stream (0-3) is being written to
};

struct AtiElf_SC_R800VSSHADERDCL_IN
{
    uint32 usage : 8;   // semantic usage. Opaque to SC.
    uint32 usageIdx    : 8;   // semantic index. Opaque to SC.
    uint32 reg         : 8;   // HW register 0-19       (unused on input)
    uint32 logicalReg  : 8;   // logical input register (unused on input)
    uint32 vertex      : 8;   // when constructing fetch shaders for
    // ScenarioA/ScenarioB shaders, adjacent
    // vertices may be referenced
};

struct AtiElf_SC_R800VSSHADERDCL_OUT
{
    uint32 usage           : 8;      // semantic usage. IL_IMPORTUSAGE.
    uint32 usageIdx        : 8;      // semantic index. Opaque to SC.
    uint32 reg             : 8;      // HW register (0-31 for DX10, 0-11 for DX9)
    uint32 unused          : 8;
    uint32 swizzles[4];    // component swizzle.
};


struct AtiElf_SC_R800CSSHADERDCL_IN
{
    uint32 usage : 8;   // semantic usage. Opaque to SC.
    int usageIdx : 8;   // semantic index. Opaque to SC.
    int reg      : 8;   // HW register 0-19
    int unused   : 8;   //
};


struct AtiElf_SC_R800PSHWSHADER
{
    AtiElf_SC_HWSHADER_COMMON;

    uint32                 X32XhShaderMemHandle;
    uint32                 X32XhConstBufferMemHandle[AtiElf_SC_R800_NUM_INTBUF];
    uint32                 CodeLenInByte;
    struct
    {
        uint32 sqPgmResourcesPs;
        uint32 sqPgmResources2Ps;
        uint32 sqPgmExportsPs;    // PS export mode
    } sqPgmStruct;
    uint32                 sqPgmOffsetCf;     // control flow offset
    uint32                 spiPSInControl_0;
    uint32                 spiPSInControl_1;
    uint32                 spiPSInControl_2;
    uint32                 spiInputZ;
    uint32                 spiBarycCntl;
    uint32                 MaxScratchRegsNeeded;           // size of scratch buffer used
    AtiElf_SC_R800PSSHADERDCL_IN  psInSemantics[AtiElf_SC_R800_PS_MAX_SEMANTIC_INPUTS];
    uint32                 psTexCoordIndex[AtiElf_SC_R800_PS_MAX_SEMANTIC_INPUTS]; // mark the texcoord index if the interpolate is texcoord, otherwise 0xffffffff;
    uint32                 numPsInSemantics;
    uint32                 cbShaderMask;
    uint32                 cbShaderControl;
    uint32                 dbShaderControl;
    uint32                 X32XerrorToken;
    uint32                 numTexStages;     // num samplers (stages) used by the shader
    uint32                 texStageMaskBits; // bitmask of samplers (stages) used by shader
    uint32                 texCubeMaskBits;  // bitmask of samplers (stages) using cubemaps.
    bool                   bHasFogMerge;     // The shader outputs FogFactor, packed in the LSBs
    uint32                 overlapInfo[AtiElf_SC_R800_MAX_CONSTANT_BUFFERS];
    bool                   sample;
    uint32                 iReductionBufferSize;
    bool                   bUseConstBuffer;  // if the shader is compiled with constBuffer
    bool                   dualBlending;     // true if dual blending is enabled
};

struct AtiElf_SC_R800VSHWSHADER
{
    AtiElf_SC_HWSHADER_COMMON;
    uint32                     X32XhShaderMemHandle;         // shader code memory
    uint32                     X32XhConstBufferMemHandle[AtiElf_SC_R800_NUM_INTBUF];
    uint32 CodeLenInByte;

    uint32 sqPgmOffsetCf;    // control flow offset
    uint32 sqPgmResourcesVs; // packed resources
    uint32 sqPgmResources2Vs;

    uint32                   paClVsOutCntl;    // VS outputs control
    uint32                   spiVsOutConfig;   // VS output configuration
    uint32                   MemExportSizeInBytes;      // max size of Vs output to Ring buffer
    uint32                   MaxScratchRegsNeeded;      // size of scratch buffer used
    uint32                   u32MergeFlags;    // some combination of MERGEFLAGS_* bits
    uint32                   gsMode;          // GS mode that VS is compiled for.
    bool                     bGSStreamIOEnabled;       //  Enable VS to write to memory stream
    uint32                   vgtStrmOutConfig;
    AtiElf_SC_R800GSSTREAMIODCL_OUT gsStreamOutMap[AtiElf_SC_R800_GS_MAX_STREAMOUT_ELEMENTS];
    uint32                   u32NumStreamOutDcls;    // Number of entries in gsStreamOutMap.
    uint32                   u32StreamOutStride[AtiElf_SC_R800_STREAMOUT_MAX_BUFFER_COUNT];
    uint32                   SpriteEnable;           // optimized for SPRITE_EN mode?
    AtiElf_SC_R800VSSHADERDCL_IN    vsInSemantics[AtiElf_SC_R800_VS_MAX_SEMANTIC_INPUTS];
    uint32                   numVsInSemantics;
    AtiElf_SC_R800VSSHADERDCL_OUT   vsOutSemantics[AtiElf_SC_R800_VS_MAX_SEMANTIC_OUTPUTS];
    uint32                   numVsOutSemantics;
    uint32                   VsParamExportCount;          //--DEPRICATED. Use spiVsOutConfig instead.
    uint32                   eVsOutSemanticMode;   //--DEPRICATED. Use spiVsOutConfig instead.
    uint32                   overlapInfo[AtiElf_SC_R800_MAX_CONSTANT_BUFFERS];
    bool                     useEdgeFlags;

    // For R8xx HW-LS
    // reports to driver the amount of output data by a LS, in BYTE!
    // All R8xx LDS address computation is changed to be in byte!
    // Driver needs to put it into a constant-buffer location. Later on,
    // HS and DS need to access that constant when computing LDS address.
    uint32 u32LsStride;
    // VS compile mode
    uint32 vsCompMode;
    bool                     bUseConstBuffer;  // if the shader is compiled with constBuffer
    bool                     bUsesVertexId;    // if the shader uses gl_VertexID
    bool                     bRemapClipDistance; //true if clip distance[4-7] is remapped to [0-3]
};


struct AtiElf_SC_R800CSHWSHADER
{
    AtiElf_SC_HWSHADER_COMMON;

    uint32                          X32XhShaderMemHandle;         // shader code memory
    uint32                          X32XhConstBufferMemHandle[AtiElf_SC_R800_NUM_INTBUF];
    uint32                          CodeLenInByte;

    uint32                          sqPgmOffsetCf;    // control flow offset
    uint32                          sqPgmResourcesCs; // packed resources
    uint32                          sqPgmResources2Cs; // packed resources
    uint32                          sqLdsAllocCs;

    uint32                          MaxScratchRegsNeeded;      // size of scratch buffer used
    AtiElf_SC_R800CSSHADERDCL_IN    csInSemantics[AtiElf_SC_R800_CS_MAX_SEMANTIC_INPUTS];
    uint32                          numCsInSemantics;
    uint32                          overlapInfo[AtiElf_SC_R800_MAX_CONSTANT_BUFFERS];

    uint32                          u32NumSharedGprUser;    // used in the shader,
    uint32                          u32NumSharedGprTotal;   // plus those used by SC for sync barrier,
    uint32                          eCsSetupMode;           // slow: set R0.xyzw, fast: set R0.x only
    uint32                          u32NumThreadPerGroup;   // number of threads per thread-group
    uint32                          u32NumThreadPerGroupX;  // dimension x of NumThreadPerGroup
    uint32                          u32NumThreadPerGroupY;  // dimension Y of NumThreadPerGroup
    uint32                          u32NumThreadPerGroupZ;  // dimension Z of NumThreadPerGroup
    uint32                          u32TotalNumThreadGroup; // total number thread groups
    uint32                          u32NumWavefrontPerSIMD; // max possible number of wavefronts to fit in a SIMD (based on resource)
    bool                            bIsMaxNumWavePerSIMD;   // is it also the maximumu number of active wavefronts?
    bool                            bSetBufferForNumGroup;  // Need to set up buffer for info on number of thread groups?
    uint32                          u32UavRtnBufStride[AtiElf_SC_R800_MAX_UAV]; // UAV mailbox stride in DWORDs per UAV
    uint32                          u32RatOpIsUsed;         // bit mask with each bit corresponding to one RAT surface with RAT ops
    uint32                          u32RatAtomicOpIsUsed;   // bit mask with each bit corresponding to one RAT surface with RAT atomic ops
};

//
//mbalci XXX-TODO: we might need to revisit this
//
struct AtiElf_SC_R800GSHWSHADER
{
    AtiElf_SC_HWSHADER_COMMON;

    uint32                          X32XhShaderMemHandle;         // shader code memory
    uint32                          X32XhConstBufferMemHandle[AtiElf_SC_R800_NUM_INTBUF];
    uint32                          CodeLenInByte;

    uint32                          X32XhCopyShaderMemHandle;         // copy shader code memory
    uint32                          X32XhCopyConstBufferMemHandle[AtiElf_SC_R800_NUM_INTBUF];
    uint32                          CopyCodeLenInByte;

    uint32                          vgtGsOutPrimType;       // output primitive type
    uint32                          MaxOutputVertexCount;   // max number of vertices we allow to be outputted

    uint32                          sqPgmResourcesGs;  // packed GS resources
    uint32                          sqPgmResources2Gs; // more packed GS resources

    uint32                          sqCopyPgmResourcesVs;  // packed VS resources
    uint32                          sqCopyPgmResources2Vs; // more packed VS resources

    uint32                          paClVsOutCntl;    // VS outputs control
    uint32                          spiVsOutConfig;   // VS output configuration
    uint32                          MemExportSizeInBytes;      // max size of Vs output to Ring buffer
    uint32                          MaxScratchRegsNeeded;      // size of scratch buffer used
    uint32                          gsMode;          // GS mode that VS is compiled for.
    bool                            bGSStreamIOEnabled;       //  Enable VS to write to memory stream
    uint32                          vgtStrmOutConfig;
    uint32                          vgtStrmoutBufferConfig;
    AtiElf_SC_R800GSSTREAMIODCL_OUT gsStreamOutMap[AtiElf_SC_R800_GS_MAX_STREAMOUT_ELEMENTS];
    uint32                          u32NumStreamOutDcls;    // Number of entries in gsStreamOutMap.
    uint32                          u32StreamOutStride[AtiElf_SC_R800_STREAMOUT_MAX_BUFFER_COUNT];
    uint32                          StreamExportVertexSizeDWords[AtiElf_SC_R800_STREAMOUT_MAX_STREAM_COUNT];
    uint32                          sqGSVSRingOffset_1;
    uint32                          sqGSVSRingOffset_2;
    uint32                          sqGSVSRingOffset_3;

    AtiElf_SC_R800VSSHADERDCL_OUT   gsOutSemantics[AtiElf_SC_R800_GS_MAX_SEMANTIC_OUTPUTS];
    uint32                          numGsOutSemantics;

    uint32                          VsParamExportCount;          //--DEPRICATED. Use spiVsOutConfig instead.
    uint32                          eVsOutSemanticMode;   //--DEPRICATED. Use spiVsOutConfig instead.
    uint32                          vgtGsInstanceCnt;
    bool                            bRemapClipDistance; //true if clip distance[4-7] is remapped to [0-3]
};

struct AtiElf_SC_R800HSHWSHADER
{
    AtiElf_SC_HWSHADER_COMMON;

    uint32                     X32XhShaderMemHandle;         // shader code memory
    uint32                     X32XhConstBufferMemHandle[AtiElf_SC_R800_NUM_INTBUF];
    uint32                     CodeLenInByte;

    uint32                     sqPgmOffsetCf;    // control flow offset
    uint32                     sqPgmResourcesHs; // packed resources
    uint32                     sqPgmResources2Hs;

    uint32                   paClHsOutCntl;    // VS outputs control
    uint32                   spiHsOutConfig;   // VS output configuration
    uint32                   MemExportSizeInBytes;      // max size of Vs output to Ring buffer
    uint32                   MaxScratchRegsNeeded;      // size of scratch buffer used
    uint32                   u32MergeFlags;    // some combination of MERGEFLAGS_* bits
    AtiElf_SC_R800VSSHADERDCL_IN    hsInSemantics[AtiElf_SC_R800_VS_MAX_SEMANTIC_INPUTS];
    uint32                   numHsInSemantics;
    AtiElf_SC_R800VSSHADERDCL_OUT   hsOutSemantics[AtiElf_SC_R800_VS_MAX_SEMANTIC_OUTPUTS];
    uint32                   numHsOutSemantics;
    uint32                   HsParamExportCount;   //--DEPRICATED. Use spiVsOutConfig instead.
    uint32                   eHsOutSemanticMode;   //--DEPRICATED. Use spiVsOutConfig instead.
    uint32                   overlapInfo[AtiElf_SC_R800_MAX_CONSTANT_BUFFERS];


    // The number of input-control-points per patch declared in HS
    uint32 u32HsNumInputCP;
    // The number of output-control-points per patch declared in HS
    uint32 u32HsNumOutputCP;
    // The number of patch-constant outputs per patch declared in HS
    uint32 u32HsNumPatchConst;
    // The size of one output control-point by a HS, in BYTE!
    uint32 u32HsCPStride;
    // The number of hs-thread that VGT invokes per patch
    // When HsCPStride is not zero, HsNumThread should always be equal
    // to HsNumOutputCP.
    // When HsCPStride is zero, i.e. there is no hs-cp-phase, we set
    // HsNumThread to a proper number for performance, the immediate
    // plan is simple 1.
    uint32 u32HsNumThread;
    // The amount of output data from HS to tessellation-factor buffer
    // per patch, again in bytes
    uint32 u32HsTessFactorStride;
    // tessellation-factor buffer can be in patch-major layout or
    // factor-major layout
    uint32 HsTessFactorBufferTFMajor;
    // tessellation domain type
    uint32 u32TsDomain;
    // tessellation partition type
    uint32 u32TsPartition;
    // tessellator output primitive type
    uint32 u32TsOutputPrimitive;
    // HS declares Primitive ID
    bool bUsePrimID;
    // HS offchip LDS mode enabled
    bool bOffchipLDS;
};

struct AtiElf_SC_R800DSHWSHADER
{
    AtiElf_SC_HWSHADER_COMMON;

    uint32                     X32XhShaderMemHandle;         // shader code memory
    uint32                     X32XhConstBufferMemHandle[AtiElf_SC_R800_NUM_INTBUF];
    uint32                     CodeLenInByte;

    uint32                     sqPgmOffsetCf;    // control flow offset
    uint32                     sqPgmResourcesDs; // packed resources
    uint32                     sqPgmResources2Ds;

    uint32                   paClDsOutCntl;    // VS outputs control
    uint32                   spiDsOutConfig;   // VS output configuration
    uint32                   MemExportSizeInBytes;      // max size of Vs output to Ring buffer
    uint32                   MaxScratchRegsNeeded;      // size of scratch buffer used
    uint32                   u32MergeFlags;    // some combination of MERGEFLAGS_* bits
    bool                     bGSStreamIOEnabled;       //  Enable VS to write to memory stream
    AtiElf_SC_R800GSSTREAMIODCL_OUT gsStreamOutMap[AtiElf_SC_R800_GS_MAX_STREAMOUT_ELEMENTS];
    uint32                   u32NumStreamOutDcls;    // Number of entries in gsStreamOutMap.
    uint32                   u32StreamOutStride[AtiElf_SC_R800_STREAMOUT_MAX_BUFFER_COUNT];
    uint32                   SpriteEnable;           // optimized for SPRITE_EN mode?
    AtiElf_SC_R800VSSHADERDCL_IN    dsInSemantics[AtiElf_SC_R800_VS_MAX_SEMANTIC_INPUTS];
    uint32                   numDsInSemantics;
    AtiElf_SC_R800VSSHADERDCL_OUT   dsOutSemantics[AtiElf_SC_R800_VS_MAX_SEMANTIC_OUTPUTS];
    uint32                   numDsOutSemantics;
    uint32                   DsParamExportCount;          //--DEPRICATED. Use spiVsOutConfig instead.
    uint32                   eDsOutSemanticMode;   //--DEPRICATED. Use spiVsOutConfig instead.
    uint32                   overlapInfo[AtiElf_SC_R800_MAX_CONSTANT_BUFFERS];
    uint32                   dsCompMode; // DS compile mode
    bool                     bRemapClipDistance; //true if clip distance[4-7] is remapped to [0-3]
};

#endif
