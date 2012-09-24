/*
 * Copyright (C) 2009-2012 the libgit2 contributors
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_posix__w32_h__
#define INCLUDE_posix__w32_h__

#include "common.h"
#include "compat/fnmatch.h"
#include "utf-conv.h"

/* For UTF-8. Fix C++ compilation bugs. Changed by Sprite Tong, 5/22/2012. */
GIT_INLINE(int) p_link(const char *old, const char *newname)
{
	GIT_UNUSED(old);
	GIT_UNUSED(newname);
	errno = ENOSYS;
	return -1;
}

GIT_INLINE(int) p_mkdir(const char *path, mode_t mode)
{
	wchar_t buf[GIT_WIN_PATH];
	GIT_UNUSED(mode);
	git__utf8_to_16(buf, GIT_WIN_PATH, path);
	return _wmkdir(buf);
}

extern int p_unlink(const char *path);
extern int p_lstat(const char *file_name, struct stat *buf);
extern int p_readlink(const char *link, char *target, size_t target_len);
extern int p_symlink(const char *old, const char *_new);
extern int p_hide_directory__w32(const char *path);
extern char *p_realpath(const char *orig_path, char *buffer);
extern int p_vsnprintf(char *buffer, size_t count, const char *format, va_list argptr);
extern int p_snprintf(char *buffer, size_t count, const char *format, ...) GIT_FORMAT_PRINTF(3, 4);
extern int p_mkstemp(char *tmp_path);
extern int p_setenv(const char* name, const char* value, int overwrite);
extern int p_stat(const char* path, struct stat* buf);
extern int p_chdir(const char* path);
extern int p_chmod(const char* path, mode_t mode);
extern int p_rmdir(const char* path);
extern int p_access(const char* path, mode_t mode);
extern int p_fsync(int fd);
extern int p_open(const char *path, int flags, ...);
extern int p_creat(const char *path, mode_t mode);
extern int p_getcwd(char *buffer_out, size_t size);
extern int p_rename(const char *from, const char *to);
extern int p_recv(GIT_SOCKET socket, void *buffer, size_t length, int flags);
extern int p_send(GIT_SOCKET socket, const void *buffer, size_t length, int flags);

#endif
