#pragma once
#include<cassert>
#include "../convolution/ntt.hpp"
template<typename T>
std::vector<T> fps_inv(const std::vector<T> &a,int deg=-1){
  int n=a.size();
  if(deg==-1)deg=n;
  const T zero=T::raw(0);
  assert(a[0]!=zero);
  std::vector<T> ret(ceil_pow2(deg));
  ret[0]=a[0].inv();
  for(int m=1;m<deg;m<<=1){
    std::vector<T> f(a.begin(),a.begin()+std::min(n,m*2));
    if(f.size()<m*2)f.resize(m*2,0);
    std::vector<T> g(ret);
    f.resize(m*2);
    dft(f);
    g.resize(m*2);
    dft(g);
    for(int i=0;i<m*2;i++)f[i]*=g[i];
    idft(f);
    T inv=T::raw(m*2).inv();
    for(int i=0;i<m;i++)f[i]=zero;
    for(int i=m;i<m*2;i++)f[i]*=inv;
    dft(f);
    for(int i=0;i<m*2;i++)f[i]*=g[i];
    idft(f);
    for(int i=0;i<m*2;i++)f[i]*=inv;
    for(int i=m;i<m*2;i++)ret[i]-=f[i];
  }
  ret.resize(deg);
  return ret;
}