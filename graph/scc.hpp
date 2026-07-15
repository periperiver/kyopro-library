#pragma once
#include<vector>
#include "datastructure/fast_stack.hpp"
#include "graph_base.hpp"
template<typename T>
std::vector<int>strongly_connected_components(Graph<T>g){
  assert(g.is_directed());
  int n=g.size();
  std::vector<bool>seen(n,false);
  fast_stack<int>st(g.edge_size()+1);
  std::vector<int>vis;
  vis.reserve(n);
  for(int i=0;i<n;i++)if(!seen[i]){
    st.push(i);
    while(!st.empty()){
      int x=st.pop();
      if(x<0){
        vis.push_back(~x);
        continue;
      }
      if(seen[x])continue;
      seen[x]=true;
      st.push(~x);
      for(const Edge<T>&e:g[x])if(!seen[e.to])st.push(e.to);
    }
  }
  std::vector<int>scc(n,-1);
  g.reverse();
  int sp=0;
  for(int i=n-1;i>=0;i--)if(scc[vis[i]]==-1){
    st.push(vis[i]);
    while(!st.empty()){
      int x=st.pop();
      scc[x]=sp;
      for(const Edge<T>&e:g[x])if(scc[e.to]==-1)st.push(e.to);
    }
    sp++;
  }
  return scc;
}