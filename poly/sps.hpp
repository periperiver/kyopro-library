#pragma once
#include "poly_sps_composition.hpp"
#include "math/factorial.hpp"
template<typename T>
std::vector<T>sps_exp(const std::vector<T>&f){
  assert(f[0]==T(0));
  int n=msb(f.size());
  std::vector<T>c(n+1);
  for(int i=0;i<=n;i++)c[i]=F<T>::factorial_inv(i);
  return poly_sps_composition(c,f);
}
template<typename T>
std::vector<T>sps_log(std::vector<T>f){
  int n=msb(f.size());
  assert(f[0]==T(1));
  assert((1<<n)==(int)f.size());
  std::vector<T>c(n+1);
  for(int i=1;i<=n;i++)c[i]=-F<T>::inverse(i);
  f[0]=T(1)-f[0];
  for(int i=1;i<(1<<n);i++)f[i]=-f[i];
  return poly_sps_composition(c,std::move(f));
}