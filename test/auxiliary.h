#ifndef AUXILIARY
#define AUXILIARY

#include "gtest/gtest.h"
#include "LongNumber.h"
#include "LongNumberExceptions.h"

LongNumber t_add(LongNumber left, LongNumber right, char& symbol)
{
    symbol = '+';
    return left + right;
};
LongNumber t_sub(LongNumber left, LongNumber right, char& symbol)
{
    symbol = '-';
    return left - right;
};
LongNumber t_mult(LongNumber left, LongNumber right, char& symbol)
{
    symbol = '*';
    return left * right;
};
LongNumber t_div(LongNumber left, LongNumber right, char& symbol)
{
    symbol = '/';
    return left / right;
};

void t_check(const char* left, const char* right,
    LongNumber(*operation)(LongNumber, LongNumber, char&),
    const char* correct)
{
    char symbol;
    LongNumber answer(correct);
    LongNumber ln1(left);
    LongNumber ln2(right);

    char res[100];
    LongNumber result = operation(ln1, ln2, symbol);
    result.toString(res);
    EXPECT_EQ(answer, result) << left << " " << symbol << " " << right
        << " == " << res << " != " << correct;
}

#endif