/*
 * Copyright (C) 2012 the libgit2 contributors
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_git_clone_h__
#define INCLUDE_git_clone_h__

#include "common.h"
#include "types.h"
#include "indexer.h"
#include "checkout.h"


/**
 * @file git2/clone.h
 * @brief Git cloning routines
 * @defgroup git_clone Git cloning routines
 * @ingroup Git
 * @{
 */
GIT_BEGIN_DECL

/**
 * Clone a remote repository, and checkout the branch pointed to by the remote
 * HEAD.
 *
 * @param out pointer that will receive the resulting repository object
 * @param origin_url repository to clone from
 * @param workdir_path local directory to clone to
 * @param fetch_stats pointer to structure that receives fetch progress information (may be NULL)
 * @param checkout_opts options for the checkout step (may be NULL)
 * @return 0 on success, GIT_ERROR otherwise (use giterr_last for information about the error)
 */
GIT_EXTERN(int) git_clone(git_repository **out,
								  const char *origin_url,
								  const char *workdir_path,
								  git_indexer_stats *fetch_stats,
								  git_indexer_stats *checkout_stats,
								  git_checkout_opts *checkout_opts);

/**
 * Create a bare clone of a remote repository.
 *
 * @param out pointer that will receive the resulting repository object
 * @param origin_url repository to clone from
 * @param dest_path local directory to clone to
 * @param fetch_stats pointer to structure that receives fetch progress information (may be NULL)
 * @return 0 on success, GIT_ERROR otherwise (use giterr_last for information about the error)
 */
GIT_EXTERN(int) git_clone_bare(git_repository **out,
										 const char *origin_url,
										 const char *dest_path,
										 git_indexer_stats *fetch_stats);

/** @} */
GIT_END_DECL
#endif
