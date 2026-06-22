#pragma once
#include<vector>
#include<cassert>
#include<bit>
#include "../math/util.hpp"
template<typename T>
std::vector<T>arbitrary_xor_convolution(std::vector<T>f,std::vector<T>g){
  assert(f.size()==g.size());
  int n=f.size();
  int lg=msb(n);
  assert(1<<lg==n);
  for(int i=0;i<lg;i++)for(int j=0;j<n;j++)if(j>>i&1){
    f[j^(1<<i)]=f[j^(1<<i)]-f[j];
    g[j^(1<<i)]=g[j^(1<<i)]-g[j];
  }
  std::vector<std::vector<T>>a(n,std::vector<T>(lg+1)),b(n,std::vector<T>(lg+1));
  for(int i=0;i<n;i++){
    a[i][std::popcount<unsigned>(i)]=f[i];
    b[i][std::popcount<unsigned>(i)]=g[i];
  }
  for(int i=0;i<lg;i++)for(int j=0;j<n;j++)if(j>>i&1){
    for(int k=0;k<=lg;k++){
      a[j][k]=a[j][k]+a[j^(1<<i)][k];
      b[j][k]=b[j][k]+b[j^(1<<i)][k];
    }
  }
  std::vector<T>h(lg+1);
  for(int i=0;i<n;i++){
    std::fill(h.begin(),h.end(),T());
    T sum=T();
    for(int j=0;j<=lg;j++){
      for(int k=0;j+k<lg;k++)h[j+k]=h[j+k]+a[i][j]*b[i][k];
      sum=sum+sum+b[i][lg-j];
      h[lg]=h[lg]+a[i][j]*sum;
    }
    for(int j=lg-1;j>=0;j--)h[j]=h[j]+h[j+1]+h[j+1];
    std::copy(h.begin(),h.end(),a[i].begin());
  }
  for(int i=0;i<lg;i++)for(int j=0;j<n;j++)if(j>>i&1){
    for(int k=0;k<=lg;k++){
      a[j][k]=a[j][k]-a[j^(1<<i)][k];
    }
  }
  for(int i=0;i<n;i++)f[i]=a[i][std::popcount<unsigned>(i)];
  for(int i=0;i<lg;i++)for(int j=0;j<n;j++)if(j>>i&1)f[j^(1<<i)]=f[j^(1<<i)]+f[j];
  return f;
}