#pragma once
#include "ntt.hpp"
template<typename T>
void ntt_doubling(std::vector<T>&a){
  static constexpr ntt_root<T::mod()>r;
  int n=a.size()/2;
  std::vector<T>b(a.begin(),a.begin()+n);
  idft(b);
  T now=T::raw(n).inv(),zeta=T::raw(r.root[msb(n)+1]);
  for(int i=0;i<n;i++){
    b[i]*=now;
    now*=zeta;
  }
  dft(b);
  std::copy(b.begin(),b.end(),a.begin()+n);
}