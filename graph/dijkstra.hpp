#pragma once
#include<type_traits>
#include<queue>
#include "graph_base.hpp"
#include "datastructure/radix_heap.hpp"
template<typename T,std::enable_if_t<std::is_integral_v<T>,std::nullptr_t> =nullptr>
std::pair<std::vector<T>,std::vector<int>>dijkstra(const Graph<T>&g,int s=0){
  int n=g.size();
  std::vector<T>dst(n,std::numeric_limits<T>::max());
  std::vector<int>pre(n,-1);
  dst[s]=0;
  radix_heap<T>heap(n);
  heap.decrease_key(s,0);
  while(!heap.empty()){
    int x=heap.pop();
    for(const Edge<T>&e:g[x]){
      if(dst[e.to]>dst[e.from]+e.weight){
        pre[e.to]=e.from;
        dst[e.to]=dst[e.from]+e.weight;
        heap.decrease_key(e.to,dst[e.to]);
      }
    }
  }
  return std::make_pair(dst,pre);
}
template<typename T,std::enable_if_t<std::is_floating_point_v<T>,std::nullptr_t> =nullptr>
std::pair<std::vector<T>,std::vector<int>>dijkstra(const Graph<T>&g,int s=0){
  int n=g.size();
  std::vector<T>dst(n,std::numeric_limits<T>::max());
  std::vector<int>pre(n,-1);
  std::priority_queue<std::pair<T,int>,std::vector<std::pair<T,int>>,std::greater<std::pair<T,int>>>que;
  dst[s]=0;
  que.push({std::make_pair(0,s)});
  while(!que.empty()){
    auto [d,x]=que.top();
    que.pop();
    if(dst[x]!=d)continue;
    for(const Edge<T>&e:g[x]){
      T nxt=d+e.weight;
      if(dst[e.to]>nxt){
        pre[e.to]=x;
        dst[e.to]=nxt;
        que.push(std::make_pair(dst[e.to],e.to));
      }
    }
  }
  return std::make_pair(dst,pre);
}