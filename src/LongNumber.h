#ifndef LONGNUMBER
#define LONGNUMBER

#include <limits.h>
#include "NumberContainer.h"

class LongNumber
{
public:
    LongNumber(char const* strnum);
    ~LongNumber();

    int length() const;
    void toString(char* str) const;

    char getDigit(int position) const;
    bool sign() const;

    LongNumber operator+(LongNumber const& other) const;
    LongNumber operator-(LongNumber const& other) const;
    LongNumber operator*(LongNumber const& other) const;
    LongNumber operator/(LongNumber const& other) const;

    bool operator==(LongNumber const& other) const;
    bool operator!=(LongNumber const& other) const;

private:
    NumberContainer container;

    LongNumber(NumberContainer num);

    static bool checkAdd(long long num1, long long num2, long long & result);
    static bool checkMult(long long num1, long long num2, long long & result);

    static int compareAbs(const NumberContainer& cont1, const NumberContainer& cont2);

    // adds two numbers from containers, result has the sign of left
    // assumes, both numbers aren't static
    static NumberContainer add(const NumberContainer& left, const NumberContainer& right);
    // subtracts right from left, result has the sign of left
    // assumes left >= right, both are not static
    static NumberContainer sub(const NumberContainer& left, const NumberContainer& right);

};

#endif