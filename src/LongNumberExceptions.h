#ifndef LONGNUMBEREXCEPTIONS
#define LONGNUMBEREXCEPTIONS

#include <exception>

class LongNumberException : public std::exception
{
public:
   LongNumberException(const char* message) : std::exception(message) {}
};

class FormatException : public LongNumberException
{
public:
   FormatException(const char* message) : LongNumberException(message) {}
};

class ArithmeticException : public LongNumberException
{
public:
   ArithmeticException(const char* message) : LongNumberException(message) {}
};

#endif