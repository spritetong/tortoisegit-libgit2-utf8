#include "clar_libgit2.h"
#include "diff_helpers.h"

static git_repository *g_repo = NULL;
static diff_expects expected;
static git_diff_options opts;
static git_blob *d, *alien;

void test_diff_blob__initialize(void)
{
	git_oid oid;

	g_repo = cl_git_sandbox_init("attr");

	memset(&opts, 0, sizeof(opts));
	opts.context_lines = 1;
	opts.interhunk_lines = 0;

	memset(&expected, 0, sizeof(expected));

	/* tests/resources/attr/root_test4.txt */
	cl_git_pass(git_oid_fromstrn(&oid, "a0f7217a", 8));
	cl_git_pass(git_blob_lookup_prefix(&d, g_repo, &oid, 4));

	/* alien.png */
	cl_git_pass(git_oid_fromstrn(&oid, "edf3dcee", 8));
	cl_git_pass(git_blob_lookup_prefix(&alien, g_repo, &oid, 4));
}

void test_diff_blob__cleanup(void)
{
	git_blob_free(d);
	git_blob_free(alien);

	cl_git_sandbox_cleanup();
}

void test_diff_blob__can_compare_text_blobs(void)
{
	git_blob *a, *b, *c;
	git_oid a_oid, b_oid, c_oid;

	/* tests/resources/attr/root_test1 */
	cl_git_pass(git_oid_fromstrn(&a_oid, "45141a79", 8));
	cl_git_pass(git_blob_lookup_prefix(&a, g_repo, &a_oid, 4));

	/* tests/resources/attr/root_test2 */
	cl_git_pass(git_oid_fromstrn(&b_oid, "4d713dc4", 8));
	cl_git_pass(git_blob_lookup_prefix(&b, g_repo, &b_oid, 4));

	/* tests/resources/attr/root_test3 */
	cl_git_pass(git_oid_fromstrn(&c_oid, "c96bbb2c2557a832", 16));
	cl_git_pass(git_blob_lookup_prefix(&c, g_repo, &c_oid, 8));

	/* Doing the equivalent of a `git diff -U1` on these files */

	/* diff on tests/resources/attr/root_test1 */
	cl_git_pass(git_diff_blobs(
		a, b, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert_equal_i(1, expected.files);
	cl_assert_equal_i(1, expected.file_mods);
	cl_assert(expected.at_least_one_of_them_is_binary == false);

	cl_assert_equal_i(1, expected.hunks);
	cl_assert_equal_i(6, expected.lines);
	cl_assert_equal_i(1, expected.line_ctxt);
	cl_assert_equal_i(5, expected.line_adds);
	cl_assert_equal_i(0, expected.line_dels);

	/* diff on tests/resources/attr/root_test2 */
	memset(&expected, 0, sizeof(expected));
	cl_git_pass(git_diff_blobs(
		b, c, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert_equal_i(1, expected.files);
	cl_assert_equal_i(1, expected.file_mods);
	cl_assert(expected.at_least_one_of_them_is_binary == false);

	cl_assert_equal_i(1, expected.hunks);
	cl_assert_equal_i(15, expected.lines);
	cl_assert_equal_i(3, expected.line_ctxt);
	cl_assert_equal_i(9, expected.line_adds);
	cl_assert_equal_i(3, expected.line_dels);

	/* diff on tests/resources/attr/root_test3 */
	memset(&expected, 0, sizeof(expected));
	cl_git_pass(git_diff_blobs(
		a, c, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert_equal_i(1, expected.files);
	cl_assert_equal_i(1, expected.file_mods);
	cl_assert(expected.at_least_one_of_them_is_binary == false);

	cl_assert_equal_i(1, expected.hunks);
	cl_assert_equal_i(13, expected.lines);
	cl_assert_equal_i(0, expected.line_ctxt);
	cl_assert_equal_i(12, expected.line_adds);
	cl_assert_equal_i(1, expected.line_dels);

	memset(&expected, 0, sizeof(expected));
	cl_git_pass(git_diff_blobs(
		c, d, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert_equal_i(1, expected.files);
	cl_assert_equal_i(1, expected.file_mods);
	cl_assert(expected.at_least_one_of_them_is_binary == false);

	cl_assert_equal_i(2, expected.hunks);
	cl_assert_equal_i(14, expected.lines);
	cl_assert_equal_i(4, expected.line_ctxt);
	cl_assert_equal_i(6, expected.line_adds);
	cl_assert_equal_i(4, expected.line_dels);

	git_blob_free(a);
	git_blob_free(b);
	git_blob_free(c);
}

void test_diff_blob__can_compare_against_null_blobs(void)
{
	git_blob *e = NULL;

	cl_git_pass(git_diff_blobs(
		d, e, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert_equal_i(1, expected.files);
	cl_assert_equal_i(1, expected.file_dels);
	cl_assert(expected.at_least_one_of_them_is_binary == false);

	cl_assert_equal_i(1, expected.hunks);
	cl_assert_equal_i(14, expected.hunk_old_lines);
	cl_assert_equal_i(14, expected.lines);
	cl_assert_equal_i(14, expected.line_dels);

	opts.flags |= GIT_DIFF_REVERSE;
	memset(&expected, 0, sizeof(expected));

	cl_git_pass(git_diff_blobs(
		d, e, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert_equal_i(1, expected.files);
	cl_assert_equal_i(1, expected.file_adds);
	cl_assert(expected.at_least_one_of_them_is_binary == false);

	cl_assert_equal_i(1, expected.hunks);
	cl_assert_equal_i(14, expected.hunk_new_lines);
	cl_assert_equal_i(14, expected.lines);
	cl_assert_equal_i(14, expected.line_adds);

	opts.flags ^= GIT_DIFF_REVERSE;
	memset(&expected, 0, sizeof(expected));

	cl_git_pass(git_diff_blobs(
		alien, NULL, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert(expected.at_least_one_of_them_is_binary == true);

	cl_assert_equal_i(1, expected.files);
	cl_assert_equal_i(1, expected.file_dels);
	cl_assert_equal_i(0, expected.hunks);
	cl_assert_equal_i(0, expected.lines);

	memset(&expected, 0, sizeof(expected));

	cl_git_pass(git_diff_blobs(
		NULL, alien, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert(expected.at_least_one_of_them_is_binary == true);

	cl_assert_equal_i(1, expected.files);
	cl_assert_equal_i(1, expected.file_adds);
	cl_assert_equal_i(0, expected.hunks);
	cl_assert_equal_i(0, expected.lines);
}

static void assert_identical_blobs_comparison(diff_expects expected)
{
	cl_assert_equal_i(1, expected.files);
	cl_assert_equal_i(1, expected.file_unmodified);
	cl_assert_equal_i(0, expected.hunks);
	cl_assert_equal_i(0, expected.lines);
}

void test_diff_blob__can_compare_identical_blobs(void)
{
	cl_git_pass(git_diff_blobs(
		d, d, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert(expected.at_least_one_of_them_is_binary == false);
	assert_identical_blobs_comparison(expected);

	memset(&expected, 0, sizeof(expected));
	cl_git_pass(git_diff_blobs(
		NULL, NULL, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert(expected.at_least_one_of_them_is_binary == false);
	assert_identical_blobs_comparison(expected);

	memset(&expected, 0, sizeof(expected));
	cl_git_pass(git_diff_blobs(
		alien, alien, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert(expected.at_least_one_of_them_is_binary == true);
	assert_identical_blobs_comparison(expected);
}

static void assert_binary_blobs_comparison(diff_expects expected)
{
	cl_assert(expected.at_least_one_of_them_is_binary == true);

	cl_assert_equal_i(1, expected.files);
	cl_assert_equal_i(1, expected.file_mods);
	cl_assert_equal_i(0, expected.hunks);
	cl_assert_equal_i(0, expected.lines);
}

void test_diff_blob__can_compare_two_binary_blobs(void)
{
	git_blob *heart;
	git_oid h_oid;

	/* heart.png */
	cl_git_pass(git_oid_fromstrn(&h_oid, "de863bff", 8));
	cl_git_pass(git_blob_lookup_prefix(&heart, g_repo, &h_oid, 4));

	cl_git_pass(git_diff_blobs(
		alien, heart, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	assert_binary_blobs_comparison(expected);

	memset(&expected, 0, sizeof(expected));

	cl_git_pass(git_diff_blobs(
		heart, alien, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	assert_binary_blobs_comparison(expected);

	git_blob_free(heart);
}

void test_diff_blob__can_compare_a_binary_blob_and_a_text_blob(void)
{
	cl_git_pass(git_diff_blobs(
		alien, d, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	assert_binary_blobs_comparison(expected);

	memset(&expected, 0, sizeof(expected));

	cl_git_pass(git_diff_blobs(
		d, alien, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	assert_binary_blobs_comparison(expected);
}

/*
 * $ git diff fe773770 a0f7217
 * diff --git a/fe773770 b/a0f7217
 * index fe77377..a0f7217 100644
 * --- a/fe773770
 * +++ b/a0f7217
 * @@ -1,6 +1,6 @@
 *  Here is some stuff at the start
 * 
 * -This should go in one hunk
 * +This should go in one hunk (first)
 * 
 *  Some additional lines
 * 
 * @@ -8,7 +8,7 @@ Down here below the other lines
 * 
 *  With even more at the end
 * 
 * -Followed by a second hunk of stuff
 * +Followed by a second hunk of stuff (second)
 * 
 *  That happens down here
 */
void test_diff_blob__comparing_two_text_blobs_honors_interhunkcontext(void)
{
	git_blob *old_d;
	git_oid old_d_oid;

	opts.context_lines = 3;

	/* tests/resources/attr/root_test1 from commit f5b0af1 */
	cl_git_pass(git_oid_fromstrn(&old_d_oid, "fe773770", 8));
	cl_git_pass(git_blob_lookup_prefix(&old_d, g_repo, &old_d_oid, 4));

	/* Test with default inter-hunk-context (not set) => default is 0 */
	cl_git_pass(git_diff_blobs(
		old_d, d, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert_equal_i(2, expected.hunks);

	/* Test with inter-hunk-context explicitly set to 0 */
	opts.interhunk_lines = 0;
	memset(&expected, 0, sizeof(expected));
	cl_git_pass(git_diff_blobs(
		old_d, d, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert_equal_i(2, expected.hunks);

	/* Test with inter-hunk-context explicitly set to 1 */
	opts.interhunk_lines = 1;
	memset(&expected, 0, sizeof(expected));
	cl_git_pass(git_diff_blobs(
		old_d, d, &opts, &expected, diff_file_fn, diff_hunk_fn, diff_line_fn));

	cl_assert_equal_i(1, expected.hunks);

	git_blob_free(old_d);
}
