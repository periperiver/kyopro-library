#pragma once
#include<algorithm>
#include "convolution/ntt.hpp"
#include "convolution/ntt_doubling.hpp"
template<typename T>
std::vector<T>polynomial_product(std::vector<std::vector<T>>f){
  if(f.empty())return std::vector<T>{1};
  std::sort(f.begin(),f.end(),[](const std::vector<T>&lhs,const std::vector<T>&rhs){return lhs.size()>rhs.size();});
  auto dfs=[&](auto self,int l,int r)->void {
    if(l+1!=r){
      int mid=(l+r)/2;
      self(self,l,mid);
      self(self,mid,r);
      f[l]=ntt_convolution(f[l],f[mid]);
    }
  };
  dfs(dfs,0,f.size());
  return f[0];
}
//prod(1+Ax)
template<typename T>
std::vector<T>polynomial_product1(std::vector<T>f){
  int m=f.size();
  int n=ceil_pow2(f.size()),log2n=msb(n);
  f.resize(n);
  for(int i=0;i<log2n;i++){
    int b=1<<i;
    std::vector<T>l(b*2),r(b*2);
    int j;
    for(j=0;j+b<m;j+=b*2){
      std::copy(f.begin()+j,f.begin()+j+b,l.begin());
      std::copy(f.begin()+j+b,f.begin()+j+b*2,r.begin());
      ntt_doubling(l),ntt_doubling(r);
      for(int k=0;k<b;k++)f[j+k]=l[k]*r[k]+l[k]+r[k];
      for(int k=b;k<b*2;k++)f[j+k]=l[k]*r[k]-l[k]-r[k];
    }
    if(j<m){
      std::copy(f.begin()+j,f.begin()+j+b,l.begin());
      ntt_doubling(l);
      for(int k=0;k<b;k++)f[j+k]=l[k];
      for(int k=b;k<b*2;k++)f[j+k]=-l[k];
    }
  }
  idft(f);
  f.push_back(1);
  std::reverse(f.begin(),f.end());
  f.resize(m+1);
  T inv=T(n).inv();
  for(int i=1;i<=m;i++)f[i]*=inv;
  return f;
}