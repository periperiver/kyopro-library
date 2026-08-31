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
  // using S=std::pair<int,T>;
  // using F=std::pair<T,T>;
  // static inline S op(const S&x,const S&y){return x.first==-1?y:std::make_pair(x.first,x.second+y.second);}
  // static inline S e(){return std::make_pair(-1,T());}
  // static inline S mapping(const F&f,const S&x,long long k){return std::make_pair(x.first,x.second+f.first*(x.first*k+(((k-1)|1)*((k&-2)>>1)))+f.second*k);}
  // static inline F composition(const F&f,const F&g){return std::make_pair(f.first+g.first,f.second+g.second);}
  // static inline F id(){return std::make_pair(T(),T());}
};
/*
偶数:k-1
奇数:k
*/