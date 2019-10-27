/****************************************************************************
 *
 * $Id: v2u_compression.h 8063 2009-11-19 23:11:46Z rekjanov $
 *
 * Copyright (C) 2007-2009 Epiphan Systems Inc. All rights reserved.
 *
 * Defines and implements decompression library for EPIPHAN VGA2USB compression
 *
 ****************************************************************************/

#ifndef _VGA2USB_COMPRESSION_H_
#define _VGA2USB_COMPRESSION_H_ 1

#include "v2u_defs.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifndef V2UDEC_API
#ifdef _WIN32
#ifdef V2UDEC_EXPORTS
#define V2UDEC_API __declspec(dllexport)
#else
#define V2UDEC_API __declspec(dllimport)
#endif
#else
#define V2UDEC_API
#endif
#endif

/**
 * Error codes used as a part of compression SDK
 */
#ifndef EPROTO
#  define EPROTO    105
#endif

#ifndef ENOSR
#  define ENOSR     106
#endif

#ifndef EOK
#  define EOK       0
#endif

/****************************************************************************
 *
 *
 * Public interface to the decompression engine 
 *
 *
 ****************************************************************************/

/**
 * Version of the library and files generatated by the compression algorithm
 */
#define V2U_COMPRESSION_LIB_VERSION		0x00000100


  
/**
 * Context of the files sequence
 * Compression algorithm internaly keeps key frame. Therefore to decode 
 * sequence of frames common context must be created and used during decoding
 * of consiquently captured frames
 */
typedef struct v2udec_ctx* V2U_DECOMPRESSION_LIB_CONTEXT;
V2UDEC_API V2U_DECOMPRESSION_LIB_CONTEXT v2udec_init_context( void );
V2UDEC_API void v2udec_deinit_context( V2U_DECOMPRESSION_LIB_CONTEXT uctx );

/**
 * Functions to obtain information about the compressed frame
 */

/**
 * Minimum header size.
 * Useful when reading in bytestream. After receiving this number of bytes
 * additional information about the frame can be obtained from
 * v2udec_get_framelen() et al.
 * This value can be different for different versions of V2U libraries.
 */
V2UDEC_API V2U_UINT32
v2udec_get_minheaderlen(V2U_DECOMPRESSION_LIB_CONTEXT uctx);

/**
 * Actual header size.
 * Add this to v2udec_get_framelen() to obtain required buffer size to
 * hold one compressed frame.
 * @return 0 on error.
 */
V2UDEC_API V2U_UINT32
v2udec_get_headerlen(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen);

  
/** Timestamp (in milliseconds) of the frame in the framebuf */
V2UDEC_API V2U_TIME
v2udec_get_timestamp(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen);

// Length of the single compressed frame in the framebuf (excluding compression
// header)
V2UDEC_API V2U_UINT32
v2udec_get_framelen(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen);

// Size of the buffer required to decompress the frame in the framebuf
// (function of frame resolution and palette)
V2UDEC_API int
v2udec_get_decompressed_framelen(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen);

// Palette of the frame in the framebuf (currently RGB24 or YUY2)
V2UDEC_API V2U_UINT32
v2udec_get_palette(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen);

// Palette of the frame in the framebuf before decompression
V2UDEC_API V2U_UINT32
v2udec_get_cpalette(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen);

// Set desired palette for the frame in the framebuf
V2UDEC_API V2U_BOOL 
v2udec_set_palette(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen,
    V2U_UINT32 palette);

// Resolution of the frame in the framebuf
V2UDEC_API void 
v2udec_get_frameres(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen,
    V2URect * rect);

// Video mode of the frame in the framebuf,
// that is width and height before cropping
// and vertical refresh rate.
V2UDEC_API void 
v2udec_get_videomode(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen,
		V2U_VideoMode	* vmode);

	
// Keyframe if not 0
V2UDEC_API int
v2udec_is_keyframe(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen);

// Ordered if not 0
V2UDEC_API int
v2udec_is_ordered(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
    int framebuflen);

/**
 * Decompression functions
 */
V2UDEC_API int
v2udec_decompress_frame(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf,
	int framebuflen,
    unsigned char * bufout, // user-allocated buffer for decompressed frame
    int bufoutlen);

V2UDEC_API int
v2udec_decompress_frame2(
    V2U_DECOMPRESSION_LIB_CONTEXT uctx,
    const unsigned char * framebuf, 
	int framebuflen,
    unsigned char * bufout,
    int bufoutlen, 
	const unsigned char *prev_frame,
    int prev_frame_len,
    int just_key_data);

#ifdef __cplusplus
} /* end of extern "C" */
#endif  /* __cplusplus */

#endif //_VGA2USB_COMPRESSION_H_
