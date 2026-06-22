#pragma once
#include "pow_mod.hpp"
template<int p1,int p2>
unsigned long long crt2(int a1,int a2){
  static_assert(p1<p2);
  static constexpr unsigned long long x=pow_mod<int>(p1,p2-2,p2);
  unsigned long long c=(a2-a1+p2)*x%p2;
  return a1+c*p1;
}