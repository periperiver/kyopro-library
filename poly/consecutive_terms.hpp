#pragma once
#include "bostan_mori_msb.hpp"
#include "fps.hpp"
template<typename T,std::enable_if_t<is_static_modint_v<T>,std::nullptr_t> =nullptr>
std::vector<T>consecutive_terms(std::vector<T>p,std::vector<T>q,long long k,int m){
  int d=q.size();
  std::vector<T>w=fps_inv(q);
  auto next=[&](std::vector<T>u)->std::vector<T> {
    std::vector<T>s=ntt_convolution(u,q);
    s.erase(s.begin(),s.begin()+d);
    s.resize(d);
    for(int i=0;i<d;i++)s[i]=-s[i];
    s=ntt_convolution(s,w);
    s.resize(d);
    return s;
  };
  std::vector<T>u=bostan_mori_msb(q,k);
  u.resize(d*2);
  std::vector<T>u2=next(u);
  for(int i=0;i<d;i++)u[i+d]+=u2[i];
  std::vector<T>res=ntt_convolution(u,p);
  res.erase(res.begin(),res.begin()+d-1);
  res.resize(d);
  std::vector<T>a=res;
  while(res.size()<m){
    a=next(a);
    res.insert(res.end(),a.begin(),a.end());
  }
  res.resize(m);
  return res;
}