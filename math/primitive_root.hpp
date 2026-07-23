#pragma once
#include<vector>
#include<algorithm>
#include "../random/generator.hpp"
#include "factorize.hpp"
#include "arbitrary_modint.hpp"
template<std::integral T>
bool is_primitive_root(T n,T g_,const std::vector<std::pair<T,int>>&n_f){
  using mint=arbitrary_modint<T,20260630>;
  mint::set_mod(n);
  mint g=g_;
  for(const auto&[p,e]:n_f)if(g.pow((n-1)/p).val()==1)return false;
  return true;
}
template<std::integral T>
T primitive_root(T n,const std::vector<std::pair<T,int>>&n_f){
  T res;
  do res=Random::range<T>(1,n);while(!is_primitive_root<T>(n,res,n_f));
  return res;
}
template<std::integral T>
inline T primitive_root(T n){
  return primitive_root<T>(n,factorize(n-1));
}