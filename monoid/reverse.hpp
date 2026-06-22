#pragma once
#include<utility>
template<typename M>
struct ReversibleMonoid{
  using S=std::pair<typename M::S,typename M::S>;
  using F=typename M::F;
  static inline S op(S x,S y){return std::make_pair(M::op(x.first,y.first),M::op(y.second,x.second));}
  static inline S e(){return std::make_pair(M::e(),M::e());}
  static inline S mapping(const F&f,const S&x,long long k){return std::make_pair(M::mapping(f,x.first,k),M::mapping(f,x.second,k));}
  static inline F composition(const F&f,const F&g){return M::composition(f,g);}
  static inline F id(){return M::id();}
  static inline S inverse(const S&x){return std::make_pair(M::inverse(x.first),M::inverse(x.second));}
  static inline void revS(S&x){std::swap(x.first,x.second);}
  static inline S pow(const S&x){return std::make_pair(M::pow(x.first),M::pow(x.second));}
};