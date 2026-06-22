#pragma once
#include<vector>
#include "datastructure/sparse_table.hpp"
#include "tree_base.hpp"
struct LowestCommonAncestor{
private:
  struct p_min{
    using S=std::pair<int,int>;
    static S op(const S&x,const S&y){return x.first<y.first?x:y;}
    static S e(){return std::make_pair(std::numeric_limits<int>::max(),-1);}
  };
  SparseTable<p_min>sp;
  std::vector<int>idx;
public:
  template<typename T>
  LowestCommonAncestor(const Tree<T>&t):idx(t.size()){
    assert(t.is_directed());
    int r=t.root();
    int ord=0;
    std::vector<int>dep(t.size());
    dep[r]=0;
    std::vector<std::pair<int,int>>init(t.size()*2-1);
    auto dfs=[&](auto&&self,int x)->void {
      idx[x]=ord;
      init[ord++]=std::make_pair(dep[x],x);
      for(const auto&e:t[x]){
        dep[e.to]=dep[e.from]+1;
        self(self,e.to);
        init[ord++]=std::make_pair(dep[x],x);
      }
    };
    dfs(dfs,r);
    sp=SparseTable<p_min>(init);
  }
  LowestCommonAncestor(){}
  int query(int u,int v)const{
    if(u==v)return u;
    if(idx[u]>idx[v])std::swap(u,v);
    return sp.prod(idx[u],idx[v]+1).second;
  }
};