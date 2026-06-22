#pragma once
#include<cassert>
#include "poly/fps.hpp"
#include "factorial.hpp"
template<typename T>
std::vector<T>stirling2_fixed_k(int n,int k){
  assert(n>=k);
  std::vector<T>res(n-k+1);
  for(int i=0;i<=n-k;i++)res[i]=F<T>::factorial_inv(i+1);
  res=fps_pow(res,k);
  T kf_inv=F<T>::factorial_inv(k);
  for(int i=0;i<=n-k;i++)res[i]*=kf_inv*F<T>::factorial(i+k);
  return res;
}