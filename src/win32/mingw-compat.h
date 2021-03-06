/*
 * Copyright (C) 2009-2012 the libgit2 contributors
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_mingw_compat__
#define INCLUDE_mingw_compat__

/* For UTF-8. Added by Sprite Tong, 12/30/2011. */
#include "../../../tgit/compat/win32_xutf8.h"

#if defined(__MINGW32__)

/* use a 64-bit file offset type */
# define lseek _lseeki64
# define stat _stati64
# define fstat _fstati64

/* stat: file mode type testing macros */
# define _S_IFLNK 0120000
# define S_IFLNK _S_IFLNK
# define S_ISLNK(m) (((m) & _S_IFMT) == _S_IFLNK)

#endif

#endif /* INCLUDE_mingw_compat__ */
