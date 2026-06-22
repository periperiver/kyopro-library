#pragma once
#include<vector>
#include<cassert>
#include<numeric>
#include<queue>
#include "datastructure/dual_sparse_table.hpp"
#include "monoid/rangechmin_rangemin.hpp"
#include "graph_base.hpp"
#include "tree_base.hpp"
template<typename T>
std::vector<T>erased_shortest_pah(const Graph<T>&g,int s,int t){
  assert(!g.is_directed());
  int n=g.size();
  int m=g.edge_size();
  static constexpr T inf=std::numeric_limits<T>::max();
  auto dijkstra_few_zero=[&](int start)->std::pair<std::vector<T>,std::vector<int>> {
    std::vector<T>dst(n,inf);
    std::vector<int>pre(n,-1),zero(n,0);
    using v_type=std::pair<std::pair<T,int>,int>;
    std::priority_queue<v_type,std::vector<v_type>,std::greater<v_type>>que;
    que.push(std::make_pair(std::make_pair(0,0),start));
    dst[start]=0;
    while(!que.empty()){
      auto [d,x]=que.top();que.pop();
      if(dst[x]!=d.first||zero[x]!=d.second)continue;
      for(const Edge<T>&e:g[x]){
        T nxt=d.first+e.weight;
        int nzero=d.second+(e.weight==0);
        if(dst[e.to]>nxt||(dst[e.to]==nxt&&zero[e.to]>nzero)){
          dst[e.to]=nxt;
          zero[e.to]=nzero;
          pre[e.to]=x;
          que.push(std::make_pair(std::make_pair(dst[e.to],zero[e.to]),e.to));
        }
      }
    }
    return std::make_pair(dst,pre);
  };
  auto [dst_s,pre_s]=dijkstra_few_zero(s);
  if(dst_s[t]==inf)return std::vector<T>(m,inf);
  auto [dst_t,pre_t]=dijkstra_few_zero(t);
  std::vector<int>shortest_vertex(n,-1);
  std::vector<int>shortest_edge(m,-1);
  int path_size=-1;
  for(int i=s;i!=-1;i=pre_t[i])shortest_vertex[i]=++path_size;
  for(int i=0;i<n;i++)if(i!=t&&shortest_vertex[i]!=-1)pre_s[pre_t[i]]=i;
  std::vector<bool>belong(n,false);
  for(int i=0;i<m;i++){
    const Edge<T>&e=g.get_edge(i);
    assert(e.index==i);
    if(shortest_vertex[e.from]!=-1&&shortest_vertex[e.to]!=-1){
      if(dst_s[e.from]+e.weight==dst_s[e.to]||dst_s[e.to]+e.weight==dst_s[e.from]){
        int id=std::min(shortest_vertex[e.from],shortest_vertex[e.to]);
        if(!belong[id])shortest_edge[i]=id,belong[id]=true;
      }
    }
  }
  Tree<> ts(n);
  for(int i=0;i<n;i++)if(i!=s)ts.add_edge(i,pre_s[i]==-1?0:pre_s[i]);
  ts.build();
  Tree<> tt(n);
  for(int i=0;i<n;i++)if(i!=t)tt.add_edge(i,pre_t[i]==-1?0:pre_t[i]);
  tt.build();
  std::vector<int>lca_s(n),lca_t(n);
  ts.remove_parent(s),tt.remove_parent(t);
  for(int i:ts.bfs_order()){
    if(shortest_vertex[i]!=-1)lca_s[i]=i;
    for(const Edge<>&e:ts[i])lca_s[e.to]=lca_s[i];
  }
  for(int i:tt.bfs_order()){
    if(shortest_vertex[i]!=-1)lca_t[i]=i;
    for(const Edge<>&e:tt[i])lca_t[e.to]=lca_t[i];
  }
  std::vector<T>res(m);
  std::vector<std::tuple<int,int,T>>query;
  for(int i=0;i<m;i++){
    if(shortest_edge[i]==-1){
      const Edge<T>&e=g.get_edge(i);
      res[i]=dst_s[t];
      {
        int u=lca_s[e.from];
        int v=lca_t[e.to];
        if(shortest_vertex[u]<shortest_vertex[v]){
          query.emplace_back(shortest_vertex[u],shortest_vertex[v],dst_s[e.from]+e.weight+dst_t[e.to]);
        }
      }
      {
        int u=lca_s[e.to];
        int v=lca_t[e.from];
        if(shortest_vertex[u]<shortest_vertex[v]){
          query.emplace_back(shortest_vertex[u],shortest_vertex[v],dst_s[e.to]+e.weight+dst_t[e.from]);
        }
      }
    }
  }
  std::vector<T>sp=dual_sparse_table<RangeChminRangeMin<T>>(path_size,query);
  for(int i=0;i<m;i++)if(shortest_edge[i]!=-1){
    res[i]=sp[shortest_edge[i]];
  }
  return res;
}