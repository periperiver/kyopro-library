#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<set>
#include<cassert>
#include "graph/maximum_independent_set.hpp"
#include "random/generator.hpp"
int naive(Graph<> g){
  int n=g.size();
  std::vector<std::vector<bool>>ng(n,std::vector<bool>(n));
  for(auto e:g)ng[e.from][e.to]=ng[e.to][e.from]=true;
  int res=0;
  for(int i=1;i<(1<<n);i++){
    bool ok=true;
    for(int j=0;j<n;j++)if(i>>j&1){
      for(int k=j+1;k<n;k++)if(i>>k&1){
        if(ng[j][k]){
          ok=false;
          j=k=n;
          break;
        }
      }
    }
    if(ok)res=std::max(res,(int)__builtin_popcount(i));
  }
  return res;
}
bool check(Graph<>g,std::vector<int>mis){
  std::vector<bool>ind(g.size());
  for(int x:mis){
    if(ind[x])return false;
    ind[x]=true;
  }
  for(auto e:g)if(ind[e.from]&&ind[e.to])return false;
  return true;
}
void test(int n){
  int m=Random::range(n*(n-1)/2+1);
  std::set<std::pair<int,int>>s;
  Graph g(n,false);
  while((int)s.size()<m){
    auto [u,v]=Random::distinct(n);
    s.emplace(u,v);
    g.add_edge(u,v);
  }
  g.build();
  std::vector<int>ans=maximum_independent_set(g);
  assert(check(g,ans));
  if(n<=20){
    assert((int)ans.size()==naive(g));
  }
}
int main(){
  for(int i=0;i<1000;i++)test(Random::range(1,20));
  for(int i=0;i<10;i++)test(Random::range(20,120));
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}