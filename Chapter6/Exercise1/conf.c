#include <stdio.h>

asm(".text\n\t"
    ".byte 0x48, 0xb8");

int main(int argc, char** argv)
{
   puts("HELLO WORLD!");
   return 0;
}

