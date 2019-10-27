/****************************************************************************
 *
 * $Id: v2u_id.h 28830 2014-11-20 21:02:03Z pzeldin $
 *
 * Copyright (C) 2003-2014 Epiphan Systems Inc. All rights reserved.
 *
 * Defines vendor and product ids of VGA2USB hardware. Included by the
 * driver and by the user level code.
 *
 ****************************************************************************/

#ifndef _VGA2USB_ID_H_
#define _VGA2USB_ID_H_ 1

/**
 * Vendor and product ids.
 *
 * NOTE: if you are adding a new product ID, don't forget to update
 * V2U_PRODUCT_MAP macro below.
 */

#define EPIPHAN_VENDORID                0x5555
#define VGA2USB_VENDORID                EPIPHAN_VENDORID
#define VGA2USB_PRODID_VGA2USB          0x1110
#define VGA2USB_PRODID_KVM2USB          0x1120
#define VGA2USB_PRODID_DVI2USB          0x2222
#define VGA2USB_PRODID_VGA2USB_LR       0x3340
#define VGA2USB_PRODID_VGA2USB_HR       0x3332
#define VGA2USB_PRODID_VGA2USB_PRO      0x3333
#define VGA2USB_PRODID_VGA2USB_LR_RESPIN  0x3382
#define VGA2USB_PRODID_KVM2USB_LR_RESPIN  0x3383
#define VGA2USB_PRODID_VGA2USB_HR_RESPIN  0x3392
#define VGA2USB_PRODID_VGA2USB_PRO_RESPIN 0x33A2
#define VGA2USB_PRODID_DVI2USB_RESPIN   0x3380
#define VGA2USB_PRODID_KVM2USB_LR       0x3344
#define VGA2USB_PRODID_KVM2USB_PRO      0x3337
#define VGA2USB_PRODID_DVI2USB_SOLO     0x3411
#define VGA2USB_PRODID_DVI2USB_DUO      0x3422
#define VGA2USB_PRODID_DVI2USB3         0x3500
#define VGA2USB_PRODID_DVI2USB3_ET      0x3510
#define VGA2USB_PRODID_SDI2USB3         0x3520
#define VGA2USB_PRODID_DVI2USB3_R3      0x3501
#define VGA2USB_PRODID_DVI2USB3_ET_R3   0x3511
#define VGA2USB_PRODID_SDI2USB3_R3      0x3521
#define VGA2USB_PRODID_VGA2FIFO         0x4000
#define VGA2USB_PRODID_KVM2FIFO         0x4004
#define VGA2USB_PRODID_DVI2FIFO         0x4080
#define VGA2USB_PRODID_DAVINCI1         0x5000
#define VGA2USB_PRODID_VGA2PCI          0x3A00
#define VGA2USB_PRODID_DVI2PCI          0x3B00
#define VGA2USB_PRODID_DVI2PCI_STD      0x3B10
#define VGA2USB_PRODID_HDMI2PCI         0x3C00
#define VGA2USB_PRODID_DVI2PCI_GEN2     0x3D00
#define VGA2USB_PRODID_DVI2PCI_GII      0x3E00
#define VGA2USB_PRODID_GIOCONDA         0x5100
#define VGA2USB_PRODID_ORNITHOPTER      0x5200

/**
 * Macros for detecting VGA2USB hardware
 */

#define VGA2USB_IS_VGA2USB(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_VGA2USB && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_KVM2USB(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_KVM2USB && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_DVI2USB(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_DVI2USB && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_VGA2USB_PRO(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_VGA2USB_PRO && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_VGA2USB_HR(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_VGA2USB_HR && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_VGA2USB_LR(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_VGA2USB_LR && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_VGA2USB_LR_RESPIN(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_VGA2USB_LR_RESPIN && \
    ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_VGA2USB_HR_RESPIN(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_VGA2USB_HR_RESPIN && \
    ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_VGA2USB_PRO_RESPIN(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_VGA2USB_PRO_RESPIN && \
    ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_KVM2USB_LR(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_KVM2USB_LR && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_KVM2USB_PRO(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_KVM2USB_PRO && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_DVI2USB_SOLO(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_DVI2USB_SOLO && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_DVI2USB_DUO(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_DVI2USB_DUO && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_DVI2USB_RESPIN(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID &&(idProduct)==VGA2USB_PRODID_DVI2USB_RESPIN &&\
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_KVM2USB_LR_RESPIN(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && (idProduct)==VGA2USB_PRODID_KVM2USB_LR_RESPIN && \
   ((iProduct)>0 || (iMfg)>0))

#define VGA2USB_IS_DVI2USB3(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && \
	 ((idProduct)==VGA2USB_PRODID_DVI2USB3 || (idProduct)==VGA2USB_PRODID_DVI2USB3_R3) && \
   (iProduct)==1 && (iMfg)==3)

#define VGA2USB_IS_DVI2USB3_ET(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && \
	 ((idProduct)==VGA2USB_PRODID_DVI2USB3_ET || (idProduct)==VGA2USB_PRODID_DVI2USB3_ET_R3) && \
   (iProduct)==1 && (iMfg)==3)

#define VGA2USB_IS_SDI2USB3(idVendor,idProduct,iProduct,iMfg) \
  ((idVendor)==VGA2USB_VENDORID && \
	 ((idProduct)==VGA2USB_PRODID_SDI2USB3 || (idProduct)==VGA2USB_PRODID_SDI2USB3_R3) && \
   (iProduct)==1 && (iMfg)==3)

#define VGA2USB_IS_ANY(idVendor,idProduct,iProduct,iMfg) \
  (VGA2USB_IS_VGA2USB(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_KVM2USB(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_DVI2USB(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_DVI2USB_SOLO(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_DVI2USB_DUO(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_DVI2USB_RESPIN(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_VGA2USB_PRO(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_VGA2USB_HR(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_VGA2USB_LR(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_VGA2USB_LR_RESPIN(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_VGA2USB_HR_RESPIN(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_VGA2USB_PRO_RESPIN(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_KVM2USB_PRO(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_KVM2USB_LR(idVendor,idProduct,iProduct,iMfg)  || \
   VGA2USB_IS_KVM2USB_LR_RESPIN(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_DVI2USB3(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_DVI2USB3_ET(idVendor,idProduct,iProduct,iMfg) || \
   VGA2USB_IS_SDI2USB3(idVendor,idProduct,iProduct,iMfg) \
  )


/**
 * Windows device name format. Used by user level code on Windows to open
 * a handle to the VGA2USB driver.
 */

#define VGA2USB_WIN_DEVICE_FORMAT   "EpiphanVga2usb%lu"

/**
 * This macro helps to map VGA2USB product ids into product names
 */
#define V2U_PRODUCT_MAP(map) \
  map( VGA2USB_PRODID_VGA2USB,       V2UProductVGA2USB,      "VGA2USB"     )\
  map( VGA2USB_PRODID_KVM2USB,       V2UProductKVM2USB,      "KVM2USB"     )\
  map( VGA2USB_PRODID_DVI2USB,       V2UProductDVI2USB,      "DVI2USB"     )\
  map( VGA2USB_PRODID_VGA2USB_LR,    V2UProductVGA2USBLR,    "VGA2USB LR"  )\
  map( VGA2USB_PRODID_VGA2USB_HR,    V2UProductVGA2USBHR,    "VGA2USB HR"  )\
  map( VGA2USB_PRODID_VGA2USB_PRO,   V2UProductVGA2USBPro,   "VGA2USB Pro" )\
  map( VGA2USB_PRODID_VGA2USB_LR_RESPIN,V2UProductVGA2USBLRRespin,"VGA2USB LR")\
  map( VGA2USB_PRODID_VGA2USB_HR_RESPIN,V2UProductVGA2USBHRRespin,"VGA2USB HR")\
  map( VGA2USB_PRODID_VGA2USB_PRO_RESPIN,V2UProductVGA2USBProRespin,"VGA2USB Pro")\
  map( VGA2USB_PRODID_DVI2USB_RESPIN,V2UProductDVI2USBRespin,"DVI2USB"     )\
  map( VGA2USB_PRODID_KVM2USB_LR,    V2UProductKVM2USBLR,    "KVM2USB LR"  )\
  map( VGA2USB_PRODID_KVM2USB_LR_RESPIN, V2UProductKVM2USBLRRespin, "KVM2USB LR")\
  map( VGA2USB_PRODID_KVM2USB_PRO,   V2UProductKVM2USBPro,   "KVM2USB Pro" )\
  map( VGA2USB_PRODID_DVI2USB_SOLO,  V2UProductDVI2USBSolo,  "DVI2USB Solo")\
  map( VGA2USB_PRODID_DVI2USB_DUO,   V2UProductDVI2USBDuo,   "DVI2USB Duo" )\
  map( VGA2USB_PRODID_VGA2FIFO,      V2UProductVGA2FIFO,     "VGA2FIFO"    )\
  map( VGA2USB_PRODID_KVM2FIFO,      V2UProductKVM2FIFO,     "KVMFIFO"     )\
  map( VGA2USB_PRODID_DVI2FIFO,      V2UProductDVI2FIFO,     "DVI2FIFO"    )\
  map( VGA2USB_PRODID_DAVINCI1,      V2UProductDVI2Davinci1, "DVI2Davinci" )\
  map( VGA2USB_PRODID_VGA2PCI,       V2UProductVGA2PCI,      "VGA2PCI"     )\
  map( VGA2USB_PRODID_GIOCONDA,      V2UProductGioconda,     "Gioconda"    )\
  map( VGA2USB_PRODID_DVI2PCI,       V2UProductDVI2PCI,      "DVI2PCI"     )\
  map( VGA2USB_PRODID_DVI2PCI_STD,   V2UProductDVI2PCIStd,   "DVI2PCI Std" )\
  map( VGA2USB_PRODID_DVI2PCI_GEN2,  V2UProductDVI2PCIGen2,  "DVI2PCI Duo" )\
  map( VGA2USB_PRODID_DVI2PCI_GII,   V2UProductDVI2PCIGII,   "DVI2PCI GII" )\
  map( VGA2USB_PRODID_HDMI2PCI,      V2UProductHDMI2PCI,     "HDMI2PCI"    )\
  V2U_FX3_PRODUCT_MAP(map)
#define V2U_FX3_PRODUCT_MAP(map) \
  map( VGA2USB_PRODID_DVI2USB3,      V2UProductDVI2USB3,     "DVI2USB 3.0" )\
  map( VGA2USB_PRODID_DVI2USB3_ET,   V2UProductDVI2USB3ET,   "DVI2USB 3.0 ET")\
  map( VGA2USB_PRODID_SDI2USB3,      V2UProductSDI2USB3,     "SDI2USB 3.0" )\
  map( VGA2USB_PRODID_DVI2USB3_R3,   V2UProductDVI2USB3,     "DVI2USB 3.0" )\
  map( VGA2USB_PRODID_DVI2USB3_ET_R3,V2UProductDVI2USB3ET,   "DVI2USB 3.0 ET")\
  map( VGA2USB_PRODID_SDI2USB3_R3,   V2UProductSDI2USB3,     "SDI2USB 3.0" )

#endif /* _VGA2USB_ID_H_ */
