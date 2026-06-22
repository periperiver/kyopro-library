#pragma once
constexpr bool is_prime_constexpr(int n){
  if(n==998244353||n==1000000007)return true;
  for(int i=2;i*i<=n;i++)if(n%i==0)return false;
  return true;
}