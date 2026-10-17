#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2230"
#include "graph/mct.hpp"
#include<iostream>
#include<vector>
#include<algorithm>
constexpr long long inf=1e9;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<std::tuple<int,int,int>>edge(m);
  for(auto &[u,v,w]:edge)std::cin>>u>>v>>w;
  std::sort(edge.begin(),edge.end());
  std::vector<int>dst(n);
  for(auto [u,v,w]:edge)dst[v]=std::max(dst[v],dst[u]+w);
  MinCostTension<long long,long long>mct(n,inf);
  mct.add_cost(0,n-1,inf,dst[n-1]);
  long long ans=0;
  for(int i=1;i<n-1;i++){
    mct.less_equal(0,i);
    mct.less_equal(i,n-1);
  }
  for(auto [u,v,w]:edge){
    ans+=dst[n-1]-w;
    mct.add_cost(v,u,inf,-w);
    mct.add_cost(v,u,1,-dst[n-1]);
  }
  ans-=*mct.solve();
  std::cout<<ans<<std::endl;
}