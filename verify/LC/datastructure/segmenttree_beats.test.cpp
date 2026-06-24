#define PROBLEM "https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum"
#include "fastio.hpp"
#include "datastructure/segmenttree_beats.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<long long>a(n);
  for(long long&x:a)rd(x);
  SegmentTreeBeats<long long>seg(a);
  while(q--){
    int t,l,r;
    rd(t),rd(l),rd(r);
    if(t==3)wt(seg.sum(l,r)),wt('\n');
    else{
      long long b;
      rd(b);
      if(t==0)seg.chmin(l,r,b);
      else if(t==1)seg.chmax(l,r,b);
      else seg.add(l,r,b);
    }
  }
}