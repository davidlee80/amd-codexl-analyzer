//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file beProgramBuilderGL.h 
/// 
//=====================================================================
#pragma once
#include "beProgramBuilder.h"
#include <AMDTBackEnd/ExternGL/elf_amdtypes.h>

#ifdef _WIN32
    #include "OpenGLModule.h"
#endif // _WIN32

using namespace beKA;
struct AtiElf_SC_SI_HWSHADER_BASE;


// An OpenGL context needs to be current in the thread calling other GL functions.
// I think it should be possible to use a *MakeCurrent function to move the context
// from the main thread to the compilation thread,
// but I've not yet been able to make that work.
// So for now I will do the initializations in the compilation thread.
// I will try to do those initializations only once.
class KA_BACKEND_DECLDIR beProgramBuilderGL : public beProgramBuilder
{

public:

    /// Options specific to OpenGL
    struct GLOptions : public beKA::CompileOptions
    {
        /// The kind of Shader we are compiling.
        /// For GL, this is: Vertex, Geometry,
        /// Tessellation Control, Tessellation Evaluation, or Fragment.
        int  m_Profile;

        /// Which chip family?  See ...\drivers\inc\asic_reg\atiid.h for the list.
        /// Currently only SI is supported.
        unsigned int m_ChipFamily;

        /// Which device?  See ...\drivers\inc\asic_reg\si_id.h &c. for the lists.
        unsigned int m_ChipRevision;

        /// Shader type: currently only vertex or fragment shaders are supported.
        std::string m_ShaderType;
    };


    ~beProgramBuilderGL(void);

    /// Get list of Kernels/Shaders.
    /// Must be called after Compile is successfully called.
    /// \param[in]  device  The name of the device to choose.
    /// \param[out] kernels Vector of names of Kernels/Shaders compiled.
    /// \returns            a status.
    /// If a Log stream is available, some failures may be diagnosed.
    virtual beKA::beStatus GetKernels(const std::string& device, std::vector<std::string>& kernels);

    /// Get a binary version of the program.
    /// \param[in]  program Handle to the built program.
    /// \param[in]  device  The name of the device to choose.
    /// \param[in]  binopts Options to customize the output object.
    ///                     If NULL, a complete object is returned.
    /// \param[out] binary  A place to return a reference to the binary.
    /// \returns            a status.
    /// If a Log stream is available, some failures may be diagnosed.
    virtual beKA::beStatus GetBinary(const std::string& device, const beKA::BinaryOptions& binopts, std::vector<char>& binary) ;

    /// Get a string for a kernel IL.
    /// \param[in]  device     The name of the device.
    /// \param[in]  kernel     The name of the kernel.
    /// \param[out] s          The output as a string.
    /// \returns               a status.
    /// If a Log stream is available, some failures may be diagnosed.
    virtual beKA::beStatus GetKernelILText(const std::string& device, const std::string& kernel, std::string& il) ;

    /// Get a string for a kernel ISA.
    /// \param[in]  device     The name of the device.
    /// \param[in]  kernel     The name of the kernel.
    /// \param[out] s          The output as a string.
    /// \returns               a status.
    /// If a Log stream is available, some failures may be diagnosed.
    virtual beKA::beStatus GetKernelISAText(const std::string& device, const std::string& kernel, std::string& isa) ;

    /// Return the statistics for a specific kernel on a certain device.
    /// \param[in]  device     The name of the device.
    /// \param[in]  kernel     The name of the kernel.
    /// \param[out] analysis   The output as a analysis.
    /// \returns               a status.
    /// If a Log stream is available, some failures may be diagnosed.
    // this one will return the AnalysisData -- note, this is currently incomplete
    virtual beKA::beStatus GetStatistics(const std::string& device, const std::string& kernel, beKA::AnalysisData& analysis);

    /// Return true if the module was loaded and initialized properly
    virtual bool IsInitialized() ;

    /// Release all data relevant to the previous compilations
    virtual void ReleaseProgram() ;

    /// Retrieve all devices as got from the loaded module
    virtual beKA::beStatus GetDeviceTable(std::vector<GDT_GfxCardInfo>& table) const ;

    /// Returns true if previous compilation succeeded for the certain device. false otherwise
    virtual bool CompileOK(std::string& device) ;

    virtual beKA::beStatus Compile(const std::string&   programSource, const GLOptions& glOptions, const std::string*   sourcePath);

    /// Returns the OpenGL version as reported by the OpenGL runtime.
    /// \param[out] version   The version string.
    /// \returns              true for success, false otherwise.
    bool GetOpenGlVersion(std::string& version) const;

protected:
    /// Ctor
    beProgramBuilderGL(void);

    virtual beKA::beStatus Initialize(const std::string& sDllModule = "") ;

private:
    beProgramBuilderGL(const beProgramBuilderGL&) = delete;
    beProgramBuilderGL& operator=(const beProgramBuilderGL&) = delete;

    class NativeImpl;
    NativeImpl* m_pNativeImpl;

    std::vector<GDT_GfxCardInfo>          m_GLDeviceTable;

#ifdef _WIN32
    OpenGLModule                          m_OpenGLModule;
#endif // _WIN32

    /// private members for creating the gl window

    static std::vector<char> m_compiledBinary;

    int m_binaryIsaSize;
    int m_binaryIlSize;
    int m_glProfile;

    char* m_pBinaryISA;
    char* m_pBinaryIL;

    AtiElf_SC_SI_HWSHADER_BASE* m_pStatistics;
    std::string m_statistics;

    bool m_IsIntialized;

private:
    /// init the supported device list
    bool InitAsicsList();

    bool ConvertProfiletoSP3_shtype(int InProfile, int& OutSP3Type);

    /// SplitDelimitedString - splits given delimited string
    /// \param [in] source
    /// \param [in] delimiter
    /// \param [out] vector of strings
    bool SplitDelimitedString(const std::string& source, const std::string& delimiter, std::vector<std::string>& errorLines);

    friend class Backend;
};

