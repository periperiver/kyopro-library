#pragma once
#include "math/util.hpp"
#include "convolution/ntt.hpp"
#include "convolution/transposed_ntt.hpp"
#include "convolution/ntt_doubling.hpp"
#include "convolution/transposed_ntt_doubling.hpp"
#include "fps.hpp"
#include "middle_product.hpp"
template<typename T>
std::vector<T>multipoint_evaluation(std::vector<T>f,const std::vector<T>&x){
  int n=ceil_pow2((int)x.size()),log2n=msb(n);
  std::vector<std::vector<T>>f2(log2n+1,std::vector<T>(n*2));
  for(int i=0;i<x.size();i++)f2[0][i*2]=-x[i];
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
  std::vector<T>g(f2[log2n].begin(),f2[log2n].begin()+n);
  idft(g);
  {
    T inv=T::raw(g.size()).inv();
    for(int i=0;i<n;i++)g[i]*=inv;
  }
  g.push_back(T::raw(1));
  std::reverse(g.begin(),g.end());
  g=fps_inv(g,f.size());
  f.resize(f.size()+n-1);
  f=middle_product(f,g);
  std::reverse(f.begin(),f.end());
  transposed_idft(f);
  {
    T inv=T::raw(f.size()).inv();
    for(int i=0;i<f.size();i++)f[i]*=inv;
  }
  for(int i=log2n-1;i>=0;i--){
    std::vector<T>nxtf(n);
    int b=1<<i;
    for(int j=0;j<n;j+=b*2){
      std::vector<T>l(b*2),r(b*2);
      for(int k=0;k<b*2;k++)l[k]=f[j+k]*f2[i][j*2+b*2+k],r[k]=f[j+k]*f2[i][j*2+k];
      transposed_ntt_doubling(l),transposed_ntt_doubling(r);
      for(int k=0;k<b;k++)nxtf[j+k]=l[k],nxtf[j+b+k]=r[k];
    }
    std::swap(f,nxtf);
  }
  f.resize(x.size());
  return f;
}