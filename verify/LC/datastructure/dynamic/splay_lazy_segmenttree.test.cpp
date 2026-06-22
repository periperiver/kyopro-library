#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum_large_array"
#include "fastio.hpp"
#include "datastructure/dynamic/splay_lazy_segmenttree.hpp"
#include "monoid/rangeaffine_rangesum.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,q;
  rd(n),rd(q);
  DynamicLazySegmentTree<int,RangeAffineRangeSum<mint>>seg;
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int l,r,b,c;
      rd(l),rd(r),rd(b),rd(c);
      seg.apply(l,r,{mint::raw(b),mint::raw(c)});
    }
    else{
      int l,r;
      rd(l),rd(r);
      wt(seg.prod(l,r).val()),wt('\n');
    }
  }
}