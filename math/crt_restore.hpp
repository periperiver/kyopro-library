#pragma once
#include<vector>
#include<cassert>
#include<type_traits>
#include "primefactor.hpp"
#include "pow_mod.hpp"
template<std::signed_integral T>
struct CRT{
  std::vector<std::pair<T,int>>f;
  std::vector<T>pe;
  std::vector<T>invs;
  using T2=std::conditional_t<(std::numeric_limits<T>::digits<=32),int64_t,__int128_t>;
  CRT(){}
  CRT(T n):f(factorize(n)){
    pe.resize(f.size());
    for(int i=0;i<(int)f.size();i++){
      pe[i]=1;
      for(int j=0;j<f[i].second;j++)pe[i]*=f[i].first;
    }
    invs.resize(f.size());
    T prod=1;
    for(int i=0;i<(int)f.size();i++){
      invs[i]=pow_mod(prod%pe[i],pe[i]/f[i].first*(f[i].first-1)-1,pe[i]);
      prod*=pe[i];
    }
  }
  T operator()(std::vector<T>v){
    assert(v.size()==pe.size());
    T res=0,prod=1;
    for(int i=0;i<(int)pe.size();i++){
      v[i]%=pe[i];
      if(v[i]<0)v[i]+=pe[i];
      T x=T2(v[i]-res)*T2(invs[i])%pe[i];
      res+=x*prod;
      prod*=pe[i];
      if(res<0)res+=prod;
    }
    return res;
  }
};