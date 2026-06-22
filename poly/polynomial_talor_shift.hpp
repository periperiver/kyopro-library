#pragma once
#include "convolution/ntt.hpp"
#include "math/factorial.hpp"
template<typename T>
[[nodiscard]]std::vector<T>polynomial_talor_shift(const std::vector<T>&a,T c,int deg=-1){
  if(deg==-1)deg=a.size();
  std::vector<T>f(deg),g(deg);
  T now=1;
  int s=std::min(deg,(int)a.size());
  for(int i=0;i<s;i++){
    f[i]=a[i]*F<T>::factorial(i);
    g[deg-i-1]=now*F<T>::factorial_inv(i);
    now*=c;
  }
  f=ntt_convolution(f,g);
  std::vector<T>res(deg);
  for(int i=0;i<deg;i++)res[i]=f[i+deg-1]*F<T>::factorial_inv(i);
  return res;
}