/****************************************************************************
 *
 * $Id: v2u_util.h 22125 2013-05-11 19:41:17Z monich $
 *
 * Copyright (C) 2003-2013 Epiphan Systems Inc. All rights reserved.
 *
 * Header file for miscellaneous utilities.
 *
 ****************************************************************************/

#ifndef _V2U_UTIL_H_
#define _V2U_UTIL_H_ 1

#include "v2u_defs.h"
#include <stdio.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef MIN
#  define MIN(x,y) (((x)<(y))?(x):(y))
#endif /* MIN */

#ifndef MAX
#  define MAX(x,y) (((x)>(y))?(x):(y))
#endif /* MAX */

#define V2U_EDID_SIZE  (128)
#define V2U_EEDID_SIZE (256)

/* Hack for line size (aligned at 32 pixels on daVinci platform) */
#ifndef DAVINCI
#  ifdef __arm__
#    define DAVINCI 1
#  else
#    define DAVINCI 0
#  endif
#endif  /* DAVINCI */

/* V2U_LINE_SIZE macro returns the line size in bytes */
#if DAVINCI
#  define V2U_LINE_SIZE_BPP(w,bpp) ((((w)+31)&(-32))*(bpp)/8)
#else
#  define V2U_LINE_SIZE_BPP(w,bpp) ((w)*(bpp)/8)
#endif /* DAVINCI */
#define V2U_LINE_SIZE(w,fmt) V2U_LINE_SIZE_BPP(w,V2UPALETTE_2_BPP(fmt))

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * Copies BMP row (or a part of it) that came from VGA2USB into 24-bit RGB 
 * row in the format understood by JPEG and PNG libraries. The following image
 * formats are supported:
 *
 *   V2U_GRABFRAME_FORMAT_Y8
 *   V2U_GRABFRAME_FORMAT_RGB4
 *   V2U_GRABFRAME_FORMAT_RGB8
 *   V2U_GRABFRAME_FORMAT_RGB16
 *   V2U_GRABFRAME_FORMAT_BGR16
 *   V2U_GRABFRAME_FORMAT_RGB24
 *   V2U_GRABFRAME_FORMAT_BGR24
 *   V2U_GRABFRAME_FORMAT_ARGB32
 */
extern V2U_BOOL v2u_copy_line(void* dst24, const void* bmp, int fmt, int w);

/**
 * Pixel format converters. Convert a single row from one format to another.
 * Parameters:
 *   dst - points to the destination buffer that receives the requested row
 *         in RGB24 or BGR24 format
 *   src - points the source image
 *   row - index of the row to transform
 *   w   - width of the row in pixels
 *   h   - height of the row in pixels
 *
 * The source image is assumed to be tightly packed, i.e. no padding between
 * the rows.
 */
void v2u_convert_yv12_to_rgb24(void* dst,const void* src,int row,int w,int h);
void v2u_convert_yv12_to_bgr24(void* dst,const void* src,int row,int w,int h);
void v2u_convert_i420_to_rgb24(void* dst,const void* src,int row,int w,int h);
void v2u_convert_i420_to_bgr24(void* dst,const void* src,int row,int w,int h);
void v2u_convert_yuy2_to_rgb24(void* dst,const void* src,int row,int w,int h);
void v2u_convert_yuy2_to_bgr24(void* dst,const void* src,int row,int w,int h);
void v2u_convert_2vuy_to_rgb24(void* dst,const void* src,int row,int w,int h);
void v2u_convert_2vuy_to_bgr24(void* dst,const void* src,int row,int w,int h);
void v2u_convert_nv12_to_rgb24(void* dst,const void* src,int row,int w,int h);
void v2u_convert_nv12_to_bgr24(void* dst,const void* src,int row,int w,int h);

/**
 * Converts a single row from the specified grab format to RGB24. Used by
 * v2u_write_jpeg and v2u_write_png. The following image formats are supported:
 *
 *   V2U_GRABFRAME_FORMAT_Y8
 *   V2U_GRABFRAME_FORMAT_RGB4
 *   V2U_GRABFRAME_FORMAT_RGB8
 *   V2U_GRABFRAME_FORMAT_RGB16
 *   V2U_GRABFRAME_FORMAT_BGR16
 *   V2U_GRABFRAME_FORMAT_RGB24
 *   V2U_GRABFRAME_FORMAT_BGR24
 *   V2U_GRABFRAME_FORMAT_ARGB32
 *   V2U_GRABFRAME_FORMAT_YUY2
 *   V2U_GRABFRAME_FORMAT_2VUY
 *   V2U_GRABFRAME_FORMAT_YV12
 *   V2U_GRABFRAME_FORMAT_I420
 */
extern V2U_BOOL v2u_convert_to_rgb24(void* dst, const void* scr, int fmt,
    int row, int w, int h);

/**
 * Returns user-readable name for the specified device type
 */
extern const char* v2u_product_name(V2UProductType type);

/**
 * Reads text EDID from the stream. The text EDID must be in the
 * Phoenix EDID Designer format. It looks like this:
 *
 * EDID BYTES:
 * 0x   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
 *     ------------------------------------------------
 * 00 | 00 FF FF FF FF FF FF 00 16 08 22 22 00 00 00 00
 * 10 | 08 10 01 03 EE 28 1E 78 0B 01 95 A3 57 4C 9C 25
 * 20 | 12 50 54 EF CF 00 81 C0 8B C0 90 40 B3 00 A9 40
 * 30 | D1 C0 01 01 01 01 C8 32 40 A0 60 B0 23 40 30 20
 * 40 | 35 00 90 2C 11 00 00 18 00 00 00 FF 00 56 33 55
 * 50 | 58 58 58 58 58 0A 0A 0A 0A 0A 00 00 00 FD 00 38
 * 60 | 55 0F 6E 10 00 0A 20 20 20 20 20 20 00 00 00 FC
 * 70 | 00 45 70 69 70 68 61 6E 20 44 32 55 0A 0A 00 14
 */
extern V2U_BOOL v2u_edid_read(FILE* in, V2U_UINT8 edid[/*V2U_EDID_SIZE*/]);

/* edid buffer passed to v2u_edid_read_ext should be 256 bytes long.
 * Function returns EDID size (128 or 256) or zero on failure. */
extern int v2u_edid_read_ext(FILE* in, V2U_UINT8 edid[/*V2U_EEDID_SIZE*/]);

/* output stream for formatted ASCII text */
typedef struct v2u_out_stream_t V2U_OUT_STREAM;

/* used like fputs() */
typedef int (*V2U_TXT_STREAM_WRITER)(const char* txt, V2U_OUT_STREAM* stream);

/**
 * Writes EDID to the stream in the text form (Phoenix EDID Designer format).
 */
extern V2U_BOOL v2u_edid_write_text(V2U_OUT_STREAM* stream,
    V2U_TXT_STREAM_WRITER writer, const V2U_UINT8 edid[/*V2U_EDID_SIZE*/]);
extern V2U_BOOL v2u_edid_write_text2(V2U_OUT_STREAM* stream,
    V2U_TXT_STREAM_WRITER writer, const V2U_UINT8* edid, int len);
    
/**
 * Writes EDID to the stream in the text form (Phoenix EDID Designer format).
 */
extern V2U_BOOL v2u_edid_write(FILE* out, const V2U_UINT8 edid[/*V2U_EDID_SIZE*/]);
extern V2U_BOOL v2u_edid_write_ext(FILE* out, const V2U_UINT8* edid, int len);
  
#ifdef __cplusplus
} /* end of extern "C" */
#endif  /* __cplusplus */

#endif /* _V2U_UTIL_H_ */
