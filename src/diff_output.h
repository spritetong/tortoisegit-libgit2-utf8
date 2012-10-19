/*
 * Copyright (C) 2009-2012 the libgit2 contributors
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_diff_output_h__
#define INCLUDE_diff_output_h__

#include "git2/blob.h"
#include "diff.h"
#include "map.h"
#include "xdiff/xdiff.h"

#define MAX_DIFF_FILESIZE 0x20000000

enum {
	GIT_DIFF_PATCH_ALLOCATED  = (1 << 0),
	GIT_DIFF_PATCH_PREPPED    = (1 << 1),
	GIT_DIFF_PATCH_LOADED     = (1 << 2),
	GIT_DIFF_PATCH_DIFFABLE   = (1 << 3),
	GIT_DIFF_PATCH_DIFFED     = (1 << 4),
};

/* context for performing diffs */
typedef struct {
	git_repository   *repo;
	git_diff_list    *diff;
	const git_diff_options *opts;
	git_diff_file_fn  file_cb;
	git_diff_hunk_fn  hunk_cb;
	git_diff_data_fn  data_cb;
	void *cb_data;
	int   cb_error;
	git_diff_range cb_range;
	xdemitconf_t xdiff_config;
	xpparam_t    xdiff_params;
} diff_context;

/* cached information about a single span in a diff */
typedef struct diff_patch_line diff_patch_line;
struct diff_patch_line {
	const char *ptr;
	size_t len;
	int lines, oldno, newno;
	char origin;
};

/* cached information about a hunk in a diff */
typedef struct diff_patch_hunk diff_patch_hunk;
struct diff_patch_hunk {
	git_diff_range range;
	char   header[128];
	size_t header_len;
	size_t line_start;
	size_t line_count;
};

struct git_diff_patch {
	git_refcount rc;
	git_diff_list *diff; /* for refcount purposes, maybe NULL for blob diffs */
	git_diff_delta *delta;
	diff_context *ctxt; /* only valid while generating patch */
	git_iterator_type_t old_src;
	git_iterator_type_t new_src;
	git_blob *old_blob;
	git_blob *new_blob;
	git_map  old_data;
	git_map  new_data;
	uint32_t flags;
	diff_patch_hunk *hunks;
	size_t hunks_asize, hunks_size;
	diff_patch_line *lines;
	size_t lines_asize, lines_size;
};

/* context for performing diff on a single delta */
typedef struct {
	git_diff_patch *patch;
	uint32_t prepped  : 1;
	uint32_t loaded   : 1;
	uint32_t diffable : 1;
	uint32_t diffed   : 1;
} diff_delta_context;

#endif
