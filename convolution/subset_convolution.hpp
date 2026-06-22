#pragma once
#include<vector>
#include<cassert>
#include<numeric>
#include "math/util.hpp"
template<typename T>
std::vector<T>subset_convolution(const std::vector<T>&a,const std::vector<T>&b){
  int n=msb(a.size());
  assert((1<<n)==(int)a.size());
  assert(a.size()==b.size());
  if(n<=12){
    std::vector<T>res(1<<n);
    for(int i=0;i<(1<<n);i++)for(int j=0;j<(1<<n);j++){
      j|=i;
      res[j]+=a[i]*b[i^j];
    }
    return res;
  }
  std::vector<int>c(1<<n,0);
  for(int i=1;i<(1<<n);i++)c[i]=c[i-(i&-i)]+1;
  std::vector<std::vector<T>>a2(1<<n,std::vector<T>(n+1,0)),b2(1<<n,std::vector<T>(n+1,0));
  for(int i=0;i<(1<<n);i++)a2[i][c[i]]=a[i],b2[i][c[i]]=b[i];
  for(int i=1;i<(1<<n);i<<=1){
    for(int j=0;j<(1<<n);j+=i*2){
      for(int k=0;k<i;k++){
        for(int l=0;l<c[i+j+k];l++){
          a2[i+j+k][l]+=a2[j+k][l];
          b2[i+j+k][l]+=b2[j+k][l];
        }
      }
    }
  }
  for(int i=0;i<(1<<n);i++){
    std::vector<T>nxt(n+1,0);
    for(int j=0;j<=n;j++){
      for(int k=0;k<=n-j;k++)nxt[j+k]+=a2[i][j]*b2[i][k];
    }
    std::swap(a2[i],nxt);
  }
  for(int i=1<<(n-1);i>0;i>>=1){
    for(int j=0;j<(1<<n);j+=i*2){
      for(int k=0;k<i;k++){
        for(int l=c[i+j+k];l<=n;l++){
          a2[i+j+k][l]-=a2[j+k][l];
        }
      }
    }
  }
  std::vector<T>ret(1<<n);
  for(int i=0;i<(1<<n);i++)ret[i]=a2[i][c[i]];
  return ret;
}