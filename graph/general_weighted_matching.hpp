#pragma once
#include<vector>
#include<queue>
#include<numeric>
#include<limits>
#include "edge_base.hpp"
template<typename T>
std::vector<Edge<T>>general_weighted_matching(const int n,const std::vector<Edge<T>>&edge){
  static constexpr T inf=std::numeric_limits<T>::max()/2;
  int m=n,in=0;
  std::vector<int>slack(n*2),root(n*2),par(n*2),is_s(n*2),used(n*2),mate(n*2);
  std::vector<std::vector<int>>flower(n*2),belong(n*2,std::vector<int>(n*2));
  std::vector<T>dual(n*2);
  std::queue<int>que;
  std::vector<std::vector<Edge<T>>>g(n*2,std::vector<Edge<T>>(n*2,Edge<T>(0,0,0)));
  for(int i=0;i<=n;i++)for(int j=0;j<=n;j++)g[i][j]=Edge<T>(i,j,0);
  std::fill(dual.begin()+1,dual.begin()+n+1,inf);
  std::iota(root.begin(),root.begin()+n+1,0);
  for(int i=0;i<=n;i++)belong[i][i]=i;
  for(const Edge<T>&e:edge)g[e.from+1][e.to+1].weight=g[e.to+1][e.from+1].weight=e.weight*2;
  #define init(x) std::fill(x.begin(),x.end(),0)
  #define dst(x) (dual[x.from]+dual[x.to]-x.weight)
  #define update(u,v) if(!slack[v]||dst(g[u][v])<dst(g[slack[v]][v]))slack[v]=u
  auto recalc=[&](int u)->void {
    slack[u]=0;
    for(int i=1;i<=n;i++)if(g[i][u].weight&&root[i]!=u&&is_s[root[i]]==1)update(i,u);
  };
  auto push=[&](auto self,int u)->void {
    if(u<=n)que.push(u);
    else for(int f:flower[u])self(self,f);
  };
  auto set=[&](auto self,int u,int rt)->void {
    root[u]=rt;
    if(u>n)for(int f:flower[u])self(self,f,rt);
  };
  auto find_even=[&](int b,int v)->int {
    int pos=std::find(flower[b].begin(),flower[b].end(),v)-flower[b].begin();
    if(pos&1){
      std::reverse(flower[b].begin()+1,flower[b].end());
      pos=flower[b].size()-pos;
    }
    return pos;
  };
  auto match=[&](auto self,int u,int v)->void {
    mate[u]=g[u][v].to;
    if(u>n){
      int x=belong[u][g[u][v].from];
      int pos=find_even(u,x);
      for(int i=0;i<pos;i++)self(self,flower[u][i],flower[u][i^1]);
      self(self,x,v);
      std::rotate(flower[u].begin(),flower[u].begin()+pos,flower[u].end());
    }
  };
  auto link=[&](int u,int v)->void {
    while(true){
      int nv=root[mate[u]];
      match(match,u,v);
      if(!nv)break;
      v=nv,u=root[par[nv]];
      match(match,v,u);
    }
  };
  auto make=[&](int u,int v,int l)->void {
    int id=n+1;
    while(id<=m&&root[id])id++;
    if(id>m)m++;
    flower[id].clear();
    for(int i=1;i<=m;i++)g[id][i].weight=g[i][id].weight=0;
    for(int i=1;i<=n;i++)belong[id][i]=0;
    is_s[id]=1,dual[id]=0,mate[id]=mate[l];
    while(u!=l){
      flower[id].push_back(u);
      u=root[mate[u]];
      push(push,u);
      flower[id].push_back(u);
      u=root[par[u]];
    }
    flower[id].push_back(l);
    std::reverse(flower[id].begin(),flower[id].end());
    while(v!=l){
      flower[id].push_back(v);
      v=root[mate[v]];
      push(push,v);
      flower[id].push_back(v);
      v=root[par[v]];
    }
    set(set,id,id);
    for(int c:flower[id]){
      for(int i=1;i<=m;i++)if(!g[id][i].weight||dst(g[c][i])<dst(g[id][i])){
        g[id][i]=g[c][i],g[i][id]=g[i][c];
      }
      for(int i=1;i<=n;i++)if(belong[c][i])belong[id][i]=c;
    }
    recalc(id);
  };
  auto expand=[&](int b)->void {
    for(int c:flower[b])set(set,c,c);
    int x=belong[b][g[b][par[b]].from];
    is_s[x]=2,par[x]=par[b];
    int pos=find_even(b,x);
    for(int i=0;i<pos;i+=2){
      int t=flower[b][i],s=flower[b][i+1];
      is_s[s]=1,is_s[t]=2;
      par[t]=g[s][t].from;
      slack[s]=slack[t]=0;
      push(push,s);
    }
    for(int i=pos+1;i<(int)flower[b].size();i++){
      is_s[flower[b][i]]=0;
      recalc(flower[b][i]);
    }
    flower[b].clear();
    root[b]=0;
  };
  auto path=[&](const Edge<T>&e)->bool {
    int u=root[e.from],v=root[e.to];
    if(!is_s[v]){
      par[v]=e.from;
      is_s[v]=2;
      int nu=root[mate[v]];
      slack[v]=slack[nu]=0;
      is_s[nu]=1;
      push(push,nu);
    }
    else if(is_s[v]==1){
      int l=0,bu=u,bv=v;
      in++;
      while(bu){
        used[bu]=in;
        bu=root[mate[bu]];
        if(bu)bu=root[par[bu]];
      }
      while(bv){
        if(used[bv]==in){
          l=bv;
          break;
        }
        bv=root[mate[bv]];
        if(bv)bv=root[par[bv]];
      }
      if(l)make(u,v,l);
      else{
        link(u,v);
        link(v,u);
        return true;
      }
    }
    return false;
  };
  auto augment=[&]()->bool {
    init(is_s),init(slack),init(par);
    que=std::queue<int>();
    for(int i=1;i<=m;i++)if(root[i]==i&&!mate[i]){
      is_s[i]=1;
      push(push,i);
    }
    if(que.empty())return false;
    while(true){
      while(!que.empty()){
        int v=que.front();
        que.pop();
        for(int i=1;i<=n;i++)if(g[v][i].weight&&root[i]!=root[v]){
          if(!dst(g[v][i])){
            if(path(g[v][i]))return true;
          }
          else if(is_s[root[i]]!=2)update(v,root[i]);
        }
      }
      T delta=inf;
      for(int i=n+1;i<=m;i++)if(root[i]==i&&is_s[i]==2&&delta>dual[i]/2)delta=dual[i]/2;
      for(int i=1;i<=m;i++)if(root[i]==i&&slack[i]&&is_s[i]!=2){
        T d2=dst(g[slack[i]][i]);
        if(!is_s[i]){
          if(delta>d2)delta=d2;
        }
        else{
          if(delta>d2/2)delta=d2/2;
        }
      }
      for(int i=1;i<=n;i++){
        if(is_s[root[i]]==1){
          dual[i]-=delta;
          if(dual[i]<=0)return false;
        }
        else if(is_s[root[i]]==2)dual[i]+=delta;
      }
      for(int i=n+1;i<=m;i++)if(root[i]==i&&is_s[i]){
        if(is_s[i]==1)dual[i]+=delta*2;
        else dual[i]-=delta*2;
      }
      for(int i=1;i<=m;i++)if(root[i]==i&&slack[i]&&root[slack[i]]!=i){
        if(dst(g[slack[i]][i])==0&&path(g[slack[i]][i]))return true;
      }
      for(int i=n+1;i<=m;i++)if(root[i]==i&&is_s[i]==2&&dual[i]==0)expand(i);
    }
  };
  while(augment());
  std::vector<Edge<T>>res;
  for(const Edge<T>&e:edge){
    if(mate[e.from+1]==e.to+1)res.push_back(e);
  }
  return res;
  #undef init
  #undef dst
  #undef update
}