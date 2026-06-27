#pragma once
#include "array16.hpp"
struct PersistentUnionFind{
private:
  int n;
  PersistentArray16<int>p;
public:
  PersistentUnionFind():n(0),p(){}
  explicit PersistentUnionFind(int n):n(n),p(n,-1){}
  int root(int u)const{
    while(p.get(u)>=0)u=p.get(u);
    return u;
  }
  bool same(int u,int v)const{return root(u)==root(v);}
  [[nodiscard]]PersistentUnionFind merge(int u,int v){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    u=root(u),v=root(v);
    if(u==v)return *this;
    int pu=p.get(u),pv=p.get(v);
    if(pu<pv)std::swap(u,v),std::swap(pu,pv);
    PersistentArray16 np=p.set(v,pu+pv).set(u,v);
    PersistentUnionFind res;
    res.n=n;
    res.p=np;
    return res;
  }
};