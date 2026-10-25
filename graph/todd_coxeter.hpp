#pragma once
#include<vector>
#include<array>
#include<algorithm>
#include<cassert>
namespace todd_coxeter_impl{
struct IncrementalUnionFind{
  std::vector<int>par;
  int root(int u){
    if(par[u]<0)return u;
    return par[u]=root(par[u]);
  }
  bool merge(int&u,int&v){
    u=root(u),v=root(v);
    if(u==v)return false;
    if(par[u]<par[v])std::swap(u,v);
    par[v]+=par[u];
    par[u]=v;
    return true;
  }
  void add_vertex(){
    par.push_back(-1);
  }
  bool same(int u,int v){return root(u)==root(v);}
};
template<int K>
struct ToddCoxeter{
  std::vector<std::pair<std::vector<int>,std::vector<int>>>a;
  std::vector<std::array<int,K>>g;
  IncrementalUnionFind uf;
  std::vector<bool>vis;
  std::vector<std::pair<int,int>>st;
  ToddCoxeter(std::vector<std::pair<std::vector<int>,std::vector<int>>>a):a(std::move(a)){}
  int add_vertex(){
    g.emplace_back().fill(-1);
    uf.add_vertex();
    vis.emplace_back(false);
    return g.size()-1;
  }
  int step(int u,int t){
    u=uf.root(u);
    if(g[u][t]==-1)g[u][t]=add_vertex();
    return uf.root(g[u][t]);
  }
  int walk(int u,const std::vector<int>&s){
    for(const int&t:s)u=step(u,t);
    return u;
  }
  void merge(int u,int v){
    st.emplace_back(u,v);
    while(!st.empty()){
      auto [x,y]=st.back();st.pop_back();
      if(uf.merge(x,y)){
        int r=uf.root(x);
        vis[r]=vis[x]||vis[y];
        for(int i=0;i<K;i++){
          int a=g[x][i],b=g[y][i];
          if(a==-1)g[r][i]=b;
          else g[r][i]=a;
          if(a!=-1&&b!=-1)st.emplace_back(a,b);
        }
      }
    }
  }
  std::vector<std::array<int,K>>solve(){
    add_vertex();
    for(int i=0;i<(int)g.size();i++)if(!vis[uf.root(i)]){
      for(const auto&[s,t]:a)merge(walk(i,s),walk(i,t));
      for(int j=0;j<K;j++)step(i,j);
      vis[uf.root(i)]=true;
    }
    std::vector<int>r;
    for(int i=0;i<(int)g.size();i++)if(!uf.same(0,i)){
      r.push_back(uf.root(i));
    }
    std::sort(r.begin(),r.end()),r.erase(std::unique(r.begin(),r.end()),r.end());
    std::vector<std::array<int,K>>res(r.size()+1);
    for(int i=0;i<(int)res.size();i++){
      int u=i==0?uf.root(0):r[i-1];
      for(int j=0;j<K;j++){
        int v=g[u][j];
        v=uf.root(v);
        if(uf.same(0,v))v=0;
        else v=(std::lower_bound(r.begin(),r.end(),v)-r.begin())+1;
        res[i][j]=v;
      }
    }
    return res;
  }
};
template<int K>
std::vector<std::array<int,K>>todd_coxeter(std::vector<std::pair<std::vector<int>,std::vector<int>>>a){
  return ToddCoxeter<K>(std::move(a)).solve();
}
}
using todd_coxeter_impl::todd_coxeter;