#pragma once
#include "../monoid/add.hpp"
#include "../monoid/min.hpp"
template<typename T>
struct RangeAddRangeMin{
  using M1=MonoidMin<T,std::numeric_limits<T>::max()/2>;
  using M2=MonoidAdd<T>;
  static inline M1::S act(M1::S x,M2::S f){return x+f;}
};