#pragma once
#include<vector>
#include<cassert>
#include "math/zeta_mobius.hpp"
template<typename T>
std::vector<T>lcm_convolution(std::vector<T>a,std::vector<T>b){
  assert(a.size()==b.size());
  divisor_zeta(a),divisor_zeta(b);
  for(int i=0;i<a.size();i++)a[i]*=b[i];
  divisor_mobius(a);
  return a;
}