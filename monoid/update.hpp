#pragma once
#include<limits>
template<typename T>
struct MonoidUpdate{
  struct S{
    T val;
    bool f;
    S():val(),f(false){}
    S(T v):val(v),f(true){}
  };
  static S op(S x,S y){return y.f?y:x;}
  static S e(){return S();}
  template<typename U>
  static inline S pow(S x,U p){
    if(p==0)return e();
    return x;
  }
};