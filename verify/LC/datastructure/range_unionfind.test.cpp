#define PROBLEM "https://judge.yosupo.jp/problem/range_parallel_unionfind"
#include "fastio.hpp"
#include "datastructure/range_unionfind.hpp"
#include "datastructure/weighted_unionfind.hpp"
#include "math/modint.hpp"
#include "monoid/add.hpp"
using mint=mint998;
int main(){
  int n,q;
  rd(n),rd(q);
  RangeUnionfind uf1(n);
  WeightedUnionFind<MonoidAdd<mint>>uf2(n);
  for(int i=0;i<n;i++){
    int x;
    rd(x);
    uf2[i]+=mint::raw(x);
  }
  mint ans=0;
  while(q--){
    int k,a,b;
    rd(k),rd(a),rd(b);
    for(auto [u,v]:uf1.merge(a,b,k)){
      ans+=uf2[u]*uf2[v];
      uf2.merge(u,v);
    }
    wt(ans.val()),wt('\n');
  }
}