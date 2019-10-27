/****************************************************************************
 *
 * $Id: v2u_ioctl.h 19092 2012-10-31 02:04:10Z monich $
 *
 * Copyright (C) 2003-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Defines IOCTL interface to the VGA2USB driver. Included by the driver
 * and by the user level code.
 *
 ****************************************************************************/

#ifndef _VGA2USB_IOCTL_H_
#define _VGA2USB_IOCTL_H_ 1

#include "v2u_defs.h"

/* Platform specific magic */
#ifdef _WIN32
#  define FILE_DEVICE_VGA2USB       FILE_DEVICE_UNKNOWN
#  define _IOCTL_VGA2USB(x,a)       CTL_CODE(FILE_DEVICE_VGA2USB,x,METHOD_NEITHER,a)
#  define IOCTL_VGA2USB(x)          _IOCTL_VGA2USB(x,FILE_ANY_ACCESS)
#  define IOCTL_VGA2USB_R(x,type)   _IOCTL_VGA2USB(x,FILE_READ_ACCESS)
#  define IOCTL_VGA2USB_W(x,type)   _IOCTL_VGA2USB(x,FILE_WRITE_ACCESS)
#  define IOCTL_VGA2USB_WR(x,type)  _IOCTL_VGA2USB(x,FILE_ANY_ACCESS)
#else  /* Unix */
#  define IOCTL_VGA2USB(x)          _IO('V',x)
#  define IOCTL_VGA2USB_R(x,type)   _IOR('V',x,type)
#  define IOCTL_VGA2USB_W(x,type)   _IOW('V',x,type)
#  define IOCTL_VGA2USB_WR(x,type)  _IOWR('V',x,type)
#endif /* Unix */

/* IOCTL definitions */

/* 
 * IOCTL_VGA2USB_VIDEOMODE
 *
 * Detects video mode. If cable is disconnected, width and height are zero.
 * Note that the vertical refresh rate is measured in milliHertz. 
 * That is, the number 59900 represents 59.9 Hz.
 *
 * Support: Linux, Windows, MacOS X
 */

#define IOCTL_VGA2USB_VIDEOMODE    IOCTL_VGA2USB_R(9,V2U_VideoMode)

/* 
 * IOCTL_VGA2USB_GETPARAMS
 * IOCTL_VGA2USB_SETPARAMS
 *
 * Support: Linux, Windows, MacOS X
 */

/* Legacy flags names */
#define GRAB_BMP_BOTTOM_UP          V2U_GRAB_BMP_BOTTOM_UP
#define GRAB_PREFER_WIDE_MODE       V2U_GRAB_PREFER_WIDE_MODE
#define GRAB_YCRCB                  V2U_GRAB_YCRCB

#define IOCTL_VGA2USB_GETPARAMS     IOCTL_VGA2USB_R(6,V2U_GrabParameters)
#define IOCTL_VGA2USB_SETPARAMS     IOCTL_VGA2USB_W(8,V2U_GrabParameters)

/* 
 * IOCTL_VGA2USB_GRABFRAME
 *
 * For pixel format check V2U_GRABFRAME_FORMAT_XXX constants in v2u_defs.h
 *
 * Support: Linux, Windows, MacOS X
 */

#define IOCTL_VGA2USB_GRABFRAME     IOCTL_VGA2USB_WR(10,V2U_GrabFrame)
#define IOCTL_VGA2USB_GRABFRAME2    IOCTL_VGA2USB_WR(20,V2U_GrabFrame2)

/*
 * IOCTL_VGA2USB_GETSN
 *
 * Support: Windows, Linux, MacOS X
 */

typedef struct ioctl_getsn {
    char sn[V2U_SN_BUFSIZ]; /* OUT serial number string */
} V2U_PACKED V2U_GetSN;

#define IOCTL_VGA2USB_GETSN         IOCTL_VGA2USB_R(7,V2U_GetSN)

/*
 * IOCTL_VGA2USB_SENDPS2 (KVM2USB capable products only)
 *
 * Support: Windows, Linux, MacOS X
 */

#define IOCTL_VGA2USB_SENDPS2       IOCTL_VGA2USB_W(16,V2U_SendPS2)

/*
 * IOCTL_VGA2USB_GET_PROPERTY
 * IOCTL_VGA2USB_SET_PROPERTY
 *
 * Get or set the value of the specified property.
 *
 * Support: Windows, Linux, MacOS X
 * Required driver version: 3.6.22 or later.
 */

#define IOCTL_VGA2USB_GET_PROPERTY  IOCTL_VGA2USB_WR(18,V2U_Property)
#define IOCTL_VGA2USB_SET_PROPERTY  IOCTL_VGA2USB_W(19,V2U_Property)

#endif /* _VGA2USB_IOCTL_H_ */
