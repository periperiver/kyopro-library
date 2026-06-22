#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite_large_array"
#include "fastio.hpp"
#include "datastructure/dynamic/splay_segmentree.hpp"
#include "monoid/affine.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,q;
  rd(n),rd(q);
  DynamicSegmentTree<int,MonoidAffine<mint>>seg;
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int p,c,d;
      rd(p),rd(c),rd(d);
      seg.set(p,{mint::raw(c),mint::raw(d)});
    }
    else{
      int l,r,x;
      rd(l),rd(r),rd(x);
      auto [a,b]=seg.prod(l,r);
      wt((a*mint::raw(x)+b).val()),wt('\n');
    }
  }
}