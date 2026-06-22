#pragma once
#include<vector>
#include<cassert>
#include "math/primitive_root_constexpr.hpp"
#include "ntt.hpp"
template<typename T>
std::vector<T>multiplicative_convolution_modp(const std::vector<T>&a,const std::vector<T>&b){
  assert(a.size()==b.size());
  int p=a.size();
  int g=primitive_root_constexpr(p);
  std::vector<int>pow_g(p-1);
  pow_g[0]=1;
  for(int i=1;i<p-1;i++)pow_g[i]=(long long)pow_g[i-1]*g%p;
  std::vector<T>a2(p-1),b2(p-1);
  for(int i=0;i<p-1;i++)a2[i]=a[pow_g[i]],b2[i]=b[pow_g[i]];
  std::vector<T>c2=ntt_convolution(a2,b2);
  for(int i=p-1;i<p*2-3;i++)c2[i-(p-1)]+=c2[i];
  std::vector<T>res(p);
  T sum_a=0,sum_b=0;
  for(int i=0;i<p;i++)sum_a+=a[i],sum_b+=b[i];
  res[0]=sum_a*b[0]+a[0]*sum_b-a[0]*b[0];
  for(int i=0;i<p-1;i++)res[pow_g[i]]=c2[i];
  return res;
}