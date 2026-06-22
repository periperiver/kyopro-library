#pragma once
#include "../poly/polynomial_talor_shift.hpp"
template<typename T>
std::vector<T>stirling1_fixed_n(int n){
  if(n==0)return std::vector<T>{1};
  std::vector<T>f{0,1};
  int log2n=msb(n);
  for(int i=log2n-1;i>=0;i--){
    int nn=n>>i;
    std::vector<T>g=polynomial_talor_shift<T>(f,-(nn/2));
    f=ntt_convolution(f,g);
    if(nn&1){
      std::vector<T>f2(f.size()+1);
      T c=T(1-nn);
      for(int i=1;i<(int)f.size();i++)f2[i]=f[i]*c+f[i-1];
      f2[f.size()]=f.back();
      f=std::move(f2);
    }
  }
  return f;
}