#pragma once
#include<limits>
template<typename T>
struct RangeUpdateRangeSum{
  using S=T;
  using F=T;
  static constexpr F none=std::numeric_limits<T>::min();
  static inline S op(const S&x,const S&y){return x+y;}
  static inline S e(){return 0;}
  static inline S mapping(const F&f,S x,long long sz){
    if(f!=none)x=f*sz;
    return x;
  }
  static inline F composition(const F&f,const F&g){return f==none?g:f;}
  static inline F id(){return none;}
  static inline void revS(S&){}
  static inline S pow(const S&x,long long p){return x*p;}
};