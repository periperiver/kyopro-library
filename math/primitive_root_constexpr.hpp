#pragma once
#include "pow_mod.hpp"
constexpr int primitive_root_constexpr(int x){
  if(x==167772161)return 3;
  if(x==469762049)return 3;
  if(x==754974721)return 11;
  if(x==880803841)return 26;
  if(x==998244353)return 3;
  if(x==2)return 1;
  int x2=x;
  int p[20]={};
  int c=0;
  x--;
  for(int i=2;i*i<=x;i++){
    if(x%i==0){
      p[c++]=i;
      while(x%i==0)x/=i;
    }
  }
  if(x!=1)p[c++]=x;
  x=x2;
  for(int g=2;;g++){
    bool ok=true;
    for(int i=0;i<c;i++)if(pow_mod(g,(x-1)/p[i],x)==1){
      ok=false;
      break;
    }
    if(ok)return g;
  }
}