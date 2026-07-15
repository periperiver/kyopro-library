#define PROBLEM "https://yukicoder.me/problems/no/1776"
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
  std::vector<int>ans=shortest_path<int,true>(g,x,x,{x,y,z});
  if(ans.empty())std::cout<<"-1\n";
  else{
    std::cout<<ans.size()-1<<'\n';
    for(int i=0;i<(int)ans.size();i++)std::cout<<ans[i]+1<<" \n"[i+1==(int)ans.size()];
  }
}