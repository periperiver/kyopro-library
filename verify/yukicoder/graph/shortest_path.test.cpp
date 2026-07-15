#define PROBLEM "https://yukicoder.me/problems/no/1775"
#include "graph/shortest_path.hpp"
int main(){
  int n,m,x,y,z;
  std::cin>>n>>m>>x>>y>>z;
  x--,y--,z--;
  std::vector<std::vector<bool>>ban(n,std::vector<bool>(n));
  for(int i=0;i<m;i++){
    int a,b;
    std::cin>>a>>b;
    a--,b--;
    ban[a][b]=ban[b][a]=true;
  }
  Graph g(n,false);
  for(int i=0;i<n;i++)for(int j=i+1;j<n;j++)if(!ban[i][j]){
    g.add_edge(i,j);
  }
  g.build();
  int ans=shortest_path(g,x,x,{x,y,z});
  if(ans<=n)std::cout<<ans<<'\n';
  else std::cout<<"-1\n";
}