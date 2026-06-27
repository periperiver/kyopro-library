#pragma once
#include "array16.hpp"
#include<optional>
template<typename M>
struct PersistentPotentialUnionFind{
private:
  using S=typename M::S;
  int n;
  PersistentArray16<std::pair<int,S>>p;
public:
  PersistentPotentialUnionFind():n(0),p(){}
  explicit PersistentPotentialUnionFind(int n):n(n),p(n,std::make_pair(-1,M::e())){}
  std::pair<int,S>root(int u)const{
    S res=M::e();
    while(true){
      auto [pu,v]=p.get(u);
      if(pu<0)break;
      u=pu;
      res=M::op(res,v);
    }
    return std::make_pair(u,res);
  }
  bool same(int u,int v)const{return root(u).first==root(v).first;}
  [[nodiscard]]std::optional<PersistentPotentialUnionFind>merge(int u,int v,S x){
    auto [ru,su]=root(u);
    auto [rv,sv]=root(v);
    if(ru==rv){
      S val=M::op(su,M::inverse(sv));
      if(val==x)return std::make_optional(*this);
      else return std::nullopt;
    }
    u=ru,v=rv;
    int pu=p.get(u).first,pv=p.get(v).first;
    if(pu<pv)std::swap(u,v),std::swap(su,sv),std::swap(pu,pv),x=M::inverse(x);
    PersistentPotentialUnionFind res;
    res.n=n;
    res.p=p.set(v,std::make_pair(pu+pv,M::e())).set(u,std::make_pair(v,M::op(M::op(M::inverse(su),x),sv)));
    return std::make_optional(res);
  }
  std::optional<S>get(int u,int v){
    auto [ru,su]=root(u);
    auto [rv,sv]=root(v);
    if(ru!=rv)return std::nullopt;
    return std::make_optional(M::op(su,M::inverse(sv)));
  }
  int size(int u)const{return -p.get(root(u)).first;}
};