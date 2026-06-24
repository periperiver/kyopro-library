#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"
#include "convolution/semi_relaxed_convolution.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n,m;
  std::cin>>n>>m;
  std::vector<mint>a(n),b(m);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a.resize(n+m-1);
  SemiRelaxedConvolution<mint>src(a);
  for(int i=0;i<n+m-1;i++){
    mint B=i<m?b[i]:mint();
    std::cout<<src.query(B)<<" \n"[i+1==n+m-1];
  }
}