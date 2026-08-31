#pragma once
#include "../monoid/add.hpp"
#include "../monoid/add_pair.hpp"
template<typename T>
struct RangeAddRangeSum{
  using M1=MonoidAddPair<T,T>;
  using M2=MonoidAdd<T>;
  static inline M1::S act(M1::S x,M2::S f){
    x.first+=x.second*f;
    return x;
  }
};