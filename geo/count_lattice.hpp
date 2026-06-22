#pragma once
#include "halfplane_intersection.hpp"
#include "../math/floor_sum.hpp"
#include "../math/util.hpp"
//ax+by≤c
template<std::integral T,typename T3>
T count_lattice(const std::vector<std::tuple<T,T,T>>&lines){
  std::vector<int>ch=halfplane_intersection<T,T3>(lines);
  int n=ch.size();
  if(n==0)return 0;
  auto cross_x=[&](int i,int j){
    const auto&[a1,b1,c1]=lines[ch[i]];
    const auto&[a2,b2,c2]=lines[ch[j]];
    return std::make_pair(b2*c1-b1*c2,a1*b2-a2*b1);
  };
  auto floor_x=[&](int i,int j){
    auto [a,b]=cross_x(i,j);
    return safe_div(a,b);
  };
  auto ceil_x=[&](int i,int j){
    auto [a,b]=cross_x(i,j);
    return safe_ceil(a,b);
  };
  T res=0;
  bool lower=false,upper=false;
  for(int i=0;i<n;i++){
    const auto&[a,b,c]=lines[ch[i]];
    if(b>0){
      T l=ceil_x(i,(i+1)%ch.size());
      T r;
      if(!upper)r=floor_x(i,(i-1+ch.size())%ch.size())+1,upper=true;
      else r=ceil_x(i,(i-1+ch.size())%ch.size());
      res+=floor_sum(r-l,b,-a,c-a*l);
    }
    else if(b<0){
      T l,r=floor_x(i,(i+1)%ch.size())+1;
      if(!lower)l=ceil_x(i,(i-1+ch.size())%ch.size()),lower=true;
      else l=floor_x(i,(i-1+ch.size())%ch.size())+1;
      res-=floor_sum(r-l,-b,a,a*l-c-1);
    }
  }
  return res;
}