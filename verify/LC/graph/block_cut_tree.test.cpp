#define PROBLEM "https://judge.yosupo.jp/problem/biconnected_components"
#include "graph/block_cut_tree.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  Graph g(n,false);
  g.read<0,0>(m);
  auto bct=block_cut_tree(g);
  std::cout<<bct.size()-n<<'\n';
  for(int i=n;i<bct.size();i++){
    std::cout<<bct[i].size();
    for(auto e:bct[i])std::cout<<' '<<e.to;
    std::cout<<'\n';
  }
}