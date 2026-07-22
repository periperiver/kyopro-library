#pragma once
#include<vector>
#include "../math/modint.hpp"
#include "../math/primitive_root.hpp"
struct RollingHashBase{
  static mint61 base;
  static std::vector<mint61>power;
  static void resize(int n){
    if((int)power.size()<=n){
      int pre=power.size();
      power.resize(n+1);
      for(int i=pre;i<=n;i++)power[i]=power[i-1]*base;
    }
  }
  template<typename LHS,typename RHS>
  static int lcp(const LHS&lhs,int l1,int r1,const RHS&rhs,int l2,int r2){
    int ok=0,ng=std::min(r1-l1,r2-l2)+1;
    while(ng-ok>1){
      int mid=(ok+ng)/2;
      (lhs.get(l1,l1+mid)==rhs.get(l2,l2+mid)?ok:ng)=mid;
    }
    return ok;
  }
};
std::vector<mint61> RollingHashBase::power{1};
mint61 RollingHashBase::base=primitive_root(mint61::mod());