#pragma once
#include<limits>
#include<map>
#include "cht.hpp"
template<typename T>
struct CHTxy{
private:
  using ld=long double;
  ConvexHullTrick<ld,true>cht_min;
  ConvexHullTrick<ld,false>cht_max;
  static constexpr T INFt=std::numeric_limits<T>::max();
  T amin=INFt,amax=-INFt,bmin=INFt,bmax=-INFt;
  int aminidx=-1,amaxidx=-1,bminidx=-1,bmaxidx=-1;
  std::map<std::pair<T,T>,int>mp;
public:
  void add(T a,T b,int id=-1){
    cht_min.add(b,a);
    cht_max.add(b,a);
    mp[{a,b}]=id;
    if(amin>a)amin=a,aminidx=id;
    if(amax<a)amax=a,amaxidx=id;
    if(bmin>b)bmin=b,bminidx=id;
    if(bmax<b)bmax=b,bmaxidx=id;
  }
  std::pair<int,T>max(T x,T y){
    if(cht_min.dat.empty())return {-1,-INFt};
    if(x==0){
      if(bmax*y>bmin*y)return {bmaxidx,bmax*y};
      else return {bminidx,bmin*y};
    }
    ld z=ld(y)/x;
    if(x>0){
      auto l=cht_max.dat.lower_bound(z);
      T a=l->b,b=l->a;
      return {mp[{a,b}],a*x+b*y};
    }
    else{
      auto l=cht_min.dat.lower_bound(z);
      T a=-l->b,b=-l->a;
      return {mp[{a,b}],a*x+b*y};
    }
  }
  std::pair<int,T>min(T x,T y){
    auto [id,v]=max(-x,-y);
    return {id,-v};
  }
  void clear(){
    cht_min.dat.clear();
    cht_max.dat.clear();
    mp.clear();
    amin=bmin=INFt;
    amax=bmax=-INFt;
    aminidx=amaxidx=bminidx=bmaxidx=-1;
  }
};