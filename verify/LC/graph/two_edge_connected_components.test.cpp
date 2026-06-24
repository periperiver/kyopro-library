#define PROBLEM "https://judge.yosupo.jp/problem/two_edge_connected_components"
#include "graph/two_edge_connected_components.hpp"
#include "datastructure/csr_array.hpp"
#include<algorithm>
int main(){
  int n,m;
  std::cin>>n>>m;
  Graph g(n,false);
  g.read<0,0>(m);
  auto tecc=two_edge_connected_components(g);
  csr_array<int>ans(*std::max_element(tecc.begin(),tecc.end())+1,tecc);
  std::cout<<ans.size()<<'\n';
  for(int i=0;i<ans.size();i++){
    std::cout<<ans[i].size();
    for(int j:ans[i])std::cout<<' '<<j;
    std::cout<<'\n';
  }
}