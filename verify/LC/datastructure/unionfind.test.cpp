#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"
#include "fastio.hpp"
#include "datastructure/unionfind.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  UnionFind uf(n);
  while(q--){
    int t,u,v;
    rd(t),rd(u),rd(v);
    if(t==0)uf.merge(u,v);
    else wt(uf.same(u,v)),wt('\n');
  }
}