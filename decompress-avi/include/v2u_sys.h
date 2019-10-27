/****************************************************************************
 *
 * $Id: v2u_sys.h 28626 2014-11-07 23:08:42Z zhilin $
 *
 * Copyright (C) 2003-2013 Epiphan Systems Inc. All rights reserved.
 *
 * System specific functions.
 *
 ****************************************************************************/

#ifndef _V2U_SYS_H_
#define _V2U_SYS_H_ 1

#include "v2u_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* Maximum number of VGA2USB devices */
#define MAX_VGA2USB_DEVICE_COUNT 64

/* OS specific device prefix */
#ifdef _WIN32
#  define V2U_DEV_PREFIX      "\\\\.\\"
#  define V2U_DEV_PREFIX_LEN  (4)
#else
#  define V2U_DEV_PREFIX      "/dev/"
#  define V2U_DEV_PREFIX_LEN  (5)
#endif

/* Handle to the driver */
typedef struct _V2U_DRIVER *V2U_DRIVER_HANDLE, *V2U_HANDLE;

/**
 * Opens the driver, returns NULL if fails.
 */
extern V2U_HANDLE v2u_open_driver(void);

/**
 * Opens the driver, returns NULL if fails.
 */
extern V2U_HANDLE v2u_open_driver_device(const char* dev);

/**
 * Opens the driver, returns NULL if fails.
 */
extern V2U_HANDLE v2u_open_driver_idx(int idx);

/**
 * Opens the driver, returns NULL if fails.
 */
extern V2U_HANDLE v2u_open_driver_sn(const char* sn);

/**
 * Opens all available devices (up to maxcount), returns number of devices
 * found.
 */
extern int v2u_open_all_devices(V2U_HANDLE* handles, int maxcount);

/**
 * Returns number of available VGA2USB devices.
 */
#define v2u_count_devices() v2u_open_all_devices(NULL,MAX_VGA2USB_DEVICE_COUNT)

/**
 * Returns OS file descriptor for the existing handle.
 */
extern int v2u_get_fd(V2U_HANDLE handle);

/**
 * Returns OS specific name of the existing handle that identifies it in the
 * device namespace (basically, the file name with OS-specific prefix removed).
 */
extern const char* v2u_get_dev(V2U_HANDLE handle);

/**
 * Sends ioctl to the driver. Returns zero on failure, non-zero on success.
 * Ioctl codes are defined in v2u_ioctl.h
 */
extern V2U_BOOL v2u_ioctl(V2U_HANDLE handle, unsigned long code, void* buf,
                          unsigned int size);

/**
 * Returns serial number of the VGA2USB device
 */
extern V2U_BOOL v2u_getsn(V2U_HANDLE handle, char* buf, int buflen);

/**
 * Returns the property type, V2UPropType_Invalid if key is unknown
 */
extern V2UPropertyType v2u_get_property_type(V2UPropertyKey key);

/**
 * Reads the device property.
 */
extern V2U_BOOL v2u_get_property(V2U_HANDLE handle, V2UPropertyKey key,
                                 V2UPropertyValue* value);

/**
 * Sets the device property.
 */
extern V2U_BOOL v2u_set_property(V2U_HANDLE handle, V2UPropertyKey key,
                                 const V2UPropertyValue* value);

/**
 * Returns the hardware type, V2UProductOther on error.
 */
extern V2UProductType v2u_get_product_type(V2U_HANDLE handle);

/**
 * Detects video mode.
 * Returns zero on failure, non-zero on success.
 */
extern V2U_BOOL v2u_detect_videomode(V2U_HANDLE handle, V2U_VideoMode * vm);

/**
 * Queries grab parameters.
 * Returns zero on failure, non-zero on success.
 */
extern V2U_BOOL v2u_get_grabparams(V2U_HANDLE handle, V2U_GrabParameters* gp);

/**
 * Sets grab parameters.
 * Returns zero on failure, non-zero on success.
 */
extern V2U_BOOL v2u_set_grabparams(V2U_HANDLE handle, const V2U_GrabParameters* gp);

/**
 * Grabs one full frame. Return V2U_GrabFrame structure on success,
 * NULL on failure.
 */
extern V2U_GrabFrame * v2u_grab_frame(V2U_HANDLE handle, int format);

/**
 * Grabs the part of the frame specified by the crop rectangle.
 * Returns V2U_GrabFrame2 structure on success, NULL on failure.
 */
extern V2U_GrabFrame2* v2u_grab_frame2(V2U_HANDLE h, const V2URect* crop, int format);

/**
 * Deallocates V2U_GrabFrame structure allocated by v2u_grab_frame
 */
extern void v2u_free_frame(V2U_GrabFrame* f);

/**
 * Deallocates V2U_GrabFrame2 structure allocated by v2u_grab_frame2
 */
extern void v2u_free_frame2(V2U_GrabFrame2* f);

/*
 * Closes the handle to the driver
 */
extern void v2u_close_driver(V2U_HANDLE handle);

/**
 * Returns current time in milliseconds since January 1st 1970, 00:00 GMT
 */
extern V2U_TIME v2u_time(void);

/**
 * Sleeps for specified amount of time (ms)
 */
extern void v2u_sleep(int ms);

#ifdef __cplusplus
} /* end of extern "C" */
#endif  /* __cplusplus */

#endif /* _V2U_SYS_H_ */
