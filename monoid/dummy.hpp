#pragma once
#include<utility>
struct MonoidDummy{
  using S=std::nullptr_t;
  using F=std::nullptr_t;
  static inline S op(S x,S y){return nullptr;}
  static inline S e(){return nullptr;}
  template<typename T>
  S mapping(F,S,T){return nullptr;}
  F composition(F,F){return nullptr;}
  static inline F id(){return nullptr;}
  static inline S inverse(S){return nullptr;}
  static inline void revS(S&){}
  template<typename T>
  static inline S pow(S,T){return nullptr;}
};