#define PROBLEM "https://judge.yosupo.jp/problem/bernoulli_number"
#include "fastio.hpp"
#include "math/bernoulli_number.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  rd(n);
  std::vector<mint>f=bernoulli_number<mint>(n);
  for(int i=0;i<=n;i++)wt(f[i].val()),wt(" \n"[i==n]);
}