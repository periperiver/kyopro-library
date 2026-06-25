#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "graph/offline_dynamic_mst.hpp"
#include "random/generator.hpp"
#include "datastructure/unionfind.hpp"
std::vector<long long>naive(int n,int q,std::vector<MSTEdge<long long>>edge){
  static constexpr long long inf=std::numeric_limits<long long>::max();
  std::vector<long long>res(q);
  for(int t=0;t<q;t++){
    UnionFind uf(n);
    std::vector<std::tuple<long long,int,int>>now;
    long long w=0;
    for(int j=0;j<(int)edge.size();j++){
      if(edge[j].l<=t&&t<edge[j].r)now.emplace_back(edge[j].weight,edge[j].u,edge[j].v);
    }
    std::sort(now.begin(),now.end());
    for(auto [c,u,v]:now)if(uf.merge(u,v))w+=c;
    if(uf.size()==1)res[t]=w;
    else res[t]=inf;
  }
  return res;
}
void test(int n,int m,int q){
  std::vector<MSTEdge<long long>>edge(m);
  for(int i=0;i<m;i++){
    auto [l,r]=Random::distinct(q+1);
    int u=Random::range(n);
    int v=Random::range(n);
    long long w=Random::range(1ll<<31);
    edge[i]=MSTEdge<long long>(l,r,u,v,w);
  }
  assert(offline_dynamic_mst(n,q,edge)==naive(n,q,edge));
}
int main(){
  for(int i=0;i<20;i++){
    int n=Random::range(1,1000);
    int m=Random::range(1,1000);
    int q=Random::range(1,1000);
    test(n,m,q);
  }
  for(int i=0;i<20;i++){
    int n=Random::range(1,50);
    int m=10000;
    int q=Random::range(1,100);
    test(n,m,q);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}