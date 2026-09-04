#pragma once
#include "../string/rolling_hash_base.hpp"
struct MonoidRH{
  using S=RH;
  static S op(S x,S y){return x+y;}
  static S e(){return {};}
};