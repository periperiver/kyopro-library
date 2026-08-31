#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_G"
#include<iostream>
#include "datastructure/lazy_segmenttree.hpp"
#include "monoid2/rangeadd_rangesum.hpp"
int main(){
  int n,q;
  std::cin>>n>>q;
  LazySegmentTree<RangeAddRangeSum<long long>>seg(n,std::make_pair(0,1));
  while(q--){
    int t;
    std::cin>>t;
    if(t==0){
      int l,r,x;
      std::cin>>l>>r>>x;
      l--;
      seg.apply(l,r,x);
    }
    else{
      int l,r;
      std::cin>>l>>r;
      l--;
      std::cout<<seg.prod(l,r).first<<'\n';
    }
  }
}