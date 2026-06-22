#pragma once
#include<queue>
#include "graph/tree_base.hpp"
struct StaticTopTree{
private:
  template<typename T>
  void build(Tree<T>t){
    int n=t.size();
    left.reserve(n*2-1),right.reserve(n*2-1),par.reserve(n*2-1),A.reserve(n*2-1),B.reserve(n*2-1);
    left.resize(n,-1),right.resize(n,-1),par.resize(n,-1),A.resize(n),B.resize(n);
    for(int i=0;i<n;i++){
      A[i]=t.parent(i);
      B[i]=i;
    }
    auto dfs=[&](auto self,int x)->std::pair<int,int> {
      std::vector<std::pair<int,int>>vs{{0,x}};
      while(t[x].size()>=1){
        std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::greater<std::pair<int,int>>>que;
        int heavy=t[x][0].to;
        que.emplace(0,heavy);
        for(int i=1;i<t[x].size();i++)que.push(self(self,t[x][i].to));
        while((int)que.size()>=2){
          auto [d1,v1]=que.top();que.pop();
          auto [d2,v2]=que.top();que.pop();
          if(B[v2]==heavy)std::swap(d1,d2),std::swap(v1,v2);
          int nv=left.size();
          left.push_back(v1),right.push_back(v2),par.push_back(-1),A.push_back(x),B.push_back(B[v1]);
          par[v1]=par[v2]=nv;
          que.emplace(std::max(d1,d2)+1,nv);
        }
        vs.push_back(que.top());
        while(true){
          int sz=vs.size();
          if(sz>=3&&(vs[sz-3].first==vs[sz-2].first||vs[sz-3].first<=vs[sz-1].first)){
            int nv=left.size();
            left.push_back(vs[sz-3].second),right.push_back(vs[sz-2].second),par.push_back(-1),A.push_back(A[vs[sz-3].second]),B.push_back(B[vs[sz-2].second]);
            par[vs[sz-3].second]=par[vs[sz-2].second]=nv;
            vs[sz-3].first=std::max(vs[sz-3].first,vs[sz-2].first)+1;
            vs[sz-3].second=nv;
            vs[sz-2]=vs[sz-1];
            vs.pop_back();
          }
          else if(sz>=2&&vs[sz-2].first<=vs[sz-1].first){
            int nv=left.size();
            left.push_back(vs[sz-2].second),right.push_back(vs[sz-1].second),par.push_back(-1),A.push_back(A[vs[sz-2].second]),B.push_back(B[vs[sz-1].second]);
            par[vs[sz-2].second]=par[vs[sz-1].second]=nv;
            vs[sz-2].first=std::max(vs[sz-2].first,vs[sz-1].first)+1;
            vs[sz-2].second=nv;
            vs.pop_back();
          }
          else break;
        }
        x=heavy;
      }
      while((int)vs.size()>=2){
        int sz=vs.size();
        int nv=left.size();
        left.push_back(vs[sz-2].second),right.push_back(vs[sz-1].second),par.push_back(-1),A.push_back(A[vs[sz-2].second]),B.push_back(B[vs[sz-1].second]);
        par[vs[sz-2].second]=par[vs[sz-1].second]=nv;
        vs[sz-2].first=std::max(vs[sz-2].first,vs[sz-1].first)+1;
        vs[sz-2].second=nv;
        vs.pop_back();
      }
      return vs[0];
    };
    dfs(dfs,t.root());
  }
public:
  std::vector<int>left,right,par,A,B;
  StaticTopTree(){}
  template<typename T>
  explicit StaticTopTree(Tree<T>t,int){
    build(std::move(t));
  }
  template<typename T>
  explicit StaticTopTree(Tree<T>t){
    assert(t.is_directed());
    t.hld();
    build(std::move(t));
  }
};