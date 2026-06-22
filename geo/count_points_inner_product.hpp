#pragma once
#include<vector>
#include<concepts>
#include<tuple>
#include<algorithm>
#include<cassert>
#include<limits>
#include "math/floor_sum.hpp"
#include "math/util.hpp"
//ax+by≤c,0≤x,0≤y
template<std::integral T>
T count_points_inner_product(std::vector<std::tuple<T,T,T>>s){
  std::sort(s.begin(),s.end(),[&](const std::tuple<T,T,T>&lhs,const std::tuple<T,T,T>&rhs){
    T lv=std::get<0>(lhs)*std::get<1>(rhs),rv=std::get<0>(rhs)*std::get<1>(lhs);
    if(lv!=rv)return lv<rv;
    return std::get<2>(lhs)*std::get<1>(rhs)>std::get<2>(rhs)*std::get<1>(lhs);
  });
  std::vector<std::tuple<T,T,T>>cht;
  auto f=[](T a2,T b2,T c2,T a1,T b1,T c1)->T {
    return safe_div(c1*b2-c2*b1,a1*b2-a2*b1);
  };
  for(auto [a,b,c]:s){
    assert(a>=0);
    assert(b>=0);
    if(c<0)return 0;
    if(a+b==0)continue;
    while(!cht.empty()){
      auto [a1,b1,c1]=cht[cht.size()-1];
      if(a*b1==a1*b)cht.pop_back();
      else{
        if(cht.size()>=2){
          auto [a2,b2,c2]=cht[cht.size()-2];
          if(f(a2,b2,c2,a1,b1,c1)<f(a1,b1,c1,a,b,c))break;
          else cht.pop_back();
        }
        else break;
      }
    }
    cht.emplace_back(a,b,c);
  }
  T res=0,prex=0;
  if(cht.size()==1&&(std::get<0>(cht[0])==0||std::get<1>(cht[0])==0)){
    return std::numeric_limits<T>::max();
  }
  for(int i=0;i<(int)cht.size();i++){
    bool fn;
    const auto&[a,b,c]=cht[i];
    T nxtx;
    if(i+1<(int)cht.size()){
      const auto&[a2,b2,c2]=cht[i+1];
      nxtx=f(a,b,c,a2,b2,c2)+1;
      fn=false;
    }
    else{
      nxtx=c/a+1;
      fn=true;
    }
    if(c-a*(nxtx-1)<0){
      nxtx=c/a+1;
      fn=true;
    }
    if(prex<nxtx){
      res+=floor_sum<T>(nxtx,b,-a,c);
      res-=floor_sum<T>(prex,b,-a,c);
      res+=nxtx-prex;
      prex=nxtx;
    }
    if(fn)break;
  }
  return res;
}