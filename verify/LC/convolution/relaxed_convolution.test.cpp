#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"
#include "convolution/relaxed_convolution.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<mint>a(n),b(m);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  RelaxedConvolution<mint>rc;
  for(int i=0;i<n+m-1;i++){
    mint A=i<n?a[i]:mint();
    mint B=i<m?b[i]:mint();
    std::cout<<rc.query(A,B)<<" \n"[i+1==n+m-1];
  }
}