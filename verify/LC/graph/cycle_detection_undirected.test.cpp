#define PROBLEM "https://judge.yosupo.jp/problem/cycle_detection_undirected"
#include "graph/cycle_detection.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  Graph g(n,false);
  g.read<0,0>(m);
  auto ans=cycle_detection(g);
  if(ans.empty())std::cout<<"-1\n";
  else{
    std::cout<<ans.size()<<std::endl;
    for(int i=0;i<ans.size();i++)std::cout<<ans[i].from<<" \n"[i+1==(int)ans.size()];
    for(int i=0;i<ans.size();i++)std::cout<<ans[i].index<<" \n"[i+1==(int)ans.size()];
  }
}