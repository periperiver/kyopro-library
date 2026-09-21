#pragma once
#include "fps2d.hpp"
//[x^k]p/q
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
    fps2d<T>mq(q);
    for(int i=1;i<n;i+=2)for(int j=0;j<m;j++)mq[i][j]=-mq[i][j];
    p*=mq;
    q*=mq;
    int nn=std::min(n,k/2+1);
    int nm=std::min(m*2-1,deg);
    fps2d<T>np(nn,nm),nq(nn,nm);
    for(int i=0;i<nn;i++)for(int j=0;j<nm;j++){
      np[i][j]=p[i*2+(k&1)][j];
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