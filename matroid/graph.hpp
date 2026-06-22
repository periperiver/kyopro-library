#pragma once
#include<algorithm>
#include<cassert>
#include "graph/graph_base.hpp"
struct GraphicMatroid{
private:
  Graph<>g;
  std::vector<std::pair<int,int>>pre;
  std::vector<int>que,dst,root;
  int p,q;
public:
  GraphicMatroid(const Graph<>&g_):g(g_),pre(g_.size()),que(g_.size()),dst(g_.size()),root(g_.size()){
    assert(!g.is_directed());
  }
  int size()const{return g.edge_size();}
  void set(const std::vector<bool>&I){
    std::fill(pre.begin(),pre.end(),std::make_pair(-1,-1));
    std::fill(dst.begin(),dst.end(),-1);
    for(int i=0;i<g.size();i++){
      if(pre[i].first==-1){
        for(p=q=0,que[q++]=i,dst[i]=0;p<q;){
          int x=que[p++];
          root[x]=i;
          for(const Edge<>&e:g[x])if(I[e.index]&&dst[e.to]==-1){
            pre[e.to]=std::make_pair(e.from,e.index);
            dst[e.to]=dst[e.from]+1;
            que[q++]=e.to;
          }
        }
      }
    }
  }
  std::vector<int>circuit(int x)const{
    const Edge<>&e=g.get_edge(x);
    if(root[e.from]!=root[e.to])return {};
    std::vector<int>res;
    int u=e.from,v=e.to;
    if(dst[u]>dst[v])std::swap(u,v);
    while(dst[u]<dst[v])res.push_back(pre[v].second),v=pre[v].first;
    while(u!=v){
      res.push_back(pre[u].second),u=pre[u].first;
      res.push_back(pre[v].second),v=pre[v].first;
    }
    return res;
  }
};
