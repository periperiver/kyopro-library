#pragma once
#include<vector>
#include "prime_power_binomial.hpp"
#include "crt.hpp"
struct ArbtraryBinomial{
private:
  int mod;
  std::vector<int>mods;
  std::vector<PrimePowerBinomial>binom;
public:
  ArbtraryBinomial(int mod_):mod(mod_){
    for(int i=2;i*i<=mod_;i++){
      if(mod_%i==0){
        int now=1,e=0;
        while(mod_%i==0)mod_/=i,now*=i,e++;
        mods.push_back(now);
        binom.push_back(PrimePowerBinomial(i,e));
      }
    }
    if(mod_!=1){
      mods.push_back(mod_);
      binom.push_back(PrimePowerBinomial(mod_,1));
    }
  }
  int operator()(long long n,long long k){
    if(mod==1)return 0;
    std::vector<std::pair<long long,long long>>a(mods.size());
    for(int i=0;i<mods.size();i++)a[i]=std::make_pair(binom[i](n,k),mods[i]);
    return crt(a).first;
  }
};