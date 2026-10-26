#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"
#include "datastructure/static_range_frequency.hpp"
#include "fastio.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<int>a(n);
  for(int&x:a)rd(x);
  std::vector<std::tuple<int,int,int>>query(q);
  for(auto&[l,r,x]:query)rd(l),rd(r),rd(x);
  for(int x:static_range_frequency(a,query))wt(x),wt('\n');
}