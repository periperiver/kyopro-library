#pragma once
unsigned long long bit_reverse(unsigned long long a){
  a=((a&0x5555555555555555)<<1)|((a>>1)&0x5555555555555555);
  a=((a&0x3333333333333333)<<2)|((a>>2)&0x3333333333333333);
  a=((a&0x0f0f0f0f0f0f0f0f)<<4)|((a>>4)&0x0f0f0f0f0f0f0f0f);
  return __builtin_bswap64(a);
}