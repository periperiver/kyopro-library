#pragma once
#include "fps_inv.hpp"
template<typename T>
std::vector<T>power_projection(const std::vector<T>&f,const std::vector<T>&g,int m){
  assert(f.size()==g.size());
  const int n=ceil_pow2(f.size());
  std::vector<T>p(n*4),q(n*4);
  std::vector<T>prep(n*4),preq(n*4);
  T invn4=T(n*4).inv();
  for(int i=0;i<(int)f.size();i++)p[i+n-f.size()]=g[i],q[i]=-f[i];
  for(int x=n,y=1;x>1;x>>=1,y<<=1){
    std::copy(p.begin(),p.begin()+n*2,prep.begin());
    std::copy(q.begin(),q.begin()+n*2,preq.begin());
    dft(p),dft(q);
    for(int i=0;i<n*2;i++){
      T l=q[i*2],r=q[i*2+1];
      p[i*2]*=r,p[i*2+1]*=l;
      q[i*2]*=r,q[i*2+1]*=l;
    }
    idft(p),idft(q);
    for(int i=0;i<n*4;i++)p[i]*=invn4,q[i]*=invn4;
    for(int i=0;i<n*2;i++){
      p[n*2+i]+=prep[i];
      if(i%2==0)q[n*2+i]+=preq[i]+preq[i];
    }
    std::fill(prep.begin(),prep.end(),T()),std::fill(preq.begin(),preq.end(),T());
    for(int j=0;j<y*2;j++)for(int i=0;i<x/2;i++){
      prep[j*x+i]=p[j*x*2+i*2+1];
      preq[j*x+i]=q[j*x*2+i*2];
    }
    std::swap(p,prep);
    std::swap(q,preq);
  }
  for(int i=0;i<n;i++)p[i]=p[i*2],q[i]=q[i*2];
  p.resize(n),q.resize(n+1);
  q[n]=1;
  std::reverse(p.begin(),p.end()),std::reverse(q.begin(),q.end());
  p=ntt_convolution(p,fps_inv(q,m));
  p.resize(m);
  return p;
}