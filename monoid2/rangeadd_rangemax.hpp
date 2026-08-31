#pragma once
#include "../monoid/add.hpp"
#include "../monoid/max.hpp"
template<typename T>
struct RangeAddRangeMax{
  using M1=MonoidMax<T,std::numeric_limits<T>::min()/2>;
  using M2=MonoidAdd<T>;
  static inline M1::S act(M1::S x,M2::S f){return x+f;}
};