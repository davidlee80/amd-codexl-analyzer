//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcCliStringConstants.h 
/// 
//=====================================================================
#ifndef __kcCliStringConstantsh_h
#define __kcCliStringConstantsh_h

// Family names.
#define KA_STR_familyNameSICards " (HD7000 / HD8000 series)"
#define KA_STR_familyNameCICards " (HD8000 / Rx 200 / 300 series)"
#define KA_STR_familyNameVICards " (Rx 200 / 300 / Fury series)"


const char* const STR_GPU_PERF_STUDIO_VERSION_PREFIX = "GPU PerfStudio Static Analyzer CLI Version: ";
const char* const STR_CXL_VERSION_PREFIX = "CodeXL Static Analyzer CLI Version ";
const char* const STR_DRIVER_VERSION = "Driver version: ";
const char* const STR_ERR_DRIVER_VERSION_EXTRACTION_FAILURE = "could not extract the driver version.";
const char* const STR_ERR_INITIALIZATION_FAILURE = "Failed to initialize.";
const char* const STR_ERR_OPENGL_VERSION_EXTRACTION_FAILURE = "Unable to extract the supported OpenGL version.\n";

// Errors.
const char* const STR_ERR_INVALID_GLSL_SHADER_TYPE = "Error: The Specified profile is invalid. Possible options: Vertex, Fragment, Compute, Geometry, TessControl, TessEval.";
const char* const STR_ERR_UNSUPPORTED_GL_RT_VERSION = "Unsupported OpenGL runtime version. Please install Catalyst 15.20 or above.";
const char* const STR_ERR_NO_VALID_CMD_DETECTED = "Error: No valid command. Please run -h for available commands.";
const char* const STR_ERR_CANNOT_EXTRACT_SUPPORTED_DEVICE_LIST = "Error: Unable to extract the list of supported devices.";
const char* const STR_ERR_MEMORY_ALLOC_FAILURE = "Error: memory allocation failure.";
const char* const STR_ERR_RENDER_COMPUTE_MIX = "Error: cannot mix compute and non-compute shaders.";
const char* const STR_ERR_CANNOT_FIND_SHADER_PREFIX = "Error: cannot find ";
const char* const STR_ERR_CANNOT_FIND_SHADER_SUFFIX = " shader: ";
const char* const STR_ERR_CANNOT_FIND_OUTPUT_DIR = "Error: output directory does not exist: ";
const char* const STR_ERR_CANNOT_INVOKE_COMPILER = "Error: unable to invoke the compiler.";
const char* const STR_ERR_CANNOT_GET_DEVICE_INFO = "Error: cannot get device info for: ";
const char* const STR_ERR_CANNOT_OPEN_FILE_FOR_WRITE_A = "Error: Unable to open ";
const char* const STR_ERR_CANNOT_OPEN_FILE_FOR_WRITE_B =  " for write.";
const char* const STR_ERR_CANNOT_LOCATE_LIVE_REG_ANALYZER = "Error: cannot locate the live register analyzer.";
const char* const STR_ERR_CANNOT_LAUNCH_LIVE_REG_ANALYZER = "Error: cannot launch the live register analyzer.";
const char* const STR_ERR_CANNOT_FIND_ISA_FILE = "Error: ISA file not found.";
const char* const STR_ERR_CANNOT_PERFORM_LIVE_REG_ANALYSIS = "Error: failed to perform live register analysis.";

// Warnings.
#define STR_WRN_DX_MIN_SUPPORTED_VERSION "Warning: AMD DirectX driver supports DX10 and above."

// Environment variables.
#define STR_OCL_ENV_VAR_GPU_FORCE_64BIT_PTR_NAME  L"GPU_FORCE_64BIT_PTR"
#define STR_OCL_ENV_VAR_GPU_FORCE_64BIT_PTR_VALUE L"1"

// CSV file.
#define STR_CSV_HEADER_DEVICE            "DEVICE"
#define STR_CSV_HEADER_SCRATCH_REGS      "SCRATCH_REGS"
#define STR_CSV_HEADER_THREADS_PER_WG    "THREADS_PER_WORKGROUP"
#define STR_CSV_HEADER_WAVEFRONT_SIZE    "WAVEFRONT_SIZE"
#define STR_CSV_HEADER_LDS_BYTES_MAX     "AVAILABLE_LDS_BYTES"
#define STR_CSV_HEADER_LDS_BYTES_ACTUAL  "USED_LDS_BYTES"
#define STR_CSV_HEADER_SGPR_AVAILABLE    "AVAILABLE_SGPRs"
#define STR_CSV_HEADER_SGPR_USED         "USED_SGPRs"
#define STR_CSV_HEADER_VGPR_AVAILABLE    "AVAILABLE_VGPRs"
#define STR_CSV_HEADER_VGPR_USED         "USED_VGPRs"
#define STR_CSV_HEADER_CL_WORKGROUP_DIM_X "CL_WORKGROUP_X_DIMENSION"
#define STR_CSV_HEADER_CL_WORKGROUP_DIM_Y "CL_WORKGROUP_Y_DIMENSION"
#define STR_CSV_HEADER_CL_WORKGROUP_DIM_Z "CL_WORKGROUP_Z_DIMENSION"
#define STR_CSV_HEADER_ISA_SIZE_BYTES     "ISA_SIZE"

// Build output.
// Status strings.
#define KA_CLI_STR_STATUS_SUCCESS "... succeeded."
#define KA_CLI_STR_STATUS_FAILURE "... failed."
#define KA_CLI_STR_ABORTING "Aborting."
#define KA_CLI_STR_COMPILING "Building for "
#define KA_CLI_STR_EXTRACTING_ISA "Extracting ISA for "
#define KA_CLI_STR_EXTRACTING_STATISTICS "Extracting statistics for "
#define KA_CLI_STR_EXTRACTING_AMDIL "Extracting AMD IL code for "
#define KA_CLI_STR_D3D_ASM_GENERATION_SUCCESS "D3D ASM code generation succeeded; See "
#define KA_CLI_STR_D3D_ASM_GENERATION_FAILURE "D3D ASM code generation failed: "
#define KA_CLI_STR_ERR_GLSL_COMPILATION_NOT_SUPPORTED_A "Offline GLSL compilation for "
#define KA_CLI_STR_ERR_GLSL_COMPILATION_NOT_SUPPORTED_B " is not supported."
#define KA_CLI_STR_ERR_D3D_COMPILATION_NOT_SUPPORTED_FOR_DEVICE "D3D ASM offline compilation to ISA is not supported for "
#define KA_CLI_STR_TMP_TXT_FILE_WILDCARD L"cxlTempFile_*.txt"
#endif // __kcCliStringConstantsh_h

// Shaders and pipeline stages.
#define KA_CLI_STR_VERTEX_SHADER "vertex" 
#define KA_CLI_STR_TESS_CTRL_SHADER "tessellation control" 
#define KA_CLI_STR_TESS_EVAL_SHADER "tessellation evaluation"
#define KA_CLI_STR_GEOMETRY_SHADER "geometry" 
#define KA_CLI_STR_FRAGMENT_SHADER "fragment" 
#define KA_CLI_STR_COMPUTE_SHADER "compute" 

// Stage abbreviations for output file names.
#define KA_CLI_STR_VERTEX_ABBREVIATION "vert" 
#define KA_CLI_STR_TESS_CTRL_ABBREVIATION "tesc" 
#define KA_CLI_STR_TESS_EVAL_ABBREVIATION "tese"
#define KA_CLI_STR_GEOMETRY_ABBREVIATION "geom" 
#define KA_CLI_STR_FRAGMENT_ABBREVIATION "frag" 
#define KA_CLI_STR_COMPUTE_ABBREVIATION "comp" 
