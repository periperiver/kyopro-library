#pragma once
#include "compressed_tree.hpp"
template<typename T,typename Func>
void one_third_centroid_decomposition(const Tree<T>&t,const Func&f){
  std::vector<int>sub(t.size());
  auto centroid=[&](auto self,const CompressedTree<T>&g,int x,int p)->int {
    sub[x]=1;
    for(const auto&e:g[x])if(e.to!=p){
      int c=self(self,g,e.to,x);
      if(c!=-1)return c;
      sub[x]+=sub[e.to];
    }
    if(g.size()-sub[x]<=g.size()/2){
      if(p!=-1)sub[p]=g.size()-sub[x];
      return x;
    }
    return -1;
  };
  auto rec=[&](auto self,const CompressedTree<T>&g)->void {
    if(g.size()<=2)return;
    int c=centroid(centroid,g,0,-1);
    std::vector<int>is_left(g[c].size());
    int lsize=1,rsize=1;
    int sum=0;
    int idx=0;
    int border=(g.size()-1)*2/3;
    for(const auto&e:g[c]){
      if(sum+sub[e.to]<=border){
        is_left[idx]=1;
        lsize+=sub[e.to];
      }
      else{
        is_left[idx]=0;
        rsize+=sub[e.to];
      }
      sum+=sub[e.to];
      idx++;
    }
    std::vector<int>mapping_left{g.get(c)},mapping_right{g.get(c)};
    mapping_left.reserve(lsize),mapping_right.reserve(rsize);
    Tree<T>left(lsize),right(rsize);
    auto dfsl=[&](auto self,int x,int p,int pid,T v)->void {
      int nid=mapping_left.size();
      left.add_edge(nid,pid,v);
      mapping_left.push_back(g.get(x));
      for(const auto&e:g[x])if(e.to!=p)self(self,e.to,x,nid,e.weight);
    };
    auto dfsr=[&](auto self,int x,int p,int pid,T v)->void {
      int nid=mapping_right.size();
      right.add_edge(nid,pid,v);
      mapping_right.push_back(g.get(x));
      for(const auto&e:g[x])if(e.to!=p)self(self,e.to,x,nid,e.weight);
    };
    idx=0;
    for(const auto&e:g[c]){
      if(is_left[idx++])dfsl(dfsl,e.to,c,0,e.weight);
      else dfsr(dfsr,e.to,c,0,e.weight);
    }
    left.build(),right.build();
    CompressedTree<T>cleft(mapping_left,left),cright(mapping_right,right);
    self(self,cleft);
    self(self,cright);
    cleft.remove_parent(0),cright.remove_parent(0);
    f(std::move(cleft),std::move(cright));
  };
  rec(rec,t);
}