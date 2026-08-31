#pragma once
#include<utility>
template<typename M>
struct ReversibleMonoid{
  struct S{
    typename M::S sum,mus;
    S():sum(M::e()),mus(M::e()){}
    S(typename M::S v):sum(v),mus(v){}
    S(typename M::S sum,typename M::S mus):sum(sum),mus(mus){}
  };
  static inline S op(S x,S y){return S(M::op(x.sum,y.sum),M::op(y.mus,x.mus));}
  static inline S e(){return S();}
  static inline void revS(S&x){std::swap(x.sum,x.mus);}
};