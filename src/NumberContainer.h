#ifndef NUMBERCONTAINER
#define NUMBERCONTAINER
#include <limits.h>

class NumberContainer
{
public:
    NumberContainer(long long value);
    NumberContainer(bool isStatic, int length);
    NumberContainer(NumberContainer const& other);
    ~NumberContainer();
    NumberContainer const& operator=(NumberContainer const& container);

    bool isStatic() const;

    long long getValue() const;
    void setValue(long long value);

    int length() const;
    char getDigit(int position) const;
    void setDigit(int position, char digit);

    void setSign(bool hasSign);
    void flipSign();
    bool hasSign() const;

    static const long long MAX_STATIC_VALUE = (LLONG_MAX >> 1);// LLONG_MAX / 2

private:
    union container
    {
        struct static_number
        {
            long long sign : 1;
            long long stat : 1;
            long long value : sizeof(long long) * 8 - 2;
        } st;
        struct dynamic_number
        {
            char sign : 1;
            char stat : 1;
            char* digits;
        } dyn;
    } number;

    void setStatic(bool isStatic);
    char getLower(char byte) const;
    char getHigher(char byte) const;
    void setLower(char* byte, char digit);
    void setHigher(char* byte, char digit);
    char* allocate(int length);

    static char const FINAL_DIGIT = (char)0xFF;// 11111111
};

#endif