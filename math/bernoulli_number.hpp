#pragma once
#include "../poly/fps_inv.hpp"
#include "factorial.hpp"
template<typename T>
std::vector<T>bernoulli_number(int n){
  std::vector<T>f(n+1);
  for(int i=0;i<=n;i++)f[i]=F<T>::factorial_inv(i+1);
  f=fps_inv(f);
  for(int i=0;i<=n;i++)f[i]*=F<T>::factorial(i);
  return f;
}