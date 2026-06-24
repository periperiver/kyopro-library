#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "fastio.hpp"
#include<cassert>
int main(){
  int a,b;
  rd(a),rd(b);
  assert(a+b<4);
  wt(a+b),wt('\n');
}