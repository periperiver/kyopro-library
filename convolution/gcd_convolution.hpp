#pragma once
#include<vector>
#include<cassert>
#include "math/zeta_mobius.hpp"
template<typename T>
std::vector<T>gcd_convolution(std::vector<T>a,std::vector<T>b){
  assert(a.size()==b.size());
  multiplier_zeta(a),multiplier_zeta(b);
  for(int i=0;i<a.size();i++)a[i]*=b[i];
  multiplier_mobius(a);
  return a;
}