#pragma once
#include<queue>
#include "graph_base.hpp"
#include "graph/dijkstra.hpp"
#include "datastructure/fast_stack.hpp"
#include "datastructure/persistent/meldable_heap.hpp"
template<typename T>
std::vector<T>k_shortest_walk(const Graph<T>&g,int s,int t,int k){
  assert(g.is_directed());
  static constexpr T inf=std::numeric_limits<T>::max();
  int n=g.size();
  Graph<T>revg=g;
  revg.reverse();
  auto [dst,pre]=dijkstra(revg,t);
  if(dst[s]==inf)return std::vector<T>(k,inf);
  PersistentMeldableHeap<std::pair<T,int>>heap;
  std::vector<PersistentMeldableHeap<std::pair<T,int>>>h(n);
  std::vector<bool>vis(n,false);
  fast_stack<int>st(n);
  st.push(t);
  vis[t]=true;
  while(!st.empty()){
    int x=st.pop();
    bool f=false;
    for(const Edge<T>&e:g[x])if(dst[e.to]!=inf){
      if(!f&&pre[x]==e.to&&dst[x]==dst[e.to]+e.weight)f=true;
      else{
        T c=-dst[x]+e.weight+dst[e.to];
        h[x]=h[x].push({c,e.to});
      }
    }
    for(const Edge<T>&e:revg[x])if(!vis[e.to]){
      if(pre[e.to]==x){
        h[e.to]=h[e.to].meld(h[x]);
        vis[e.to]=true;
        st.push(e.to);
      }
    }
  }
  T a=dst[s];
  std::vector<T>ans;
  ans.reserve(k);
  ans.push_back(a);
  if(!h[s].empty()){
    using P=std::pair<T,typename PersistentMeldableHeap<std::pair<T,int>>::node*>;
    std::priority_queue<P,std::vector<P>,std::greater<P>>que;
    que.emplace(a+h[s].top().first,h[s].root);
    while((int)ans.size()<k&&!que.empty()){
      auto [d,nd]=que.top();que.pop();
      ans.push_back(d);
      if(nd->left)que.emplace(d+nd->left->val.first-nd->val.first,nd->left);
      if(nd->right)que.emplace(d+nd->right->val.first-nd->val.first,nd->right);
      PersistentMeldableHeap<std::pair<T,int>>p=h[nd->val.second];
      if(!p.empty())que.emplace(d+p.top().first,p.root);
    }
  }
  while((int)ans.size()<k)ans.push_back(inf);
  return ans;
}