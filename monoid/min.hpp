#pragma once
#include<limits>
template<typename T,T E=std::numeric_limits<T>::max()>
struct MonoidMin{
  using S=T;
  using F=std::nullptr_t;
  static inline S op(const S&x,const S&y){return x<y?x:y;}
  static inline S e(){return E;}
  static inline S mapping(F,const S&x,long long){return x;}
  static inline F composition(F,F){return nullptr;}
  static inline F id(){return nullptr;}
  static inline void revS(S&x){}
  static inline S pow(const S&x,long long p){return x;}
};