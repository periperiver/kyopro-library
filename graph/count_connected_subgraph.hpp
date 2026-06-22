#pragma once
#include "graph_base.hpp"
#include "../poly/sps.hpp"
template<typename T>
std::vector<T>count_connected_subgraph(const Graph<>&g){
  int n=g.size();
  std::vector<int>cnt(1<<n);
  for(const Edge<>&e:g)cnt[(1<<e.from)|(1<<e.to)]++;
  for(int i=0;i<n;i++)for(int j=0;j<(1<<n);j++)if(j>>i&1)cnt[j]+=cnt[j^(1<<i)];
  std::vector<T>pow2(g.edge_size()+1);
  pow2[0]=1;
  for(int i=1;i<=g.edge_size();i++)pow2[i]=pow2[i-1]+pow2[i-1];
  std::vector<T>dp(1<<n);
  for(int i=0;i<(1<<n);i++)dp[i]=pow2[cnt[i]];
  return sps_log(dp);
}