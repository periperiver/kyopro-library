#pragma once
#include<numeric>
template<typename T>
struct RangeAddRangeMax{
  static_assert(std::is_arithmetic_v<T>);
  using S=T;
  using F=T;
  static inline S op(const S&x,const S&y){return x>y?x:y;}
  static inline S e(){return std::numeric_limits<S>::min()/2;}
  static inline S mapping(const F&f,const S&x,long long){return x+f;}
  static inline F composition(const F&f,const F&g){return f+g;}
  static inline F id(){return 0;}
  static inline void revS(S&x){}
  static inline S pow(const S&x,long long){return x;}
};