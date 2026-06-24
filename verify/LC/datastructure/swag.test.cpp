#define PROBLEM "https://judge.yosupo.jp/problem/deque_operate_all_composite"
#include "fastio.hpp"
#include "datastructure/swag.hpp"
#include "monoid/affine.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int q;
  rd(q);
  SWAG<MonoidAffine<mint>>swag;
  while(q--){
    int t;
    rd(t);
    if(t<=1){
      int a,b;
      rd(a),rd(b);
      if(t==0)swag.push_front({mint::raw(a),mint::raw(b)});
      else swag.push_back({mint::raw(a),mint::raw(b)});
    }
    else if(t==2)swag.pop_front();
    else if(t==3)swag.pop_back();
    else{
      int x;
      rd(x);
      auto [a,b]=swag.all_prod();
      wt((a*mint::raw(x)+b).val()),wt('\n');
    }
  }
}