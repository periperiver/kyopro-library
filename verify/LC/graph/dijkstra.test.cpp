#define PROBLEM "https://judge.yosupo.jp/problem/shortest_path"
#include "fastio.hpp"
#include "graph/dijkstra.hpp"
int main(){
  int n,m,s,t;
  rd(n),rd(m),rd(s),rd(t);
  Graph<long long>g(n,true);
  g.reserve(m);
  for(int i=0;i<m;i++){
    int a,b,c;
    rd(a),rd(b),rd(c);
    g.add_edge(a,b,c);
  }
  g.build();
  auto [dst,pre]=dijkstra(g,s);
  if(dst[t]>1e18)wt("-1\n");
  else{
    wt(dst[t]),wt(' ');
    std::vector<int>ans;
    while(s!=t){
      ans.push_back(t);
      t=pre[t];
    }
    ans.push_back(s);
    std::reverse(ans.begin(),ans.end());
    wt(ans.size()-1),wt('\n');
    for(int i=0;i<(int)ans.size()-1;i++)wt(ans[i]),wt(' '),wt(ans[i+1]),wt('\n');
  }
}