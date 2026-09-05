#define PROBLEM "https://judge.yosupo.jp/problem/prefix_sum_of_polynomial"
#include "fastio.hpp"
#include "poly/prefix_sum_of_polynomial.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  rd(n);
  std::vector<mint>f(n);
  for(mint&x:f){
    int v;
    rd(v);
    x=mint::raw(v);
  }
  std::vector<mint>ans=prefix_sum_of_polynomial(f);
  for(int i=0;i<=n;i++){
    if(i!=n)ans[i]-=f[i];
    wt(ans[i].val()),wt(" \n"[i==n]);
  }
}