#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"
#include "fastio.hpp"
#include "datastructure/bit.hpp"
#include "monoid/add.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<long long>a(n);
  for(long long&x:a)rd(x);
  BinaryIndexedTree<MonoidAdd<long long>>BIT(a);
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int p,x;
      rd(p),rd(x);
      BIT.add(p,x);
    }
    else{
      int l,r;
      rd(l),rd(r);
      wt(BIT.sum(l,r)),wt('\n');
    }
  }
}