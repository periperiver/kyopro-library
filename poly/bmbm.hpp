#pragma once
#include "bostan_mori.hpp"
#include "find_linear_recurrence.hpp"
#include "../convolution/arbitrary_mod_convolution.hpp"
template<typename T>
T kth_term(std::vector<T>p,long long k){
  std::vector<T>q=find_linear_recurrence(p);
  if constexpr(is_static_modint_v<T>){
    if((T::mod()-1)%(ceil_pow2(q.size()*2-1))==0){
      p=ntt_convolution(p,q);
      p.resize(q.size()-1);
      return bostan_mori(p,q,k);
    }
  }
  p=arbitrary_mod_convolution(p,q);
  int n=q.size();
  p.resize(n-1);
  std::vector<T>mq(n);
  while(k){
    for(int i=0;i<n;i++)mq[i]=i&1?-q[i]:q[i];
    p=arbitrary_mod_convolution(p,mq);
    q=arbitrary_mod_convolution(q,mq);
    std::vector<T>u(n-1),v(n);
    if(k&1)for(int i=0;i<n-1;i++)u[i]=p[i*2+1];
    else for(int i=0;i<n-1;i++)u[i]=p[i*2];
    for(int i=0;i<n;i++)v[i]=q[i*2];
    p=std::move(u),q=std::move(v);
    k>>=1;
  }
  if(p.empty())return 0;
  else return p[0];
}