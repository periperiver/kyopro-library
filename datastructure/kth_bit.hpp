#pragma once
#include<cstdint>
#include<bit>
#include "../math/util.hpp"
namespace kth_bit_impl{
struct Pre{
  std::int8_t table[1<<16][16];
  Pre(){
    for(int i=0;i<(1<<16);i++){
      for(int j=0,x=i;x&&j<16;j++){
        int l=lsb(x);
        table[i][j]=l;
        x^=1<<l;
      }
    }
  }
}pre;
constexpr unsigned long long mask1=(1ull<<16)-1;
template<bool check_over=true>
int kth_bit(unsigned long long x,int k){
  if constexpr(check_over){
    if(std::popcount(x)<=k)return -1;
  }
  int c=std::popcount<unsigned int>(x&-1u);
  if(k<c){
    unsigned int y=x;
    int d=std::popcount(y&mask1);
    if(k<d)return pre.table[y&mask1][k];
    else return pre.table[y>>16][k-d]+16;
  }
  else{
    k-=c;
    unsigned int y=x>>32;
    int d=std::popcount(y&mask1);
    if(k<d)return pre.table[y&mask1][k]+32;
    else return pre.table[y>>16][k-d]+48;
  }
}
}
using kth_bit_impl::kth_bit;