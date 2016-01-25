//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file elf_amdtypes.h 
/// 
//=====================================================================
#ifndef ELF_AMD_TYPES
#define ELF_AMD_TYPES

#ifdef _WINDOWS
    typedef signed __int64 int64, *pint64;

    /// 64-bit unsigned integer
    typedef unsigned __int64 uint64, *puint64;
#else
    #include <string.h>
    typedef int64_t int64, *pint64;
    typedef uint64_t uint64, *puint64;
#endif



typedef unsigned char char8, *pchar8;

/// 8-bit signed integer
typedef signed char int8, *pint8;

/// 16-bit signed integer
typedef signed short int16, *pint16;

/// 32-bit signed integer
typedef signed int int32, *pint32;

/// 8-bit unsigned integer
typedef unsigned char uint8, *puint8;

/// 16-bit unsigned integer
typedef unsigned short uint16, *puint16;

/// 32-bit unsigned integer
typedef unsigned int uint32, *puint32;

/// 32-bit floating point
typedef float float32, *pfloat32;

/// 64-bit floating point
typedef double float64, *pfloat64;

/// pointer to void
typedef void* pvoid;

/// unsigned integer, the same size as a pointer
typedef uintptr_t uintp;


typedef enum AtiElfSectionOffsetsEnum
{
    ATIELF_CONSTANT_SECTION_OFFSET        = 1,
    ATIELF_INPUT_SECTION_OFFSET           = 2,  ///< vertex/pixel shader input section
    ATIELF_OUTPUT_SECTION_OFFSET          = 3,
    ATIELF_PROGRAMINFO_SECTION_OFFSET     = 4,
    ATIELF_SYMBOL_SECTION_OFFSET          = 5,
    ATIELF_SCALAR_CONSTANT_SECTION_OFFSET = 6,
    ATIELF_IL_SECTION_OFFSET              = 7,
    ATIELF_FETCHINPUT_SECTION_OFFSET      = 8,  ///< fetch shader input section
    ATIELF_SOFTVAPIL_SECTION_OFFSET       = 9,  ///< softvap compiled il section
    ATIELF_S2H_INPUT_MAP_SECTION_OFFSET   = 10, ///< svp input section offset
    ATIELF_PATCHED_IL_SECTION_OFFSET      = 11,
    ATIELF_PASSTHRU_VS_SECTION_OFFSET     = 12,
    ATIELF_LOOPCONSTANT_SECTION_OFFSET    = 13,
    ATIELF_SETTING_SECTION_OFFSET         = 14,
    ATIELF_PARAMETER_SECTION_OFFSET       = 15,
    ATIELF_SOURCESTRING_SECTION_OFFSET    = 16,
    ATIELF_KEY_BINARY_SECTION_OFFSET      = 17,
    ATIELF_ILTEXT_SECTION_OFFSET          = 18,
    ATIELF_RODATA_SECTION_OFFSET          = 19,
    ATIELF_PROGUSAGEINFO_SECTION_OFFSET   = 20,
    ATIELF_INTERNAL_SECTION_OFFSET         = 21,
    ATIELF_CHECKSUM_SECTION_OFFSET         = 22,
    AtiElfSectionOffsets_FIRST             = ATIELF_CONSTANT_SECTION_OFFSET,
    AtiElfSectionOffsets_LAST              = ATIELF_CHECKSUM_SECTION_OFFSET,
} AtiElfSectionOffsets;



enum AtiElfSymbolDataType
{
    ATIELF_SYMBOL_DATATYPE_VOID,                   ///< void
    ATIELF_SYMBOL_DATATYPE_BOOL,                   ///< bool
    ATIELF_SYMBOL_DATATYPE_INT,                    ///< int
    ATIELF_SYMBOL_DATATYPE_UINT,                   ///< unsigned int
    ATIELF_SYMBOL_DATATYPE_FLOAT,                  ///< float
    ATIELF_SYMBOL_DATATYPE_DOUBLE,                 ///< double
    ATIELF_SYMBOL_DATATYPE_INT64,                  ///< int64_t
    ATIELF_SYMBOL_DATATYPE_UINT64,                 ///< uint64_t
    ATIELF_SYMBOL_DATATYPE_FLOAT16,                ///< float16
    ATIELF_SYMBOL_DATATYPE_DVEC2,                  ///< dvec2
    ATIELF_SYMBOL_DATATYPE_DVEC3,                  ///< dvec3
    ATIELF_SYMBOL_DATATYPE_DVEC4,                  ///< dvec4
    ATIELF_SYMBOL_DATATYPE_VEC2,                   ///< vec2
    ATIELF_SYMBOL_DATATYPE_VEC3,                   ///< vec3
    ATIELF_SYMBOL_DATATYPE_VEC4,                   ///< vec4
    ATIELF_SYMBOL_DATATYPE_BVEC2,                  ///< bvec2
    ATIELF_SYMBOL_DATATYPE_BVEC3,                  ///< bvec3
    ATIELF_SYMBOL_DATATYPE_BVEC4,                  ///< bvec4
    ATIELF_SYMBOL_DATATYPE_IVEC2,                  ///< ivec2
    ATIELF_SYMBOL_DATATYPE_IVEC3,                  ///< ivec3
    ATIELF_SYMBOL_DATATYPE_IVEC4,                  ///< ivec4
    ATIELF_SYMBOL_DATATYPE_UVEC2,                  ///< unsigned ivec2
    ATIELF_SYMBOL_DATATYPE_UVEC3,                  ///< unsigned ivec3
    ATIELF_SYMBOL_DATATYPE_UVEC4,                  ///< unsigned ivec4
    ATIELF_SYMBOL_DATATYPE_I64VEC2,                ///< i64vec2
    ATIELF_SYMBOL_DATATYPE_I64VEC3,                ///< i64vec3
    ATIELF_SYMBOL_DATATYPE_I64VEC4,                ///< i64vec4
    ATIELF_SYMBOL_DATATYPE_U64VEC2,                ///< u64vec2
    ATIELF_SYMBOL_DATATYPE_U64VEC3,                ///< u64vec3
    ATIELF_SYMBOL_DATATYPE_U64VEC4,                ///< u64vec4
    ATIELF_SYMBOL_DATATYPE_F16VEC2,                ///< f16vec2
    ATIELF_SYMBOL_DATATYPE_F16VEC3,                ///< f16vec3
    ATIELF_SYMBOL_DATATYPE_F16VEC4,                ///< f16vec4
    ATIELF_SYMBOL_DATATYPE_MAT2,                   ///< mat2
    ATIELF_SYMBOL_DATATYPE_MAT2X3,                 ///< mat2x3
    ATIELF_SYMBOL_DATATYPE_MAT2X4,                 ///< mat2x4
    ATIELF_SYMBOL_DATATYPE_MAT3X2,                 ///< mat3x2
    ATIELF_SYMBOL_DATATYPE_MAT3,                   ///< mat3
    ATIELF_SYMBOL_DATATYPE_MAT3X4,                 ///< mat3x4
    ATIELF_SYMBOL_DATATYPE_MAT4X2,                 ///< mat4x2
    ATIELF_SYMBOL_DATATYPE_MAT4X3,                 ///< mat4x3
    ATIELF_SYMBOL_DATATYPE_MAT4,                   ///< mat4
    ATIELF_SYMBOL_DATATYPE_DMAT2,                  ///< dmat2
    ATIELF_SYMBOL_DATATYPE_DMAT2X3,                ///< dmat2x3
    ATIELF_SYMBOL_DATATYPE_DMAT2X4,                ///< dmat2x4
    ATIELF_SYMBOL_DATATYPE_DMAT3X2,                ///< dmat3x2
    ATIELF_SYMBOL_DATATYPE_DMAT3,                  ///< dmat3
    ATIELF_SYMBOL_DATATYPE_DMAT3X4,                ///< dmat3x4
    ATIELF_SYMBOL_DATATYPE_DMAT4X2,                ///< dmat4x2
    ATIELF_SYMBOL_DATATYPE_DMAT4X3,                ///< dmat4x3
    ATIELF_SYMBOL_DATATYPE_DMAT4,                  ///< dmat4
    ATIELF_SYMBOL_DATATYPE_F16MAT2,                ///< f16mat2
    ATIELF_SYMBOL_DATATYPE_F16MAT2X3,              ///< f16mat2x3
    ATIELF_SYMBOL_DATATYPE_F16MAT2X4,              ///< f16mat2x4
    ATIELF_SYMBOL_DATATYPE_F16MAT3X2,              ///< f16mat3x2
    ATIELF_SYMBOL_DATATYPE_F16MAT3,                ///< f16mat3
    ATIELF_SYMBOL_DATATYPE_F16MAT3X4,              ///< f16mat3x4
    ATIELF_SYMBOL_DATATYPE_F16MAT4X2,              ///< f16mat4x2
    ATIELF_SYMBOL_DATATYPE_F16MAT4X3,              ///< f16mat4x3
    ATIELF_SYMBOL_DATATYPE_F16MAT4,                ///< f16mat4

    ATIELF_SYMBOL_DATATYPE_SAMPLER_1D,             ///< sampler1D
    ATIELF_SYMBOL_DATATYPE_SAMPLER_2D,             ///< sampler2D
    ATIELF_SYMBOL_DATATYPE_SAMPLER_2D_RECT,        ///< sampler2DRect
    ATIELF_SYMBOL_DATATYPE_SAMPLER_EXTERNAL,       ///< samplerExternalOES
    ATIELF_SYMBOL_DATATYPE_SAMPLER_3D,             ///< sampler3D
    ATIELF_SYMBOL_DATATYPE_SAMPLER_CUBE,           ///< samplerCube
    ATIELF_SYMBOL_DATATYPE_SAMPLER_1D_ARRAY,       ///< sampler1DArray
    ATIELF_SYMBOL_DATATYPE_SAMPLER_2D_ARRAY,       ///< sampler2DArray
    ATIELF_SYMBOL_DATATYPE_SAMPLER_CUBE_ARRAY,        ///<samplerCubeArray
    ATIELF_SYMBOL_DATATYPE_SAMPLER_BUFFER,         ///< samplerBuffer
    ATIELF_SYMBOL_DATATYPE_SAMPLER_RENDERBUFFER,   ///< samplerRenderbuffer
    ATIELF_SYMBOL_DATATYPE_SAMPLER_2D_MS,          ///< sampler2DMS
    ATIELF_SYMBOL_DATATYPE_SAMPLER_2D_MS_ARRAY,    ///< sampler2DMSArray

    ATIELF_SYMBOL_DATATYPE_SAMPLER_1D_SHADOW,      ///< sampler1DShadow
    ATIELF_SYMBOL_DATATYPE_SAMPLER_2D_SHADOW,      ///< sampler2DShadow
    ATIELF_SYMBOL_DATATYPE_SAMPLER_2D_RECT_SHADOW, ///< sampler2DRectShadow
    ATIELF_SYMBOL_DATATYPE_SAMPLER_CUBE_SHADOW,    ///< samplerCubeShadow
    ATIELF_SYMBOL_DATATYPE_SAMPLER_1D_ARRAY_SHADOW,///< sampler1DArrayShadow
    ATIELF_SYMBOL_DATATYPE_SAMPLER_2D_ARRAY_SHADOW,///< sampler2DArrayShadow
    ATIELF_SYMBOL_DATATYPE_SAMPLER_CUBE_ARRAY_SHADOW, ///<samplerCubeArrayShadow
    ///
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_1D,         ///< isampler1D
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_2D,         ///< isampler2D
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_2D_RECT,    ///< isampler2DRect
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_3D,         ///< isampler3D
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_CUBE,       ///< isamplerCube
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_1D_ARRAY,   ///< isampler1DArray
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_2D_ARRAY,   ///< isampler2DArray
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_CUBE_ARRAY, ///< isamplerCubeArray
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_BUFFER,     ///< isamplerBuffer
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_RENDERBUFFER,   ///< isamplerRenderbuffer
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_2D_MS,          ///< isampler2DMS
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_2D_MS_ARRAY,    ///< isampler2DMSArray

    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_1D,         ///< usampler1D
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_2D,         ///< usampler2D
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_2D_RECT,    ///< usampler2DRect
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_3D,         ///< usampler3D
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_CUBE,       ///< usamplerCube
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_1D_ARRAY,   ///< usampler1DArray
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_2D_ARRAY,   ///< usampler2DArray
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_CUBE_ARRAY, ///< usamplerCubeArray
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_BUFFER,     ///< usamplerBuffer
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_RENDERBUFFER,   ///< usamplerRenderbuffer
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_2D_MS,          ///< usampler2DMS
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_2D_MS_ARRAY,    ///< usampler2DMSArray

    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_1D,             ///< f16sampler1D
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_2D,             ///< f16sampler2D
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_2D_RECT,        ///< f16sampler2DRect
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_3D,             ///< f16sampler3D
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_CUBE,           ///< f16samplerCube
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_1D_ARRAY,       ///< f16sampler1DArray
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_2D_ARRAY,       ///< f16sampler2DArray
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_CUBE_ARRAY,        ///<f16samplerCubeArray
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_BUFFER,         ///< f16samplerBuffer
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_2D_MS,          ///< f16sampler2DMS
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_2D_MS_ARRAY,    ///< f16sampler2DMSArray
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_1D_SHADOW,      ///< f16sampler1DShadow
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_2D_SHADOW,      ///< f16sampler2DShadow
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_2D_RECT_SHADOW, ///< f16sampler2DRectShadow
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_CUBE_SHADOW,    ///< f16samplerCubeShadow
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_1D_ARRAY_SHADOW,///< f16sampler1DArrayShadow
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_2D_ARRAY_SHADOW,///< f16sampler2DArrayShadow
    ATIELF_SYMBOL_DATATYPE_FLOAT16_SAMPLER_CUBE_ARRAY_SHADOW,///< f16samplerCubeArrayShadow

    ATIELF_SYMBOL_DATATYPE_IMAGE_1D,                        ///< image1D
    ATIELF_SYMBOL_DATATYPE_IMAGE_2D,                        ///< image2D
    ATIELF_SYMBOL_DATATYPE_IMAGE_3D,                        ///< image3D
    ATIELF_SYMBOL_DATATYPE_IMAGE_2D_RECT,                   ///< image2DRect
    ATIELF_SYMBOL_DATATYPE_IMAGE_CUBEMAP,                   ///< imageCube
    ATIELF_SYMBOL_DATATYPE_IMAGE_BUFFER,                    ///< imageBuffer
    ATIELF_SYMBOL_DATATYPE_IMAGE_1D_ARRAY,                  ///< image1DArray
    ATIELF_SYMBOL_DATATYPE_IMAGE_2D_ARRAY,                  ///< image2DArray
    ATIELF_SYMBOL_DATATYPE_IMAGE_CUBEMAP_ARRAY,             ///< imageCubeArray
    ATIELF_SYMBOL_DATATYPE_IMAGE_2D_MS,                     ///< image2DMultisample
    ATIELF_SYMBOL_DATATYPE_IMAGE_2D_MS_ARRAY,               ///< image2DMultisampleArray

    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_1D,                    ///< iimage1D
    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_2D,                    ///< iimage2D
    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_3D,                    ///< iimage3D
    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_2D_RECT,               ///< iimage2DRect
    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_CUBEMAP,               ///< iimageCube
    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_BUFFER,                ///< iimageBuffer
    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_1D_ARRAY,              ///< iimage1DArray
    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_2D_ARRAY,              ///< iimage2DArray
    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_CUBEMAP_ARRAY,         ///< iimageCubeArray
    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_2D_MS,                 ///< iimage2DMultisample
    ATIELF_SYMBOL_DATATYPE_INT_IMAGE_2D_MS_ARRAY,           ///< iimage2DMultisampleArray

    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_1D,           ///< uimage1D
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_2D,           ///< uimage2D
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_3D,           ///< uimage3D
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_2D_RECT,      ///< uimage2DRect
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_CUBEMAP,      ///< uimageCube
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_BUFFER,       ///< uimageBuffer
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_1D_ARRAY,     ///< uimage1DArray
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_2D_ARRAY,     ///< uimage2DArray
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_CUBEMAP_ARRAY,///< uimageCubeArray
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_2D_MS,        ///< uimage2DMultisample
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_IMAGE_2D_MS_ARRAY,  ///< uimage2DMultisampleArray

    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_1D,                ///< f16image1D
    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_2D,                ///< f16image2D
    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_3D,                ///< f16image3D
    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_2D_RECT,           ///< f16image2DRect
    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_CUBEMAP,           ///< f16imageCube
    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_BUFFER,            ///< f16imageBuffer
    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_1D_ARRAY,          ///< f16image1DArray
    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_2D_ARRAY,          ///< f16image2DArray
    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_CUBEMAP_ARRAY,     ///< f16imageCubeArray
    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_2D_MS,             ///< f16image2DMultisample
    ATIELF_SYMBOL_DATATYPE_FLOAT16_IMAGE_2D_MS_ARRAY,       ///< f16image2DMultisampleArray

    ATIELF_SYMBOL_DATATYPE_SAMPLER_BUFFER_AMD,                 ///< used for tessellation (from vbo)
    ATIELF_SYMBOL_DATATYPE_INT_SAMPLER_BUFFER_AMD,             ///< used for vertex shader tesssellation
    ATIELF_SYMBOL_DATATYPE_UNSIGNED_INT_SAMPLER_BUFFER_AMD,    ///< used for vertex shader tesssellation

    ATIELF_SYMBOL_DATATYPE_ATOMIC_COUNTER,          ///< atomic counter

    ATIELF_SYMBOL_DATATYPE_STRUCT,                 ///< struct, used for bindable uniform, it is also used for type check in separate program mode
    ATIELF_SYMBOL_DATATYPE_INTERFACE,              ///< interface block, used for type check in separate progarm mode
    AtiElfSymbolDataType_FIRST = ATIELF_SYMBOL_DATATYPE_VOID,
    AtiElfSymbolDataType_LAST  = ATIELF_SYMBOL_DATATYPE_INTERFACE


};


//
///  Output Types
//
typedef enum AtiElfOutputTypeEnum
{
    ATIELF_OUTPUT_POS,            ///< Position
    ATIELF_OUTPUT_POINTSIZE,      ///< Point size
    ATIELF_OUTPUT_COLOR,          ///< Primary color, offset 0 is front, offset 1 is back
    ATIELF_OUTPUT_SECONDARYCOLOR, ///< Secondary color, offset 0 is front, offset 1 is back
    ATIELF_OUTPUT_GENERIC,        ///< Texture coordinate and user define varyings for pre R5xx asics, but on R6xx above, generic contains colors
    ATIELF_OUTPUT_DEPTH,          ///< Fragment depth
    ATIELF_OUTPUT_CLIPDISTANCE,   ///< Clip distance
    ATIELF_OUTPUT_PRIMITIVEID,    ///< PrimitiveID
    ATIELF_OUTPUT_LAYER,          ///< Layer
    ATIELF_OUTPUT_VIEWPORTINDEX,  ///< viewportindex
    ATIELF_OUTPUT_STENCIL,        ///< Fragment stencil ref value
    ATIELF_OUTPUT_STENCIL_VALUE,  ///< Fragment stencil operation value
    ATIELF_OUTPUT_SAMPLEMASK,      ///< Output sample coverage mask (FS only)
    ATIELF_OUTPUT_STREAMID,       ///< Ouput stream id
} AtiElfOutputType;



struct AtiElfOutput
{
    unsigned int            structSize;     ///< The total size of this structure, including variable string
    AtiElfOutputType        type;           ///< Semantic type
    unsigned int            vofs;           ///< Virtual offset
    unsigned int            pofs;           ///< Physical offset
    AtiElfSymbolDataType    data_type;      ///< Data type
    unsigned int            array_size;     ///< Array size
    unsigned int            buffer;         ///< transform feedback buffer index
    unsigned int            stride;         ///< transform feedback buffer stride
    bool                    isTfVarying;    ///< Is transform feedback
    const char*             name;           ///< Name of the output
    AtiElfOutput() :
        structSize(0),
        type(ATIELF_OUTPUT_COLOR),
        vofs(0),
        pofs(0),
        data_type(ATIELF_SYMBOL_DATATYPE_VOID),
        array_size(0),
        buffer(0),
        stride(0),
        isTfVarying(false),
        name(nullptr)
    {}
};

//
///  Constant Types
//
typedef enum AtiElfConstantTypeEnum
{
    ATIELF_CONST_LITERAL,         ///< constant is a literal value
    ATIELF_CONST_LOCAL,           ///< constant is a local value (per program)
    ATIELF_CONST_GLOBAL           ///< constant is a global value (global state)
} AtiElfConstantType;





//
///  Input Types
//
typedef enum AtiElfInputTypeEnum
{
    ATIELF_INPUT_ATTRIB,             ///< generic attribute
    ATIELF_INPUT_COLOR,              ///< primary color
    ATIELF_INPUT_SECONDARYCOLOR,     ///< secondary color
    ATIELF_INPUT_TEXCOORD,           ///< texture coordinate
    ATIELF_INPUT_TEXID,              ///< texture unit id
    ATIELF_INPUT_BUFFERID,           ///< buffer unit id
    ATIELF_INPUT_CONSTANTBUFFERID,   ///< constant buffer unit id
    ATIELF_INPUT_TEXTURERESOURCEID,  ///< texture resource id
} AtiElfInputType;


///
/// Input swizzles
///
typedef enum AtiElfSwizzleTypeEnum
{
    ATIELF_SWIZZLE_X,             ///< Swizzle the X component into this component
    ATIELF_SWIZZLE_Y,             ///< Swizzle the Y component into this component
    ATIELF_SWIZZLE_Z,             ///< Swizzle the Z component into this component
    ATIELF_SWIZZLE_W,             ///< Swizzle the W component into this component
    ATIELF_SWIZZLE_0,             ///< Swizzle constant 0 into this component
    ATIELF_SWIZZLE_1              ///< Swizzle constant 1 into this component
} AtiElfSwizzleType;


//
///  Input Descriptor
//
struct AtiElfInput
{
    AtiElfInputType    type;         ///< Type of the input
    unsigned int       vofs;         ///< virtual offset of the input
    unsigned int       pofs;         ///< physical offset of the input
    bool               isFloat16;    ///< input data type is float16
    AtiElfSwizzleType  swizzles[4];  ///< Swizzle for each component of the input
    AtiElfInput() :
        type(ATIELF_INPUT_ATTRIB),
        vofs(0),
        pofs(0),
        isFloat16(false)
    {
        swizzles[0] = ATIELF_SWIZZLE_X;
        swizzles[1] = ATIELF_SWIZZLE_Y;
        swizzles[2] = ATIELF_SWIZZLE_Z;
        swizzles[3] = ATIELF_SWIZZLE_W;
    }
};





//
/// Symbol data precision qualifier
//
enum AtiElfSymbolDataPrecisionQualifier
{
    ATIELF_SYMBOL_DATAPQ_LOW,
    ATIELF_SYMBOL_DATAPQ_MEDIUM,
    ATIELF_SYMBOL_DATAPQ_HIGH,
    ATIELF_SYMBOL_DATAPQ_DOUBLE
};

//
/// Symbol types
//
enum AtiElfSymbolType
{
    ATIELF_SYMBOL_UNIFORM,                      ///< GLSL uniform
    ATIELF_SYMBOL_BINDABLE_UNIFORM,             ///< GLSL bindable uniform
    ATIELF_SYMBOL_UNIFORM_BLOCK,                ///< GLSL uniform block
    ATIELF_SYMBOL_ATTRIB_POSITION,              ///< Vertex position
    ATIELF_SYMBOL_ATTRIB_NORMAL,                ///< Vertex normal
    ATIELF_SYMBOL_ATTRIB_PRIMARY_COLOR,         ///< Vertex primary color
    ATIELF_SYMBOL_ATTRIB_SECONDARY_COLOR,       ///< Vertex secondary color
    ATIELF_SYMBOL_ATTRIB_FOGCOORD,              ///< Vertex fog coord
    ATIELF_SYMBOL_ATTRIB_EDGEFLAG,              ///< Vertex edge flag
    ATIELF_SYMBOL_ATTRIB_TEXCOORD0,             ///< Vertex texture coord 0
    ATIELF_SYMBOL_ATTRIB_TEXCOORD1,             ///< Vertex texture coord 1
    ATIELF_SYMBOL_ATTRIB_TEXCOORD2,             ///< Vertex texture coord 2
    ATIELF_SYMBOL_ATTRIB_TEXCOORD3,             ///< Vertex texture coord 3
    ATIELF_SYMBOL_ATTRIB_TEXCOORD4,             ///< Vertex texture coord 4
    ATIELF_SYMBOL_ATTRIB_TEXCOORD5,             ///< Vertex texture coord 5
    ATIELF_SYMBOL_ATTRIB_TEXCOORD6,             ///< Vertex texture coord 6
    ATIELF_SYMBOL_ATTRIB_TEXCOORD7,             ///< Vertex texture coord 7
    ATIELF_SYMBOL_ATTRIB_TEXCOORD8,             ///< Vertex texture coord 8
    ATIELF_SYMBOL_ATTRIB_TEXCOORD9,             ///< Vertex texture coord 9
    ATIELF_SYMBOL_ATTRIB_TEXCOORD10,            ///< Vertex texture coord 10
    ATIELF_SYMBOL_ATTRIB_TEXCOORD11,            ///< Vertex texture coord 11
    ATIELF_SYMBOL_ATTRIB_TEXCOORD12,            ///< Vertex texture coord 12
    ATIELF_SYMBOL_ATTRIB_TEXCOORD13,            ///< Vertex texture coord 13
    ATIELF_SYMBOL_ATTRIB_TEXCOORD14,            ///< Vertex texture coord 14
    ATIELF_SYMBOL_ATTRIB_TEXCOORD15,            ///< Vertex texture coord 15
    ATIELF_SYMBOL_ATTRIB_GENERIC0,              ///< Generic attrib 0
    ATIELF_SYMBOL_ATTRIB_GENERIC1,              ///< Generic attrib 1
    ATIELF_SYMBOL_ATTRIB_GENERIC2,              ///< Generic attrib 2
    ATIELF_SYMBOL_ATTRIB_GENERIC3,              ///< Generic attrib 3
    ATIELF_SYMBOL_ATTRIB_GENERIC4,              ///< Generic attrib 4
    ATIELF_SYMBOL_ATTRIB_GENERIC5,              ///< Generic attrib 5
    ATIELF_SYMBOL_ATTRIB_GENERIC6,              ///< Generic attrib 6
    ATIELF_SYMBOL_ATTRIB_GENERIC7,              ///< Generic attrib 7
    ATIELF_SYMBOL_ATTRIB_GENERIC8,              ///< Generic attrib 8
    ATIELF_SYMBOL_ATTRIB_GENERIC9,              ///< Generic attrib 9
    ATIELF_SYMBOL_ATTRIB_GENERIC10,             ///< Generic attrib 10
    ATIELF_SYMBOL_ATTRIB_GENERIC11,             ///< Generic attrib 11
    ATIELF_SYMBOL_ATTRIB_GENERIC12,             ///< Generic attrib 12
    ATIELF_SYMBOL_ATTRIB_GENERIC13,             ///< Generic attrib 13
    ATIELF_SYMBOL_ATTRIB_GENERIC14,             ///< Generic attrib 14
    ATIELF_SYMBOL_ATTRIB_GENERIC15,             ///< Generic attrib 15
    ATIELF_SYMBOL_ATTRIB_GENERIC16,             ///< Generic attrib 16
    ATIELF_SYMBOL_ATTRIB_GENERIC17,             ///< Generic attrib 17
    ATIELF_SYMBOL_ATTRIB_GENERIC18,             ///< Generic attrib 18
    ATIELF_SYMBOL_ATTRIB_GENERIC19,             ///< Generic attrib 19
    ATIELF_SYMBOL_ATTRIB_GENERIC20,             ///< Generic attrib 20
    ATIELF_SYMBOL_ATTRIB_GENERIC21,             ///< Generic attrib 21
    ATIELF_SYMBOL_ATTRIB_GENERIC22,             ///< Generic attrib 22
    ATIELF_SYMBOL_ATTRIB_GENERIC23,             ///< Generic attrib 23
    ATIELF_SYMBOL_ATTRIB_GENERIC24,             ///< Generic attrib 24
    ATIELF_SYMBOL_ATTRIB_GENERIC25,             ///< Generic attrib 25
    ATIELF_SYMBOL_ATTRIB_GENERIC26,             ///< Generic attrib 26
    ATIELF_SYMBOL_ATTRIB_GENERIC27,             ///< Generic attrib 27
    ATIELF_SYMBOL_ATTRIB_GENERIC28,             ///< Generic attrib 28
    ATIELF_SYMBOL_ATTRIB_GENERIC29,             ///< Generic attrib 29
    ATIELF_SYMBOL_ATTRIB_GENERIC30,             ///< Generic attrib 30
    ATIELF_SYMBOL_ATTRIB_GENERIC31,             ///< Generic attrib 31
    ATIELF_SYMBOL_ATTRIB_GENERIC32,             ///< Generic attrib 32
    ATIELF_SYMBOL_ATTRIB_GENERIC33,             ///< Generic attrib 33
    ATIELF_SYMBOL_ATTRIB_GENERIC34,             ///< Generic attrib 34
    ATIELF_SYMBOL_ATTRIB_GENERIC35,             ///< Generic attrib 35
    ATIELF_SYMBOL_ATTRIB_GENERIC36,             ///< Generic attrib 36
    ATIELF_SYMBOL_ATTRIB_GENERIC37,             ///< Generic attrib 37
    ATIELF_SYMBOL_ATTRIB_GENERIC38,             ///< Generic attrib 38
    ATIELF_SYMBOL_ATTRIB_GENERIC39,             ///< Generic attrib 39
    ATIELF_SYMBOL_ATTRIB_VERTEXID,              ///< VertexID
    ATIELF_SYMBOL_ATTRIB_INSTANCEID,            ///< InstanceID
    ATIELF_SYMBOL_ATTRIB_DRAWID,                ///< DrawID
    ATIELF_SYMBOL_ATTRIB_BASE_VERTEX,           ///< Base vertex
    ATIELF_SYMBOL_ATTRIB_BASE_INSTANCE,         ///< Base instance
    ATIELF_SYMBOL_FSVARYINGOUT,                 ///< Varying out
    ATIELF_SYMBOL_SUBROUTINE_UNIFORM,           ///< GLSL Subroutine Uniform
    ATIELF_SYMBOL_SUBROUTINE_FUNCTION,          ///< GLSL Subroutine Function
    ATIELF_SYMBOL_SEPARATE_VARYING_IN_BASE,     ///< base varying in symbol of separate mode
    ATIELF_SYMBOL_SEPARATE_VARYING_OUT_BASE,    ///< base varying out symbol of sparate mode
    ATIELF_SYMBOL_SEPARATE_VARYING_IN_DERIVED,  ///< derived varying in symbol of separate mode
    ATIELF_SYMBOL_SEPARATE_VARYING_OUT_DERIVED, ///< derived varying out symbol of sparate mode
    ATIELF_SYMBOL_NONSEPARATE_VARYING_IN,       ///< varying in of nonseparate mode
    ATIELF_SYMBOL_NONSEPARATE_VARYING_OUT,      ///< varying out of nonseparate mode
    ATIELF_SYMBOL_SHADER_STORAGE_BUFFER,        ///< shader storage buffer
    ATIELF_SYMBOL_ATTRIB_INPUTVALID,            ///< input valide bits for sparse texture
    ATIELF_SYMBOL_UNKNOWN = -1,                 ///< invalid symbol type
    ATIELF_SYMBOL_ATTRIB_FIRST = ATIELF_SYMBOL_ATTRIB_POSITION, ///< The start of vertex attribute range
    ATIELF_SYMBOL_ATTRIB_LAST = ATIELF_SYMBOL_ATTRIB_BASE_INSTANCE, ///< The end of vertex attribute range
};

//
/// Style for layout qualifier
//
typedef enum
{
    ATI_ELF_LAYOUT_STYLE_NONE,
    ATI_ELF_LAYOUT_SHARED,
    ATI_ELF_LAYOUT_PACKED,
    ATI_ELF_LAYOUT_STD140,
} AtiElfLayoutStyleQualifier;


/// Major for layout qualifier
//
typedef enum
{
    ATI_ELF_LAYOUT_MAJOR_NONE,
    ATI_ELF_LAYOUT_ROW_MAJOR,
    ATI_ELF_LAYOUT_COLUMN_MAJOR,
} AtiElfLayoutMajorQualifier;

//
/// invariant qualifier
///
typedef enum
{
    ATI_ELF_INVARIANT_OFF,
    ATI_ELF_INVARIANT_ON,
} AtiElfInvariantQualifier;

//
/// precise qualifier
//
typedef enum
{
    ATI_ELF_PRECISE_OFF,
    ATI_ELF_PRECISE_ON,
} AtiElfPreciseQualifier;

//
/// storage qualifier
///
typedef enum
{
    ATI_ELF_STORAGE_DEFAULT,
    ATI_ELF_STORAGE_CENTROID,
    ATI_ELF_STORAGE_SAMPLE,
} AtiElfStorageQualifier;

//
/// interpolation qualifier
///
typedef enum
{
    ATI_ELF_INTERPOLATION_SMOOTH,
    ATI_ELF_INTERPOLATION_FLAT,
    ATI_ELF_INTERPOLATION_NOPERSPECTIVE,
} AtiElfInterpolationQualifier;

//
/// patch qualifier
//
typedef enum
{
    ATI_ELF_PER_PATCH_OFF,
    ATI_ELF_PER_PATCH_ON,
} AtiElfPatchQualifier;

//
/// float16 format qualifier
//
typedef enum
{
    ATI_ELF_IEEE_NONE,
    ATI_ELF_IEEE_FLOAT32,
    ATI_ELF_IEEE_FLOAT16,
} AtiElfFloat16FormatQualifier;

//
/// sampler/image bind mode qualifier
//
typedef enum
{
    ATI_ELF_LAYOUT_BOUND,
    ATI_ELF_LAYOUT_BINDLESS,
} AtiElfLayoutBindModeQualifier;

//
/// Swizzle types
//
typedef enum {ATI_ELF_SWIZZLE_X,
              ATI_ELF_SWIZZLE_Y,
              ATI_ELF_SWIZZLE_Z,
              ATI_ELF_SWIZZLE_W,
              ATI_ELF_SWIZZLE_UNKNOWN
             } AtiElfSwizzle;

//
/// per stage value of interface symbol
//
struct AtiElfSymbolStageData
{
    int value;                  ///< symbol value, uniform/bindable uniform: reg offset, unit: vec4
    ///                block member:  block offset in a ubo or ssbo, unit: byte
    ///                opaque type: physical slot
    int cbindex;                ///< constant buffer index
    AtiElfSwizzle swizzle;      ///< destination swizzle
};

//
/// per dimension array information descriptor for AtiElfSymbol
//
struct AtiElfSymbolArrayInfo
{
    int arraySize;                  ///< array size
    int arrayMinIndex;              ///< min index used in array
    int offsetStride;               ///< linkid stride for each array element, unit: byte
    int memberStride;               ///< member stride for each array element, it is used to caculate location
};

struct AtiElfIntegerConstant
{
    AtiElfConstantType type;            ///< type of the constant
    uint32             vofs;            ///< virtual offset of the constant
    AtiElfSwizzle      vofs_swizzle;    ///< which component of the vofs to use
    uint32             pofs;            ///< physical offset of the constant
    uint32             value;        ///< value of the constant (if literal, 0,0,0,0 otherwise)
    AtiElfIntegerConstant() :
        type(ATIELF_CONST_LOCAL),
        vofs(0),
        vofs_swizzle(ATI_ELF_SWIZZLE_X),
        pofs(0)
    {
        value = 0;

    }
};

//
///  Constant Descriptor
//
struct AtiElfConstant
{
    AtiElfConstantType type;     ///< type of the constant
    unsigned int       vofs;     ///< virtual offset of the constant
    unsigned int       pofs;     ///< physical offset of the constant
    float              value[4]; ///< value of the constant (if literal, 0,0,0,0 otherwise)
    AtiElfConstant() :
        type(ATIELF_CONST_LOCAL),
        vofs(0),
        pofs(0)
    {
        value[0] = 0.f;
        value[1] = 0.f;
        value[2] = 0.f;
        value[3] = 1.f;
    }
};

#ifdef _WINDOWS
    #define ALIGNED(X,Y) __declspec(align(Y)) X
#else
    #define ALIGNED(X,Y)  X __attribute__ ((aligned(Y)))
#endif

struct AtiElfSymbol
{
    uint32               structSize; ///< The total size of this structure, including variable strings
    AtiElfSymbolType     type;       ///< Uniform, normal, texcoord, generic attrib, etc.
    AtiElfSymbolDataType dataType;   ///< float, vec2, mat4, etc.
    //
    /// union of qualifier struct
    //
    union
    {
        //
        /// qualifier struct
        //
        struct
        {
            AtiElfSymbolDataPrecisionQualifier      dataPrecQual            : 4;    ///< low, medium, high, double
            AtiElfLayoutStyleQualifier              layoutStyleQualifier    : 4;    ///< Style of layout qualifier
            AtiElfLayoutMajorQualifier              layoutMajorQualifier    : 4;    ///< Major of layout qualifier
            AtiElfInvariantQualifier                invariantQualifier      : 2;    ///< invariant qualifier
            AtiElfPreciseQualifier                  preciseQualifier        : 2;    ///< precise qualifier
            AtiElfStorageQualifier                  storageQualifier        : 3;    ///< storage qualifier
            AtiElfInterpolationQualifier            interpQualifier         : 3;    ///< interpolation qualifier
            AtiElfPatchQualifier                    patchQualifier          : 2;    ///< patch qualifier
            AtiElfFloat16FormatQualifier            float16FormatQualifier  : 3;    ///< float16 format qualifier
            AtiElfLayoutBindModeQualifier           layoutBindModeQualifier : 2;    ///< bind mode qualifier
        } AtiElfStruct;
        uint32  qualifier;                                                          ///< qualifier
    };

    AtiElfSymbolStageData stage[6]; ///< per stage values
    int                  size;     ///< register_count * 4 * sizeof(float), no packing, with padding. This is for bindable uniform (GetUniformBufferSizeEXT)
    int                  count;    ///< if this symbol is part of an array, the number of remaining elements (including this one), 1 otherwise
    ///< If anything is put between count and name, code in Uniform/VertexInterface ScanSymbols needs to be updated
    bool                 isActive; ///< TRUE if the symbol is active
    bool                 isArray;      ///< TRUE if the symbol is arrayed, FALSE otherwise
    uint32               matrixStride; ///< stride of columns of column major matrix or rows of row major matrix
    uint32               subSize;      ///< the number of subroutine functions.
    uint32               uniformBinding;  ///< Binding (versatile usages for uniform block, sampler and image, atomic counter, array buffer)
    int16                layoutLocation;  ///< layout location (versatile usages for uniform variables, including opaque types, and subroutine uniforms)
    int16                layoutIndex;     ///< layout Index for subroutine function
    uint32               uniformOffset;   ///< Offset (currently used for atomic counter)
    uint32               resourceIndex;   ///< Resource index (currently for storing GDS offset for atomic counter in DWs)

    uint32               subroutineTypeID; ///< subroutine type id

    int32                arrayInfoSize;           ///< the size of arrayInfo array
    int32                arrayDim;                ///< array dimension of this symbol, arrayDim is 0 if it isn't an array
    bool                 hasTopLevelArray;        ///< true if this symbol is a part of symbol which has top level array
    bool                 isUnsizedTopLevelArray;  ///< true if the top level array of this symbol is unsized
    //ALIGNED(const char*  name, 8);                ///< NULL-terminated string which is the name of this symbol
    const char* name;
    const char*          baseName;                ///< if this symbol is part of an array, NULL-terminated string which is the unsubscripted name of the array
    const char*          uniformBlockName;        ///< Name of uniform block
    const char*          mangledName;             ///< available if this sysmbol is struct or interface block, it is used to check type.
    AtiElfSymbolArrayInfo* arrayInfo;             ///< per dimension array information
    AtiElfSymbol() :
        structSize(0),
        type(ATIELF_SYMBOL_ATTRIB_GENERIC0),
        dataType(ATIELF_SYMBOL_DATATYPE_VOID),
        size(0),
        count(0),
        isActive(false),
        isArray(false),
        matrixStride(0),
        subSize(0),
        uniformBinding(0),
        layoutLocation(-1),
        layoutIndex(-1),
        uniformOffset(0),
        resourceIndex(0),
        subroutineTypeID(0),
        arrayInfoSize(0),
        arrayDim(0),
        hasTopLevelArray(false),
        isUnsizedTopLevelArray(false),
        name(nullptr),
        baseName(nullptr),
        uniformBlockName(nullptr),
        mangledName(nullptr),
        arrayInfo(nullptr)
    {
        qualifier = 0;
        AtiElfStruct.dataPrecQual = ATIELF_SYMBOL_DATAPQ_HIGH;

        for (int i = 0; i < 6; i++)
        {
            stage[i].value = 0;
            stage[i].cbindex = -1;
            stage[i].swizzle = ATI_ELF_SWIZZLE_UNKNOWN;
        }
    }
};



//
/// texture resource and sampler binding
//
struct AtiElfTextureResourceBound
{
    uint32 resourceId;          ///< resource id
    uint32 samplerMask;         ///< samplers bind to resource id
};
//
/// the struct of ELF section "info", it is to store information which is needed by GSL module, and this struct is shared with CAL.
//


#define AtiElf_MAX_UAV      12      ///< mask uav supported
#define AtiElf_MAX_TRANSFORM_FEEDBACK_BUFFERS 4  ///< maximum number of transform feedback buffers supported

#define AtiElf_MAX_UAV_RESOURCES 1024 ///< The maximum number of UAVs supported
#define AtiElf_MAX_UAV_MASKS (((AtiElf_MAX_UAV_RESOURCES) + 31) / 32)   ///< The maximum of UAV masks


// for AtiProgramBinary
#define AtiElf_MAX_TEXTURE_IMAGE_UNITS        32 ///< The maximum number of texture image units supported by GLL
#define AtiElf_MAX_UAV_IMAGE_UNITS            8  ///< The maximum number of image (UAV) supported by GLL
#define AtiElf_NUM_GLOBAL_RETURN_BUFFERS      8  ///< max global return buffers
#define AtiElf_MAX_TEXTURE_RESOURCES          160 ///< The maximus number of texture resource
#define AtiElf_PS_MAX_OUTPUTS                 8   ///< Max Render Targets

class AtiElfProgramInfo
{
public:
    ///generic shader resource information

    //texture
    uint32 _fetch4Mask;                    ///< Fetch4 mask
    uint32 _textureMask;                   ///< Texture unit masks
    uint32 _textureSamplerMask;            ///< texture sampler mask
    uint32 _textureResourceMask[AtiElf_MAX_TEXTURE_RESOURCES / 32]; ///< texture resource mask
    uint32 _bufferMask[AtiElf_MAX_TEXTURE_RESOURCES / 32]; ///< texture buffer mask
    uint32 _textureResourceBoundCount;     ///< the size of texture resource bound array
    //scratch
    uint32 _maxScratchRegsNeeded;          ///< scratch registers needed
    //constant buffer
    uint32 _constantBufferMask;            ///< constant buffer mask
    //uav
    uint32 _uavOpIsUsed;                   ///< mask for uav used delete!!
    bool   _uavInCB;                       ///< UAV is in CB
    uint32 _uavResourceCount;              ///< size of uav resource mask array
    //atomic counter
    uint32 _uavAtomicOpIsUsed;             ///< mask for atomic op used
    //bindless texture
    bool   _bindlessResourceIsUsed;        ///< flag for bindless resource usage
    //subroutine
    int16  maxUniformLocation;             ///< max explicit uniform location assigned by application
    int16  maxSubroutineIndex;             ///< max explicit Subroutine index assigned by application
    int16  maxSubroutineUniformLocation;   ///< max explicit Subroutine uniform location assigned by application
    /// union for per shader stage parameters
    union
    {
        /// Vexter Shader,  Tessellation Evaluation Shader and Geometry Shader parameter
        struct
        {
            // VS input mask
            uint32 _inputStreamMask;                ///< input stream mask (phsyical id)
            bool   _usesVertexID;                   ///< tells whether this program uses VertexID
            bool   _usesBaseVertex;                 ///< tells whether this program uses BaseVertex
            bool   _usesBaseInstance;               ///< tells whether this program uses BaseInstance
            // transform feedback
            uint32 _streamOutStrideInDWORDs0;       ///< streamout stride0
            uint32 _streamOutStrideInDWORDs1;       ///< streamout stride1
            uint32 _streamOutStrideInDWORDs2;       ///< streamout stride2
            uint32 _streamOutStrideInDWORDs3;       ///< streamout stride3
            int8   _streamOutBufferMapping[AtiElf_MAX_TRANSFORM_FEEDBACK_BUFFERS];   ///< streamout buffer config
            // vertex shader tessellation
            uint8  _tessPrimType;                            ///< tessellation shader primitive type (sclTessPrimType)
            // viewport array index
            bool   _outputViewportArrayIndex : 8;            ///< true if output viewport array index
            // svp members
            uint8  _frontColorOutputReg;                     ///< front color output register number
            uint8  _frontSecondaryColorOutputReg;            ///< front secondary color output register number
            uint8  _backColorOutputReg;                      ///< back color output register number
            uint8  _backSecondaryColorOutputReg;             ///< back secondary color output register number
            uint8  _aaStippleTexCoord;                       ///< Bitfield representing which texture cood will be used for aastipple patch
            //AtiElfFetchShaderType _fetchTypeForPassThruVS: 8;///< Fetch shader type (SVP PassThruVS)
            uint8  _fetchReturnAddrRegForPassThruVS;         ///< Fetch shader subroutine start SGPR (SVP PassThruVS)
            uint8  _fetchInputStreamTableRegForPassThruVS;   ///< Fetch shader input stream table start SGPR (SVP PassThruVS)
            int32  _fetchAttribValidMaskReg;                 ///< VPGR which Fetch shader should populate, if sparse buffers are used.
        } VertexShaderStruct;
        /// Fragment Shader Parameters
        struct
        {
            uint32 _texKillPresent;                         ///< Program uses texkill
            int32  _pointCoordReg;                          ///< register number of gl_PointCoord which is an input of FS
            uint8  _outputColorMap[AtiElf_PS_MAX_OUTPUTS];  ///< fragment shader color output map (from virtual to physical)
            bool   _useFlatInterpMode;                      ///< if flat has been used on a varying
            bool   _forcePerSampleShading;                  ///< true if the FS is required to run in per sample frequency
            bool   _uncached_writes;                        ///< uncached writes
            bool   _outputDepth;                            ///< true if fragment shader output depth
            uint32 _usingDualBlendMask;                     ///< indicates using an index = 1 for dual blending, in glsl layout
        } FragmentShaderStruct;
        /// Compute Shader Parameters
        struct
        {
            uint32 _numSharedGPRUser;    ///< shared GPR
            uint32 _numSharedGPRTotal;   ///< shared GPR total including ones used by SC.

            uint32 _numThreadPerGroup;   ///< threads per group
            uint32 _numThreadPerGroupX;  ///< dimension x of NumThreadPerGroup
            uint32 _numThreadPerGroupY;  ///< dimension y of NumThreadPerGroup
            uint32 _numThreadPerGroupZ;  ///< dimension z of NumThreadPerGroup
            uint32 _totalNumThreadGroup; ///< total number of thread groups
            uint32 _NumWavefrontPerSIMD; ///< wavefronts per simd
            bool   _eCsSetupMode;        ///< compute slow/fast mode
            bool   _IsMaxNumWavePerSIMD; ///< Is this the max active num wavefronts per simd
            bool   _SetBufferForNumGroup;///< Need to set up buffer for info on number of thread groups?
        } ComputeShaderStruct;
        /// Fetch Shader Parameters
        struct
        {
            bool   _usesVertexCache;      ///< vertex cache used? (fetch shader only)
        } FetchShaderStruct;
    };
    ///dynamic array, offset fields is valid in ELF package, int64 is to keep the struct size fixed in all operation system.
    /// texture resource bound array
    union
    {
        AtiElfTextureResourceBound* _textureResourceBound; ///< texture resoruce and sampler bounding
        int64 _textureResourceBoundOffset;                 ///< resource binding array offset
    };
    /// uav resource mask array
    union
    {
        uint32* _uavResourceMask;                          ///< UAV resource mask
        int64   _uavResourceMaskOffset;                    ///< UAV resource mask array offset
    };
    /// uav return buffer
    union
    {
        uint32* _uavRtnBufStride;       ///< save stride of uav return buffer for each UAV
        int64   _uavRtnBufStrideOffset;                    ///< uav return buffer stride array offset
    };
    /// uav dynamic resource map
    union
    {
        uint32* _uavDynamicResouceMap;  ///< save fetch const offset of each UAV
        int64   _uavDynamicResouceMapOffset;               ///< uav dynamic resource map offset
    };

    enum { MAX_PROGRAMINFO_OFFSET = 0x0ffff };             ///< max valid value of offset
    ///
    AtiElfProgramInfo()
    {
        memset(this, 0, sizeof(AtiElfProgramInfo));
    }
    ///
    ~AtiElfProgramInfo()
    {
        Destroy();

    }
    /// destroy ati elf program info object
    void Destroy()
    {
        // free dynamic array
        if (_uavRtnBufStrideOffset > MAX_PROGRAMINFO_OFFSET)
        {
            delete[] _uavRtnBufStride;
            _uavRtnBufStride = nullptr;
        }

        if (_uavDynamicResouceMapOffset > MAX_PROGRAMINFO_OFFSET)
        {
            delete[] _uavDynamicResouceMap;
            _uavDynamicResouceMap = nullptr;
        }

        if (_textureResourceBoundOffset > MAX_PROGRAMINFO_OFFSET)
        {
            delete[] _textureResourceBound;
        }

        if (_uavResourceMaskOffset > MAX_PROGRAMINFO_OFFSET)
        {
            delete[] _uavResourceMask;
            _uavResourceMask = nullptr;
        }
    }
    /// copy ati elf program info object
    AtiElfProgramInfo& operator=(const AtiElfProgramInfo& rhs)
    {
        Destroy();
        memcpy(this, &rhs, sizeof(AtiElfProgramInfo));

        //copy dynamic array

        if (_uavOpIsUsed && _uavInCB)
        {
            if (rhs._uavRtnBufStrideOffset)
            {
                _uavRtnBufStride = new uint32[AtiElf_MAX_UAV];

                if (rhs._uavRtnBufStrideOffset > MAX_PROGRAMINFO_OFFSET)
                {
                    memcpy(_uavRtnBufStride, rhs._uavRtnBufStride, sizeof(uint32)*AtiElf_MAX_UAV);
                }
                else
                {
                    memcpy(_uavRtnBufStride, (uint32*)((uintp)&rhs + (uintp)rhs._uavRtnBufStrideOffset), sizeof(uint32)*AtiElf_MAX_UAV);
                }
            }

            if (rhs._uavDynamicResouceMapOffset)
            {

                _uavDynamicResouceMap = new uint32[AtiElf_MAX_UAV];

                if (rhs._uavDynamicResouceMapOffset > MAX_PROGRAMINFO_OFFSET)
                {
                    memcpy(_uavDynamicResouceMap, rhs._uavDynamicResouceMap, sizeof(uint32)*AtiElf_MAX_UAV);
                }
                else
                {
                    memcpy(_uavDynamicResouceMap, (uint32*)((uintp)&rhs + (uintp)rhs._uavDynamicResouceMapOffset), sizeof(uint32)*AtiElf_MAX_UAV);
                }
            }
        }
        else
        {
            _uavRtnBufStride = nullptr;
            _uavDynamicResouceMap = nullptr;

        }

        if (_textureResourceBoundCount)
        {
            //assert(_textureResourceBoundOffset);
            _textureResourceBound = new AtiElfTextureResourceBound[_textureResourceBoundCount];

            if (rhs._textureResourceBoundOffset > MAX_PROGRAMINFO_OFFSET)
            {
                memcpy(_textureResourceBound, rhs._textureResourceBound, sizeof(AtiElfTextureResourceBound)* _textureResourceBoundCount);
            }
            else
            {
                memcpy(_textureResourceBound, (AtiElfTextureResourceBound*)((uintp)&rhs + (uintp)rhs._textureResourceBoundOffset), sizeof(AtiElfTextureResourceBound)* _textureResourceBoundCount);
            }
        }
        else
        {
            _textureResourceBound = nullptr;
        }

        if (_uavResourceCount)
        {
            //assert(_uavResourceMaskOffset);
            _uavResourceMask = new uint32[_uavResourceCount];

            if (rhs._uavResourceMaskOffset > MAX_PROGRAMINFO_OFFSET)
            {
                memcpy(_uavResourceMask, rhs._uavResourceMask, sizeof(uint32)* _uavResourceCount);
            }
            else
            {
                memcpy(_uavResourceMask, (uint32*)((uintp)&rhs + (uintp)rhs._uavResourceMaskOffset), sizeof(uint32)* _uavResourceCount);
            }
        }
        else
        {
            _uavResourceMask = nullptr;
        }

        return *this;
    }

};


//
/// ARB program parameter
//
struct AtiElfARBProgramParameter
{
    uint32 _instructions;              ///< Number of instructions
    uint32 _nativeInstructions;        ///< Number of native instructions
    uint32 _aluInstructions;           ///< Number of alu instructions
    uint32 _nativeAluInstructions;     ///< Number of native alu instructions
    uint32 _texInstructions;           ///< Number of tex instructions
    uint32 _nativeTexInstructions;     ///< Number of native tex instructions
    uint32 _texIndirections;           ///< Number of tex indirections
    uint32 _nativeTexIndirections;     ///< Number of native tex indirections
    uint32 _temporaries;               ///< Number of temporaries
    uint32 _nativeTemporaries;         ///< Number of native temporaries
    uint32 _parameters;                ///< Number of parameters
    uint32 _nativeParameters;          ///< Number of native parameters
    uint32 _attribs;                   ///< Number of attribs
    uint32 _nativeAttribs;             ///< Number of native attribs
    uint32 _addressRegisters;          ///< Number of address registers
    uint32 _nativeAddressRegisters;    ///< Number of native address registers
    bool   _underNativeLimits;         ///< Is shader under native limits?
};

//
/// This Structure collects information on which shader keys need to be tracked for the compiled/linked shader, Right now limited to L4D2 perf cases
//
struct AtiElfShaderKeysInfo
{
    bool  needPatchClipDistance;        ///< Re-link shader because it outputs clip vertex or position invariant?
};

//
/// the struct of ELF section "usageinfo", It is used by GLL module.
///
class AtiElfProgramUsageInfo
{
public:
    AtiElfARBProgramParameter   _arbProgramParameter;   ///< ARB program parameters
    AtiElfShaderKeysInfo        _shaderKeysInfo;        ///< Shader keys patching information
    uint16 _layoutParamMask;                            ///< layout parameters mask, see type gllShaderLayoutType for detail info
    bool _usesPrimitiveID;                              ///< Does this program use PrimitiveID
    bool _usesClipDistance;                             ///< vp outputs clip distance(s)

    uint32 _texImageMask;                               ///< orignial teximage unit usage mask (without SC optimization and it isn't include patched texture stage)
    uint32 _texInternalResourceMask;                    ///< texture resource mask that is internally only used for fMask MSAA texturing optimization.
    uint32 _usesTexImageMask;                           ///< teximge unit usage returned by SC
    uint32 _usesShadowMask;                             ///< shadow texture unit usage mask
    uint32 _uavImageMask;                               ///< original uav image usage mask
    uint32 _uavImageMaskShaderStorageBuffer;            ///< uav used by shader storage buffer
    uint32 _usesBufMask;                                ///< vertex tessellation buffer mask

    uint32 _imageTypeAndFormatSize;                     ///< size of array _imageType and _imageFormat
    uint32 _textureTypeSize;                            ///< size of array _textureType

    //
    /// per shader stage parameter
    //
    union
    {
        //
        /// Fragment Shader Parameters
        //
        struct
        {
            ///< fs input usage
            uint32 _usesTexCoordMask;                   ///< texcoord unit usage mask
            bool   _usesFogCoord;                       ///< fogcoord usage
            bool   _usePrimaryColor;                    ///< true if primary color is used
            bool   _useSecondaryColor;                  ///< true if secondary color is used
            ///< aa stipple
            int8   _aaStippleTexCoord;                  ///< the texture coord used by SCL to simulate aa/stipple
            int8   _aaStippleTexUnit;                   ///< the texture unit used by aa/stipple texture image
            int8   _aaStippleTexVirtUnit;               ///< default logic unit for aa stipple texture unit
            ///< bitmap
            int8   _bitmapTexCoord;                     ///< the texture coord used for bitmap drawing
            int8   _bitmapTexUnit;                      ///< the texture unit used for bitmap drawing
            int8   _bitmapTexVirtUnit;                  ///< default logic unit for bitmap drawing
            ///< misc fields
            bool   _needSampleInfoCBs;                  ///< whether the FP needs the 2 constant buffers for the SAMPLEINFO, SAMPLEPOS and EVAL_SAMPLE_INDEX.
            bool   _earlyFragTest;                      ///< true if early frag test is enabled
            uint8  _conservativeZ;                      ///< 0:defult, 1:less, 2:greater
        } FragmentShaderParamStruct;
        //
        /// Geometry Shader Parametes
        //
        struct
        {
            uint16 _maxGSVertices[4];                   ///< max gs output vertices
            uint16 _gsInvocations;                      ///< gs invocation number
            uint8  _inputTopology;                      ///< gs input topology
            uint8  _outputTopology[4];                  ///< gs output topology
        } GeometryShaderParamStruct;
        //
        /// Tessellation Evaluation Shader Parameters
        //
        struct
        {
            uint32 _tessOutputVertices;                 ///< output control point number
        } TessellationEvalShaderParamStruct;
        //
        /// Tessellation Control Shader Parameters
        //
        struct
        {
            uint32 _tessGenMode;                        ///< domain mode
            uint32 _tessGenSpacing;                     ///< partition mode
            uint32 _tessGenVertexOrder;                 ///< output primitive mode
            bool   _tessGenPointMode;                   ///< If point mode when tessellated
        } TessellationControlShaderParamStruct;
        //
        /// Vertex Shader Parameters
        //
        struct
        {
            bool   _positionInvariant;                  ///< Has the ARB_position_invariant option been requested
            bool   _enableAnotherSetAttribAlias;        ///< if it is cg generated program or nv_vertex_program
            uint32 _lsStride;                           ///< The number of input-control-points per patch declared in HS
            bool   _usesDrawID;                         ///< Is draw id used in this shader?
            ///< SI+ fetch shader parameters
            int8   _fetchType;                          ///< Fetch shader type (immediate vs flat), SI only
            int8   _fetchReturnAddrReg;                 ///< Fetch shader subroutine return address SGPR, SI only
            int8   _fetchInputStreamTableReg;           ///< Fetch shader input stream table start SGPR (either first data element or pointer depending on FS type), SI only
            int8   _fetchVertexIdChannel;               ///< Fetch shader channel(R,G,B,A) to compute the vertexID with
            int8   _fetchBaseVertexReg;                 ///< Fetch shader base vertex SGPR, SI only
            int8   _fetchBaseInstanceReg;               ///< Fetch shader base instance SGPR, SI only
        } VertexShaderParamStruct;
        //
        ///< Compute Shader Parameters
        //
        struct
        {
            uint32  _workDimension;                     ///< Work dimension (1, 2 or 3 right now)
            uint32  _workSizeX;                         ///< Work size in the X dimension
            uint32  _workSizeY;                         ///< Work size in the Y dimension
            uint32  _workSizeZ;                         ///< Work size in the Z dimension
        } ComputeShaderParamStruct;

    };
    enum { MAX_PROGRAMINFO_OFFSET = 0x0ffff };         ///< max valid value for dynamic array offset
    //
    /// texture type array
    //
    union
    {
        uint32* _textureType;                         ///< teximage unit usage
        int64   _textureTypeOffset;                   ///< texture type array offset in elf section
    };
    //
    /// uav image type array
    //
    union
    {
        uint32* _imageType;                            ///< UAV image usage
        int64   _imageTypeOffset;                      ///< uav image type array offset in elf section
    };
    //
    /// uav image format array
    //
    union
    {
        uint32* _imageFormat;                          ///< uav image format array
        int64   _imageFormatOffset;                    ///< uav image format array offset in elf section
    };
    ///
    AtiElfProgramUsageInfo()
    {
        memset(this, 0, sizeof(AtiElfProgramUsageInfo));
    }
    /// destroy program usage info
    void Destroy()
    {
        if (_textureTypeOffset > MAX_PROGRAMINFO_OFFSET)
        {
            delete[] _textureType;
            _textureType = nullptr;
        }

        if (_imageTypeOffset > MAX_PROGRAMINFO_OFFSET)
        {
            delete[] _imageType;
            _imageType = nullptr;
        };

        if (_imageFormatOffset > MAX_PROGRAMINFO_OFFSET)
        {
            delete[] _imageFormat;
            _imageFormat = nullptr;
        }
    }
    ~AtiElfProgramUsageInfo()
    {
        Destroy();

    }
};


#define AtiElf_MAX_NUM_SAMPLER  32
#define AtiElf_MAX_NUM_RESOURCE 256
#define AtiElf_MAX_NUM_UAV      1024
#define AtiElf_MAX_CONSTANTS    256

struct AtiElf_SC_SHADERSTATS
{
    uint32   uNumInst;
    uint32   uNumTempReg;
    uint32   uNumClauseTempReg;
    uint32   uGPRPoolSize;
    uint32   uNumFetchReg;
    uint32   uNumExportReg;
    uint32   uNumInterp;
    uint32   uLoopNestingDepth;
    uint32   uNumALUInst;
    uint32   uNumTfetchInst;
    uint32   uNumVfetchInst;
    uint32   uNumMemExportInst;
    uint32   uNumCflowInst;
    uint32   uNumBoolConstReg;
    uint32   uNumALUConstReg;
    uint32   uNumIntConstReg;
    uint32   uNumInternalALUConst;
    uint32   uNumInternalTfetch;
};

struct AtiElf_SC_CONSTANTUSAGE
{
    uint32 maskBits[(AtiElf_MAX_CONSTANTS + 31) / 32];
};


#define AtiElf_SC_HWSHADER_COMMON                                                   \
    uint32                  uSizeInBytes;      /* size of structure */                \
    AtiElf_SC_SHADERSTATS   stats;             /* hw-neutral stats */                 \
    uint32                  uShaderType;        /* IL shader type */                  \
    uint32                  eInstSet;          /* Instruction set */                  \
    uint32                  texResourceUsage[(AtiElf_MAX_NUM_RESOURCE + 31) / 32];    \
    uint32                  uavResourceUsage[(AtiElf_MAX_NUM_UAV + 31) / 32];         \
    uint32                  texSamplerUsage;                                          \
    uint32                  constBufUsage;





#endif
