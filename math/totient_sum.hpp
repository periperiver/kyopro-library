#pragma once
#include<cmath>
#include<numeric>
#include "prime_sieve.hpp"
#include "quotients.hpp"
#include "util.hpp"
#include "../other/type.hpp"
template<typename T>
std::vector<T> totient_sum(unsigned long long n){
  int s=std::pow(n/std::max(1,msb(std::max(1,msb(n)))),(double)2/3);
  std::vector<int>tot(s+1);
  std::iota(tot.begin(),tot.end(),0);
  for(int p:prime_sieve(s)){
    for(int i=p;i<=s;i+=p)tot[i]=tot[i]/p*(p-1);
  }
  Quotients q(n);
  std::vector<T>res(q.size());
  res[0]=1;
  for(int i=1;i<q.size();i++){
    if(q[i]<=s){
      res[i]=res[i-1];
      int l=q[i-1],r=q[i];
      for(int j=l+1;j<=r;j++){
        if constexpr(is_modint_v<T>)res[i]+=T::raw(tot[j]);
        else res[i]+=tot[j];
      }
    }
    else{
      unsigned long long key=q[i];
      int sq=std::sqrt(key);
      res[i]=T((key+1)/2)*(key|1);
      for(int s=2;s<=sq;s++){
        res[i]-=res[q.get_key(key/s)];
      }
      if(key/sq==sq)sq--;
      for(int s=1;s<=sq;s++){
        res[i]-=res[q.get_key(s)]*(key/s-key/(s+1));
      }
    }
  }
  return res;
}