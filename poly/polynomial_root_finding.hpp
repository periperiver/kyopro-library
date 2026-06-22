#pragma once
#include "half_gcd.hpp"
#include "../random/generator.hpp"
namespace polynomial_root_finding_impl{
template<typename T>
std::vector<T>pow_mod(std::vector<T>f,std::vector<T>mod,long long e){
  std::vector<T>res{1};
  while(e){
    if(e&1){
      res=ntt_convolution(res,f);
      res=poly_div_mod(res,mod).second;
    }
    f=ntt_convolution(f,f);
    f=poly_div_mod(f,mod).second;
    e>>=1;
  }
  return res;
}
template<typename T>
std::vector<T>polynomial_root_finding(std::vector<T>f){
  half_gcd_impl::shrink(f);
  if((int)f.size()<2)return{};
  std::vector<T>g=pow_mod(std::vector<T>{0,1},f,T::mod());
  if((int)g.size()<2){
    g.resize(2);
    g[1]--;
    g=poly_div_mod(g,f).second;
  }
  else g[1]--;
  f=half_gcd<T,false>(f,g);
  std::vector<T>res;
  auto dfs=[&res](auto self,std::vector<T>f)->void {
    if((int)f.size()<=2){
      if((int)f.size()==2)res.push_back(-f[0]/f[1]);
      return;
    }
    std::vector<T>g(f.size()-1);
    for(int i=0;i<(int)g.size();i++)g[i]=Random::range(T::mod());
    g=pow_mod(g,f,(T::mod()-1)/2);
    g[0]--;
    std::vector<T>left=half_gcd<T,false>(f,g);
    std::vector<T>right=poly_div_mod(f,left).first;
    self(self,std::move(left));
    self(self,std::move(right));
  };
  dfs(dfs,std::move(f));
  std::sort(res.begin(),res.end());
  return res;
}
}
using polynomial_root_finding_impl::polynomial_root_finding;