#define PROBLEM "https://judge.yosupo.jp/problem/rooted_tree_topological_order_with_minimum_inversions"
#include "fastio.hpp"
#include "graph/optimal_topological_sort_tree.hpp"
int main(){
  int n;
  rd(n);
  std::vector<int>p(n);
  p[0]=-1;
  for(int i=1;i<n;i++)rd(p[i]);
  std::vector<long long>c(n),d(n);
  for(int i=0;i<n;i++)rd(c[i]);
  for(int i=0;i<n;i++)rd(d[i]);
  auto [cost,ord]=optimal_topological_sort_tree(p,d,c);
  wt(cost),wt('\n');
  for(int i=0;i<n;i++)wt(ord[i]),wt(" \n"[i+1==n]);
}