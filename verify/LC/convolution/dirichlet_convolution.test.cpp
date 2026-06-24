#define PROBLEM "https://judge.yosupo.jp/problem/dirichlet_convolution_and_prefix_sums"
#include "convolution/dirichlet_convolution.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int t;
  std::cin>>t;
  while(t--){
    long long n;
    std::cin>>n;
    Quotients quo(n);
    std::vector<mint>f(quo.size()),g(quo.size());
    for(mint&x:f)std::cin>>x;
    for(mint&x:g)std::cin>>x;
    f=dirichlet_convolution(f,g,quo);
    for(int i=0;i<quo.size();i++)std::cout<<f[i]<<" \n"[i+1==quo.size()];
  }
}