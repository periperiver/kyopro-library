#pragma once
#include "count_connected_subgraph.hpp"
template<typename T>
std::vector<T>count_biconnected_subgraph(const Graph<>&g){
  int n=g.size();
  std::vector<T>dp=count_connected_subgraph<T>(g);
  for(int i=0;i<n;i++){
    std::vector<T>f(1<<(n-1));
    for(int j=0;j<(1<<i);j++){
      for(int k=0;k<(1<<(n-i-1));k++){
        f[j|(k<<i)]=dp[j|(1<<i)|(k<<(i+1))];
      }
    }
    f=sps_log(f);
    for(int j=0;j<(1<<i);j++){
      for(int k=0;k<(1<<(n-i-1));k++){
        dp[j|(1<<i)|(k<<(i+1))]=f[j|(k<<i)];
      }
    }
  }
  return dp;
}