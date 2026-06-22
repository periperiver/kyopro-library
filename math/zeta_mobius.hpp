#pragma once
#include<vector>
#include "prime_sieve.hpp"
template<typename T>
void divisor_zeta(std::vector<T>&a){
  int n=a.size()-1;
  std::vector<int>p=prime_sieve(n);
  for(int i:p)for(int j=1;j<=n/i;j++)a[i*j]+=a[j];
}
template<typename T>
void divisor_mobius(std::vector<T>&a){
  int n=a.size()-1;
  std::vector<int>p=prime_sieve(n);
  for(int i:p)for(int j=n/i;j>=1;j--)a[i*j]-=a[j];
}
template<typename T>
void multiplier_zeta(std::vector<T>&a){
  int n=a.size()-1;
  std::vector<int>p=prime_sieve(n);
  for(int i:p)for(int j=n/i;j>=1;j--)a[j]+=a[i*j];
}
template<typename T>
void multiplier_mobius(std::vector<T>&a){
  int n=a.size()-1;
  std::vector<int>p=prime_sieve(n);
  for(int i:p)for(int j=1;j<=n/i;j++)a[j]-=a[i*j];
}