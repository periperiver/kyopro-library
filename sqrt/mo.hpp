#pragma once
#include "mo_order.hpp"
struct Mo{
private:
  int n,z;
  std::vector<std::pair<int,int>>Q;
public:
  Mo(int N):n(N){
    z=1;
    while(z<=n)z<<=1;
  }
  void add(int l,int r){
    Q.emplace_back(l,r);
  }
public:
  void build(const auto &add,const auto &del,const auto &out){
    build(add,add,del,del,out);
  }
  void build(const auto&addl,const auto&addr,const auto&dell,const auto&delr,const auto&out){
    int q=Q.size();
    std::vector<int>ord=mo_order(n,Q);
    int l=0,r=0;
    for(int i:ord){
      if(Q[i].first<l)addl(Q[i].first,l),l=Q[i].first;
      if(r<Q[i].second)addr(r,Q[i].second),r=Q[i].second;
      if(l<Q[i].first)dell(l,Q[i].first),l=Q[i].first;
      if(Q[i].second<r)delr(Q[i].second,r),r=Q[i].second;
      out(i);
    }
  }
};