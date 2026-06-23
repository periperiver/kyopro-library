#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_taylor_shift"
#include "poly/polynomial_talor_shift.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  mint c;
  std::cin>>n>>c;
  std::vector<mint>f(n);
  for(mint&x:f)std::cin>>x;
  f=polynomial_talor_shift(f,c);
  for(int i=0;i<n;i++)std::cout<<f[i]<<" \n"[i+1==n];
}