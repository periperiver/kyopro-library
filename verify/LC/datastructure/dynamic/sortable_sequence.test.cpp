#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_sort_range_composite"
#include "fastio.hpp"
#include "datastructure/dynamic/sortable_sequence.hpp"
#include "monoid/affine.hpp"
#include "monoid/reverse.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,q;
  rd(n),rd(q);
  SortableSequence<int,ReversibleMonoid<MonoidAffine<mint>>>seg;
  for(int i=0;i<n;i++){
    int p,a,b;
    rd(p),rd(a),rd(b);
    seg.insert(i,p,{{mint::raw(a),mint::raw(b)},{mint::raw(a),mint::raw(b)}});
  }
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int i,p,a,b;
      rd(i),rd(p),rd(a),rd(b);
      seg.set(i,p,{{mint::raw(a),mint::raw(b)},{mint::raw(a),mint::raw(b)}});
    }
    else if(t==1){
      int l,r,x;
      rd(l),rd(r),rd(x);
      auto [a,b]=seg.prod(l,r).first;
      wt((a*mint::raw(x)+b).val()),wt('\n');
    }
    else{
      int l,r;
      rd(l),rd(r);
      seg.sort(l,r);
      if(t==3)seg.reverse(l,r);
    }
  }
}