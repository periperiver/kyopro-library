#pragma once
#include<numeric>
#include "../monoid/max.hpp"
template<typename T>
struct RangeChmaxRangeMax{
  using M1=MonoidMax<T>;
  using M2=MonoidMax<T>;
  static inline M1::S act(M1::S x,M2::S f){return std::max(x,f);}
};