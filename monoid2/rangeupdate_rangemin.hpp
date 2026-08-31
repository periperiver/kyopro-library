#pragma once
#include "../monoid/min.hpp"
#include "../monoid/update.hpp"
template<typename T>
struct RangeUpdateRangeMin{
  using M1=MonoidMin<T>;
  using M2=MonoidUpdate<T>;
  static inline M1::S act(M1::S x,M2::S f){
    if(f.f)x=f.val;
    return x;
  }
};