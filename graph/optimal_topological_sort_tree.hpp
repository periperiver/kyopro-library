#pragma once
#include<vector>
#include<cassert>
#include<numeric>
#include<queue>
#include "../datastructure/unionfind.hpp"
template<typename T>
std::pair<T,std::vector<int>>optimal_topological_sort_tree(const std::vector<int>&par,std::vector<T>c,std::vector<T>d){
  int n=par.size();
  assert((int)c.size()==n);
  assert((int)d.size()==n);
  int root=std::find(par.begin(),par.end(),-1)-par.begin();
  assert(root!=n);
  assert(std::find(par.begin()+root+1,par.end(),-1)==par.end());
  std::vector<int>l(n),r(n),nxt(n);
  std::iota(l.begin(),l.end(),0);
  std::iota(r.begin(),r.end(),0);
  auto comp=[](const std::tuple<T,T,int,int>&lhs,const std::tuple<T,T,int,int>&rhs)->bool {
    return std::get<1>(lhs)*std::get<0>(rhs)<std::get<1>(rhs)*std::get<0>(lhs);
  };
  T res=0;
  std::priority_queue<std::tuple<T,T,int,int>,std::vector<std::tuple<T,T,int,int>>,decltype(comp)>que{comp};
  for(int i=0;i<n;i++)if(i!=root)que.emplace(c[i],d[i],i,1);
  UnionFind uf(n);
  while(!que.empty()){
    auto [cv,dv,v,sz]=que.top();que.pop();
    if(uf.size(v)!=sz)continue;
    int l1=l[v],r1=r[v];
    int p=uf.root(par[l1]);
    int l2=l[p],r2=r[p];
    nxt[r2]=l1;
    res+=c[p]*d[v];
    uf.merge(p,v);
    int x=uf.root(v);
    c[x]=c[v]+c[p];
    d[x]=d[v]+d[p];
    l[x]=l2;
    r[x]=r1;
    if(!uf.same(root,x))que.emplace(c[x],d[x],x,uf.size(x));
  }
  std::vector<int>ord(n);
  ord[0]=root;
  for(int i=1;i<n;i++)ord[i]=nxt[ord[i-1]];
  return std::make_pair(res,ord);
}