#pragma once
#include<utility>
#include<type_traits>
#include<limits>
#include "../monoid/add_pair.hpp"
#include "../monoid/affine.hpp"
template<typename T>
struct RangeAffineRangeSum{
  using M1=MonoidAddPair<T,T>;
  using M2=MonoidAffine<T>;
  static inline M1::S act(M1::S x,M2::S f){
    x.first=x.first*f.first+x.second*f.second;
    return x;
  }
};