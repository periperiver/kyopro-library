#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"
#include "fastio.hpp"
#include "geo/dynamic_cht.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  DynamicConvexHullTrick<long long,__int128_t>cht;
  for(int i=0;i<n;i++){
    long long a,b;
    rd(a),rd(b);
    cht.add(a,b);
  }
  while(q--){
    int t;
    rd(t);
    if(t==0){
      long long a,b;
      rd(a),rd(b);
      cht.add(a,b);
    }
    else{
      long long x;
      rd(x);
      wt(cht.min(x)(x)),wt('\n');
    }
  }
}