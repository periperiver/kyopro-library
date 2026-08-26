#define PROBLEM "https://yukicoder.me/problems/no/2513"
#include "convolution/ntt_simd.hpp"
#include "fastio.hpp"
#include "poly/vandermonde_det.hpp"
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
  std::reverse(f.begin(),f.end());
  wt(vandermonde_det(f).val()),wt('\n');
}