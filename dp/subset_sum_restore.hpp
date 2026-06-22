#pragma once
#include<vector>
#include<type_traits>
#include<optional>
#include "poly/fps.hpp"
#include "other/type.hpp"
template<typename T,std::enable_if_t<is_static_modint_v<T>,std::nullptr_t> =nullptr>
std::optional<std::vector<int>>subset_sum_restore(const std::vector<int>&v){
  std::vector<int>res(v.size());
  std::vector<T>f(v.size());
  std::vector<T>inv(v.size());
  inv[1]=1;
  for(int i=2;i<v.size();i++)inv[i]=-inv[T::mod()%i]*(T::mod()/i);
  for(int i=1;i<v.size();i++){
    for(int j=1;i*j<v.size();j++){
      T now=inv[j]*v[i];
      if(!(j&1))now=-now;
      f[i*j]+=now;
    }
  }
  f=fps_exp(f);
  if(f.back().val()==0)return std::nullopt;
  auto dfs=[&](auto self,int l,int r,int target)->void {
    if(target==0)return;
    if(l+1==r){
      res[l]=target/l;
      return;
    }
    int mid=(l+r)/2;
    std::vector<T>left(target+1),right(target+1);
    for(int i=l;i<mid;i++){
      for(int j=1;i*j<=target;j++){
        T now=inv[j]*v[i];
        if(!(j&1))now=-now;
        left[i*j]+=now;
      }
    }
    for(int i=mid;i<r;i++){
      for(int j=1;i*j<=target;j++){
        T now=inv[j]*v[i];
        if(!(j&1))now=-now;
        right[i*j]+=now;
      }
    }
    left=fps_exp(left),right=fps_exp(right);
    for(int i=0;i<=target;i++){
      if(left[i].val()>0&&right[target-i].val()>0){
        self(self,l,mid,i);
        self(self,mid,r,target-i);
        return;
      }
    }
    exit(0715);
  };
  dfs(dfs,1,v.size(),v.size()-1);
  return std::make_optional(res);
}