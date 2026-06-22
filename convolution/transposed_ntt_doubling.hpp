#pragma once
#include "transposed_ntt.hpp"
template<typename T>
void transposed_ntt_doubling(std::vector<T>&a){
  static constexpr ntt_root<T::mod()>r;
  int n=a.size()/2;
  std::vector<T>b(a.begin(),a.begin()+n);
  a=std::vector<T>(a.begin()+n,a.end());
  transposed_dft(a);
  T now=T::raw(n).inv(),zeta=T::raw(r.root[msb(n)+1]);
  for(int i=0;i<n;i++)a[i]*=now,now*=zeta;
  transposed_idft(a);
  for(int i=0;i<n;i++)a[i]+=b[i];
}