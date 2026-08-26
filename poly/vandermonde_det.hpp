#pragma once
#include "fps_inv.hpp"
#include "../convolution/ntt_doubling.hpp"
#include "../convolution/transposed_ntt_doubling.hpp"
#include<algorithm>
template<typename T>
T vandermonde_det(std::vector<T>f){
  int n=ceil_pow2(f.size());
  int log2n=msb(n);
  std::vector<std::vector<T>>f2(log2n+1,std::vector<T>(n*2));
  for(int i=0;i<f.size();i++)f2[0][i*2]=-f[i];
  for(int i=0;i<log2n;i++){
    int b=1<<i;
    std::vector<T>l(b*2),r(b*2);
    for(int j=0;j<n*2;j+=b*4){
      std::copy(f2[i].begin()+j,f2[i].begin()+j+b,l.begin());
      std::copy(f2[i].begin()+j+b*2,f2[i].begin()+j+b*3,r.begin());
      ntt_doubling(l),ntt_doubling(r);
      for(int k=0;k<b;k++)l[k]++,r[k]++;
      for(int k=b;k<b*2;k++)l[k]--,r[k]--;
      std::copy(l.begin(),l.end(),f2[i].begin()+j);
      std::copy(r.begin(),r.end(),f2[i].begin()+j+b*2);
      for(int k=0;k<b*2;k++)f2[i+1][j+k]=l[k]*r[k]-T::raw(1);
    }
  }
  std::vector<std::vector<T>>g(log2n+1,std::vector<T>(n*2));
  {
    std::vector<T>h(f2[log2n].begin(),f2[log2n].begin()+n);
    idft(h);
    T inv=T::raw(h.size()).inv();
    for(T&x:h)x*=inv;
    std::reverse(h.begin(),h.end());
    for(int i=n-1;i>=1;i--)h[i]=h[i-1];
    h[0]=1;
    h=fps_inv(h);
    std::copy(h.begin(),h.end(),g[log2n].begin()+n-1);
  }
  transposed_idft(g[log2n]);
  for(int i=0;i<n*2;i++)g[log2n][i]/=n*2;
  for(int i=log2n-1;i>=0;i--){
    int b=1<<i;
    for(int j=0;j<n*2;j+=b*4){
      std::vector<T>l(f2[i].begin()+j,f2[i].begin()+j+b*2);
      std::vector<T>r(f2[i].begin()+j+b*2,f2[i].begin()+j+b*4);
      std::vector<T>g2(g[i+1].begin()+j,g[i+1].begin()+j+b*4);
      l.resize(b*4);
      r.resize(b*4);
      ntt_doubling(l),ntt_doubling(r);
      for(int k=0;k<b*4;k++)l[k]=g2[k]*l[k]*l[k],r[k]=g2[k]*r[k];
      transposed_ntt_doubling(l);
      transposed_ntt_doubling(r);
      std::copy(l.begin(),l.end(),g[i].begin()+j+b*2);
      std::copy(r.begin(),r.end(),g[i].begin()+j);
    }
  }
  T res=1;
  for(int i=0;i<(int)f.size();i++)res*=g[0][i*2]+g[0][i*2+1];
  return res;
}