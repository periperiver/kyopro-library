#pragma once
#include "fps.hpp"
#include "../convolution/ntt_doubling.hpp"
#include<cassert>
/*
0≤j<mに対するsum(f_i*g_i^j)の列挙
sum_i(f_i/(1-g_i*x))
*/
template<typename T>
std::vector<T>power_prod_sum(std::vector<T>f,std::vector<T>g,int m){
  assert(f.size()==g.size());
  int s=f.size();
  int n=ceil_pow2(f.size()),log2n=msb(n);
  for(int i=0;i<s;i++)g[i]=-g[i];
  f.resize(n),g.resize(n);
  for(int i=0;i<log2n;i++){
    int b=1<<i;
    std::vector<T>lf(b*2),lg(b*2),rf(b*2),rg(b*2);
    int j;
    for(j=0;j+b<s;j+=b*2){
      std::copy(f.begin()+j,f.begin()+j+b,lf.begin());
      std::copy(f.begin()+j+b,f.begin()+j+b*2,rf.begin());
      std::copy(g.begin()+j,g.begin()+j+b,lg.begin());
      std::copy(g.begin()+j+b,g.begin()+j+b*2,rg.begin());
      ntt_doubling(lf),ntt_doubling(rf),ntt_doubling(lg),ntt_doubling(rg);
      for(int k=0;k<b;k++)lg[k]++,rg[k]++;
      for(int k=b;k<b*2;k++)lg[k]--,rg[k]--;
      for(int k=0;k<b*2;k++){
        f[j+k]=lf[k]*rg[k]+lg[k]*rf[k];
        g[j+k]=lg[k]*rg[k];
        g[j+k]--;
      }
    }
    if(j<s){
      std::copy(f.begin()+j,f.begin()+j+b,lf.begin());
      std::copy(g.begin()+j,g.begin()+j+b,lg.begin());
      ntt_doubling(lf),ntt_doubling(lg);
      for(int k=0;k<b;k++){
        f[j+k]=lf[k];
        g[j+k]=lg[k];
      }
      for(int k=b;k<b*2;k++){
        f[j+k]=-lf[k];
        g[j+k]=-lg[k];
      }
    }
  }
  idft(f),idft(g);
  g.push_back(n);
  std::reverse(f.begin(),f.end());
  std::reverse(g.begin(),g.end());
  f.resize(m),g.resize(m);
  f=ntt_convolution(f,fps_inv(g));
  f.resize(m);
  return f;
}