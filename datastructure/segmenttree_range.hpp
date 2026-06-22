#pragma once
#include<concepts>
#include<cassert>
#include "../math/util.hpp"
template<std::integral T>
std::pair<T,T>segmenttree_range(T x,int logn){
  assert(T(1)<=x&&x<(T(1)<<(logn+1)));
  int m=msb(x);
  T len=T(1)<<(logn-m);
  T left=floor_pow2(x);
  return std::make_pair(len*(x-left),len*(x-left+1));
}