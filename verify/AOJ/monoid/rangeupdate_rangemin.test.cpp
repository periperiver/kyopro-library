#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_F"
#include<iostream>
#include "datastructure/lazy_segmenttree.hpp"
#include "monoid2/rangeupdate_rangemin.hpp"
int main(){
  int n,q;
  std::cin>>n>>q;
  LazySegmentTree<RangeUpdateRangeMin<int>>seg(n);
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
      int l,r;
      std::cin>>l>>r;
      r++;
      std::cout<<seg.prod(l,r)<<'\n';
    }
  }
}