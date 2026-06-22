#pragma once
#include<limits>
//min
template<typename T>
struct Top2{
  int key1,key2;
  T top1,top2;
  Top2():key1(std::numeric_limits<int>::min()),key2(std::numeric_limits<int>::min()),top1(std::numeric_limits<T>::max()),top2(std::numeric_limits<T>::max()){}
  bool set(int k,T v){
    if(k==key1){
      if(top1>v){
        top1=v;
        return true;
      }
    }
    else if(top1>v){
      key2=key1;
      top2=top1;
      key1=k;
      top1=v;
      return true;
    }
    else if(top2>v){
      key2=k;
      top2=v;
      return true;
    }
    return false;
  }
  T neq_min(int k)const{return k!=key1?top1:top2;}
  bool contains(int k,T v)const{return (k==key1&&v==top1)||(k==key2&&v==top2);}
};