#pragma once
#include "bit.hpp"
#include<algorithm>
#include<numeric>
template<typename I,typename M>
std::vector<typename M::S>static_rectangle_sum(std::vector<std::tuple<I,I,typename M::S>>points,const std::vector<std::tuple<I,I,I,I>>&query){
  using S=typename M::S;
  int n=points.size(),q=query.size();
  std::vector<I>z(n);
  for(int i=0;i<n;i++)z[i]=std::get<1>(points[i]);
  std::sort(z.begin(),z.end()),z.erase(std::unique(z.begin(),z.end()),z.end());
  std::sort(points.begin(),points.end(),[&](const std::tuple<I,I,S>&lhs,const std::tuple<I,I,S>&rhs){return std::get<0>(lhs)<std::get<0>(rhs);});
  struct Q{
    int i,x,l,r;
    bool operator<(const Q&rhs)const{return this->x<rhs.x;}
  };
  std::vector<Q>query2;
  query2.reserve(q*2);
  int p=0;
  for(const auto&[lx,rx,ly,ry]:query){
    int l=std::lower_bound(z.begin(),z.end(),ly)-z.begin();
    int r=std::lower_bound(z.begin(),z.end(),ry)-z.begin();
    query2.push_back({~p,lx,l,r});
    query2.push_back({p,rx,l,r});
    p++;
  }
  std::sort(query2.begin(),query2.end());
  std::vector<S>res(q);
  p=0;
  BinaryIndexedTree<M>BIT(z.size());
  for(const auto&[i,x,l,r]:query2){
    while(p<n){
      if(std::get<0>(points[p])<x){
        BIT.add(std::lower_bound(z.begin(),z.end(),std::get<1>(points[p]))-z.begin(),std::get<2>(points[p]));
        p++;
      }
      else break;
    }
    if(i>=0)res[i]=M::op(res[i],BIT.sum(l,r));
    else res[~i]=M::op(res[~i],M::inverse(BIT.sum(l,r)));
  }
  return res;
}