#pragma once
#include<vector>
#include<numeric>
#include<queue>
#include<cassert>
#include "tree_base.hpp"
#include "datastructure/unionfind.hpp"
template<typename M,typename T>
std::pair<typename M::S,std::vector<int>>optimal_topological_sort_tree(const Tree<T>&t,std::vector<typename M::S>dat){
  assert(t.is_directed());
  using S=typename M::S;
  int n=t.size();
  int root=t.root();
  UnionFind uf(n);
  using Tp=std::tuple<S,int,int>;
  auto comp=[](const Tp&lhs,const Tp&rhs)->bool {
    const S&l=std::get<0>(lhs);
    const S&r=std::get<0>(rhs);
    return M::compare(r,l);
  };
  std::priority_queue<Tp,std::vector<Tp>,decltype(comp)>que(comp);
  for(int i=0;i<n;i++)if(i!=root)que.emplace(dat[i],i,1);
  std::vector<int>l(n),r(n),pr(n);
  std::iota(l.begin(),l.end(),0);
  std::iota(r.begin(),r.end(),0);
  while(!que.empty()){
    auto [v,x,sz]=que.top();que.pop();
    if(uf.size(x)!=sz)continue;
    int l1=l[x],r1=r[x];
    int p=uf.root(t.parent(l1));
    int l2=l[uf.root(p)],r2=r[uf.root(p)];
    S nxt=M::op(dat[p],dat[x]);
    uf.merge(p,x);
    x=uf.root(x);
    dat[x]=nxt;
    l[x]=l2;
    r[x]=r1;
    pr[r2]=l1;
    if(!uf.same(root,x))que.emplace(dat[x],x,uf.size(x));
  }
  std::vector<int>ord(n);
  ord[0]=root;
  for(int i=1;i<n;i++)ord[i]=pr[ord[i-1]];
  return std::make_pair(dat[uf.root(0)],ord);
}