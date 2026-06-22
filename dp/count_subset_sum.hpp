#pragma once
#include<cassert>
#include<type_traits>
#include "../poly/fps_exp.hpp"
#include "../other/type.hpp"
#include "../math/factorial.hpp"
template<typename T,std::enable_if_t<is_static_modint_v<T>,std::nullptr_t> =nullptr>
std::vector<T>count_subset_sum(const std::vector<T>&v){
  if(v.empty())return v;
  assert(v[0].val()==0);
  if(v.size()==1)return std::vector<T>{1};
  std::vector<T>f(v.size());
  for(int i=1;i<std::ssize(v);i++){
    for(int j=1;i*j<std::ssize(v);j++){
      T now=v[i]*F<T>::inverse(j);
      if(!(j&1))now=-now;
      f[i*j]+=now;
    }
  }
  return fps_exp(f);
}