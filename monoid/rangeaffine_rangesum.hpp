#pragma once
#include<utility>
#include<type_traits>
#include<limits>
template<typename T>
struct RangeAffineRangeSum{
  using S=T;
  using F=std::pair<T,T>;
  static const F ID;
  static inline S op(const S&x,const S&y){return x+y;}
  static inline S e(){return 0;}
  template<typename T2>
  static inline S mapping(const F&f,const S&x,T2 sz){return f.first*x+f.second*sz;}
  static inline F composition(const F&f,const F&g){return std::make_pair(f.first*g.first,g.second*f.first+f.second);}
  static inline F id(){return ID;}
  static inline void revS(S&x){}
  static inline S inverse(const S&x){return -x;}
  template<typename T2>
  static inline S pow(const S&x,T2 p){return x*p;}
};
template<typename T>const typename RangeAffineRangeSum<T>::F RangeAffineRangeSum<T>::ID{1,0};
template<int mod>
struct RangeAffineRangeSumMod{
  using S=long long;
  using F=std::pair<long long,long long>;
  static inline S op(S x,S y){return x+y>=mod?x+y-mod:x+y;}
  static inline S e(){return 0;}
  template<typename T,std::enable_if_t<(std::numeric_limits<T>::digits<=32),std::nullptr_t> =nullptr>
  static inline S mapping(const F&f,S x,T sz){return (f.first*x+f.second*sz)%mod;}
  template<typename T,std::enable_if_t<(std::numeric_limits<T>::digits>32),std::nullptr_t> =nullptr>
  static inline S mapping(const F&f,S x,T sz){return (f.first*x+f.second*(sz%mod))%mod;}
  static inline F composition(const F&f,const F&g){return std::make_pair((f.first*g.first)%mod,(g.second*f.first+f.second)%mod);}
  static inline F id(){return std::make_pair(1,0);}
  static inline void revS(S&x){}
  static inline S inverse(S x){return x?mod-x:0;}
  template<typename T,std::enable_if_t<(std::numeric_limits<T>::digits<=32),std::nullptr_t> =nullptr>
  static inline S pow(S x,T p){return (x*p)%mod;}
  template<typename T,std::enable_if_t<(std::numeric_limits<T>::digits>32),std::nullptr_t> =nullptr>
  static inline S pow(S x,T p){return (x*(p%mod))%mod;}
};