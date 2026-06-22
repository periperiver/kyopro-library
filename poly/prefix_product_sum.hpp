#pragma once
#include "convolution/ntt.hpp"
//pair(prod_all(f),prefix_prod_sum(f))
template<typename T>
std::pair<std::vector<T>,std::vector<T>>prefix_product_sum(const std::vector<std::vector<T>>&f){
  auto dfs=[&](auto self,int l,int r)->std::pair<std::vector<T>,std::vector<T>> {
    if(l+1==r)return std::make_pair(f[l],f[l]);
    int mid=(l+r)/2;
    auto [l_all,l_pref]=self(self,l,mid);
    auto [r_all,r_pref]=self(self,mid,r);
    r_all=ntt_convolution(l_all,r_all);
    r_pref=ntt_convolution(l_all,r_pref);
    for(int i=0;i<(int)l_pref.size();i++)r_pref[i]+=l_pref[i];
    return std::make_pair(std::move(r_all),std::move(r_pref));
  };
  return dfs(dfs,0,f.size());
}