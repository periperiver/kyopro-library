#pragma once
#include "template.hpp"
struct rollbackUF{
  stack<pair<int,int>>history;
  vector<int>par;
  int sz;
  rollbackUF(int n):sz(n),par(n,-1){}
  int root(int u)const{
    while(par[u]>=0)u=par[u];
    return u;
  }
  bool same(int u,int v)const{return root(u)==root(v);}
  bool merge(int u,int v){
    int ru=root(u),rv=root(v);
    if(ru==rv)return false;
    if(par[ru]<par[rv])swap(ru,rv);
    history.emplace(ru,par[ru]);
    par[ru]=rv;
    sz--;
    return true;
  }
  void undo(int cnt=1){
    sz+=cnt;
    while(cnt--){
      auto [u,p]=history.top();
      history.pop();
      par[u]=p;
    }
  }
  int size(int u)const{return -par[root(u)];}
  int size()const{return sz;}
};