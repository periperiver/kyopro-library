#pragma once
template<typename T>
struct RangeAddRangeSum{
  using S=T;
  using F=T;
  static inline S op(const S&x,const S&y){return x+y;}
  static inline S e(){return 0;}
  static inline S mapping(const F&f,const S&x,long long sz){return x+f*sz;}
  static inline F composition(const F&f,const F&g){return f+g;}
  static inline F id(){return 0;}
  static inline void revS(S&x){}
  static inline S pow(const S&x,long long p){return x*p;}
  static inline S inverse(const S&x){return -x;}
};