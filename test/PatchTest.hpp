
#ifndef PATCH_TEST
#define PATCH_TEST

#include "dtl_test_common.hpp"

class PatchTest : public ::testing::Test
{
protected :
  typedef char elem;
  typedef string sequence;
  typedef struct case_t {
    sequence A;
    sequence B;
    Diff< elem, sequence > diff;
  } case_t;
  typedef vector< case_t > caseVec;

  caseVec cases;

  case_t createCase (sequence a, sequence b) {
    case_t c;
    c.A = a;
    c.B = b;
    c.diff = Diff< elem, sequence >(a, b);
    c.diff.compose();
    c.diff.composeUnifiedHunks();
    return c;
  }

  void SetUp() {
    cases.push_back(createCase("abc",        "abd"));          // 0
    cases.push_back(createCase("acbdeacbed", "acebdabbabed")); // 1
    cases.push_back(createCase("abcdef",     "dacfea"));       // 2
    cases.push_back(createCase("abcbda",     "bdcaba"));       // 3
    cases.push_back(createCase("bokko",      "bokkko"));       // 4
    cases.push_back(createCase("",           ""));             // 5
    cases.push_back(createCase("a",          ""));             // 6
    cases.push_back(createCase("",           "b"));            // 7
  }

  void TearDown () {}
  
};

/**
 * PatchTest
 * check list is following
 * - patch function
 * - uniPatch function
 */
TEST_F (PatchTest, patch_test0) {
  ASSERT_EQ(cases[0].B, cases[0].diff.patch(cases[0].A));
  ASSERT_EQ(cases[0].B, cases[0].diff.uniPatch(cases[0].A));
}

TEST_F (PatchTest, patch_test1) {
  ASSERT_EQ(cases[1].B, cases[1].diff.patch(cases[1].A));
  ASSERT_EQ(cases[1].B, cases[1].diff.uniPatch(cases[1].A));
}

TEST_F (PatchTest, patch_test2) {
  ASSERT_EQ(cases[2].B, cases[2].diff.patch(cases[2].A));
  ASSERT_EQ(cases[2].B, cases[2].diff.uniPatch(cases[2].A));
}

TEST_F (PatchTest, patch_test3) {
  ASSERT_EQ(cases[3].B, cases[3].diff.patch(cases[3].A));
  ASSERT_EQ(cases[3].B, cases[3].diff.uniPatch(cases[3].A));
}

TEST_F (PatchTest, patch_test4) {
  ASSERT_EQ(cases[4].B, cases[4].diff.patch(cases[4].A));
  ASSERT_EQ(cases[4].B, cases[4].diff.uniPatch(cases[4].A));
}

TEST_F (PatchTest, patch_test5) {
  ASSERT_EQ(cases[5].B, cases[5].diff.patch(cases[5].A));
  ASSERT_EQ(cases[5].B, cases[5].diff.uniPatch(cases[5].A));
}

TEST_F (PatchTest, patch_test6) {
  ASSERT_EQ(cases[6].B, cases[6].diff.patch(cases[6].A));
  ASSERT_EQ(cases[6].B, cases[6].diff.uniPatch(cases[6].A));
}

TEST_F (PatchTest, patch_test7) {
  ASSERT_EQ(cases[7].B, cases[7].diff.patch(cases[7].A));
  ASSERT_EQ(cases[7].B, cases[7].diff.uniPatch(cases[7].A));
}

#endif