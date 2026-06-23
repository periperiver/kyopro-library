#pragma once
#include "pow_mod.hpp"
template<int p1,int p2,int p3,int p4,int p5>
unsigned long long crt5(unsigned long long a1,unsigned long long a2,unsigned long long a3,unsigned long long a4,unsigned long long a5){
  static constexpr unsigned long long x1=pow_mod<int>(p1,p2-2,p2);
  static constexpr unsigned long long x2=pow_mod<int>((unsigned long long)p1*p2%p3,p3-2,p3);
  static constexpr unsigned long long x3=pow_mod<int>((unsigned long long)p1*p2%p4*p3%p4,p4-2,p4);
  static constexpr unsigned long long x4=pow_mod<int>((unsigned long long)p1*p2%p5*p3%p5*p4%p5,p5-2,p5);
  static constexpr unsigned long long p12=(unsigned long long)p1*p2;
  static constexpr unsigned long long p34=(unsigned long long)p3*p4;
  unsigned long long c=(a2-a1+p2)*x1%p2;
  __uint128_t d=a1+c*p1;
  c=(a3-d%p3+p3)*x2%p3;
  d+=c*static_cast<__uint128_t>(p12);
  c=static_cast<unsigned long long>(a4-d%p4+p4)*x3%p4;
  d+=static_cast<__uint128_t>(c*p3)*p12;
  c=static_cast<unsigned long long>(a5-d%p5+p5)*x4%p5;
  return static_cast<unsigned long long>(d)+c*p12*p34;
}