#pragma once
#include "datastructure/unionfind.hpp"
#include "tree_base.hpp"
template<typename T>
std::vector<int>offline_lca(const Tree<T>&t,const std::vector<std::pair<int,int>>&query,int root=0){
  int n=t.size();
  UnionFind uf(n);
  std::vector<int>st(n),mark(n),p(n),ret(query.size(),-1);
  int top=0;
  st[0]=root;
  for(const auto&[l,r]:query)mark[l]++,mark[r]++;
  std::vector<int>ptr;
  std::vector<pair<int,int>>q(query.size()*2);
  {
    vector<int>cnt(n+1);
    cnt[0]=0;
    for(int i=1;i<=n;i++)cnt[i]=cnt[i-1]+mark[i-1];
    ptr=cnt;
    for(int i=0;i<query.size();i++){
      q[cnt[query[i].first]++]=make_pair(query[i].second,i);
      q[cnt[query[i].second]++]=make_pair(query[i].first,i);
    }
  }
  for(int i=0;i<n;i++){
    p[i]=t[i].size();
    mark[i]=-1;
  }
  auto run=[&](int u)->bool {
    while(p[u]){
      int v=t[u][--p[u]].to;
      if(mark[v]==-1){
        st[++top]=v;
        return true;
      }
    }
    return false;
  };
  while(top!=-1){
    int u=st[top];
    if(mark[u]==-1)mark[u]=u;
    else{
      uf.merge(u,t[u][p[u]].to);
      mark[uf.root(u)]=u;
    }
    if(!run(u)){
      for(int i=ptr[u];i<ptr[u+1];i++){
        const auto&[v,j]=q[i];
        if(mark[v]!=-1&&ret[j]==-1)ret[j]=mark[uf.root(v)];
      }
      top--;
    }
  }
  return ret;
}