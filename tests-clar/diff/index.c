#include "clar_libgit2.h"
#include "diff_helpers.h"

static git_repository *g_repo = NULL;

void test_diff_index__initialize(void)
{
	g_repo = cl_git_sandbox_init("status");
}

void test_diff_index__cleanup(void)
{
	cl_git_sandbox_cleanup();
}

void test_diff_index__0(void)
{
	/* grabbed a couple of commit oids from the history of the attr repo */
	const char *a_commit = "26a125ee1bf"; /* the current HEAD */
	const char *b_commit = "0017bd4ab1ec3"; /* the start */
	git_tree *a = resolve_commit_oid_to_tree(g_repo, a_commit);
	git_tree *b = resolve_commit_oid_to_tree(g_repo, b_commit);
	git_diff_options opts = {0};
	git_diff_list *diff = NULL;
	diff_expects exp;

	cl_assert(a);
	cl_assert(b);

	opts.context_lines = 1;
	opts.interhunk_lines = 1;

	memset(&exp, 0, sizeof(exp));

	cl_git_pass(git_diff_index_to_tree(g_repo, &opts, a, &diff));

	cl_git_pass(git_diff_foreach(
		diff, &exp, diff_file_fn, diff_hunk_fn, diff_line_fn));

	/* to generate these values:
	 * - cd to tests/resources/status,
	 * - mv .gitted .git
	 * - git diff --name-status --cached 26a125ee1bf
	 * - git diff -U1 --cached 26a125ee1bf
	 * - mv .git .gitted
	 */
	cl_assert_equal_i(8, exp.files);
	cl_assert_equal_i(3, exp.file_adds);
	cl_assert_equal_i(2, exp.file_dels);
	cl_assert_equal_i(3, exp.file_mods);

	cl_assert_equal_i(8, exp.hunks);

	cl_assert_equal_i(11, exp.lines);
	cl_assert_equal_i(3, exp.line_ctxt);
	cl_assert_equal_i(6, exp.line_adds);
	cl_assert_equal_i(2, exp.line_dels);

	git_diff_list_free(diff);
	diff = NULL;
	memset(&exp, 0, sizeof(exp));

	cl_git_pass(git_diff_index_to_tree(g_repo, &opts, b, &diff));

	cl_git_pass(git_diff_foreach(
		diff, &exp, diff_file_fn, diff_hunk_fn, diff_line_fn));

	/* to generate these values:
	 * - cd to tests/resources/status,
	 * - mv .gitted .git
	 * - git diff --name-status --cached 0017bd4ab1ec3
	 * - git diff -U1 --cached 0017bd4ab1ec3
	 * - mv .git .gitted
	 */
	cl_assert_equal_i(12, exp.files);
	cl_assert_equal_i(7, exp.file_adds);
	cl_assert_equal_i(2, exp.file_dels);
	cl_assert_equal_i(3, exp.file_mods);

	cl_assert_equal_i(12, exp.hunks);

	cl_assert_equal_i(16, exp.lines);
	cl_assert_equal_i(3, exp.line_ctxt);
	cl_assert_equal_i(11, exp.line_adds);
	cl_assert_equal_i(2, exp.line_dels);

	git_diff_list_free(diff);
	diff = NULL;

	git_tree_free(a);
	git_tree_free(b);
}

static int diff_stop_after_2_files(
	void *cb_data,
	const git_diff_delta *delta,
	float progress)
{
	diff_expects *e = cb_data;

	GIT_UNUSED(progress);
	GIT_UNUSED(delta);

	e->files++;

	return (e->files == 2);
}

void test_diff_index__1(void)
{
	/* grabbed a couple of commit oids from the history of the attr repo */
	const char *a_commit = "26a125ee1bf"; /* the current HEAD */
	const char *b_commit = "0017bd4ab1ec3"; /* the start */
	git_tree *a = resolve_commit_oid_to_tree(g_repo, a_commit);
	git_tree *b = resolve_commit_oid_to_tree(g_repo, b_commit);
	git_diff_options opts = {0};
	git_diff_list *diff = NULL;
	diff_expects exp;

	cl_assert(a);
	cl_assert(b);

	opts.context_lines = 1;
	opts.interhunk_lines = 1;

	memset(&exp, 0, sizeof(exp));

	cl_git_pass(git_diff_index_to_tree(g_repo, &opts, a, &diff));

	cl_assert_equal_i(
		GIT_EUSER,
		git_diff_foreach(diff, &exp, diff_stop_after_2_files, NULL, NULL)
	);

	cl_assert_equal_i(2, exp.files);

	git_diff_list_free(diff);
	diff = NULL;

	git_tree_free(a);
	git_tree_free(b);
}
