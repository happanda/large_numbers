#ifndef "input.h"
#define INPUTH

#include "gtest/gtest.h"
#include "LongNumber.h"
#include "LongNumberExceptions.h"

TEST(INPUT, Correct)
{
    char ch[100];
    char numstr[100];
    numstr = "   123213";
    EXPECT_NO_THROW({LongNumber ln(numstr);});
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "   -123213";
    EXPECT_NO_THROW({ln = LongNumber(numstr);});
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "123213";
    EXPECT_NO_THROW({ln = LongNumber(numstr);});
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "-123213";
    EXPECT_NO_THROW({ln = LongNumber(numstr);});
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);
}

TEST(INPUT, Incorrect)
{
    char ch[100];
    char numstr[100];
    numstr = "   123213  ";
    EXPECT_THROW({LongNumber ln(numstr);}, FormatException);
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "   -123213  ";
    EXPECT_THROW({ln = LongNumber(numstr);}, FormatException);
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "123213  ";
    EXPECT_THROW({ln = LongNumber(numstr);}, FormatException);
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "-123213  ";
    EXPECT_THROW({ln = LongNumber(numstr);}, FormatException);
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "sdf123213  ";
    EXPECT_THROW({ln = LongNumber(numstr);}, FormatException);
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "sdf-123213  ";
    EXPECT_THROW({ln = LongNumber(numstr);}, FormatException);
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "123213sdf";
    EXPECT_THROW({ln = LongNumber(numstr);}, FormatException);
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "-123213sdf";
    EXPECT_THROW({ln = LongNumber(numstr);}, FormatException);
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "--123213";
    EXPECT_THROW({ln = LongNumber(numstr);}, FormatException);
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);

    numstr = "-123213-";
    EXPECT_THROW({ln = LongNumber(numstr);}, FormatException);
    ln.toString(ch);
    EXPECT_STREQ(numstr, ch);
}

#endif