#define PROBLEM "https://judge.yosupo.jp/problem/static_rectangle_add_rectangle_sum"
#include "fastio.hpp"
#include "datastructure/static_rectangle_add_rectangle_sum.hpp"
#include "monoid/add.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<std::tuple<int,int,int,int,mint>>a(n);
  for(int i=0;i<n;i++){
    int lx,rx,ly,ry,v;
    rd(lx),rd(ly),rd(rx),rd(ry),rd(v);
    a[i]=std::make_tuple(lx,rx,ly,ry,mint::raw(v));
  }
  std::vector<std::tuple<int,int,int,int>>query(q);
  for(int i=0;i<q;i++){
    int lx,rx,ly,ry;
    rd(lx),rd(ly),rd(rx),rd(ry);
    query[i]=std::make_tuple(lx,rx,ly,ry);
  }
  auto ans=static_rectangle_add_rectangle_sum<int,MonoidAdd<mint>>(a,query);
  for(int i=0;i<q;i++)wt(ans[i].val()),wt('\n');
}