//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file scl_types.h 
/// 
//=====================================================================

#ifndef SCL_TYPES_H
#define SCL_TYPES_H

#include "elf_amdtypes.h"
#include <string>
//
/// Language of an inputted shader.
//
typedef enum sclShaderLanguageEnum
{

    SCL_VERTEX_SHADER,                                 ///< GLSL vertex shaders
    SCL_HULL_SHADER,                                   ///< GLSL hull shaders
    SCL_DOMAIN_SHADER,                                 ///< GLSL domain shaders
    SCL_GEOMETRY_SHADER,                               ///< GLSL geometry shaders
    SCL_FRAGMENT_SHADER,                               ///< GLSL fragment shaders
    SCL_COMPUTE_SHADER,                                ///< OpenGL GLSL Compute Shader
    SCL_FETCH_SHADER_BIN,                              ///< Fetch Shader Binary Description
    SCL_VERTEX_PROGRAM,                                ///< GL_VERTEX_PROGRAM_ARB
    SCL_FRAGMENT_PROGRAM,                              ///< GL_FRAGMENT_PROGRAM_ARB
    SCL_IL_VERTEX_SHADER,                              ///< raw IL vertex shaders
    SCL_IL_FRAGMENT_SHADER,                            ///< raw IL fragment shaders
    SCL_ESSL_VERTEX_SHADER,                            ///< OpenGL ES Shading language Vertex Shader
    SCL_ESSL_FRAGMENT_SHADER,                          ///< OpenGL ES Shading language Fragment Shader
    SCL_IL_COMPUTE_SHADER,                             ///< raw IL compute shaders

    sclShaderLanguage_FIRST = SCL_VERTEX_SHADER,       ///< First enum
    sclShaderLanguage_LAST  = SCL_IL_COMPUTE_SHADER,    ///< Last enum
    sclShaderLanguage_GLSLCOUNT = SCL_COMPUTE_SHADER - SCL_VERTEX_SHADER + 1  ///< Real count of GLSL shading language
} sclShaderLanguage;

typedef enum sclErrorCodeEnum
{
    SCL_NO_ERROR,
    SCL_INVALID_OPERATION,
    SCL_OUT_OF_MEMORY,
    SCL_AASTIPPLE_INTERP_NOT_FREE,
    SCL_NO_FREE_INTERP_FOR_POSITION,
    SCL_NO_FREE_INTERP_FOR_FOGCOORD,
    SCL_TOO_MANY_INTERPOLANTS,
    SCL_TOO_MANY_ALU_INSTRUCTIONS,
    SCL_TOO_MANY_TEX_INSTRUCTIONS,
    SCL_TOO_MANY_ATTRIBUTES,
    SCL_TOO_MANY_TEXTURES,
    SCL_PUNT_SVP,
    SCL_HW_UNSUPPORTED,
    SCL_BITMAP_INTERP_NOT_FREE,
    sclErrorCode_FIRST = SCL_NO_ERROR,
    sclErrorCode_LAST  = SCL_BITMAP_INTERP_NOT_FREE
} sclErrorCode;

struct sclProgram
{
    sclProgram() :
        data(0),
        len(0),
        isa_data(0),
        isa_len(0),
        errorCode(SCL_NO_ERROR),
        errorPos(-1),
        errorString(""),
        bOffChipParameterCache(false)
    {
    }

    const void*  data;
    uint32        len;
    const void*  isa_data;
    uint32       isa_len;
    sclErrorCode errorCode;
    int32        errorPos;
    std::string     errorString;
    sclShaderLanguage language;  ///< source language that the shader was compiled
    bool         bOffChipParameterCache;     ///< the program is compiled with off-chip parameter cache
};

#endif