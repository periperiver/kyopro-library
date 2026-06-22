#pragma once
#include<cmath>
constexpr unsigned long long kth_root(unsigned long long a,unsigned long long k){
  if(a<=1||k==1)return a;
  if(k>=64)return 1;
  if(k==2)return sqrtl(a);
  if(a==-1)a--;
  unsigned long long res=(k==3?cbrt(a):pow(a,(long double)1/k))-1;
  while(true){
    unsigned long long p=k,prod=1,b=res+1;
    while(p>0){
      if(p&1){
        if(b==-1){
          prod=-1ull;
          break;
        }
        if(prod<=(-1ull)/b)prod*=b;
        else{
          prod=-1ull;
          break;
        }
      }
      if(b<(1ull<<32))b*=b;
      else b=-1ull;
      p>>=1;
    }
    if(prod>a)break;
    else res++;
  }
  return res;
}