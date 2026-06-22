#pragma once
#include<vector>
#include<algorithm>
struct UnionFind{
private:
  std::vector<int>par;
  int cs;
public:
  UnionFind(int n):par(n,-1),cs(n){}
  UnionFind(){}
  int root(int u){
    if(par[u]<0)return u;
    return par[u]=root(par[u]);
  }
  bool merge(int u,int v){
    int ru=root(u),rv=root(v);
    if(ru==rv)return false;
    if(par[ru]<par[rv])std::swap(ru,rv);
    par[rv]+=par[ru];
    par[ru]=rv;
    cs--;
    return true;
  }
  bool same(int u,int v){return root(u)==root(v);}
  int size(int u=-1){return u==-1?cs:-par[root(u)];}
  void reset(int n){
    cs=n;
    std::fill(par.begin(),par.begin()+n,-1);
  }
  std::vector<std::vector<int>>get_all(){
    std::vector<std::vector<int>>res(par.size());
    for(int i=0;i<(int)par.size();i++)res[root(i)].push_back(i);
    int p=0;
    for(int i=0;i<(int)res.size();i++){
      if(!res[i].empty()){
        if(i==p)p++;
        else res[p++]=std::move(res[i]);
      }
    }
    res.resize(p);
    return res;
  }
};