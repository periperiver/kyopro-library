#pragma once
#include<vector>
#include "../math/util.hpp"
namespace karatsuba_impl{
template<typename T>
std::vector<T>karatsuba_rec(const std::vector<T>&a,const std::vector<T>&b){
  int n=a.size();
  std::vector<T>res(n*2-1);
  if(n<=32){
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)res[i+j]+=a[i]*b[j];
    return res;
  }
  std::vector<T>z2=karatsuba_rec(std::vector<T>(a.begin()+n/2,a.end()),std::vector<T>(b.begin()+n/2,b.end()));
  std::vector<T>z0=karatsuba_rec(std::vector<T>(a.begin(),a.begin()+n/2),std::vector<T>(b.begin(),b.begin()+n/2));
  std::vector<T>a1(n/2),b1(n/2);
  for(int i=0;i<n/2;i++)a1[i]=a[i+n/2]-a[i],b1[i]=b[i+n/2]-b[i];
  std::vector<T>z1=karatsuba_rec(a1,b1);
  for(int i=0;i<n-1;i++){
    res[i]+=z0[i];
    res[i+n]+=z2[i];
    res[i+n/2]+=z0[i]+z2[i]-z1[i];
  }
  return res;
}
template<typename T>
std::vector<T>karatsuba(std::vector<T>a,std::vector<T>b){
  int s=a.size()+b.size()-1;
  int n=ceil_pow2(std::max(a.size(),b.size()));
  a.resize(n),b.resize(n);
  std::vector<T>res=karatsuba_rec(a,b);
  res.resize(s);
  return res;
}
}
using karatsuba_impl::karatsuba;