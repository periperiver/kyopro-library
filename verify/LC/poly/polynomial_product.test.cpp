#define PROBLEM "https://judge.yosupo.jp/problem/product_of_polynomial_sequence"
#include "poly/polynomial_product.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<std::vector<mint>>a(n);
  for(int i=0;i<n;i++){
    int d;
    std::cin>>d;
    a[i].resize(d+1);
    for(mint&x:a[i])std::cin>>x;
  }
  std::vector<mint>f=polynomial_product(a);
  for(int i=0;i<(int)f.size();i++)std::cout<<f[i]<<" \n"[i+1==(int)f.size()];
}