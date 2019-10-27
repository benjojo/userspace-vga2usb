/****************************************************************************
 *
 * $Id: v2u_pack_off.h 23083 2013-07-16 19:44:46Z monich $
 *
 * Copyright (C) 2003-2013 Epiphan Systems Inc. All rights reserved.
 *
 * Structure packing tweaks
 *
 ****************************************************************************/

#ifdef V2U_PACK_ON
#  undef V2U_PACK_ON
#  ifdef _WIN32
/* Restore previous packing */
#    include <poppack.h>
#  endif
#else
#  error "packing already off"
#endif /* V2U_PACK_ON */
