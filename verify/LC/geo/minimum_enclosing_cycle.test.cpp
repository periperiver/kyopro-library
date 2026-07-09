#define PROBLEM "https://judge.yosupo.jp/problem/minimum_enclosing_circle"
#include "fastio.hpp"
#include "geo/minimum_enclosing_cycle.hpp"
int main(){
  int n;
  rd(n);
  std::vector<Point<int>>a(n);
  for(int i=0;i<n;i++){
    int x,y;
    rd(x),rd(y);
    a[i]={x,y};
  }
  auto ans=minimum_enclosing_cycle<int,long long>(a);
  for(int i=0;i<n;i++)wt((int)ans[i]);
  wt('\n');
}