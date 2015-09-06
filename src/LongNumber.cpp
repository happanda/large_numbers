
#include <stdlib.h>
#include "LongNumber.h"
#include "LongNumberExceptions.h"

/********************************************************************************
* LongNumber
* public methods
*********************************************************************************/
LongNumber::LongNumber(const char* strnum) : container(0)
{
   bool hasSign = false;
   bool isStatic = true;
   while (*strnum == ' ')
      strnum++;
   if (*strnum == '+')
      strnum++;
   else if (*strnum == '-')
   {
      strnum++;
      hasSign = true;
   }
   const char* iter = strnum;
   long long curVal = 0;
   int curLength = 0;
   while (*iter != 0)
   {
      curLength++;
      if (isStatic)
      {
         if (*iter < '0' || *iter > '9')
            throw FormatException("Wrong character");
         char digit = *iter - '0';
         if (!checkMult(curVal, 10, curVal))
            isStatic = false;
         else if (!checkAdd(curVal, digit, curVal))
            isStatic = false;
      }
      iter++;
   }
   container = NumberContainer(isStatic, curLength);
   container.setSign(hasSign);
   if (isStatic)
      container.setValue(curVal);
   else
   {
      int position = 0;
      iter--;
      while (iter != strnum)
      {
         if (*iter < '0' || *iter > '9')
         {
            container.~NumberContainer();
            throw FormatException("Wrong character");
         }
         container.setDigit(position, *iter - '0');
         position++;
         iter--;
      }
      container.setDigit(position, *iter - '0');
   }
}

LongNumber::~LongNumber()
{
   container.~NumberContainer();
}

LongNumber LongNumber::operator +(const LongNumber& other) const
{   
   if (container.isStatic() && other.container.isStatic())
   {
      // both numbers are inside long long type
      long long val1 = container.getValue() * (container.hasSign() ? -1 : 1);
      long long val2 = other.container.getValue() * (other.container.hasSign() ? -1 : 1);
      if (checkAdd(val1, val2, val1))
         return LongNumber(NumberContainer(val1));
   }

   if (container.hasSign() ^ other.container.hasSign())
   {
      // different signs, substract min abs from greater by abs
      // use sign of greater by abs
      int comp = compareAbs(container, other.container);
      if (comp >= 0)
         return LongNumber(sub(container, other.container));
      else
         return LongNumber(sub(other.container, container));
   }
   else
      // same signs, just add them
      return LongNumber(add(container, other.container));
}

LongNumber LongNumber::operator -(const LongNumber& other) const
{
   if (container.isStatic() && other.container.isStatic())
   {
      // both numbers are inside long long type
      long long val1 = container.getValue() * (container.hasSign() ? -1 : 1);
      long long val2 = other.container.getValue() * (other.container.hasSign() ? -1 : 1);
      if (checkAdd(val1, -val2, val1))
         return LongNumber(NumberContainer(val1));
   }

   if (container.hasSign() ^ other.container.hasSign())
      // different signs, just add them,
      // use sign of left operand
      return LongNumber(add(container, other.container));
   else
   {
      // same sign, below |X| >= |y|
      int comp = compareAbs(container, other.container);
      if (comp >= 0)
         // X - y, us sign of X
         return LongNumber(sub(container, other.container));
      else
      {
         // y - X, use opposite to sign of X
         NumberContainer numCont = sub(other.container, container);
         numCont.setSign(!other.container.hasSign());
         return LongNumber(numCont);
      }
   }
}

LongNumber LongNumber::operator *(const LongNumber& other) const
{
   if (container.isStatic() && other.container.isStatic())
   {
      // both numbers are inside long long type
      long long val1 = container.getValue() * (container.hasSign() ? -1 : 1);
      long long val2 = other.container.getValue() * (other.container.hasSign() ? -1 : 1);
      if (checkMult(val1, val2, val1))
         return LongNumber(NumberContainer(val1));
   }

   // check for zero
   if (container.isStatic() && container.getValue() == 0
      || other.container.isStatic() && other.container.getValue() == 0)
      return LongNumber(NumberContainer(0));

   int thisLen = container.length();
   int otherLen = other.container.length();
   // use upper bound of length
   NumberContainer num(false, thisLen + otherLen);

   char transfered = 0;
   for (int i = 0; i < thisLen; i++)
   {
      char thisDigit = container.getDigit(i);
      transfered = 0;
      for (int j = 0; j < otherLen; j++)
      {
         char otherDigit = other.getDigit(j);
         char sum = thisDigit * otherDigit + num.getDigit(i + j) + transfered;
         num.setDigit(i + j, sum % 10);
         transfered = sum / 10;
      }
      if (transfered > 0)
         num.setDigit(i + otherLen, transfered);
   }
   num.setSign(container.hasSign() ^ other.container.hasSign());

   int len = transfered > 0 ? thisLen + otherLen : thisLen + otherLen - 1;
   if (transfered > 0)
   {
      // ok, length equals estimated
      num.setDigit(len - 1, transfered);
      return LongNumber(num);
   }
   // need to resize (remove one digit)
   NumberContainer resized(false, len);
   resized.setSign(num.hasSign());
   for (int i = 0; i < len; i++)
      resized.setDigit(i, num.getDigit(i));
   return LongNumber(resized);
}

LongNumber LongNumber::operator /(const LongNumber& other) const
{
   if (other == LongNumber("0"))
      throw ArithmeticException("Division by zero");
   bool needSign = container.hasSign() ^ other.container.hasSign();
   if (container.isStatic() && other.container.isStatic())
   {
      // both numbers are inside long long type, division always inside type
      long long val = container.getValue() / other.container.getValue();
      val *= needSign ? -1 : 1;
      return LongNumber(NumberContainer(val));
   }
   int comp = compareAbs(container, other.container);
   if (comp < 0)
      return LongNumber(NumberContainer(0));
   else if (comp == 0)
   {
      NumberContainer num(1);
      num.setSign(needSign);
      return LongNumber(num);
   }
   // we have to divide here
   int thisLen = container.length();
   int otherLen = other.container.length();
   // use upper bound of length
   int numLen = thisLen - otherLen + 1;
   int pos = numLen;
   NumberContainer num(false, pos--);
   num.setSign(needSign);
   NumberContainer tmp(container);// create copy

   // find where first to place other
   int from = thisLen - 1;
   for (int k1 = 0; k1 < otherLen; k1++)
   {
      int k2 = otherLen - 1 - k1;
      char dig = tmp.getDigit(from - k1);
      char odig = other.getDigit(k2);
      if (dig < odig)
      {
         from--;
         break;
      }
      if (dig > odig)
         break;
   }

   for (; from >= otherLen - 1; from--)
   {
      bool found = false;
      char curDigit = 10;
      // pick digit in answer
      while (!found && curDigit > 0)
      {
         curDigit--;
         found = true;
         int borrowed = 0;
         if (from < thisLen - 1)
            borrowed = tmp.getDigit(from + 1);
         for (int k2 = otherLen - 1; k2 >= 0 && found; k2--)
         {
            int k1 = otherLen - 1 - k2;
            char dig = tmp.getDigit(from - k1);
            char subt = curDigit * other.getDigit(k2);
            borrowed -= subt / 10;
            if (borrowed < 0 || dig + borrowed * 10 - (subt % 10) < 0)
               found = false;
            borrowed = dig + borrowed * 10 - (subt % 10);
            if (borrowed > 99)
               // subtracting from our dividend at current position
               // we got mass left in higer digits, than where we are currently
               // so we can subtract
               break;
         }
      }
      // set digit in answer
      num.setDigit(pos--, curDigit);
      if (curDigit > 0)
      {
         // subtract from dividend
         char borrowed = 0;
         char transfered = 0;
         for (int k2 = 0; k2 < otherLen && found; k2++)
         {
            int k1 = otherLen - 1 - k2;
            char dig = tmp.getDigit(from - k1) + borrowed;
            char subt = curDigit * other.getDigit(k2) + transfered;
            transfered = subt / 10;
            subt %= 10;
            if (dig < 0)
               tmp.setDigit(from - k1, 9 - subt);
            else if (dig >= subt)
            {
               borrowed = 0;
               tmp.setDigit(from - k1, dig - subt);
            }
            else
            {
               borrowed = -1;
               tmp.setDigit(from - k1, 10 + dig - subt);
            }
         }
      }
   }
   if (pos < 0)
      return LongNumber(num);
   // need to resize (remove first zero digit)
   NumberContainer resized(false, numLen - pos - 1);
   resized.setSign(needSign);
   for (int i = pos + 1; i < numLen; i++)
      resized.setDigit(i - pos - 1, num.getDigit(i));
   return LongNumber(resized);
}

bool LongNumber::operator==(const LongNumber& other) const
{
   if (container.isStatic() && other.container.isStatic())
   {
      // both numbers are inside long long type
      return (sign() == other.sign()
         && container.getValue() == other.container.getValue());
   }
   if (length() != other.length())
      return false;
   for(int i = length() - 1; i >= 0; i--)
   {
      if (getDigit(i) != other.getDigit(i))
         return false;
   }
   return true;
}

bool LongNumber::operator!=(const LongNumber& other) const
{
   return !(*this == other);
}

void LongNumber::toString(char* str) const
{
   int len = length();
   str += len;
   if (container.hasSign())
      str++;
   *str = '\0';
   str--;
   for (int i = 0; i < len; i++)
   {
      *str = getDigit(i) + '0';
      str--;
   }
   if (container.hasSign())
      *str = '-';
}


/********************************************************************************
* LongNumber
* private methods
*********************************************************************************/
LongNumber::LongNumber(NumberContainer num) : container(0)
{
   container = NumberContainer(num);
}

bool LongNumber::checkAdd(long long num1, long long num2, long long & result)
{
   result = num1 + num2;   
   if ((num1 >= 0 && num2 >= 0) && (result < 0 || result > NumberContainer::MAX_STATIC_VALUE))
      return false;
   if ((num1 < 0 && num2 < 0) && (result > 0 || result < -NumberContainer::MAX_STATIC_VALUE))
      return false;
   return true;
}

bool LongNumber::checkMult(long long num1, long long num2, long long & result)
{
   result = 0;
   int sign = (num1 > 0 && num2 < 0) || (num1 < 0 && num2 > 0) ?
      -1 : 1;
   if (num1 < 0) num1 = -num1;
   if (num2 < 0) num2 = -num2;
   long long tens = 1;
   while (num2 > 0)
   {
      char digit = num2 % 10;
      long long prod = num1 * digit;
      if (prod < 0 || prod > NumberContainer::MAX_STATIC_VALUE)
         return false;
      result += prod * tens;
      if (result < 0 || result > NumberContainer::MAX_STATIC_VALUE)
         return false;
      num2 /= 10;
      tens *= 10;
   }
   result *= sign;
   return true;
}

int LongNumber::compareAbs(const NumberContainer& cont1, const NumberContainer& cont2)
{
   if (cont1.isStatic() && cont2.isStatic())
   {
      long long absVal1 = cont1.getValue() * (cont1.getValue() < 0 ? -1 : 1);
      long long absVal2 = cont2.getValue() * (cont2.getValue() < 0 ? -1 : 1);
      if (absVal1 == absVal2)
         return 0;
      if (absVal1 < absVal2)
         return -1;
      return 1;
   }
   int len1 = cont1.length();
   int len2 = cont2.length();
   if (len1 - len2 != 0)
      return len1 - len2;
   for (int i = len1 - 1; i >= 0; i--)
   {
      char dig1 = cont1.getDigit(i);
      char dig2 = cont2.getDigit(i);
      if (dig1 == dig2)
         continue;
      return dig1 - dig2;
   }
   return 0;
}

NumberContainer LongNumber::add(const NumberContainer& left, const NumberContainer& right)
{
   int leftLen = left.length();
   int rightLen = right.length();
   int len = leftLen > rightLen ? leftLen : rightLen;
   char transfered = 0;
   for (int i = 0; i < len; i++)
   {
      char leftDigit = i < leftLen ? left.getDigit(i) : 0;
      char rightDigit = i < rightLen ? right.getDigit(i) : 0;
      transfered = (char)((leftDigit + rightDigit + transfered) / 10);
   }
   if (transfered > 0)
      len++;
   NumberContainer result = NumberContainer(false, len);
   transfered = 0;
   for (int i = 0; i < len; i++)
   {
      char leftDigit = i < leftLen ? left.getDigit(i) : 0;
      char rightDigit = i < rightLen ? right.getDigit(i) : 0;
      char sum = (char)(leftDigit + rightDigit + transfered);
      char digit = sum % 10;
      result.setDigit(i, digit);
      transfered = sum / 10;
   }
   if (transfered == 1)
      result.setDigit(len, transfered);
   result.setSign(left.hasSign());
   return result;
}

NumberContainer LongNumber::sub(const NumberContainer& left, const NumberContainer& right)
{
   int leftLen = left.length();
   int rightLen = right.length();
   int len = leftLen > rightLen ? leftLen : rightLen;

   long long val = 0, tens = 1;
   bool isStatic = true;
   char borrowed = 0;
   int nonZeroDigit = 0;// last non zero digit (length of result)

   for (int i = 0; i < len; i++)
   {
      char leftDigit = i < leftLen ? left.getDigit(i) : 0;
      char rightDigit = i < rightLen ? right.getDigit(i) : 0;
      char sum = (char)(leftDigit - rightDigit + borrowed);
      char digit = (sum + 10) % 10;
      nonZeroDigit = digit > 0 ? i : nonZeroDigit;
      if (isStatic)
      {
         long long dt = 0;
         if (!checkMult(tens, digit, dt))
            isStatic = false;
         else if (!checkAdd(val, dt, val))
            isStatic = false;
         // if this exceeds, next iteration tens * digit will exceed
         else if (i < len - 1 && !checkMult(tens, 10, tens))
            isStatic = false;
      }
      borrowed = (sum < 0 ? -1 : 0);
   }

   NumberContainer result(0);
   if (isStatic)
   {
      result = NumberContainer(true, 0);
      result.setSign(left.hasSign());
      result.setValue(val);
   }
   else
   {
      result = NumberContainer(false, nonZeroDigit + 1);
      borrowed = 0;
      for (int i = 0; i < len; i++)
      {
         char leftDigit = i < leftLen ? left.getDigit(i) : 0;
         char rightDigit = i < rightLen ? right.getDigit(i) : 0;
         char sum = (char)(leftDigit - rightDigit + borrowed);
         char digit = (sum + 10) % 10;
         result.setDigit(i, digit);
         borrowed = (sum < 0 ? -1 : 0);
      }
      result.setSign(left.hasSign());
   }
   return result;
}

