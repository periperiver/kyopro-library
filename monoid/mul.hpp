#pragma once
template<typename T>
struct MonoidMul{
  using S=T;
  static inline S op(S x,S y){return x*y;}
  static inline S e(){
    if constexpr(requires(){T::one();})return T::one();
    else return T(1);
  }
  static inline void revS(S&){}
  static inline S inverse(S x){return x.inv();}
};