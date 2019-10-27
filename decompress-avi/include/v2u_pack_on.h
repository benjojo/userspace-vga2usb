/****************************************************************************
 *
 * $Id: v2u_pack_on.h 23083 2013-07-16 19:44:46Z monich $
 *
 * Copyright (C) 2003-2013 Epiphan Systems Inc. All rights reserved.
 *
 * Structure packing tweaks
 *
 ****************************************************************************/

#ifndef V2U_PACKED
#  ifdef __GNUC__
#    define V2U_PACKED __attribute__((packed))
#  else
#    define V2U_PACKED
#  endif
#endif

#ifndef V2U_PACK_ON
#  define V2U_PACK_ON
#  ifdef _WIN32
#    include <pshpack1.h>
#  endif
#else
#  error "packing already on"
#endif /* V2U_PACK_ON */
