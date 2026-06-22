#pragma once
#include<type_traits>
#include<numeric>
template<typename T>
constexpr std::enable_if_t<(std::numeric_limits<T>::digits<=32),T>pow_mod(T a,T n,T mod){
  using u64=unsigned long long;
  u64 res=1;
  while(n>0){
    if(n&1)res=((u64)res*a)%mod;
    a=((u64)a*a)%mod;
    n>>=1;
  }
  return T(res);
}
template<typename T>
constexpr std::enable_if_t<(std::numeric_limits<T>::digits>32),T>pow_mod(T a,T n,T mod){
  using u128=__uint128_t;
  u128 res=1;
  while(n>0){
    if(n&1)res=((u128)res*a)%mod;
    a=((u128)a*a)%mod;
    n>>=1;
  }
  return T(res);
}