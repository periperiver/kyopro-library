#pragma once
#include<vector>
#include<cassert>
#include "graph_base.hpp"
#include "math/util.hpp"
template<typename T>
int chromatic_number(const Graph<T>&g){
  assert(!g.is_directed());
  int n=g.size();
  using u64=unsigned long long;
  std::vector<int>mask(n);
  int ec=g.edge_size();
  for(int i=0;i<ec;i++){
    const Edge<T>&e=g.get_edge(i);
    mask[e.from]|=1<<e.to;
    mask[e.to]|=1<<e.from;
  }
  std::vector<u64>dp(1<<n),coef(1<<n);
  for(int i=0;i<(1<<n);i++)coef[i]=(n-__builtin_popcount(i))&1?-1:1;
  dp[0]=1;
  for(int i=1;i<(1<<n);i++){
    dp[i]=dp[i^(i&-i)]+dp[(i^(i&-i))&~mask[lsb(i)]];
  }
  for(int k=1;k<n;k++){
    u64 sum=0;
    for(int i=0;i<(1<<n);i++){
      coef[i]*=dp[i];
      sum+=coef[i];
    }
    if(sum)return k;
  }
  return n;
}