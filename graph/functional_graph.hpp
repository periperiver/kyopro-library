#pragma once
#include<concepts>
#include<algorithm>
#include "../datastructure/csr_array.hpp"
struct FunctionalGraph{
private:
std::vector<int>dep,p,pos,dfs_ord;
csr_array<int>g,cycle,bin;
public:
  FunctionalGraph(){}
  FunctionalGraph(const std::vector<int>&p_):p(p_),g(p_.size(),p_){
    int n=p.size();
    dep.resize(n);
    pos.resize(n);
    dfs_ord.reserve(n);
    std::vector<bool>vis(n,false);
    std::vector<int>st(n);
    int r=0;
    std::vector<std::pair<int,int>>cycle_v;
    std::vector<int>bin_v;
    cycle_v.reserve(n);
    bin_v.reserve(n);
    for(int i=0;i<n;i++)if(!vis[i]){
      int u=i;
      while(true){
        if(vis[u])break;
        vis[u]=true;
        u=p[u];
      }
      r=0;
      st[r++]=u;
      while(r){
        int x=st[--r];
        vis[x]=true;
        pos[x]=dfs_ord.size();
        dfs_ord.push_back(x);
        bin_v.push_back(dep[x]);
        for(int y:g[x])if(y!=u){
          dep[y]=dep[x]+1;
          st[r++]=y;
        }
      }
      for(int v=p[u];;v=p[v]){
        cycle_v.emplace_back(u,v);
        if(u==v)break;
      }
    }
    std::reverse(cycle_v.begin(),cycle_v.end());
    cycle=csr_array<int>(n,cycle_v);
    bin=csr_array<int>(*std::max_element(bin_v.begin(),bin_v.end())+1,bin_v);
  }
  template<std::integral T>
  int jump(int u,T k)const{
    int c=std::min<std::common_type_t<int,T>>(k,dep[u]);
    int u2;
    if(c<=10){
      u2=u;
      while(c--)u2=p[u2];
    }
    else{
      int d=dep[u]-std::min<T>(k,dep[u]);
      u2=dfs_ord[*std::prev(std::upper_bound(bin[d].begin(),bin[d].end(),pos[u]))];
    }
    if(dep[u]>=k)return u2;
    else{
      k-=dep[u];
      k%=cycle[u2].size();
      if(k==0)return u2;
      else return cycle[u2][cycle[u2].size()-k];
    }
  }
  inline int cycle_size(int u)const{return cycle[jump(u,dep[u])].size();}
};