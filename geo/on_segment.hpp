#pragma once
#include "point.hpp"
//a,b,cがこの順に同一直線上にあるか
template<typename T>
bool on_segment(Point<T>a,Point<T>b,Point<T>c){
  b-=a,c-=a;
  if(std::abs(cross(b,c))>epsilon<T>())return false;
  T d=dot(b,c);
  return -epsilon<T>()<=d&&d<=dot(c,c)+epsilon<T>();
}