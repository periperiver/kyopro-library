#pragma once
#include "util.hpp"
constexpr unsigned long long binary_gcd(unsigned long long a,unsigned long long b){
  if(a==0||b==0||a==b)return a<b?b:a;
  int n=lsb(a),m=lsb(b);
  while(a!=b){
    if(a>b)a=(a-b)>>lsb(a-b);
    else b=(b-a)>>lsb(b-a);
  }
  return a<<(n<m?n:m);
}