#pragma once
#include<type_traits>
template<typename T>
struct MonoidAdd{
  using S=T;
  static inline S op(S x,S y){return x+y;}
  static inline S e(){return T();}
  static inline S inverse(S x){return -x;}
  static inline void revS(S&x){}
  template<typename U>
  static inline S pow(S x,U p){return x*p;}
};