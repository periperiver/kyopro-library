#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"
#include "fastio.hpp"
#include "datastructure/static_range_min.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<int>a(n);
  for(int&x:a)rd(x);
  StaticRangeMin<int>sp(std::move(a));
  while(q--){
    int l,r;
    rd(l),rd(r);
    wt(sp.min(l,r)),wt('\n');
  }
}