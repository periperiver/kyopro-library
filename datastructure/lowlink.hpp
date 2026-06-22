#pragma once
#include "template.hpp"
struct lowlink{
private:
  int n;
  vector<vector<int>>to;
  vector<bool>seen;
  vector<int>order,low;
public:
  vector<bool>ap;
  vector<pair<int,int>>bridge;
  lowlink(int n):n(n),to(n),seen(n,false),order(n),low(n),ap(n){}
  void add_edge(int u,int v){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    to[u].emplace_back(v);
    to[v].emplace_back(u);
  }
  void build(){
    int k=0;
    rep(i,n)if(!seen[i]){
      k=dfs(i,-1,k);
    }
    sort(all(bridge));
  }
private:
  int dfs(int x,int p,int k){
    seen[x]=true;
    order[x]=k++;
    low[x]=order[x];
    int cnt=0;
    for(auto i:to[x])if(i!=p){
      if(!seen[i]){
        cnt++;
        k=dfs(i,x,k);
        chmin(low[x],low[i]);
        if(p!=-1&&order[x]<=low[i])ap[x]=true;
        if(order[x]<low[i])bridge.emplace_back(min(x,i),max(x,i));
      }
      else chmin(low[x],order[i]);
    }
    if(p==-1&&cnt>=2)ap[x]=true;
    return k;
  }
};