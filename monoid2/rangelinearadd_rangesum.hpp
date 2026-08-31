#pragma once
#include<utility>
#include "../monoid/add_pair.hpp"
//bbstは壊れる
template<typename T>
struct RangeLinearAddRangeSum{
  struct M1{
    struct S{
      T sum;
      int sz,id;
    };
    static inline S op(S x,S y){
      return x.id==-1?y:S{x.sum+y.sum,x.sz+y.sz,x.id};
    }
    static inline S e(){return {0,0,-1};}
  };
  using M2=MonoidAddPair<T,T>;
  static inline M1::S act(M1::S x,M2::S f){
    long long k=x.sz;
    x.sum+=f.first*(x.id*k+(((k-1)|1)*(k&-2)>>1))+f.second*k;
    return x;
  }
};