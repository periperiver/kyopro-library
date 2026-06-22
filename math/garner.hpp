#pragma once
#include<vector>
#include<utility>
#include "ext_gcd.hpp"
int garner(std::vector<std::pair<int,int>>r,int mod){
  r.emplace_back(0,mod);
  std::vector<long long>a(r.size(),1),b(r.size(),0);
  for(int i=0;i<r.size()-1;i++){
    long long v=(r[i].first-b[i])*inv_mod(a[i],r[i].second)%r[i].second;
    if(v<0)v+=r[i].second;
    for(int j=i+1;j<r.size();j++){
      b[j]+=a[j]*v;
      b[j]%=r[j].second;
      a[j]*=r[i].second;
      a[j]%=r[j].second;
    }
  }
  return b[r.size()-1];
}