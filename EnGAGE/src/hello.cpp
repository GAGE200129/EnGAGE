/* hello.cpp */
#include <stdio.h>

#ifdef EN_DEBUG
#pragma message("Hello in debug mode !")
#elif EN_NDEBUG
#pragma message("Hello in release mode !")
#endif

int main(void) {
   puts("Hello, world!");
   return 0;
}