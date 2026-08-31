#pragma once
#include<limits>
#include "../monoid/add_pair.hpp"
#include "../monoid/update.hpp"
template<typename T>
struct RangeUpdateRangeSum{
  using M1=MonoidAddPair<T,T>;
  using M2=MonoidUpdate<T>;
  static inline M1::S act(M1::S x,M2::S f){
    if(f.f)x.first=f.val*x.second;
    return x;
  }
};