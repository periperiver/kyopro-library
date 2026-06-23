#define PROBLEM "https://judge.yosupo.jp/problem/connected_components_of_complement_graph"
#include "graph/complement_connected_component.hpp"
#include "datastructure/csr_array.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  Graph g(n,false);
  g.read<0,0>(m);
  auto cc=complement_connected_component(g);
  csr_array<int>ans(*std::max_element(cc.begin(),cc.end())+1,cc);
  std::cout<<ans.size()<<'\n';
  for(int i=0;i<ans.size();i++){
    std::cout<<ans[i].size();
    for(int j:ans[i])std::cout<<' '<<j;
    std::cout<<'\n';
  }
}