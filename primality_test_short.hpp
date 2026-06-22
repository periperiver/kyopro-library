#pragma once
#include<initializer_list>
bool isprime(unsigned long long n){
  using u64=unsigned long long;
  if(n<=1)return false;
  if(n%2==0)return n==2;
  u64 d=n-1;
  int s=0;
  while(!(d&1))d>>=1,s++;
  int q=63;
  while(!(d>>q))q--;
  u64 r=n;
  for(int i=0;i<5;i++)r*=2-r*n;
  auto redc=[&r,&n](__uint128_t x)->u64 {
    x=(x+__uint128_t(u64(x)*-r)*n)>>64;
    return x>=n?x-n:x;
  };
  __uint128_t r2=-__uint128_t(n)%n;
  u64 one=redc(r2);
  for(u64 base:{2,325,9375,28178,450775,9780504,1795265022}){
    if(base%n==0)continue;
    u64 a=base=redc((base%n)*r2);
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