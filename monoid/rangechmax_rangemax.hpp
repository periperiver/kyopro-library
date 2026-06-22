#pragma once
#include<numeric>
template<typename T>
struct RangeChmaxRangeMax{
  using S=T;
  using F=T;
  static inline S op(const S&x,const S&y){return x>y?x:y;}
  static inline S e(){return std::numeric_limits<S>::min();}
  static inline S mapping(const F&f,const S&x,long long){return f>x?f:x;}
  static inline F composition(const F&f,const F&g){return f>g?f:g;}
  static inline F id(){return std::numeric_limits<F>::min();}
  static inline void revS(S&x){}
  static inline S pow(const S&x,long long){return x;}
};