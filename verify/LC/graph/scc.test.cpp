#define PROBLEM "https://judge.yosupo.jp/problem/scc"
#include "graph/scc.hpp"
#include "datastructure/csr_array.hpp"
#include<algorithm>
int main(){
  int n,m;
  std::cin>>n>>m;
  Graph g(n,true);
  g.read<0,0>(m);
  auto scc=strongly_connected_components(g);
  csr_array<int>ans(*std::max_element(scc.begin(),scc.end())+1,scc);
  std::cout<<ans.size()<<std::endl;
  for(int i=0;i<(int)ans.size();i++){
    std::cout<<ans[i].size();
    for(int j:ans[i])std::cout<<' '<<j;
    std::cout<<'\n';
  }
}