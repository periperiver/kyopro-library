#pragma once
#include "template.hpp"
#include "../convolution/ntt.hpp"
#include "../convolution/arbitrary_mod_convolution.hpp"
#include "bostan_mori.hpp"
#include "berlekamp_massey.hpp"
template<typename T>
T kth_term(vector<T>p,ll k){
  vector<T>q=berlekamp_massey(p);
  if constexpr(T::mod()==998244353){
    p=ntt_convolution(p,q);
    p.resize(q.size()-1);
    return bostan_mori(p,q,k);
  }
  p=arbitrary_mod_convolution(p,q);
  int n=q.size();
  p.resize(n-1);
  vector<T>mq(n);
  while(k){
    rep(i,n)mq[i]=q[i]*(i&1?-1:1);
    p=arbitrary_mod_convolution(p,mq);
    q=arbitrary_mod_convolution(q,mq);
    vector<T>u(n-1),v(n);
    if(k&1)rep(i,n-1)u[i]=p[i*2+1];
    else rep(i,n-1)u[i]=p[i*2];
    rep(i,n)v[i]=q[i*2];
    swap(p,u),swap(q,v);
    k>>=1;
  }
  if(p.empty())return 0;
  else return p[0];
}