#define PROBLEM "https://judge.yosupo.jp/problem/static_range_mode_query"
#include "fastio.hpp"
#include "datastructure/static_range_mode.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<int>a(n);
  for(int&x:a)rd(x);
  StaticRangeMode<int> s(std::move(a));
  while(q--){
    int l,r;
    rd(l),rd(r);
    auto [v,freq]=s.mode(l,r);
    wt(v),wt(' '),wt(freq),wt('\n');
  }
}