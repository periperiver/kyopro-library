#pragma once
#include "matrix/monotone_minima.hpp"
//aが凸
template<typename T,bool MIN=true>
std::vector<T>min_plus_convolution(const std::vector<T>&a,const std::vector<T>&b){
  int n=a.size(),m=b.size();
  auto f=[&](int i,int j,int k)->bool {
    if(i<k)return false;
    if(i-j>=n)return true;
    if constexpr(MIN)return a[i-j]+b[j]>a[i-k]+b[k];
    else return a[i-j]+b[j]<a[i-k]+b[k];
  };
  std::vector<int>argmin=monotone_minima(n+m-1,m,f);
  std::vector<T>res(n+m-1);
  for(int i=0;i<n+m-1;i++)res[i]=a[i-argmin[i]]+b[argmin[i]];
  return res;
}