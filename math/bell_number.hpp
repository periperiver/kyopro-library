#pragma once
#include "poly/fps.hpp"
#include "factorial.hpp"
template<typename T>
std::vector<T>bell_number(int n){
  std::vector<T>f(n+1);
  F<T>::resize(n);
  for(int i=1;i<=n;i++)f[i]=F<T>::factorial_inv(i);
  f=fps_exp(f);
  for(int i=2;i<=n;i++)f[i]*=F<T>::factorial(i);
  return f;
}