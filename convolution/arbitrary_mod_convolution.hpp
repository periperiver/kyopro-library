#pragma once
#include "math/crt3.hpp"
#include "ntt.hpp"
#include "math/modint.hpp"
template<typename T>
std::vector<T>arbitrary_mod_convolution(const std::vector<T>&a,const std::vector<T>&b){
  if(a.empty()||b.empty())return std::vector<T>{};
  if(std::min(a.size(),b.size())<60){
    std::vector<T>ret(a.size()+b.size()-1,0);
    for(int i=0;i<a.size();i++)for(int j=0;j<b.size();j++)ret[i+j]+=a[i]*b[j];
    return ret;
  }
  using mint1=mod_int<167772161>;
  using mint2=mod_int<469762049>;
  using mint3=mod_int<998244353>;
  std::vector<mint1>a1(a.size()),b1(b.size());
  std::vector<mint2>a2(a.size()),b2(b.size());
  std::vector<mint3>a3(a.size()),b3(b.size());
  for(int i=0;i<a.size();i++){
    a1[i]=a[i].val();
    a2[i]=a[i].val();
    a3[i]=a[i].val();
  }
  for(int i=0;i<b.size();i++){
    b1[i]=b[i].val();
    b2[i]=b[i].val();
    b3[i]=b[i].val();
  }
  a1=ntt_convolution(a1,b1),a2=ntt_convolution(a2,b2),a3=ntt_convolution(a3,b3);
  std::vector<T>ret(a.size()+b.size()-1);
  for(int i=0;i<ret.size();i++)ret[i]=crt3<T,mint1::mod(),mint2::mod(),mint3::mod()>(a1[i].val(),a2[i].val(),a3[i].val());
  return ret;
}