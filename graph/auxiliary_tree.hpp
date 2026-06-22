#pragma once
#include<vector>
#include<algorithm>
#include<cassert>
#include<stack>
#include "lca.hpp"
using namespace std;
template<typename T=int>
struct AuxiliaryTree{
private:
  vector<vector<Edge<T>>>g;
  LowestCommonAncestor lca;
  vector<int>in,out;
  vector<T>dep;
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
  int build(vector<int>v){
    assert(!v.empty());
    auto comp=[&](int x,int y)->bool {return in[x]<in[y];};
    sort(v.begin(),v.end(),comp);
    v.reserve(v.size()*2-1);
    int vsize=v.size();
    for(int i=1;i<vsize;i++)v.push_back(lca.query(v[i-1],v[i]));
    sort(v.begin(),v.end(),comp);
    v.erase(unique(v.begin(),v.end()),v.end());
    for(int i=0;i<(int)v.size();i++)g[v[i]].clear();
    stack<int>st;
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
  vector<Edge<T>> &operator[](int i){return g[i];}
  const vector<Edge<T>> &operator[](int i)const{return g[i];}
};