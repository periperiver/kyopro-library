#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_D"
#include<iostream>
#include "datastructure/dual_segmenttree.hpp"
#include "monoid/update.hpp"
int main(){
  int n,q;
  std::cin>>n>>q;
  DualSegmentTree<MonoidUpdate<int>>seg(n);
  while(q--){
    int t;
    std::cin>>t;
    if(t==0){
      int l,r,x;
      std::cin>>l>>r>>x;
      r++;
      seg.apply(l,r,x);
    }
    else{
      int i;
      std::cin>>i;
      auto ans=seg.get(i);
      if(ans.f)std::cout<<ans.val<<'\n';
      else std::cout<<std::numeric_limits<int>::max()<<'\n';
    }
  }
}