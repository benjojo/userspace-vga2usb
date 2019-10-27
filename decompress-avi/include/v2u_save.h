/****************************************************************************
 *
 * $Id: v2u_save.h 10816 2010-09-02 07:58:50Z monich $
 *
 * Copyright (C) 2003-2010 Epiphan Systems Inc. All rights reserved.
 *
 * Functions for saving image into a file
 *
 ****************************************************************************/

#ifndef _V2U_SAVE_H_
#define _V2U_SAVE_H_ 1

#include "v2u_defs.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* Palettes entry, compatible with RGBQUAD type on Windows */
typedef struct _BmpPalEntry {
    V2U_UINT8 rgbBlue;
    V2U_UINT8 rgbGreen;
    V2U_UINT8 rgbRed;
    V2U_UINT8 rgbReserved;
} BmpPalEntry;

/* VGA2USB palettes */
extern const BmpPalEntry v2u_palette_4bpp[16];
extern const BmpPalEntry v2u_palette_8bpp[256];
extern const BmpPalEntry v2u_palette_y8[256];

/* And 16bpp RGB masks */
extern const V2U_UINT32 v2u_mask_bgr16[3];
extern const V2U_UINT32 v2u_mask_rgb16[3];

/*
 * Writes the image file to the specified stream. Returns V2U_FALSE on failure,
 * V2U_TRUE on success. The fmt parameter defined the input pixel format.
 * The following formats are supported:
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
 *
 * possibly, with V2U_GRABFRAME_BOTTOM_UP_FLAG bit set. The pixels are assumed
 * to be tightly packed, i.e. the line size is NOT aligned at 32-bit boundary.
 */
typedef V2U_BOOL (*V2U_SAVE_PROC)(FILE* f,int w,int h,int fmt, const void* b);
extern V2U_BOOL v2u_write_bmp(FILE* f, int w, int h, int fmt, const void* b);
extern V2U_BOOL v2u_write_jpeg(FILE* f, int w, int h, int fmt, const void* b);
extern V2U_BOOL v2u_write_png(FILE* f, int w, int h, int fmt, const void* b);

#ifdef __cplusplus
} /* end of extern "C" */
#endif  /* __cplusplus */

#endif /* _V2U_SAVE_H_ */
