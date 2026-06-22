#pragma once
#include<cassert>
#include "convolution/ntt_root.hpp"
#include "convolution/ntt.hpp"
#include "convolution/ntt_doubling.hpp"
#include "fps_inv.hpp"
#include "math/util.hpp"
template<typename T>
T bostan_mori(std::vector<T> p,std::vector<T> q,long long k){
  static constexpr ntt_root<T::mod()>r;
  int n=ceil_pow2((int)std::max(p.size()+1,q.size())*2-1);
  p.resize(n,T::raw(0));
  q.resize(n,T::raw(0));
  dft(p),dft(q);
  T inv2=T::raw(2).inv();
  int n2=n/2;
  while(k>=n){
    for(int i=0;i<n;i++)p[i]*=q[i^1];
    if(k&1){
      T prod=inv2;
      for(int i=0;i<n2;i++){
        p[i]=(p[i*2]-p[i*2+1])*prod;
        prod*=T::raw(r.invrate2[lsb(~i)]);
      }
    }
    else{
      for(int i=0;i<n2;i++){
        p[i]=(p[i*2]+p[i*2+1])*inv2;
      }
    }
    for(int i=0;i<n2;i++)q[i]=q[i*2]*q[i*2+1];
    ntt_doubling(p),ntt_doubling(q);
    k>>=1;
  }
  idft(p),idft(q);
  q=fps_inv(q,k+1);
  T res=T();
  for(int i=0;i<=k;i++)res+=p[i]*q[k-i];
  return res;
}