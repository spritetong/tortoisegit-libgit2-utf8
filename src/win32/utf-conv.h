/*
 * Copyright (C) 2009-2012 the libgit2 contributors
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */

#include <wchar.h>

#ifndef INCLUDE_git_utfconv_h__
#define INCLUDE_git_utfconv_h__

#define GIT_WIN_PATH (260 + 1)

void git__utf8_to_16(wchar_t *dest, size_t length, const char *src);
void git__utf16_to_8(char *dest, const wchar_t *src);

#endif

