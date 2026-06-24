#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum"
#include<iostream>
#include<map>
#include "graph/offline_dynamic_connectivity.hpp"
#include "monoid/add.hpp"
int main(){
  int n,q;
  std::cin>>n>>q;
  std::vector<long long>a(n);
  for(int i=0;i<n;i++)std::cin>>a[i];
  OfflineDynamicConnectivity<MonoidAdd<long long>>odc(n,q);
  std::map<std::pair<int,int>,int>mp;
  std::vector<std::pair<int,int>>query(q,{-1,-1});
  for(int i=0;i<q;i++){
    int t;
    std::cin>>t;
    if(t==0){
      int u,v;
      std::cin>>u>>v;
      if(u>v)std::swap(u,v);
      mp[{u,v}]=i;
    }
    else if(t==1){
      int u,v;
      std::cin>>u>>v;
      if(u>v)std::swap(u,v);
      auto itr=mp.find({u,v});
      odc.add_edge(itr->second,i,u,v);
      mp.erase(itr);
    }
    else if(t==2){
      int v,x;
      std::cin>>v>>x;
      query[i]={v,x};
    }
    else{
      int v;
      std::cin>>v;
      query[i]={v,-1};
    }
  }
  for(auto [uv,i]:mp)odc.add_edge(i,q,uv.first,uv.second);
  for(int i=0;i<n;i++)odc.modify(i,a[i]);
  odc.build([&](int i){
    auto [a,b]=query[i];
    if(a==-1)return;
    if(b==-1)std::cout<<odc.prod(a)<<'\n';
    else odc.modify(a,b);
  });
}