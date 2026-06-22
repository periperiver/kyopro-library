#pragma once
#include "graph_base.hpp"
#include "../datastructure/veb_tree.hpp"
#include<queue>
#include<algorithm>
template<typename T>
std::vector<int>complement_connected_component(Graph<T>g){
  int n=g.size();
  VanEmdeBoasTree veb(n);
  for(int i=0;i<n;i++)veb.insert(i);
  std::vector<int>res(n,-1);
  for(int i=0,j=0;i<n;i++)if(res[i]==-1){
    std::queue<int>que;
    veb.erase(i);
    que.push(i);
    res[i]=j;
    while(!que.empty()){
      int x=que.front();que.pop();
      std::sort(g[x].begin(),g[x].end(),[](const Edge<T>&lhs,const Edge<T>&rhs){return lhs.to<rhs.to;});
      int l=0;
      for(const Edge<T>&e:g[x]){
        while(true){
          l=veb.successor(l);
          if(l>=e.to)break;
          veb.erase(l);
          que.push(l);
          res[l]=j;
        }
        l=e.to+1;
      }
      while(true){
        l=veb.successor(l);
        if(l>=n)break;
        veb.erase(l);
        que.push(l);
        res[l]=j;
      }
    }
    j++;
  }
  return res;
}