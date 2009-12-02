
#ifndef STRDIFF_TEST
#define STRDIFF_TEST

#include "dtl_test_common.hpp"

class StrDiffTest : public ::testing::Test
{
protected :
  typedef char elem;
  typedef string sequence;
  typedef pair< elem, elemInfo > sesElem;
  typedef vector< elem > elemVec;
  typedef vector< sesElem > sesElemVec;
  typedef vector < uniHunk< sesElem > > uniHunkVec;
  typedef struct case_t {
    sequence A;
    sequence B;
    Diff< elem, sequence > diff;
    elemVec lcs_v;
    sequence lcs_s;
    sesElemVec ses_seq;
    uniHunkVec hunk_v;
  } case_t;
  typedef vector< case_t > caseVec;

  caseVec diff_cases;
  caseVec only_editdis_cases;

  case_t createCase (sequence a, sequence b, bool onlyEditdis = false) {
    case_t c;
    c.A = a;
    c.B = b;
    c.diff = Diff< elem, sequence >(a, b);
    if (onlyEditdis) {
      c.diff.onOnlyEditDistance();
    }
    c.diff.compose();
    c.diff.composeUnifiedHunks();
    elemVec lcs_v = c.diff.getLcsVec();
    c.lcs_s       = sequence(lcs_v.begin(), lcs_v.end());
    c.ses_seq     = c.diff.getSes().getSequence();
    c.hunk_v      = c.diff.getUniHunks();
    return c;
  }

  void SetUp() {
    diff_cases.push_back(createCase("abc",        "abd"));          // 0
    diff_cases.push_back(createCase("acbdeacbed", "acebdabbabed")); // 1
    diff_cases.push_back(createCase("abcdef",     "dacfea"));       // 2
    diff_cases.push_back(createCase("abcbda",     "bdcaba"));       // 3
    diff_cases.push_back(createCase("bokko",      "bokkko"));       // 4
    diff_cases.push_back(createCase("",           ""));             // 5
    diff_cases.push_back(createCase("a",          ""));             // 6
    diff_cases.push_back(createCase("",           "b"));            // 7
    
    only_editdis_cases.push_back(createCase("abc",        "abd",          true));       // 0
    only_editdis_cases.push_back(createCase("acbdeacbed", "acebdabbabed", true));       // 1
    only_editdis_cases.push_back(createCase("abcdef",     "dacfea",       true));       // 2
    only_editdis_cases.push_back(createCase("abcbda",     "bdcaba",       true));       // 3
    only_editdis_cases.push_back(createCase("bokko",      "bokkko",       true));       // 4
    only_editdis_cases.push_back(createCase("",           "",             true));       // 5
    only_editdis_cases.push_back(createCase("a",          "",             true));       // 6
    only_editdis_cases.push_back(createCase("",           "b",            true));       // 7
  }

  void TearDown () {}
  
};

/**
 * StrDiffTest
 * check list is following
 * - editdistance
 * - LCS
 * - SES
 * - Unified Format Difference
 */

TEST_F (StrDiffTest, diff_test0) {
  
  EXPECT_EQ(2,          diff_cases[0].diff.getEditDistance());
  
  EXPECT_EQ("ab",       diff_cases[0].lcs_s);

  ASSERT_EQ('a',        diff_cases[0].ses_seq[0].first);
  ASSERT_EQ('b',        diff_cases[0].ses_seq[1].first);
  ASSERT_EQ('c',        diff_cases[0].ses_seq[2].first);
  ASSERT_EQ('d',        diff_cases[0].ses_seq[3].first);
  ASSERT_EQ(SES_COMMON, diff_cases[0].ses_seq[0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[0].ses_seq[1].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[0].ses_seq[2].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[0].ses_seq[3].second.type);
  
  ASSERT_EQ(1,          diff_cases[0].hunk_v[0].a);
  ASSERT_EQ(3,          diff_cases[0].hunk_v[0].b);
  ASSERT_EQ(1,          diff_cases[0].hunk_v[0].c);
  ASSERT_EQ(3,          diff_cases[0].hunk_v[0].d);
  ASSERT_EQ('a',        diff_cases[0].hunk_v[0].common[0][0].first);
  ASSERT_EQ('b',        diff_cases[0].hunk_v[0].common[0][1].first);
  ASSERT_EQ('c',        diff_cases[0].hunk_v[0].change[0].first);
  ASSERT_EQ('d',        diff_cases[0].hunk_v[0].change[1].first);
  ASSERT_EQ(SES_COMMON, diff_cases[0].hunk_v[0].common[0][0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[0].hunk_v[0].common[0][1].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[0].hunk_v[0].change[0].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[0].hunk_v[0].change[1].second.type);
  ASSERT_TRUE(diff_cases[0].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test1) {
  EXPECT_EQ(6,          diff_cases[1].diff.getEditDistance());

  EXPECT_EQ("acbdabed", diff_cases[1].lcs_s);
  
  ASSERT_EQ('a',        diff_cases[1].ses_seq[0].first);
  ASSERT_EQ('c',        diff_cases[1].ses_seq[1].first);
  ASSERT_EQ('e',        diff_cases[1].ses_seq[2].first);
  ASSERT_EQ('b',        diff_cases[1].ses_seq[3].first);
  ASSERT_EQ('d',        diff_cases[1].ses_seq[4].first);
  ASSERT_EQ('e',        diff_cases[1].ses_seq[5].first);
  ASSERT_EQ('a',        diff_cases[1].ses_seq[6].first);
  ASSERT_EQ('c',        diff_cases[1].ses_seq[7].first);
  ASSERT_EQ('b',        diff_cases[1].ses_seq[8].first);
  ASSERT_EQ('b',        diff_cases[1].ses_seq[9].first);
  ASSERT_EQ('a',        diff_cases[1].ses_seq[10].first);
  ASSERT_EQ('b',        diff_cases[1].ses_seq[11].first);
  ASSERT_EQ('e',        diff_cases[1].ses_seq[12].first);
  ASSERT_EQ('d',        diff_cases[1].ses_seq[13].first);
  ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[1].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[1].ses_seq[2].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[3].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[4].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[1].ses_seq[5].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[6].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[1].ses_seq[7].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[8].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[1].ses_seq[9].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[1].ses_seq[10].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[1].ses_seq[11].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[12].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[13].second.type);

  ASSERT_EQ(1,          diff_cases[1].hunk_v[0].a);
  ASSERT_EQ(10,         diff_cases[1].hunk_v[0].b);
  ASSERT_EQ(1,          diff_cases[1].hunk_v[0].c);
  ASSERT_EQ(12,         diff_cases[1].hunk_v[0].d);
  ASSERT_EQ('a',        diff_cases[1].hunk_v[0].common[0][0].first);
  ASSERT_EQ('c',        diff_cases[1].hunk_v[0].common[0][1].first);
  ASSERT_EQ('e',        diff_cases[1].hunk_v[0].change[0].first);
  ASSERT_EQ('b',        diff_cases[1].hunk_v[0].change[1].first);
  ASSERT_EQ('d',        diff_cases[1].hunk_v[0].change[2].first);
  ASSERT_EQ('e',        diff_cases[1].hunk_v[0].change[3].first);
  ASSERT_EQ('a',        diff_cases[1].hunk_v[0].change[4].first);
  ASSERT_EQ('c',        diff_cases[1].hunk_v[0].change[5].first);
  ASSERT_EQ('b',        diff_cases[1].hunk_v[0].change[6].first);
  ASSERT_EQ('b',        diff_cases[1].hunk_v[0].change[7].first);
  ASSERT_EQ('a',        diff_cases[1].hunk_v[0].change[8].first);
  ASSERT_EQ('b',        diff_cases[1].hunk_v[0].change[9].first);
  ASSERT_EQ('e',        diff_cases[1].hunk_v[0].change[10].first);
  ASSERT_EQ('d',        diff_cases[1].hunk_v[0].change[11].first);
  ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].common[0][0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].common[0][1].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[1].hunk_v[0].change[0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[1].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[2].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[1].hunk_v[0].change[3].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[4].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[1].hunk_v[0].change[5].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[6].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[1].hunk_v[0].change[7].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[1].hunk_v[0].change[8].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[1].hunk_v[0].change[9].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[10].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[11].second.type);
  ASSERT_TRUE(diff_cases[1].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test2) {
  EXPECT_EQ(6,          diff_cases[2].diff.getEditDistance());

  EXPECT_EQ("acf",      diff_cases[2].lcs_s);

  ASSERT_EQ('d',        diff_cases[2].ses_seq[0].first);
  ASSERT_EQ('a',        diff_cases[2].ses_seq[1].first);
  ASSERT_EQ('b',        diff_cases[2].ses_seq[2].first);
  ASSERT_EQ('c',        diff_cases[2].ses_seq[3].first);
  ASSERT_EQ('d',        diff_cases[2].ses_seq[4].first);
  ASSERT_EQ('e',        diff_cases[2].ses_seq[5].first);
  ASSERT_EQ('f',        diff_cases[2].ses_seq[6].first);
  ASSERT_EQ('e',        diff_cases[2].ses_seq[7].first);
  ASSERT_EQ('a',        diff_cases[2].ses_seq[8].first);
  ASSERT_EQ(SES_ADD,    diff_cases[2].ses_seq[0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[2].ses_seq[1].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[2].ses_seq[2].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[2].ses_seq[3].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[2].ses_seq[4].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[2].ses_seq[5].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[2].ses_seq[6].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[2].ses_seq[7].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[2].ses_seq[8].second.type);

  ASSERT_EQ(1,          diff_cases[2].hunk_v[0].a);
  ASSERT_EQ(6,          diff_cases[2].hunk_v[0].b);
  ASSERT_EQ(1,          diff_cases[2].hunk_v[0].c);
  ASSERT_EQ(6,          diff_cases[2].hunk_v[0].d);
  ASSERT_EQ('d',        diff_cases[2].hunk_v[0].change[0].first);
  ASSERT_EQ('a',        diff_cases[2].hunk_v[0].change[1].first);
  ASSERT_EQ('b',        diff_cases[2].hunk_v[0].change[2].first);
  ASSERT_EQ('c',        diff_cases[2].hunk_v[0].change[3].first);
  ASSERT_EQ('d',        diff_cases[2].hunk_v[0].change[4].first);
  ASSERT_EQ('e',        diff_cases[2].hunk_v[0].change[5].first);
  ASSERT_EQ('f',        diff_cases[2].hunk_v[0].change[6].first);
  ASSERT_EQ('e',        diff_cases[2].hunk_v[0].change[7].first);
  ASSERT_EQ('a',        diff_cases[2].hunk_v[0].change[8].first);
  ASSERT_TRUE(diff_cases[2].hunk_v[0].common[0].empty());
  ASSERT_EQ(SES_ADD,    diff_cases[2].hunk_v[0].change[0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[2].hunk_v[0].change[1].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[2].hunk_v[0].change[2].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[2].hunk_v[0].change[3].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[2].hunk_v[0].change[4].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[2].hunk_v[0].change[5].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[2].hunk_v[0].change[6].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[2].hunk_v[0].change[7].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[2].hunk_v[0].change[8].second.type);
  ASSERT_TRUE(diff_cases[2].hunk_v[0].common[1].empty());
  
}

TEST_F (StrDiffTest, diff_test3) {
  EXPECT_EQ(4,          diff_cases[3].diff.getEditDistance());

  EXPECT_EQ("bcba",     diff_cases[3].lcs_s);

  ASSERT_EQ('a',        diff_cases[3].ses_seq[0].first);
  ASSERT_EQ('b',        diff_cases[3].ses_seq[1].first);
  ASSERT_EQ('d',        diff_cases[3].ses_seq[2].first);
  ASSERT_EQ('c',        diff_cases[3].ses_seq[3].first);
  ASSERT_EQ('a',        diff_cases[3].ses_seq[4].first);
  ASSERT_EQ('b',        diff_cases[3].ses_seq[5].first);
  ASSERT_EQ('d',        diff_cases[3].ses_seq[6].first);
  ASSERT_EQ('a',        diff_cases[3].ses_seq[7].first);
  ASSERT_EQ(SES_DELETE, diff_cases[3].ses_seq[0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[3].ses_seq[1].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[3].ses_seq[2].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[3].ses_seq[3].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[3].ses_seq[4].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[3].ses_seq[5].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[3].ses_seq[6].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[3].ses_seq[7].second.type);

  ASSERT_EQ(1,          diff_cases[3].hunk_v[0].a);
  ASSERT_EQ(6,          diff_cases[3].hunk_v[0].b);
  ASSERT_EQ(1,          diff_cases[3].hunk_v[0].c);
  ASSERT_EQ(6,          diff_cases[3].hunk_v[0].d);
  ASSERT_TRUE(diff_cases[3].hunk_v[0].common[0].empty());
  ASSERT_EQ('a',        diff_cases[3].hunk_v[0].change[0].first);
  ASSERT_EQ('b',        diff_cases[3].hunk_v[0].change[1].first);
  ASSERT_EQ('d',        diff_cases[3].hunk_v[0].change[2].first);
  ASSERT_EQ('c',        diff_cases[3].hunk_v[0].change[3].first);
  ASSERT_EQ('a',        diff_cases[3].hunk_v[0].change[4].first);
  ASSERT_EQ('b',        diff_cases[3].hunk_v[0].change[5].first);
  ASSERT_EQ('d',        diff_cases[3].hunk_v[0].change[6].first);
  ASSERT_EQ('a',        diff_cases[3].hunk_v[0].change[7].first);
  ASSERT_EQ(SES_DELETE, diff_cases[3].hunk_v[0].change[0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[3].hunk_v[0].change[1].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[3].hunk_v[0].change[2].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[3].hunk_v[0].change[3].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[3].hunk_v[0].change[4].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[3].hunk_v[0].change[5].second.type);
  ASSERT_EQ(SES_DELETE, diff_cases[3].hunk_v[0].change[6].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[3].hunk_v[0].change[7].second.type);
  ASSERT_TRUE(diff_cases[3].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test4) {
  EXPECT_EQ(1,          diff_cases[4].diff.getEditDistance());

  EXPECT_EQ("bokko",    diff_cases[4].lcs_s);

  ASSERT_EQ('b',        diff_cases[4].ses_seq[0].first);
  ASSERT_EQ('o',        diff_cases[4].ses_seq[1].first);
  ASSERT_EQ('k',        diff_cases[4].ses_seq[2].first);
  ASSERT_EQ('k',        diff_cases[4].ses_seq[3].first);
  ASSERT_EQ('k',        diff_cases[4].ses_seq[4].first);
  ASSERT_EQ('o',        diff_cases[4].ses_seq[5].first);
  ASSERT_EQ(SES_COMMON, diff_cases[4].ses_seq[0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[4].ses_seq[1].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[4].ses_seq[2].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[4].ses_seq[3].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[4].ses_seq[4].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[4].ses_seq[5].second.type);

  ASSERT_EQ(2,          diff_cases[4].hunk_v[0].a);
  ASSERT_EQ(4,          diff_cases[4].hunk_v[0].b);
  ASSERT_EQ(2,          diff_cases[4].hunk_v[0].c);
  ASSERT_EQ(5,          diff_cases[4].hunk_v[0].d);
  ASSERT_EQ('o',        diff_cases[4].hunk_v[0].common[0][0].first);
  ASSERT_EQ('k',        diff_cases[4].hunk_v[0].common[0][1].first);
  ASSERT_EQ('k',        diff_cases[4].hunk_v[0].common[0][2].first);
  ASSERT_EQ('k',        diff_cases[4].hunk_v[0].change[0].first);
  ASSERT_EQ('o',        diff_cases[4].hunk_v[0].change[1].first);
  ASSERT_EQ(SES_COMMON, diff_cases[4].hunk_v[0].common[0][0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[4].hunk_v[0].common[0][1].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[4].hunk_v[0].common[0][2].second.type);
  ASSERT_EQ(SES_ADD,    diff_cases[4].hunk_v[0].change[0].second.type);
  ASSERT_EQ(SES_COMMON, diff_cases[4].hunk_v[0].change[1].second.type);
  ASSERT_TRUE(diff_cases[4].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test5) {
  EXPECT_EQ(0,  diff_cases[5].diff.getEditDistance());

  EXPECT_EQ("", diff_cases[5].lcs_s);

  ASSERT_TRUE(diff_cases[5].ses_seq.empty());

  ASSERT_TRUE(diff_cases[5].hunk_v.empty());
}

TEST_F (StrDiffTest, diff_test6) {
  EXPECT_EQ(1,          diff_cases[6].diff.getEditDistance());

  EXPECT_EQ("",         diff_cases[6].lcs_s);

  ASSERT_EQ('a',        diff_cases[6].ses_seq[0].first);
  ASSERT_EQ(SES_DELETE, diff_cases[6].ses_seq[0].second.type);

  ASSERT_EQ(1,          diff_cases[6].hunk_v[0].a);
  ASSERT_EQ(1,          diff_cases[6].hunk_v[0].b);
  ASSERT_EQ(1,          diff_cases[6].hunk_v[0].c);
  ASSERT_EQ(0,          diff_cases[6].hunk_v[0].d);
  ASSERT_TRUE(diff_cases[6].hunk_v[0].common[0].empty());
  ASSERT_EQ('a',        diff_cases[6].hunk_v[0].change[0].first);
  ASSERT_EQ(SES_DELETE, diff_cases[6].hunk_v[0].change[0].second.type);
  ASSERT_TRUE(diff_cases[6].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test7) {
  EXPECT_EQ(1,       diff_cases[7].diff.getEditDistance());

  EXPECT_EQ("",      diff_cases[7].lcs_s);

  ASSERT_EQ('b',     diff_cases[7].ses_seq[0].first);
  ASSERT_EQ(SES_ADD, diff_cases[7].ses_seq[0].second.type);

  ASSERT_EQ(1,       diff_cases[7].hunk_v[0].a);
  ASSERT_EQ(0,       diff_cases[7].hunk_v[0].b);
  ASSERT_EQ(1,       diff_cases[7].hunk_v[0].c);
  ASSERT_EQ(1,       diff_cases[7].hunk_v[0].d);
  ASSERT_TRUE(diff_cases[7].hunk_v[0].common[0].empty());
  ASSERT_EQ('b',     diff_cases[7].hunk_v[0].change[0].first);
  ASSERT_EQ(SES_ADD, diff_cases[7].hunk_v[0].change[0].second.type);
  ASSERT_TRUE(diff_cases[7].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, only_editdis_test0) {
  EXPECT_EQ(2,       only_editdis_cases[0].diff.getEditDistance());

  EXPECT_EQ("",      only_editdis_cases[0].lcs_s);

  ASSERT_TRUE(only_editdis_cases[0].ses_seq.empty());

  ASSERT_TRUE(only_editdis_cases[0].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test1) {
  EXPECT_EQ(6,       only_editdis_cases[1].diff.getEditDistance());

  EXPECT_EQ("",      only_editdis_cases[1].lcs_s);

  ASSERT_TRUE(only_editdis_cases[1].ses_seq.empty());

  ASSERT_TRUE(only_editdis_cases[1].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test2) {
  EXPECT_EQ(6,       only_editdis_cases[2].diff.getEditDistance());

  EXPECT_EQ("",      only_editdis_cases[2].lcs_s);

  ASSERT_TRUE(only_editdis_cases[2].ses_seq.empty());

  ASSERT_TRUE(only_editdis_cases[2].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test3) {
  EXPECT_EQ(4,       only_editdis_cases[3].diff.getEditDistance());

  EXPECT_EQ("",      only_editdis_cases[3].lcs_s);

  ASSERT_TRUE(only_editdis_cases[3].ses_seq.empty());

  ASSERT_TRUE(only_editdis_cases[3].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test4) {
  EXPECT_EQ(1,       only_editdis_cases[4].diff.getEditDistance());

  EXPECT_EQ("",      only_editdis_cases[4].lcs_s);

  ASSERT_TRUE(only_editdis_cases[4].ses_seq.empty());

  ASSERT_TRUE(only_editdis_cases[4].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test5) {
  EXPECT_EQ(0,       only_editdis_cases[5].diff.getEditDistance());

  EXPECT_EQ("",      only_editdis_cases[5].lcs_s);

  ASSERT_TRUE(only_editdis_cases[5].ses_seq.empty());

  ASSERT_TRUE(only_editdis_cases[5].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test6) {
  EXPECT_EQ(1,       only_editdis_cases[6].diff.getEditDistance());

  EXPECT_EQ("",      only_editdis_cases[6].lcs_s);

  ASSERT_TRUE(only_editdis_cases[6].ses_seq.empty());

  ASSERT_TRUE(only_editdis_cases[6].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test7) {
  EXPECT_EQ(1,       only_editdis_cases[7].diff.getEditDistance());

  EXPECT_EQ("",      only_editdis_cases[7].lcs_s);

  ASSERT_TRUE(only_editdis_cases[7].ses_seq.empty());

  ASSERT_TRUE(only_editdis_cases[7].hunk_v.empty());
}


#endif