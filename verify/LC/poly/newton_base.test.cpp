#define PROBLEM "https://judge.yosupo.jp/problem/conversion_from_monomial_basis_to_newton_basis"
#include "poly/newton_base.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>a(n),p(n);
  for(mint&x:a)std::cin>>x;
  for(mint&x:p)std::cin>>x;
  a=newton_base(a,p);
  for(int i=0;i<n;i++)std::cout<<a[i]<<" \n"[i+1==n];
}