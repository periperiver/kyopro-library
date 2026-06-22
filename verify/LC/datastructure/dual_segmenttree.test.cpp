#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_point_get"
#include "fastio.hpp"
#include "datastructure/dual_segmenttree.hpp"
#include "monoid/rangeaffine_rangesum.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<mint>a(n);
  for(mint&x:a){
    int v;
    rd(v);
    x=mint::raw(v);
  }
  DualSegmentTree<RangeAffineRangeSum<mint>>seg(a);
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int l,r,b,c;
      rd(l),rd(r),rd(b),rd(c);
      seg.apply(l,r,{mint::raw(b),mint::raw(c)});
    }
    else{
      int i;
      rd(i);
      wt(seg.get(i).val()),wt('\n');
    }
  }
}