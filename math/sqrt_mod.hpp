#pragma once
#include "math/pow_mod.hpp"
int sqrt_mod(int n,int mod){
  if(n<=1)return n;
  if(pow_mod(n,(mod-1)/2,mod)!=1)return -1;
  if(mod%8==1){
    int b=2;
    while(pow_mod(b,(mod-1)/2,mod)==1)b++;
    int m2=mod-1,e=0;
    while(m2%2==0)m2>>=1,e++;
    int x=pow_mod(n,(m2-1)/2,mod);
    int y=((long long)(n)*x%mod)*x%mod;
    x=(long long)(x)*n%mod;
    int z=pow_mod(b,m2,mod);
    while(y!=1){
      int j=0;
      int t=y;
      while(t!=1)t=(long long)(t)*t%mod,j++;
      z=pow_mod(z,1<<(e-j-1),mod);
      x=(long long)(x)*z%mod;
      z=(long long)(z)*z%mod;
      y=(long long)(y)*z%mod;
      e=j;
    }
    return x;
  }
  else if(mod%8==5){
    int ret=pow_mod(n,(mod+3)/8,mod);
    if((long long)(ret)*ret%mod==n)return ret;
    else{
      ret=((long long)(ret)*pow_mod(2,(mod-1)/4,mod))%mod;
      return ret;
    }
  }
  else return pow_mod(n,(mod+1)/4,mod);
}