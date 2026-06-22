#pragma once
#include<queue>
#include<limits>
#include "point.hpp"
template<typename T,bool LOWERHULL=true>
struct ConvexHullTrickDeque{
  static constexpr T inf=std::numeric_limits<T>::max();
  static constexpr T eps=epsilon<T>();
  static inline bool is_concave(const Point<T>&l,const Point<T>&m,const Point<T>&r){
    if constexpr(LOWERHULL)return cross<T>(m-l,r-l)>eps;
    else return cross<T>(m-l,r-l)<eps;
  }
  std::deque<Point<T>>que;
  static constexpr Point<T>none{inf,inf};
  ConvexHullTrickDeque():que(){}
  Point<T>add_left(const Point<T>&v){
    if(que.empty()){
      que.push_front(v);
      return none;
    }
    while(que.size()>=2){
      const Point<T>&m=que[0];
      const Point<T>&r=que[1];
      if(is_concave(v,m,r))break;
      que.pop_front();
    }
    Point<T>res=que.front();
    que.push_front(v);
    return res;
  }
  Point<T>add_right(const Point<T>&v){
    if(que.empty()){
      que.push_back(v);
      return none;
    }
    while(que.size()>=2){
      const Point<T>&l=que[que.size()-2];
      const Point<T>&m=que.back();
      if(is_concave(l,m,v))break;
      que.pop_back();
    }
    Point<T>res=que.back();
    que.push_back(v);
    return res;
  }
};