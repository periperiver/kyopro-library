#define PROBLEM "https://judge.yosupo.jp/problem/minimum_spanning_tree"
#include "graph/mst.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  Graph g(n,false);
  g.read<1,0>(m);
  auto ans=minimum_spanning_tree(g);
  int sum=0;
  for(auto e:ans)sum+=e.weight;
  std::cout<<sum<<'\n';
  for(int i=0;i<n-1;i++)std::cout<<ans.get_edge(i).index<<" \n"[i+1==n-1];
}