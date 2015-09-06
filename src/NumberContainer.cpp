
#include "NumberContainer.h"

/********************************************************************************
* NumberContainer
* public methods
*********************************************************************************/
NumberContainer::NumberContainer(long long value)
{
    setStatic(true);
    if (value <= MAX_STATIC_VALUE && value >= -MAX_STATIC_VALUE)
    {
        setSign(value < 0);
        setValue(value);
    }
}

NumberContainer::NumberContainer(bool isStatic, int length)
{
    setStatic(isStatic);
    if (!isStatic)
        number.dyn.digits = allocate(length);
}

NumberContainer::NumberContainer(NumberContainer const& other)
{
    setStatic(other.isStatic());
    setSign(other.hasSign());
    if (other.isStatic())
        setValue(other.getValue());
    else
    {
        int length = other.length();
        number.dyn.digits = allocate(length);
        for (int i = 0; i < length; i++)
            setDigit(i, other.getDigit(i));
    }
}

NumberContainer::~NumberContainer()
{
    if (!isStatic())
    {
        delete[] number.dyn.digits;
        number.dyn.digits = nullptr;
    }
}

NumberContainer const& NumberContainer::operator=(NumberContainer const& other)
{
    if (this == &other)
        return *this;
    if (!isStatic())
        delete[] number.dyn.digits;
    setStatic(other.isStatic());
    setSign(other.hasSign());
    if (other.isStatic())
        setValue(other.getValue());
    else
    {
        int length = other.length();
        number.dyn.digits = allocate(length);
        for (int i = 0; i < length; i++)
            setDigit(i, other.getDigit(i));
    }
    return *this;
}

bool NumberContainer::isStatic() const
{
    return (number.st.stat & 1) == 1;
}

int NumberContainer::length() const
{
    if (isStatic())
    {
        if (number.st.value == 0)
            return 1;
        long long val = number.st.value;
        int length = 0;
        while (val > 0)
        {
            val /= 10;
            length++;
        }
        return length;
    }
    else
    {
        char* byte = number.dyn.digits;
        int length = 0;
        while (*byte != FINAL_DIGIT)
        {
            length += 2;
            byte++;
        }
        byte--;
        if (getHigher(*byte) == getHigher(FINAL_DIGIT))
            length--;
        return length;
    }
}

long long NumberContainer::getValue() const
{
    if (isStatic())
        return number.st.value;
    else
        return 0;
}

void NumberContainer::setValue(long long value)
{
    if (isStatic() && value < MAX_STATIC_VALUE && value > -MAX_STATIC_VALUE - 1)
    {
        number.st.value = value;
        if (value < 0)
        {
            number.st.value = -value;
            setSign(true);
        }
    }
}

char NumberContainer::getDigit(int position) const
{
    int len = length();
    if (position < 0 || position >= len)
        return -1;
    if (isStatic())
    {
        long long tens = 1;
        while (position > 0)
        {
            tens *= 10;
            position--;
        }
        return (char)((number.st.value / tens) % 10);
    }
    else
    {
        char* byte = number.dyn.digits;
        byte += (position >> 1);// division by two
        if ((position & 1) == 0)
            return getLower(*byte);
        else
            return getHigher(*byte);
    }
}

void NumberContainer::setDigit(int position, char digit)
{
    int len = length();
    if (position < 0 || position >= len || digit > 9 || digit < 0)
        return;
    if (isStatic())
    {
        long long val = number.st.value;
        long long tens = 1;
        while (position > 0)
        {
            tens *= 10;
            position--;
        }
        val = number.st.value;
        val = val - ((val / tens) % 10) * tens + digit * tens;
        number.st.value = val;
    }
    else
    {
        char* byte = number.dyn.digits;
        byte += (position >> 1);// division by two
        if ((position & 1) == 0)
            return setLower(byte, digit);
        else
            return setHigher(byte, digit);
    }
}

void NumberContainer::setSign(bool hasSign)
{
    if (hasSign)
        number.st.sign |= 1;
    else
        number.st.sign &= 0;
}

void NumberContainer::flipSign()
{
    number.st.sign = !number.st.sign;
}

bool NumberContainer::hasSign() const
{
    return (number.st.sign & 1) == 1;
}

void NumberContainer::setStatic(bool isStatic)
{
    if (isStatic)
        number.st.stat |= 1;
    else
        number.st.stat &= 0;
}

char NumberContainer::getLower(char byte) const
{
    return byte & 0xF;// 0xF == 00001111
}

char NumberContainer::getHigher(char byte) const
{
    return (byte & 0xF0) >> 4;// 0xF0 == 11110000
}

void NumberContainer::setLower(char* byte, char digit)
{
    *byte &= 0xF0;// 0xF0 == 11110000, clear lower 4 bits
    digit &= 0xF;// 0xF == 00001111, clear higher bits just in case
    *byte |= digit;// set lower bits
}

void NumberContainer::setHigher(char* byte, char digit)
{
    *byte &= 0xF;// 0xF == 00001111, clear higher 4 bits
    digit &= 0xF;// 0xF == 00001111, clear higher bits just in case
    digit = digit << 4;// move lower 4 bits to upper 4 bits
    *byte |= digit;// set higher bits
}

char* NumberContainer::allocate(int length)
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
