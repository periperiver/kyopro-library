#pragma once
#include "convolution/ntt.hpp"
#include "other/type.hpp"
//0≤i<q.size()に対する [x^(k-i)]1/qの列挙
template<typename T,std::enable_if_t<is_static_modint_v<T>,std::nullptr_t> =nullptr>
std::vector<T>bostan_mori_msb(std::vector<T>q,long long k){
  int d=q.size();
  if(k==0){
    std::vector<T>res(d);
    res[d-1]=q[0].inv();
    return res;
  }
  int z=ceil_pow2(d*2);
  q.resize(z);
  dft(q);
  std::vector<T>v(z/2);
  for(int i=0;i<(z/2);i++)v[i]=q[i*2]*q[i*2+1]*T::raw(2);
  idft(v);
  T inv=T(z).inv();
  for(int i=0;i<(int)v.size();i++)v[i]*=inv;
  v.resize(d);
  std::vector<T>w=bostan_mori_msb(v,k/2);
  std::vector<T>s(z);
  if(k%2==0)for(int i=0;i<d;i++)s[i*2+1]=w[i];
  else for(int i=0;i<d;i++)s[i*2]=w[i];
  dft(s);
  for(int i=0;i<z;i+=2){
    s[i]*=q[i+1];
    s[i+1]*=q[i];
  }
  idft(s);
  s.erase(s.begin(),s.begin()+d);
  s.resize(d);
  for(int i=0;i<d;i++)s[i]*=inv;
  return s;
}