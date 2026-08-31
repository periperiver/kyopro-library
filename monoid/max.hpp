#pragma once
#include<limits>
template<typename T,T E=std::numeric_limits<T>::min()>
struct MonoidMax{
  using S=T;
  static inline S op(const S&x,const S&y){return x<y?y:x;}
  static inline S e(){return E;}
  static inline void revS(S&x){}
  template<typename U>
  static inline S pow(const S&x,U){return x;}
};