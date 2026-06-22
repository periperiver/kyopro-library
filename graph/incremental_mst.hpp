#pragma once
#include<vector>
#include<limits>
#include<numeric>
#include "random/permutation.hpp"
template<typename T>
struct IncrementalMST{
private:
  static constexpr T inf=std::numeric_limits<T>::max();
  std::vector<int>par,sz;
  std::vector<std::pair<T,int>>weight;
  std::vector<int>perm;
  int find(int u){
    while(par[u]!=u){
      while(weight[par[u]].first<=weight[u].first){
        sz[par[u]]-=sz[u];
        par[u]=par[par[u]];
      }
      u=par[u];
    }
    return u;
  }
  void disconnect(int u){
    for(int pre=sz[u];par[u]!=u;){
      u=par[u];
      sz[u]-=pre;
      pre+=sz[u];
    }
  }
  int connect(int u,T w){
    while(weight[u].first<=w){
      sz[par[u]]+=sz[u];
      u=par[u];
    }
    return u;
  }
  void add(int u,int v,std::pair<T,int>w){
    disconnect(u);
    disconnect(v);
    while(u!=v){
      u=connect(u,w.first);
      v=connect(v,w.first);
      if(perm[u]<perm[v])std::swap(u,v);
      std::swap(par[v],u);
      std::swap(weight[v],w);
    }
    connect(u,inf-1);
  }
  int max_edge(int u,int v){
    if(find(u)!=find(v))return -1;
    while(true){
      if(weight[u].first>weight[v].first)std::swap(u,v);
      if(par[u]==v)break;
      u=par[u];
    }
    return u;
  }
public:
  IncrementalMST(){}
  explicit IncrementalMST(int n):par(n,-1),sz(n,1),weight(n,std::make_pair(inf,-1)){
    std::iota(par.begin(),par.end(),0);
    perm=Random::permutation(n);
  }
  int add_edge(int u,int v,T w,int idx){
    if(u==v)return idx;
    std::pair<T,int>wp=std::make_pair(w,idx);
    int p=max_edge(u,v);
    if(p==-1){
      add(u,v,wp);
      return -1;
    }
    else if(weight[p].first>w){
      int res=weight[p].second;
      if(par[p]!=p){
        int x=p;
        while(par[x]!=x){
          x=par[x];
          sz[x]-=sz[p];
        }
        par[p]=p;
        weight[p]=std::make_pair(inf,-1);
      }
      add(u,v,wp);
      return res;
    }
    return idx;
  }
  inline std::pair<T,int>path_max(int u,int v){return weight[max_edge(u,v)];}
};