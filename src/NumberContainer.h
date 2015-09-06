#ifndef NUMBERCONTAINER
#define NUMBERCONTAINER
#include <limits.h>

class NumberContainer
{
public:
    NumberContainer(long long value)
    {
        setStatic(true);
        if (value <= MAX_STATIC_VALUE && value >= -MAX_STATIC_VALUE)
        {
            setSign(value < 0);
            setValue(value);
        }
    }
    NumberContainer(bool isStatic, int length);
    NumberContainer(const NumberContainer& other);
    ~NumberContainer()
    {
        if (!isStatic())
        {
            delete[] number.dyn.digits;
            number.dyn.digits = 0;
        }
    }
    const NumberContainer& operator=(const NumberContainer& container);

    bool isStatic() const { return (number.st.stat & 1) == 1; }

    long long getValue() const;
    void setValue(long long value);

    int length() const;
    char getDigit(int position) const;
    void setDigit(int position, char digit);

    void setSign(bool hasSign)
    {
        if (hasSign)
            number.st.sign |= 1;
        else
            number.st.sign &= 0;
    }
    void flipSign() { number.st.sign = !number.st.sign; }
    bool hasSign() const { return (number.st.sign & 1) == 1; }

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

    void setStatic(bool isStatic)
    {
        if (isStatic)
            number.st.stat |= 1;
        else
            number.st.stat &= 0;
    }

    char getLower(char byte) const
    {
        return byte & 0xF;// 0xF == 00001111
    }
    char getHigher(char byte) const
    {
        return (byte & 0xF0) >> 4;// 0xF0 == 11110000
    }
    void setLower(char* byte, char digit)
    {
        *byte &= 0xF0;// 0xF0 == 11110000, clear lower 4 bits
        digit &= 0xF;// 0xF == 00001111, clear higher bits just in case
        *byte |= digit;// set lower bits
    }
    void setHigher(char* byte, char digit)
    {
        *byte &= 0xF;// 0xF == 00001111, clear higher 4 bits
        digit &= 0xF;// 0xF == 00001111, clear higher bits just in case
        digit = digit << 4;// move lower 4 bits to upper 4 bits
        *byte |= digit;// set higher bits
    }

    char* allocate(int length)
    {
        int memLen = sizeof(char) * (length / 2 + (length & 1) + 1);
        char* mem = new char[memLen];
        for (int i = 0; i < memLen; i++)
            mem[i] = 0;
        mem[memLen - 1] = FINAL_DIGIT;
        if ((length & 1) == 1)
            setHigher(&mem[memLen - 2], FINAL_DIGIT);
        return mem;
    }

    static const char FINAL_DIGIT = (char)0xFF;// 11111111
};

#endif