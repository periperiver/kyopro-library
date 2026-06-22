#pragma once
#include<vector>
struct RangeUnionfind{
  std::vector<int>par;
  int log2n,n;
  int root(int u,int k){
    if(par[k*n+u]<0)return u;
    return par[k*n+u]=root(par[k*n+u],k);
  }
  bool same(int u,int v,int k){return root(u,k)==root(v,k);}
  void inmerge(int u,int v,int k){
    int ru=root(u,k),rv=root(v,k);
    if(ru>rv)std::swap(ru,rv);
    par[k*n+ru]+=par[k*n+rv];
    par[k*n+rv]=ru;
  }
  RangeUnionfind(int n):n(n){
    log2n=0;
    while((1<<log2n)<n)log2n++;
    n=1<<log2n;
    par.resize(n*log2n,-1);
  }
  std::vector<std::pair<int,int>>merge(int u,int v,int k){
    std::vector<std::pair<int,int>>ret;
    if(u==v)return ret;
    if(k<=0)return ret;
    if(u>v)std::swap(u,v);
    int d=v-u;
    auto dfs=[&](auto self,int u,int k) ->void {
      if(same(u,u+d,k))return;
      inmerge(u,u+d,k);
      if(k==0)ret.push_back({u,u+d});
      else{
        self(self,u,k-1);
        self(self,u+(1<<(k-1)),k-1);
      }
    };
    int c=0;
    while((1<<(c+1))<k)c++;
    dfs(dfs,u,c);
    dfs(dfs,u+k-(1<<c),c);
    return ret;
  }
};