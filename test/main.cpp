
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include "LongNumber.h"
#include "static.h"
#include "dynamic.h"


int main(int argc, char** argv)
{
   printf("LongNumber size: %d bytes\n", sizeof(LongNumber));

   ::testing::InitGoogleTest(&argc, argv);
   int res = RUN_ALL_TESTS();
   getchar();
   return res;
}

