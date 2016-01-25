//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file beProgramBuilderGL.cpp 
/// 
//=====================================================================

// For internal/public version.
#include <AMDTBaseTools/Include/gtDefinitions.h>

// ************************************************************************
// NOTE: COMMENTED CODE IN THIS .CPP FILE WAS LEFT INTENTIONALLY.
//       IT WILL BE REMOVED IN THE FINAL VERSION BEFORE CODEXL 1.9 RELEASE.
//       THIS .CPP FILE ALSO INCLUDES CODE THAT WAS DUPLICATED FROM
//       OSWRAPPERS. THIS WILL BE FIXED IN THE NEAR FUTURE.
// ************************************************************************

#if GR_BUILD_TARGET == GR_WINDOWS_OS
    #include <Include/GL/glew.h>
    #include <Include/GL/wglext.h>
#else
    #include<X11/X.h>
    //#include <Include/GL/glxew.h>
    #include <AMDTBackEnd/Include/GL/glew.h>

    #include <AMDTOSAPIWrappers/Include/GL/linux/GL/gl.h>
    #include <AMDTOSAPIWrappers/Include/GL/glext.h>
    //#include <AMDTOSAPIWrappers/Include/GL/glextremoved.h>
    //#include <AMDTOSAPIWrappers/Include/GL/linux/GL/glext_additions.h>
    #include <AMDTOSAPIWrappers/Include/GL/linux/GL/glx.h>
    #include <AMDTOSAPIWrappers/Include/GL/linux/GL/glxext.h>
    //#include <AMDTOSAPIWrappers/Include/GL/GRemdeyGLExtensions.h>

    //#include <AMDTOSAPIWrappers/src/linux/oaPlatformSpecificFunctionPointers.h>


    //#include <AMDTOSAPIWrappers/src/common/oaOpenGLFunctionPointers.h>

    // oaOpenGLFunctionPointers - BEGIN

    // Local:
    #include <AMDTOSWrappers/Include/osOSDefinitions.h>
    //#include <AMDTOSAPIWrappers/Include/oaOpenGLIncludes.h>

    // OpenGL function types:
    typedef const GLubyte* (APIENTRY* PFNGLGETSTRING)(GLenum name);

    // OpenGL function pointers:
    extern PFNGLGETSTRING pOAglGetString;

    // Aid functions:
    bool oaLoadSystemOpenGLModule();
    bool oaUnLoadSystemOpenGLModule();
    osModuleHandle oaSystemOpenGLModuleHandle();
    bool oaLoadOpenGLFunctionPointers();


    // oaOpenGLFunctionPointers - END


    #include <AMDTOSWrappers/Include/osModule.h>
#endif

// C++.
#include <algorithm>
#include <cctype>
#include <locale>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

#include <ExternGL/elf_dumper.h>
#include <ExternGL/AtiElfSI.h>

#include <sp3.h>
#include <sp3-int.h>

// This is from ADL's include directory.
#include <ADLUtil.h>
#include <customer/oem_structures.h>
#include <DeviceInfoUtils.h>

#if GR_BUILD_TARGET == GR_WINDOWS_OS
    #pragma warning(push)
    #pragma warning(disable:4100)
#endif

#if GR_BUILD_TARGET == GR_WINDOWS_OS
    #pragma warning(pop)
#endif

// Local.
#include <DeviceInfoUtils.h>
#include <Include/beProgramBuilderGL.h>
#include <AMDTBackEnd/Include/beInclude.h>

// Constants.
#include <Include/beStringConstants.h>

// Infra.
#include <AMDTBaseTools/Include/gtAssert.h>

using namespace std;

#define WGL_CONTEXT_DUMMY_DEVICE    ~0UL
#define MAX_LOADSTRING              100
#define IDM_EXIT                    105

// This will be used for compiling and getting the IL.
#define GL_PROGRAM_BUILD_PARAMETER_AMD                    0x9400
#define GL_BUILD_DEBUG_AMD                                0x0001
#define GL_BUILD_ILBIN_AMD                                0x0040

std::vector<char> beProgramBuilderGL::m_compiledBinary;

static bool GfxCardInfoSortPredicate(const GDT_GfxCardInfo& a, const GDT_GfxCardInfo& b)
{
    // Generation is the primary key.
    if (a.m_generation < b.m_generation) { return true; }

    if (a.m_generation > b.m_generation) { return false; }

    // CAL name is next.
    int ret = ::strcmp(a.m_szCALName, b.m_szCALName);

    if (ret < 0) { return true; }

    if (ret > 0) { return false; }

    // Marketing name next.
    ret = ::strcmp(a.m_szMarketingName, b.m_szMarketingName);

    if (ret < 0) { return true; }

    if (ret > 0) { return false; }

    // DeviceID last.
    return a.m_deviceID < b.m_deviceID;
}

#if GR_BUILD_TARGET == GR_WINDOWS_OS

// Current instance.
HINSTANCE  hInst;

// The title bar text.
LPCTSTR    szTitle = L"Title";

// The main window class name.
LPCTSTR    szWindowClass = L"OfflineGL";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
        case WM_COMMAND:
            wmId = LOWORD(wParam);
            wmEvent = HIWORD(wParam);

            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;

                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }

            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code here...
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
#endif


#if GR_BUILD_TARGET == GR_WINDOWS_OS

// Encapsulates the native OpenGL interactions.
class beProgramBuilderGL::NativeImpl
{
public:

    NativeImpl(beProgramBuilderGL& owner) :
        m_hWnd(nullptr),
        m_hInstance(nullptr),
        m_hDC(nullptr),
        m_wglCreateAssociatedContextAMD(nullptr),
        m_wglCreateAssociatedContextAttribsAMD(nullptr),
        m_wglDeleteAssociatedContextAMD(nullptr),
        m_wglGetGPUIDsAMD(nullptr),
        m_wglMakeAssociatedContextCurrentAMD(nullptr),
        m_hRC(false),
        m_owner(owner) {}

    ~NativeImpl()
    {
        m_hWnd = nullptr;          // Holds Our Window Handle
        m_hInstance = nullptr;  // Holds The Instance Of The Application
        m_hDC = nullptr;      // Private GDI Device Context
    }

    bool Initialize(beKA::beStatus& initializationStatus)
    {
        bool ret = CreateGLWindow();

        if (ret)
        {
            ret = InitWglAMD();

            if (!ret)
            {
                initializationStatus = beKA::beStatus_GLInitWglAMDFailed;
            }
        }
        else
        {
            initializationStatus = beKA::beStatus_GLglCreateGLWindowFailed;
        }

        return ret;
    }

    bool CreateVirtualContext(const GLOptions& glOptions)
    {
        bool ret = false;

        // Prepare the attribute list (this is the device we are going to compile for).
        int attriblist[] = { WGL_CONTEXT_ASIC_FAMILY_AMD,
                             int(glOptions.m_ChipFamily),
                             WGL_CONTEXT_ASIC_REV_AMD,
                             int(glOptions.m_ChipRevision),
                             WGL_CONTEXT_DEVICE_MEMORY_SIZE_AMD,
                             805306368,
                             0
                           };

        // Create the virtual context.tlvlax
        HGLRC isAttribs = m_wglCreateAssociatedContextAttribsAMD(WGL_CONTEXT_DUMMY_DEVICE, NULL, attriblist);
        BOOL isCurrent = m_wglMakeAssociatedContextCurrentAMD(isAttribs);
        ret = (isAttribs && isCurrent);

        return ret;
    }

private:

    /// create the dummy openGL window
    bool CreateGLWindow()
    {
        bool ret = true;
        int width = 640;
        int height = 480;
        int bits = 16;

        // Holds The Results After Searching For A Match.
        GLuint  PixelFormat;

        // Windows Class Structure.
        WNDCLASS wc;

        // Window Extended Style.
        DWORD  dwExStyle;

        // Window Style.
        DWORD  dwStyle;

        // Grabs Rectangle Upper Left / Lower Right Values.
        RECT  WindowRect;

        // Set Left Value To 0.
        WindowRect.left = (long)0;

        // Set Right Value To Requested Width.
        WindowRect.right = (long)width;

        // Set Top Value To 0.
        WindowRect.top = (long)0;

        // Set Bottom Value To Requested Height.
        WindowRect.bottom = (long)height;

        // Grab An Instance For Our Window.
        HINSTANCE hInstance = GetModuleHandle(NULL);

        // Redraw on size, and own DC For Window.
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

        // WndProc handles the messages.
        wc.lpfnWndProc = (WNDPROC)WndProc;

        // No Extra Window Data
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;

        // Set The instance.
        wc.hInstance = hInstance;

        // Load the default icon.
        wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);

        // Load The Arrow Pointer.
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);

        // No Background Required For GL.
        wc.hbrBackground = NULL;

        // We Don't Want A Menu.
        wc.lpszMenuName = NULL;

        // Set The Class Name.
        wc.lpszClassName = szWindowClass;

        // Attempt To Register The Window Class.
        if (!RegisterClass(&wc))
        {
            std::stringstream ss;
            ss << "beProgramBuilderGL::CreateGLWindow: RegisterClass failed." << endl;
            m_owner.LogCallBack(ss.str());
            ret = false;           // Return FALSE
        }

        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
        dwStyle = WS_OVERLAPPEDWINDOW;       // Windows Style

        AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);  // Adjust Window To True Requested Size

        // Create The Window
        if (ret)
        {
            m_hWnd = CreateWindowEx(dwExStyle,                  // Extended Style For The Window
                                    szWindowClass,                                      // Class Name
                                    szTitle,                                            // Window Title
                                    dwStyle |                                           // Defined Window Style
                                    WS_CLIPSIBLINGS |                                   // Required Window Style
                                    WS_CLIPCHILDREN,                                    // Required Window Style
                                    0, 0,                                               // Window Position
                                    WindowRect.right - WindowRect.left,                 // Calculate Window Width
                                    WindowRect.bottom - WindowRect.top,                 // Calculate Window Height
                                    NULL,                                               // No Parent Window
                                    NULL,                                               // No Menu
                                    hInstance,                                        // Instance
                                    NULL);                                               // Dont Pass Anything To WM_CREATE

            if (nullptr == m_hWnd)
            {
                KillGLWindow();        // Reset The Display
                std::stringstream ss;
                ss << "beProgramBuilderGL::CreateGLWindow: CreateWindowEx failed." << endl;
                m_owner.LogCallBack(ss.str());
                ret = false;           // Return FALSE
            }
        }

        static PIXELFORMATDESCRIPTOR pfd =    // pfd Tells Windows How We Want Things To Be
        {
            sizeof(PIXELFORMATDESCRIPTOR),    // Size Of This Pixel Format Descriptor
            1,                                // Version Number
            PFD_DRAW_TO_WINDOW |              // Format Must Support Window
            PFD_SUPPORT_OPENGL |              // Format Must Support OpenGL
            PFD_DOUBLEBUFFER,                 // Must Support Double Buffering
            PFD_TYPE_RGBA,                    // Request An RGBA Format
            (BYTE)bits,                             // Select Our Color Depth
            0, 0, 0, 0, 0, 0,                 // Color Bits Ignored
            0,                                // No Alpha Buffer
            0,                                // Shift Bit Ignored
            0,                                // No Accumulation Buffer
            0, 0, 0, 0,                       // Accumulation Bits Ignored
            16,                               // 16Bit Z-Buffer (Depth Buffer)
            0,                                // No Stencil Buffer
            0,                                // No Auxiliary Buffer
            PFD_MAIN_PLANE,                   // Main Drawing Layer
            0,                                // Reserved
            0, 0, 0                           // Layer Masks Ignored
        };

        if (ret)
        {
            m_hDC = GetDC(m_hWnd);       // Did We Get A Device Context?

            if (nullptr == m_hDC)
            {
                KillGLWindow();        // Reset The Display
                std::stringstream ss;
                ss << "beProgramBuilderGL::CreateGLWindow: GetDC failed." << endl;
                m_owner.LogCallBack(ss.str());
                ret = false;
            }
        }

        if (ret)
        {
            PixelFormat = ChoosePixelFormat(m_hDC, &pfd); // Did Windows Find A Matching Pixel Format?

            if (0 == PixelFormat)
            {
                KillGLWindow();        // Reset The Display
                std::stringstream ss;
                ss << "beProgramBuilderGL::CreateGLWindow: ChoosePixelFormat failed." << endl;
                m_owner.LogCallBack(ss.str());
                ret = false;

            }
        }

        if (ret && !SetPixelFormat(m_hDC, PixelFormat, &pfd))  // Are We Able To Set The Pixel Format?
        {
            KillGLWindow();        // Reset The Display
            std::stringstream ss;
            ss << "beProgramBuilderGL::CreateGLWindow: PixelFormat failed." << endl;
            m_owner.LogCallBack(ss.str());
            ret = false;
        }

        return ret;
    }

    /// Properly Kill The OpenGL Window created in CreateGLWindow
    void KillGLWindow()
    {
        if (m_hRC)           // Do We Have A Rendering Context?
        {
            m_owner.m_OpenGLModule.m_wglMakeCurrent_fn(NULL, NULL);     // Are We Able To Release The DC And RC Contexts?
            m_owner.m_OpenGLModule.m_wglDeleteContext_fn(m_hRC);      // Are We Able To Delete The RC?
            m_hRC = NULL;          // Set RC To NULL
        }

        if (m_hDC && !ReleaseDC(m_hWnd, m_hDC))     // Are We Able To Release The DC
        {
            m_hDC = NULL;          // Set DC To NULL
        }

        if (m_hWnd && !DestroyWindow(m_hWnd))     // Are We Able To Destroy The Window?
        {
            m_hWnd = NULL;          // Set hWnd To NULL
        }

        if (!UnregisterClass(szWindowClass, m_hInstance))   // Are We Able To Unregister Class
        {
            m_hInstance = NULL;         // Set hInstance To NULL
        }
    }

    /// init OpenGL AMD extensions api
    bool InitWglAMD()
    {
        bool bRet = true;
        m_hRC = m_owner.m_OpenGLModule.m_wglCreateContext_fn(m_hDC);

        if (nullptr == m_hRC)    // Are We Able To Get A Rendering Context?
        {
            KillGLWindow();        // Reset The Display
            std::stringstream ss;
            ss << "beProgramBuilderGL::InitWglAMD: Can't Create A GL Rendering Context." << endl;
            m_owner.LogCallBack(ss.str());
            bRet = false;
        }

        if (bRet && !m_owner.m_OpenGLModule.m_wglMakeCurrent_fn(m_hDC, m_hRC))     // Try To Activate The Rendering Context
        {
            KillGLWindow();        // Reset The Display
            std::stringstream ss;
            ss << "beProgramBuilderGL::InitWglAMD: Can't Activate The GL Rendering Context." << endl;
            m_owner.LogCallBack(ss.str());
            bRet = false;
        }

        if (bRet)
        {
            m_wglCreateAssociatedContextAttribsAMD = (PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC)m_owner.m_OpenGLModule.m_wglGetProcAddress_fn("wglCreateAssociatedContextAttribsAMD");
            m_wglDeleteAssociatedContextAMD = (PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC)m_owner.m_OpenGLModule.m_wglGetProcAddress_fn("wglDeleteAssociatedContextAMD");
            m_wglGetGPUIDsAMD = (PFNWGLGETGPUIDSAMDPROC)m_owner.m_OpenGLModule.m_wglGetProcAddress_fn("wglGetGPUIDsAMD");
            m_wglMakeAssociatedContextCurrentAMD = (PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC)m_owner.m_OpenGLModule.m_wglGetProcAddress_fn("wglMakeAssociatedContextCurrentAMD");

            if (bRet && ((m_wglCreateAssociatedContextAttribsAMD == NULL) ||
                         (m_wglDeleteAssociatedContextAMD == NULL) ||
                         (m_wglGetGPUIDsAMD == NULL) ||
                         (m_wglMakeAssociatedContextCurrentAMD == NULL)))
            {
                KillGLWindow();        // Reset The Display
                std::stringstream ss;
                ss << "beProgramBuilderGL::InitWglAMD: Cannot get AMD extention interface" << endl;
                m_owner.LogCallBack(ss.str());
                bRet = false;
            }

            GLenum err = glewInit();

            if (GLEW_OK != err)
            {
                bRet = false;
            }
        }

        return bRet;
    }

    // Holds Our Window Handle.
    HWND        m_hWnd;

    // Holds The Instance Of The Application.
    HINSTANCE   m_hInstance;

    // Private GDI Device Context.
    HDC         m_hDC;

    // Permanent Rendering Context.
    HGLRC       m_hRC;

    /// wgl parameters
    PFNWGLCREATEASSOCIATEDCONTEXTAMDPROC m_wglCreateAssociatedContextAMD;
    PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC m_wglCreateAssociatedContextAttribsAMD;
    PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC m_wglDeleteAssociatedContextAMD;
    PFNWGLGETGPUIDSAMDPROC  m_wglGetGPUIDsAMD;
    PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC m_wglMakeAssociatedContextCurrentAMD;

    beProgramBuilderGL& m_owner;

private:
    NativeImpl(const NativeImpl&) = delete;
    NativeImpl& operator=(const NativeImpl&) = delete;
};

#else

// oaPlatformSpecificFunctionPointers - BEGIN

// GLX function types:
typedef  GLXContext(*PFNGLXCREATECONTEXT)(Display* dpy, XVisualInfo* vis, GLXContext share_list, Bool direct);
typedef void (*PFNGLXDESTROYCONTEXT)(Display* dpy, GLXContext ctx);
typedef Bool(*PFNGLXMAKECURRENT)(Display* dpy, GLXDrawable drawable, GLXContext ctx);
typedef const char* (*PFNGLXQUERYEXTENSIONSSTRING)(Display* dpy, int screen);
typedef void (*(*PFNGLXGETPROCADDRESS)(const GLubyte* procname))(void);
typedef int (*PFNGLXGETCONFIG)(Display* dpy, XVisualInfo* vis, int attrib, int* value);
typedef XVisualInfo* (*PFNGLXCHOOSEVISUAL)(Display* dpy, int screen, int* attrib_list);

// GLX function pointers:
extern PFNGLXCREATECONTEXT pOAglXCreateContext;
extern PFNGLXDESTROYCONTEXT pOAglXDestroyContext;
extern PFNGLXMAKECURRENT pOAglXMakeCurrent;
extern PFNGLXQUERYEXTENSIONSSTRING pOAglXQueryExtensionsString;
extern PFNGLXGETPROCADDRESS pOAglXGetProcAddress;
extern PFNGLXGETCONFIG pOAglXGetConfig;
extern PFNGLXCHOOSEVISUAL pOAglXChooseVisual;

typedef GLXContext(*PFNGLXGETCURRCTX)(void);
extern PFNGLXGETCURRCTX pOAglXGetCurrentContext;
PFNGLXGETCURRCTX pOAglXGetCurrentContext = NULL;

// ---------------------------------------------------------------------------
// Name:        oaLoadGLXFunctionPointers
// Description: Loads all GLX functions needed by the GROSWrappers module.
// Return Val:  bool - Success / failure.
// Author:      AMD Developer Tools Team
// Date:        8/4/2010
// ---------------------------------------------------------------------------
bool _oaLoadGLXFunctionPointers()
{
    // This function could fail if any function was not yet acquired:
    bool retVal = !((pOAglXCreateContext == NULL) || (pOAglXDestroyContext == NULL) || (pOAglXMakeCurrent == NULL) ||
                    (pOAglXQueryExtensionsString == NULL) || (pOAglXGetProcAddress == NULL) || (pOAglXGetConfig == NULL) ||
                    (pOAglXChooseVisual == NULL));

    // Make sure we have all function pointers:
    if (!retVal)
    {
        // We are supposed to have all functions, or none at all:
        GT_ASSERT((pOAglXCreateContext == NULL) && (pOAglXDestroyContext == NULL) && (pOAglXMakeCurrent == NULL) &&
                  (pOAglXQueryExtensionsString == NULL) && (pOAglXGetProcAddress == NULL) && (pOAglXGetConfig == NULL) &&
                  (pOAglXChooseVisual == NULL));

        // Make sure the system OpenGL module is loaded:
        bool rcMod = oaLoadSystemOpenGLModule();
        GT_IF_WITH_ASSERT(rcMod)
        {
            // Get the OpenGL module handle:
            osModuleHandle systemOpenGLModuleHandle = oaSystemOpenGLModuleHandle();
            GT_IF_WITH_ASSERT(systemOpenGLModuleHandle != OS_NO_MODULE_HANDLE)
            {
                // Get the function pointers:
                osProcedureAddress pglXCreateContext = NULL;
                bool rc1 = osGetProcedureAddress(systemOpenGLModuleHandle, "glXCreateContext", pglXCreateContext);
                osProcedureAddress pglXDestroyContext = NULL;
                bool rc2 = osGetProcedureAddress(systemOpenGLModuleHandle, "glXDestroyContext", pglXDestroyContext);
                osProcedureAddress pglXMakeCurrent = NULL;
                bool rc3 = osGetProcedureAddress(systemOpenGLModuleHandle, "glXMakeCurrent", pglXMakeCurrent);
                osProcedureAddress pglXQueryExtensionsString = NULL;
                bool rc4 = osGetProcedureAddress(systemOpenGLModuleHandle, "glXQueryExtensionsString", pglXQueryExtensionsString);
                osProcedureAddress pglXGetProcAddress = NULL;
                bool rc5 = osGetProcedureAddress(systemOpenGLModuleHandle, "glXGetProcAddress", pglXGetProcAddress);
                osProcedureAddress pglXGetConfig = NULL;
                bool rc6 = osGetProcedureAddress(systemOpenGLModuleHandle, "glXGetConfig", pglXGetConfig);
                osProcedureAddress pglXChooseVisual = NULL;
                bool rc7 = osGetProcedureAddress(systemOpenGLModuleHandle, "glXChooseVisual", pglXChooseVisual);
                osProcedureAddress pgetcurr = NULL;
                osGetProcedureAddress(systemOpenGLModuleHandle, "glXGetCurrentContext", pgetcurr);
                GT_IF_WITH_ASSERT(rc1 && (pglXCreateContext != NULL) && rc2 && (pglXDestroyContext != NULL) &&
                                  rc3 && (pglXMakeCurrent != NULL) && rc4 && (pglXQueryExtensionsString  != NULL) &&
                                  rc5 && (pglXGetProcAddress  != NULL) && rc6 && (pglXGetConfig != NULL) &&
                                  rc7 && (pglXChooseVisual != NULL))
                {
                    // Copy the pointers to the variables:
                    pOAglXCreateContext = (PFNGLXCREATECONTEXT)pglXCreateContext;
                    pOAglXDestroyContext = (PFNGLXDESTROYCONTEXT)pglXDestroyContext;
                    pOAglXMakeCurrent = (PFNGLXMAKECURRENT)pglXMakeCurrent;
                    pOAglXQueryExtensionsString = (PFNGLXQUERYEXTENSIONSSTRING)pglXQueryExtensionsString;
                    pOAglXGetProcAddress = (PFNGLXGETPROCADDRESS)pglXGetProcAddress;
                    pOAglXGetConfig = (PFNGLXGETCONFIG)pglXGetConfig;
                    pOAglXChooseVisual = (PFNGLXCHOOSEVISUAL)pglXChooseVisual;
                    pOAglXGetCurrentContext = (PFNGLXGETCURRCTX)pgetcurr;

                    retVal = true;
                }
            }
        }
    }

    return retVal;
}

// oaPlatformSpecificFunctionPointers - END

// glxew.h IMPORT - BEGIN
typedef GLXContext(* PFNGLXCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC)(unsigned int id, GLXContext share_context, const int* attribList);
typedef Bool(* PFNGLXDELETEASSOCIATEDCONTEXTAMDPROC)(GLXContext ctx);
typedef Bool(* PFNGLXMAKEASSOCIATEDCONTEXTCURRENTAMDPROC)(GLXContext ctx);
// glxew.h IMPORT - END




#define GLX_CONTEXT_DUMMY_DEVICE_AMD          0xffffffff  //dumy id
#define GLX_CONTEXT_ASIC_ID_AMD               0x91c4  //asic chip id
#define GLX_CONTEXT_ASIC_FAMILY_AMD           0x91c5  //asic family
#define GLX_CONTEXT_ASIC_REV_AMD              0x91c6  //asic revision
#define GLX_CONTEXT_DEVICE_MEMORY_SIZE_AMD    0x91c7  //asic device memory
#define GL_PROGRAM_BUILD_PARAMETER_AMD                    0x9400 // 
#define GL_BUILD_DEBUG_AMD                                0x0001
#define GL_BUILD_ILBIN_AMD                                0x0040

// ******************************************
// TO BE RELOCATED TO LINUX-SPECIFIC CPP FILE
// ******************************************
class beProgramBuilderGL::NativeImpl
{
public:

    explicit NativeImpl(beProgramBuilderGL& owner) :
        m_owner(owner), m_pDpy(NULL), m_pVi(NULL), m_assoc_ctx(0) {}

    ~NativeImpl()
    {
        // Clean up.
        CleanUp();
        m_glXDeleteAssociatedContextAMD(m_assoc_ctx);
    }

    void CleanUp()
    {
        pOAglXMakeCurrent(m_pDpy, None, NULL);
        pOAglXDestroyContext(m_pDpy, m_glc);
        XDestroyWindow(m_pDpy, m_win);
        XCloseDisplay(m_pDpy);
    }


    bool Initialize(beKA::beStatus& initializationStatus)
    {
        initializationStatus = beStatus::beStatus_SUCCESS;

        // Load the GLX function pointers.
        bool ret = _oaLoadGLXFunctionPointers();
        GT_ASSERT_EX(ret, L"Loading glx function pointers.");

        if (ret)
        {
            m_pDpy = XOpenDisplay(NULL);

            if (m_pDpy != NULL)
            {
                m_root = DefaultRootWindow(m_pDpy);
                m_pVi = pOAglXChooseVisual(m_pDpy, 0, m_att);
                ret = (m_pVi != NULL);

                if (!ret)
                {
                    initializationStatus = beStatus_GLOpenGLXServerChooseVisualError;
                }
            }
            else
            {
                initializationStatus = beStatus_GLOpenGLXServerConnectionError;
                std::stringstream logStream;
                logStream << "Failed to connect to X-server." << std::endl;
                m_owner.LogCallBack(logStream.str());
            }
        }
        else
        {
            initializationStatus = beStatus_GLOpenGLFunctionPointersNotLoaded;
        }

        return ret;
    }

    bool CreateVirtualContext(const GLOptions& glOptions)
    {
        bool ret = false;

        if (m_pVi != nullptr)
        {
            m_cmap = XCreateColormap(m_pDpy, m_root, m_pVi->visual, AllocNone);

            m_swa.colormap = m_cmap;
            m_swa.event_mask = ExposureMask | KeyPressMask;
            m_win = XCreateWindow(m_pDpy, m_root, 0, 0, 600, 600, 0, m_pVi->depth, InputOutput, m_pVi->visual, CWColormap | CWEventMask, &m_swa);

            // Uncomment the following two lines to display the GL window.
            //XMapWindow(m_dpy, m_win);
            //XStoreName(m_dpy, m_win, "Analyzer GLSL");

            // Create the context.
            m_glc = pOAglXCreateContext(m_pDpy, m_pVi, NULL, GL_TRUE);

            if (m_glc != 0)
            {
                // Make the context the current context.
                pOAglXMakeCurrent(m_pDpy, m_win, m_glc);
                glewInit();

                GLubyte* funcname = (GLubyte*)("glXCreateAssociatedContextAttribsAMD");
                m_glXCreateAssociatedContextAttribsAMD = (PFNGLXCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC)pOAglXGetProcAddress(funcname);
                funcname = (GLubyte*)("glXDeleteAssociatedContextAMD");
                m_glXDeleteAssociatedContextAMD = (PFNGLXDELETEASSOCIATEDCONTEXTAMDPROC)pOAglXGetProcAddress(funcname);
                funcname = (GLubyte*)("glXMakeAssociatedContextCurrentAMD");
                m_glXMakeAssociatedContextCurrentAMD = (PFNGLXMAKEASSOCIATEDCONTEXTCURRENTAMDPROC)pOAglXGetProcAddress(funcname);

                if (m_glXCreateAssociatedContextAttribsAMD != nullptr && m_glXMakeAssociatedContextCurrentAMD != nullptr)
                {

                    // In the following comments there are examples of the required parameters for different architectures.
                    //gpu core: bonaire
                    //int attriblist[] = {GLX_CONTEXT_ASIC_FAMILY_AMD, 120 , GLX_CONTEXT_ASIC_REV_AMD , 20 , GLX_CONTEXT_DEVICE_MEMORY_SIZE_AMD, 805306368,0};

                    //gpu core : DEVASTATOR
                    //  int attriblist[] = {GLX_CONTEXT_ASIC_FAMILY_AMD, 105 , GLX_CONTEXT_ASIC_REV_AMD , 2 , GLX_CONTEXT_DEVICE_MEMORY_SIZE_AMD, 805306368,0};

                    //gpu core: barts
                    //  int attriblist[] = {GLX_CONTEXT_ASIC_FAMILY_AMD, 100 , GLX_CONTEXT_ASIC_REV_AMD , 20 , GLX_CONTEXT_DEVICE_MEMORY_SIZE_AMD, 805306368,0};

                    //gpu core: CAPEVERDE
                    //int attriblist[] = { GLX_CONTEXT_ASIC_FAMILY_AMD, 110, GLX_CONTEXT_ASIC_REV_AMD, 41, GLX_CONTEXT_DEVICE_MEMORY_SIZE_AMD, 805306368, 0 };

                    // Take the attributes from the user's input.
                    const int attriblist[] = {GLX_CONTEXT_ASIC_FAMILY_AMD,
                                              static_cast<int>(glOptions.m_ChipFamily),
                                              GLX_CONTEXT_ASIC_REV_AMD,
                                              static_cast<int>(glOptions.m_ChipRevision),
                                              GLX_CONTEXT_DEVICE_MEMORY_SIZE_AMD,
                                              805306368,
                                              0
                                             };

                    // Create the context.
                    m_assoc_ctx = m_glXCreateAssociatedContextAttribsAMD(GLX_CONTEXT_DUMMY_DEVICE_AMD, NULL, attriblist);

                    if (m_assoc_ctx == 0)
                    {
                        std::stringstream stream;
                        stream << "Unable to create AMD context.";
                        m_owner.LogCallBack(stream.str());
                    }

                    // Make the created context the current context.
                    ret = m_glXMakeAssociatedContextCurrentAMD(m_assoc_ctx);
                }
            }
        }

        return ret;
    }

private:

    beProgramBuilderGL& m_owner;

    PFNGLXCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC  m_glXCreateAssociatedContextAttribsAMD = nullptr;
    PFNGLXDELETEASSOCIATEDCONTEXTAMDPROC m_glXDeleteAssociatedContextAMD = nullptr;
    PFNGLXMAKEASSOCIATEDCONTEXTCURRENTAMDPROC m_glXMakeAssociatedContextCurrentAMD = nullptr;

    Display*                 m_pDpy;
    Window                  m_root;
    static GLint            m_att[];
    XVisualInfo*             m_pVi;
    Colormap                m_cmap;
    XSetWindowAttributes    m_swa;
    Window                  m_win;
    GLXContext              m_glc;
    XWindowAttributes       m_gwa;
    XEvent                  m_xev;
    GLXContext              m_assoc_ctx;
};

// Static member definitions.
GLint beProgramBuilderGL::NativeImpl::m_att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };


#endif

beProgramBuilderGL::beProgramBuilderGL(void) :
#ifdef _WIN32
    m_OpenGLModule(OpenGLModule::s_DefaultModuleName),
#endif // _WIN32
    m_pNativeImpl(new beProgramBuilderGL::NativeImpl(*this)),
    m_binaryIsaSize(0),
    m_binaryIlSize(0),
    m_glProfile(-1),
    m_pBinaryISA(nullptr),
    m_pBinaryIL(nullptr),
    m_pStatistics(nullptr),
    m_IsIntialized(false) {}


beProgramBuilderGL::~beProgramBuilderGL(void) {}

beKA::beStatus beProgramBuilderGL::Initialize(const std::string& sDllModule)
{
    GT_UNREFERENCED_PARAMETER(sDllModule);

    beKA::beStatus  beRet = beKA::beStatus_General_FAILED;

    if (m_pNativeImpl != nullptr)
    {
        // For now, we bypass the OpenGLModule on Linux.
#ifdef _WIN32
        if (!m_OpenGLModule.IsModuleLoaded())
        {
            beRet = beKA::beStatus_GLOpenGLModuleFailedToLoad;
        }
        else
#endif
            if (m_pNativeImpl->Initialize(beRet))
            {
                if (!InitAsicsList())
                {
                    beRet = beKA::beStatus_GLFailedGetAsicList;
                }
                else
                {
                    m_IsIntialized = true;
                    beRet = beKA::beStatus_SUCCESS;
                }
            }
    }

    return beRet;
}

beKA::beStatus beProgramBuilderGL::GetKernels(const std::string& device, std::vector<std::string>& kernels)
{
    GT_UNREFERENCED_PARAMETER(&device);
    GT_UNREFERENCED_PARAMETER(&kernels);

    return beKA::beStatus_General_FAILED;
}

beKA::beStatus beProgramBuilderGL::GetBinary(const std::string& device, const beKA::BinaryOptions& binopts, std::vector<char>& binary)
{
    GT_UNREFERENCED_PARAMETER(&device);
    GT_UNREFERENCED_PARAMETER(&binopts);
    GT_UNREFERENCED_PARAMETER(&binary);

    return beKA::beStatus_General_FAILED;
}

beKA::beStatus beProgramBuilderGL::GetKernelILText(const std::string& device, const std::string& kernel, std::string& il)
{
    GT_UNREFERENCED_PARAMETER(il);
    GT_UNREFERENCED_PARAMETER(kernel);
    GT_UNREFERENCED_PARAMETER(&device);

    // We should have the IL in binary format inside m_pBinaryIL. In order to convert it into text, we need a decode which
    // we currently do not have. We need to request a decoder from the GL driver guys.
    return beKA::beStatus_General_FAILED;
}


beKA::beStatus beProgramBuilderGL::GetKernelISAText(const std::string& device, const std::string& kernel, std::string& isa)
{
    beStatus ret = beKA::beStatus_NO_ISA_FOR_DEVICE;

    // Clear the output buffer.
    isa.clear();

    if ((m_binaryIsaSize > 0) && (m_pBinaryISA != nullptr))
    {
        // Reset the return value.
        ret = beStatus_General_FAILED;

        // Do the conversion.
        int iSp3ProfileType = 0;
        bool isConversionOk = ConvertProfiletoSP3_shtype(m_glProfile, iSp3ProfileType);

        if (isConversionOk)
        {
            // Use the sp3 library to do the disassembly.
            // This only works with SI & CI for now.
            // We rely heavily on the stand-alone assembly/disassembly tool to get this code going.
            // Unfortunately, the sp3.h documentation is not too detailed.
            // For more info see: sc/Src/R1000/R1000Disassembler.cpp.
            struct sp3_context* pDisState = sp3_new();

            // What HW generation are we looking for.
            // Dana: there might be a more intelligent way to do this.
            bool isFound = false;

            for (const GDT_GfxCardInfo& glDevice : m_GLDeviceTable)
            {
                if (glDevice.m_szCALName == device)
                {
                    switch (glDevice.m_generation)
                    {
                        case GDT_HW_GENERATION_SOUTHERNISLAND:
                            sp3_setasic(pDisState, "SI");
                            isFound = true;
                            break;

                        case GDT_HW_GENERATION_SEAISLAND:
                            sp3_setasic(pDisState, "CI");
                            isFound = true;
                            break;

                        case GDT_HW_GENERATION_VOLCANICISLAND:
                            sp3_setasic(pDisState, "VI");
                            isFound = true;
                            break;

                        default:
                            ret = beStatus_GLUnknownHardwareFamily;
                            GT_ASSERT_EX(false, L"Unknown HW family");
                            sp3_setasic(pDisState, "SI");
                            break;
                    }

                    if (isFound)
                    {
                        // We can stop.
                        break;
                    }
                }
            }

            if (ret != beStatus_GLUnknownHardwareFamily)
            {
                unsigned* pIsaTokens = (unsigned*)&m_pBinaryISA[0];
                size_t isaTokensLen = m_binaryIsaSize / 4;

                sp3_vma* pVm = sp3_vm_new_ptr(0, isaTokensLen, pIsaTokens);

                if (pVm != nullptr)
                {
                    unsigned disassemblerFlags = 0;
                    disassemblerFlags |= SP3DIS_FORCEVALID;

                    char* pDisassembledShader = sp3_disasm(
                                                    pDisState,
                                                    pVm,
                                                    0,                      // base.
                                                    kernel.c_str(),
                                                    iSp3ProfileType,        // Get the shader kind here.
                                                    NULL,                   // Include ... which I don't think makes sense.
                                                    (unsigned)isaTokensLen, // max_len  (ugly type here... HW team code).
                                                    disassemblerFlags);             // flags

                    GT_IF_WITH_ASSERT(pDisassembledShader != nullptr)
                    {
                        // Fill the output buffer.
                        isa = std::string(pDisassembledShader);
                        UsePlatformNativeLineEndings(isa);

                        // Cleanup.
                        sp3_free(pDisassembledShader);

                        // We are done.
                        ret = beKA::beStatus_SUCCESS;
                    }

                    // Cleanup.
                    sp3_vm_free(pVm);
                }
            }

            // Cleanup.
            sp3_close(pDisState);
        }
    }

    return ret;
}

beKA::beStatus beProgramBuilderGL::GetStatistics(const std::string& device, const std::string& kernel, beKA::AnalysisData& analysis)
{
    GT_UNREFERENCED_PARAMETER(&device);
    GT_UNREFERENCED_PARAMETER(&kernel);

    beStatus ret = beKA::beStatus_General_FAILED;

    if (m_pStatistics != nullptr)
    {
        // *******************************************************************************
        // Amit: This code should be relocated to beKA::AnalysisData as a member function.
        //       This will be done in the future.
        //       Note: This comment was submitted intentionally.
        // *******************************************************************************
        analysis.ISASize = m_pStatistics->CodeLenInByte;

        // Not provided.
        analysis.LDSSizeAvailable = 0;
        analysis.numGPRsUsed = 0;
        analysis.stackSizeAvailable = 0;
        analysis.stackSizeUsed = 0;
        analysis.numSGPRsUsed = 0;
        analysis.wavefrontSize = 0;

        // Compute shaders only.
        analysis.numThreadPerGroup = 0;
        analysis.numThreadPerGroupX = 0;
        analysis.numThreadPerGroupY = 0;
        analysis.numThreadPerGroupZ = 0;

        // Scratch registers.
        analysis.maxScratchRegsNeeded = m_pStatistics->scratchSize;

        // Number of ALU instructions.
        analysis.numAluInst = m_pStatistics->stats.uNumALUInst;

        // Number of flow control instructions.
        analysis.numControlFlowInst = m_pStatistics->stats.uNumCflowInst;

        // Number of fetch instructions.
        analysis.numTfetchInst = m_pStatistics->stats.uNumTfetchInst;

        // Available GPRs.
        analysis.numGPRsAvailable = m_pStatistics->stats.uGPRPoolSize;

        // Used SGPRs.
        analysis.numSGPRsUsed = m_pStatistics->u32NumSgprs;

        // Used VGPRs.
        analysis.numVGPRsUsed = m_pStatistics->u32NumVgprs;

        switch (m_glProfile)
        {
            case GL_GEOMETRY_SHADER:
            {
                AtiElf_SC_SIGSHWSHADER* pGeometryStatistics = (AtiElf_SC_SIGSHWSHADER*)m_pStatistics;
                analysis.LDSSizeUsed = pGeometryStatistics->ldsEsGsSize; // is it the right thing?
                analysis.numVGPRsUsed = pGeometryStatistics->vgtGsInstanceCnt; // // is it the right thing?
            }
            break;

            case GL_FRAGMENT_SHADER:
            {
                // Nothing to do (the next commented line was submitted intentionally).
                //AtiElf_SC_SIPSHWSHADER* pFragmentStatistics = (AtiElf_SC_SIPSHWSHADER*)m_pStatistics;
            }
            break;

            case GL_VERTEX_SHADER:
            {
                AtiElf_SC_SIVSHWSHADER* pVertexStatistics = (AtiElf_SC_SIVSHWSHADER*)m_pStatistics;
                analysis.LDSSizeUsed = pVertexStatistics->targetLdsSize;
            }
            break;

            case GL_COMPUTE_SHADER:
            {
                // AtiElf_SC_SICSHWSHADER* pComputeStatistics = (AtiElf_SC_SICSHWSHADER*)m_pStatistics;
                analysis.LDSSizeUsed = 0; // ???
                analysis.numVGPRsUsed = 0; // ???
            }
            break;

            case GL_TESS_CONTROL_SHADER:
            case GL_TESS_EVALUATION_SHADER:
            {
                // Nothing to add for these shader types.
                analysis.LDSSizeUsed = 0;
                analysis.numVGPRsUsed = 0;
            }
            break;

            default:
                GT_ASSERT_EX(false, L"Unknown shader type.");
                break;
        }

        ret = beKA::beStatus_SUCCESS;
    }

    return ret;
}

bool beProgramBuilderGL::IsInitialized()
{
    return m_IsIntialized;
}

/// Release all data relevant to the previous builds.
void beProgramBuilderGL::ReleaseProgram()
{
    if (m_IsIntialized)
    {
        if (m_pBinaryIL != nullptr)
        {
            delete[] m_pBinaryIL;
            m_pBinaryIL = nullptr;
            m_binaryIlSize = 0;
        }

        if (m_pBinaryISA != nullptr)
        {
            delete[] m_pBinaryISA;
            m_pBinaryISA = nullptr;
            m_binaryIsaSize = 0;
        }
    }
}

/// retrieve all devices as got from the loaded module
beKA::beStatus beProgramBuilderGL::GetDeviceTable(std::vector<GDT_GfxCardInfo>& table) const
{
    table = m_GLDeviceTable;
    return beKA::beStatus_SUCCESS;
}

bool beProgramBuilderGL::CompileOK(std::string& device)
{
    GT_UNREFERENCED_PARAMETER(&device);
    return false;
}

beKA::beStatus beProgramBuilderGL::Compile(const std::string&   programSource, const GLOptions& glOptions, const std::string*   sourcePath)
{
    GT_UNREFERENCED_PARAMETER(sourcePath);
    beStatus ret = beKA::beStatus_OpenGLModuleNotInitialized;

    if (m_IsIntialized)
    {
        ret = beKA::beStatus_General_FAILED;

        if (m_pNativeImpl != nullptr)
        {
            bool isVirtualCtxCreated = m_pNativeImpl->CreateVirtualContext(glOptions);

            if (isVirtualCtxCreated)
            {
                GLuint prog = glCreateProgram();

                if (prog != 0)
                {
                    // Store the profile for later use.
                    m_glProfile = glOptions.m_Profile;

                    // If required, create a vertex shader.
                    if (glOptions.m_Profile != GL_VERTEX_SHADER   &&
                        glOptions.m_Profile != GL_FRAGMENT_SHADER &&
                        glOptions.m_Profile != GL_COMPUTE_SHADER)
                    {
                        // If this is a geometry or tessellation shader, create a dummy vertex shader first.
                        GLuint dummyVertexShader = glCreateShader(GL_VERTEX_SHADER);
                        GT_ASSERT(dummyVertexShader != 0);
                        glAttachShader(prog, dummyVertexShader);

                        const char* pDummyVertexShader = BE_STR_DUMMY_VRTX_SHADER;
                        glShaderSource(dummyVertexShader, 1, &pDummyVertexShader, NULL);
                        glCompileShader(dummyVertexShader);
                        glAttachShader(prog, dummyVertexShader);

                        if (glOptions.m_Profile == GL_TESS_CONTROL_SHADER)
                        {
                            // If this is a tessellation-control shader, we must
                            // create a dummy tessellation evaluation shader due to a driver limitation.
                            GLuint dummyTessEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
                            GT_ASSERT(dummyTessEvalShader != 0);
                            glAttachShader(prog, dummyTessEvalShader);
                            const char* pDummyTessEvalShader = BE_STR_DUMMY_TESS_EVAL_SHADER;
                            glShaderSource(dummyTessEvalShader, 1, &pDummyTessEvalShader, NULL);
                            glCompileShader(dummyTessEvalShader);
                            glAttachShader(prog, dummyTessEvalShader);
                        }
                    }

                    // Now let's compile the shader.
                    GLuint shader = glCreateShader(glOptions.m_Profile);
                    const char* pProgramSource = programSource.c_str();
                    glShaderSource(shader, 1, &pProgramSource, NULL);
                    glCompileShader(shader);

                    GLint param, len;
                    std::string logstr;
                    glGetShaderiv(shader, GL_COMPILE_STATUS, &param);

                    if (param == GL_TRUE)
                    {
                        // Attach the shader.
                        glAttachShader(prog, shader);

                        // Link the program.
                        glLinkProgram(prog);
                        GLint param2, len2;
                        glGetProgramiv(prog, GL_LINK_STATUS, &param2);

                        if (param2 == GL_TRUE)
                        {
                            // This will be used for getting the IL.
                            glProgramParameteri(prog, GL_PROGRAM_BUILD_PARAMETER_AMD, GL_BUILD_ILBIN_AMD);

                            // Get the output binary- this is what we want to save.
                            int binarylength;
                            glGetProgramiv(prog, GL_PROGRAM_BINARY_LENGTH, &binarylength);
                            int nformats;
                            glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &nformats);
                            GLenum formats;

                            std::vector<char> binary;
                            binary.resize(binarylength + 1);
                            GLsizei length;
                            glGetProgramBinary(prog, binarylength, &length, &formats, &binary[0]);

                            if (binary.size() > 0)
                            {
                                // Save the result.
                                m_compiledBinary = binary;

                                // Extract the data we are looking for.
                                bool isDecoded = ELFDumper::singleton().DecodeProgram(&m_compiledBinary[0], (int)m_compiledBinary.size());

                                if (isDecoded)
                                {
                                    ELFDumper::singleton().InitOutputStream("");
                                    ELFDumper::singleton().DumpData();
                                    ELFDumper::singleton().DecodeShader();

#ifdef GL_IL_SUPPORTED
                                    // This is the binary IL.
                                    // Currently, we do not know how to decode it.
                                    m_binaryIlSize = ELFDumper::singleton().GetIlBinarySize();
                                    GT_ASSERT(m_binaryIlSize > 0);

                                    if (m_pBinaryIL != nullptr)
                                    {
                                        delete[] m_pBinaryIL;
                                        m_pBinaryIL = nullptr;
                                    }

                                    m_pBinaryIL = new char[m_binaryIlSize];
                                    ELFDumper::singleton().GetILBinary(m_pBinaryIL);
#endif

                                    // Get the binary ISA. We decode it using SP3.
                                    m_binaryIsaSize = ELFDumper::singleton().GetIsaBinarySize();

                                    GT_IF_WITH_ASSERT(m_binaryIsaSize > 0)
                                    {
                                        if (m_pBinaryISA != nullptr)
                                        {
                                            delete[] m_pBinaryISA;
                                            m_pBinaryISA = nullptr;
                                        }

                                        m_pBinaryISA = new char[m_binaryIsaSize];
                                        ELFDumper::singleton().GetIsaBinary(m_pBinaryISA);

                                        ELFDumper::singleton().GetStatistics(m_statistics);
                                        ELFDumper::singleton().GetStatistics(m_pStatistics);

                                        // For know, ignore the IL.
                                        if (m_pBinaryISA != nullptr)
                                        {
                                            // We are done.
                                            ret = beKA::beStatus_SUCCESS;
                                        }
                                    }

                                    // Clean-up.
                                    ELFDumper::singleton().destroy();
                                }
                                else
                                {
                                    std::stringstream logStream;
                                    logStream << "Failed to decode the binary output" << endl;
                                    LogCallBack(logStream.str());
                                    ret = beKA::beStatus_GLCannotDecodeCompiledElf;
                                }
                            }
                            else
                            {
                                std::stringstream logStream;
                                logStream << "Could not get the compilation binary output" << endl;
                                LogCallBack(logStream.str());
                                return beKA::beStatus_General_FAILED;
                            }
                        }
                        else
                        {
                            glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len2);

                            if (len2 > 0)
                            {
                                char* pLogBuffer = new char[len2];
                                glGetProgramInfoLog(prog, len2, &len2, pLogBuffer);
                                std::stringstream logStream;
                                logStream << "Link Shader failed: " << pLogBuffer << endl;
                                LogCallBack(logStream.str());
                                delete[] pLogBuffer;
                            }
                        }
                    }
                    else
                    {
                        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
                        logstr.resize(len + 1);
                        glGetShaderInfoLog(shader, len, &len, &logstr[0]);
                        std::stringstream logStream;
                        const std::string newLineDelimiter("\n");
                        const std::string warningIndicator("WARNING:");
                        const std::string errorIndicator("ERROR:");
                        // adding source path before warnings and errors
                        std::vector<std::string> errorLines;
                        bool errorsReceived = SplitDelimitedString(logstr, newLineDelimiter, errorLines);

                        if (errorsReceived)
                        {
                            for (const auto& it : errorLines)
                            {
                                if ((it.find(warningIndicator) == 0) || (it.find(errorIndicator) == 0))
                                {
                                    logStream << *sourcePath << " " << it << endl;
                                }
                                else
                                {
                                    logStream << it << endl;
                                }
                            }

                            LogCallBack(logStream.str());
                        }
                    }
                }
                else
                {
                    std::stringstream logStream;
                    logStream << "glCreateProgram failed ";
                    LogCallBack(logStream.str());
                }
            }
        }
        else
        {
            std::stringstream logStream;
            logStream << "Cannot create virtual Context associated to the requested device. ";
            LogCallBack(logStream.str());
        }
    }
    else
    {
        std::stringstream ss;
        ss << "GL Module not initialized";
        LogCallBack(ss.str());
    }

    return ret;
}

bool beProgramBuilderGL::InitAsicsList()
{
    bool ret = false;

    // Populate the sorted device (card) info table.
    std::vector<GDT_GfxCardInfo> cardList;

    if (AMDTDeviceInfoUtils::Instance()->GetAllCardsInHardwareGeneration(GDT_HW_GENERATION_SOUTHERNISLAND, cardList))
    {
        m_GLDeviceTable.insert(m_GLDeviceTable.end(), cardList.begin(), cardList.end());
    }

    if (AMDTDeviceInfoUtils::Instance()->GetAllCardsInHardwareGeneration(GDT_HW_GENERATION_SEAISLAND, cardList))
    {
        m_GLDeviceTable.insert(m_GLDeviceTable.end(), cardList.begin(), cardList.end());
    }

    if (AMDTDeviceInfoUtils::Instance()->GetAllCardsInHardwareGeneration(GDT_HW_GENERATION_VOLCANICISLAND, cardList))
    {
        m_GLDeviceTable.insert(m_GLDeviceTable.end(), cardList.begin(), cardList.end());
    }

    std::sort(m_GLDeviceTable.begin(), m_GLDeviceTable.end(), GfxCardInfoSortPredicate);

    if (!m_GLDeviceTable.empty())
    {
        ret = true;
    }

    return ret;
}

bool beProgramBuilderGL::ConvertProfiletoSP3_shtype(int InProfile, int& OutSP3Type)
{
    bool ret = false;

    switch (InProfile)
    {
        case GL_GEOMETRY_SHADER:
            ret = true;
            OutSP3Type = SP3_SHTYPE_GS;
            break;

        case GL_FRAGMENT_SHADER:
            ret = true;
            OutSP3Type = SP3_SHTYPE_PS;
            break;

        case GL_VERTEX_SHADER:
            ret = true;
            OutSP3Type = SP3_SHTYPE_VS;
            break;

        case GL_COMPUTE_SHADER:
            ret = true;
            OutSP3Type = SP3_SHTYPE_CS;
            break;

        case GL_TESS_CONTROL_SHADER:
            ret = true;
            OutSP3Type = SP3_SHTYPE_HS;
            break;

        case GL_TESS_EVALUATION_SHADER:
            ret = true;
            OutSP3Type = SP3_SHTYPE_ES;
            break;

        default:
            ret = false;
            GT_ASSERT_EX(false, L"Unknown GL shader type.");
            OutSP3Type = SP3_SHTYPE_NONE;
            break;
    }

    // CURRENTLY NOT SUPPORTED: SP3_SHTYPE_HS, SP3_SHTYPE_LS, SP3_SHTYPE_ES

    return ret;
}

bool beProgramBuilderGL::SplitDelimitedString(const std::string& source, const std::string& delimiter, std::vector<std::string>& errorLines)
{
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;

    while ((pos = source.find(delimiter, prev)) != std::string::npos)
    {
        errorLines.push_back(source.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    errorLines.push_back(source.substr(prev));
    return !errorLines.empty();
}

bool beProgramBuilderGL::GetOpenGlVersion(std::string& version) const
{
    bool ret = false;
    version.clear();

#ifdef _WIN32
    const char* pVersion = (const char*)m_OpenGLModule.m_glGetString_fn(GL_VERSION);

    if (pVersion != nullptr)
    {
        version = pVersion;
        ret = true;
    }

#endif // _WIN32


    return ret;
}

