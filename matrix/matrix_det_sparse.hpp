#pragma once
#include "black_box.hpp"
template<typename T>
T matrix_det_sparse(int n,std::vector<std::tuple<int,int,T>>mat){
  std::vector<T>d(n);
  T prod=1;
  for(int i=0;i<n;i++){
    d[i]=Random::range(1ll,(long long)T::mod());
    prod*=d[i];
  }
  for(auto &[i,j,v]:mat)v*=d[j];
  auto mi=black_box_matrix_minimal_polynomial<T>(n,[&](const std::vector<T>&b)->std::vector<T> {
    std::vector<T>res(n);
    for(const auto&[i,j,v]:mat)res[i]+=v*b[j];
    return res;
  });
  T res=mi.back();
  if(n&1)res=-res;
  return res/prod;
}