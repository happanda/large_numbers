#include "auxiliary.h"

TEST(STATIC, PP_Plus_01)
{
   t_check("999", "101", t_add, "1100");
   t_check("99090222", "909888", t_add, "100000110");

   t_check("999", "-101", t_add, "898");
   t_check("99090222", "-909888", t_add, "98180334");

   t_check("-999", "101", t_add, "-898");
   t_check("-99090222", "909888", t_add, "-98180334");

   t_check("-999", "-101", t_add, "-1100");
   t_check("-99090222", "-909888", t_add, "-100000110");
}
TEST(STATIC, PP_Plus_02)
{
   t_check("10000000000", "10000000000", t_add, "20000000000");
   t_check("89998", "2", t_add, "90000");

   t_check("10000000000", "-10000000000", t_add, "0");
   t_check("89998", "-2", t_add, "89996");

   t_check("-10000000000", "10000000000", t_add, "0");
   t_check("-89998", "2", t_add, "-89996");

   t_check("-10000000000", "-10000000000", t_add, "-20000000000");
   t_check("-89998", "-2", t_add, "-90000");
}
TEST(STATIC, PP_Minus_01)
{
   t_check("10000000000001", "21111111", t_sub, "9999978888890");
   t_check("1234567890", "1030507090", t_sub, "204060800");

   t_check("10000000000001", "-21111111", t_sub, "10000021111112");
   t_check("1234567890", "-1030507090", t_sub, "2265074980");

   t_check("-10000000000001", "21111111", t_sub, "-10000021111112");
   t_check("-1234567890", "1030507090", t_sub, "-2265074980");

   t_check("-10000000000001", "-21111111", t_sub, "-9999978888890");
   t_check("-1234567890", "-1030507090", t_sub, "-204060800");
}
TEST(STATIC, PP_Minus_02)
{
   t_check("1", "2", t_sub, "-1");
   t_check("1", "1234567890", t_sub, "-1234567889");

   t_check("1", "-2", t_sub, "3");
   t_check("1", "-1234567890", t_sub, "1234567891");

   t_check("-1", "2", t_sub, "-3");
   t_check("-1", "1234567890", t_sub, "-1234567891");

   t_check("-1", "-2", t_sub, "1");
   t_check("-1", "-1234567890", t_sub, "1234567889");
}
TEST(STATIC, PP_t_mult_01)
{
   t_check("99", "99", t_mult, "9801");
   t_check("55555", "55555", t_mult, "3086358025");

   t_check("99", "-99", t_mult, "-9801");
   t_check("55555", "-55555", t_mult, "-3086358025");

   t_check("-99", "99", t_mult, "-9801");
   t_check("-55555", "55555", t_mult, "-3086358025");

   t_check("-99", "-99", t_mult, "9801");
   t_check("-55555", "-55555", t_mult, "3086358025");
}
TEST(STATIC, PP_t_mult_02)
{
   t_check("454545898989", "1", t_mult, "454545898989");
   t_check("1", "232323121212", t_mult, "232323121212");

   t_check("454545898989", "-1", t_mult, "-454545898989");
   t_check("1", "-232323121212", t_mult, "-232323121212");

   t_check("-454545898989", "1", t_mult, "-454545898989");
   t_check("-1", "232323121212", t_mult, "-232323121212");

   t_check("-454545898989", "-1", t_mult, "454545898989");
   t_check("-1", "-232323121212", t_mult, "232323121212");
}
TEST(STATIC, PP_t_div_01)
{
   t_check("9876598765", "98765", t_div, "100001");
   t_check("555568", "55", t_div, "10101");
   t_check("12345", "55555", t_div, "0");

   t_check("9876598765", "-98765", t_div, "-100001");
   t_check("555568", "-55", t_div, "-10101");
   t_check("12345", "-55555", t_div, "0");

   t_check("-9876598765", "98765", t_div, "-100001");
   t_check("-555568", "55", t_div, "-10101");
   t_check("-12345", "55555", t_div, "0");

   t_check("-9876598765", "-98765", t_div, "100001");
   t_check("-555568", "-55", t_div, "10101");
   t_check("-12345", "-55555", t_div, "0");

   EXPECT_THROW({t_check("30012012", "0", t_div, "0");}, ArithmeticException);
}
TEST(STATIC, PP_t_div_02)
{
   t_check("9876598765", "1", t_div, "9876598765");
   t_check("31415926", "1", t_div, "31415926");
   t_check("555568", "555568", t_div, "1");
   t_check("31415926", "31415926", t_div, "1");

   t_check("9876598765", "-1", t_div, "-9876598765");
   t_check("31415926", "-1", t_div, "-31415926");
   t_check("555568", "-555568", t_div, "-1");
   t_check("31415926", "-31415926", t_div, "-1");

   t_check("-9876598765", "1", t_div, "-9876598765");
   t_check("-31415926", "1", t_div, "-31415926");
   t_check("-555568", "555568", t_div, "-1");
   t_check("-31415926", "31415926", t_div, "-1");

   t_check("-9876598765", "-1", t_div, "9876598765");
   t_check("-31415926", "-1", t_div, "31415926");
   t_check("-555568", "-555568", t_div, "1");
   t_check("-31415926", "-31415926", t_div, "1");
}
