/****************************************************************************
 *
 * $Id: v2u_defs.h 30225 2015-03-18 20:36:07Z pzeldin $
 *
 * Copyright (C) 2003-2015 Epiphan Systems Inc. All rights reserved.
 *
 * VGA2USB common defines and types
 *
 ****************************************************************************/

#ifndef _VGA2USB_DEFS_H_
#define _VGA2USB_DEFS_H_ 1

/* Required system headers */
#if !defined(__KERNEL__) && !defined(__C51__) && !defined(__FX3__)
#  ifdef _WIN32
#    include <windows.h>
#    include <winioctl.h>
#  else  /* Unix */
#    include <stdint.h>
#    include <sys/ioctl.h>
#  endif /* Unix */
#endif /* !__KERNEL__ && !__C51__ && !__FX3__*/

/* Turn packing on */
#include "v2u_pack_on.h"

/* Basic data types */
#ifdef _WIN32
typedef __int8 V2U_INT8;
typedef unsigned __int8 V2U_UINT8;
typedef __int16 V2U_INT16;
typedef unsigned __int16 V2U_UINT16;
typedef __int32 V2U_INT32;
typedef unsigned __int32 V2U_UINT32;
typedef __int64 V2U_INT64;
typedef unsigned __int64 V2U_UINT64;
#elif defined(__C51__)
typedef char V2U_INT8;
typedef unsigned char V2U_UINT8;
typedef short V2U_INT16;
typedef unsigned short V2U_UINT16;
typedef long V2U_INT32;
typedef unsigned long V2U_UINT32;
/* V2U_INT64 and V2U_UINT64 should not be used on FX2 microcontroller */
typedef BYTE V2U_INT64[8];
typedef BYTE V2U_UINT64[8];
#else
typedef int8_t V2U_INT8;
typedef uint8_t V2U_UINT8;
typedef int16_t V2U_INT16;
typedef uint16_t V2U_UINT16;
typedef int32_t V2U_INT32;
typedef uint32_t V2U_UINT32;
typedef int64_t V2U_INT64;
typedef uint64_t V2U_UINT64;
#endif

typedef const char* V2U_STR;
typedef V2U_UINT8 V2U_BYTE;
typedef V2U_UINT16 V2U_UCS2;
typedef V2U_INT32 V2U_BOOL;
#if !defined(__C51__)
typedef V2U_INT64 V2U_TIME;
#endif

#define V2U_TRUE  1
#define V2U_FALSE 0

typedef struct v2u_size {
    V2U_INT32 width;
    V2U_INT32 height;
} V2U_PACKED V2USize;

typedef struct v2u_rect {
    V2U_INT32 x;
    V2U_INT32 y;
    V2U_INT32 width;
    V2U_INT32 height;
} V2U_PACKED V2URect;

typedef struct v2u_str_ucs2 {
    V2U_UCS2* buffer;       /* not necessarily NULL-terminated */
    V2U_UINT32 len;         /* string length, in characters */
    V2U_UINT32 maxlen;      /* buffer size, in characters */
} V2U_PACKED V2UStrUcs2;

/*
 * V2U_VideoMode
 *
 * Video mode descriptor.
 *
 * Note that the vertical refresh rate is measured in milliHertz. 
 * That is, the number 59900 represents 59.9 Hz.
 */
typedef struct ioctl_videomode {
    V2U_INT32  width;       /* screen width, pixels */
    V2U_INT32  height;      /* screen height, pixels */
    V2U_INT32  vfreq;       /* vertical refresh rate, mHz */
} V2U_PACKED V2U_VideoMode, V2UVideoMode;

/* 
 * V2U_GrabParameters
 *
 * VGA capture parameters.
 *
 * Gain = Contrast
 * Offset =  Brightness
 */
typedef struct ioctl_setparams {
    V2U_UINT32 flags;       /* Validity flags for fields below             */
                            /* When any of the fields below is used,       */
                            /* corresponding V2U_FLAG_VALID_* flag  is set */
                            /* otherwise the field is ignored              */
    V2U_INT32  hshift;      /* Shifts image left (<0) or right(>0).        */
                            /* Valid range depends on the video mode.      */
                            /* Invalid values are rounded to the nearest   */
                            /* valid value                                 */
    V2U_UINT8  phase;       /* Pixel sampling phase, [0,31]                */
    V2U_UINT8  gain_r;      /* Gain for the red channel, [0,255]           */
    V2U_UINT8  gain_g;      /* Gain for the green channel, [0,255]         */
    V2U_UINT8  gain_b;      /* Gain for the blue channel, [0,255]          */
    V2U_UINT8  offset_r;    /* Offset for the red channel, [0,63]          */
    V2U_UINT8  offset_g;    /* Offset for the green channel, [0,63]        */
    V2U_UINT8  offset_b;    /* Offset for the blue channel, [0,63]         */
    V2U_UINT8  reserved;    /* added for alignment, don't use              */
    V2U_INT32  vshift;      /* Shifts image up or down                     */
                            /* Valid range depends on the video mode.      */
                            /* Invalid values are rounded to the nearest   */
                            /* valid value                                 */
    V2U_INT32  pllshift;    /* Adjusts the PLL value                       */
    V2U_UINT32 grab_flags;  /* Sets grab_flags V2U_GRAB_*                  */
    V2U_UINT32 grab_flags_mask; /* Marks which bits from grab_flags are used */
} V2U_PACKED V2U_GrabParameters, V2UGrabParameters;

/* Indicates that hshift field is used */
#define V2U_FLAG_VALID_HSHIFT       0x0001
/* Indicates that phase field is used */
#define V2U_FLAG_VALID_PHASE        0x0002
/* Indicates that all gain_{rgb} and offset_{rgb} fields are used */
#define V2U_FLAG_VALID_OFFSETGAIN   0x0004
/* Indicates that vshift field is used */
#define V2U_FLAG_VALID_VSHIFT       0x0008
/* Indicates that pllshift field is used */
#define V2U_FLAG_VALID_PLLSHIFT     0x0010
/* Indicates that grab_flags and grab_flags_mask are used */
#define V2U_FLAG_VALID_GRABFLAGS    0x0020

/* Flags allowed in grab_flags and grab_flags_mask fields */
/* Grab image upside-down */
#define V2U_GRAB_BMP_BOTTOM_UP      0x10000
/* Sometimes 4:3 and wide modes with the same height are indistinguishable, 
 * so this flag can force choosing wide mode */
#define V2U_GRAB_PREFER_WIDE_MODE   0x20000
/* We have no way to distinguish Component Video (YCrCb) from RGB+Sync-on-Green,
 * so this flag can force YCrCb->RGB conversion on input */
#define V2U_GRAB_YCRCB              0x40000
/* Swap fields in the interfaced signal. Supported since 3.28.1.7 */
#define V2U_GRAB_SWAP_FIELDS        0x80000

/* 
 * The ranges below are obsolete. They depend on the frame grabber model
 * as well as the video mode. Use V2UKey_AdjustmentsRange property to get
 * the ranges that are valid for the parcular frame grabber and current
 * video mode.
 */
#define V2U_MIN_PHASE     0
#define V2U_MAX_PHASE     31
#define V2U_MIN_GAIN      0
#define V2U_MAX_GAIN      255
#define V2U_MIN_OFFSET    0
#define V2U_MAX_OFFSET    63

/* 
 * V2U_SendPS2
 *
 * PS/2 packet descriptor.
 */
typedef struct ioctl_sendps2 {
    V2U_INT16 addr;
    V2U_INT16 len;
    V2U_BYTE buf[64];
} V2U_PACKED V2U_SendPS2, V2USendPS2;

#define V2U_PS2ADDR_KEYBOARD        0x01
#define V2U_PS2ADDR_MOUSE           0x02

/* 
 * Serial number length.
 */

#define V2U_SN_BUFSIZ 32

/* 
 * Product type.
 */
typedef enum v2u_product_type {
    V2UProductOther,
    V2UProductVGA2USB,
    V2UProductKVM2USB,
    V2UProductDVI2USB,
    V2UProductVGA2USBPro,
    V2UProductVGA2USBLR,
    V2UProductDVI2USBSolo,
    V2UProductDVI2USBDuo,
    V2UProductKVM2USBPro,
    V2UProductKVM2USBLR,
    V2UProductDVI2USBRespin,
    V2UProductVGA2USBHR,
    V2UProductVGA2USBLRRespin,
    V2UProductVGA2USBHRRespin,
    V2UProductVGA2USBProRespin,
    V2UProductVGA2FIFO,
    V2UProductKVM2FIFO,
    V2UProductDVI2FIFO,
    V2UProductDVI2Davinci1,
    V2UProductVGA2PCI,
    V2UProductGioconda,
    V2UProductDVI2PCI,
    V2UProductKVM2USBLRRespin,
    V2UProductHDMI2PCI,
    V2UProductDVI2PCIStd,
    V2UProductDVI2USB3,
    V2UProductDVI2PCIGen2,
    V2UProductDVI2USB3ET,
    V2UProductSDI2USB3,
    V2UProductDVI2PCIGII,
    V2UProductCount                 /* Number of known product types */
} V2UProductType;

/*
 * Resize algorithms.
 */
typedef enum v2u_scale_mode {
    V2UScaleNone,                   /* No scaling */
    V2UScaleModeNearestNeighbor,    /* Nearest neighbor algorithm */
    V2UScaleModeWeightedAverage,    /* Weighted average algorithm */
    V2UScaleModeFastBilinear,       /* Fast bilinear */
    V2UScaleModeBilinear,           /* Bilinear */
    V2UScaleModeBicubic,            /* Bicubic */
    V2UScaleModeExperimental,       /* Experimental */
    V2UScaleModePoint,              /* Nearest neighbour# 2 */
    V2UScaleModeArea,               /* Weighted average */
    V2UScaleModeBicubLin,           /* Luma bicubic, chroma bilinear */
    V2UScaleModeSinc,               /* Sinc */
    V2UScaleModeLanczos,            /* Lanczos */
    V2UScaleModeSpline,             /* Natural bicubic spline */
    V2UScaleModeHardware,           /* Scale in hardware, if supported */
    V2UScaleModeCount               /* Number of valid modes */
} V2UScaleMode;

/* Macros to translate V2UScaleMode to capture flags and back */
#define V2U_SCALE_MODE_TO_FLAGS(_m) \
    (((_m) << 16) & V2U_GRABFRAME_SCALE_MASK)
#define V2U_SCALE_FLAGS_TO_MODE(_f) \
    ((V2UScaleMode)(((_f) & V2U_GRABFRAME_SCALE_MASK) >> 16))

typedef enum v2u_rotation_mode {
    V2URotationNone,                /* V2U_GRABFRAME_ROTATION_NONE */
    V2URotationLeft90,              /* V2U_GRABFRAME_ROTATION_LEFT90 */
    V2URotationRight90,             /* V2U_GRABFRAME_ROTATION_RIGHT90 */
    V2URotation180,                 /* V2U_GRABFRAME_ROTATION_180 */
    V2URotationCount                /* Number of valid rotation types */
} V2URotationMode;

/* Macros to translate V2URotationMode to capture flags and back */
#define V2U_ROTATION_MODE_TO_FLAGS(_m) \
    (((_m) << 20) & V2U_GRABFRAME_ROTATION_MASK)
#define V2U_ROTATION_FLAGS_TO_MODE(_f) \
    ((V2URotationMode)(((_f) & V2U_GRABFRAME_ROTATION_MASK) >> 20))

typedef struct v2u_adjustment_range {
    V2U_UINT32 flags;
    V2U_UINT32 valid_flags;
    V2U_INT16 hshift_min;
    V2U_INT16 hshift_max;
    V2U_INT16 phase_min;
    V2U_INT16 phase_max;
    V2U_INT16 offset_min;
    V2U_INT16 offset_max;
    V2U_INT16 gain_min;
    V2U_INT16 gain_max;
    V2U_INT16 vshift_min;
    V2U_INT16 vshift_max;
    V2U_INT16 pll_min;
    V2U_INT16 pll_max;
} V2U_PACKED V2UAdjRange;

/* Video mode descriptor */
typedef struct vesa_videomode {
    V2U_UINT32 VerFrequency;        /* mHz */
    V2U_UINT16 HorAddrTime;         /* pixels */
    V2U_UINT16 HorFrontPorch;       /* pixels */
    V2U_UINT16 HorSyncTime;         /* pixels */
    V2U_UINT16 HorBackPorch;        /* pixels */
    V2U_UINT16 VerAddrTime;         /* lines */
    V2U_UINT16 VerFrontPorch;       /* lines */
    V2U_UINT16 VerSyncTime;         /* lines */
    V2U_UINT16 VerBackPorch;        /* lines */
    V2U_UINT32 Type;                /* flags, see below */

#define VIDEOMODE_TYPE_VALID            0x01
#define VIDEOMODE_TYPE_ENABLED          0x02
#define VIDEOMODE_TYPE_SUPPORTED        0x04
#define VIDEOMODE_TYPE_DUALLINK         0x08
#define VIDEOMODE_TYPE_DIGITAL          0x10
#define VIDEOMODE_TYPE_INTERLACED       0x20
#define VIDEOMODE_TYPE_HSYNCPOSITIVE    0x40
#define VIDEOMODE_TYPE_VSYNCPOSITIVE    0x80
#define VIDEOMODE_TYPE_TOPFIELDFIRST    0x100

} V2U_PACKED V2UVideoModeDescr;
typedef const V2UVideoModeDescr* V2UVideoModeDescrCPtr;

/* Maximum number of custom videomodes */
#define V2U_CUSTOM_VIDEOMODE_COUNT      8

/*
 * The first 8 (V2U_CUSTOM_VIDEOMODE_COUNT) entries in the video mode table
 * are reserved for custom video modes. The remaining entries are standard
 * video modes.
 */
typedef struct custom_videomode {
    V2U_INT32 idx;
    V2UVideoModeDescr vesa_mode;
} V2U_PACKED V2UVGAMode;

typedef struct v2u_version {
    V2U_INT32 major;
    V2U_INT32 minor;
    V2U_INT32 micro;
    V2U_INT32 nano;
} V2U_PACKED V2UVersion;

/* 
 * Property keys and types
 */

/* V2UKey_DirectShowFlags */
#define V2U_DSHOW_LIMIT_FPS         0x200
#define V2U_DSHOW_FLIP_VERTICALLY   0x400
#define V2U_DSHOW_FIX_FPS           0x800

/* V2UKey_DShowCompatMode */
/* V2UKey_DShowActiveCompatMode */
typedef enum v2u_dshow_compat_mode {
    V2UDShowCompatMode_Invalid = -1,    /* Invalid value */
    V2UDShowCompatMode_Auto,            /* Automatic action (default) */
    V2UDShowCompatMode_None,            /* No tweaks */
    V2UDShowCompatMode_Skype,           /* Skype compatiblity */
    V2UDShowCompatMode_WebEx,           /* WebEx compatibility */
    V2UDShowCompatMode_Count            /* Number of valid values */
} V2UDShowCompatMode;

/* Type of video input (V2UKey_InputSignalType) */
#define V2U_INPUT_NONE              0x00
#define V2U_INPUT_ANALOG            0x01
#define V2U_INPUT_DIGITAL           0x02
#define V2U_INPUT_SOG               0x04
#define V2U_INPUT_COMPOSITE         0x08
#define V2U_INPUT_ENCRYPTED         0x10
#define V2U_INPUT_SOG3              0x20

/* V2UKey_DigitalModeDetect */
typedef enum v2u_digital_mode_detect {
    V2UDigitalMode_AutoDetect,      /* Automatic detection (default) */
    V2UDigitalMode_SingleLink,      /* Force single link */
    V2UDigitalMode_DualLink,        /* Force dual link */
    V2UDigitalMode_Count            /* Not a valid value */
} V2UDigitalModeDetect;

/* V2UKey_NoiseFilter */
typedef enum v2u_noise_filter {
    V2UNoiseFilter_Auto,            /* Automatic configuration (default) */
    V2UNoiseFilter_None,            /* Disable noise filter */
    V2UNoiseFilter_Low,             /* Good for Apple */
    V2UNoiseFilter_Moderate,
    V2UNoiseFilter_High,
    V2UNoiseFilter_Extreme,         /* Good for black and white */
    V2UNoiseFilter_Count            /* Not a valid value */
} V2UNoiseFilter;

/* V2UKey_BusType */
typedef enum v2u_bus_type {
    V2UBusType_Other,               /* Doesn't fall into any known category */
    V2UBusType_USB,                 /* USB bus */
    V2UBusType_PCI,                 /* PCI (Express) bus */
    V2UBusType_VPFE,                /* Davinci platform bus */
    V2UBusType_Count                /* Not a valid value */
} V2UBusType;

/* V2UKey_UsbSpeed */
typedef enum v2u_usb_speed {
    V2U_USB_SPEED_MIN = 1,
    V2U_Usb1 = V2U_USB_SPEED_MIN,
    V2U_Usb2,
    V2U_Usb3,
    V2U_USB_SPEED_MAX = V2U_Usb3
} V2U_USB_SPEED;

/* H/VSync threshold values */
#define V2U_MIN_SYNC_THRESHOLD      0
#define V2U_MAX_SYNC_THRESHOLD      255
#define V2U_DEFAULT_SYNC_THRESHOLD  128

/* ResetADC options */
#define ResetADC_Reset              0x01
#define ResetADC_PowerDown          0x02
#define ResetADC_PowerUp            0x03

/* Divide the value of V2UKey_DirectShowMaxFps by 100 to get the fps value */
#define V2U_FPS_DENOMINATOR         100

/* Device capabilities for V2UKey_DeviceCaps */
#define V2U_CAPS_VGA_CAPTURE      0x00000001  /* Captures VGA signal */
#define V2U_CAPS_DVI_CAPTURE      0x00000002  /* Captures DVI single-link */
#define V2U_CAPS_DVI_DUAL_LINK    0x00000004  /* Captures DVI dual-link */
#define V2U_CAPS_KVM              0x00000008  /* KVM functionality */
#define V2U_CAPS_EDID             0x00000010  /* Programmable EDID */
#define V2U_CAPS_HW_COMPRESSION   0x00000020  /* On-board compression */
#define V2U_CAPS_SYNC_THRESHOLD   0x00000040  /* (obsolete) */
#define V2U_CAPS_HW_SCALE         0x00000080  /* Hardware scale */
#define V2U_CAPS_SIGNATURE        0x00000100  /* Signed hardware */
#define V2U_CAPS_SD_VIDEO         0x00000200  /* Captures SD video */
#define V2U_CAPS_MULTIGRAB        0x00000400  /* Several grabbers on board */
#define V2U_CAPS_AUDIO_CAPTURE    0x00000800  /* Captures audio */
#define V2U_CAPS_HSYNC_THRESHOLD  0x00001000  /* Adjustable hsync threshold */
#define V2U_CAPS_VSYNC_THRESHOLD  0x00002000  /* Adjustable vsync threshold */
#define V2U_CAPS_SDI_CAPTURE      0x00004000  /* Capture SDI signal */
#define V2U_CAPS_VIDEO_CAPTURE    0x00008000  /* Any sort of video capture */
#define V2U_CAPS_COMPONENT_VIDEO  0x00010000  /* Captures Component Video */
#define V2U_CAPS_TEMPERATURE      0x00020000  /* V2UKey_Temperature support */

/* V2UKey_VideoFormat. For grabbers with V2U_CAPS_SD_VIDEO capability */
typedef enum {
    V2UVideoFormat_Unknown = -1,
    V2UVideoFormat_SVideo,
    V2UVideoFormat_Composite,
    V2UVideoFormat_Count
} V2UVideoFormat;

typedef enum v2u_property_access {
    V2UPropAccess_NO,               /* unaccesible */
    V2UPropAccess_RO,               /* Read only */
    V2UPropAccess_RW,               /* Read/Write */
    V2UPropAccess_WO                /* Write only */
} V2UPropertyAccess;

#define V2U_PROPERTY_TYPE_LIST(type) \
    /*    Name          Field     */ \
    type( Int8,         int8       ) \
    type( Int16,        int16      ) \
    type( Int32,        int32      ) \
    type( Boolean,      boolean    ) \
    type( Size,         size       ) \
    type( Rect,         rect       ) \
    type( Version,      version    ) \
    type( Binary,       blob       ) \
    type( EDID,         edid       ) \
    type( AdjustRange,  adj_range  ) \
    type( VGAMode,      vgamode    ) \
    type( String,       str        ) \
    type( StrUcs2,      wstr       ) \
    type( Uint8,        uint8      ) \
    type( Uint16,       uint16     ) \
    type( Uint32,       uint32     ) \
    type( Reserved,     int8       ) \
    type( UserData,     userdata   ) \
    type( Int64,        int64      ) \
    type( Uint64,       uint64     ) \
    type( VESAMode,     vesa_mode  ) \
    type( String2,      str2       )

/* Define V2UPropertyType enum */
typedef enum v2u_property_type {
    V2UPropType_Invalid = -1,       /* Not a valid type */
#define V2U_PROPERTY_TYPE_ENUM(name,field) V2UPropType_##name,
    V2U_PROPERTY_TYPE_LIST(V2U_PROPERTY_TYPE_ENUM)
#undef V2U_PROPERTY_TYPE_ENUM
    V2UPropType_Count               /* Number of valid types */
} V2UPropertyType;

#define V2UPropType_Enum    V2UPropType_Int32
#define V2UKey_UsbProductID V2UKey_ProductID

#define V2U_PROPERTY_LIST(property) \
    property( V2UKey_ProductID,             \
              "Product id",                 \
              V2UPropType_Int16,            \
              V2UPropAccess_RO)             \
    property( V2UKey_ProductType,           \
              "Product type",               \
              V2UPropType_Enum,             \
              V2UPropAccess_RO)             \
    property( V2UKey_DirectShowFixRes,      /* Windows only */\
              "DS Fix Resolution",          \
              V2UPropType_Size,             \
              V2UPropAccess_RW)             \
    property( V2UKey_DirectShowFlags,       /* Windows only */\
              "DS Flags",                   \
              V2UPropType_Int32,            \
              V2UPropAccess_RW)             \
    property( V2UKey_DirectShowDefaultBmp,  /* Windows only */\
              "DS Default BMP",             \
              V2UPropType_StrUcs2,          \
              V2UPropAccess_RW)             \
    property( V2UKey_ModeMeasurmentsDump,   \
              "Mode Measurments dump",      \
              V2UPropType_Binary,           \
              V2UPropAccess_RO)             \
    property( V2UKey_ResetADC,              /* Not really a property */\
              "Reset ADC",                  \
              V2UPropType_Int32,            \
              V2UPropAccess_WO)             \
    property( V2UKey_DirectShowScaleMode,   /* Windows only */\
              "DirectShow Scale Mode",      \
              V2UPropType_Enum,             \
              V2UPropAccess_RW)             \
    property( V2UKey_HardwareCompression,   \
              "Hardware Compression",       \
              V2UPropType_Boolean,          \
              V2UPropAccess_RO)             \
    property( V2UKey_AdjustmentsRange,      \
              "Adjustments Range",          \
              V2UPropType_AdjustRange,      \
              V2UPropAccess_RO)             \
    property( V2UKey_Version,               \
              "Version",                    \
              V2UPropType_Version,          \
              V2UPropAccess_RO)             \
    property( V2UKey_EDID,                  \
              "EDID",                       \
              V2UPropType_EDID,             \
              V2UPropAccess_RW)             \
    property( V2UKey_DirectShowMaxFps,      /* Windows only */\
              "DS Max fps",                 \
              V2UPropType_Int32,            \
              V2UPropAccess_RW)             \
    property( V2UKey_KVMCapable,            \
              "KVM capable",                \
              V2UPropType_Boolean,          \
              V2UPropAccess_RO)             \
    property( V2UKey_VGAMode,               \
              "VGA mode",                   \
              V2UPropType_VGAMode,          \
              V2UPropAccess_RW)             \
    property( V2UKey_CurrentVGAMode,        /* Since 3.24.6 */\
              "Current VGA mode",           \
              V2UPropType_VESAMode,         \
              V2UPropAccess_RO)             \
    property( V2UKey_ModeMeasureInterval,   \
              "Mode Measure interval",      \
              V2UPropType_Int32,            \
              V2UPropAccess_RW)             \
    property( V2UKey_EDIDSupport,           \
              "EDID support",               \
              V2UPropType_Boolean,          \
              V2UPropAccess_RO)             \
    property( V2UKey_ProductName,           \
              "Product name",               \
              V2UPropType_String,           \
              V2UPropAccess_RO)             \
    property( V2UKey_TuneInterval,          /* ms */\
              "Tune interval",              \
              V2UPropType_Uint32,           \
              V2UPropAccess_RW)             \
    property( V2UKey_UserData,              \
              "User data",                  \
              V2UPropType_UserData,         \
              V2UPropAccess_RW)             \
    property( V2UKey_SerialNumber,          \
              "Serial number",              \
              V2UPropType_String,           \
              V2UPropAccess_RO)             \
    property( V2UKey_InputSignalType,       /* Since 3.23.7.3 */\
              "Input signal type",          \
              V2UPropType_Uint32,           \
              V2UPropAccess_RO)             \
    property( V2UKey_DigitalModeDetect,     /* Since 3.24.5 */\
              "Digital mode detection",     \
              V2UPropType_Enum,             \
              V2UPropAccess_RW)             \
    property( V2UKey_NoiseFilter,           /* Since 3.24.6 */\
              "Noise filter",               \
              V2UPropType_Enum,             \
              V2UPropAccess_RW)             \
    property( V2UKey_HSyncThreshold,        /* Since 3.24.6 */\
              "Hsync threshold",            \
              V2UPropType_Uint8,            \
              V2UPropAccess_RW)             \
    property( V2UKey_VSyncThreshold,        /* Since 3.24.6 */\
              "Vsync threshold",            \
              V2UPropType_Uint8,            \
              V2UPropAccess_RW)             \
    property( V2UKey_DeviceCaps,            /* Since 3.24.6 */\
              "Device capabilities",        \
              V2UPropType_Uint32,           \
              V2UPropAccess_RO)             \
    property( V2UKey_DirectShowDefaultBmp2, /* Since 3.24.9.4, Windows */\
              "DS Default BMP",             \
              V2UPropType_String2,          \
              V2UPropAccess_RW)             \
    property( V2UKey_BusType,               /* Since 3.24.9.5 */\
              "Grabber bus type",           \
              V2UPropType_Enum,             \
              V2UPropAccess_RO)             \
    property( V2UKey_GrabberId,             /* Since 3.27.4 */\
              "Grabber ID",                 \
              V2UPropType_String,           \
              V2UPropAccess_RO)             \
    property( V2UKey_VideoFormat,           /* Since 3.27.4 */\
              "Video input format",         \
              V2UPropType_Enum,             \
              V2UPropAccess_RW)             \
    property( V2UKey_DShowCompatMode,       /* Since 3.27.10, Windows */\
              "DShow compatiblity mode",    \
              V2UPropType_Enum,             \
              V2UPropAccess_RW)             \
    property( V2UKey_DShowActiveCompatMode, /* Since 3.27.10, Windows */\
              "Active compatiblity mode",   \
              V2UPropType_Enum,             \
              V2UPropAccess_RO)             \
    property( V2UKey_EEDID,                 /* Since 3.27.14.6 */\
              "E-EDID",                     \
              V2UPropType_Binary,           \
              V2UPropAccess_RW)             \
    property( V2UKey_LogLevel,              /* Since 3.28.1.1 */\
              "Log level",                  \
              V2UPropType_Int32,            \
              V2UPropAccess_RW)             \
    property( V2UKey_MaxLogLevel,           /* Since 3.28.1.1 */\
              "Maximum log level",          \
              V2UPropType_Int32,            \
              V2UPropAccess_RO)             \
    property( V2UKey_UsbSpeed,              /* Since 3.28.3.18 */\
              "USB speed",                  \
              V2UPropType_Enum,             \
              V2UPropAccess_RO)             \
    property( V2UKey_BuildDate,             /* Since 3.28.3.18 */\
              "Driver build date",          \
              V2UPropType_String,           \
              V2UPropAccess_RO)             \
    property( V2UKey_BuildTime,             /* Since 3.28.3.18 */\
              "Driver build time",          \
              V2UPropType_String,           \
              V2UPropAccess_RO)             \
    property( V2UKey_BoardId,               /* Since 3.28.3.22 */\
              "Board identifier",           \
              V2UPropType_String,           \
              V2UPropAccess_RO)             \
    property( V2UKey_Temperature,           /* Since 3.30.0.1 */\
              "Temperature",                \
              V2UPropType_Uint32,           \
              V2UPropAccess_RO)             \
    property( V2UKey_InputName,             /* Since 3.30.0.11 */\
             "Input Name",                  \
              V2UPropType_String,           \
              V2UPropAccess_RO)

/* Define V2UPropertyKey enum */
typedef enum v2u_property_key {
#define V2U_PROPERTY_KEY_ENUM(key,name,type,access) key,
    V2U_PROPERTY_LIST(V2U_PROPERTY_KEY_ENUM)
#undef V2U_PROPERTY_KEY_ENUM
    V2UKey_Count        /* Number of known properties */
} V2UPropertyKey;

#define V2U_USERDATA_LEN    8

/* Convert V2UKey_Temperature to degrees Celsius */
#define V2U_TEMP_CELSIUS(val) ((((val)*50397)/256 - 27315)/100)

typedef union v2u_property_value {
    V2U_INT8        int8;
    V2U_UINT8       uint8;
    V2U_INT16       int16;
    V2U_UINT16      uint16;
    V2U_INT32       int32;
    V2U_UINT32      uint32;
    V2U_INT64       int64;
    V2U_UINT64      uint64;
    V2U_BOOL        boolean;
    V2UProductType  product_type;
    V2USize         size;
    V2URect         rect;
    V2UStrUcs2      wstr;
    V2UAdjRange     adj_range;
    V2UVersion      version;
    V2UVGAMode      vgamode;
    V2UVideoModeDescr vesa_mode;
    char            str[256];
    V2U_UCS2        str2[128];
    V2U_UINT8       edid[128];
    V2U_UINT8       blob[256];
    V2U_UINT8       userdata[V2U_USERDATA_LEN];
} V2UPropertyValue;

typedef struct v2u_ioctl_property {
    V2UPropertyKey key;             /* IN      property key */
    V2UPropertyValue value;         /* IN/OUT  property value */
} V2U_PACKED V2U_Property, V2UProperty;

/* 
 * V2U_GrabFrame
 *
 * Frame buffer.
 */
typedef struct ioctl_grabframe {
#define V2U_GrabFrame_Fields(pointer) \
    pointer     pixbuf;     /* IN  should be filled by user process */\
    V2U_UINT32  pixbuflen;  /* IN  should be filled by user process */\
    V2U_INT32   width;      /* OUT width in pixels */\
    V2U_INT32   height;     /* OUT height in pixels */\
    V2U_UINT32  bpp;        /* IN  pixel format */
    V2U_GrabFrame_Fields(void*)
} V2U_PACKED V2U_GrabFrame, V2UGrabFrame;

typedef struct ioctl_grabframe2 {
#define V2U_GrabFrame2_Fields(pointer) \
    pointer     pixbuf;     /* IN  should be filled by user process */\
    V2U_UINT32  pixbuflen;  /* IN  should be filled by user process */\
    V2U_UINT32  palette;    /* IN pixel format */\
    V2URect     crop;       /* IN/OUT cropping area; all zeros = full frame */\
    V2U_VideoMode mode;     /* OUT VGA mode */\
    V2U_UINT32  imagelen;   /* OUT size of the image stored in pixbuf */\
    V2U_INT32   retcode;    /* OUT return/error code */
    V2U_GrabFrame2_Fields(void*)
} V2U_PACKED V2U_GrabFrame2, V2UGrabFrame2;

/*
 * Error codes
 */
#define V2UERROR_OK             0   /* Success */
#define V2UERROR_FAULT          1   /* Unspecified error */
#define V2UERROR_INVALARG       2   /* Invalid argument */
#define V2UERROR_SMALLBUF       3   /* Insufficient buffer size */
#define V2UERROR_OUTOFMEMORY    4   /* Out of memory */
#define V2UERROR_NOSIGNAL       5   /* No signal detected */
#define V2UERROR_UNSUPPORTED    6   /* Unsupported video mode */
#define V2UERROR_TIMEOUT        7   /* grab timeout */

/*
 * The following flags can be OR'ed with V2U_GrabFrame::bpp or
 * V2U_GrabFrame2::palette field. The flags are preserved on return.
 *
 * V2U_GRABFRAME_BOTTOM_UP_FLAG
 * This flag requests the bitmap data to be sent in bottom-up format.
 * By default, the bitmap data are stored in the top-down order.
 *
 * V2U_GRABFRAME_KEYFRAME_FLAG
 * This flag only makes sense when the data are requested in compressed
 * format (V2U_GRABFRAME_FORMAT_CRGB24 and such). If this flag is set, the
 * driver updates and returns a full frame (i.e. keyframe).
 */
#define V2U_GRABFRAME_RESERVED          0x0f000000 /* These are ignored */
#define V2U_GRABFRAME_FLAGS_MASK        0xf0000000 /* Bits reserved for flags */
#define V2U_GRABFRAME_BOTTOM_UP_FLAG    0x80000000 /* Invert order of lines */
#define V2U_GRABFRAME_KEYFRAME_FLAG     0x40000000 /* Full frame is requested */
#define V2U_GRABFRAME_ADDR_IS_PHYS      0x20000000 /* Buffer addr is physical */
#define V2U_GRABFRAME_DEINTERLACE       0x10000000 /* De-interlace image, if it is interlaced */

/*
 * Image rotation mode
 * 
 * V2U_GRABFRAME_ROTATION_NONE
 * No rotation, image is grabbed in its original orientation.
 *
 * V2U_GRABFRAME_ROTATION_LEFT90
 * Grab the image rotated 90 degrees to the left with respect 
 * to its original orientation.
 *
 * V2U_GRABFRAME_ROTATION_RIGHT90
 * Grab the image rotated 90 degrees to the right with respect 
 * to its original orientation.
 *
 * V2U_GRABFRAME_ROTATION_180
 * Grab the image rotated 180 degrees with respect to its 
 * original orientation.
 */
#define V2U_GRABFRAME_ROTATION_MASK     0x00300000 /* Bits reserved for mode */
#define V2U_GRABFRAME_ROTATION_NONE     0x00000000 /* No rotation */
#define V2U_GRABFRAME_ROTATION_LEFT90   0x00100000 /* 90 degrees to the left */
#define V2U_GRABFRAME_ROTATION_RIGHT90  0x00200000 /* 90 degrees to the right */
#define V2U_GRABFRAME_ROTATION_180      0x00300000 /* Rotation 180 degrees */

/* If a valid algoruthm code is set, the image will be scaled image to
 * width/height in V2U_GrabFrame or crop.width/crop.height in V2U_GrabFrame2.
 * If no bits in V2U_GRABFRAME_SCALE_MASK are set, no scaling is performed.
 * If an unknown algorithm code is specified, a default one will be used.
 * Scaling works for the following capture formats:
 *
 *   V2U_GRABFRAME_FORMAT_Y8
 *   V2U_GRABFRAME_FORMAT_RGB4
 *   V2U_GRABFRAME_FORMAT_RGB8
 *   V2U_GRABFRAME_FORMAT_RGB16
 *   V2U_GRABFRAME_FORMAT_BGR16
 *   V2U_GRABFRAME_FORMAT_RGB24
 *   V2U_GRABFRAME_FORMAT_ARGB32
 *   V2U_GRABFRAME_FORMAT_BGR24
 *   V2U_GRABFRAME_FORMAT_YUY2
 *   V2U_GRABFRAME_FORMAT_2VUY
 *   V2U_GRABFRAME_FORMAT_YV12
 *   V2U_GRABFRAME_FORMAT_I420
 *   V2U_GRABFRAME_FORMAT_NV12
 *
 * Scaling is available since version 3.10.9
 *
 * Note that V2U_GRABFRAME_SCALE_AVERAGE scale mode is EXPERIMENTAL and
 * currently only works for RGB24 and BGR24 images.
 */
#define V2U_GRABFRAME_SCALE_MASK          0x000F0000 /* Scale algorithm mask */
#define V2U_GRABFRAME_SCALE_NEAREST       0x00010000 /* Nearest neighbour */
#define V2U_GRABFRAME_SCALE_AVERAGE       0x00020000 /* Weighted average */
#define V2U_GRABFRAME_SCALE_FAST_BILINEAR 0x00030000 /* Fast bilinear */
#define V2U_GRABFRAME_SCALE_BILINEAR      0x00040000 /* Bilinear */
#define V2U_GRABFRAME_SCALE_BICUBIC       0x00050000 /* Bicubic */
#define V2U_GRABFRAME_SCALE_EXPERIMENTAL  0x00060000 /* Experimental */
#define V2U_GRABFRAME_SCALE_POINT         0x00070000 /* Nearest neighbour */
#define V2U_GRABFRAME_SCALE_AREA          0x00080000 /* Weighted average */
#define V2U_GRABFRAME_SCALE_BICUBLIN      0x00090000 /* Lum bicub,Chr bilinear*/
#define V2U_GRABFRAME_SCALE_SINC          0x000A0000 /* Sinc */
#define V2U_GRABFRAME_SCALE_LANCZOS       0x000B0000 /* Lanczos */
#define V2U_GRABFRAME_SCALE_SPLINE        0x000C0000 /* Natural bicubic spline*/
#define V2U_GRABFRAME_SCALE_HW            0x000D0000 /* Hardware provided */
#define V2U_GRABFRAME_SCALE_MAX_MODE      0x000D0000 /* Maximum valid mode */

/* Bits that define image format */
#define V2U_GRABFRAME_FORMAT_MASK       0x0000ffff /* Image format mask */
#define V2U_GRABFRAME_FORMAT_RGB_MASK   0x0000001f /* Mask for RGB formats */
#define V2U_GRABFRAME_FORMAT_RGB4       0x00000004
#define V2U_GRABFRAME_FORMAT_RGB8       0x00000008 /* R2:G3:B3 */
#define V2U_GRABFRAME_FORMAT_RGB16      0x00000010
#define V2U_GRABFRAME_FORMAT_RGB24      0x00000018
#define V2U_GRABFRAME_FORMAT_YUY2       0x00000100 /* Same as YUV422 */
#define V2U_GRABFRAME_FORMAT_YV12       0x00000200
#define V2U_GRABFRAME_FORMAT_2VUY       0x00000300
#define V2U_GRABFRAME_FORMAT_BGR16      0x00000400
#define V2U_GRABFRAME_FORMAT_Y8         0x00000500
#define V2U_GRABFRAME_FORMAT_CRGB24     0x00000600 /* Compressed RGB24 */
#define V2U_GRABFRAME_FORMAT_CYUY2      0x00000700 /* Compressed YUY2 */
#define V2U_GRABFRAME_FORMAT_BGR24      0x00000800
#define V2U_GRABFRAME_FORMAT_CBGR24     0x00000900 /* Compressed BGR24 */
#define V2U_GRABFRAME_FORMAT_I420       0x00000A00 /* Same as YUV420P */
#define V2U_GRABFRAME_FORMAT_ARGB32     0x00000B00
#define V2U_GRABFRAME_FORMAT_NV12       0x00000C00
#define V2U_GRABFRAME_FORMAT_C2VUY      0x00000D00 /* Compressed 2VUY */

/* Old flag names, defined here for backward compatibility */ 
#define V2U_GRABFRAME_PALETTE_MASK      V2U_GRABFRAME_FORMAT_MASK
#define V2U_GRABFRAME_PALETTE_RGB_MASK  V2U_GRABFRAME_FORMAT_RGB_MASK
#define V2U_GRABFRAME_PALETTE_RGB4      V2U_GRABFRAME_FORMAT_RGB4
#define V2U_GRABFRAME_PALETTE_RGB8      V2U_GRABFRAME_FORMAT_RGB8
#define V2U_GRABFRAME_PALETTE_RGB16     V2U_GRABFRAME_FORMAT_RGB16
#define V2U_GRABFRAME_PALETTE_RGB24     V2U_GRABFRAME_FORMAT_RGB24
#define V2U_GRABFRAME_PALETTE_ARGB32    V2U_GRABFRAME_FORMAT_ARGB32
#define V2U_GRABFRAME_PALETTE_YUY2      V2U_GRABFRAME_FORMAT_YUY2
#define V2U_GRABFRAME_PALETTE_YV12      V2U_GRABFRAME_FORMAT_YV12
#define V2U_GRABFRAME_PALETTE_I420      V2U_GRABFRAME_FORMAT_I420
#define V2U_GRABFRAME_PALETTE_2VUY      V2U_GRABFRAME_FORMAT_2VUY
#define V2U_GRABFRAME_PALETTE_BGR16     V2U_GRABFRAME_FORMAT_BGR16
#define V2U_GRABFRAME_PALETTE_Y8        V2U_GRABFRAME_FORMAT_Y8
#define V2U_GRABFRAME_PALETTE_BGR24     V2U_GRABFRAME_FORMAT_BGR24
#define V2U_GRABFRAME_PALETTE_NV12      V2U_GRABFRAME_FORMAT_NV12

#define V2U_GRABFRAME_FORMAT(p)  ((p) & V2U_GRABFRAME_FORMAT_MASK)

/* Macro to determine bpp (bits per pixel) for particular grab format */
#define V2UPALETTE_2_BPP(p) \
 (((p) & V2U_GRABFRAME_FORMAT_RGB_MASK) ? \
  ((V2U_UINT8)((p) & V2U_GRABFRAME_FORMAT_RGB_MASK)) : \
  (((V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_BGR16) || \
    (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_YUY2)  || \
    (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_2VUY)  || \
    (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_CYUY2) || \
    (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_C2VUY)) ? 16 :\
   (((V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_YV12) || \
     (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_NV12) || \
     (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_I420)) ? 12 : \
    (((V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_Y8) || \
      (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_RGB8)) ? 8 : \
     (((V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_CRGB24) || \
       (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_CBGR24) || \
       (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_BGR24)) ? 24 : \
      ((V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_ARGB32) ? 32 : 0))))))

/* Macro to determine whether frame data is compressed */
#define V2UPALETTE_COMPRESSED(p) \
    ((V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_CRGB24) || \
     (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_CBGR24) || \
     (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_CYUY2)  || \
     (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_C2VUY))

/* Macro to determine whether frame data is planar with separate color planes */
#define V2UPALETTE_PLANAR_SEPARATE(p) \
    ((V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_YV12) || \
     (V2U_GRABFRAME_FORMAT(p) == V2U_GRABFRAME_FORMAT_I420))

/* Restore previous packing */
#include "v2u_pack_off.h"

#endif /* _VGA2USB_DEFS_H_ */
