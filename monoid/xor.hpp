#pragma once
template<typename T>
struct MonoidXor{
  using S=T;
  static inline S op(S x,S y){return x^y;}
  static inline S e(){return 0;}
  static inline void revS(S&){}
  static inline S inverse(S x){return x;}
  template<typename U>
  static inline S pow(S x,U k){return k&1?x:e();}
};