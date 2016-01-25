//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file glext-iPhoneAdditions.h 
/// 
//=====================================================================

//------------------------------ glext-iPhoneAdditions.h ------------------------------

#ifndef __GLEXT_IPHONEADDITIONS_H
#define __GLEXT_IPHONEADDITIONS_H


#ifndef ES1_GLEXT_H_GUARD
#define ES1_GLEXT_H_GUARD


#ifdef __cplusplus
extern "C" {
#endif

#define GL_EXT_texture_filter_anisotropic                       1
#define GL_EXT_texture_lod_bias                                 1
#define GL_IMG_read_format                                      1
#define GL_IMG_texture_compression_pvrtc                        1
#define GL_IMG_texture_format_BGRA8888                          1
#define GL_OES_blend_subtract                                   1
#define GL_OES_depth24                                          1
#define GL_OES_framebuffer_object                               1
#define GL_OES_mapbuffer                                        1
#define GL_OES_rgb8_rgba8                                       1
#define GL_OES_texture_mirrored_repeat                          1

#if GL_EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT                           0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT                       0x84FF
#endif

#if GL_EXT_texture_lod_bias
#define GL_MAX_TEXTURE_LOD_BIAS_EXT                             0x84FD
#define GL_TEXTURE_FILTER_CONTROL_EXT                           0x8500
#define GL_TEXTURE_LOD_BIAS_EXT                                 0x8501
#endif

#if GL_IMG_read_format
#define GL_UNSIGNED_SHORT_4_4_4_4_REV                           0x8365
#endif

#if GL_IMG_read_format || GL_IMG_texture_format_BGRA8888
#define GL_BGRA                                                 0x80E1
#endif

#if GL_IMG_texture_compression_pvrtc
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                      0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                     0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                     0x8C03
#endif

#if GL_OES_blend_subtract
#define GL_BLEND_EQUATION_OES                                   0x8009
#define GL_FUNC_ADD_OES                                         0x8006
#define GL_FUNC_SUBTRACT_OES                                    0x800A
#define GL_FUNC_REVERSE_SUBTRACT_OES                            0x800B
#endif

#if GL_OES_depth24
#define GL_DEPTH_COMPONENT24_OES                                0x81A6
#endif

#if GL_OES_framebuffer_object
#define GL_FRAMEBUFFER_OES                                      0x8D40
#define GL_RENDERBUFFER_OES                                     0x8D41
#define GL_RGBA4_OES                                            0x8056
#define GL_RGB5_A1_OES                                          0x8057
#define GL_RGB565_OES                                           0x8D62
#define GL_DEPTH_COMPONENT16_OES                                0x81A5
#define GL_RENDERBUFFER_WIDTH_OES                               0x8D42
#define GL_RENDERBUFFER_HEIGHT_OES                              0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_OES                     0x8D44
#define GL_RENDERBUFFER_RED_SIZE_OES                            0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE_OES                          0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE_OES                           0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE_OES                          0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE_OES                          0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE_OES                        0x8D55
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES               0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES               0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES             0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES     0x8CD3
#define GL_COLOR_ATTACHMENT0_OES                                0x8CE0
#define GL_DEPTH_ATTACHMENT_OES                                 0x8D00
#define GL_STENCIL_ATTACHMENT_OES                               0x8D20
#define GL_FRAMEBUFFER_COMPLETE_OES                             0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES                0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES        0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES                0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES                   0x8CDA
#define GL_FRAMEBUFFER_UNSUPPORTED_OES                          0x8CDD
#define GL_FRAMEBUFFER_BINDING_OES                              0x8CA6
#define GL_RENDERBUFFER_BINDING_OES                             0x8CA7
#define GL_MAX_RENDERBUFFER_SIZE_OES                            0x84E8
#define GL_INVALID_FRAMEBUFFER_OPERATION_OES                    0x0506
#endif

#if GL_OES_mapbuffer
#define GL_WRITE_ONLY_OES                                       0x88B9
#define GL_BUFFER_ACCESS_OES                                    0x88BB
#define GL_BUFFER_MAPPED_OES                                    0x88BC
#define GL_BUFFER_MAP_POINTER_OES                               0x88BD
#endif

#if GL_OES_rgb8_rgba8
#define GL_RGB8_OES                                             0x8051
#define GL_RGBA8_OES                                            0x8058
#endif

#if GL_OES_texture_mirrored_repeat
#define GL_MIRRORED_REPEAT_OES                                  0x8370
#endif

/*************************************************************/

#if GL_OES_blend_subtract
GL_API void GL_APIENTRY glBlendEquationOES(GLenum mode);
#endif

#if GL_OES_framebuffer_object
GL_API GLboolean GL_APIENTRY glIsRenderbufferOES(GLuint renderbuffer);
GL_API void GL_APIENTRY glBindRenderbufferOES(GLenum target, GLuint renderbuffer);
GL_API void GL_APIENTRY glDeleteRenderbuffersOES(GLsizei n, const GLuint* renderbuffers);
GL_API void GL_APIENTRY glGenRenderbuffersOES(GLsizei n, GLuint* renderbuffers);
GL_API void GL_APIENTRY glRenderbufferStorageOES(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
GL_API void GL_APIENTRY glGetRenderbufferParameterivOES(GLenum target, GLenum pname, GLint* params);
GL_API GLboolean GL_APIENTRY glIsFramebufferOES(GLuint framebuffer);
GL_API void GL_APIENTRY glBindFramebufferOES(GLenum target, GLuint framebuffer);
GL_API void GL_APIENTRY glDeleteFramebuffersOES(GLsizei n, const GLuint* framebuffers);
GL_API void GL_APIENTRY glGenFramebuffersOES(GLsizei n, GLuint* framebuffers);
GL_API GLenum GL_APIENTRY glCheckFramebufferStatusOES(GLenum target);
GL_API void GL_APIENTRY glFramebufferRenderbufferOES(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GL_API void GL_APIENTRY glFramebufferTexture2DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GL_API void GL_APIENTRY glGetFramebufferAttachmentParameterivOES(GLenum target, GLenum attachment, GLenum pname, GLint* params);
GL_API void GL_APIENTRY glGenerateMipmapOES(GLenum target);
#endif

#if GL_OES_mapbuffer
GL_API void GL_APIENTRY glGetBufferPointervOES(GLenum target, GLenum pname, GLvoid** params);
GL_API GLvoid* GL_APIENTRY glMapBufferOES(GLenum target, GLenum access);
GL_API GLboolean GL_APIENTRY glUnmapBufferOES(GLenum target);
#endif

//////////////////////////////////////////////////////////////////////////
// OpenGL ES 2.0-only functions (from ES2/gl.h):
//////////////////////////////////////////////////////////////////////////
GL_API void GL_APIENTRY glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
GL_API void GL_APIENTRY glReleaseShaderCompiler(void);
GL_API void GL_APIENTRY glShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length);

#ifdef __cplusplus
}
#endif

#endif /* ES1_GLEXT_H_GUARD */

#endif //__GLEXT_IPHONEADDITIONS_H
