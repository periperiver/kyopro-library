#pragma once
#include "convolution/ntt.hpp"
#include "factorial.hpp"
#include "enumerate_fully_multiplicative_function.hpp"
template<typename T>
std::vector<T>stirling2_fixed_n(int n){
  int s=ceil_pow2(n*2+1);
  std::vector<T>f=enumerate_fully_multiplicative_function(n,[&n](int x)->T {return T::raw(x).pow(n);});
  f.resize(s);
  std::vector<T>g(s);
  for(int i=0;i<=n;i++){
    T finv=F<T>::factorial_inv(i);
    f[i]*=finv;
    g[i]=(i&1)?-finv:finv;
  }
  T inv=T::raw(s).inv();
  dft(f),dft(g);
  for(int i=0;i<s;i++)f[i]*=g[i];
  idft(f);
  f.resize(n+1);
  for(int i=0;i<=n;i++)f[i]*=inv;
  return f;
}