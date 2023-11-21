
#include <gtest/gtest.h>

#include "s21_SmartCalc.h"

TEST(test_errors, errors) {
  // char tt[260] = "))(35.54.43. */+84 in(+5)-  wef- ssin - x-X + 3,5";
  std::string tt = "))(35.54.43. */+84 in(+5)-  wef- ssin - x-X + 3,5";
  s21::back R(tt);
  int test = R.Checks();
  EXPECT_EQ(test, 1);
  double test2 = R.GetCalcRezult(0);
  EXPECT_TRUE(test2 = NAN);
}

TEST(test_result, test_res_1) {
  std::string tt = "25 +4,6/ 4 - 54 * SQrt(4)";
  s21::back OBJ(tt);
  double test = OBJ.GetCalcRezult(0);
  EXPECT_TRUE(fabs(test + 81.85) < 10E-7);
}

TEST(test_result, test_res_2) {
  double test = 0;
  std::string rr = "(cos(-2^4)^2)*5/3+(ln(sqrt(+25))-(sin(-4)))";
  s21::back OBJ(rr);
  test = OBJ.GetCalcRezult(0);
  EXPECT_TRUE(fabs(test - 2.3811548842) < 10E-7);
}

TEST(test_result, test_res_3) {
  double test = 0;
  std::string pp = "(-cos(-2^4)^2)*5/3+(ln(sqrt(25))-(sin(-4)))";
  s21::back OBJ(pp);
  test = OBJ.GetCalcRezult(0);
  EXPECT_TRUE(fabs(test + 0.67588404) < 10E-7);
}

TEST(test_result, test_res_4) {
  double test = 0;
  std::string ww = "log(15) / ln(30) * 34 mod 15 - asin(0.34)";
  s21::back OBJ(ww);
  test = OBJ.GetCalcRezult(0);
  EXPECT_TRUE(fabs(test - 11.409855297) < 10E-7);
}

TEST(test_result, test_res_5) {
  double test = 0;
  std::string vv = "atan(15) *(-34.67) / tan(30) * 34 mod 15 - acos(0,34)";
  s21::back OBJ(vv);
  test = OBJ.GetCalcRezult(0);
  EXPECT_TRUE(fabs(test - 5.6008497794) < 10E-7);
}

TEST(test_result, test_res_6) {
  double test = 0;
  std::string zz =
      "atan(15) *(-34.67) / tan(30) * X *34 mod 15 - acos(0,34) +x";
  s21::back OBJ(zz);
  test = OBJ.GetCalcRezult(15.456);
  EXPECT_TRUE(fabs(test - 17.83513522032) < 10E-7);
}

TEST(test_result, test_res_7) {
  double test = 0;
  std::string zz =
      "atan(15) *(-3.467E1) / tan(30) * X *34 mod 15 - acos(34E-2) +x";
  s21::back OBJ(zz);
  test = OBJ.GetCalcRezult(15.456);
  EXPECT_TRUE(fabs(test - 17.83513522032) < 10E-7);
}

TEST(test_result, test_credit) {
  double month;
  double over;
  double all;
  double summ = 200000;
  int term = 30;
  double proc = 10;
  int type = 1;
  s21::back OBJ;
  OBJ.Credit(summ, term, proc, type, &month, &over, &all);
  EXPECT_EQ((int)month, 7562);
  type = 2;
  OBJ.Credit(summ, term, proc, type, &month, &over, &all);
  EXPECT_EQ((int)month, 8333);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
