#pragma once
#include "fps2d.hpp"
//[x^k]p/q
#include<iostream>
template<typename T>
std::vector<T>fps2d_kth_inv(fps2d<T>p,fps2d<T>q,int k,int deg){
  assert(q[0][0]!=T(0));
  assert(0<=k);
  assert(0<deg);
  p.shrink(),q.shrink();
  int n=std::min(std::max(p.size().first,q.size().first),k+1);
  int m=std::max(p.size().second,q.size().second);
  p.resize(n,m),q.resize(n,m);
  while(k){
    int m2=m*2-1;
    int z=ceil_pow2(n*2*m2-1);
    std::vector<T>pbuf(z),qbuf(z),mqbuf(z);
    for(int i=0;i<n;i++)for(int j=0;j<m;j++){
      pbuf[i*m2+j]=p[i][j];
      qbuf[i*m2+j]=mqbuf[i*m2+j]=q[i][j];
      if(i&1)mqbuf[i*m2+j]=-mqbuf[i*m2+j];
    }
    dft(pbuf),dft(qbuf),dft(mqbuf);
    for(int i=0;i<z;i++)pbuf[i]*=mqbuf[i],qbuf[i]*=mqbuf[i];
    idft(pbuf),idft(qbuf);
    pbuf.resize((n*2-1)*m2);
    qbuf.resize((n*2-1)*m2);
    p.a=std::move(pbuf);
    q.a=std::move(qbuf);
    p.n=q.n=n*2-1;
    p.m=q.m=m*2-1;
    int nn=std::min(n,k/2+1);
    int nm=std::min(m*2-1,deg);
    fps2d<T>np(nn,nm),nq(nn,nm);
    for(int i=0;i<nn;i++)for(int j=0;j<nm;j++){
      if(i*2+(k&1)<n*2-1)np[i][j]=p[i*2+(k&1)][j];
      nq[i][j]=q[i*2][j];
    }
    k>>=1;
    n=nn,m=nm;
    p=std::move(np);
    q=std::move(nq);
  }
  std::vector<T>res=ntt_convolution(p.a,fps_inv(q.a,deg));
  res.resize(deg);
  return res;
}