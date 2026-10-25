#define PROBLEM "https://judge.yosupo.jp/problem/prefix_sum_of_binomial_coefficient"
#include "fastio.hpp"
#include "math/binomial_prefix_sum.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int t;
  rd(t);
  while(t--){
    int n,k;
    rd(n),rd(k);
    wt(BinomialPrefixSum<mint>::get(n,k).val()),wt('\n');
  }
}