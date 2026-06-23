#define PROBLEM "https://judge.yosupo.jp/problem/range_linear_add_range_min"
#include "fastio.hpp"
#include "datastructure/range_linear_add_range_min.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<long long>a(n);
  for(long long&x:a)rd(x);
  RangeLinearAddRangeMin<long long>seg(std::move(a));
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int l,r,b,c;
      rd(l),rd(r),rd(b),rd(c);
      seg.add(l,r,b,c);
    }
    else{
      int l,r;
      rd(l),rd(r);
      wt(seg.min(l,r).second),wt('\n');
    }
  }
}