#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb_128bit"
#include "fastio.hpp"
int main(){
  int t;
  rd(t);
  while(t--){
    __int128_t a,b;
    rd(a),rd(b);
    wt(a+b),wt('\n');
  }
}