#pragma once
#include<vector>
#include<optional>
template<typename M>
struct PotentialUnionFind{
private:
  using S=typename M::S;
  std::vector<int>par;
  std::vector<S>dat;
  int cs;
public:
  PotentialUnionFind(int n):par(n,-1),dat(n,M::e()),cs(n){}
  inline int root(int u){
    if(par[u]<0)return u;
    int r=root(par[u]);
    dat[u]=M::op(dat[u],dat[par[u]]);
    return par[u]=r;
  }
  inline bool same(int u,int v){return root(u)==root(v);}
  bool merge(int u,int v,S x){
    int ru=root(u),rv=root(v);
    if(ru==rv){
      S val=M::op(dat[u],M::inverse(dat[v]));
      return val==x;
    }
    if(par[ru]<par[rv])std::swap(u,v),std::swap(ru,rv),x=M::inverse(x);
    par[rv]+=par[ru];
    par[ru]=rv;
    dat[ru]=M::op(M::op(M::inverse(dat[u]),x),dat[v]);
    cs--;
    return true;
  }
  std::optional<S> get(int u,int v){
    int ru=root(u),rv=root(v);
    if(ru==rv)return std::make_optional(M::op(dat[u],M::inverse(dat[v])));
    else return std::nullopt;
  }
  inline int size(int u=-1){return u==-1?cs:-par[root(u)];}
};