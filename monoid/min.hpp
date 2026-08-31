#pragma once
#include<limits>
template<typename T,T E=std::numeric_limits<T>::max()>
struct MonoidMin{
  using S=T;
  static inline S op(const S&x,const S&y){return x<y?x:y;}
  static inline S e(){return E;}
  static inline void revS(S&x){}
  template<typename U>
  static inline S pow(const S&x,U){return x;}
};