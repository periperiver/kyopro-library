#pragma once
#include<vector>
#include "prime_power_binomial.hpp"
#include "crt_restore.hpp"
struct ArbtraryBinomial{
private:
  CRT<int> crt;
  std::vector<PrimePowerBinomial>binom;
public:
  ArbtraryBinomial(int mod):crt(mod){
    binom.resize(crt.f.size());
    for(int i=0;i<(int)binom.size();i++)binom[i]=PrimePowerBinomial(crt.f[i].first,crt.f[i].second);
  }
  int operator()(long long n,long long k){
    std::vector<int>vals(binom.size());
    for(int i=0;i<(int)binom.size();i++)vals[i]=binom[i](n,k);
    return crt(vals);
  }
};