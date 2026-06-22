#pragma once
#include<ranges>
#include<deque>
#include<concepts>
#include<cmath>
#include "argsort.hpp"
//ax+by≤c
//有界を仮定
template<typename T,typename T3>
std::vector<int>halfplane_intersection(const std::vector<std::tuple<T,T,T>>&lines){
  int n=lines.size();
  std::vector<Point<T>>points(n);
  for(const auto&[i,l]:lines|std::views::enumerate){
    const auto&[a,b,c]=l;
    if(a==0&&b==0&&c<0)return {};
    points[i]=Point<T>(a,b);
  }
  auto is_convex=[&](int l1,int l2,int l3)->bool {
    const auto&[a1,b1,c1]=lines[l1];
    const auto&[a2,b2,c2]=lines[l2];
    const auto&[a3,b3,c3]=lines[l3];
    T3 det=T3(a1)*T3(b2)-T3(a2)*T3(b1);
    T3 x=T3(b2)*T3(c1)-T3(b1)*T3(c2);
    T3 y=T3(a1)*T3(c2)-T3(a2)*T3(c1);
    if(det<0){
      det=-det;
      x=-x;
      y=-y;
    }
    return T3(a3)*x+T3(b3)*y<=T3(c3)*det;
  };
  std::deque<int>que;
  for(int i:arg_sort(points)){
    if(std::get<0>(lines[i])==0&&std::get<1>(lines[i])==0)continue;
    if(!que.empty()){
      const auto&[a1,b1,c1]=lines[que.back()];
      const auto&[a2,b2,c2]=lines[i];
      if(a1*b2==a2*b1&&a1*a2+b1*b2>0){
        T d1,d2;
        if constexpr(std::is_integral_v<T>)d1=std::gcd(a1,b1),d2=std::gcd(a2,b2);
        else d1=std::hypot(a1,b1),d2=std::hypot(a2,b2);
        if(c1*d2>c2*d1){
          que.pop_back();
        }
        else continue;
      }
    }
    while((int)que.size()>=2&&!is_convex(que[que.size()-2],que[que.size()-1],i))que.pop_back();
    while((int)que.size()>=3&&!is_convex(que[1],que[0],i))que.pop_front();
    if((int)que.size()==2&&!is_convex(que[1],que[0],i)){
      const auto&[a1,b1,c1]=lines[que[0]];
      const auto&[a2,b2,c2]=lines[i];
      if(a1*b2!=a2*b1)return {};
    }
    que.push_back(i);
  }
  while((int)que.size()>=2&&!is_convex(que[que.size()-2],que[que.size()-1],que[0]))que.pop_back();
  while((int)que.size()>=2&&!is_convex(que[que.size()-1],que[0],que[1]))que.pop_front();
  if((int)que.size()<=2)return {};
  return std::vector<int>(que.begin(),que.end());
}