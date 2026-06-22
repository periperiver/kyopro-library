#pragma once
#include<type_traits>
template<typename T=int>
struct MonoidAdd{
  using S=T;
  using F=std::nullptr_t;
  static inline S op(S x,S y){return x+y;}
  static inline S e(){return T();}
  static inline S mapping(F,const S&x,long long){return x;}
  static inline F composition(F,F){return nullptr;}
  static inline F id(){return nullptr;}
  static inline S inverse(S x){return -x;}
  static inline void revS(S&x){}
  static inline S pow(S x,long long p){return x*p;}
};