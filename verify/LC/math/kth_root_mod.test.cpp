#define PROBLEM "https://judge.yosupo.jp/problem/kth_root_mod"
#include "fastio.hpp"
#include "math/kth_root_mod.hpp"
int main(){
  int t;
  rd(t);
  while(t--){
    int k,y,p;
    rd(k),rd(y),rd(p);
    wt(kth_root_mod(y,k,p)),wt('\n');
  }
}