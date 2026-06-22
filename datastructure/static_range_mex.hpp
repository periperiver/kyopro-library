#pragma once
#include<vector>
#include "persistent/segmenttree.hpp"
#include "monoid/min.hpp"
struct StaticRangeMex{
private:
  int n;
  std::vector<PersistentSegmentTree<int,MonoidMin<int>>>seg;
public:
  StaticRangeMex(const std::vector<int>&init):n(init.size()),seg(init.size()+1){
    seg[0]=PersistentSegmentTree<int,MonoidMin<int>>(std::vector<int>(n,-1));
    for(int i=0;i<n;i++){
      seg[i+1]=seg[i];
      if(init[i]<n){
        seg[i+1]=seg[i+1].set(init[i],i);
      }
    }
  }
  inline int mex(int l,int r)const{
    return seg[r].max_right([&](int x)->bool {return x>=l;});
  }
};