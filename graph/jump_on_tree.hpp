#pragma once
#include "level_ancestor.hpp"
#include "lca.hpp"
struct JumponTree{
  LowestCommonAncestor lca;
  LevelAncestor la;
  template<typename T>
  JumponTree(Tree<T>t){
    t.remove_parent();
    lca=LowestCommonAncestor(t);
    la=LevelAncestor(std::move(t));
  }
  int jump(int u,int v,int d)const{
    int l=lca.query(u,v);
    if(la.dep[u]+la.dep[v]-la.dep[l]*2<d)return -1;
    if(la.dep[u]-la.dep[l]>=d)return la.query(u,d);
    else return la.query(v,la.dep[u]+la.dep[v]-la.dep[l]*2-d);
  }
};