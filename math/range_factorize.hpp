#pragma once
#include<cmath>
#include<numeric>
#include<cassert>
#include "../datastructure/csr_array.hpp"
#include "prime_sieve.hpp"
//[l,r)
csr_array<long long>range_factorize(long long l,long long r){
  assert(1<=l);
  std::vector<long long>f(r-l);
  std::iota(f.begin(),f.end(),l);
  std::vector<std::pair<int,long long>>c;
  for(int p:prime_sieve(std::sqrt(r-1))){
    for(long long x=(l+p-1)/p;p*x<r;x++){
      while(f[x*p-l]%p==0){
        c.emplace_back(x*p-l,p);
        f[x*p-l]/=p;
      }
    }
  }
  for(int i=0;i<r-l;i++)if(f[i]!=1)c.emplace_back(i,f[i]);
  return csr_array<long long>(r-l,c);
}