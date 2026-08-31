#pragma once
#include<type_traits>
#include<utility>
template<typename T1,typename T2>
struct MonoidAddPair{
  using S=std::pair<T1,T2>;
  static inline S op(S x,S y){return std::make_pair(x.first+y.first,x.second+y.second);}
  static inline S e(){return std::make_pair(T1(),T2());}
  static inline S inverse(S x){return std::make_pair(-x.first,-x.second);}
  static inline void revS(S&x){}
  template<typename U>
  static inline S pow(S x,U p){return std::make_pair(x.first*p,x.second*p);}
};