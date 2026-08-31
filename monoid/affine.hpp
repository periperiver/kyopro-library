#pragma once
#include<utility>
template<typename T>
struct MonoidAffine{
  using S=std::pair<T,T>;
  static inline S op(const S&x,const S&y){return std::make_pair(x.first*y.first,x.second*y.first+y.second);}
  static inline S e(){return std::make_pair(1,0);}
  static inline S inverse(const S&x){
    T inv=T(1)/x.first;
    return std::make_pair(inv,-x.second*inv);
  }
};