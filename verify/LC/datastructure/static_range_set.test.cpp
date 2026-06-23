#define PROBLEM "https://judge.yosupo.jp/problem/static_range_count_distinct"
#include "fastio.hpp"
#include "datastructure/static_range_set.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<int>a(n);
  for(int&x:a)rd(x);
  StaticRangeSet s(std::move(a));
  while(q--){
    int l,r;
    rd(l),rd(r);
    wt(s.set(l,r)),wt('\n');
  }
}