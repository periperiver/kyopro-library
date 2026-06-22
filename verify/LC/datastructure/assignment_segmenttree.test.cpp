#define PROBLEM "https://judge.yosupo.jp/problem/range_set_range_composite"
#include "fastio.hpp"
#include "datastructure/assignment_segmenttree.hpp"
#include "monoid/affine.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<MonoidAffine<mint>::S>init(n);
  for(int i=0;i<n;i++){
    int a,b;
    rd(a),rd(b);
    init[i]={mint::raw(a),mint::raw(b)};
  }
  AssignmentSegmentTree<MonoidAffine<mint>>seg(init);
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int l,r,b,c;
      rd(l),rd(r),rd(b),rd(c);
      seg.assign(l,r,{mint::raw(b),mint::raw(c)});
    }
    else{
      int l,r,x;
      rd(l),rd(r),rd(x);
      auto [a,b]=seg.prod(l,r);
      wt((a*mint::raw(x)+b).val()),wt('\n');
    }
  }
}