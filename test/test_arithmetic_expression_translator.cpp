#include "Arithmetic_Expression_Translator.h"
#include <gtest.h>


TEST(Arithmetic_Expression_Translator, can_create_expression)
{
  ASSERT_NO_THROW(Expression exp("(2+2)*2+1"));
}

TEST(Arithmetic_Expression_Translator, expression_with_invalid_symbols)
{
    Expression exp1("2#4x+f");
    Expression exp2("2..5*3");
    Expression exp3("2.5.3*3");
    ASSERT_ANY_THROW(exp1.translator());
    ASSERT_ANY_THROW(exp2.translator());
    ASSERT_ANY_THROW(exp3.translator());
}

TEST(Arithmetic_Expression_Translator, wrong_next_permissible_symbols)
{
    Expression exp1("*+");
    Expression exp2("*)");
    Expression exp3("2(");
    Expression exp4("(*");
    Expression exp5("()");
    Expression exp6(")5");
    Expression exp7(")(");
  ASSERT_ANY_THROW(exp1.translator());
  ASSERT_ANY_THROW(exp2.translator());
  ASSERT_ANY_THROW(exp3.translator());
  ASSERT_ANY_THROW(exp4.translator());
  ASSERT_ANY_THROW(exp5.translator());
  ASSERT_ANY_THROW(exp6.translator());
  ASSERT_ANY_THROW(exp7.translator());
}

TEST(Arithmetic_Expression_Translator, incorrect_brackets)
{
    Expression exp1("((5+4)");
    Expression exp2("(4*2+1))");
    ASSERT_ANY_THROW(exp1.translator());
    ASSERT_ANY_THROW(exp2.translator());
}

TEST(Arithmetic_Expression_Translator, impossibility_of_division_by_zero)
{
    Expression exp("5/0");
    ASSERT_ANY_THROW(exp.translator());
}

TEST(Arithmetic_Expression_Translator, can_add)
{
    Expression exp("4.5+5.5");
    double ans = 4.5 + 5.5;
    EXPECT_EQ(ans, exp.calculation());
}

TEST(Arithmetic_Expression_Translator, can_multiply)
{
    Expression exp("4.5*5.5");
    double ans = 4.5 * 5.5;
    EXPECT_EQ(ans, exp.calculation());
}

TEST(Arithmetic_Expression_Translator, can_subtract)
{
    Expression exp("14.5-5.5");
    double ans = 14.5 - 5.5;
    EXPECT_EQ(ans, exp.calculation());
}

TEST(Arithmetic_Expression_Translator, can_divide)
{
    Expression exp("14.5/5.5");
    double ans = 14.5 / 5.5;
    EXPECT_EQ(ans, exp.calculation());
}

TEST(Arithmetic_Expression_Translator, give_the_right_answer)
{
    Expression exp("(3.7*9.18+1)/4-(2+7*3.1)*1.5");
    double ans = (3.7 * 9.18 + 1) / 4 - (2 + 7 * 3.1) * 1.5;
    EXPECT_EQ(ans, exp.calculation());
}