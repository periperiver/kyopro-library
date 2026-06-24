#define PROBLEM "https://judge.yosupo.jp/problem/eulerian_trail_directed"
#include "graph/eulerian_trail.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    int n,m;
    std::cin>>n>>m;
    Graph g(n,true);
    g.read<0,0>(m);
    if(m==0){
      std::cout<<"Yes\n0\n\n";
      continue;
    }
    auto ans=eulerian_trail(g);
    if(ans.empty())std::cout<<"No\n";
    else{
      std::cout<<"Yes\n";
      for(int i=0;i<m;i++)std::cout<<ans[i].from<<' ';
      std::cout<<ans.back().to<<'\n';
      for(int i=0;i<m;i++)std::cout<<ans[i].index<<" \n"[i+1==m];
    }
  }
}