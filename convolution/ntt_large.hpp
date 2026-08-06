#pragma once
#include<cassert>
#include "ntt.hpp"
template<typename T>
std::vector<T>ntt_convolution_large(const std::vector<T>&a,const std::vector<T>&b){
  int n=a.size(),m=b.size();
  int s=n+m-1;
  if(s<=1<<lsb(T::mod()-1))return ntt_convolution(a,b);
  static constexpr int mx=lsb(T::mod()-1);
  static constexpr int c=1<<(mx-1);
  std::vector<std::vector<T>>f((s+c-1)/c,std::vector<T>(1<<mx)),g((s+c-1)/c,std::vector<T>(1<<mx));
  for(int i=0;i<n;i++)f[i>>(mx-1)][i&(c-1)]=a[i];
  for(int i=0;i<m;i++)g[i>>(mx-1)][i&(c-1)]=b[i];
  for(int i=0;i<f.size();i++)dft(f[i]),dft(g[i]);
  T inv=T::raw(1<<mx).inv();
  std::vector<T>res(s);
  for(int i=0;i<2*f.size()-1;i++)if((i<<(mx-1))<s){
    std::vector<T>h(1<<mx);
    for(int j=0;j<f.size();j++)for(int k=0;k<g.size();k++)if(j+k==i){
      for(int l=0;l<(1<<mx);l++)h[l]+=f[j][l]*g[k][l];
    }
    idft(h);
    for(int j=0;j<(1<<mx);j++)if((i<<(mx-1))+j<s)res[(i<<(mx-1))+j]+=h[j]*inv;
  }
  return res;
}