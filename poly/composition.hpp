#pragma once
#include<algorithm>
#include "polynomial_talor_shift.hpp"
#include "../convolution/transposed_ntt.hpp"
template<typename T>
std::vector<T>fps_composition(std::vector<T>f,std::vector<T>g){
  assert(f.size()==g.size());
  const int n=ceil_pow2(f.size());
  T inv4=T(n*4).inv();
  std::vector<T>prep(n*4),preq(n*4);
  auto dfs=[&](auto self,int x,int y,std::vector<T>q)->std::vector<T> {
    if(x==1){
      std::vector<T>p(n*4);
      for(int i=0;i<std::ssize(f);i++){
        p[(n-i-1)*2]=f[i];
      }
      return p;
    }
    std::copy(q.begin(),q.begin()+n*2,preq.begin());
    dft(q);
    std::vector<T>q_memo(q);
    for(int i=0;i<n*2;i++){
      T l=q[i*2],r=q[i*2+1];
      q[i*2]*=r,q[i*2+1]*=l;
    }
    idft(q);
    for(int i=0;i<n*4;i++)q[i]*=inv4;
    for(int i=0;i<n*2;i+=2)q[n*2+i]+=preq[i]+preq[i];
    std::fill(preq.begin(),preq.end(),T());
    for(int j=0;j<y*2;j++)for(int i=0;i<x/2;i++){
      preq[j*x+i]=q[j*x*2+i*2];
    }
    std::swap(q,preq);
    std::vector<T>p=self(self,x>>1,y<<1,q);
    std::swap(p,prep);
    for(int j=y*2-1;j>=0;j--)for(int i=x/2-1;i>=0;i--){
      p[j*x*2+i*2+1]+=prep[j*x+i];
      prep[j*x+i]=T();
    }
    std::fill(prep.begin(),prep.end(),T());
    for(int i=n*2-1;i>=0;i--){
      prep[i]+=p[n*2+i];
    }
    for(int i=0;i<n*4;i++)p[i]*=inv4;
    transposed_idft(p);
    for(int i=0;i<n*2;i++){
      T l=q_memo[i*2],r=q_memo[i*2+1];
      p[i*2]*=r,p[i*2+1]*=l;
    }
    transposed_dft(p);
    for(int i=0;i<n*2;i++)p[i]+=prep[i];
    std::fill(prep.begin(),prep.begin()+n*2,T());
    return p;
  };
  if(g[0].val()){
    f=polynomial_talor_shift(f,g[0]);
    g[0]=T();
  }
  std::vector<T>q(n*4);
  for(int i=0;i<std::ssize(g);i++)q[i]=-g[i];
  std::vector<T>res=dfs(dfs,n,1,q);
  res.erase(res.begin(),res.begin()+n-std::ssize(f));
  res.resize(f.size());
  std::reverse(res.begin(),res.end());
  return res;
}