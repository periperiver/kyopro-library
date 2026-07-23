#pragma once
#include<numeric>
#include<algorithm>
#include<type_traits>
#include "factorize.hpp"
#include "carmichael_constexpr.hpp"
template<typename T>
std::enable_if_t<std::is_integral_v<T>,T>carmichael(T n){
  T res=1;
  for(auto [p,e]:factorize(n)){
    if(p==2){
      if(e==2)res=2;
      else if(e>=3)res=T(1)<<(e-2);
    }
    else{
      T prod=p-1;
      for(int i=0;i<e-1;i++)prod*=p;
      res=std::lcm(res,prod);
    }
  }
  return res;
}