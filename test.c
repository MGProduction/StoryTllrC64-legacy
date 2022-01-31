typedef unsigned char u8;
typedef unsigned short u16;

#include "font.h"
#define IMPLEMENT_C_HBUNPACK
#include "mgLib/hupack.c"



u8 buf[4096];

int main()
{
 hunpack(font,(u8*)4000);
}