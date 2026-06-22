#pragma once
#include "pow_mod.hpp"
template<typename T,int p1,int p2,int p3>
T crt3(int a1,int a2,int a3){
  static_assert(p1<p2&&p2<p3);
  static constexpr long long x=pow_mod<int>(p1,p2-2,p2);
  static constexpr long long y=pow_mod<int>((long long)p1*p2%p3,p3-2,p3);
  long long c=(a2-a1+p2)*x%p2;
  long long c2=a1+c*p1;
  c=(a3-c2%p3+p3)*y%p3;
  return T(c2)+T(c)*T(p1)*T(p2);
}