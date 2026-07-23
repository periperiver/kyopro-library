#pragma once
#include<numeric>
#include "primality_test_constexpr.hpp"
constexpr int factorize_constexpr(unsigned long long n,unsigned long long*a){
  if(n<=1)return 0;
  int ptr=0;
  while(n%2==0){
    a[ptr++]=2;
    n/=2;
  }
  while(n>1){
    if(isprime_constexpr(n)){
      a[ptr++]=n;
      break;
    }
    unsigned long long pf=n;
    unsigned long long x=0,y=0;
    int c=1;
    do{
      x=((__uint128_t)x*x+c)%pf;
      y=((__uint128_t)y*y+c)%pf;
      y=((__uint128_t)y*y+c)%pf;
      unsigned long long g=std::gcd(x>y?x-y:y-x,pf);
      if(g==pf){
        c++;
        x=y=0;
        continue;
      }
      if(g>1)pf=g;
    }while(!isprime_constexpr(pf));
    while(n%pf==0){
      a[ptr++]=pf;
      n/=pf;
    }
  }
  return ptr;
}