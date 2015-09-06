
#include "NumberContainer.h"

/********************************************************************************
* NumberContainer
* public methods
*********************************************************************************/
NumberContainer::NumberContainer(bool isStatic, int length)
{
    setStatic(isStatic);
    if (!isStatic)
        number.dyn.digits = allocate(length);
}

NumberContainer::NumberContainer(const NumberContainer& other)
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

const NumberContainer& NumberContainer::operator=(const NumberContainer& other)
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
