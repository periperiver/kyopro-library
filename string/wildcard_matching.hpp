#pragma once
#include<type_traits>
#include<string>
#include "convolution/ntt.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
template<typename T>
std::vector<bool>wildcard_matching(const T&s,const T&t,const typename T::value_type w){
  static_assert(std::is_integral_v<typename T::value_type>);
  assert(s.size()>=t.size());
  using mint=mint998;
  int n=ceil_pow2((int)s.size());
  std::vector<mint>x(n),y(n),z(n);
  for(int i=0;i<t.size();i++)if(t[i]!=w){
    mint r=Random::get<long long>();
    y[-i&(n-1)]=r;
    z[-i&(n-1)]=r*mint::raw(t[i]);
  }
  for(int i=0;i<s.size();i++){
    if(s[i]!=w)x[i]=mint::raw(s[i]);
  }
  dft(x),dft(y),dft(z);
  for(int i=0;i<n;i++)x[i]*=y[i];
  for(int i=0;i<s.size();i++)y[i]=s[i]!=w;
  dft(y);
  for(int i=0;i<n;i++)x[i]-=y[i]*z[i];
  idft(x);
  std::vector<bool>res(s.size()-t.size()+1);
  for(int i=0;i<res.size();i++)res[i]=!x[i].val();
  return res;
}