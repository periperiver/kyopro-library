#pragma once
#include "../math/util.hpp"
#include<vector>
#include<numeric>
#include<algorithm>
std::vector<int>mo_order(int n,const std::vector<std::pair<int,int>>&query){
  int z=ceil_pow2(n+1);
  auto hilbert=[&](int x,int y)->long long {
    long long rx,ry,res=0;
    for(long long s=z>>1;s;s>>=1){
      rx=(x&s)>0,ry=(y&s)>0;
      res+=s*s*((rx*3)^ry);
      if(ry)continue;
      if(rx){
        x=z-1-x;
        y=z-1-y;
      }
      std::swap(x,y);
    }
    return res;
  };
  std::vector<long long>h(query.size());
  for(int i=0;i<(int)query.size();i++)h[i]=hilbert(query[i].first,query[i].second);
  std::vector<int>ord(query.size());
  std::iota(ord.begin(),ord.end(),0);
  std::sort(ord.begin(),ord.end(),[&](int lhs,int rhs){return h[lhs]<h[rhs];});
  return ord;
}