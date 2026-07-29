#define PROBLEM "https://judge.yosupo.jp/problem/multipoint_evaluation"
#include "fastio.hpp"
#include "convolution/ntt_simd.hpp"
#include "poly/multipoint_evaluation.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  rd(n),rd(m);
  std::vector<mint>f(n),p(m);
  for(mint&x:f){
    int v;
    rd(v);
    x=mint::raw(v);
  }
  for(mint&x:p){
    int v;
    rd(v);
    x=mint::raw(v);
  }
  p=multipoint_evaluation(f,p);
  for(int i=0;i<m;i++)wt(p[i].val()),wt(" \n"[i+1==m]);
}