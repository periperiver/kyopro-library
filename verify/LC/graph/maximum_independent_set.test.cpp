#define PROBLEM "https://judge.yosupo.jp/problem/maximum_independent_set"
#include "graph/maximum_independent_set.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  Graph g(n,false);
  g.read<0,0>(m);
  auto ans=maximum_independent_set(g);
  std::cout<<ans.size()<<'\n';
  for(int i=0;i<(int)ans.size();i++)std::cout<<ans[i]<<" \n"[i+1==(int)ans.size()];
}