#pragma once
#include<vector>
#include<algorithm>
#include<cassert>
#include<stack>
#include "lca.hpp"
template<typename T=int>
struct AuxiliaryTree{
private:
  std::vector<std::vector<Edge<T>>>g;
  LowestCommonAncestor lca;
  std::vector<int>in,out;
  std::vector<T>dep;
public:
  AuxiliaryTree(const Tree<T>&t):g(t.size()),in(t.size()),out(t.size()){
    assert(t.is_directed());
    lca=LowestCommonAncestor(t);
    int root=t.root();
    int i=0;
    dep.resize(t.size());
    auto dfs=[&](auto&&self,int x,int p)->void {
      in[x]=i++;
      for(const auto&e:t[x])if(e.to!=p){
        dep[e.to]=dep[e.from]+e.weight;
        self(self,e.to,e.from);
      }
      out[x]=i;
    };
    dep[root]=0;
    dfs(dfs,root,-1);
  }
  AuxiliaryTree(){}
  int build(std::vector<int>v){
    assert(!v.empty());
    auto comp=[&](int x,int y)->bool {return in[x]<in[y];};
    std::sort(v.begin(),v.end(),comp);
    v.reserve(v.size()*2-1);
    int vsize=v.size();
    for(int i=1;i<vsize;i++)v.push_back(lca.query(v[i-1],v[i]));
    std::sort(v.begin(),v.end(),comp);
    v.erase(std::unique(v.begin(),v.end()),v.end());
    for(int i=0;i<(int)v.size();i++)g[v[i]].clear();
    std::stack<int>st;
    for(int i=0;i<(int)v.size();i++){
      while(!st.empty()&&out[st.top()]<=in[v[i]])st.pop();
      if(!st.empty()){
        T w=dep[v[i]]-dep[st.top()];
        g[st.top()].push_back(Edge<T>(st.top(),v[i],w));
        g[v[i]].push_back(Edge<T>(v[i],st.top(),w));
      }
      st.push(v[i]);
    }
    while(st.size()>1)st.pop();
    return st.top();
  }
  std::vector<Edge<T>> &operator[](int i){return g[i];}
  const std::vector<Edge<T>> &operator[](int i)const{return g[i];}
};