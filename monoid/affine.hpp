#pragma once
#include<utility>
template<typename T>
struct MonoidAffine{
  using S=std::pair<T,T>;
  using F=std::nullptr_t;
  static inline S op(const S&x,const S&y){return std::make_pair(x.first*y.first,x.second*y.first+y.second);}
  static inline S e(){return std::make_pair(1,0);}
  static inline S mapping(F,const S&x,long long){return x;}
  static inline F composition(F,F){return nullptr;}
  static inline F id(){return nullptr;}
  static inline S inverse(const S&x){
    T inv=T(1)/x.first;
    return std::make_pair(inv,-x.second*inv);
  }
  static inline S pow(S x,long long p){
    S res=e();
    while(p){
      if(p&1)res=op(res,x);
      x=op(x,x);
      p>>=1;
    }
    return res;
  }
};