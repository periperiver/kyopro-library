#pragma once
#include "generator.hpp"
#include "../math/primality_test.hpp"
namespace Random{
template<typename T>
inline std::enable_if_t<std::is_integral_v<T>,T>prime(T l,T r){
  T res=0;
  while(!isprime(res))res=range(l,r);
  return res;
}
}