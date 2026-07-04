#pragma once
#include<vector>
#include "ext_gcd.hpp"
#include "util.hpp"
#include "barrett_reduction.hpp"
struct PrimePowerBinomial{
private:
  int p,e,mod;
  std::vector<int>fac,ifac;
  int delta;
  BarrettReduction bm,bp;
  void resize(int n){
    n=ceil_pow2(n);
    int pre=fac.size();
    fac.resize(n+1),ifac.resize(n+1);
    for(int i=pre;i<=n;i++){
      if(bp.rem(i)==0)fac[i]=fac[i-1];
      else fac[i]=bm.rem((long long)fac[i-1]*i);
    }
    ifac[n]=inv_mod(fac[n],mod).second;
    for(int i=n-1;i>=pre;i--){
      if((bp.rem(i+1)==0))ifac[i]=ifac[i+1];
      else ifac[i]=bm.rem((long long)ifac[i+1]*(i+1));
    }
  }
  int lucas(long long n,long long k){
    int res=1;
    while(n){
      int n0=bp.rem(n);
      int k0=bp.rem(k);
      n=bp.quo(n),k=bp.quo(k);
      if(n0<k0)return 0;
      res=bm.rem((long long)res*fac[n0]);
      res=bm.rem((long long)res*ifac[k0]);
      res=bm.rem((long long)res*ifac[n0-k0]);
    }
    return res;
  }
public:
  PrimePowerBinomial(){}
  PrimePowerBinomial(int p_,int e_):p(p_),e(e_),fac{1,1},ifac{1,1},bp(p_){
    mod=1;
    while(e_--)mod*=p;
    delta=(p==2&&e>=3)?1:mod-1;
    bm=BarrettReduction(mod);
  }
  int operator()(long long n,long long k){
    if(n<k||k<0)return 0;
    if(std::min<long long>(n,mod)>=fac.size())resize(std::min<long long>(n,mod));
    if(e==1)return lucas(n,k);
    long long r=n-k;
    int e0=0,eq=0,i=0;
    int res=1;
    while(n){
      res=bm.rem((long long)res*fac[bm.rem(n)]);
      res=bm.rem((long long)res*ifac[bm.rem(k)]);
      res=bm.rem((long long)res*ifac[bm.rem(r)]);
      n=bp.quo(n),k=bp.quo(k),r=bp.quo(r);
      int eps=n-k-r;
      e0+=eps;
      if(e0>=e)return 0;
      if(++i>=e)eq+=eps;
    }
    if(eq&1)res=bm.rem((long long)res*delta);
    res=bm.rem((long long)res*bm.pow(p,e0));
    return res;
  }
};