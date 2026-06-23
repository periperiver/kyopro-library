#define PROBLEM "https://judge.yosupo.jp/problem/set_xor_min"
#include "fastio.hpp"
#include "datastructure/set_xor_min.hpp"
using bt=SetXorMin<unsigned,2000000,32>;
int main(){
  int q;
  rd(q);
  while(q--){
    int t,x;
    rd(t),rd(x);
    if(t==0)bt::add(x);
    else if(t==1)bt::erase(x);
    else wt(x^bt::query(x)),wt('\n');
  }
}