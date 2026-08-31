#pragma once
#include<numeric>
#include "../monoid/min.hpp"
template<typename T>
struct RangeChminRangeMin{
  using M1=MonoidMin<T>;
  using M2=MonoidMin<T>;
  static inline M1::S act(M1::S x,M2::S f){return std::min(x,f);}
};