#pragma once
#include "factorize_constexpr.hpp"
#include "pow_mod.hpp"
constexpr unsigned long long primitive_root_constexpr(unsigned long long x){
  if(x==167772161)return 3;
  if(x==469762049)return 3;
  if(x==754974721)return 11;
  if(x==880803841)return 26;
  if(x==998244353)return 3;
  if(x==2)return 1;
  unsigned long long a[64]={};
  int ptr=factorize_constexpr(x-1,a);
  for(int g=2;;g++){
    bool ok=true;
    for(int i=0;i<ptr;i++){
      if(i>0&&a[i-1]==a[i])continue;
      if(pow_mod<unsigned long long>(g,(x-1)/a[i],x)==1){
        ok=false;
        break;
      }
    }
    if(ok)return g;
  }
}