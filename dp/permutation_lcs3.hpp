#pragma once
#include "datastructure/segmenttree2d.hpp"
#include<cassert>
std::vector<int> permutation_lcs3(std::vector<int>a,std::vector<int>b,std::vector<int>c){
  assert(a.size()==b.size()&&b.size()==c.size());
  struct M{
    using S=std::pair<int,int>;
    static inline S op(const S&x,const S&y){return x.first<y.first?y:x;}
    static inline S e(){return std::make_pair(0,-1);}
  };
  int n=a.size();
  std::vector<int>pos(n);
  for(int i=0;i<n;i++)pos[a[i]]=i;
  for(int i=0;i<n;i++){
    b[i]=pos[b[i]];
    c[i]=pos[c[i]];
  }
  for(int i=0;i<n;i++)pos[c[i]]=i;
  std::vector<std::tuple<int,int,typename M::S>>init(n);
  for(int i=0;i<n;i++)init[i]=std::make_tuple(b[i],pos[b[i]],M::e());
  SegmentTree2d<int,M>seg(init);
  std::vector<int>pre(n);
  int ptr=0;
  for(const auto&[x,y,_]:init){
    std::pair<int,int>now=seg.prod(0,x,0,pos[x]);
    seg.set(x,pos[x],std::make_pair(now.first+1,ptr));
    pre[ptr++]=now.second;
  }
  std::pair<int,int>al=seg.all_prod();
  std::vector<int>res(al.first);
  int i=al.first-1,j=al.second;
  while(i>=0){
    res[i--]=a[b[j]];
    j=pre[j];
  }
  return res;
}