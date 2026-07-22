#pragma once
#include<initializer_list>
constexpr bool isprime_constexpr(unsigned long long n){
  if(n<64)return 2891462833508853932ll>>n&1;
  if(n%2==0)return false;
  unsigned long long d=n-1;
  int s=0;
  while(!(d&1))d>>=1,s++;
  int q=63;
  while(!(d>>q))q--;
  unsigned long long r=n;
  for(int i=0;i<5;i++)r*=2-r*n;
  auto redc=[&r,&n](__uint128_t x)->unsigned long long {
    x=(x+__uint128_t((unsigned long long)x*-r)*n)>>64;
    return x>=n?x-n:x;
  };
  __uint128_t r2=-__uint128_t(n)%n;
  unsigned long long one=redc(r2);
  for(unsigned long long base:{2,325,9375,28178,450775,9780504,1795265022}){
    if(base%n==0)continue;
    unsigned long long a=base=redc((base%n)*r2);
    for(int i=q-1;i>=0;i--){
      a=redc(__uint128_t(a)*a);
      if(d>>i&1)a=redc(__uint128_t(a)*base);
    }
    if(a==one)continue;
    for(int i=1;a!=n-one;i++){
      if(i>=s)return false;
      a=redc(__uint128_t(a)*a);
    }
  }
  return true;
}