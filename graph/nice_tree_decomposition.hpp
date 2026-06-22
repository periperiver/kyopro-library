#pragma once
#include<vector>
#include<array>
#include<queue>
#include<utility>
#include<unordered_set>
#include<cassert>
#include<algorithm>
#include "graph_base.hpp"
template<typename T>
std::pair<std::vector<std::pair<int,int>>,std::vector<std::array<int,3>>>nice_tree_decomposition(const Graph<T>&g){
  assert(!g.is_directed());
  std::vector<std::unordered_set<int>>adj(g.size());
  for(const Edge<T>&e:g){
    adj[e.from].insert(e.to);
    adj[e.to].insert(e.from);
  }
  std::vector<int>deg(g.size());
  std::queue<int>que;
  for(int i=0;i<g.size();i++)if((deg[i]=adj[i].size())<=2)que.push(i);
  std::vector<int>idx(g.size(),-1);
  std::vector<std::pair<int,int>>child(1);
  std::vector<std::array<int,3>>vs(1);
  auto make_nice=[&](int&ch,const std::array<int,3>&par)->void {
    std::array<int,3>ar;
    int pos1=0,pos2=0;
    while(pos1<3&&vs[ch][pos1]!=-1)pos1++;
    while(pos2<3&&par[pos2]!=-1)pos2++;
    for(int i=0;pos1>1;){
      bool found=false;
      for(;i<pos1;i++){
        if(std::find(par.begin(),par.begin()+pos2,vs[ch][i])==par.begin()+pos2){
          child.emplace_back(ch,-1);
          assert(ch!=child.size()-1);
          std::array<int,3>&fix=vs.emplace_back(vs[ch]);
          for(int j=i;j+1<3;j++)fix[j]=fix[j+1];
          fix[2]=-1;
          ch=child.size()-1;
          found=true;
          pos1--;
          break;
        }
      }
      if(!found)break;
    }
    for(int i=0;i<pos2;i++){
      if(std::find(vs[ch].begin(),vs[ch].begin()+pos1,par[i])==vs[ch].begin()+pos1){
        child.emplace_back(ch,-1);
        std::array<int,3>&fix=vs.emplace_back(vs[ch]);
        fix[pos1++]=par[i];
        ch=child.size()-1;
      }
    }
  };
  auto merge=[&](int lc,int rc,const std::array<int,3>&par)->int {
    make_nice(lc,par);
    make_nice(rc,par);
    child.emplace_back(lc,rc);
    vs.emplace_back(par);
    return child.size()-1;
  };
  while(!que.empty()){
    int x=que.front();que.pop();
    if(idx[x]!=-1)continue;
    std::array<int,3>v;
    v[0]=x;
    v[1]=v[2]=-1;
    for(int y:adj[x]){
      if(idx[y]==-1){
        if(v[1]==-1)v[1]=y;
        else v[2]=y;
      }
    }
    std::pair<int,int>c=std::make_pair(-1,-1);
    for(int y:adj[x])if(idx[y]>=0){
      if(c.first==-1)c.first=idx[y];
      else if(c.second==-1)c.second=idx[y];
      else{
        c.first=merge(c.first,c.second,v);
        c.second=idx[y];
      }
      idx[y]=-2;
    }
    deg[x]=0;
    if(c.first==-1){
      idx[x]=child.size();
      child.emplace_back(c);
      vs.emplace_back(std::move(v));
      int u=idx[x];
      if(vs[u][2]!=-1){
        child[u].first=child.size();
        child.emplace_back(-1,-1);
        vs.emplace_back(vs[u])[2]=-1;
        u=child.size()-1;
      }
      if(vs[u][1]!=-1){
        child[u].first=child.size();
        child.emplace_back(-1,-1);
        vs.emplace_back(vs[u])[1]=-1;
        u=child.size()-1;
      }
    }
    else if(c.second==-1){
      make_nice(c.first,v);
      idx[x]=c.first;
    }
    else{
      idx[x]=merge(c.first,c.second,v);
    }
    if(v[1]!=-1){
      if(v[2]!=-1){
        if(adj[v[1]].contains(v[2])){
          for(int i=1;i<=2;i++){
            if(--deg[v[i]]<=2&&idx[v[i]]==-1)que.push(v[i]);
          }
        }
        else{
          adj[v[1]].insert(v[2]);
          adj[v[2]].insert(v[1]);
        }
      }
      else if(--deg[v[1]]<=2&&idx[v[1]]==-1)que.push(v[1]);
    }
  }
  if(std::any_of(deg.begin(),deg.end(),[](int x){return x!=0;}))return {{},{}};
  child[0]=std::move(child.back()),child.pop_back();
  vs[0]=std::move(vs.back()),vs.pop_back();
  que.push(0);
  while(!que.empty()){
    int x=que.front();que.pop();
    if(child[x].second!=-1){
      vs[child[x].first]=vs[child[x].second]=vs[x];
      que.push(child[x].first);
      que.push(child[x].second);
    }
    else if(child[x].first!=-1){
      std::array<int,3>nxt=vs[x];
      std::array<int,3>&nc=vs[child[x].first];
      if(std::count(vs[x].begin(),vs[x].end(),-1)<std::count(nc.begin(),nc.end(),-1)){
        for(int i=0;i<3;i++){
          if(nxt[i]!=-1&&std::find(nc.begin(),nc.end(),nxt[i])==nc.end()){
            nxt[i]=-1;
          }
        }
      }
      else{
        for(int i=0;i<3;i++){
          if(nc[i]!=-1&&std::find(vs[x].begin(),vs[x].end(),nc[i])==vs[x].end()){
            nxt[std::find(nxt.begin(),nxt.end(),-1)-nxt.begin()]=nc[i];
          }
        }
      }
      nc=std::move(nxt);
      que.push(child[x].first);
    }
  }
  return std::make_pair(std::move(child),std::move(vs));
}