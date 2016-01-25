//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file AtiElfR600.h 
/// 
//=====================================================================
#ifndef __AtiElfR600_h__
#define __AtiElfR600_h__

#include "atitypes.h"
#include "AtiElf.h"

#define AtiElf_SC_R600_GS_MAX_STREAMOUT_ELEMENTS    64
#define AtiElf_SC_R600_STREAMOUT_MAX_BUFFER_COUNT   4
#define AtiElf_SC_R600_VS_MAX_SEMANTIC_INPUTS       32
#define AtiElf_SC_R600_VS_MAX_SEMANTIC_OUTPUTS      48
#define AtiElf_SC_R600_GS_MAX_SEMANTIC_INPUTS       48
#define AtiElf_SC_R600_GS_MAX_SEMANTIC_OUTPUTS      48
#define AtiElf_SC_R600_PS_MAX_SEMANTIC_INPUTS       48
#define AtiElf_SC_R600_CS_MAX_SEMANTIC_INPUTS       32
#define AtiElf_SC_R600_MAX_CONSTANT_BUFFERS         16

#define AtiElf_SC_R600_NUM_OUT_SEMANTIC_REGS        10

#define AtiElf_SC_R600_NUM_INTBUF                   2

struct AtiElf_SC_R600PSSHADERDCL_IN
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

struct AtiElf_SC_R600GSSTREAMIODCL_OUT
{
    uint32 index;         // semantic index. Opaque to SC
    uint32 memOffset;     // memory offset in DWORDS for writing into I/O stream
    uint32 outputSlot;    // which stream (0-4) is being written to
    uint32 writeMask;     // lower 4 bits specify number of channels written
};

struct AtiElf_SC_R600VSSHADERDCL_IN
{
    uint32 usage : 8;   // semantic usage. Opaque to SC.
    uint32 usageIdx    : 8;   // semantic index. Opaque to SC.
    uint32 reg         : 8;   // HW register 0-19       (unused on input)
    uint32 logicalReg  : 8;   // logical input register (unused on input)
    uint32 vertex      : 8;   // when constructing fetch shaders for
    // ScenarioA/ScenarioB shaders, adjacent
    // vertices may be referenced
};

struct AtiElf_SC_R600VSSHADERDCL_OUT
{
    uint32 usage           : 8;      // semantic usage. IL_IMPORTUSAGE.
    uint32 usageIdx        : 8;      // semantic index. Opaque to SC.
    uint32 reg             : 8;      // HW register (0-31 for DX10, 0-11 for DX9)
    uint32 unused          : 8;
    uint32 swizzles[4];    // component swizzle.
};


struct AtiElf_SC_R600CSSHADERDCL_IN
{
    uint32 usage : 8;   // semantic usage. Opaque to SC.
    int usageIdx : 8;   // semantic index. Opaque to SC.
    int reg      : 8;   // HW register 0-19
    int unused   : 8;   //
};


struct AtiElf_SC_R600PSHWSHADER
{
    AtiElf_SC_HWSHADER_COMMON;


    uint32                 X32XhShaderMemHandle;
    uint32                 X32XhConstBufferMemHandle[AtiElf_SC_R600_NUM_INTBUF];
    uint32                 CodeLenInByte;
    struct
    {
        uint32 sqPgmResourcesPs;
        uint32 sqPgmExportsPs;    // PS export mode
    };
    uint32                 sqPgmOffsetCf;     // control flow offset
    uint32                 spiPSInControl_0;
    uint32                 spiPSInControl_1;
    uint32                 spiInputZ;
    uint32                 MaxScratchRegsNeeded;           // size of scratch buffer used
    AtiElf_SC_R600PSSHADERDCL_IN  psInSemantics[AtiElf_SC_R600_PS_MAX_SEMANTIC_INPUTS];
    uint32                 psTexCoordIndex[AtiElf_SC_R600_PS_MAX_SEMANTIC_INPUTS]; // mark the texcoord index if the interpolate is texcoord, otherwise 0xffffffff;
    uint32                 numPsInSemantics;
    uint32                 cbShaderMask;
    uint32                 cbShaderControl;
    uint32                 dbShaderControl;
    uint32                 X32XerrorToken;
    uint32                 numTexStages;     // num samplers (stages) used by the shader
    uint32                 texStageMaskBits; // bitmask of samplers (stages) used by shader
    uint32                 texCubeMaskBits;  // bitmask of samplers (stages) using cubemaps.
    bool                   bHasFogMerge; // The shader outputs FogFactor, packed in the LSBs
    uint32                 overlapInfo[AtiElf_SC_R600_MAX_CONSTANT_BUFFERS];
    bool                   sample;
    uint32                 iReductionBufferSize;
    bool                   bUseConstBuffer;  // if the shader is compiled with constBuffer
    bool                   dualBlending;     // true if dual blending is enabled
};

struct AtiElf_SC_R600VSHWSHADER
{
    AtiElf_SC_HWSHADER_COMMON;
    uint32                     X32XhShaderMemHandle;         // shader code memory
    uint32                     X32XhConstBufferMemHandle[AtiElf_SC_R600_NUM_INTBUF];
    uint32 CodeLenInByte;

    uint32 sqPgmOffsetCf;    // control flow offset
    uint32 sqPgmResourcesVs; // packed resources

    uint32                   paClVsOutCntl;    // VS outputs control
    uint32                   spiVsOutConfig;   // VS output configuration
    uint32                   MemExportSizeInBytes;      // max size of Vs output to Ring buffer
    uint32                   MaxScratchRegsNeeded;      // size of scratch buffer used
    uint32                   u32MergeFlags;    // some combination of MERGEFLAGS_* bits
    uint32                   gsMode;          // GS mode that VS is compiled for.
    bool                     bGSStreamIOEnabled;       //  Enable VS to write to memory stream
    AtiElf_SC_R600GSSTREAMIODCL_OUT gsStreamOutMap[AtiElf_SC_R600_GS_MAX_STREAMOUT_ELEMENTS];
    uint32                   u32NumStreamOutDcls;    // Number of entries in gsStreamOutMap.
    uint32                   u32StreamOutStride[AtiElf_SC_R600_STREAMOUT_MAX_BUFFER_COUNT];
    uint32                   SpriteEnable;           // optimized for SPRITE_EN mode?
    AtiElf_SC_R600VSSHADERDCL_IN    vsInSemantics[AtiElf_SC_R600_VS_MAX_SEMANTIC_INPUTS];
    uint32                   numVsInSemantics;
    AtiElf_SC_R600VSSHADERDCL_OUT   vsOutSemantics[AtiElf_SC_R600_VS_MAX_SEMANTIC_OUTPUTS];
    uint32                   numVsOutSemantics;
    uint32                   VsParamExportCount;          //--DEPRICATED. Use spiVsOutConfig instead.
    uint32                   eVsOutSemanticMode;   //--DEPRICATED. Use spiVsOutConfig instead.
    uint32                   overlapInfo[AtiElf_SC_R600_MAX_CONSTANT_BUFFERS];
    bool                     useEdgeFlags;
    bool                     bUseConstBuffer;  // if the shader is compiled with constBuffer
    bool                     bUsesVertexId;    // if the shader uses gl_VertexID
};


struct AtiElf_SC_R600CSHWSHADER
{
    AtiElf_SC_HWSHADER_COMMON;
    uint32                          X32XhShaderMemHandle;         // shader code memory
    uint32                          X32XhConstBufferMemHandle[AtiElf_SC_R600_NUM_INTBUF];
    uint32                          CodeLenInByte;

    uint32                          sqPgmOffsetCf;    // control flow offset
    struct
    {
        // **Note** These 4 SQ PGM CS regs are loaded sequentially &
        //          must be kept in order.
        uint32                      sqPgmEndCf;       // control flow end
        uint32                      sqPgmEndAlu;      // ALU  end
        uint32                      sqPgmEndFetch;    // Fetch  end
        uint32                      sqPgmResourcesCs; // packed resources
    };

    uint32                          MaxScratchRegsNeeded;      // size of scratch buffer used
    AtiElf_SC_R600CSSHADERDCL_IN    csInSemantics[AtiElf_SC_R600_CS_MAX_SEMANTIC_INPUTS];
    uint32                          numCsInSemantics;
    uint32                          overlapInfo[AtiElf_SC_R600_MAX_CONSTANT_BUFFERS];

    uint32                          u32NumSharedGprUser; // used in the shader,
    uint32                          u32NumSharedGprTotal; // plus those used by SC for sync barrier,
    uint32                          eCsSetupMode; // slow: set R0.xyzw, fast: set R0.x only
    uint32                          u32NumThreadPerGroup; // number of threads per thread-group
    uint32                          u32TotalNumThreadGroup; // total number thread groups
    uint32                          u32NumWavefrontPerSIMD; // max possible number of wavefronts to fit in a SIMD (based on resource)
    bool                            bIsMaxNumWavePerSIMD; // is it also the maximumu number of active wavefronts?
    bool                            bSetBufferForNumGroup; // Need to set up buffer for info on number of thread groups?
};

//
//mbalci XXX-TODO: we might need to revisit this
//
struct AtiElf_SC_R600GSHWSHADER
{
    AtiElf_SC_HWSHADER_COMMON;

    uint32                     X32XhShaderMemHandle;         // shader code memory
    uint32                     X32XhConstBufferMemHandle[AtiElf_SC_R600_NUM_INTBUF];
    uint32 CodeLenInByte;

    uint32 sqPgmOffsetCf;    // control flow offset
    uint32 sqPgmResourcesGs; // packed resources

    uint32                     X32XhCopyShaderMemHandle;         // copy shader code memory
    uint32                     X32XhCopyConstBufferMemHandle[AtiElf_SC_R600_NUM_INTBUF];
    uint32 CopyCodeLenInByte;

    uint32 vgtGSOutPrimType;


    uint32 sqCopyPgmOffsetCf;    // control flow offset
    uint32 sqCopyPgmResourcesVs; // packed resources

    uint32                   paClVsOutCntl;    // VS outputs control
    uint32                   spiVsOutConfig;   // VS output configuration
    uint32                   MemExportSizeInBytes;      // max size of Vs output to Ring buffer
    uint32                   MaxOutputVertexCount;      //
    uint32                   MaxScratchRegsNeeded;      // size of scratch buffer used
    uint32                   u32MergeFlags;    // some combination of MERGEFLAGS_* bits
    uint32                   u32MergeFetchFlags;
    uint32                   gsMode;          // GS mode that VS is compiled for.
    bool                     bGSStreamIOEnabled;       //  Enable VS to write to memory stream
    AtiElf_SC_R600GSSTREAMIODCL_OUT gsStreamOutMap[AtiElf_SC_R600_GS_MAX_STREAMOUT_ELEMENTS];
    uint32                   u32NumStreamOutDcls;    // Number of entries in gsStreamOutMap.
    uint32                   u32StreamOutStride[AtiElf_SC_R600_STREAMOUT_MAX_BUFFER_COUNT];
    uint32                   SpriteEnable;           // optimized for SPRITE_EN mode?
    AtiElf_SC_R600VSSHADERDCL_IN    gsInSemantics[AtiElf_SC_R600_GS_MAX_SEMANTIC_INPUTS];
    uint32                   numGsInSemantics;
    AtiElf_SC_R600VSSHADERDCL_OUT   gsOutSemantics[AtiElf_SC_R600_GS_MAX_SEMANTIC_OUTPUTS];
    uint32                   numGsOutSemantics;
    uint32                   VsParamExportCount;          //--DEPRICATED. Use spiVsOutConfig instead. [ Well, this is said to be not-depricated ]
    uint32                   eVsOutSemanticMode;          //--DEPRICATED. Use spiVsOutConfig instead.

    //    uint32                   spiOutSemanticRegs_ID_N[AtiElf_SC_R600_NUM_OUT_SEMANTIC_REGS];

    uint32                   vgtVertexReuseBlockCntl;
    uint32                   vgtHosReuseDepth;
    uint32                   vgtOutReallocCntl;
    uint32                   vgtStrmOutBufferEn;

    uint32                   copyPgmEndAlu;
    uint32                   copyPgmEndCf;
    uint32                   copyPgmEndFetch;

    uint32                   pgmEndAlu;
    uint32                   pgmEndCf;
    uint32                   pgmEndFetch;

    uint32                   overlapInfo[AtiElf_SC_R600_MAX_CONSTANT_BUFFERS];


    /*
      regVGT_STRMOUT_BUFFER_EN vgtStrmOutBufferEn; // SO buffer enable
      regVGT_GS_OUT_PRIM_TYPE  vgtGsOutPrimType;   // output primitive type
      regSQ_PGM_CF_OFFSET_VS sqCopyPgmCfOffset; //deprecated, unused by HW, remove after
                                                //dxx driver stops referencing it.
      regSQ_PGM_RESOURCES_VS sqCopyPgmResourcesVs;

      // HW register for copyshader outptus
      regPA_CL_VS_OUT_CNTL     paClVsOutCntl;    // copyshader interpret outputs control
    */

};

#endif
