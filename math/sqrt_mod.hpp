#pragma once
#include "template.hpp"
int sqrt_mod(int n,int mod){
  if(n<=1)return n;
  auto powmod=[&](int a,int b)->int {
    ll ret=1;
    while(b){
      if(b&1)ret=(ret*a)%mod;
      a=ll(a)*a%mod;
      b>>=1;
    }
    return ret;
  };
  if(powmod(n,(mod-1)/2)!=1)return -1;
  if(mod%8==1){
    int b=2;
    while(powmod(b,(mod-1)/2)==1)b++;
    int m2=mod-1,e=0;
    while(m2%2==0)m2>>=1,e++;
    int x=powmod(n,(m2-1)/2);
    int y=(ll(n)*x%mod)*x%mod;
    x=ll(x)*n%mod;
    int z=powmod(b,m2);
    while(y!=1){
      int j=0;
      int t=y;
      while(t!=1)t=ll(t)*t%mod,j++;
      z=powmod(z,1<<(e-j-1));
      x=ll(x)*z%mod;
      z=ll(z)*z%mod;
      y=ll(y)*z%mod;
      e=j;
    }
    return x;
  }
  else if(mod%8==5){
    int ret=powmod(n,(mod+3)/8);
    if(ll(ret)*ret%mod==n)return ret;
    else{
      ret=(ll(ret)*powmod(2,(mod-1)/4))%mod;
      return ret;
    }
  }
  else return powmod(n,(mod+1)/4);
}