#pragma once
#include<cassert>
#include <type_traits>
#include<limits>
#include<algorithm>
#include "datastructure/unionfind.hpp"
template<typename T>
struct MSTEdge{
  int l,r;
  int u,v;
  T weight;
  MSTEdge(){}
  MSTEdge(int l_,int r_,int u_,int v_,T weight_):l(l_),r(r_),u(u_),v(v_),weight(weight_){}
  MSTEdge(int u_,int v_,T weight_):l(0),r(numeric_limits<T>::max()),u(u_),v(v_),weight(weight_){}
  bool operator<(const MSTEdge<T>&rhs)const{
    return this->weight<rhs.weight;
  }
};
template<typename T>
std::vector<T>offline_dynamic_mst(int n,int q,std::vector<MSTEdge<T>>edge){
  static_assert(std::is_integral_v<T>);
  static const T inf=std::numeric_limits<T>::max();
  std::vector<T>ret(q);
  UnionFind uf1(n),uf2(n);
  std::vector<int>idx(n);
  auto dfs=[&](auto self,int l,int r,int vn,std::vector<MSTEdge<T>>edges,T compressed)->void {
    std::erase_if(edges,[&](const MSTEdge<T>&e)->bool {return e.r<=l||r<=e.l;});
    uf1.reset(vn),uf2.reset(vn);
    for(const MSTEdge<T>&e:edges)if(l<e.l||e.r<r)uf1.merge(e.u,e.v);
    for(const MSTEdge<T>&e:edges)if(e.l<=l&&r<=e.r&&uf1.merge(e.u,e.v)){
      uf2.merge(e.u,e.v);
      compressed+=e.weight;
    }
    if(l+1==r)ret[l]=uf2.size(0)==vn?compressed:inf;
    else{
      int p=0;
      for(int i=0;i<vn;i++)if(uf2.root(i)==i)idx[i]=p++;
      uf1.reset(p);
      for(MSTEdge<T>&e:edges){
        e.u=idx[uf2.root(e.u)],e.v=idx[uf2.root(e.v)];
        if(e.l<=l&&r<=e.r){
          if(uf1.same(e.u,e.v))e.l=e.r=q;
          else uf1.merge(e.u,e.v);
        }
      }
      int mid=(l+r)>>1;
      self(self,l,mid,p,edges,compressed);
      self(self,mid,r,p,edges,compressed);
    }
  };
  std::sort(edge.begin(),edge.end());
  dfs(dfs,0,q,n,edge,0);
  return ret;
}